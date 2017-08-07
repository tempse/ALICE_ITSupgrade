#include <iostream>
#include <cstdlib>

#include <TROOT.h>
#include <TApplication.h>
#include <TStyle.h>
#include <TString.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TStopwatch.h>
#include <TMath.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLatex.h>

void PlotMass_analysisComparison() {

  // RP conv. rej. via classical cuts:
  TString infile1_filename = "~/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/RPConvRejClassicalCuts-prefilter_DNN/cutVariations/mass_histos_comparison.root";

  // RP conv. rej. via MVA cuts:
  TString infile2_filename = "~/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/RPConvRejMVA-prefilter_DNN_mass0.05/cutVariations/mass_histos_comparison.root";

  // single conv. track rej. via MVA cuts:
  TString infile3_filename = "~/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/singleTrackConvRejMVA_DNN_noMassCut/cutVariations/mass_histos_comparison.root";

  // Comb. conv. rej. via MVA cuts:
  TString infile4_filename = "~/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/CombConvRejMVA_DNN_0.05mass/cutVariations/mass_histos_comparison.root";

  TString infile_noMVAcuts_filename = "~/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/singleTrackConvRejMVA_DNN_noMassCut/cutVariations/mass_histos_comparison.root";


  TString leg_label1 = "classical cuts";
  TString leg_label2 = "MVA 3";
  TString leg_label3 = "MVA 2";
  TString leg_label4 = "MVA 1";
  TString leg_label_noMVAcuts = "no cuts";
  TString leg_label_ideal = "ideal backgr. rejection";

  const Int_t kMarker_1 = 24;
  const Int_t kMarker_2 = 25;
  const Int_t kMarker_3 = 26;
  const Int_t kMarker_4 = 32;

  const Int_t kColor_1 = kBlack;
  const Int_t kColor_2 = kBlue;
  const Int_t kColor_3 = kRed;
  const Int_t kColor_4 = kGreen+2;
  const Int_t kColor_noMVAcuts = kGray+2;


  //////////////////////////////////////////////////////////////////////////////
  

  TFile *infile1 = new TFile(infile1_filename, "READ");
  TH1D *h_significance_class_1      = (TH1D*)infile1->Get("h_significance_1");
  TH1D *h_significance_exp_1        = (TH1D*)infile1->Get("h_significance_exp_1");
  TH1D *h_SoverB_class_1            = (TH1D*)infile1->Get("h_SoverB_1");
  TH1D *h_SoverB_exp_1              = (TH1D*)infile1->Get("h_SoverB_exp_1");
  TH1D *h_eff_S_1                   = (TH1D*)infile1->Get("h_S_eff_1");
  TH1D *h_eff_CombiWithConvLeg_1    = (TH1D*)infile1->Get("h_CombiWithConvLeg_eff_1");
  TH1D *h_eff_CombiWithoutConvLeg_1 = (TH1D*)infile1->Get("h_CombiWithoutConvLeg_eff_1");
  TH1D *h_eff_HF_1                  = (TH1D*)infile1->Get("h_HF_eff_1");
  TH1D *h_eff_RPConv_1              = (TH1D*)infile1->Get("h_RPConv_eff_1");
  h_significance_class_1->SetName("h_significance_class_1");
  h_significance_exp_1->SetName("h_significance_exp_1");
  h_SoverB_class_1->SetName("h_SoverB_class_1");
  h_SoverB_exp_1->SetName("h_SoverB_exp_1");

  TFile *infile2 = new TFile(infile2_filename, "READ");
  TH1D *h_significance_class_2      = (TH1D*)infile2->Get("h_significance_1");
  TH1D *h_significance_exp_2        = (TH1D*)infile2->Get("h_significance_exp_1");
  TH1D *h_SoverB_class_2            = (TH1D*)infile2->Get("h_SoverB_1");
  TH1D *h_SoverB_exp_2              = (TH1D*)infile2->Get("h_SoverB_exp_1");
  TH1D *h_eff_S_2                   = (TH1D*)infile2->Get("h_S_eff_1");
  TH1D *h_eff_CombiWithConvLeg_2    = (TH1D*)infile2->Get("h_CombiWithConvLeg_eff_1");
  TH1D *h_eff_CombiWithoutConvLeg_2 = (TH1D*)infile2->Get("h_CombiWithoutConvLeg_eff_1");
  TH1D *h_eff_HF_2                  = (TH1D*)infile2->Get("h_HF_eff_1");
  TH1D *h_eff_RPConv_2              = (TH1D*)infile2->Get("h_RPConv_eff_1");
  h_significance_class_2->SetName("h_significance_class_2");
  h_significance_exp_2->SetName("h_significance_exp_2");
  h_SoverB_class_2->SetName("h_SoverB_class_2");
  h_SoverB_exp_2->SetName("h_SoverB_exp_2");
  
  TFile *infile3 = new TFile(infile3_filename, "READ");
  TH1D *h_significance_class_3      = (TH1D*)infile3->Get("h_significance_1");
  TH1D *h_significance_exp_3        = (TH1D*)infile3->Get("h_significance_exp_1");
  TH1D *h_SoverB_class_3            = (TH1D*)infile3->Get("h_SoverB_1");
  TH1D *h_SoverB_exp_3              = (TH1D*)infile3->Get("h_SoverB_exp_1");
  TH1D *h_eff_S_3                   = (TH1D*)infile3->Get("h_S_eff_1");
  TH1D *h_eff_CombiWithConvLeg_3    = (TH1D*)infile3->Get("h_CombiWithConvLeg_eff_1");
  TH1D *h_eff_CombiWithoutConvLeg_3 = (TH1D*)infile3->Get("h_CombiWithoutConvLeg_eff_1");
  TH1D *h_eff_HF_3                  = (TH1D*)infile3->Get("h_HF_eff_1");
  TH1D *h_eff_RPConv_3              = (TH1D*)infile3->Get("h_RPConv_eff_1");
  h_significance_class_3->SetName("h_significance_class_3");
  h_significance_exp_3->SetName("h_significance_exp_3");
  h_SoverB_class_3->SetName("h_SoverB_class_3");
  h_SoverB_exp_3->SetName("h_SoverB_exp_3");
  
  TFile *infile4 = new TFile(infile4_filename, "READ");
  TH1D *h_significance_class_4      = (TH1D*)infile4->Get("h_significance_1");
  TH1D *h_significance_exp_4        = (TH1D*)infile4->Get("h_significance_exp_1");
  TH1D *h_SoverB_class_4            = (TH1D*)infile4->Get("h_SoverB_1");
  TH1D *h_SoverB_exp_4              = (TH1D*)infile4->Get("h_SoverB_exp_1");
  TH1D *h_eff_S_4                   = (TH1D*)infile4->Get("h_S_eff_1");
  TH1D *h_eff_CombiWithConvLeg_4    = (TH1D*)infile4->Get("h_CombiWithConvLeg_eff_1");
  TH1D *h_eff_CombiWithoutConvLeg_4 = (TH1D*)infile4->Get("h_CombiWithoutConvLeg_eff_1");
  TH1D *h_eff_HF_4                  = (TH1D*)infile4->Get("h_HF_eff_1");
  TH1D *h_eff_RPConv_4              = (TH1D*)infile4->Get("h_RPConv_eff_1");
  h_significance_class_4->SetName("h_significance_class_4");
  h_significance_exp_4->SetName("h_significance_exp_4");
  h_SoverB_class_4->SetName("h_SoverB_class_4");
  h_SoverB_exp_4->SetName("h_SoverB_exp_4");

  TFile *infile_noMVAcuts = new TFile(infile_noMVAcuts_filename, "READ");
  TH1D *h_significance_class_noMVAcuts = (TH1D*)infile_noMVAcuts->Get("h_significance_noMVAcuts");
  TH1D *h_significance_exp_noMVAcuts = (TH1D*)infile_noMVAcuts->Get("h_significance_exp_noMVAcuts");
  TH1D *h_significance_class_ideal = (TH1D*)infile_noMVAcuts->Get("h_significance_ideal");
  TH1D *h_significance_exp_ideal = (TH1D*)infile_noMVAcuts->Get("h_significance_exp_ideal");
  TH1D *h_SoverB_class_noMVAcuts = (TH1D*)infile_noMVAcuts->Get("h_SoverB_noMVAcuts");
  TH1D *h_SoverB_exp_noMVAcuts = (TH1D*)infile_noMVAcuts->Get("h_SoverB_exp_noMVAcuts");
  TH1D *h_SoverB_exp_ideal = (TH1D*)infile_noMVAcuts->Get("h_SoverB_exp_ideal");
  h_significance_class_noMVAcuts->SetName("h_significance_class_noMVAcuts");
  h_significance_exp_noMVAcuts->SetName("h_significance_exp_noMVAcuts");
  h_significance_class_ideal->SetName("h_significance_class_ideal");
  h_significance_exp_ideal->SetName("h_significance_exp_ideal");
  h_SoverB_class_noMVAcuts->SetName("h_SoverB_class_noMVAcuts");
  h_SoverB_exp_noMVAcuts->SetName("h_SoverB_exp_noMVAcuts");
  h_SoverB_exp_ideal->SetName("h_SoverB_exp_ideal");
  


  

  const Int_t kMarkerStyle_1 = 25;
  const Int_t kMarkerStyle_2 = 20;
  const Int_t kMarkerStyle_3 = 23;
  const Int_t kMarkerStyle_4 = 26;
  const Int_t kMarkerStyle_ideal = 7;
  const Int_t kMarkerStyle_noMVAcuts = 5;
  
  
  h_significance_class_noMVAcuts->SetLineColor(kColor_noMVAcuts);
  h_significance_class_noMVAcuts->SetMarkerColor(kColor_noMVAcuts);
  h_significance_class_noMVAcuts->SetMarkerStyle(kMarkerStyle_noMVAcuts);
  h_significance_exp_noMVAcuts->SetLineColor(kColor_noMVAcuts);
  h_significance_exp_noMVAcuts->SetMarkerColor(kColor_noMVAcuts);
  h_significance_exp_noMVAcuts->SetMarkerStyle(kMarkerStyle_noMVAcuts);
  h_significance_class_ideal->SetLineColor(kColor_noMVAcuts);
  h_significance_class_ideal->SetMarkerColor(kColor_noMVAcuts);
  h_significance_class_ideal->SetMarkerStyle(kMarkerStyle_ideal);
  h_significance_exp_ideal->SetLineColor(kColor_noMVAcuts);
  h_significance_exp_ideal->SetMarkerColor(kColor_noMVAcuts);
  h_significance_exp_ideal->SetMarkerStyle(kMarkerStyle_ideal);
  
  h_SoverB_class_noMVAcuts->SetLineColor(kColor_noMVAcuts);
  h_SoverB_class_noMVAcuts->SetMarkerColor(kColor_noMVAcuts);
  h_SoverB_class_noMVAcuts->SetMarkerStyle(kMarkerStyle_noMVAcuts);
  h_SoverB_exp_noMVAcuts->SetLineColor(kColor_noMVAcuts);
  h_SoverB_exp_noMVAcuts->SetMarkerColor(kColor_noMVAcuts);
  h_SoverB_exp_noMVAcuts->SetMarkerStyle(kMarkerStyle_noMVAcuts);
  h_SoverB_exp_ideal->SetLineColor(kColor_noMVAcuts);
  h_SoverB_exp_ideal->SetMarkerColor(kColor_noMVAcuts);
  h_SoverB_exp_ideal->SetMarkerStyle(kMarkerStyle_ideal);

  
  h_significance_class_1->SetLineColor(kColor_1);
  h_significance_class_1->SetMarkerColor(kColor_1);
  h_significance_class_1->SetMarkerStyle(kMarkerStyle_1);
  h_significance_exp_1->SetLineColor(kColor_1);
  h_significance_exp_1->SetMarkerColor(kColor_1);
  h_significance_exp_1->SetMarkerStyle(kMarkerStyle_1);
  h_SoverB_class_1->SetLineColor(kColor_1);
  h_SoverB_class_1->SetMarkerColor(kColor_1);
  h_SoverB_class_1->SetMarkerStyle(kMarkerStyle_1);
  h_SoverB_exp_1->SetLineColor(kColor_1);
  h_SoverB_exp_1->SetMarkerColor(kColor_1);
  h_SoverB_exp_1->SetMarkerStyle(kMarkerStyle_1);
  
  h_significance_class_2->SetLineColor(kColor_2);
  h_significance_class_2->SetMarkerColor(kColor_2);
  h_significance_exp_2->SetLineColor(kColor_2);
  h_significance_exp_2->SetMarkerColor(kColor_2);
  h_SoverB_class_2->SetLineColor(kColor_2);
  h_SoverB_class_2->SetMarkerColor(kColor_2);
  h_SoverB_exp_2->SetLineColor(kColor_2);
  h_SoverB_exp_2->SetMarkerColor(kColor_2);

  h_significance_class_3->SetLineColor(kColor_3);
  h_significance_class_3->SetMarkerColor(kColor_3);
  h_significance_class_3->SetMarkerStyle(kMarkerStyle_3);
  h_significance_exp_3->SetLineColor(kColor_3);
  h_significance_exp_3->SetMarkerColor(kColor_3);
  h_significance_exp_3->SetMarkerStyle(kMarkerStyle_3);
  h_SoverB_class_3->SetLineColor(kColor_3);
  h_SoverB_class_3->SetMarkerColor(kColor_3);
  h_SoverB_class_3->SetMarkerStyle(kMarkerStyle_3);
  h_SoverB_exp_3->SetLineColor(kColor_3);
  h_SoverB_exp_3->SetMarkerColor(kColor_3);
  h_SoverB_exp_3->SetMarkerStyle(kMarkerStyle_3);

  h_significance_class_4->SetLineColor(kColor_4);
  h_significance_class_4->SetMarkerColor(kColor_4);
  h_significance_class_4->SetMarkerStyle(kMarkerStyle_4);
  h_significance_exp_4->SetLineColor(kColor_4);
  h_significance_exp_4->SetMarkerColor(kColor_4);
  h_significance_exp_4->SetMarkerStyle(kMarkerStyle_4);
  h_SoverB_class_4->SetLineColor(kColor_4);
  h_SoverB_class_4->SetMarkerColor(kColor_4);
  h_SoverB_class_4->SetMarkerStyle(kMarkerStyle_4);
  h_SoverB_exp_4->SetLineColor(kColor_4);
  h_SoverB_exp_4->SetMarkerColor(kColor_4);
  h_SoverB_exp_4->SetMarkerStyle(kMarkerStyle_4);

  h_eff_S_1->SetLineColor(kColor_1);
  h_eff_S_1->SetMarkerColor(kColor_1);
  h_eff_S_2->SetMarkerColor(kColor_2);
  h_eff_S_2->SetLineColor(kColor_2);
  h_eff_S_3->SetMarkerColor(kColor_3);
  h_eff_S_3->SetLineColor(kColor_3);
  h_eff_S_4->SetMarkerColor(kColor_4);
  h_eff_S_4->SetLineColor(kColor_4);

  h_eff_CombiWithConvLeg_1->SetLineColor(kColor_1);
  h_eff_CombiWithConvLeg_1->SetMarkerColor(kColor_1);
  h_eff_CombiWithConvLeg_2->SetLineColor(kColor_2);
  h_eff_CombiWithConvLeg_2->SetMarkerColor(kColor_2);
  h_eff_CombiWithConvLeg_3->SetLineColor(kColor_3);
  h_eff_CombiWithConvLeg_3->SetMarkerColor(kColor_3);
  h_eff_CombiWithConvLeg_4->SetLineColor(kColor_4);
  h_eff_CombiWithConvLeg_4->SetMarkerColor(kColor_4);

  h_eff_CombiWithoutConvLeg_1->SetLineColor(kColor_1);
  h_eff_CombiWithoutConvLeg_1->SetMarkerColor(kColor_1);
  h_eff_CombiWithoutConvLeg_2->SetLineColor(kColor_2);
  h_eff_CombiWithoutConvLeg_2->SetMarkerColor(kColor_2);
  h_eff_CombiWithoutConvLeg_3->SetLineColor(kColor_3);
  h_eff_CombiWithoutConvLeg_3->SetMarkerColor(kColor_3);
  h_eff_CombiWithoutConvLeg_4->SetLineColor(kColor_4);
  h_eff_CombiWithoutConvLeg_4->SetMarkerColor(kColor_4);

  h_eff_HF_1->SetLineColor(kColor_1);
  h_eff_HF_1->SetMarkerColor(kColor_1);
  h_eff_HF_2->SetLineColor(kColor_2);
  h_eff_HF_2->SetMarkerColor(kColor_2);
  h_eff_HF_3->SetLineColor(kColor_3);
  h_eff_HF_3->SetMarkerColor(kColor_3);
  h_eff_HF_4->SetLineColor(kColor_4);
  h_eff_HF_4->SetMarkerColor(kColor_4);

  h_eff_RPConv_1->SetLineColor(kColor_1);
  h_eff_RPConv_1->SetMarkerColor(kColor_1);
  h_eff_RPConv_2->SetLineColor(kColor_2);
  h_eff_RPConv_2->SetMarkerColor(kColor_2);
  h_eff_RPConv_3->SetLineColor(kColor_3);
  h_eff_RPConv_3->SetMarkerColor(kColor_3);
  h_eff_RPConv_4->SetLineColor(kColor_4);
  h_eff_RPConv_4->SetMarkerColor(kColor_4);
  
  h_eff_S_1->SetMarkerStyle(kMarkerStyle_1);
  h_eff_S_2->SetMarkerStyle(kMarkerStyle_2);
  h_eff_S_3->SetMarkerStyle(kMarkerStyle_3);
  h_eff_S_4->SetMarkerStyle(kMarkerStyle_4);

  h_eff_CombiWithConvLeg_1->SetMarkerStyle(kMarkerStyle_1);
  h_eff_CombiWithConvLeg_2->SetMarkerStyle(kMarkerStyle_2);
  h_eff_CombiWithConvLeg_3->SetMarkerStyle(kMarkerStyle_3);
  h_eff_CombiWithConvLeg_4->SetMarkerStyle(kMarkerStyle_4);

  h_eff_CombiWithoutConvLeg_1->SetMarkerStyle(kMarkerStyle_1);
  h_eff_CombiWithoutConvLeg_2->SetMarkerStyle(kMarkerStyle_2);
  h_eff_CombiWithoutConvLeg_3->SetMarkerStyle(kMarkerStyle_3);
  h_eff_CombiWithoutConvLeg_4->SetMarkerStyle(kMarkerStyle_4);

  h_eff_HF_1->SetMarkerStyle(kMarkerStyle_1);
  h_eff_HF_2->SetMarkerStyle(kMarkerStyle_2);
  h_eff_HF_3->SetMarkerStyle(kMarkerStyle_3);
  h_eff_HF_4->SetMarkerStyle(kMarkerStyle_4);

  h_eff_RPConv_1->SetMarkerStyle(kMarkerStyle_1);
  h_eff_RPConv_2->SetMarkerStyle(kMarkerStyle_2);
  h_eff_RPConv_3->SetMarkerStyle(kMarkerStyle_3);
  h_eff_RPConv_4->SetMarkerStyle(kMarkerStyle_4);


  h_significance_class_ideal->SetXTitle("M_{ee} / (GeV/c^{2})");
  h_significance_class_ideal->SetYTitle("S_{class} / #sqrt{S_{class} + B_{class}}");
  h_significance_class_ideal->GetXaxis()->SetTitleOffset(1.2);
  h_significance_class_ideal->GetYaxis()->SetTitleOffset(1.4);

  h_significance_exp_ideal->SetXTitle("M_{ee} / (GeV/c^{2})");
  h_significance_exp_ideal->SetYTitle("S_{exp} / #sqrt{S_{exp} + B_{exp}}");
  h_significance_exp_ideal->GetXaxis()->SetTitleOffset(1.2);
  h_significance_exp_ideal->GetYaxis()->SetTitleOffset(1.4);
  h_significance_exp_ideal->GetYaxis()->SetRangeUser(1e-2,1e3);

  h_SoverB_class_1->SetXTitle("M_{ee} / (GeV/c^{2})");
  h_SoverB_class_1->SetYTitle("S_{class} / B_{class}");
  h_SoverB_class_1->GetXaxis()->SetTitleOffset(1.2);
  h_SoverB_class_1->GetYaxis()->SetTitleOffset(1.4);


  h_eff_CombiWithConvLeg_1->SetXTitle("M_{ee} / (GeV/c^{2})");
  h_eff_CombiWithConvLeg_1->SetYTitle("Efficiency");
  h_eff_CombiWithConvLeg_1->GetXaxis()->SetTitleOffset(1.2);
  h_eff_CombiWithConvLeg_1->GetYaxis()->SetTitleOffset(1.4);

  
  gStyle->SetOptStat(0);


  TLegend *leg1 = new TLegend(.65, .7, .95, .95);
  leg1->AddEntry(h_significance_class_1, leg_label1, "p");
  leg1->AddEntry(h_significance_class_4, leg_label4, "p");
  leg1->AddEntry(h_significance_class_3, leg_label3, "p");
  leg1->AddEntry(h_significance_class_2, leg_label2, "p");
  leg1->AddEntry(h_significance_class_noMVAcuts, leg_label_noMVAcuts, "p");
  leg1->AddEntry(h_significance_class_ideal, leg_label_ideal, "p");

  TLegend *leg2 = new TLegend(.65, .7, .95, .95);
  leg2->AddEntry(h_significance_exp_1, leg_label1, "p");
  leg2->AddEntry(h_significance_exp_4, leg_label4, "p");
  leg2->AddEntry(h_significance_exp_3, leg_label3, "p");
  leg2->AddEntry(h_significance_exp_2, leg_label2, "p");
  leg2->AddEntry(h_significance_exp_noMVAcuts, leg_label_noMVAcuts, "p");
  leg2->AddEntry(h_significance_exp_ideal, leg_label_ideal, "p");

  TLegend *leg3 = new TLegend(.65, .7, .95, .95);
  leg3->AddEntry(h_SoverB_class_1, leg_label1, "p");
  leg3->AddEntry(h_SoverB_class_4, leg_label4, "p");
  leg3->AddEntry(h_SoverB_class_3, leg_label3, "p");
  leg3->AddEntry(h_SoverB_class_2, leg_label2, "p");
  leg3->AddEntry(h_SoverB_class_noMVAcuts, leg_label_noMVAcuts, "p");

  TLegend *leg4 = new TLegend(.65, .7, .95, .95);
  leg4->AddEntry(h_SoverB_exp_1, leg_label1, "p");
  leg4->AddEntry(h_SoverB_exp_4, leg_label4, "p");
  leg4->AddEntry(h_SoverB_exp_3, leg_label3, "p");
  leg4->AddEntry(h_SoverB_exp_2, leg_label2, "p");
  leg4->AddEntry(h_SoverB_exp_noMVAcuts, leg_label_noMVAcuts, "p");

  TLegend *leg5 = new TLegend(.65, .7, .95, .95);
  leg5->AddEntry(h_eff_S_1, "S", "l");
  leg5->AddEntry(h_eff_CombiWithConvLeg_1, "Comb. w. conv. leg", "l");
  leg5->AddEntry(h_eff_CombiWithoutConvLeg_1, "Comb. w/o conv. leg", "l");
  leg5->AddEntry(h_eff_HF_1, "Corr. HF", "l");
  leg5->AddEntry(h_eff_RPConv_1, "RP Conv.", "l");

  TLegend *leg_eff = new TLegend(.65,.2,.88,.38);
  leg_eff->AddEntry(h_eff_S_1, leg_label1, "p");
  leg_eff->AddEntry(h_eff_S_4, leg_label4, "p");
  leg_eff->AddEntry(h_eff_S_3, leg_label3, "p");
  leg_eff->AddEntry(h_eff_S_2, leg_label2, "p");



  TString drawOptions = "hist p e1 x0";
  TString drawOptions_same = drawOptions + " same";

  TString drawOptions_eff = "hist p x0 e1";
  TString drawOptions_eff_same = drawOptions_eff + " same";
  
  
  TCanvas *c_significance_class = new TCanvas("c_significance_class","",1024,768);
  c_significance_class->SetGridy();
  c_significance_class->SetLogy();
  h_significance_class_ideal->Draw(drawOptions);
  h_significance_class_1->Draw(drawOptions_same);
  h_significance_class_2->Draw(drawOptions_same);
  h_significance_class_3->Draw(drawOptions_same);
  h_significance_class_4->Draw(drawOptions_same);
  h_significance_class_noMVAcuts->Draw(drawOptions_same);
  leg1->Draw();
  c_significance_class->SaveAs("temp_output/c_significance_class.pdf");
  c_significance_class->SaveAs("temp_output/c_significance_class.root");
  c_significance_class->SaveAs("temp_output/c_significance_class.png");

  TCanvas *c_significance_exp = new TCanvas("c_significance_exp","",1024,768);
  c_significance_exp->SetGridy();
  c_significance_exp->SetLogy();
  h_significance_exp_ideal->Draw(drawOptions);
  h_significance_exp_1->Draw(drawOptions_same);
  h_significance_exp_2->Draw(drawOptions_same);
  h_significance_exp_3->Draw(drawOptions_same);
  h_significance_exp_4->Draw(drawOptions_same);
  h_significance_exp_noMVAcuts->Draw(drawOptions_same);
  leg2->Draw();
  c_significance_exp->SaveAs("temp_output/c_significance_exp.pdf");
  c_significance_exp->SaveAs("temp_output/c_significance_exp.root");
  c_significance_exp->SaveAs("temp_output/c_significance_exp.png");


  TCanvas *c_SoverB_class = new TCanvas("c_SoverB_class","",1024,768);
  c_SoverB_class->SetGridy();
  h_SoverB_class_1->Draw(drawOptions);
  h_SoverB_class_2->Draw(drawOptions_same);
  h_SoverB_class_3->Draw(drawOptions_same);
  h_SoverB_class_4->Draw(drawOptions_same);
  h_SoverB_class_noMVAcuts->Draw(drawOptions_same);
  leg3->Draw();
  c_SoverB_class->SaveAs("temp_output/c_SoverB_class.pdf");
  c_SoverB_class->SaveAs("temp_output/c_SoverB_class.root");
  c_SoverB_class->SaveAs("temp_output/c_SoverB_class.png");


  TCanvas *c_SoverB_exp = new TCanvas("c_SoverB_exp","",1024,768);
  c_SoverB_exp->SetGridy();
  c_SoverB_exp->SetLogy();
  h_SoverB_exp_1->Draw(drawOptions);
  h_SoverB_exp_2->Draw(drawOptions_same);
  h_SoverB_exp_3->Draw(drawOptions_same);
  h_SoverB_exp_4->Draw(drawOptions_same);
  h_SoverB_exp_noMVAcuts->Draw(drawOptions_same);
  h_SoverB_exp_ideal->Draw(drawOptions_same);
  leg4->Draw();
  c_SoverB_exp->SaveAs("temp_output/c_SoverB_exp.pdf");
  c_SoverB_exp->SaveAs("temp_output/c_SoverB_exp.root");
  c_SoverB_exp->SaveAs("temp_output/c_SoverB_exp.png");
  


  h_eff_S_1->SetXTitle("M_{ee} / (GeV/c^{2})");
  h_eff_S_1->SetYTitle("Efficiency (S)");
  h_eff_S_1->GetXaxis()->SetTitleOffset(1.2);
  h_eff_S_1->GetYaxis()->SetTitleOffset(1.4);
  h_eff_S_1->GetYaxis()->SetRangeUser(0, 1.2);


  h_eff_CombiWithConvLeg_1->SetXTitle("M_{ee} / (GeV/c^{2})");
  h_eff_CombiWithConvLeg_1->SetYTitle("Efficiency (Comb. w. conv. leg)");
  h_eff_CombiWithConvLeg_1->GetXaxis()->SetTitleOffset(1.2);
  h_eff_CombiWithConvLeg_1->GetYaxis()->SetTitleOffset(1.4);
  h_eff_CombiWithConvLeg_1->GetYaxis()->SetRangeUser(0, 1.2);

  h_eff_CombiWithoutConvLeg_1->SetXTitle("M_{ee} / (GeV/c^{2})");
  h_eff_CombiWithoutConvLeg_1->SetYTitle("Efficiency (Comb. w/o conv. leg)");
  h_eff_CombiWithoutConvLeg_1->GetXaxis()->SetTitleOffset(1.2);
  h_eff_CombiWithoutConvLeg_1->GetYaxis()->SetTitleOffset(1.4);
  h_eff_CombiWithoutConvLeg_1->GetYaxis()->SetRangeUser(0, 1.2);

  h_eff_HF_1->SetXTitle("M_{ee} / (GeV/c^{2})");
  h_eff_HF_1->SetYTitle("Efficiency (corr. HF)");
  h_eff_HF_1->GetXaxis()->SetTitleOffset(1.2);
  h_eff_HF_1->GetYaxis()->SetTitleOffset(1.4);
  h_eff_HF_1->GetYaxis()->SetRangeUser(0, 1.2);

  h_eff_RPConv_1->SetXTitle("M_{ee} / (GeV/c^{2})");
  h_eff_RPConv_1->SetYTitle("Efficiency (RP conv.)");
  h_eff_RPConv_1->GetXaxis()->SetTitleOffset(1.2);
  h_eff_RPConv_1->GetYaxis()->SetTitleOffset(1.4);
  h_eff_RPConv_1->GetYaxis()->SetRangeUser(0, 1.2);


  
  
  TCanvas *c_effs_S = new TCanvas("c_effs_S","",1024,768);
  c_effs_S->SetGridy();
  h_eff_S_1->Draw(drawOptions_eff);
  h_eff_S_2->Draw(drawOptions_eff_same);
  h_eff_S_3->Draw(drawOptions_eff_same);
  h_eff_S_4->Draw(drawOptions_eff_same);
  leg_eff->Draw();
  c_effs_S->SaveAs("temp_output/c_effs_S.pdf");
  c_effs_S->SaveAs("temp_output/c_effs_S.root");
  c_effs_S->SaveAs("temp_output/c_effs_S.png");

  TCanvas *c_effs_CombiWithConvLeg = new TCanvas("c_effs_CombiWithConvLeg","",1024,768);
  c_effs_CombiWithConvLeg->SetGridy();
  h_eff_CombiWithConvLeg_1->Draw(drawOptions_eff);
  h_eff_CombiWithConvLeg_2->Draw(drawOptions_eff_same);
  h_eff_CombiWithConvLeg_3->Draw(drawOptions_eff_same);
  h_eff_CombiWithConvLeg_4->Draw(drawOptions_eff_same);
  leg_eff->Draw();
  c_effs_CombiWithConvLeg->SaveAs("temp_output/c_effs_CombiWithConvLeg.pdf");
  c_effs_CombiWithConvLeg->SaveAs("temp_output/c_effs_CombiWithConvLeg.root");
  c_effs_CombiWithConvLeg->SaveAs("temp_output/c_effs_CombiWithConvLeg.png");

  TCanvas *c_effs_CombiWithoutConvLeg = new TCanvas("c_effs_CombiWithoutConvLeg","",1024,768);
  c_effs_CombiWithoutConvLeg->SetGridy();
  h_eff_CombiWithoutConvLeg_1->Draw(drawOptions_eff);
  h_eff_CombiWithoutConvLeg_2->Draw(drawOptions_eff_same);
  h_eff_CombiWithoutConvLeg_3->Draw(drawOptions_eff_same);
  h_eff_CombiWithoutConvLeg_4->Draw(drawOptions_eff_same);
  leg_eff->Draw();
  c_effs_CombiWithoutConvLeg->SaveAs("temp_output/c_effs_CombiWithoutConvLeg.pdf");
  c_effs_CombiWithoutConvLeg->SaveAs("temp_output/c_effs_CombiWithoutConvLeg.root");
  c_effs_CombiWithoutConvLeg->SaveAs("temp_output/c_effs_CombiWithoutConvLeg.png");

  TCanvas *c_effs_HF = new TCanvas("c_effs_HF","",1024,768);
  c_effs_HF->SetGridy();
  h_eff_HF_1->Draw(drawOptions_eff);
  h_eff_HF_2->Draw(drawOptions_eff_same);
  h_eff_HF_3->Draw(drawOptions_eff_same);
  h_eff_HF_4->Draw(drawOptions_eff_same);
  leg_eff->Draw();
  c_effs_HF->SaveAs("temp_output/c_effs_HF.pdf");
  c_effs_HF->SaveAs("temp_output/c_effs_HF.root");
  c_effs_HF->SaveAs("temp_output/c_effs_HF.png");

  TCanvas *c_effs_RPConv = new TCanvas("c_effs_RPConv","",1024,768);
  c_effs_RPConv->SetGridy();
  h_eff_RPConv_1->Draw(drawOptions_eff);
  h_eff_RPConv_2->Draw(drawOptions_eff_same);
  h_eff_RPConv_3->Draw(drawOptions_eff_same);
  h_eff_RPConv_4->Draw(drawOptions_eff_same);
  leg_eff->Draw();
  c_effs_RPConv->SaveAs("temp_output/c_effs_RPConv.pdf");
  c_effs_RPConv->SaveAs("temp_output/c_effs_RPConv.root");
  c_effs_RPConv->SaveAs("temp_output/c_effs_RPConv.png");    

  
  h_eff_S_1->SetLineColor(kColor_4);
  h_eff_S_1->SetMarkerColor(kColor_4);
  h_eff_S_1->SetMarkerStyle(kMarker_1);
  h_eff_CombiWithConvLeg_1->SetLineColor(kColor_2);
  h_eff_CombiWithConvLeg_1->SetMarkerColor(kColor_2);
  h_eff_CombiWithConvLeg_1->SetMarkerStyle(kMarker_1);
  h_eff_CombiWithoutConvLeg_1->SetLineColor(kColor_3);
  h_eff_CombiWithoutConvLeg_1->SetMarkerColor(kColor_3);
  h_eff_CombiWithoutConvLeg_1->SetMarkerStyle(kMarker_1);
  h_eff_HF_1->SetLineColor(kOrange);
  h_eff_HF_1->SetMarkerColor(kOrange);
  h_eff_HF_1->SetMarkerStyle(kMarker_1);
  h_eff_RPConv_1->SetLineColor(kMagenta);
  h_eff_RPConv_1->SetMarkerColor(kMagenta);
  h_eff_RPConv_1->SetMarkerStyle(kMarker_1);

  h_eff_S_2->SetLineColor(kColor_4);
  h_eff_S_2->SetMarkerColor(kColor_4);
  h_eff_S_2->SetMarkerStyle(kMarker_2);
  h_eff_CombiWithConvLeg_2->SetLineColor(kColor_2);
  h_eff_CombiWithConvLeg_2->SetMarkerColor(kColor_2);
  h_eff_CombiWithConvLeg_2->SetMarkerStyle(kMarker_2);
  h_eff_CombiWithoutConvLeg_2->SetLineColor(kColor_3);
  h_eff_CombiWithoutConvLeg_2->SetMarkerColor(kColor_3);
  h_eff_CombiWithoutConvLeg_2->SetMarkerStyle(kMarker_2);
  h_eff_HF_2->SetLineColor(kOrange);
  h_eff_HF_2->SetMarkerColor(kOrange);
  h_eff_HF_2->SetMarkerStyle(kMarker_2);
  h_eff_RPConv_2->SetLineColor(kMagenta);
  h_eff_RPConv_2->SetMarkerColor(kMagenta);
  h_eff_RPConv_2->SetMarkerStyle(kMarker_2);

  h_eff_S_3->SetLineColor(kColor_4);
  h_eff_S_3->SetMarkerColor(kColor_4);
  h_eff_S_3->SetMarkerStyle(kMarker_3);
  h_eff_CombiWithConvLeg_3->SetLineColor(kColor_2);
  h_eff_CombiWithConvLeg_3->SetMarkerColor(kColor_2);
  h_eff_CombiWithConvLeg_3->SetMarkerStyle(kMarker_3);
  h_eff_CombiWithoutConvLeg_3->SetLineColor(kColor_3);
  h_eff_CombiWithoutConvLeg_3->SetMarkerColor(kColor_3);
  h_eff_CombiWithoutConvLeg_3->SetMarkerStyle(kMarker_3);
  h_eff_HF_3->SetLineColor(kOrange);
  h_eff_HF_3->SetMarkerColor(kOrange);
  h_eff_HF_3->SetMarkerStyle(kMarker_3);
  h_eff_RPConv_3->SetLineColor(kMagenta);
  h_eff_RPConv_3->SetMarkerColor(kMagenta);
  h_eff_RPConv_3->SetMarkerStyle(kMarker_3);

  h_eff_S_4->SetLineColor(kColor_4);
  h_eff_S_4->SetMarkerColor(kColor_4);
  h_eff_S_4->SetMarkerStyle(kMarker_4);
  h_eff_CombiWithConvLeg_4->SetLineColor(kColor_2);
  h_eff_CombiWithConvLeg_4->SetMarkerColor(kColor_2);
  h_eff_CombiWithConvLeg_4->SetMarkerStyle(kMarker_4);
  h_eff_CombiWithoutConvLeg_4->SetLineColor(kColor_3);
  h_eff_CombiWithoutConvLeg_4->SetMarkerColor(kColor_3);
  h_eff_CombiWithoutConvLeg_4->SetMarkerStyle(kMarker_4);
  h_eff_HF_4->SetLineColor(kOrange);
  h_eff_HF_4->SetMarkerColor(kOrange);
  h_eff_HF_4->SetMarkerStyle(kMarker_4);
  h_eff_RPConv_4->SetLineColor(kMagenta);
  h_eff_RPConv_4->SetMarkerColor(kMagenta);
  h_eff_RPConv_4->SetMarkerStyle(kMarker_4);


  
  h_eff_S_1->SetYTitle("Efficiency");
  
  TCanvas *c_effs_all = new TCanvas("c_effs","",1024,768);
  c_effs_all->SetGridy();
  h_eff_S_1->Draw(drawOptions_eff);
  h_eff_S_2->Draw(drawOptions_eff_same);
  h_eff_S_3->Draw(drawOptions_eff_same);
  h_eff_S_4->Draw(drawOptions_eff_same);
  h_eff_CombiWithConvLeg_1->Draw(drawOptions_eff_same);
  h_eff_CombiWithConvLeg_2->Draw(drawOptions_eff_same);
  h_eff_CombiWithConvLeg_3->Draw(drawOptions_eff_same);
  h_eff_CombiWithConvLeg_4->Draw(drawOptions_eff_same);
  h_eff_CombiWithoutConvLeg_1->Draw(drawOptions_eff_same);
  h_eff_CombiWithoutConvLeg_2->Draw(drawOptions_eff_same);
  h_eff_CombiWithoutConvLeg_3->Draw(drawOptions_eff_same);
  h_eff_CombiWithoutConvLeg_4->Draw(drawOptions_eff_same);
  h_eff_HF_1->Draw(drawOptions_eff_same);
  h_eff_HF_2->Draw(drawOptions_eff_same);
  h_eff_HF_3->Draw(drawOptions_eff_same);
  h_eff_HF_4->Draw(drawOptions_eff_same);
  h_eff_RPConv_1->Draw(drawOptions_eff_same);
  h_eff_RPConv_2->Draw(drawOptions_eff_same);
  h_eff_RPConv_3->Draw(drawOptions_eff_same);
  h_eff_RPConv_4->Draw(drawOptions_eff_same);
  leg5->Draw();
  c_effs_all->SaveAs("temp_output/c_effs_all.pdf");
  c_effs_all->SaveAs("temp_output/c_effs_all.root");
  c_effs_all->SaveAs("temp_output/c_effs_all.png");


  gApplication->Terminate();
}
