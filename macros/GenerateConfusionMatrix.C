#include <iostream>

#include <TROOT.h>
#include <TApplication.h>
#include <TString.h>
#include <TFile.h>
#include <TTree.h>


void GenerateConfusionMatrix() {
  TString infileName = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/analysis_singleConvTrackRejMVAcuts/applicationPhase1/FT2_AnalysisResults_Upgrade_addFeat_pairtree_us_part2_1-9-split.root";

  // "+"...signal-like events are near 1,
  // "-"...signal-like events are near 0 or -1
  TString signalRegion = "-";
  
  TFile *infile = new TFile(infileName, "READ");
  TTree *tree = (TTree*)infile->Get("pairTree_us");

  Int_t isTaggedSignal, isSignal;
  tree->SetBranchAddress("IsTaggedRPConv_classicalCuts_prefilter",
			 &isTaggedSignal);
  tree->SetBranchAddress("IsConv", &isSignal);

  Float_t mass;
  tree->SetBranchAddress("mass", &mass);


  // true positives, false positives, true negatives, false negatives
  Long64_t TP = 0, FP = 0, TN = 0, FN = 0;
  
  Long64_t nentries = tree->GetEntries();
  
  for(Long64_t i=0; i<nentries; i++) {
    if((i%5000)==0) std::cout << "\rProcessing entry " << i << " of "
			      << nentries << " (" << i*100/((Float_t)nentries)
			      << " %)...";
    tree->GetEntry(i);

    if(mass<.05) continue;
    
    if(signalRegion == "+") {
      if(isTaggedSignal && isSignal) TP += 1;
      if(!isTaggedSignal && isSignal) FN += 1;
      if(isTaggedSignal && !isSignal) FP += 1;
      if(!isTaggedSignal && !isSignal) TN += 1;
    }else if(signalRegion == "-") {
      if(!isTaggedSignal && !isSignal) TP += 1;
      if(isTaggedSignal && !isSignal) FN += 1;
      if(!isTaggedSignal && isSignal) FP += 1;
      if(isTaggedSignal && isSignal) TN += 1;
    }
  }
  std::cout << "\rProcessing entry " << nentries << " of " << nentries
	    << " (100 %)... DONE" << std::endl;


  std::cout << "Confusion matrix:" << std::endl;
  std::cout << "  [[TP\tFN]\n   [FP\tTN]] =\n"
	    << "= [[" << TP << "\t" << FN << "]\n   ["
	    << FP << "\t" << TN << "]]" << std::endl << std::endl;

  std::cout << "TPR = TP/(TP+FN) = " << TP/((Float_t)(TP+FN)) << std::endl;
  std::cout << "FPR = FP/(FP+TN) = " << FP/((Float_t)(FP+TN)) << std::endl;
  
  std::cout << std::endl;
  std::cout << "TP + FN = " << TP+FN << std::endl
	    << "FP + TN = " << FP+TN << std::endl
	    << "     sum: " << TP+FN+FP+TN << std::endl;

  gApplication->Terminate();
}
