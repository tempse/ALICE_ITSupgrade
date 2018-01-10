#include <iostream>
#include <vector>
#include <algorithm>

#include <TROOT.h>
#include <TMath.h>
#include <TSystem.h>
#include <TApplication.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TRandom.h>
#include <TStopwatch.h>



Float_t getPairPIDefficiency(Float_t, Float_t, TH1D&);




void createSignificanceData(TString MCdatafilename,
                            TString treename_MCdatafile,
			    
                            TString MVAoutput_RPConvRejMVA_filename,
                            TString treename_MVAoutput_RPConvRejMVA_file,
                            TString branchname_MVAoutput_RPConvRejMVA_file,
			    
                            TString MVAoutput_CombConvRejMVA_filename,
                            TString treename_MVAoutput_CombConvRejMVA_file,
                            TString branchname_MVAoutput_CombConvRej_file,
			    
                            TString MVAoutput_singleConvTrackRejMVA_filename,
                            TString treename_MVAoutput_singleConvTrackRejMVA_file,
                            TString branchname_MVAoutput_singleConvTrackRejMVA_file_1,
                            TString branchname_MVAoutput_singleConvTrackRejMVA_file_2,
			    
                            Int_t   num_steps = 10, // number of steps in the MVA cut scan
                            UInt_t  considerLooseCuts = 0, // 0...no loose cuts, 1...with loose cuts, 2...all tracks
                            Float_t massCut = 0.05, // do not count events with mass < massCut
                            Float_t MVAoutputRange_min = 0.,
                            Float_t MVAoutputRange_max = 1.,
                            TString outfilename = "temp_output/significance_data",
                            TString PIDeffs_filename = "/home/sebastian/analysis/data/finalAnalysis_FT2/inputdata/ITSU_PIDefficiency_lowB.root") {

  
  Float_t processDataFraction = -1.; // process only this fraction of the data (if "-1", use a fixed number)
  Long64_t processDataEntries = 5000000; // process this number of entries (if "-1", all entries are selected)

  Float_t MVAout_RPConvRejMVA, MVAout_CombConvRejMVA;
  Float_t MVAout_singleConvTrackRejMVA_1, MVAout_singleConvTrackRejMVA_2;
  Int_t isAccepted;


  std::cout << "Reading files...";
  TFile *MCdatafile = new TFile(MCdatafilename, "READ");
  TTree *tree_MCdatafile = (TTree*)MCdatafile->Get(treename_MCdatafile);
  Int_t EventID, TrackID1, TrackID2;
  Int_t IsConv, IsRP;
  Float_t pt1, pt2;
  Float_t mass;
  Int_t TrackCut1, TrackCut2;
  Bool_t containsTrackCutInfo = kTRUE;
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

  if(!containsTrackCutInfo && considerLooseCuts>0) {
    std::cout << "  ERROR: Loose track cuts have been chosen to be considered, but do not exist "
              << "in the file. Exiting without changing the files..." << std::endl;
    MCdatafile->Close();
    gSystem->Exit(1);
  }

  if(containsTrackCutInfo && considerLooseCuts>=3) {
    std::cout << "  ERROR: considerLooseCuts>=2. Exiting without changing the files..." << std::endl;
    MCdatafile->Close();
    gSystem->Exit(1);
  }
  

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
    gApplication->Terminate();
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
    gApplication->Terminate();
  }

  

  TFile *MVAoutput_singleConvTrackRejMVA_file = new TFile(MVAoutput_singleConvTrackRejMVA_filename, "READ");
  TTree *tree_MVAoutput_singleConvTrackRejMVA_file = (TTree*)MVAoutput_singleConvTrackRejMVA_file->Get(treename_MVAoutput_singleConvTrackRejMVA_file);
  tree_MVAoutput_singleConvTrackRejMVA_file->SetBranchAddress(branchname_MVAoutput_singleConvTrackRejMVA_file_1,
                                                              &MVAout_singleConvTrackRejMVA_1);
  tree_MVAoutput_singleConvTrackRejMVA_file->SetBranchAddress(branchname_MVAoutput_singleConvTrackRejMVA_file_2,
                                                              &MVAout_singleConvTrackRejMVA_2);
  

  if(tree_MCdatafile->GetEntries() != tree_MVAoutput_singleConvTrackRejMVA_file->GetEntries()) {
    std::cout << "  ERROR: Trees have different sizes." << std::endl
              << "    Size of tree " << tree_MCdatafile->GetEntries()
              << std::endl
              << "    Size of tree " << tree_MVAoutput_singleConvTrackRejMVA_file->GetEntries()
              << std::endl << std::endl;
    gApplication->Terminate();
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
  Float_t Seff_singleConvTrackRejMVA, Beff_singleConvTrackRejMVA;
  Float_t significance_RPConvRejMVA;
  Float_t significance_CombConvRejMVA;
  Float_t significance_singleConvTrackRejMVA;
  Float_t significance_gain_RPConvRejMVA;
  Float_t significance_gain_CombConvRejMVA;
  Float_t significance_gain_singleConvTrackRejMVA;
  outtree->Branch("MVAcut", &MVAcut, "MVAcut/F");
  outtree->Branch("S_eff_RPConvRejMVA", &Seff_RPConvRejMVA, "S_eff_RPConvRejMVA/F");
  outtree->Branch("B_eff_RPConvRejMVA", &Beff_RPConvRejMVA, "B_eff_RPConvRejMVA/F");
  outtree->Branch("S_eff_CombConvRejMVA", &Seff_CombConvRejMVA, "S_eff_CombConvRejMVA/F");
  outtree->Branch("B_eff_CombConvRejMVA", &Beff_CombConvRejMVA, "B_eff_CombConvRejMVA/F");
  outtree->Branch("S_eff_singleConvTrackRejMVA", &Seff_singleConvTrackRejMVA, "S_eff_singleConvTrackRejMVA/F");
  outtree->Branch("B_eff_singleConvTrackRejMVA", &Beff_singleConvTrackRejMVA, "B_eff_singleConvTrackRejMVA/F");
  outtree->Branch("significance_RPConvRejMVA", &significance_RPConvRejMVA, "significance_RPConvRejMVA/F");
  outtree->Branch("significance_CombConvRejMVA", &significance_CombConvRejMVA, "significance_CombConvRejMVA/F");
  outtree->Branch("significance_singleConvTrackRejMVA", &significance_singleConvTrackRejMVA, "significance_singleConvTrackRejMVA/F");
  outtree->Branch("significance_gain_RPConvRejMVA", &significance_gain_RPConvRejMVA, "significance_gain_RPConvRejMVA/F");
  outtree->Branch("significance_gain_CombConvRejMVA", &significance_gain_CombConvRejMVA, "significance_gain_CombConvRejMVA/F");
  outtree->Branch("significance_gain_singleConvTrackRejMVA", &significance_gain_singleConvTrackRejMVA, "significance_gain_singleConvTrackRejMVA/F");


  TFile *infile_PIDeffs = new TFile(PIDeffs_filename, "READ");
  TH1D *h_PIDeffs = (TH1D*)infile_PIDeffs->Get("efficiencyLHC17d12_TPCandTOF3sigma");
  h_PIDeffs->GetXaxis()->SetRangeUser(0,5);


  
  Long64_t nentries = tree_MCdatafile->GetEntries();
  if(processDataFraction != -1) nentries = nentries*processDataFraction;
  else if(processDataEntries != -1) nentries = processDataEntries;
  
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
  Float_t *MVAout_RPConvRejMVA_all = NULL;
  Float_t *MVAout_CombConvRejMVA_all = NULL;
  Float_t *MVAout_singleConvTrackRejMVA_1_all = NULL;
  Float_t *MVAout_singleConvTrackRejMVA_2_all = NULL;
  
  EventID_all = new Int_t[nentries];
  TrackID1_all = new Int_t[nentries];
  TrackID2_all = new Int_t[nentries];
  IsConv_all = new Int_t[nentries];
  IsRP_all = new Int_t[nentries];
  pt1_all = new Float_t[nentries];
  pt2_all = new Float_t[nentries];
  mass_all = new Float_t[nentries];
  if(containsTrackCutInfo) trackCut1_all = new Int_t[nentries];
  if(containsTrackCutInfo) trackCut2_all = new Int_t[nentries];
  MVAout_RPConvRejMVA_all = new Float_t[nentries];
  MVAout_CombConvRejMVA_all = new Float_t[nentries];
  MVAout_singleConvTrackRejMVA_1_all = new Float_t[nentries];
  MVAout_singleConvTrackRejMVA_2_all = new Float_t[nentries];
  
  for(Long64_t i=0; i<nentries; i++) {
    tree_MCdatafile->GetEntry(i);
    tree_MVAoutput_RPConvRejMVA_file->GetEntry(i);
    tree_MVAoutput_CombConvRejMVA_file->GetEntry(i);
    tree_MVAoutput_singleConvTrackRejMVA_file->GetEntry(i);
    
    EventID_all[i] = EventID;
    TrackID1_all[i] = TrackID1;
    TrackID2_all[i] = TrackID2;
    IsConv_all[i] = IsConv;
    IsRP_all[i] = IsRP;
    pt1_all[i] = pt1;
    pt2_all[i] = pt2;
    mass_all[i] = mass;
    if(containsTrackCutInfo) trackCut1_all[i] = TrackCut1;
    if(containsTrackCutInfo) trackCut2_all[i] = TrackCut2;
    MVAout_RPConvRejMVA_all[i] = MVAout_RPConvRejMVA;
    MVAout_CombConvRejMVA_all[i] = MVAout_CombConvRejMVA;
    MVAout_singleConvTrackRejMVA_1_all[i] = MVAout_singleConvTrackRejMVA_1;
    MVAout_singleConvTrackRejMVA_2_all[i] = MVAout_singleConvTrackRejMVA_2;
  }
  

  // significances in case of (1) ideal and (2) no background rejection:
  Float_t significance_ideal;
  Float_t significance_RPConvRejMVA_all;
  Float_t significance_CombConvRejMVA_all;
  Float_t significance_singleConvTrackRejMVA_all;
  
  
  TStopwatch *watch_overall = new TStopwatch();
  watch_overall->Start();


  for(Int_t scan=0; scan<=num_steps; scan++) {

    TStopwatch *watch_step = new TStopwatch();
    watch_step->Start();
    
    Int_t *tags_RPConvRejMVA = NULL;
    Int_t *tags_CombConvRejMVA = NULL;
    Int_t *tags_singleConvTrackRejMVA = NULL;

    tags_RPConvRejMVA = new Int_t[nentries];
    tags_CombConvRejMVA = new Int_t[nentries];
    tags_singleConvTrackRejMVA = new Int_t[nentries];

    for(Long64_t i=0; i<nentries; i++) {
      tags_RPConvRejMVA[i] = 0;
      tags_CombConvRejMVA[i] = 0;
      tags_singleConvTrackRejMVA[i] = 0;
    }

    MVAcut = scan/(num_steps*1.0)*(MVAoutputRange_max-MVAoutputRange_min) + MVAoutputRange_min;

    std::cout << std::endl << "---------- Step " << scan << " of " << num_steps
              << " (MVA cut: " << MVAcut << "): ----------" << std::endl;

    std::cout << "Reading the data...";


    TRandom *rand = new TRandom();
    
    std::cout << "Tagging pairs and applying the prefilter..."
              << std::endl;
    
    std::cout << "Step 1/3:" << std::endl;

    for(Long64_t j=0; j<nentries; j++) {
      if((j%5000)==0) {
        std::cout << "\r  (" << j << " / " << nentries << ")";
      }

      if( (MVAout_RPConvRejMVA_all[j] < MVAoutputRange_min ||
           MVAout_RPConvRejMVA_all[j] > MVAoutputRange_max) ) {
        tags_RPConvRejMVA[j] = -999;
        continue;
      }

      // loose track cut selection (see also second occurrence below!):
      if(containsTrackCutInfo) {
        if(considerLooseCuts==0 &&
           (trackCut1_all[j]!=2 || trackCut2_all[j]!=2)) {
          tags_RPConvRejMVA[j] = -99;
          continue;
        }else if(considerLooseCuts==1 &&
                 (trackCut1_all==0 || trackCut2_all[j]==0)) {
          tags_RPConvRejMVA[j] = -99;
          continue;
        }
        // else: Only case left is considerLooseCuts==2, since
        // considerLooseCuts>=3 gets already rejected in the beginning of the code.
        // In this case, all tracks are processed, therefore no skipping.
      }

      if(MVAout_RPConvRejMVA_all[j] < MVAcut) {
        tags_RPConvRejMVA[j] = 1;
      }
      
      Int_t EventID_current = EventID_all[j];
      Int_t TrackID1_current = TrackID1_all[j];
      Int_t TrackID2_current = TrackID2_all[j];

      Float_t pairWeight_j = getPairPIDefficiency(pt1_all[j], pt2_all[j], *h_PIDeffs);

      Bool_t doForwardProp = (rand->Uniform() < pairWeight_j) ? kTRUE : kFALSE;

      
      for(Long64_t i=j+1; i<nentries; i++) {
	
        if(EventID_all[i] != EventID_current) break;

        if(MVAout_RPConvRejMVA_all[i] < MVAoutputRange_min ||
           MVAout_RPConvRejMVA_all[i] > MVAoutputRange_max) {
          continue;
        }

        // loose track cut selection (see also occurrence above!):
        if(containsTrackCutInfo) {
          if(considerLooseCuts==0 &&
             (trackCut1_all[i]!=2 || trackCut2_all[i]!=2)) {
            continue;
          }else if(considerLooseCuts==1 &&
                   (trackCut1_all[i]==0 || trackCut2_all[i]==0)) {
            continue;
          }
        }

        // 'forward propagation' of tag information:
        if(doForwardProp &&
           MVAout_RPConvRejMVA_all[j] < MVAcut &&
           (TrackID1_current == TrackID1_all[i] || TrackID2_current == TrackID2_all[i] ||
            TrackID1_current == TrackID2_all[i] || TrackID2_current == TrackID1_all[i])) {
          tags_RPConvRejMVA[i] = 1;
        }

        Float_t pairWeight_i = getPairPIDefficiency(pt1_all[i], pt2_all[i], *h_PIDeffs);

        Bool_t doBackwardProp = (rand->Uniform() < pairWeight_i) ? kTRUE : kFALSE;

        // 'backward propagation' of tag information:
        if(doBackwardProp &&
           MVAout_RPConvRejMVA_all[i] < MVAcut &&
           (TrackID1_current == TrackID1_all[i] || TrackID2_current == TrackID2_all[i] ||
            TrackID1_current == TrackID2_all[i] || TrackID2_current == TrackID1_all[i])) {
          tags_RPConvRejMVA[j] = 1;
        }
      }
    }
    std::cout << std::endl;
    

    std::cout << "Step 2/3:" << std::endl;

    for(Long64_t j=0; j<nentries; j++) {
      if((j%5000)==0) {
        std::cout << "\r  (" << j << " / " << nentries << ")";
      }

      if( (MVAout_CombConvRejMVA_all[j] < MVAoutputRange_min ||
           MVAout_CombConvRejMVA_all[j] > MVAoutputRange_max) ) {
        tags_CombConvRejMVA[j] = -999;
        continue;
      }

      if(containsTrackCutInfo && considerLooseCuts==0 &&
         (trackCut1_all[j]!=2 || trackCut2_all[j]!=2)) {
        tags_CombConvRejMVA[j] = -99;
        continue;
      }

      if(containsTrackCutInfo && considerLooseCuts==1 &&
         (trackCut1_all[j]==0 || trackCut2_all[j]==0)) {
        tags_CombConvRejMVA[j] = -99;
        continue;
      }

      if(MVAout_CombConvRejMVA_all[j] < MVAcut) {
        tags_CombConvRejMVA[j] = 1;
      }
    }
    std::cout << std::endl;

    
    std::cout << "Step 3/3:" << std::endl;

    for(Long64_t j=0; j<nentries; j++) {
      if((j%5000)==0) {
        std::cout << "\r  (" << j << " / " << nentries << ")";
      }

      if( (MVAout_singleConvTrackRejMVA_1_all[j] < MVAoutputRange_min ||
           MVAout_singleConvTrackRejMVA_2_all[j] < MVAoutputRange_min ||
           MVAout_singleConvTrackRejMVA_1_all[j] > MVAoutputRange_max ||
           MVAout_singleConvTrackRejMVA_2_all[j] > MVAoutputRange_max) ) {
        tags_singleConvTrackRejMVA[j] = -999;
        continue;
      }

      if(containsTrackCutInfo && considerLooseCuts==0 &&
         (trackCut1_all[j]!=2 || trackCut2_all[j]!=2)) {
        tags_singleConvTrackRejMVA[j] = -99;
        continue;
      }

      if(containsTrackCutInfo && considerLooseCuts==1 &&
         (trackCut1_all[j]==0 || trackCut2_all[j]==0)) {
        tags_singleConvTrackRejMVA[j] = -99;
        continue;
      }

      if((MVAout_singleConvTrackRejMVA_1_all[j] < MVAcut ||
          MVAout_singleConvTrackRejMVA_2_all[j] < MVAcut)) {
        tags_singleConvTrackRejMVA[j] = 1;
      }
    }



    // determine significance in case of (1) ideal and (2) no background rejection:
    if(scan==0) {

      Float_t num_S_ideal_temp=0., num_B_ideal_temp=0.;
      Float_t num_S_ideal_singleConvTrackRejMVA_temp=0., num_B_ideal_singleConvTrackRejMVA_temp=0.;
      
      Float_t num_S_RPConvRejMVA_temp=0., num_B_RPConvRejMVA_temp=0.;
      Float_t num_S_CombConvRejMVA_temp=0., num_B_CombConvRejMVA_temp=0.;
      Float_t num_S_singleConvTrackRejMVA_temp=0., num_B_singleConvTrackRejMVA_temp=0.;

      for(Long64_t i=0; i<nentries; i++) {

        if( mass_all[i] < massCut ) {
          continue;
        }

        if(containsTrackCutInfo && considerLooseCuts==0 &&
           (trackCut1_all[i]!=2 || trackCut2_all[i]!=2)) {
          continue;
        }

        if(containsTrackCutInfo && considerLooseCuts==1 &&
           (trackCut1_all[i]==0 || trackCut2_all[i]==0)) {
          continue;
        }
	
        Float_t pairweight_temp = getPairPIDefficiency(pt1_all[i], pt2_all[i], *h_PIDeffs);

        if(IsConv_all[i]==0) {
          num_S_ideal_temp += pairweight_temp;
        }else {
          num_B_ideal_temp += pairweight_temp;
        }

        if(tags_RPConvRejMVA[i]==0 && IsConv_all[i]==0) num_S_RPConvRejMVA_temp += pairweight_temp;
        if(tags_RPConvRejMVA[i]==0 && IsConv_all[i]==1) num_B_RPConvRejMVA_temp += pairweight_temp;

        if(tags_CombConvRejMVA[i]==0 && IsConv_all[i]==0) num_S_CombConvRejMVA_temp += pairweight_temp;
        if(tags_CombConvRejMVA[i]==0 && IsConv_all[i]==1) num_B_CombConvRejMVA_temp += pairweight_temp;

        if(tags_singleConvTrackRejMVA[i]==0 && IsConv_all[i]==0) num_S_singleConvTrackRejMVA_temp += pairweight_temp;
        if(tags_singleConvTrackRejMVA[i]==0 && IsConv_all[i]==1) num_B_singleConvTrackRejMVA_temp += pairweight_temp;
      }


      if(num_S_ideal_temp!=0) {
        significance_ideal = num_S_ideal_temp/TMath::Sqrt(num_S_ideal_temp);
      }else significance_ideal = 0.;

      if(num_S_RPConvRejMVA_temp+num_B_RPConvRejMVA_temp>0) {
        significance_RPConvRejMVA_all = num_S_RPConvRejMVA_temp/TMath::Sqrt(num_S_RPConvRejMVA_temp+num_B_RPConvRejMVA_temp);
      }else significance_RPConvRejMVA_all = 0.;

      if(num_S_CombConvRejMVA_temp+num_B_CombConvRejMVA_temp>0) {
        significance_CombConvRejMVA_all = num_S_CombConvRejMVA_temp/TMath::Sqrt(num_S_CombConvRejMVA_temp+num_B_CombConvRejMVA_temp);
      }else significance_CombConvRejMVA_all = 0.;

      if(num_S_singleConvTrackRejMVA_temp+num_B_singleConvTrackRejMVA_temp>0) {
        significance_singleConvTrackRejMVA_all = num_S_singleConvTrackRejMVA_temp/TMath::Sqrt(num_S_singleConvTrackRejMVA_temp+num_B_singleConvTrackRejMVA_temp);
      }else significance_singleConvTrackRejMVA_all = 0.;

      //testing
      std::cout << " Ideal significance: " << significance_ideal << std::endl
                << " Significance (no backgr. rej., RPConvRejMVA): " << significance_RPConvRejMVA_all << std::endl
                << " Significance (no backgr. rej., CombConvRejMVA): " << significance_CombConvRejMVA_all << std::endl
                << " Significance (no backgr. rej., singleConvTrackRejMVA): " << significance_singleConvTrackRejMVA_all << std::endl;

    }
      

    Float_t num_totalConvs = 0, num_totalNonConvs = 0;
    for(Long64_t i=0; i<nentries; i++) {

      if( mass_all[i] < massCut ) {
        continue;
      }
      
      if(containsTrackCutInfo && considerLooseCuts==0 &&
         (trackCut1_all[i]!=2 || trackCut2_all[i]!=2)) {
        continue;
      }
      
      if(containsTrackCutInfo && considerLooseCuts==1 &&
         (trackCut1_all[i]==0 || trackCut2_all[i]==0)) {
        continue;
      }
      
      if(IsConv_all[i]==1) num_totalConvs += getPairPIDefficiency(pt1_all[i], pt2_all[i], *h_PIDeffs);
      else num_totalNonConvs += getPairPIDefficiency(pt1_all[i], pt2_all[i], *h_PIDeffs);
    }

    Float_t num_S_RPConvRejMVA=0., num_B_RPConvRejMVA=0.;
    Float_t num_S_CombConvRejMVA=0., num_B_CombConvRejMVA=0.;
    Float_t num_S_singleConvTrackRejMVA=0., num_B_singleConvTrackRejMVA=0.;

    for(Long64_t i=0; i<nentries; i++) {

      if( mass_all[i] < massCut ) {
        continue;
      }

      if(containsTrackCutInfo && considerLooseCuts==0 &&
         (trackCut1_all[i]!=2 || trackCut2_all[i]!=2)) {
        continue;
      }

      if(containsTrackCutInfo && considerLooseCuts==1 &&
         (trackCut1_all[i]==0 || trackCut2_all[i]==0)) {
        continue;
      }
      
      Float_t pairweight = getPairPIDefficiency(pt1_all[i], pt2_all[i], *h_PIDeffs);

      if(tags_RPConvRejMVA[i]==0 && IsConv_all[i]==0) num_S_RPConvRejMVA += pairweight;
      if(tags_RPConvRejMVA[i]==0 && IsConv_all[i]==1) num_B_RPConvRejMVA += pairweight;

      if(tags_CombConvRejMVA[i]==0 && IsConv_all[i]==0) num_S_CombConvRejMVA += pairweight;
      if(tags_CombConvRejMVA[i]==0 && IsConv_all[i]==1) num_B_CombConvRejMVA += pairweight;

      if(tags_singleConvTrackRejMVA[i]==0 && IsConv_all[i]==0) num_S_singleConvTrackRejMVA += pairweight;
      if(tags_singleConvTrackRejMVA[i]==0 && IsConv_all[i]==1) num_B_singleConvTrackRejMVA += pairweight;
	
    }

    Seff_RPConvRejMVA = num_S_RPConvRejMVA / num_totalNonConvs;
    Beff_RPConvRejMVA = num_B_RPConvRejMVA / num_totalConvs;
    if(num_S_RPConvRejMVA+num_B_RPConvRejMVA>0) {
      significance_RPConvRejMVA = num_S_RPConvRejMVA/TMath::Sqrt(num_S_RPConvRejMVA+num_B_RPConvRejMVA);
    }else significance_RPConvRejMVA = 0.;

    Seff_CombConvRejMVA = num_S_CombConvRejMVA / num_totalNonConvs;
    Beff_CombConvRejMVA = num_B_CombConvRejMVA / num_totalConvs;
    if(num_S_CombConvRejMVA+num_B_CombConvRejMVA>0) {
      significance_CombConvRejMVA = num_S_CombConvRejMVA/TMath::Sqrt(num_S_CombConvRejMVA+num_B_CombConvRejMVA);
    }else significance_CombConvRejMVA = 0.;

    Seff_singleConvTrackRejMVA = num_S_singleConvTrackRejMVA / num_totalNonConvs;
    Beff_singleConvTrackRejMVA = num_B_singleConvTrackRejMVA / num_totalConvs;
    if(num_S_singleConvTrackRejMVA+num_B_singleConvTrackRejMVA>0) {
      significance_singleConvTrackRejMVA = num_S_singleConvTrackRejMVA/TMath::Sqrt(num_S_singleConvTrackRejMVA+num_B_singleConvTrackRejMVA);
    }else significance_singleConvTrackRejMVA = 0.;

      
    if(significance_ideal-significance_RPConvRejMVA_all != 0) {
      significance_gain_RPConvRejMVA = (significance_RPConvRejMVA-significance_RPConvRejMVA_all) / (significance_ideal-significance_RPConvRejMVA_all);
    }else significance_gain_RPConvRejMVA = 0.;

    if(significance_ideal-significance_CombConvRejMVA_all != 0) {
      significance_gain_CombConvRejMVA = (significance_CombConvRejMVA-significance_CombConvRejMVA_all) / (significance_ideal-significance_CombConvRejMVA_all);
    }else significance_gain_CombConvRejMVA = 0.;

    if(significance_ideal-significance_singleConvTrackRejMVA_all != 0) {
      significance_gain_singleConvTrackRejMVA = (significance_singleConvTrackRejMVA-significance_singleConvTrackRejMVA_all) / (significance_ideal-significance_singleConvTrackRejMVA_all);
    }else significance_gain_singleConvTrackRejMVA = 0.;



    outtree->Fill();

    std::cout << " DONE" << std::endl;
    std::cout << "  S_eff_RPConvRejMVA = " << Seff_RPConvRejMVA << ",\tB_eff_RPConvRejMVA = " << Beff_RPConvRejMVA
              << ",\tsignificance_RPConvRejMVA = " << significance_RPConvRejMVA << std::endl;
    std::cout << "  S_eff_CombConvRejMVA = " << Seff_CombConvRejMVA << ",\tB_eff_CombConvRejMVA = " << Beff_CombConvRejMVA
              << ",\tsignificance_CombConvRejMVA = " << significance_CombConvRejMVA << std::endl;
    std::cout << "  S_eff_singleConvTrackRejMVA = " << Seff_singleConvTrackRejMVA << ",\tB_eff_singleConvTrackRejMVA = " << Beff_singleConvTrackRejMVA
              << ",\tsignificance_singleConvTrackRej = " << significance_singleConvTrackRejMVA << std::endl;
    std::cout << "  significance_gain_RPConvRejMVA = " << significance_gain_RPConvRejMVA << std::endl;
    std::cout << "  significance_gain_CombConvRejMVA = " << significance_gain_CombConvRejMVA << std::endl;
    std::cout << "  significance_gain_singleConvTrackRejMVA = " << significance_gain_singleConvTrackRejMVA << std::endl;

    std::cout << std::endl << "Time elapsed: " << watch_step->RealTime()
              << " seconds." << std::endl;


    delete [] tags_RPConvRejMVA;
    delete [] tags_CombConvRejMVA;
    delete [] tags_singleConvTrackRejMVA;

    tags_RPConvRejMVA = NULL;
    tags_CombConvRejMVA = NULL;
    tags_singleConvTrackRejMVA = NULL;
  }
    


  std::cout << "Write data...";
  outfile->cd();
  outfile->Write("", TObject::kOverwrite);
  std::cout << " DONE." << std::endl;
  std::cout << "File " << outfile->GetName() << " created." << std::endl;



  MCdatafile->Close();
  MVAoutput_RPConvRejMVA_file->Close();
  MVAoutput_CombConvRejMVA_file->Close();
  MVAoutput_singleConvTrackRejMVA_file->Close();

  delete [] EventID_all;
  delete [] TrackID1_all;
  delete [] TrackID2_all;
  delete [] IsConv_all;
  delete [] IsRP_all;
  delete [] pt1_all;
  delete [] pt2_all;
  delete [] mass_all;
  if(containsTrackCutInfo) delete [] trackCut1_all;
  if(containsTrackCutInfo) delete [] trackCut2_all;
  delete [] MVAout_RPConvRejMVA_all;
  delete [] MVAout_CombConvRejMVA_all;
  delete [] MVAout_singleConvTrackRejMVA_1_all;
  delete [] MVAout_singleConvTrackRejMVA_2_all;

  EventID_all = NULL;
  TrackID1_all = NULL;
  TrackID2_all = NULL;
  IsConv_all = NULL;
  IsRP_all = NULL;
  pt1_all = NULL;
  pt2_all = NULL;
  mass_all = NULL;
  if(containsTrackCutInfo) trackCut1_all = NULL;
  if(containsTrackCutInfo) trackCut2_all = NULL;
  MVAout_RPConvRejMVA_all = NULL;
  MVAout_CombConvRejMVA_all = NULL;
  MVAout_singleConvTrackRejMVA_1_all = NULL;
  MVAout_singleConvTrackRejMVA_2_all = NULL;


  gSystem->Exit(0);
}



Float_t getPairPIDefficiency(Float_t pt1, Float_t pt2, TH1D &h_PIDeff) {

  Float_t PIDeff1 = (pt1 >= h_PIDeff.GetBinLowEdge(h_PIDeff.GetNbinsX())) ?
    h_PIDeff.GetBinContent(h_PIDeff.GetNbinsX()) : h_PIDeff.GetBinContent(h_PIDeff.FindBin(pt1));

  Float_t PIDeff2 = (pt2 >= h_PIDeff.GetBinLowEdge(h_PIDeff.GetNbinsX())) ?
    h_PIDeff.GetBinContent(h_PIDeff.GetNbinsX()) : h_PIDeff.GetBinContent(h_PIDeff.FindBin(pt2));

  return PIDeff1 * PIDeff2;
}
