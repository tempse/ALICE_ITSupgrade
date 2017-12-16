#include <iostream>

#include <TROOT.h>
#include <TSystem.h>
#include <TApplication.h>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TH1D.h>


void addPIDeffBranch(TString filename = "",
		     TString treename = "",
		     TString filename_PIDeff = "/home/sebastian/analysis/data/finalAnalysis_FT2/inputdata/ITSU_PIDefficiency_lowB.root",
		     TString histname_PIDeff = "efficiencyLHC17d12_TPCandTOF3sigma") {

  
  
  TFile *file_update = new TFile(filename, "UPDATE");
  if(file_update->IsZombie()) {
    std::cout << "  ERROR: Cannot open file " << filename << std::endl;
    gApplication->Terminate();
  }
  if(!file_update->GetListOfKeys()->Contains(treename)) {
    std::cout << "  ERROR: No tree " << treename << " in file "
              << filename << std::endl;
    gApplication->Terminate();
  }

  TTree *tree = (TTree*)file_update->Get(treename);
  Double_t pt;
  tree->SetBranchAddress("pt", &pt);

  Float_t PIDeff;
  TBranch *b_PIDeff = tree->Branch("PIDeff", &PIDeff, "PIDeff/F");

  

  TFile *file_PIDeff = new TFile(filename_PIDeff, "READ");
  if(file_PIDeff->IsZombie()) {
    std::cout << "  ERROR: Cannot open file " << filename_PIDeff << std::endl;
    gApplication->Terminate();
  }
  if(!file_PIDeff->GetListOfKeys()->Contains(histname_PIDeff)) {
    std::cout << "  ERROR: No object " << histname_PIDeff << " in file "
	      << filename_PIDeff << std::endl;
    gApplication->Terminate();
  }

  TH1D *h_PIDeff = (TH1D*)file_PIDeff->Get(histname_PIDeff);

  

  Long64_t nentries = tree->GetEntries();

  std::cout << std::endl;

  
  for(Long64_t i=0; i<nentries; i++) {
    if((i%1000)==0) std::cout << "\rProcessing event " << i << " of " << nentries
			      << " (" << i*100/nentries << "%)...";
    
    tree->GetEntry(i);

    PIDeff = (pt >= h_PIDeff->GetBinLowEdge(h_PIDeff->GetNbinsX())) ?
      h_PIDeff->GetBinContent(h_PIDeff->GetNbinsX()) : h_PIDeff->GetBinContent(h_PIDeff->FindBin(pt));

    b_PIDeff->Fill();
  }
  std::cout << "\rProcessing event " << nentries << " of " << nentries
	    << " (100%)... DONE." << std::endl;

  
  file_update->cd();
  tree->Write("", TObject::kOverwrite);

  gSystem->Exit(0);
  
}
