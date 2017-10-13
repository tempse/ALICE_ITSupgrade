#include <TROOT.h>
#include "TApplication.h"
#include <TStyle.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TMath.h>

Float_t getPairPIDefficiency(Float_t, Float_t, TH1D&);

void PlotPhiv() {

  gROOT->ForceStyle();
  SetStyle();
  
  TString filename = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/properPrefiltering_wPIDeffs/RPConvRejClassicalCuts_prefiltering_PIDeffs/prefiltering/FT2_ITSup_pairTree-us_part2_538163tightCutEvents.root";
  TString treename = "pairTree_us";

  Bool_t doConsiderPIDefficiency = kTRUE;
  Bool_t doExcludeTaggedPairs = kTRUE;
  Float_t massCut_low = 0., massCut_up = 0.01;
  
  
  TFile *infile = new TFile(filename, "READ");
  TTree *tree = (TTree*)infile->Get(treename);

  Int_t IsRP, IsConv;
  Float_t phiv, pt1, pt2;
  Float_t mass;
  Int_t IsTaggedRPConv;
  Bool_t containsTrackCutInfo = kTRUE;
  Int_t TrackCut1, TrackCut2;
  tree->SetBranchAddress("IsRP", &IsRP);
  tree->SetBranchAddress("IsConv", &IsConv);
  tree->SetBranchAddress("phiv", &phiv);
  tree->SetBranchAddress("mass", &mass);
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
  if(doExcludeTaggedPairs) tree->SetBranchAddress("IsTaggedRPConv_classicalCuts_prefilter", &IsTaggedRPConv);
  
  
  // File containing the pt-dependent PID efficiencies:
  TString infile_PIDefficiencies_name = "~/analysis/data/FT2_AnalysisResults_Upgrade/inputData/ITSU_PIDefficiency_lowB.root";
  TFile *infile_PIDefficiencies = new TFile(infile_PIDefficiencies_name, "READ");
  TH1D *h_PIDeff = (TH1D*)infile_PIDefficiencies->Get("efficiencyLHC17d12_TPCandTOF3sigma");
  h_PIDeff->GetXaxis()->SetRangeUser(0,5);
  
  THStack hs("hs","");
  
  TH1D *convs = new TH1D("convs","",100,0,TMath::Pi());
  TH1D *nonconvs = new TH1D("nonconvs","",100,0,TMath::Pi());


  Long64_t nentries = 50000000; //tree->GetEntries();

  for(Long64_t i=0; i<nentries; i++) {
    if((i%5000)==0) std::cout << "\r(" << i << " / " << nentries << ")";
    
    tree->GetEntry(i);

    if(containsTrackCutInfo && (TrackCut1 != 2 || TrackCut2 != 2)) continue;
    if(mass < massCut_low || mass > massCut_up) continue;
    if(doExcludeTaggedPairs && IsTaggedRPConv==1) continue;

    Float_t pairWeight = doConsiderPIDefficiency ? getPairPIDefficiency(pt1, pt2, *h_PIDeff) : 1.;
    if(IsRP==1 && IsConv==1) convs->Fill(phiv, pairWeight);
    if(!(IsRP==1 && IsConv==1)) nonconvs->Fill(phiv, pairWeight);    
  }
  std::cout << std::endl;

  
  hs.Add(convs);
  hs.Add(nonconvs);
  

  Float_t lineWidth = 3;
  TString XTitle = "#it{#varphi}_{V} (rad)";
  TString YTitle = "Yield (arb. units)";
  
  TCanvas *c = new TCanvas("c","",768,768);
  // c->SetLogy();
  convs->SetLineColor(kRed);
  convs->SetLineWidth(lineWidth);
  convs->SetXTitle(XTitle);
  convs->SetYTitle(YTitle);
  convs->GetYaxis()->SetTitleOffset(1.5);
  
  nonconvs->SetLineColor(kGreen+1);
  nonconvs->SetLineWidth(lineWidth);
  nonconvs->SetXTitle(XTitle);
  nonconvs->SetYTitle(YTitle);

  hs.Draw("nostack");
  hs.GetXaxis()->SetTitle(XTitle);
  hs.GetYaxis()->SetTitle(YTitle);
  hs.GetYaxis()->SetTitleOffset(1.5);
  c->Modified();

  TLatex *l_ALICE = new TLatex(.19,.82,"ALICE work in progress");
  l_ALICE->SetTextSize(0.035);
  l_ALICE->SetTextFont(42);
  l_ALICE->SetNDC();
  l_ALICE->Draw();

  TLatex *l_info = new TLatex(.19,.78,"HIJING, Pb-Pb #sqrt{#it{s}_{NN}} = 5.5 TeV, 0-10%");
  l_info->SetTextSize(.03);
  l_info->SetTextFont(42);
  l_info->SetNDC();
  l_info->Draw();

  TLegend *leg = new TLegend(.21,.56,.75,.74);
  leg->AddEntry(convs, "MC e^{+}e^{-} pairs from secondary conversions", "l");
  leg->AddEntry(nonconvs, "MC e^{+}e^{-} pairs from primary hadron decays", "l");
  leg->SetFillStyle(0);
  leg->Draw();

  c->SaveAs("temp_output/phiv_contributions.pdf");
  c->SaveAs("temp_output/phiv_contributions.png");
  c->SaveAs("temp_output/phiv_contributions.root");

  gSystem->Exit(0);
}


Float_t getPairPIDefficiency(Float_t pt1, Float_t pt2, TH1D &h_PIDeff) {

  Float_t PIDeff1 = (pt1 >= h_PIDeff.GetBinLowEdge(h_PIDeff.GetNbinsX())) ?
    h_PIDeff.GetBinContent(h_PIDeff.GetNbinsX()) : h_PIDeff.GetBinContent(h_PIDeff.FindBin(pt1));

  Float_t PIDeff2 = (pt2 >= h_PIDeff.GetBinLowEdge(h_PIDeff.GetNbinsX())) ?
    h_PIDeff.GetBinContent(h_PIDeff.GetNbinsX()) : h_PIDeff.GetBinContent(h_PIDeff.FindBin(pt2));

  return PIDeff1 * PIDeff2;
  
}
