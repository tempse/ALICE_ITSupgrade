#include <iostream>

#include <TSystem.h>
#include <TFile.h>
#include <TTree.h>


void addLabelBranch(TString updatefile_filename,
                    TString updatefile_treename,
                    TString newBranch_name,
                    Int_t newBranch_value) {

  TFile *updatefile = new TFile(updatefile_filename, "UPDATE");

  if(updatefile->IsZombie()) {
    std::cout << "  ERROR: Cannot open file " << updatefile_filename << std::endl;
    gSystem->Exit(1);
  }

  if(updatefile->GetListOfKeys()->Contains(newBranch_name)) {
    std::cout << "  ERROR: A branch named " << newBranch_name <<
      " already exists in file " << updatefile_filename << std::endl;
    gSystem->Exit(1);
  }

  TTree *updatefile_tree = (TTree*)updatefile->Get(updatefile_treename);

  Int_t tag_val = newBranch_value;
  TBranch *newBranch = updatefile_tree->Branch(newBranch_name, &tag_val, newBranch_name+"/I");

  Long64_t nentries = updatefile_tree->GetEntries();

  for(Long64_t i=0; i<nentries; i++) {
    updatefile_tree->GetEntry(i);
    newBranch->Fill();
  }

  updatefile_tree->Write(0, TObject::kOverwrite);

  delete updatefile;

  gSystem->Exit(0);
}
