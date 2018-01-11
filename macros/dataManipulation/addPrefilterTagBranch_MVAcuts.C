#include <iostream>
#include <algorithm>

#include <TROOT.h>
#include <TMath.h>
#include <TSystem.h>
#include <TApplication.h>
#include <TFile.h>
#include <TTree.h>
#include <TRandom.h>
#include <TStopwatch.h>


Float_t getPairPIDefficiency(Float_t, Float_t, TH1D&);

void addPrefilterTagBranch_MVAcuts(TString branchfilename,
                                   TString treename_branchfile,
                                   TString branchname_branchfile,
                                   TString updatefilename,
                                   TString treename_updatefile,
                                   TString branchname_updatefile,
                                   Float_t MVAcut,
                                   TString signalRegion = "-",
                                   UInt_t  considerLooseCuts = 0, // 0...no loose cuts, 1...with loose cuts, 2...all tracks
                                   TString PIDefficiencies_fileName = "/home/sebastian/analysis/data/finalAnalysis_FT2/inputdata/ITSU_PIDefficiency_lowB.root") {

  Float_t MVAoutput;
  Int_t isAccepted;

  Float_t MVAoutputRange_min = 0.;
  Float_t MVAoutputRange_max = 1.;
  

  // File containing the pt-dependent PID efficiencies:
  TFile *infile_PIDefficiencies = new TFile(PIDefficiencies_fileName, "READ");
  TH1D *h_PIDeff = (TH1D*)infile_PIDefficiencies->Get("efficiencyLHC17d12_TPCandTOF3sigma");
  h_PIDeff->GetXaxis()->SetRangeUser(0,5);

  TH1D *h_sample_weight = new TH1D("h_sample_weight","",1000,0,1);
  
  
  std::cout << "Reading file...";
  TFile *updatefile = new TFile(updatefilename, "UPDATE");
  TTree *tree_updatefile = (TTree*)updatefile->Get(treename_updatefile);
  Long64_t EventID;
  Int_t TrackID1, TrackID2;
  Float_t pt1, pt2;
  Bool_t containsTrackCutInfo = kTRUE;
  Int_t TrackCut1, TrackCut2;
  tree_updatefile->SetBranchAddress("EventID1_unique", &EventID);
  tree_updatefile->SetBranchAddress("TrackID1", &TrackID1);
  tree_updatefile->SetBranchAddress("TrackID2", &TrackID2);
  tree_updatefile->SetBranchAddress("pt1", &pt1);
  tree_updatefile->SetBranchAddress("pt2", &pt2);
  if(tree_updatefile->GetListOfBranches()->FindObject("TrackCut1") != NULL &&
     tree_updatefile->GetListOfBranches()->FindObject("TrackCut2") != NULL) {
    tree_updatefile->SetBranchAddress("TrackCut1", &TrackCut1);
    tree_updatefile->SetBranchAddress("TrackCut2", &TrackCut2);
  }else {
    std::cout << "  Info: No branch holding track cut information found. "
              << "All tracks will be processed." << std::endl;
    containsTrackCutInfo = kFALSE;
  }
  if(tree_updatefile->GetListOfBranches()->FindObject(branchname_updatefile) != NULL) {
    std::cout << "  ERROR: A branch named " << branchname_updatefile
              << " already exists in file " << updatefile->GetName() << std::endl;
    gApplication->Terminate();
  }
  std::cout << " DONE." << std::endl;
  
  TString branchname_updatefile_vartype = branchname_updatefile + "/I";
  TBranch *newBranch = tree_updatefile->Branch(branchname_updatefile,
                                               &isAccepted,
                                               branchname_updatefile_vartype);


  if(!containsTrackCutInfo && considerLooseCuts>0) {
    std::cout << "  ERROR: Loose track cuts have been chosen to be considered, but do not exist "
              << "in the file. Exiting without changing the files..." << std::endl;
    updatefile->Close();
    gSystem->Exit(1);
  }

  if(containsTrackCutInfo && considerLooseCuts>=3) {
    std::cout << "  ERROR: considerLooseCuts>=2. Exiting without changing the files..." << std::endl;
    updatefile->Close();
    gSystem->Exit(1);
  }
  
  
  TFile *branchfile = new TFile(branchfilename, "READ");
  TTree *tree_branchfile = (TTree*)branchfile->Get(treename_branchfile);
  tree_branchfile->SetBranchAddress(branchname_branchfile, &MVAoutput);
  

  if(tree_updatefile->GetEntries() != tree_branchfile->GetEntries()) {
    std::cout << "  ERROR: Trees have different sizes." << std::endl
              << "    Size of tree " << treename_updatefile << " in "
              << updatefilename << ": " << tree_updatefile->GetEntries()
              << std::endl
              << "    Size of tree " << treename_branchfile << " in "
              << branchfilename << ": " << tree_branchfile->GetEntries()
              << std::endl << std::endl;
    gApplication->Terminate();
  }


  // TFile *outfile = new TFile("output_prefilterTagBranchMVA.root","RECREATE");
  // TTree *tree_outfile = new TTree("prefilterTags","prefilterTags");
  // tree_outfile->Branch(branchname_updatefile, &isAccepted, branchname_updatefile + "/I");

  
  const Long64_t nentries = tree_updatefile->GetEntries();

  Int_t *EventID_all = NULL;
  Int_t *TrackID1_all = NULL;
  Int_t *TrackID2_all = NULL;
  Int_t *TrackCut1_all = NULL;
  Int_t *TrackCut2_all = NULL;
  Float_t *pt1_all = NULL;
  Float_t *pt2_all = NULL;
  Float_t *MVAoutput_all = NULL;
  Int_t *tags_prefilter = NULL;
  
  EventID_all = new Int_t[nentries];
  TrackID1_all = new Int_t[nentries];
  TrackID2_all = new Int_t[nentries];
  if(containsTrackCutInfo) TrackCut1_all = new Int_t[nentries];
  if(containsTrackCutInfo) TrackCut2_all = new Int_t[nentries];
  pt1_all = new Float_t[nentries];
  pt2_all = new Float_t[nentries];
  MVAoutput_all = new Float_t[nentries];
  tags_prefilter = new Int_t[nentries];
  
  for(Long64_t i=0; i<nentries; i++) {
    EventID_all[i] = 0;
    TrackID1_all[i] = 0;
    TrackID2_all[i] = 0;
    if(containsTrackCutInfo) TrackCut1_all[i] = 0;
    if(containsTrackCutInfo) TrackCut2_all[i] = 0;
    pt1_all[i] = 0.;
    pt2_all[i] = 0.;
    MVAoutput_all[i] = 0.;
    tags_prefilter[i] = 0;
  }

  std::cout << "Reading the data into memory..." << std::endl;
  
  for(Long64_t i=0; i<nentries; i++) {
    if((i%5000)==0) std::cout << "\r  (" << i << " / " << nentries << ")";
    
    tree_updatefile->GetEntry(i);
    tree_branchfile->GetEntry(i);
    
    EventID_all[i] = EventID;
    TrackID1_all[i] = TrackID1;
    TrackID2_all[i] = TrackID2;
    if(containsTrackCutInfo) TrackCut1_all[i] = TrackCut1;
    if(containsTrackCutInfo) TrackCut2_all[i] = TrackCut2;
    pt1_all[i] = pt1;
    pt2_all[i] = pt2;
    MVAoutput_all[i] = MVAoutput;
  }
  std::cout << "\r  (" << nentries << " / " << nentries << ")" << std::endl;


  TRandom *rand = new TRandom();
  
  
  std::cout << "Tagging pairs and applying the prefilter..." << std::endl;


  for(Long64_t j=0; j<nentries; j++) {

    if((j%1000)==0) std::cout << "\r  (" << j << " / " << nentries << ")";
    
    if( MVAoutput_all[j] < MVAoutputRange_min ||
        MVAoutput_all[j] > MVAoutputRange_max ) {
      tags_prefilter[j] = -999;
      continue;
    }

    // loose track cut selection (see also second occurrence below!):
    if(containsTrackCutInfo) {
      if(considerLooseCuts==0 &&
         (TrackCut1_all[j]!=2 || TrackCut2_all[j]!=2)) {
        tags_prefilter[j] = -99;
        continue;
      }else if(considerLooseCuts==1 &&
               (TrackCut1_all[j]==0 || TrackCut2_all[j]==0)) {
        tags_prefilter[j] = -99;
        continue;
      }
      // else: Only case left is considerLooseCuts==2, since
      // considerLooseCuts>=3 gets already rejected in the beginning of the code.
      // In this case, all tracks are processed, therefore no skipping.
    }

    if(signalRegion == "+" && MVAoutput_all[j] > MVAcut) {
      tags_prefilter[j] = 1;
    }else if(signalRegion == "-" && MVAoutput_all[j] < MVAcut) {
      tags_prefilter[j] = 1;
    }

    Int_t EventID_current = EventID_all[j];
    Int_t TrackID1_current = TrackID1_all[j];
    Int_t TrackID2_current = TrackID2_all[j];

    Float_t pairWeight_j = getPairPIDefficiency(pt1_all[j], pt2_all[j], *h_PIDeff);

    Bool_t doForwardProp = (rand->Uniform() < pairWeight_j) ? kTRUE : kFALSE;
    

    for(Int_t i=j+1; i<nentries; i++) {

      if(EventID_all[i] != EventID_current) break;

      if( MVAoutput_all[i] < MVAoutputRange_min ||
          MVAoutput_all[i] > MVAoutputRange_max ) {
        continue;
      }

      // loose track cut selection (see also occurrence above!):
      if(containsTrackCutInfo) {
        if(considerLooseCuts==0 &&
           (TrackCut1_all[i]!=2 || TrackCut2_all[i]!=2)) {
          continue;
        }else if(considerLooseCuts==1 &&
                 (TrackCut1_all[i]==0 || TrackCut2_all[i]==0)) {
          continue;
        }
      }

      // 'forward propagation' of tag information:
      if(doForwardProp &&
         ((signalRegion == "+" && MVAoutput_all[j] > MVAcut) ||
          (signalRegion == "-" && MVAoutput_all[j] < MVAcut)) &&
         (TrackID1_current == TrackID1_all[i] || TrackID2_current == TrackID2_all[i] ||
          TrackID1_current == TrackID2_all[i] || TrackID2_current == TrackID1_all[i])) {
        tags_prefilter[i] = 1;
      }

      Float_t pairWeight_i = getPairPIDefficiency(pt1_all[i], pt2_all[i], *h_PIDeff);

      Bool_t doBackwardProp = (rand->Uniform() < pairWeight_i) ? kTRUE : kFALSE;
      
      // 'backward propagation' of tag information:
      if(doBackwardProp &&
         ((signalRegion == "+" && MVAoutput_all[i] > MVAcut) ||
          (signalRegion == "-" && MVAoutput_all[i] < MVAcut)) &&
         (TrackID1_current == TrackID1_all[i] || TrackID2_current == TrackID2_all[i] ||
          TrackID1_current == TrackID2_all[i] || TrackID2_current == TrackID1_all[i])) {
        tags_prefilter[j] = 1;
      }
    }

  }
  std::cout << "\r  (" << nentries << " / " << nentries << ")" << std::endl;


  std::cout << "Fill new branch with appropriate tags...";
  for(Long64_t i=0; i<nentries; i++) {
    isAccepted = tags_prefilter[i];
    newBranch->Fill();
  }
  std::cout << " DONE" << std::endl << std::endl;

  
  updatefile->cd();
  std::cout << "Write data...";
  tree_updatefile->Write("", TObject::kOverwrite);
  std::cout << " DONE." << std::endl;
  std::cout << "File " << updatefile->GetName() << " updated." << std::endl;

  updatefile->Close();
  branchfile->Close();

  delete [] EventID_all;
  delete [] TrackID1_all;
  delete [] TrackID2_all;
  if(containsTrackCutInfo) delete [] TrackCut1_all;
  if(containsTrackCutInfo) delete [] TrackCut2_all;
  delete [] MVAoutput_all;
  delete [] tags_prefilter;

  EventID_all = NULL;
  TrackID1_all = NULL;
  TrackID2_all = NULL;
  if(containsTrackCutInfo) TrackCut1_all = NULL;
  if(containsTrackCutInfo) TrackCut2_all = NULL;
  MVAoutput_all = NULL;
  tags_prefilter = NULL;
  
  gSystem->Exit(0);
  
}


Float_t getPairPIDefficiency(Float_t pt1, Float_t pt2, TH1D &h_PIDeff) {

  Float_t PIDeff1 = (pt1 >= h_PIDeff.GetBinLowEdge(h_PIDeff.GetNbinsX())) ?
    h_PIDeff.GetBinContent(h_PIDeff.GetNbinsX()) : h_PIDeff.GetBinContent(h_PIDeff.FindBin(pt1));

  Float_t PIDeff2 = (pt2 >= h_PIDeff.GetBinLowEdge(h_PIDeff.GetNbinsX())) ?
    h_PIDeff.GetBinContent(h_PIDeff.GetNbinsX()) : h_PIDeff.GetBinContent(h_PIDeff.FindBin(pt2));

  return PIDeff1 * PIDeff2;
  
}
