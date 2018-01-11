#include <TSystem.h>
#include <TTree.h>
#include <TFile.h>
#include <TString.h>


void renameBranch(TString filename,
                  TString treename,
                  TString leafname_old,
                  TString leafname_new) {


  TFile *updatefile = TFile::Open(filename, "update");

  if(updatefile->IsZombie()) {
    std::cout << "  ERROR: Cannot open file " << filename << std::endl;
    gSystem->Exit(1);
  }
  
  if(!updatefile->GetListOfKeys()->Contains(treename)) {
    std::cout << "  ERROR: No tree named " << treename << " in file "
              << filename << std::endl;
    gSystem->Exit(1);
  }

  TTree *t = (TTree*)updatefile->Get(treename);

  if(!t->GetListOfBranches()->FindObject(leafname_old)) {
    std::cout << "  ERROR: No branch named " << leafname_old << " in file "
              << filename << std::endl;
    gSystem->Exit(1);
  }

  if(t->GetListOfBranches()->FindObject(leafname_new)) {
    std::cout << "  ERROR: A branch named " << leafname_new
              << " already exists in file " << filename << std::endl;
    gSystem->Exit(1);
  }

  t->GetBranch(leafname_old)->SetTitle(leafname_new);
  t->GetBranch(leafname_old)->SetName(leafname_new);
  t->GetLeaf(leafname_old)->SetTitle(leafname_new);
  t->GetLeaf(leafname_old)->SetName(leafname_new);

  updatefile->Write(0, TObject::kOverwrite);
  updatefile->Close();
  
  std::cout << "Renaming done: " << leafname_old << "->" << leafname_new
            << std::endl;
  std::cout << " File closed." << std::endl;

  gSystem->Exit(0);
}
