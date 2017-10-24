#include <iostream>

#include <TROOT.h>
#include <TMath.h>
#include <TApplication.h>
#include <TString.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>


Float_t getPairPIDefficiency(Float_t, Float_t, TH1D&);


void GenerateConfusionMatrix(TString infileName = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/improperPrefiltering_noPIDeffs/RPConvRejClassicalCuts/prefilter/FT2_ITSup_pairTree-us_part2_538163tightCutEvents.root",
			     TString branchname = "IsTaggedRPConv_classicalCuts_prefilter") {
  
  // TString infileName = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/improperPrefiltering_noPIDeffs/RPConvRejClassicalCuts/prefilter/FT2_ITSup_pairTree-us_part2_538163tightCutEvents.root";

  // "+"...signal-like events are near 1,
  // "-"...signal-like events are near 0
  TString signalRegion = "-";
  
  TFile *infile = new TFile(infileName, "READ");
  TTree *tree = (TTree*)infile->Get("pairTree_us");

  Int_t isTaggedSignal, isSignal;
  Float_t pt1, pt2;
  Bool_t containsTrackCutInfo = kTRUE;
  Int_t TrackCut1, TrackCut2;
  tree->SetBranchAddress(branchname,
			 &isTaggedSignal);
  tree->SetBranchAddress("IsConv", &isSignal);
  tree->SetBranchAddress("pt1", &pt1);
  tree->SetBranchAddress("pt2", &pt2);
  if(tree->GetListOfBranches()->FindObject("TrackCut1") != NULL &&
     tree->GetListOfBranches()->FindObject("TrackCut2") != NULL) {
    tree->SetBranchAddress("TrackCut1", &TrackCut1);
    tree->SetBranchAddress("TrackCut2", &TrackCut2);
  }else {
    std::cout << "  Info: No branch holding track cut information found. "
	      << "All tracks will be processed." << std::endl;
    containsTrackCutInfo = kFALSE;
  }

  Float_t mass;
  tree->SetBranchAddress("mass", &mass);

  const Bool_t doConsiderPIDefficiencies = kFALSE;

  // File containing the pt-dependent PID efficiencies:
  TString infile_PIDefficiencies_name = "~/analysis/data/FT2_AnalysisResults_Upgrade/inputData/ITSU_PIDefficiency_lowB.root";
  TFile *infile_PIDefficiencies = new TFile(infile_PIDefficiencies_name, "READ");
  TH1D *h_PIDeff = (TH1D*)infile_PIDefficiencies->Get("efficiencyLHC17d12_TPCandTOF3sigma");
  h_PIDeff->GetXaxis()->SetRangeUser(0,5);

  
  
  // true positives, false positives, true negatives, false negatives
  Double_t TP = 0., FP = 0., TN = 0., FN = 0.;
  
  Long64_t nentries = 50000000; //tree->GetEntries();
  
  
  for(Long64_t i=0; i<nentries; i++) {
    if((i%5000)==0) std::cout << "\rProcessing entry " << i << " of "
			      << nentries << " (" << i*100/((Float_t)nentries)
			      << " %)...";
    tree->GetEntry(i);

    if(mass<.05) continue;
    if(containsTrackCutInfo && (TrackCut1!=2 || TrackCut2!=2)) continue;

    Float_t pair_weight = 1.;
    if(doConsiderPIDefficiencies) {
      pair_weight = getPairPIDefficiency(pt1, pt2, *h_PIDeff);
    }
    
    if(signalRegion == "+") {
      if(isTaggedSignal==1 && isSignal==1) TP += pair_weight;
      if(isTaggedSignal==0 && isSignal==1) FN += pair_weight;
      if(isTaggedSignal==1 && isSignal==0) FP += pair_weight;
      if(isTaggedSignal==0 && isSignal==0) TN += pair_weight;
    }else if(signalRegion == "-") {
      if(isTaggedSignal==0 && isSignal==0) TP += pair_weight;
      if(isTaggedSignal==1 && isSignal==0) FN += pair_weight;
      if(isTaggedSignal==0 && isSignal==1) FP += pair_weight;
      if(isTaggedSignal==1 && isSignal==1) TN += pair_weight;
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
  std::cout << "significance = " << TP/TMath::Sqrt(TP+FP) <<std::endl;
  std::cout << "significance gain = " << (TP/TMath::Sqrt(TP+FP)-(TP+FN)/TMath::Sqrt(TP+FN+FP+TN)) / (TMath::Sqrt(TP+FN)-(TP+FN)/TMath::Sqrt(TP+FN+FP+TN)) << std::endl;
  
  std::cout << std::endl;
  std::cout << "TP + FN = " << TP+FN << std::endl
	    << "FP + TN = " << FP+TN << std::endl
	    << "     sum: " << TP+FN+FP+TN << std::endl;

  gApplication->Terminate();
}




Float_t getPairPIDefficiency(Float_t pt1, Float_t pt2, TH1D &h_PIDeff) {

  Float_t PIDeff1 = (pt1 >= h_PIDeff.GetBinLowEdge(h_PIDeff.GetNbinsX())) ?
    h_PIDeff.GetBinContent(h_PIDeff.GetNbinsX()) : h_PIDeff.GetBinContent(h_PIDeff.FindBin(pt1));

  Float_t PIDeff2 = (pt2 >= h_PIDeff.GetBinLowEdge(h_PIDeff.GetNbinsX())) ?
    h_PIDeff.GetBinContent(h_PIDeff.GetNbinsX()) : h_PIDeff.GetBinContent(h_PIDeff.FindBin(pt2));

  return PIDeff1 * PIDeff2;
  
}
