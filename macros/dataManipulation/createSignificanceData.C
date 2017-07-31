#include <iostream>
#include <vector>
#include <algorithm>

#include <TROOT.h>
#include <TMath.h>
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
  Int_t    IsTaggedAccepted_RPConvRejMVA;
  Int_t    IsTaggedAccepted_CombConvRejMVA;
  Int_t    IsTaggedAccepted_singleTrackConvRejMVA;
  Int_t    IsTrueConv;
  Int_t    IsTrueRP;
  Float_t  pt1;
  Float_t  pt2;
  Int_t    IsTrueConv_singleTrackConvRejMVA; //testing
  Int_t    IsTrueRP_singleTrackConvRejMVA; //testing
  Float_t  pt1_singleTrackConvRejMVA; //testing
  Float_t  pt2_singleTrackConvRejMVA; //testing
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




void createSignificanceData(TString MCdatafilename,
			    TString treename_MCdatafile,
			    
			    TString MVAoutput_RPConvRejMVA_filename,
			    TString treename_MVAoutput_RPConvRejMVA_file,
			    TString branchname_MVAoutput_RPConvRejMVA_file,
			    
			    TString MVAoutput_CombConvRejMVA_filename,
			    TString treename_MVAoutput_CombConvRejMVA_file,
			    TString branchname_MVAoutput_CombConvRej_file,
			    
			    TString MVAoutput_singleTrackConvRejMVA_filename,
			    TString treename_MVAoutput_singleTrackConvRejMVA_file,
			    TString branchname_MVAoutput_singleTrackConvRejMVA_file_1,
			    TString branchname_MVAoutput_singleTrackConvRejMVA_file_2,
			    
			    TString signalRegion = "+",
			    Int_t   num_steps = 10, // number of steps in the MVA cut scan
			    Float_t MVAoutputRange_min = 0.,
			    Float_t MVAoutputRange_max = 1.,
			    TString outfilename = "temp_output/significance_data",
			    TString PIDeffs_filename = "~/analysis/data/FT2_AnalysisResults_Upgrade/inputData/ITSU_PIDefficiency_lowB.root") {


  Float_t processDataFraction = -1.; // process only this fraction of the data (if "-1", use a fixed number)
  Long64_t processDataEntries = 16400000; // process this number of entries (if "-1", all entries are selected)
  
  Float_t MVAout_RPConvRejMVA, MVAout_CombConvRejMVA;
  Float_t MVAout_singleTrackConvRejMVA_1, MVAout_singleTrackConvRejMVA_2;
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


  TFile *MVAoutput_RPConvRejMVA_file = new TFile(MVAoutput_RPConvRejMVA_filename, "READ");
  TTree *tree_MVAoutput_RPConvRejMVA_file = (TTree*)MVAoutput_RPConvRejMVA_file->Get(treename_MVAoutput_RPConvRejMVA_file);
  tree_MVAoutput_RPConvRejMVA_file->SetBranchAddress(branchname_MVAoutput_RPConvRejMVA_file,
						     &MVAout_RPConvRejMVA);
  

  if(tree_MCdatafile->GetEntries() != tree_MVAoutput_RPConvRejMVA_file->GetEntries()) {
    std::cout << "  ERROR: Trees have different sizes." << std::endl
	      << "    Size of tree " << treename_MCdatafile << " in "
	      << MCdatafilename << ": " << tree_MCdatafile->GetEntries()
	      << std::endl
	      << "    Size of tree " << treename_MVAoutput_RPConvRejMVA_file << " in "
	      << MVAoutput_RPConvRejMVA_filename << ": " << tree_MVAoutput_RPConvRejMVA_file->GetEntries()
	      << std::endl << std::endl;
    // gApplication->Terminate(); //testing
  }



  TFile *MVAoutput_CombConvRejMVA_file = new TFile(MVAoutput_CombConvRejMVA_filename, "READ");
  TTree *tree_MVAoutput_CombConvRejMVA_file = (TTree*)MVAoutput_CombConvRejMVA_file->Get(treename_MVAoutput_CombConvRejMVA_file);
  tree_MVAoutput_CombConvRejMVA_file->SetBranchAddress(branchname_MVAoutput_CombConvRej_file,
						       &MVAout_CombConvRejMVA);

  if(tree_MCdatafile->GetEntries() != tree_MVAoutput_CombConvRejMVA_file->GetEntries()) {
    std::cout << "  ERROR: Trees have different sizes." << std::endl
	      << "    Size of tree " << tree_MCdatafile->GetEntries()
	      << std::endl
	      << "    Size of tree " << tree_MVAoutput_CombConvRejMVA_file->GetEntries()
	      << std::endl << std::endl;
    // gApplication->Terminate(); //testing
  }

  

  TFile *MVAoutput_singleTrackConvRejMVA_file = new TFile(MVAoutput_singleTrackConvRejMVA_filename, "READ");
  TTree *tree_MVAoutput_singleTrackConvRejMVA_file = (TTree*)MVAoutput_singleTrackConvRejMVA_file->Get(treename_MVAoutput_singleTrackConvRejMVA_file);
  tree_MVAoutput_singleTrackConvRejMVA_file->SetBranchAddress(branchname_MVAoutput_singleTrackConvRejMVA_file_1,
   							      &MVAout_singleTrackConvRejMVA_1);
  tree_MVAoutput_singleTrackConvRejMVA_file->SetBranchAddress(branchname_MVAoutput_singleTrackConvRejMVA_file_2,
							      &MVAout_singleTrackConvRejMVA_2);
  Int_t IsConv_singleTrackConvRejMVA, IsRP_singleTrackConvRejMVA; //testing
  Float_t pt1_singleTrackConvRejMVA, pt2_singleTrackConvRejMVA; //testing
  tree_MVAoutput_singleTrackConvRejMVA_file->SetBranchAddress("IsConv", &IsConv_singleTrackConvRejMVA); //testing
  tree_MVAoutput_singleTrackConvRejMVA_file->SetBranchAddress("IsRP", &IsRP_singleTrackConvRejMVA); //testing
  tree_MVAoutput_singleTrackConvRejMVA_file->SetBranchAddress("pt1", &pt1_singleTrackConvRejMVA); //testing
  tree_MVAoutput_singleTrackConvRejMVA_file->SetBranchAddress("pt2", &pt2_singleTrackConvRejMVA); //testing
  

  if(tree_MCdatafile->GetEntries() != tree_MVAoutput_singleTrackConvRejMVA_file->GetEntries()) {
    std::cout << "  ERROR: Trees have different sizes." << std::endl
  	      << "    Size of tree " << tree_MCdatafile->GetEntries()
  	      << std::endl
  	      << "    Size of tree " << tree_MVAoutput_singleTrackConvRejMVA_file->GetEntries()
  	      << std::endl << std::endl;
    // gApplication->Terminate(); //testing
  }

  std::cout << " DONE." << std::endl;
  
  

  if(!outfilename.EndsWith(".root")) {
    outfilename.Append(".root");
  }

  TFile *outfile = new TFile(outfilename, "RECREATE");
  TString outtree_name = "significanceData";
  TTree *outtree = new TTree(outtree_name, outtree_name);

  Float_t MVAcut;
  Float_t Seff_RPConvRejMVA, Beff_RPConvRejMVA;
  Float_t Seff_CombConvRejMVA, Beff_CombConvRejMVA;
  Float_t Seff_singleTrackConvRejMVA, Beff_singleTrackConvRejMVA;
  Float_t significance_RPConvRejMVA;
  Float_t significance_CombConvRejMVA;
  Float_t significance_singleTrackConvRejMVA;
  Float_t significance_gain_RPConvRejMVA;
  Float_t significance_gain_CombConvRejMVA;
  Float_t significance_gain_singleTrackConvRejMVA;
  outtree->Branch("MVAcut", &MVAcut, "MVAcut/F");
  outtree->Branch("S_eff_RPConvRejMVA", &Seff_RPConvRejMVA, "S_eff_RPConvRejMVA/F");
  outtree->Branch("B_eff_RPConvRejMVA", &Beff_RPConvRejMVA, "B_eff_RPConvRejMVA/F");
  outtree->Branch("S_eff_CombConvRejMVA", &Seff_CombConvRejMVA, "S_eff_CombConvRejMVA/F");
  outtree->Branch("B_eff_CombConvRejMVA", &Beff_CombConvRejMVA, "B_eff_CombConvRejMVA/F");
  outtree->Branch("S_eff_singleTrackConvRejMVA", &Seff_singleTrackConvRejMVA, "S_eff_singleTrackConvRejMVA/F");
  outtree->Branch("B_eff_singleTrackConvRejMVA", &Beff_singleTrackConvRejMVA, "B_eff_singleTrackConvRejMVA/F");
  outtree->Branch("significance_RPConvRejMVA", &significance_RPConvRejMVA, "significance_RPConvRejMVA/F");
  outtree->Branch("significance_CombConvRejMVA", &significance_CombConvRejMVA, "significance_CombConvRejMVA/F");
  outtree->Branch("significance_singleTrackConvRejMVA", &significance_singleTrackConvRejMVA, "significance_singleTrackConvRejMVA/F");
  outtree->Branch("significance_gain_RPConvRejMVA", &significance_gain_RPConvRejMVA, "significance_gain_RPConvRejMVA/F");
  outtree->Branch("significance_gain_CombConvRejMVA", &significance_gain_CombConvRejMVA, "significance_gain_CombConvRejMVA/F");
  outtree->Branch("significance_gain_singleTrackConvRejMVA", &significance_gain_singleTrackConvRejMVA, "significance_gain_singleTrackConvRejMVA/F");


  TFile *infile_PIDeffs = new TFile(PIDeffs_filename, "READ");
  TH1D *h_PIDeffs = (TH1D*)infile_PIDeffs->Get("efficiencyLHC17d12_TPCandTOF3sigma");
  h_PIDeffs->GetXaxis()->SetRangeUser(0,5);


  
  Long64_t nentries = tree_MCdatafile->GetEntries();
  if(processDataFraction != -1) nentries = nentries*processDataFraction;
  else if(processDataEntries != -1) nentries = processDataEntries;


  // significances in case of (1) ideal and (2) no background rejection:
  Float_t significance_ideal;
  Float_t significance_ideal_singleTrackConvRejMVA; //testing
  Float_t significance_RPConvRejMVA_all;
  Float_t significance_CombConvRejMVA_all;
  Float_t significance_singleTrackConvRejMVA_all;
  
  
  TStopwatch *watch_overall = new TStopwatch();
  watch_overall->Start();


  for(Int_t scan=0; scan<=num_steps; scan++) {

    TStopwatch *watch_step = new TStopwatch();
    watch_step->Start();
    
    
    // Collection of all particle pairs (of all events):
    std::vector<particlePair> allPairs;
    
    // Collection of all accepted particle tracks (of all events):
    std::vector<particleTrack> tracksTaggedAccepted;

    
    MVAcut = scan/(num_steps*1.0)*(MVAoutputRange_max-MVAoutputRange_min) + MVAoutputRange_min;


    std::cout << std::endl << std::endl
	      << "---------- Step " << scan << " of " << num_steps
	      << " (MVA cut: " << MVAcut << "): ----------" << std::endl;

    
    std::cout << "Tagging pairs based on MVA output..." << std::endl;
  
    for(Long64_t j=0; j<nentries; j++) {
      if((j%10000)==0) {
	std::cout << "\r  Processing event " << j << " of " << nentries
		  << " (" << j/((Float_t)nentries)*100 << " %)...";
      }


      tree_MCdatafile->GetEntry(j);
      tree_MVAoutput_RPConvRejMVA_file->GetEntry(j);
      tree_MVAoutput_CombConvRejMVA_file->GetEntry(j);
      tree_MVAoutput_singleTrackConvRejMVA_file->GetEntry(j);
      
    
      particlePair currentPair;
      currentPair.EventID = EventID;
      currentPair.TrackID1 = TrackID1;
      currentPair.TrackID2 = TrackID2;
      currentPair.IsTrueConv = IsConv;
      currentPair.IsTrueRP = IsRP;
      currentPair.pt1 = pt1;
      currentPair.pt2 = pt2;
      currentPair.IsTrueConv_singleTrackConvRejMVA = IsConv_singleTrackConvRejMVA; //testing
      currentPair.IsTrueRP_singleTrackConvRejMVA = IsRP_singleTrackConvRejMVA; //testing
      currentPair.pt1_singleTrackConvRejMVA = pt1_singleTrackConvRejMVA; //testing
      currentPair.pt2_singleTrackConvRejMVA = pt2_singleTrackConvRejMVA; //testing
      currentPair.originalPosition = j;
    
      if(signalRegion == "+") {

	if(MVAout_RPConvRejMVA < MVAoutputRange_min || MVAout_RPConvRejMVA > MVAoutputRange_max) {
	  currentPair.IsTaggedAccepted_RPConvRejMVA = -999;
	}else if(MVAout_RPConvRejMVA >= MVAcut) {
	  currentPair.IsTaggedAccepted_RPConvRejMVA = 1;

	  particleTrack currentTrack1 = {EventID, TrackID1};
	  particleTrack currentTrack2 = {EventID, TrackID2};
	  tracksTaggedAccepted.push_back(currentTrack1);
	  tracksTaggedAccepted.push_back(currentTrack2);
	}else currentPair.IsTaggedAccepted_RPConvRejMVA = 0;

	if(MVAout_CombConvRejMVA < MVAoutputRange_min || MVAout_CombConvRejMVA > MVAoutputRange_max) {
	  currentPair.IsTaggedAccepted_CombConvRejMVA = -999;
	}else if(MVAout_CombConvRejMVA >= MVAcut) {
	  currentPair.IsTaggedAccepted_CombConvRejMVA = 1;
	}else currentPair.IsTaggedAccepted_CombConvRejMVA = 0;

	if((MVAout_singleTrackConvRejMVA_1 < MVAoutputRange_min || MVAout_singleTrackConvRejMVA_1 > MVAoutputRange_max) ||
	   (MVAout_singleTrackConvRejMVA_2 < MVAoutputRange_min || MVAout_singleTrackConvRejMVA_2 > MVAoutputRange_max)) {
	  currentPair.IsTaggedAccepted_singleTrackConvRejMVA = -999;
	}else if(TMath::Max(MVAout_singleTrackConvRejMVA_1, MVAout_singleTrackConvRejMVA_2) >= MVAcut) {
	  currentPair.IsTaggedAccepted_singleTrackConvRejMVA = 1;
	}else currentPair.IsTaggedAccepted_singleTrackConvRejMVA = 0;
	
      }else if(signalRegion == "-") {

	if(MVAout_RPConvRejMVA < MVAoutputRange_min || MVAout_RPConvRejMVA > MVAoutputRange_max) {
	  currentPair.IsTaggedAccepted_RPConvRejMVA = -999;
	}else if(MVAout_RPConvRejMVA <= MVAcut) {
	  currentPair.IsTaggedAccepted_RPConvRejMVA = 1;

	  particleTrack currentTrack1 = {EventID, TrackID1};
	  particleTrack currentTrack2 = {EventID, TrackID2};
	  tracksTaggedAccepted.push_back(currentTrack1);
	  tracksTaggedAccepted.push_back(currentTrack2);
	}else currentPair.IsTaggedAccepted_RPConvRejMVA = 0;

	if(MVAout_CombConvRejMVA < MVAoutputRange_min || MVAout_CombConvRejMVA > MVAoutputRange_max) {
	  currentPair.IsTaggedAccepted_CombConvRejMVA = -999;
	}else if(MVAout_CombConvRejMVA <= MVAcut) {
	  currentPair.IsTaggedAccepted_CombConvRejMVA = 1;
	}else currentPair.IsTaggedAccepted_CombConvRejMVA = 0;

	if((MVAout_singleTrackConvRejMVA_1 < MVAoutputRange_min || MVAout_singleTrackConvRejMVA_1 > MVAoutputRange_max) ||
	   (MVAout_singleTrackConvRejMVA_2 < MVAoutputRange_min || MVAout_singleTrackConvRejMVA_2 > MVAoutputRange_max)) {
	  currentPair.IsTaggedAccepted_singleTrackConvRejMVA = -999;
	}else if(TMath::Min(MVAout_singleTrackConvRejMVA_1, MVAout_singleTrackConvRejMVA_2) <= MVAcut) {
	  currentPair.IsTaggedAccepted_singleTrackConvRejMVA = 1;
	}else currentPair.IsTaggedAccepted_singleTrackConvRejMVA = 0;
	
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

    std::cout << "Number of pairs (RPConvRejMVA): " << allPairs.size() << std::endl;
    std::cout << "Number of accepted tracks (RPConvRejMVA): " << tracksTaggedAccepted.size() << std::endl;
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
    
      if( allPairs[i].IsTaggedAccepted_RPConvRejMVA == -999 ) continue; // skip irrelevant entries
    
      for(Long64_t j=eventID_startPos[allPairs[i].EventID];
	  j<(Long64_t)tracksTaggedAccepted.size();
	  j++) {
	if( allPairs[i].EventID != tracksTaggedAccepted[j].EventID ) {
	  break;
	}else if(allPairs[i].TrackID1==tracksTaggedAccepted[j].TrackID ||
		 allPairs[i].TrackID2==tracksTaggedAccepted[j].TrackID) {
	  allPairs[i].IsTaggedAccepted_RPConvRejMVA = 1.;
	}
      }
    }
    std::cout << "\r  Processing event " << nentries << " of " << nentries
	      << " (100 %)... DONE." << std::endl;


    // restore original order of allPairs vector:
    std::cout << "Restoring original order of data...";
    std::sort(allPairs.begin(), allPairs.end(), sortByOriginalPosition);
    std::cout << " DONE." << std::endl;


    
    std::cout << "Calculating signal and background efficiencies...";

    // determine significance in case of (1) ideal and (2) no background rejection:
    if(scan==0) {
      
      Float_t num_S_ideal_temp=0., num_B_ideal_temp=0.;
      Float_t num_S_ideal_singleTrackConvRejMVA_temp=0., num_B_ideal_singleTrackConvRejMVA_temp=0.;
      
      Float_t num_S_RPConvRejMVA_temp=0., num_B_RPConvRejMVA_temp=0.;
      Float_t num_S_CombConvRejMVA_temp=0., num_B_CombConvRejMVA_temp=0.;
      Float_t num_S_singleTrackConvRejMVA_temp=0., num_B_singleTrackConvRejMVA_temp=0.;
      
      for(Long64_t i=0; i<nentries; i++) {
	
	Float_t pairweight_temp = getPairPIDefficiency(allPairs[i].pt1, allPairs[i].pt2, *h_PIDeffs);
	Float_t pairweight_singleTrackConvRejMVA_temp = getPairPIDefficiency(allPairs[i].pt1_singleTrackConvRejMVA, allPairs[i].pt2_singleTrackConvRejMVA, *h_PIDeffs); //testing

	if(!allPairs[i].IsTrueConv) num_S_ideal_temp += pairweight_temp;
	if(allPairs[i].IsTrueConv) num_B_ideal_temp += pairweight_temp;

	if(!allPairs[i].IsTrueConv_singleTrackConvRejMVA) num_S_ideal_singleTrackConvRejMVA_temp += pairweight_singleTrackConvRejMVA_temp; //testing
	if(allPairs[i].IsTrueConv_singleTrackConvRejMVA) num_B_ideal_singleTrackConvRejMVA_temp += pairweight_singleTrackConvRejMVA_temp; //testing
	
	if(signalRegion == "+") {
	  if(allPairs[i].IsTaggedAccepted_RPConvRejMVA && !allPairs[i].IsTrueConv) num_S_RPConvRejMVA_temp += pairweight_temp;
	  if(allPairs[i].IsTaggedAccepted_RPConvRejMVA && allPairs[i].IsTrueConv) num_B_RPConvRejMVA_temp += pairweight_temp;

	  if(allPairs[i].IsTaggedAccepted_CombConvRejMVA && !allPairs[i].IsTrueConv) num_S_CombConvRejMVA_temp += pairweight_temp;
	  if(allPairs[i].IsTaggedAccepted_CombConvRejMVA && allPairs[i].IsTrueConv) num_B_CombConvRejMVA_temp += pairweight_temp;

	  if(allPairs[i].IsTaggedAccepted_singleTrackConvRejMVA && !allPairs[i].IsTrueConv) num_S_singleTrackConvRejMVA_temp += pairweight_singleTrackConvRejMVA_temp; //testing
	  if(allPairs[i].IsTaggedAccepted_singleTrackConvRejMVA && allPairs[i].IsTrueConv) num_B_singleTrackConvRejMVA_temp += pairweight_singleTrackConvRejMVA_temp; //testing
	}
	
	if(signalRegion == "-") {
	  if(!allPairs[i].IsTaggedAccepted_RPConvRejMVA && !allPairs[i].IsTrueConv) num_S_RPConvRejMVA_temp += pairweight_temp;
	  if(!allPairs[i].IsTaggedAccepted_RPConvRejMVA && allPairs[i].IsTrueConv) num_B_RPConvRejMVA_temp += pairweight_temp;

	  if(!allPairs[i].IsTaggedAccepted_CombConvRejMVA && !allPairs[i].IsTrueConv) num_S_CombConvRejMVA_temp += pairweight_temp;
	  if(!allPairs[i].IsTaggedAccepted_CombConvRejMVA && allPairs[i].IsTrueConv) num_B_CombConvRejMVA_temp += pairweight_temp;

	  if(!allPairs[i].IsTaggedAccepted_singleTrackConvRejMVA && !allPairs[i].IsTrueConv) num_S_singleTrackConvRejMVA_temp += pairweight_singleTrackConvRejMVA_temp; //testing
	  if(!allPairs[i].IsTaggedAccepted_singleTrackConvRejMVA && allPairs[i].IsTrueConv) num_B_singleTrackConvRejMVA_temp += pairweight_singleTrackConvRejMVA_temp; //testing
	}
      }
      

      if(num_S_ideal_temp!=0 || num_B_ideal_temp!=0) {
	significance_ideal = num_S_ideal_temp/TMath::Sqrt(num_S_ideal_temp);
      }else significance_ideal = 0.;

      if(num_S_ideal_singleTrackConvRejMVA_temp!=0 || num_B_ideal_singleTrackConvRejMVA_temp!=0) {
	significance_ideal_singleTrackConvRejMVA = num_S_ideal_singleTrackConvRejMVA_temp/TMath::Sqrt(num_S_ideal_singleTrackConvRejMVA_temp); //testing
      }else significance_ideal_singleTrackConvRejMVA = 0.; //testing
	
      if(num_S_RPConvRejMVA_temp!=0 || num_B_RPConvRejMVA_temp!=0) {
	significance_RPConvRejMVA_all = num_S_RPConvRejMVA_temp/TMath::Sqrt(num_S_RPConvRejMVA_temp+num_B_RPConvRejMVA_temp);
      }else significance_RPConvRejMVA_all = 0.;
      
      if(num_S_CombConvRejMVA_temp!=0 || num_B_CombConvRejMVA_temp!=0) {
	significance_CombConvRejMVA_all = num_S_CombConvRejMVA_temp/TMath::Sqrt(num_S_CombConvRejMVA_temp+num_B_CombConvRejMVA_temp);
      }else significance_CombConvRejMVA_all = 0.;
      
      if(num_S_singleTrackConvRejMVA_temp!=0 || num_B_singleTrackConvRejMVA_temp!=0) {
	significance_singleTrackConvRejMVA_all = num_S_singleTrackConvRejMVA_temp/TMath::Sqrt(num_S_singleTrackConvRejMVA_temp+num_B_singleTrackConvRejMVA_temp);
      }else significance_singleTrackConvRejMVA_all = 0.;

      //testing
      std::cout << " Ideal significance: " << significance_ideal << std::endl
		<< " Significance (no backgr. rej., RPConvRejMVA): " << significance_RPConvRejMVA_all << std::endl
		<< " Significance (no backgr. rej., CombConvRejMVA): " << significance_CombConvRejMVA_all << std::endl
		<< " Significance (no backgr. rej., singleTrackConvRejMVA): " << significance_singleTrackConvRejMVA_all << std::endl;
	
	
      
    }
    
    Float_t num_totalConvs=0, num_totalNonConvs=0;
    for(Long64_t i=0; i<nentries; i++) {
      if(allPairs[i].IsTrueConv) num_totalConvs += getPairPIDefficiency(allPairs[i].pt1, allPairs[i].pt2, *h_PIDeffs);
      else num_totalNonConvs += getPairPIDefficiency(allPairs[i].pt1, allPairs[i].pt2, *h_PIDeffs);
    }

    Float_t num_S_RPConvRejMVA=0., num_B_RPConvRejMVA=0.;
    Float_t num_S_CombConvRejMVA=0., num_B_CombConvRejMVA=0.;
    Float_t num_S_singleTrackConvRejMVA=0., num_B_singleTrackConvRejMVA=0.;
    
    for(Long64_t i=0; i<nentries; i++) {

      Float_t pairweight = getPairPIDefficiency(allPairs[i].pt1, allPairs[i].pt2, *h_PIDeffs);
      Float_t pairweight_singleTrackConvRejMVA = getPairPIDefficiency(allPairs[i].pt1_singleTrackConvRejMVA, allPairs[i].pt2_singleTrackConvRejMVA, *h_PIDeffs); //testing
	
      if(signalRegion == "+") {
	
	if(allPairs[i].IsTaggedAccepted_RPConvRejMVA && !allPairs[i].IsTrueConv) num_S_RPConvRejMVA += pairweight;
	if(allPairs[i].IsTaggedAccepted_RPConvRejMVA && allPairs[i].IsTrueConv) num_B_RPConvRejMVA += pairweight;

	if(allPairs[i].IsTaggedAccepted_CombConvRejMVA && !allPairs[i].IsTrueConv) num_S_CombConvRejMVA += pairweight;
	if(allPairs[i].IsTaggedAccepted_CombConvRejMVA && allPairs[i].IsTrueConv) num_B_CombConvRejMVA += pairweight;

	if(allPairs[i].IsTaggedAccepted_singleTrackConvRejMVA && !allPairs[i].IsTrueConv_singleTrackConvRejMVA) num_S_singleTrackConvRejMVA += pairweight_singleTrackConvRejMVA; //testing
	if(allPairs[i].IsTaggedAccepted_singleTrackConvRejMVA && allPairs[i].IsTrueConv_singleTrackConvRejMVA) num_B_singleTrackConvRejMVA += pairweight_singleTrackConvRejMVA; //testing
	
      }else if(signalRegion == "-") {
	
	if(!allPairs[i].IsTaggedAccepted_RPConvRejMVA && !allPairs[i].IsTrueConv) num_S_RPConvRejMVA += pairweight;
	if(!allPairs[i].IsTaggedAccepted_RPConvRejMVA && allPairs[i].IsTrueConv) num_B_RPConvRejMVA += pairweight;

	if(!allPairs[i].IsTaggedAccepted_CombConvRejMVA && !allPairs[i].IsTrueConv) num_S_CombConvRejMVA += pairweight;
	if(!allPairs[i].IsTaggedAccepted_CombConvRejMVA && allPairs[i].IsTrueConv) num_B_CombConvRejMVA += pairweight;

	if(!allPairs[i].IsTaggedAccepted_singleTrackConvRejMVA && !allPairs[i].IsTrueConv_singleTrackConvRejMVA) num_S_singleTrackConvRejMVA += pairweight_singleTrackConvRejMVA; //testing
	if(!allPairs[i].IsTaggedAccepted_singleTrackConvRejMVA && allPairs[i].IsTrueConv_singleTrackConvRejMVA) num_B_singleTrackConvRejMVA += pairweight_singleTrackConvRejMVA; //testing
	
      }
    }

    Seff_RPConvRejMVA = num_S_RPConvRejMVA/(num_totalNonConvs*1.0);
    Beff_RPConvRejMVA = num_B_RPConvRejMVA/(num_totalConvs*1.0);
    if(num_S_RPConvRejMVA!=0 || num_B_RPConvRejMVA!=0)
      significance_RPConvRejMVA = num_S_RPConvRejMVA/TMath::Sqrt(num_S_RPConvRejMVA+num_B_RPConvRejMVA);
    else significance_RPConvRejMVA = 0.;
    
    Seff_CombConvRejMVA = num_S_CombConvRejMVA/(num_totalNonConvs*1.0);
    Beff_CombConvRejMVA = num_B_CombConvRejMVA/(num_totalConvs*1.0);
    if(num_S_CombConvRejMVA!=0 || num_B_CombConvRejMVA!=0)
      significance_CombConvRejMVA = num_S_CombConvRejMVA/TMath::Sqrt(num_S_CombConvRejMVA+num_B_CombConvRejMVA);
    else significance_CombConvRejMVA = 0.;

    Seff_singleTrackConvRejMVA = num_S_singleTrackConvRejMVA/(num_totalNonConvs*1.0);
    Beff_singleTrackConvRejMVA = num_B_singleTrackConvRejMVA/(num_totalConvs*1.0);
    if(num_S_singleTrackConvRejMVA!=0 || num_B_singleTrackConvRejMVA!=0)
      significance_singleTrackConvRejMVA = num_S_singleTrackConvRejMVA/TMath::Sqrt(num_S_singleTrackConvRejMVA+num_B_singleTrackConvRejMVA);
    else significance_singleTrackConvRejMVA = 0.;


    if(significance_ideal-significance_RPConvRejMVA_all != 0) {
      significance_gain_RPConvRejMVA = (significance_RPConvRejMVA-significance_RPConvRejMVA_all) / (significance_ideal-significance_RPConvRejMVA_all);
    }else significance_gain_RPConvRejMVA = 0.;

    if(significance_ideal-significance_CombConvRejMVA_all != 0) {
      significance_gain_CombConvRejMVA = (significance_CombConvRejMVA-significance_CombConvRejMVA_all) / (significance_ideal-significance_CombConvRejMVA_all);
    }else significance_gain_CombConvRejMVA = 0.;

    if(significance_ideal_singleTrackConvRejMVA-significance_singleTrackConvRejMVA_all != 0) {
      significance_gain_singleTrackConvRejMVA = (significance_singleTrackConvRejMVA-significance_singleTrackConvRejMVA_all) / (significance_ideal_singleTrackConvRejMVA-significance_singleTrackConvRejMVA_all); //testing
    }else significance_gain_singleTrackConvRejMVA = 0.;


    outtree->Fill();

    std::cout << " DONE" << std::endl;
    std::cout << "  S_eff_RPConvRejMVA = " << Seff_RPConvRejMVA << ",\tB_eff_RPConvRejMVA = " << Beff_RPConvRejMVA
	      << ",\tsignificance_RPConvRejMVA = " << significance_RPConvRejMVA << std::endl;
    std::cout << "  S_eff_CombConvRejMVA = " << Seff_CombConvRejMVA << ",\tB_eff_CombConvRejMVA = " << Beff_CombConvRejMVA
	      << ",\tsignificance_CombConvRejMVA = " << significance_CombConvRejMVA << std::endl;
    std::cout << "  S_eff_singleTrackConvRejMVA = " << Seff_singleTrackConvRejMVA << ",\tB_eff_singleTrackConvRejMVA = " << Beff_singleTrackConvRejMVA
	      << ",\tsignificance_singleTrackConvRej = " << significance_singleTrackConvRejMVA << std::endl;
    std::cout << "  significance_gain_RPConvRejMVA = " << significance_gain_RPConvRejMVA << std::endl;
    std::cout << "  significance_gain_CombConvRejMVA = " << significance_gain_CombConvRejMVA << std::endl;
    std::cout << "  significance_gain_singleTrackConvRejMVA = " << significance_gain_singleTrackConvRejMVA << std::endl;

    std::cout << std::endl << "Time elapsed: " << watch_step->RealTime()
	      << " seconds." << std::endl;
  }


  std::cout << "Write data...";
  outfile->cd();
  outfile->Write("", TObject::kOverwrite);
  std::cout << " DONE." << std::endl;
  std::cout << "File " << outfile->GetName() << " created." << std::endl;



  MCdatafile->Close();
  MVAoutput_RPConvRejMVA_file->Close();
}





Float_t getPairPIDefficiency(Float_t pt1, Float_t pt2, TH1D &h_PIDeff) {

  Float_t PIDeff1 = (pt1 >= h_PIDeff.GetBinLowEdge(h_PIDeff.GetNbinsX())) ?
    h_PIDeff.GetBinContent(h_PIDeff.GetNbinsX()) : h_PIDeff.GetBinContent(h_PIDeff.FindBin(pt1));

  Float_t PIDeff2 = (pt2 >= h_PIDeff.GetBinLowEdge(h_PIDeff.GetNbinsX())) ?
    h_PIDeff.GetBinContent(h_PIDeff.GetNbinsX()) : h_PIDeff.GetBinContent(h_PIDeff.FindBin(pt2));

  return PIDeff1 * PIDeff2;
  
}
