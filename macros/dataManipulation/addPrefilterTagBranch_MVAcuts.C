#include <iostream>
#include <algorithm>

#include <TROOT.h>
#include <TMath.h>
#include <TApplication.h>
#include <TFile.h>
#include <TTree.h>
#include <TStopwatch.h>

void addPrefilterTagBranch_MVAcuts(TString branchfilename,
				   TString treename_branchfile,
				   TString branchname_branchfile,
				   TString updatefilename,
				   TString treename_updatefile,
				   TString branchname_updatefile,
				   Float_t MVAcut,
				   TString signalRegion = "+") {

  Float_t MVAoutput;
  Int_t isAccepted;

  Float_t MVAoutputRange_min = 0.;
  Float_t MVAoutputRange_max = 1.;
  

  std::cout << "Reading file...";
  TFile *updatefile = new TFile(updatefilename, "UPDATE");
  TTree *tree_updatefile = (TTree*)updatefile->Get(treename_updatefile);
  Long64_t EventID;
  Int_t TrackID1, TrackID2;
  Bool_t containsTrackCutInfo = kTRUE;
  Int_t TrackCut1, TrackCut2;
  tree_updatefile->SetBranchAddress("EventID1_unique", &EventID);
  tree_updatefile->SetBranchAddress("TrackID1", &TrackID1);
  tree_updatefile->SetBranchAddress("TrackID2", &TrackID2);
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
  Float_t *MVAoutput_all = NULL;
  Int_t *tags_prefilter = NULL;
  
  EventID_all = new Int_t[nentries];
  TrackID1_all = new Int_t[nentries];
  TrackID2_all = new Int_t[nentries];
  if(containsTrackCutInfo) TrackCut1_all = new Int_t[nentries];
  if(containsTrackCutInfo) TrackCut2_all = new Int_t[nentries];
  MVAoutput_all = new Float_t[nentries];
  tags_prefilter = new Int_t[nentries];
  
  for(Long64_t i=0; i<nentries; i++) {
    EventID_all[i] = 0;
    TrackID1_all[i] = 0;
    TrackID2_all[i] = 0;
    if(containsTrackCutInfo) TrackCut1_all[i] = 0;
    if(containsTrackCutInfo) TrackCut2_all[i] = 0;
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
    MVAoutput_all[i] = MVAoutput;
  }
  std::cout << "(" << nentries << " / " << nentries << ")" << std::endl;
  

  Int_t EventID_prev = -1;

  Int_t accTracks_startPos = 0, accTracks_nextStartPos;

  
  std::cout << "Tagging pairs and applying the prefilter..." << std::endl;


  for(Long64_t j=0; j<nentries; j++) {

    if((j%1000)==0) std::cout << "\r  (" << j << " / " << nentries << ")";

    if( MVAoutput_all[j] < MVAoutputRange_min ||
	MVAoutput_all[j] > MVAoutputRange_max ) continue;

    if(signalRegion == "+" && MVAoutput_all[j] >= MVAcut) {
      tags_prefilter[j] = 1;
    }else if(signalRegion == "-" && MVAoutput_all[j] <= MVAcut) {
      tags_prefilter[j] = 1;
    }

    Int_t EventID_current = EventID_all[j];
    Int_t TrackID1_current = TrackID1_all[j];
    Int_t TrackID2_current = TrackID2_all[j];

    for(Int_t i=j; i<nentries; i++) {

      if(EventID_all[i] != EventID_current) break;

      if(tags_prefilter[j] == 1) tags_prefilter[i] = 1;

      if(signalRegion == "+" && MVAoutput_all[i] >= MVAcut &&
	 (TrackID1_current == TrackID1_all[i] || TrackID2_current == TrackID2_all[i] ||
	  TrackID1_current == TrackID2_all[i] || TrackID2_current == TrackID1_all[i])) {
	tags_prefilter[j] = 1;
      }else if(signalRegion == "-" && MVAoutput_all[i] <= MVAcut &&
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
