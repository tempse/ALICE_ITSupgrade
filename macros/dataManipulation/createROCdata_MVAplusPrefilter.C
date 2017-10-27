#include <iostream>
#include <vector>
#include <algorithm>

#include <TROOT.h>
#include <TApplication.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TRandom.h>
#include <TStopwatch.h>


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
                                    Float_t massCut = 0.05, // do not count events with mass < massCut
				    Float_t MVAoutputRange_min = 0.,
				    Float_t MVAoutputRange_max = 1.,
				    TString outfilename = "temp_output/ROCdata_MVAplusPrefilter",
				    TString PIDeffs_filename = "~/analysis/data/FT2_AnalysisResults_Upgrade/inputData/ITSU_PIDefficiency_lowB.root") {

  Float_t MVAout_prefilter, MVAout_noPrefilter;
  Int_t isAccepted;
  
  
  std::cout << "Opening files...";
  TFile *MCdatafile = new TFile(MCdatafilename, "READ");
  TTree *tree_MCdatafile = (TTree*)MCdatafile->Get(treename_MCdatafile);
  Int_t EventID, TrackID1, TrackID2;
  Int_t IsConv, IsRP;
  Float_t pt1, pt2;
  Float_t mass;
  Bool_t containsTrackCutInfo = kTRUE;
  Int_t TrackCut1, TrackCut2;
  tree_MCdatafile->SetBranchAddress("EventID1", &EventID);
  tree_MCdatafile->SetBranchAddress("TrackID1", &TrackID1);
  tree_MCdatafile->SetBranchAddress("TrackID2", &TrackID2);
  tree_MCdatafile->SetBranchAddress("IsConv", &IsConv);
  tree_MCdatafile->SetBranchAddress("IsRP", &IsRP);
  tree_MCdatafile->SetBranchAddress("pt1", &pt1);
  tree_MCdatafile->SetBranchAddress("pt2", &pt2);
  tree_MCdatafile->SetBranchAddress("mass", &mass);
  if(tree_MCdatafile->GetListOfBranches()->FindObject("TrackCut1") != NULL &&
     tree_MCdatafile->GetListOfBranches()->FindObject("TrackCut2") != NULL) {
    tree_MCdatafile->SetBranchAddress("TrackCut1", &TrackCut1);
    tree_MCdatafile->SetBranchAddress("TrackCut2", &TrackCut2);
  }else {
    std::cout << "  Warning: No branch holding track cut information found. "
	      << "All tracks will be processed." << std::endl;
    containsTrackCutInfo = kFALSE;
  }
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
  

  
  TFile *infile_PIDeffs = new TFile(PIDeffs_filename, "READ");
  TH1D *h_PIDeffs = (TH1D*)infile_PIDeffs->Get("efficiencyLHC17d12_TPCandTOF3sigma");
  h_PIDeffs->GetXaxis()->SetRangeUser(0,5);

  
  
  const Long64_t nentries = 50000000; //tree_MCdatafile->GetEntries();
  std::cout << "Number of entries: " << nentries << std::endl;

  
  Int_t *EventID_all = NULL;
  Int_t *TrackID1_all = NULL;
  Int_t *TrackID2_all = NULL;
  Int_t *IsConv_all = NULL;
  Int_t *IsRP_all = NULL;
  Float_t *pt1_all = NULL;
  Float_t *pt2_all = NULL;
  Float_t *mass_all = NULL;
  Int_t *trackCut1_all = NULL;
  Int_t *trackCut2_all = NULL;
  Float_t *MVAout_prefilter_all = NULL;
  Float_t *MVAout_noPrefilter_all = NULL;

  EventID_all = new Int_t[nentries];
  TrackID1_all = new Int_t[nentries];
  TrackID2_all = new Int_t[nentries];
  IsConv_all = new Int_t[nentries];
  IsRP_all = new Int_t[nentries];
  pt1_all = new Float_t[nentries];
  pt2_all = new Float_t[nentries];
  mass_all = new Float_t[nentries];
  trackCut1_all = new Int_t[nentries];
  trackCut2_all = new Int_t[nentries];
  MVAout_prefilter_all = new Float_t[nentries];
  MVAout_noPrefilter_all = new Float_t[nentries];

  for(Long64_t i=0; i<nentries; i++) {
    EventID_all[i] = 0;
    TrackID1_all[i] = 0;
    TrackID2_all[i] = 0;
    IsConv_all[i] = 0;
    IsRP_all[i] = 0;
    pt1_all[i] = 0.;
    pt2_all[i] = 0.;
    mass_all[i] = 0.;
    trackCut1_all[i] = -1;
    trackCut2_all[i] = -1;
    MVAout_prefilter_all[i] = 0.;
    MVAout_noPrefilter_all[i] = 0.;
  }

  std::cout << "Reading the data...";

  for(Long64_t i=0; i<nentries; i++) {
    tree_MCdatafile->GetEntry(i);
      
    EventID_all[i] = EventID;
    TrackID1_all[i] = TrackID1;
    TrackID2_all[i] = TrackID2;
    IsConv_all[i] = IsConv;
    IsRP_all[i] = IsRP;
    pt1_all[i] = pt1;
    pt2_all[i] = pt2;
    mass_all[i] = mass;
    trackCut1_all[i] = TrackCut1;
    trackCut2_all[i] = TrackCut2;
  }
  std::cout << " DONE" << std::endl;


  
  TStopwatch *watch_overall = new TStopwatch();
  watch_overall->Start();

  
  for(Int_t scan=0; scan<=num_steps; scan++) {

    TStopwatch *watch_step = new TStopwatch();
    watch_step->Start();

    Int_t *tags_prefilter = NULL;
    Int_t *tags_noPrefilter = NULL;

    tags_prefilter = new Int_t[nentries];
    tags_noPrefilter = new Int_t[nentries];
    
    for(Long64_t i=0; i<nentries; i++) {
      tags_prefilter[i] = 0;
      tags_noPrefilter[i] = 0;
    }
    
    
    pairs_IsTaggedAccepted_prefilter.clear();
    pairs_IsTaggedAccepted_noPrefilter.clear();
    pairs_IsTrueConv.clear();
    pairs_pairweight.clear();

    
    Float_t MVAcut = scan/(num_steps*1.0)*(MVAoutputRange_max-MVAoutputRange_min) + MVAoutputRange_min;


    std::cout << std::endl << "---------- Step " << scan << " of " << num_steps
	      << " (MVA cut: " << MVAcut << "): ----------" << std::endl;

    std::cout << "Reading the data...";

    for(Long64_t i=0; i<nentries; i++) {
      tree_MVAoutput_prefilter_file->GetEntry(i);
      tree_MVAoutput_noPrefilter_file->GetEntry(i);
      
      MVAout_prefilter_all[i] = MVAout_prefilter;
      MVAout_noPrefilter_all[i] = MVAout_noPrefilter;
    }
    std::cout << " DONE" << std::endl;


    TRandom *rand = new TRandom();
    
    std::cout << "Tagging pairs and applying the prefilter..."
	      << std::endl;
    
    std::cout << "Step 1/2:" << std::endl;
    
    for(Long64_t j=0; j<nentries; j++) {
      if((j%1000)==0) {
	std::cout << "\r  (" << j << " / " << nentries << ")";
      }
      
      if( (MVAout_prefilter_all[j] < MVAoutputRange_min ||
	   MVAout_prefilter_all[j] > MVAoutputRange_max) ) {
        tags_prefilter[j] = -999;
	continue;
      }

      if(signalRegion == "+" && MVAout_prefilter_all[j] > MVAcut) {
	tags_prefilter[j] = 1;
      }else if(signalRegion == "-" && MVAout_prefilter_all[j] < MVAcut) {
	tags_prefilter[j] = 1;
      }

      Int_t EventID_current = EventID_all[j];
      Int_t TrackID1_current = TrackID1_all[j];
      Int_t TrackID2_current = TrackID2_all[j];

      Float_t pairWeight_j = getPairPIDefficiency(pt1_all[j], pt2_all[j], *h_PIDeffs);

      Bool_t doForwardProp = (rand->Uniform() < pairWeight_j) ? kTRUE : kFALSE;
      
      
      for(Int_t i=j+1; i<nentries; i++) {
	  
	if(EventID_all[i] != EventID_current) break;

	// 'forward propagation' of tag information:
	if(doForwardProp &&
	   ((signalRegion == "+" && MVAout_prefilter_all[j] > MVAcut) ||
	    (signalRegion == "-" && MVAout_prefilter_all[j] < MVAcut)) &&
	   (TrackID1_current == TrackID1_all[i] || TrackID2_current == TrackID2_all[i] ||
	    TrackID1_current == TrackID2_all[i] || TrackID2_current == TrackID1_all[i])) {
	  tags_prefilter[i] = 1;
	}

	Float_t pairWeight_i = getPairPIDefficiency(pt1_all[i], pt2_all[i], *h_PIDeffs);

	Bool_t doBackwardProp = (rand->Uniform() < pairWeight_i) ? kTRUE : kFALSE;
	
	// 'backward propagation' of tag information:
	if(doBackwardProp &&
	   ((signalRegion == "+" && MVAout_prefilter_all[i] > MVAcut) ||
	    (signalRegion == "-" && MVAout_prefilter_all[i] < MVAcut)) &&
	   (TrackID1_current == TrackID1_all[i] || TrackID2_current == TrackID2_all[i] ||
	    TrackID1_current == TrackID2_all[i] || TrackID2_current == TrackID1_all[i])) {
	  tags_prefilter[j] = 1;
	}
      }
      
    }
    std::cout << std::endl << "Step 2/2:" << std::endl;

    for(Long64_t j=0; j<nentries; j++) {
      if((j%1000)==0) {
        std::cout << "\r  (" << j << " / " << nentries << ")";
      }

      if( (MVAout_noPrefilter_all[j] < MVAoutputRange_min ||
           MVAout_noPrefilter_all[j] > MVAoutputRange_max) ) {
        tags_noPrefilter[j] = -999;
        continue;
      }

      if(signalRegion == "+" && MVAout_noPrefilter_all[j] > MVAcut) {
        tags_noPrefilter[j] = 1;
      }else if(signalRegion == "-" && MVAout_noPrefilter_all[j] < MVAcut) {
        tags_noPrefilter[j] = 1;
      }
    }
    std::cout << std::endl;

    
    std::cout << std::endl << "Calculating output values...";
    Double_t tp_noPrefilter=0, fp_noPrefilter=0, fn_noPrefilter=0, tn_noPrefilter=0;
    Double_t tp_prefilter=0, fp_prefilter=0, fn_prefilter=0, tn_prefilter=0;
    
    for(Long64_t i=0; i<nentries; i++) {

      Double_t pairweight = getPairPIDefficiency(pt1_all[i], pt2_all[i], *h_PIDeffs);
      
      if( (tags_prefilter[i]!=0 && tags_prefilter[i]!=1) ||
          mass_all[i]<massCut ||
          (containsTrackCutInfo && (trackCut1_all[i]!=2 || trackCut2_all[i]!=2)) ) {
	pairs_IsTaggedAccepted_prefilter.push_back(-999);
	pairs_IsTrueConv.push_back(IsConv_all[i]);
	pairs_pairweight.push_back(pairweight);
	continue;
      }

      if(tags_prefilter[i]==1 && IsConv_all[i]==1) tn_prefilter += pairweight;
      if(tags_prefilter[i]==1 && IsConv_all[i]==0) fn_prefilter += pairweight;
      if(tags_prefilter[i]==0 && IsConv_all[i]==1) fp_prefilter += pairweight;
      if(tags_prefilter[i]==0 && IsConv_all[i]==0) tp_prefilter += pairweight;

      pairs_IsTaggedAccepted_prefilter.push_back(tags_prefilter[i]);
      pairs_IsTrueConv.push_back(IsConv_all[i]);
      pairs_pairweight.push_back(pairweight);
      
    }

    for(Long64_t i=0; i<nentries; i++) {

      if( (tags_noPrefilter[i]!=0 && tags_noPrefilter[i]!=1) ||
          mass_all[i]<massCut ||
          (containsTrackCutInfo && (trackCut1_all[i]!=2 || trackCut2_all[i]!=2)) ) {
        pairs_IsTaggedAccepted_noPrefilter.push_back(-999);
        continue;
      }

      Double_t pairweight = getPairPIDefficiency(pt1_all[i], pt2_all[i], *h_PIDeffs);

      if(tags_noPrefilter[i]==1 && IsConv_all[i]==1) tn_noPrefilter += pairweight;
      if(tags_noPrefilter[i]==1 && IsConv_all[i]==0) fn_noPrefilter += pairweight;
      if(tags_noPrefilter[i]==0 && IsConv_all[i]==1) fp_noPrefilter += pairweight;
      if(tags_noPrefilter[i]==0 && IsConv_all[i]==0) tp_noPrefilter += pairweight;

      pairs_IsTaggedAccepted_noPrefilter.push_back(tags_noPrefilter[i]);
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

    delete [] tags_prefilter;
    delete [] tags_noPrefilter;
    
    tags_prefilter = NULL;
    tags_noPrefilter = NULL;
  }

  delete [] EventID_all;
  delete [] TrackID1_all;
  delete [] TrackID2_all;
  delete [] IsConv_all;
  delete [] IsRP_all;
  delete [] pt1_all;
  delete [] pt2_all;
  delete [] mass_all;
  delete [] MVAout_prefilter_all;
  delete [] MVAout_noPrefilter_all;
    
  EventID_all = NULL;
  TrackID1_all = NULL;
  TrackID2_all = NULL;
  IsConv_all = NULL;
  IsRP_all = NULL;
  pt1_all = NULL;
  pt2_all = NULL;
  mass_all = NULL;
  MVAout_prefilter_all = NULL;
  MVAout_noPrefilter_all = NULL;
  
  std::cout << "Write data...";
  outfile->cd();
  outfile->Write("", TObject::kOverwrite);
  std::cout << " DONE." << std::endl;
  std::cout << "File " << outfile->GetName() << " created." << std::endl;


  outfile->Close();
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
    std::cout << "\r  Processing entry " << i+1 << " of " << num_steps << "...";
    
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

        if( (pairs_prefilter_compare->at(k)!=0 && pairs_prefilter_compare->at(k)!=1) ||
            (pairs_IsTaggedAccepted_noPrefilter_combi->at(k)!=0 && pairs_IsTaggedAccepted_noPrefilter_combi->at(k)!=1) ) {
          continue;
        }

	if( (pairs_prefilter_compare->at(k)==0 && pairs_IsTaggedAccepted_noPrefilter_combi->at(k)==0) &&
	    pairs_IsTrueConv_combi->at(k)==0 ) {
	  tp_combi += pairs_pairweight_combi->at(k);
	}
	
	if( (pairs_prefilter_compare->at(k)==1 || pairs_IsTaggedAccepted_noPrefilter_combi->at(k)==1) &&
	    pairs_IsTrueConv_combi->at(k)==0 ) {
	  fn_combi += pairs_pairweight_combi->at(k);
	}
	
	if( (pairs_prefilter_compare->at(k)==0 && pairs_IsTaggedAccepted_noPrefilter_combi->at(k)==0) &&
	    pairs_IsTrueConv_combi->at(k)==1 ) {
	  fp_combi += pairs_pairweight_combi->at(k);
	}
	
	if( (pairs_prefilter_compare->at(k)==1 || pairs_IsTaggedAccepted_noPrefilter_combi->at(k)==1) &&
	    pairs_IsTrueConv_combi->at(k)==1 ) {
	  tn_combi += pairs_pairweight_combi->at(k);
	}

      }

      tpr_combi = (tp_combi+fn_combi==0) ? 0 : tp_combi/(tp_combi+fn_combi*1.0);
      fpr_combi = (fp_combi+tn_combi==0) ? 0 : fp_combi/(fp_combi+tn_combi*1.0);

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
  
  gSystem->Exit(0);
  
}



Float_t getPairPIDefficiency(Float_t pt1, Float_t pt2, TH1D &h_PIDeff) {

  Float_t PIDeff1 = (pt1 >= h_PIDeff.GetBinLowEdge(h_PIDeff.GetNbinsX())) ?
    h_PIDeff.GetBinContent(h_PIDeff.GetNbinsX()) : h_PIDeff.GetBinContent(h_PIDeff.FindBin(pt1));

  Float_t PIDeff2 = (pt2 >= h_PIDeff.GetBinLowEdge(h_PIDeff.GetNbinsX())) ?
    h_PIDeff.GetBinContent(h_PIDeff.GetNbinsX()) : h_PIDeff.GetBinContent(h_PIDeff.FindBin(pt2));

  return PIDeff1 * PIDeff2;
  
}
