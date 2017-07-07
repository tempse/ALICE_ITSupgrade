#include <iostream>
#include <vector>
#include <algorithm>

#include <TROOT.h>
#include <TApplication.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TStopwatch.h>



// stores all relevant information of a particle pair:
struct particlePair {
  Long64_t EventID;
  Long64_t TrackID1;
  Long64_t TrackID2;
  Int_t    IsTaggedAccepted_prefilter;
  Int_t    IsTaggedAccepted_noPrefilter;
  Int_t    IsTrueConv;
  Int_t    IsTrueRP;
  Float_t  pt1;
  Float_t  pt2;
  Long64_t originalPosition;
};

bool sortPairsByEventID(const particlePair &lhs, const particlePair &rhs) {
  return lhs.EventID < rhs.EventID;
}

bool sortByOriginalPosition(const particlePair &lhs, const particlePair &rhs) {
  return lhs.originalPosition < rhs.originalPosition;
}

// store all relevant information of a particle track:
struct particleTrack {
  Long64_t EventID;
  Long64_t TrackID;
};

bool sortTracksByEventID(const particleTrack &lhs, const particleTrack &rhs) {
  return lhs.EventID < rhs.EventID;
}


Float_t getPairPIDefficiency(Float_t, Float_t, TH1D&);



void createROCdata_MVAplusPrefilter(TString MCdatafilename,
				    TString treename_MCdatafile,
				    TString MVAoutput_prefilter_filename,
				    TString treename_MVAoutput_prefilter_file,
				    TString branchname_MVAoutput_prefilter_file,
				    TString MVAoutput_noPrefilter_filename,
				    TString treename_MVAoutput_noPrefilter_file,
				    TString branchname_MVAoutput_noPrefilter_file,
				    TString signalRegion = "+",
				    Int_t   num_steps = 10, // number of steps in the MVA cut scan
				    Int_t   stride_combi = 1, // stride when combining two classifiers
				    Float_t MVAoutputRange_min = 0.,
				    Float_t MVAoutputRange_max = 1.,
				    TString outfilename = "temp_output/ROCdata_MVAplusPrefilter",
				    TString PIDeffs_filename = "~/analysis/data/FT2_AnalysisResults_Upgrade/inputData/ITSU_PIDefficiency_lowB.root") {

  Float_t MVAout_prefilter, MVAout_noPrefilter;
  Int_t isAccepted;
  
  
  std::cout << "Reading files...";
  TFile *MCdatafile = new TFile(MCdatafilename, "READ");
  TTree *tree_MCdatafile = (TTree*)MCdatafile->Get(treename_MCdatafile);
  Int_t EventID, TrackID1, TrackID2;
  Int_t IsConv, IsRP;
  Float_t pt1, pt2;
  tree_MCdatafile->SetBranchAddress("EventID1", &EventID);
  tree_MCdatafile->SetBranchAddress("TrackID1", &TrackID1);
  tree_MCdatafile->SetBranchAddress("TrackID2", &TrackID2);
  tree_MCdatafile->SetBranchAddress("IsConv", &IsConv);
  tree_MCdatafile->SetBranchAddress("IsRP", &IsRP);
  tree_MCdatafile->SetBranchAddress("pt1", &pt1);
  tree_MCdatafile->SetBranchAddress("pt2", &pt2);
  std::cout << " DONE." << std::endl;


  TFile *MVAoutput_prefilter_file = new TFile(MVAoutput_prefilter_filename, "READ");
  TTree *tree_MVAoutput_prefilter_file = (TTree*)MVAoutput_prefilter_file->Get(treename_MVAoutput_prefilter_file);
  tree_MVAoutput_prefilter_file->SetBranchAddress(branchname_MVAoutput_prefilter_file,
						  &MVAout_prefilter);
  

  if(tree_MCdatafile->GetEntries() != tree_MVAoutput_prefilter_file->GetEntries()) {
    std::cout << "  ERROR: Trees have different sizes." << std::endl
	      << "    Size of tree " << treename_MCdatafile << " in "
	      << MCdatafilename << ": " << tree_MCdatafile->GetEntries()
	      << std::endl
	      << "    Size of tree " << treename_MVAoutput_prefilter_file << " in "
	      << MVAoutput_prefilter_filename << ": " << tree_MVAoutput_prefilter_file->GetEntries()
	      << std::endl << std::endl;
    gApplication->Terminate();
  }


  TFile *MVAoutput_noPrefilter_file = new TFile(MVAoutput_noPrefilter_filename, "READ");
  TTree *tree_MVAoutput_noPrefilter_file = (TTree*)MVAoutput_noPrefilter_file->Get(treename_MVAoutput_noPrefilter_file);
  tree_MVAoutput_noPrefilter_file->SetBranchAddress(branchname_MVAoutput_noPrefilter_file,
						    &MVAout_noPrefilter);

  
  if(tree_MCdatafile->GetEntries() != tree_MVAoutput_noPrefilter_file->GetEntries()) {
    std::cout << "  ERROR: Trees have different sizes." << std::endl
	      << "    Size of tree " << tree_MCdatafile->GetEntries() << " in "
	      << MCdatafilename << ": " << tree_MCdatafile->GetEntries()
	      << std::endl
	      << "    Size of tree " << tree_MVAoutput_noPrefilter_file << " in "
	      << MVAoutput_noPrefilter_filename << ": " << tree_MVAoutput_noPrefilter_file->GetEntries()
	      << std::endl << std::endl;
    gApplication->Terminate();
  }


  if(!outfilename.EndsWith(".root")) {
    outfilename.Append(".root");
  }
  

  TFile *outfile = new TFile(outfilename, "RECREATE");
  TString outtree_name = "ROCdata";
  TTree *outtree = new TTree(outtree_name, outtree_name);
  Float_t tpr_prefilter, fpr_prefilter;
  Float_t tpr_noPrefilter, fpr_noPrefilter;
  Float_t currentMVAcut;
  // Long64_t nTaggedAccepted_prefilter;
  // Long64_t nTaggedAccepted_noPrefilter;
  // Long64_t nIsConv, nIsRP;
  std::vector<Int_t> pairs_IsTaggedAccepted_prefilter;
  std::vector<Int_t> pairs_IsTaggedAccepted_noPrefilter;
  std::vector<Int_t> pairs_IsTrueConv;
  std::vector<Double_t> pairs_pairweight;
  outtree->Branch("pairs_IsTaggedAccepted_prefilter", &pairs_IsTaggedAccepted_prefilter);
  outtree->Branch("pairs_IsTaggedAccepted_noPrefilter", &pairs_IsTaggedAccepted_noPrefilter);
  outtree->Branch("pairs_IsTrueConv", &pairs_IsTrueConv);
  outtree->Branch("pairs_pairweight", &pairs_pairweight);
  outtree->Branch("tpr_prefilter", &tpr_prefilter, "tpr_prefilter/F");
  outtree->Branch("fpr_prefilter", &fpr_prefilter, "fpr_prefilter/F");
  outtree->Branch("tpr_noPrefilter", &tpr_noPrefilter, "tpr_noPrefilter/F");
  outtree->Branch("fpr_noPrefilter", &fpr_noPrefilter, "fpr_noPrefilter/F");
  outtree->Branch("MVAcut", &currentMVAcut, "MVAcut/F");
  // outtree->Branch("nTaggedAccepted_prefilter", &nTaggedAccepted_prefilter, "nTaggedAccepted_prefilter/I");
  // outtree->Branch("nTaggedAccepted_noPrefilter", &nTaggedAccepted_noPrefilter, "nTaggedAccepted_noPrefilter/I");
  // outtree->Branch("nIsConv", &nIsConv, "nIsConv/I");
  // outtree->Branch("nIsRP", &nIsRP, "nIsRP/I");
  

  
  TFile *infile_PIDeffs = new TFile(PIDeffs_filename, "READ");
  TH1D *h_PIDeffs = (TH1D*)infile_PIDeffs->Get("efficiencyLHC17d12_TPCandTOF3sigma");
  h_PIDeffs->GetXaxis()->SetRangeUser(0,5);

  
  
  const Long64_t nentries = tree_MCdatafile->GetEntries()/10;


  TStopwatch *watch_overall = new TStopwatch();
  watch_overall->Start();

  
  for(Int_t scan=0; scan<=num_steps; scan++) {

    TStopwatch *watch_step = new TStopwatch();
    watch_step->Start();

    
    pairs_IsTaggedAccepted_prefilter.clear();
    pairs_IsTaggedAccepted_noPrefilter.clear();
    pairs_IsTrueConv.clear();
    pairs_pairweight.clear();
    
    
    // Collection of all particle pairs (of all events):
    std::vector<particlePair> allPairs;

    // Collection of all accepted particle tracks (of all events):
    std::vector<particleTrack> tracksTaggedAccepted;

    
    Float_t MVAcut = scan/(num_steps*1.0)*(MVAoutputRange_max-MVAoutputRange_min) + MVAoutputRange_min;


    std::cout << std::endl << "---------- Step " << scan << " of " << num_steps
	      << " (MVA cut: " << MVAcut << "): ----------" << std::endl;

    
    std::cout << "Tagging pairs based on MVA output..." << std::endl;
  
    for(Long64_t j=0; j<nentries; j++) {
      if((j%10000)==0) {
	std::cout << "\r  Processing event " << j << " of " << nentries
		  << " (" << j/((Float_t)nentries)*100 << " %)...";
      }


      tree_MCdatafile->GetEntry(j);
      tree_MVAoutput_prefilter_file->GetEntry(j);
      tree_MVAoutput_noPrefilter_file->GetEntry(j);
    
      particlePair currentPair;
      currentPair.EventID = EventID;
      currentPair.TrackID1 = TrackID1;
      currentPair.TrackID2 = TrackID2;
      currentPair.IsTrueConv = IsConv;
      currentPair.IsTrueRP = IsRP;
      currentPair.pt1 = pt1;
      currentPair.pt2 = pt2;
      currentPair.originalPosition = j;
    
      if(signalRegion == "+") {

	if(MVAout_prefilter < MVAoutputRange_min || MVAout_prefilter > MVAoutputRange_max) { // check if previously tagged as non-accepted
	  currentPair.IsTaggedAccepted_prefilter = -999;
	}else if(MVAout_prefilter > MVAcut) {
	  currentPair.IsTaggedAccepted_prefilter = 1;

	  particleTrack currentTrack1 = {EventID, TrackID1};
	  particleTrack currentTrack2 = {EventID, TrackID2};
	  tracksTaggedAccepted.push_back(currentTrack1);
	  tracksTaggedAccepted.push_back(currentTrack2);
	}else currentPair.IsTaggedAccepted_prefilter = 0;

	if(MVAout_noPrefilter < MVAoutputRange_min || MVAout_noPrefilter > MVAoutputRange_max) { // check if previously tagged as non-accepted
	  currentPair.IsTaggedAccepted_noPrefilter = -999;
	}else if(MVAout_noPrefilter > MVAcut) {
	  currentPair.IsTaggedAccepted_noPrefilter = 1;
	}else currentPair.IsTaggedAccepted_noPrefilter = 0;
	
      }else if(signalRegion == "-") {

	if(MVAout_prefilter < MVAoutputRange_min || MVAout_prefilter > MVAoutputRange_max) { // check if previously tagged as non-accepted
	  currentPair.IsTaggedAccepted_prefilter = -999;
	}else if(MVAout_prefilter < MVAcut) {
	  currentPair.IsTaggedAccepted_prefilter = 1;

	  particleTrack currentTrack1 = {EventID, TrackID1};
	  particleTrack currentTrack2 = {EventID, TrackID2};
	  tracksTaggedAccepted.push_back(currentTrack1);
	  tracksTaggedAccepted.push_back(currentTrack2);
	}else currentPair.IsTaggedAccepted_prefilter = 0;

	if(MVAout_noPrefilter < MVAoutputRange_min || MVAout_noPrefilter > MVAoutputRange_max) { // check if previously tagged as non-accepted
	  currentPair.IsTaggedAccepted_noPrefilter = -999;
	}else if(MVAout_noPrefilter < MVAcut) {
	  currentPair.IsTaggedAccepted_noPrefilter = 1;
	}else currentPair.IsTaggedAccepted_noPrefilter = 0;
	
      }else {
	std::cout << "  ERROR: 'signalRegion' definition is wrong. "
		  << "(It should be either '+' (default) or '-'.)" << std::endl;
	std::cout << "  Abort." << std::endl << std::endl;
	gApplication->Terminate();
      }

      allPairs.push_back(currentPair);
    }
    std::cout << "\r  Processing event " << nentries << " of " << nentries
	      << " (100 %)... DONE" << std::endl;

    std::cout << "Number of pairs (prefilter): " << allPairs.size() << std::endl;
    std::cout << "Number of accepted tracks (prefilter): " << tracksTaggedAccepted.size() << std::endl;
    std::cout << std::endl;


    // sort pairs and tracks by event ID:
    std::cout << "Sorting data...";
    std::sort(allPairs.begin(), allPairs.end(), sortPairsByEventID);
    std::sort(tracksTaggedAccepted.begin(), tracksTaggedAccepted.end(), sortTracksByEventID);
    std::cout << " DONE." << std::endl;
  

    // store start positions of new events:
    std::cout << "Finding new event start positions...";
    std::map<Long64_t, Long64_t> eventID_startPos;
    Long64_t EventID_prev = -1;
    for(Long64_t i=0; i<(Long64_t)tracksTaggedAccepted.size(); i++) {
      if(tracksTaggedAccepted[i].EventID != EventID_prev) {
	eventID_startPos[tracksTaggedAccepted[i].EventID] = i;
      }
      EventID_prev = tracksTaggedAccepted[i].EventID;
    }
    std::cout << " DONE." << std::endl;
  


    std::cout << "Propagate tag information to other pairs..." << std::endl;
  

    for(Long64_t i=0; i<nentries; i++) {
      if((i%10000)==0) {
	std::cout << "\r  Processing event " << i << " of " << nentries
		  << " (" << i/((Float_t)nentries)*100 << " %)...";
      }
    
      if( allPairs[i].IsTaggedAccepted_prefilter == -999 ) continue; // skip irrelevant entries
    
      for(Long64_t j=eventID_startPos[allPairs[i].EventID];
	  j<(Long64_t)tracksTaggedAccepted.size();
	  j++) {
	if( allPairs[i].EventID != tracksTaggedAccepted[j].EventID ) {
	  break;
	}else if(allPairs[i].TrackID1==tracksTaggedAccepted[j].TrackID ||
		 allPairs[i].TrackID2==tracksTaggedAccepted[j].TrackID) {
	  allPairs[i].IsTaggedAccepted_prefilter = 1.;
	}
      }
    }
    std::cout << "\r  Processing event " << nentries << " of " << nentries
	      << " (100 %)... DONE." << std::endl;


    // restore original order of allPairs vector:
    std::cout << "Restoring original order of data...";
    std::sort(allPairs.begin(), allPairs.end(), sortByOriginalPosition);
    std::cout << " DONE." << std::endl;


    
    std::cout << "Calculating output values...";    
    
    // nTaggedAccepted_prefilter = 0;
    // nTaggedAccepted_noPrefilter = 0;
    // nIsConv = 0;
    // nIsRP = 0;
    Double_t tp_noPrefilter=0, fp_noPrefilter=0, fn_noPrefilter=0, tn_noPrefilter=0;
    Double_t tp_prefilter=0, fp_prefilter=0, fn_prefilter=0, tn_prefilter=0;
    
    for(Long64_t i=0; i<nentries; i++) {
      if(allPairs[i].IsTaggedAccepted_prefilter!=0 && allPairs[i].IsTaggedAccepted_prefilter!=1)
	continue;

      // if(allPairs[i].IsTrueConv) nIsConv++;
      // if(allPairs[i].IsTrueRP) nIsRP++;


      // if(allPairs[i].IsTaggedAccepted_noPrefilter) nTaggedAccepted_noPrefilter++;

      Double_t pairweight = getPairPIDefficiency(allPairs[i].pt1, allPairs[i].pt2, *h_PIDeffs);
      
      if(allPairs[i].IsTaggedAccepted_noPrefilter && allPairs[i].IsTrueConv) tn_noPrefilter += pairweight;
      if(allPairs[i].IsTaggedAccepted_noPrefilter && !allPairs[i].IsTrueConv) fn_noPrefilter += pairweight;
      if(!allPairs[i].IsTaggedAccepted_noPrefilter && allPairs[i].IsTrueConv) fp_noPrefilter += pairweight;
      if(!allPairs[i].IsTaggedAccepted_noPrefilter && !allPairs[i].IsTrueConv) tp_noPrefilter += pairweight;

      
      // if(allPairs[i].IsTaggedAccepted_prefilter) nTaggedAccepted_prefilter++;
      
      if(allPairs[i].IsTaggedAccepted_prefilter && allPairs[i].IsTrueConv) tn_prefilter += pairweight;
      if(allPairs[i].IsTaggedAccepted_prefilter && !allPairs[i].IsTrueConv) fn_prefilter += pairweight;
      if(!allPairs[i].IsTaggedAccepted_prefilter && allPairs[i].IsTrueConv) fp_prefilter += pairweight;
      if(!allPairs[i].IsTaggedAccepted_prefilter && !allPairs[i].IsTrueConv) tp_prefilter += pairweight;


      pairs_IsTaggedAccepted_prefilter.push_back(allPairs[i].IsTaggedAccepted_prefilter);
      pairs_IsTaggedAccepted_noPrefilter.push_back(allPairs[i].IsTaggedAccepted_noPrefilter);
      pairs_IsTrueConv.push_back(allPairs[i].IsTrueConv);
      pairs_pairweight.push_back(pairweight);
      
    }
    
    tpr_prefilter = tp_prefilter/((Float_t)tp_prefilter+fn_prefilter);
    fpr_prefilter = fp_prefilter/((Float_t)fp_prefilter+tn_prefilter);

    tpr_noPrefilter = tp_noPrefilter/((Float_t)tp_noPrefilter+fn_noPrefilter);
    fpr_noPrefilter = fp_noPrefilter/((Float_t)fp_noPrefilter+tn_noPrefilter);
    
    
    currentMVAcut = MVAcut;
    
    std::cout << " DONE" << std::endl;
    std::cout << "  (TPR (prefilter) = " << tpr_prefilter << ",\tFPR (prefilter) = " << fpr_prefilter
	      << ")" << std::endl;
    std::cout << "  (TPR (no prefilter) = " << tpr_noPrefilter << ",\tFPR (no prefilter) = " << fpr_noPrefilter
	      << ")" << std::endl;
    outtree->Fill();


    std::cout << std::endl << "Time elapsed: " << watch_step->RealTime()
	      << " seconds." << std::endl;
  }

  
  std::cout << "Write data...";
  outfile->cd();
  outfile->Write("", TObject::kOverwrite);
  std::cout << " DONE." << std::endl;
  std::cout << "File " << outfile->GetName() << " created." << std::endl;



  MCdatafile->Close();
  MVAoutput_prefilter_file->Close();
  MVAoutput_noPrefilter_file->Close();


  
  //////////////////////////////////////////////////////////////////////////////

  
  
  std::cout << std::endl << "Combining classifiers..." << std::endl;

  std::vector<Int_t> *pairs_IsTaggedAccepted_prefilter_combi = 0;
  std::vector<Int_t> *pairs_IsTaggedAccepted_noPrefilter_combi = 0;
  std::vector<Int_t> *pairs_IsTrueConv_combi = 0;
  std::vector<Double_t> *pairs_pairweight_combi = 0;
  
  
  TFile *infile_ROCdata = new TFile(outfilename, "READ");
  TTree *tree_infile_ROCdata;
  infile_ROCdata->GetObject(outtree_name, tree_infile_ROCdata);

  TBranch *branch_IsTaggedAccepted_prefilter_combi = 0;
  TBranch *branch_IsTaggedAccepted_noPrefilter_combi = 0;
  TBranch *branch_IsTrueConv_combi = 0;
  TBranch *branch_pairweight_combi = 0;
  
  tree_infile_ROCdata->SetBranchAddress("pairs_IsTaggedAccepted_prefilter",
					&pairs_IsTaggedAccepted_prefilter_combi,
					&branch_IsTaggedAccepted_prefilter_combi);
  tree_infile_ROCdata->SetBranchAddress("pairs_IsTaggedAccepted_noPrefilter",
					&pairs_IsTaggedAccepted_noPrefilter_combi,
					&branch_IsTaggedAccepted_noPrefilter_combi);
  tree_infile_ROCdata->SetBranchAddress("pairs_IsTrueConv",
					&pairs_IsTrueConv_combi,
					&branch_IsTrueConv_combi);
  tree_infile_ROCdata->SetBranchAddress("pairs_pairweight",
					&pairs_pairweight_combi,
					&branch_pairweight_combi);


  
  outfilename.ReplaceAll(".root","_combined.root");
  outtree_name += "_combi";
  
  TFile *outfile_combi = new TFile(outfilename, "RECREATE");
  TTree *outtree_combi = new TTree(outtree_name, outtree_name);

  Float_t tpr_combi, fpr_combi;
  outtree_combi->Branch("tpr_combi", &tpr_combi, "tpr_combi/F");
  outtree_combi->Branch("fpr_combi", &fpr_combi, "fpr_combi/F");


  TStopwatch *watch_combi = new TStopwatch();
  watch_combi->Start();
  
  
  for(Int_t i=0; i<num_steps; i+=stride_combi) {
    std::cout << "\r  Processing entry " << i << " of " << num_steps << "...";
    
    tree_infile_ROCdata->GetEntry(i);

    std::vector<Int_t> *pairs_prefilter_compare = 0;
    pairs_prefilter_compare = pairs_IsTaggedAccepted_prefilter_combi;
    
    for(Int_t j=0; j<num_steps; j+=stride_combi) {
      tree_infile_ROCdata->GetEntry(j);

      Double_t tp_combi = 0;
      Double_t fp_combi = 0;
      Double_t fn_combi = 0;
      Double_t tn_combi = 0;
      
      for(Long64_t k=0; k<(Long64_t)pairs_IsTaggedAccepted_noPrefilter.size(); k++) {
	if( !(pairs_prefilter_compare->at(k)==1 || pairs_IsTaggedAccepted_noPrefilter_combi->at(k)==1) && pairs_IsTrueConv_combi->at(k)!=1 ) tp_combi += pairs_pairweight_combi->at(k);
	if( (pairs_prefilter_compare->at(k)==1 || pairs_IsTaggedAccepted_noPrefilter_combi->at(k)==1) && pairs_IsTrueConv_combi->at(k)!=1 ) fn_combi += pairs_pairweight_combi->at(k);
	if( !(pairs_prefilter_compare->at(k)==1 || pairs_IsTaggedAccepted_noPrefilter_combi->at(k)==1) && pairs_IsTrueConv_combi->at(k)==1 ) fp_combi += pairs_pairweight_combi->at(k);
	if( (pairs_prefilter_compare->at(k)==1 || pairs_IsTaggedAccepted_noPrefilter_combi->at(k)==1) && pairs_IsTrueConv_combi->at(k)==1 ) tn_combi += pairs_pairweight_combi->at(k);
      }
      
      tpr_combi = tp_combi/(tp_combi+fn_combi*1.0);
      fpr_combi = fp_combi/(fp_combi+tn_combi*1.0);

      outtree_combi->Fill();
      
    }
    
  }
  std::cout << std::endl;

  std::cout << "Time elapsed: " << watch_combi->RealTime() << " seconds." << std::endl;

  
  outtree_combi->Write("", TObject::kOverwrite);

  outfile_combi->Close();
  tree_infile_ROCdata->ResetBranchAddresses();


  
  std::cout << std::endl << "All done." << std::endl
	    << "Overall time elapsed: " << watch_overall->RealTime() << " seconds."
	    << std::endl << std::endl;
  
  gApplication->Terminate();
  
}



Float_t getPairPIDefficiency(Float_t pt1, Float_t pt2, TH1D &h_PIDeff) {

  Float_t PIDeff1 = (pt1 >= h_PIDeff.GetBinLowEdge(h_PIDeff.GetNbinsX())) ?
    h_PIDeff.GetBinContent(h_PIDeff.GetNbinsX()) : h_PIDeff.GetBinContent(h_PIDeff.FindBin(pt1));

  Float_t PIDeff2 = (pt2 >= h_PIDeff.GetBinLowEdge(h_PIDeff.GetNbinsX())) ?
    h_PIDeff.GetBinContent(h_PIDeff.GetNbinsX()) : h_PIDeff.GetBinContent(h_PIDeff.FindBin(pt2));

  return PIDeff1 * PIDeff2;
  
}
