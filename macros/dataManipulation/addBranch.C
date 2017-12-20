#include <iostream>

#include <TSystem.h>
#include <TApplication.h>
#include <TFile.h>
#include <TTree.h>


void addBranch(TString updatefile_filename,
	       TString updatefile_treename,
	       TString branchfile_filename,
	       TString branchfile_treename,
	       TString branchfile_branchname,
	       TString newBranch_name) {
  
  Int_t new_v;
  TString newBranch_vartype = "I";


  
  
  TFile *updatefile = new TFile(updatefile_filename, "UPDATE");
  if(updatefile->IsZombie()) {
    std::cout << "  ERROR: Cannot open file " << updatefile_filename << std::endl;
    gApplication->Terminate();
  }

  if(!updatefile->GetListOfKeys()->Contains(updatefile_treename)) {
    std::cout << "  ERROR: No tree " << updatefile_treename << " in file "
	      << updatefile_filename << std::endl;
    gApplication->Terminate();
  }
  TTree *updatefile_tree = (TTree*)updatefile->Get(updatefile_treename);
  
  
  TFile *branchfile = new TFile(branchfile_filename, "READ");
  if(branchfile->IsZombie()) {
    std::cout << "  ERROR: Cannot open file " << branchfile_filename << std::endl;
    gApplication->Terminate();
  }

  if(!branchfile->GetListOfKeys()->Contains(branchfile_treename)) {
    std::cout << "  ERROR: No tree " << branchfile_treename << " in file "
	      << branchfile_filename << std::endl;
    gApplication->Terminate();
  }
  TTree *branchfile_tree = (TTree*)branchfile->Get(branchfile_treename);

  
  if(updatefile_tree->GetEntries() != branchfile_tree->GetEntries()) {
    std::cout << "  ERROR: Trees in different files do not have the same size." << std::endl;
    gApplication->Terminate();
  }


  if(updatefile_tree->GetListOfBranches()->FindObject(newBranch_name) != NULL) {
    std::cout << "  ERROR: A branch called " << newBranch_name << " already exists in file "
	      << updatefile_filename << std::endl;
    gApplication->Terminate();
  }

  if(branchfile_tree->GetListOfBranches()->FindObject(branchfile_branchname) == NULL) {
    std::cout << "  ERROR: The branch " << branchfile_branchname << " does not exist in file "
	      << branchfile_filename << std::endl;
    gApplication->Terminate();
  }else {
    branchfile_tree->SetBranchAddress(branchfile_branchname, &new_v);
  }
  
  TString newBranch_name_ext = newBranch_name + "/" + newBranch_vartype;
  TBranch *newBranch = updatefile_tree->Branch(newBranch_name,
					       &new_v,
					       newBranch_name_ext);

  Long64_t nentries = updatefile_tree->GetEntries();
  
  for(Long64_t i=0; i<nentries; i++) {
    if((i%5000)==0) std::cout << "\r" << i << " / " << nentries;
    
    branchfile_tree->GetEntry(i);
    newBranch->Fill();
  }
  std::cout << "\r" << nentries << " / " << nentries << std::endl;

  updatefile->cd();
  updatefile_tree->Write("", TObject::kOverwrite);

  gSystem->Exit(0);
}
