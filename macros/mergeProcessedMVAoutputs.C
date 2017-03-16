#include <iostream>

#include <TROOT.h>
#include <TApplication.h>
#include <TFile.h>
#include <TTree.h>


void mergeProcessedMVAoutputs(TString branchfilename,
			      TString treename_branchfile,
			      TString branchname_branchfile,
			      TString updatefilename,
			      TString treename_updatefile,
			      TString branchname_updatefile,
			      Float_t MVAcut,
			      TString signalRegion = "+") {
  
  Float_t var;
  Int_t isAccepted;
  
  
  TFile *updatefile = new TFile(updatefilename, "UPDATE");  TTree *tree_updatefile = (TTree*)updatefile->Get(treename_updatefile);
  TString branchname_updatefile_vartype = branchname_updatefile + "/I";
  TBranch *newBranch = tree_updatefile->Branch(branchname_updatefile,
					       &isAccepted,
					       branchname_updatefile_vartype);
  
  
  TFile *branchfile = new TFile(branchfilename, "READ");
  TTree *tree_branchfile = (TTree*)branchfile->Get(treename_branchfile);
  tree_branchfile->SetBranchAddress(branchname_branchfile, &var);
  

  Long64_t nentries = tree_updatefile->GetEntries();

  if(nentries != tree_branchfile->GetEntries()) {
    std::cout << "  ERROR: Trees have different sizes." << std::endl
	      << "    Size of tree " << treename_updatefile << " in "
	      << updatefilename << ": " << tree_updatefile->GetEntries()
	      << std::endl
	      << "    Size of tree " << treename_branchfile << " in "
	      << branchfilename << ": " << tree_branchfile->GetEntries()
	      << std::endl << std::endl;
    gApplication->Terminate();
  }

  
  for(Long64_t i=0; i<nentries; i++) {
    tree_branchfile->GetEntry(i);
    if(signalRegion == "+") {
      if(var > MVAcut) isAccepted = 1.;
      else isAccepted = 0.;
    }else if(signalRegion == "-") {
      if(var < MVAcut) isAccepted = 1.;
      else isAccepted = 0.;
    }else {
      std::cout << "  ERROR: 'signalRegion' definition is wrong or not provided. "
		<< "(It should be either '+' (default) or '-'.)" << std::endl;
      std::cout << "  Abort." << std::endl << std::endl;
      gApplication->Terminate();
    }
    
    newBranch->Fill();
  }
  
  updatefile->cd();
  tree_updatefile->Write("", TObject::kOverwrite);
  
  
  gApplication->Terminate();
  
}
