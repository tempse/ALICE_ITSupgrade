#include <iostream>

#include <TROOT.h>
#include <TMath.h>
#include <TApplication.h>
#include <TFile.h>
#include <TTree.h>
#include <TStopwatch.h>
#include <TH1F.h>
#include <TCanvas.h>

void addPrefilterTagBranch_classicalCuts(TString updatefilename,
					 TString treename_updatefile,
					 TString branchname_read,
					 TString branchname_add,
					 TString signalRegion = "+") {
  
  Int_t input_tag;
  Int_t isAccepted;

  Float_t input_tag_min = 0.;
  Float_t input_tag_max = 1.;
  
  std::cout << "Reading file...";
  TFile *updatefile = new TFile(updatefilename, "UPDATE");
  TTree *tree_updatefile = (TTree*)updatefile->Get(treename_updatefile);
  Int_t EventID;
  Int_t TrackID1, TrackID2;
  tree_updatefile->SetBranchAddress("EventID1", &EventID);
  tree_updatefile->SetBranchAddress("TrackID1", &TrackID1);
  tree_updatefile->SetBranchAddress("TrackID2", &TrackID2);
  tree_updatefile->SetBranchAddress(branchname_read, &input_tag);
  if(tree_updatefile->GetListOfBranches()->FindObject(branchname_add) != NULL) {
    std::cout << "  ERROR: A branch named " << branchname_add
	      << " already exists in file " << updatefile->GetName() << std::endl;
    gApplication->Terminate();
  }
  std::cout << " DONE." << std::endl;
  
  TString branchname_add_vartype = branchname_add + "/I";
  TBranch *newBranch = tree_updatefile->Branch(branchname_add,
					       &isAccepted,
					       branchname_add_vartype);

  
  const Long64_t nentries = tree_updatefile->GetEntries();

  Int_t *EventID_all = NULL;
  Int_t *TrackID1_all = NULL;
  Int_t *TrackID2_all = NULL;
  Float_t *input_tag_all = NULL;
  Int_t *tags_prefilter = NULL;
  
  EventID_all = new Int_t[nentries];
  TrackID1_all = new Int_t[nentries];
  TrackID2_all = new Int_t[nentries];
  input_tag_all = new Float_t[nentries];
  tags_prefilter = new Int_t[nentries];
  
  for(Long64_t i=0; i<nentries; i++) {
    EventID_all[i] = 0;
    TrackID1_all[i] = 0;
    TrackID2_all[i] = 0;
    input_tag_all[i] = 0.;
    tags_prefilter[i] = 0;
  }

  std::cout << "Reading the data into memory..." << std::endl;
  
  for(Long64_t i=0; i<nentries; i++) {
    if((i%5000)==0) std::cout << "\r  (" << i << " / " << nentries << ")";
    
    tree_updatefile->GetEntry(i);
    
    EventID_all[i] = EventID;
    TrackID1_all[i] = TrackID1;
    TrackID2_all[i] = TrackID2;
    input_tag_all[i] = input_tag;
  }
  std::cout << "\r  (" << nentries << " / " << nentries << ")" << std::endl;


  Int_t EventID_prev = -1;

  Int_t accTracks_startPos = 0, accTracks_nextStartPos;

  
  std::cout << "Tagging pairs and applying the prefilter..." << std::endl;


  for(Long64_t j=0; j<nentries; j++) {

    if((j%1000)==0) std::cout << "\r  (" << j << " / " << nentries << ")";

    if( input_tag_all[j] < input_tag_min ||
	input_tag_all[j] > input_tag_max ) continue;

    if(signalRegion == "+" && input_tag_all[j] == 1) {
      tags_prefilter[j] = 1;
    }else if(signalRegion == "-" && input_tag_all[j] == 0) {
      tags_prefilter[j] = 1;
    }

    Int_t EventID_current = EventID_all[j];
    Int_t TrackID1_current = TrackID1_all[j];
    Int_t TrackID2_current = TrackID2_all[j];

    for(Int_t i=j; i<nentries; i++) {

      if(EventID_all[i] != EventID_current) break;

      if(tags_prefilter[j] == 1) tags_prefilter[i] = 1;

      if(signalRegion == "+" && input_tag_all[i] == 1 &&
	 (TrackID1_current == TrackID1_all[i] || TrackID2_current == TrackID2_all[i] ||
	  TrackID1_current == TrackID2_all[i] || TrackID2_current == TrackID1_all[i])) {
	tags_prefilter[j] = 1;
      }else if(signalRegion == "-" && input_tag_all[i] == 0 &&
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

  delete [] EventID_all;
  delete [] TrackID1_all;
  delete [] TrackID2_all;
  delete [] input_tag_all;
  delete [] tags_prefilter;

  EventID_all = NULL;
  TrackID1_all = NULL;
  TrackID2_all = NULL;
  input_tag_all = NULL;
  tags_prefilter = NULL;
  
  gSystem->Exit(0);
  
}
