#include <TSystem.h>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TList.h>
#include <TStopwatch.h>

void addLooseCutBranches(TString filename_normalTrackCuts,
			 TString filename_looseTrackCuts,
			 TString treename_normalTrackCuts="outputITSup/tracks",
			 TString treename_looseTrackCuts="outputITSup/tracks",
			 TString treename_mergedTree="tracks",
			 TString filename_output="output_mergedTree.root") {


  TStopwatch *watch = new TStopwatch();
  watch->Start();


  Int_t event;
  Double_t IP;
  Int_t global;
  Double_t eta;
  Double_t phi;
  Double_t pt;
  Double_t dcaR_norm;
  Double_t dcaZ_norm;
  Double_t dcaX;
  Double_t dcaY;
  Double_t dcaZ;
  Int_t label;
  Int_t labelFirstMother;
  Int_t labelMinFirstMother;
  Int_t labelMaxFirstMother;
  Double_t mcEta;
  Double_t mcPhi;
  Double_t mcPt;
  Int_t pdg;
  Int_t pdgMother;
  Int_t pdgFirstMother;
  TParticle *particle = NULL;
  Int_t nITS;
  Double_t nITSshared;
  Int_t nTPC;
  Double_t ITSchi2;
  Double_t TPCchi2;
  
  
  
  TFile *f_normalCuts = new TFile(filename_normalTrackCuts, "READ");
  TTree *t_normalCuts = (TTree*)f_normalCuts->Get(treename_normalTrackCuts);
  t_normalCuts->SetBranchAddress("event",&event);
  t_normalCuts->SetBranchAddress("IP",&IP);
  t_normalCuts->SetBranchAddress("global",&global);
  t_normalCuts->SetBranchAddress("eta",&eta);
  t_normalCuts->SetBranchAddress("phi",&phi);
  t_normalCuts->SetBranchAddress("pt",&pt);
  t_normalCuts->SetBranchAddress("dcaR_norm",&dcaR_norm);
  t_normalCuts->SetBranchAddress("dcaZ_norm",&dcaZ_norm);
  t_normalCuts->SetBranchAddress("dcaX",&dcaX);
  t_normalCuts->SetBranchAddress("dcaY",&dcaY);
  t_normalCuts->SetBranchAddress("dcaZ",&dcaZ);
  t_normalCuts->SetBranchAddress("label",&label);
  t_normalCuts->SetBranchAddress("labelFirstMother",&labelFirstMother);
  t_normalCuts->SetBranchAddress("labelMinFirstMother",&labelMinFirstMother);
  t_normalCuts->SetBranchAddress("labelMaxFirstMother",&labelMaxFirstMother);
  t_normalCuts->SetBranchAddress("mcEta",&mcEta);
  t_normalCuts->SetBranchAddress("mcPhi",&mcPhi);
  t_normalCuts->SetBranchAddress("mcPt",&mcPt);
  t_normalCuts->SetBranchAddress("pdg",&pdg);
  t_normalCuts->SetBranchAddress("pdgMother",&pdgMother);
  t_normalCuts->SetBranchAddress("pdgFirstMother",&pdgFirstMother);
  t_normalCuts->SetBranchAddress("particle.",&particle);
  t_normalCuts->SetBranchAddress("nITS",&nITS);
  t_normalCuts->SetBranchAddress("nITSshared",&nITSshared);
  t_normalCuts->SetBranchAddress("nTPC",&nTPC);
  t_normalCuts->SetBranchAddress("ITSchi2",&ITSchi2);
  t_normalCuts->SetBranchAddress("TPCchi2",&TPCchi2);

  TFile *f_looseCuts = new TFile(filename_looseTrackCuts, "READ");
  TTree *t_looseCuts = (TTree*)f_looseCuts->Get(treename_looseTrackCuts);
  t_looseCuts->SetBranchAddress("event",&event);
  t_looseCuts->SetBranchAddress("IP",&IP);
  t_looseCuts->SetBranchAddress("global",&global);
  t_looseCuts->SetBranchAddress("eta",&eta);
  t_looseCuts->SetBranchAddress("phi",&phi);
  t_looseCuts->SetBranchAddress("pt",&pt);
  t_looseCuts->SetBranchAddress("dcaR_norm",&dcaR_norm);
  t_looseCuts->SetBranchAddress("dcaZ_norm",&dcaZ_norm);
  t_looseCuts->SetBranchAddress("dcaX",&dcaX);
  t_looseCuts->SetBranchAddress("dcaY",&dcaY);
  t_looseCuts->SetBranchAddress("dcaZ",&dcaZ);
  t_looseCuts->SetBranchAddress("label",&label);
  t_looseCuts->SetBranchAddress("labelFirstMother",&labelFirstMother);
  t_looseCuts->SetBranchAddress("labelMinFirstMother",&labelMinFirstMother);
  t_looseCuts->SetBranchAddress("labelMaxFirstMother",&labelMaxFirstMother);
  t_looseCuts->SetBranchAddress("mcEta",&mcEta);
  t_looseCuts->SetBranchAddress("mcPhi",&mcPhi);
  t_looseCuts->SetBranchAddress("mcPt",&mcPt);
  t_looseCuts->SetBranchAddress("pdg",&pdg);
  t_looseCuts->SetBranchAddress("pdgMother",&pdgMother);
  t_looseCuts->SetBranchAddress("pdgFirstMother",&pdgFirstMother);
  t_looseCuts->SetBranchAddress("particle.",&particle);
  t_looseCuts->SetBranchAddress("nITS",&nITS);
  t_looseCuts->SetBranchAddress("nITSshared",&nITSshared);
  t_looseCuts->SetBranchAddress("nTPC",&nTPC);
  t_looseCuts->SetBranchAddress("ITSchi2",&ITSchi2);
  t_looseCuts->SetBranchAddress("TPCchi2",&TPCchi2);

  TFile *f_mergedTree = new TFile(filename_output, "RECREATE");
  TTree *t_mergedTree = new TTree(treename_mergedTree, treename_mergedTree);
  t_mergedTree->Branch("event",&event);
  t_mergedTree->Branch("IP",&IP);
  t_mergedTree->Branch("global",&global);
  t_mergedTree->Branch("eta",&eta);
  t_mergedTree->Branch("phi",&phi);
  t_mergedTree->Branch("pt",&pt);
  t_mergedTree->Branch("dcaR_norm",&dcaR_norm);
  t_mergedTree->Branch("dcaZ_norm",&dcaZ_norm);
  t_mergedTree->Branch("dcaX",&dcaX);
  t_mergedTree->Branch("dcaY",&dcaY);
  t_mergedTree->Branch("dcaZ",&dcaZ);
  t_mergedTree->Branch("label",&label);
  t_mergedTree->Branch("labelFirstMother",&labelFirstMother);
  t_mergedTree->Branch("labelMinFirstMother",&labelMinFirstMother);
  t_mergedTree->Branch("labelMaxFirstMother",&labelMaxFirstMother);
  t_mergedTree->Branch("mcEta",&mcEta);
  t_mergedTree->Branch("mcPhi",&mcPhi);
  t_mergedTree->Branch("mcPt",&mcPt);
  t_mergedTree->Branch("pdg",&pdg);
  t_mergedTree->Branch("pdgMother",&pdgMother);
  t_mergedTree->Branch("pdgFirstMother",&pdgFirstMother);
  t_mergedTree->Branch("particle.",&particle);
  t_mergedTree->Branch("nITS",&nITS);
  t_mergedTree->Branch("nITSshared",&nITSshared);
  t_mergedTree->Branch("nTPC",&nTPC);
  t_mergedTree->Branch("ITSchi2",&ITSchi2);
  t_mergedTree->Branch("TPCchi2",&TPCchi2);

  // Add branch preserving the information of the tracks' file origin:
  Int_t IsLooseCuts = 0;
  t_mergedTree->Branch("IsLooseCuts", &IsLooseCuts);

  
  Long64_t num_entries_normalCuts = t_normalCuts->GetEntries();
  Long64_t num_entries_looseCuts = t_looseCuts->GetEntries();


  
  std::cout << "Merging " << f_normalCuts->GetName() << " (IsLooseCuts=0) and "
	    << f_looseCuts->GetName() << " (IsLooseCuts=1)..." << std::endl;

  
  for(Long64_t i=0; i<num_entries_normalCuts; i++) {
    if((i%1000)==0) std::cout << "\rProcessing event " << i << " of "
			      << num_entries_normalCuts+num_entries_looseCuts << " ("
			      << i*100/(num_entries_normalCuts+num_entries_looseCuts)
			      << "%)...";
    t_normalCuts->GetEntry(i);
    t_mergedTree->Fill();
  }

  IsLooseCuts = 1;

  for(Long64_t i=0; i<num_entries_looseCuts; i++) {
    if((i%1000)==0) std::cout << "\rProcessing event " << i+num_entries_normalCuts << " of "
			      << num_entries_normalCuts+num_entries_looseCuts << " ("
			      << (i+num_entries_normalCuts)*100/(num_entries_normalCuts+num_entries_looseCuts)
			      << "%)...";
    t_looseCuts->GetEntry(i);
    t_mergedTree->Fill();
  }

  std::cout << "\rProcessing event " << num_entries_normalCuts+num_entries_looseCuts
	    << " of " << num_entries_normalCuts+num_entries_looseCuts
	    << " (100%)... DONE" << std::endl;



  t_mergedTree->Write(0, TObject::kOverwrite);
  std::cout << "The tree \"" << treename_mergedTree << "\" has been written to the file \""
	    << f_mergedTree->GetName() << "\"" << std::endl;

  
  std::cout << std::endl << "Finished. Time passed: " << watch->RealTime()
	    << " seconds." << std::endl;

  
  gSystem->Exit(0);
}
