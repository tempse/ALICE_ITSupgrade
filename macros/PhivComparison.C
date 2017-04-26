#include <iostream>

#include <TROOT.h>
#include <TMath.h>
#include <TFile.h>
#include <TTree.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TH1D.h>

void PhivComparison() {
  TFile *infile_w = new TFile("/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/analysis_singleConvTrackRejMVAcuts/applicationPhase1/FT2_AnalysisResults_Upgrade_addFeat_partialTagging_pairtree_us_withPhivSubtr_part2_1-9-split.root","READ");
  TTree *tree_w = (TTree*)infile_w->Get("pairTree_us");

  TFile *infile_wo = new TFile("/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/analysis_singleConvTrackRejMVAcuts/applicationPhase1/FT2_AnalysisResults_Upgrade_addFeat_partialTagging_pairtree_us_part2_1-9-split.root","READ");
  TTree *tree_wo = (TTree*)infile_wo->Get("pairTree_us");

  TFile *infile_AliPhysicsPhiv = new TFile("/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/analysis_singleConvTrackRejMVAcuts/applicationPhase1/FT2_AnalysisResults_Upgrade_addFeat_partialTagging_pairtree_us_AliPhysicsPhiv_part2_1-9-split.root","READ");
  TTree *tree_AliPhysicsPhiv = (TTree*)infile_AliPhysicsPhiv->Get("pairTree_us");

  Float_t phiv_w, phiv_wo, phiv_AliPhysics;
  tree_w->SetBranchAddress("phiv", &phiv_w);
  tree_wo->SetBranchAddress("phiv", &phiv_wo);
  tree_AliPhysicsPhiv->SetBranchAddress("phiv", &phiv_AliPhysics);

  Int_t IsRP, IsConv;
  tree_w->SetBranchAddress("IsRP", &IsRP);
  tree_w->SetBranchAddress("IsConv", &IsConv);

  Float_t bin_min = 0.,
    bin_max = TMath::Pi(),
    nBins = 100;
  
  TH1D *h_w = new TH1D("","",nBins,bin_min,bin_max);
  TH1D *h_wo = new TH1D("","",nBins,bin_min,bin_max);
  TH1D *h_AliPhysicsPhiv = new TH1D("","",nBins,bin_min,bin_max);

  TH1D *h_w_RPConv = new TH1D("","",nBins,bin_min,bin_max);
  TH1D *h_wo_RPConv = new TH1D("","",nBins,bin_min,bin_max);
  TH1D *h_AliPhysicsPhiv_RPConv = new TH1D("","",nBins,bin_min,bin_max);

  

  Long64_t nentries = tree_w->GetEntries();

  std::cout << std::endl;
  for(Long64_t i=0; i<nentries; i++) {
    if((i%1000)==0) std::cout << "\r" << i*100/((Float_t)nentries) << " %          ";
    
    tree_w->GetEntry(i);
    tree_wo->GetEntry(i);
    tree_AliPhysicsPhiv->GetEntry(i);

    h_w->Fill(phiv_w);
    h_wo->Fill(phiv_wo);
    h_AliPhysicsPhiv->Fill(phiv_AliPhysics);

    if(IsRP==1 && IsConv==1) {
      h_w_RPConv->Fill(phiv_w);
      h_wo_RPConv->Fill(phiv_wo);
      h_AliPhysicsPhiv_RPConv->Fill(phiv_AliPhysics);
    }
  }
  std::cout << "\r100 %          " << std::endl;


  h_wo->SetLineWidth(2);
  h_wo->SetMarkerStyle(20);
  h_wo->SetMarkerSize(1);

  h_wo_RPConv->SetLineWidth(2);
  h_wo_RPConv->SetMarkerStyle(20);
  h_wo_RPConv->SetMarkerSize(1);
  
  
  h_AliPhysicsPhiv->SetLineColor(kGreen+1);
  h_AliPhysicsPhiv->SetMarkerColor(kGreen+1);
  h_AliPhysicsPhiv->SetMarkerStyle(26);
  h_AliPhysicsPhiv->SetMarkerSize(.5);

  h_AliPhysicsPhiv_RPConv->SetLineColor(kGreen+1);
  h_AliPhysicsPhiv_RPConv->SetMarkerColor(kGreen+1);
  h_AliPhysicsPhiv_RPConv->SetMarkerStyle(26);
  h_AliPhysicsPhiv_RPConv->SetMarkerSize(.5);
  
  
  h_w->SetLineColor(kRed);
  h_w->SetMarkerColor(kRed);
  h_w->SetMarkerStyle(20);
  h_w->SetMarkerSize(1);
  h_w->SetXTitle("#phi_{V}");
  h_w->SetYTitle("Counts");
  h_w->GetYaxis()->SetTitleOffset(1.4);
  h_w->GetXaxis()->SetTitleOffset(1.2);

  h_w_RPConv->SetLineColor(kRed);
  h_w_RPConv->SetMarkerColor(kRed);
  h_w_RPConv->SetMarkerStyle(20);
  h_w_RPConv->SetMarkerSize(1);
  h_w_RPConv->SetXTitle("#phi_{V}");
  h_w_RPConv->SetYTitle("Counts");
  h_w_RPConv->GetYaxis()->SetTitleOffset(1.4);
  h_w_RPConv->GetXaxis()->SetTitleOffset(1.2);


  gStyle->SetOptStat(0);

  TLegend *leg = new TLegend(.65,.1,.9,.2);
  leg->AddEntry(h_w, "with phiv subtr.", "ep");
  leg->AddEntry(h_wo, "without phiv subtr.", "ep");
  leg->AddEntry(h_AliPhysicsPhiv, "AliPhysics phiv implementation", "ep");

  
  TCanvas *c1 = new TCanvas("c1","Phiv distributions",1024,768);
  c1->SetGridy();
  
  h_w->Draw("e");
  h_wo->Draw("e same");
  h_AliPhysicsPhiv->Draw("e same");

  leg->Draw();

  c1->Modified();
  c1->Update();
  
  c1->SaveAs("temp_output/temp_phiv_comp.root");
  c1->SaveAs("temp_output/temp_phiv_comp.pdf");
  c1->SaveAs("temp_output/temp_phiv_comp.png");


  TCanvas *c2 = new TCanvas("c2","Phiv distributions of RP conversions",1024,768);
  c2->SetGridy();

  h_w_RPConv->Draw("e");
  h_wo_RPConv->Draw("e same");
  h_AliPhysicsPhiv_RPConv->Draw("e same");

  leg->Draw();

  c2->Modified();
  c2->Update();

  c2->SaveAs("temp_output/temp_phiv_RPConv_comp.root");
  c2->SaveAs("temp_output/temp_phiv_RPConv_comp.pdf");
  c2->SaveAs("temp_output/temp_phiv_RPConv_comp.png");
  
  
}
