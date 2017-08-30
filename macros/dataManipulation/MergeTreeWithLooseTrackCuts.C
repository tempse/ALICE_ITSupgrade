#include <TSystem.h>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TList.h>
#include <TStopwatch.h>

void MergeTreeWithLooseTrackCuts(TString filename_normalTrackCuts,
				 TString treename_normalTrackCuts,
				 TString filename_looseTrackCuts,
				 TString treename_looseTrackCuts,
				 TString filename_output="output_mergedTree.root",
				 TString treename_mergedTree="tracks") {


  TStopwatch *watch = new TStopwatch();
  watch->Start();
  
  TFile *f_normalCuts = new TFile(filename_normalTrackCuts, "UPDATE");
  TTree *t_normalCuts = (TTree*)f_normalCuts->Get(treename_normalTrackCuts);

  TFile *f_looseCuts = new TFile(filename_looseTrackCuts, "UPDATE");
  TTree *t_looseCuts = (TTree*)f_looseCuts->Get(treename_looseTrackCuts);

  Long64_t num_entries_normalCuts = t_normalCuts->GetEntries();
  Long64_t num_entries_looseCuts = t_looseCuts->GetEntries();


  std::cout << "Create new branches in each tree with "
	    << "information about their track cuts...";

  Int_t IsLooseCuts_normalCuts;
  TBranch *b_IsLooseCuts_normalCuts =
    t_normalCuts->Branch("IsLooseCuts", &IsLooseCuts_normalCuts, "IsLooseCuts/I");
  
  for(Long64_t i=0; i<num_entries_normalCuts; i++) {
    IsLooseCuts_normalCuts = 0;
    b_IsLooseCuts_normalCuts->Fill();
  }

  
  Int_t IsLooseCuts_looseCuts;
  TBranch *b_IsLooseCuts_looseCuts =
    t_looseCuts->Branch("IsLooseCuts", &IsLooseCuts_looseCuts, "IsLooseCuts/I");


  for(Long64_t i=0; i<num_entries_looseCuts; i++) {
    IsLooseCuts_looseCuts = 1;
    b_IsLooseCuts_looseCuts->Fill();
  }

  std::cout << "DONE" << std::endl;

  
  std::cout << "Merge the new trees...";

  TList *treelist = new TList;
  treelist->Add(t_normalCuts);
  treelist->Add(t_looseCuts);

  TFile *f_mergedTree = new TFile(filename_output, "RECREATE");
  TTree *mergedTree = TTree::MergeTrees(treelist);
  mergedTree->SetName(treename_mergedTree);
  
  std::cout << "DONE" << std::endl;
  
  std::cout << "Write the merged to " << filename_output << "...";
  mergedTree->Write(0, TObject::kOverwrite);
  std::cout << "DONE" << std::endl;

  std::cout << std::endl << "Finished. Time passed: " << watch->RealTime()
	    << " seconds." << std::endl;

  gSystem->Exit(0);

}
