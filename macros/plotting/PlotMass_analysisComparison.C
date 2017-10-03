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
#include <TPad.h>
#include <TLegend.h>
#include <TLatex.h>



void PlotMass_analysisComparison() {
  
  // RP conv. rej. via classical cuts:
  TString infile1_filename = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/properPrefiltering/RPConvRejClassicalCuts/massPlots/standardCuts/temp_output/mass_histos.root";

  // RP conv. rej. via MVA cuts (tight cuts):
  TString infile2_filename = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/properPrefiltering/RPConvRejMVA/tightCuts/massPlots/temp_output/mass_histos.root";

  // RP conv. rej. via MVA cuts (tight cuts + loose cuts):
  TString infile21_filename = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/properPrefiltering/RPConvRejMVA/tightCuts_looseCuts/massPlots/temp_output/mass_histos.root";

  // RP conv. rej. via MVA cuts (tight cuts + loose cuts + all tracks):
  TString infile22_filename = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/properPrefiltering/RPConvRejMVA/tightCuts_looseCuts_allTracks/massPlots/temp_output/mass_histos.root";

  // single conv. track rej. via MVA cuts:
  TString infile3_filename = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/properPrefiltering/singleConvTrackRejMVA/massPlots/temp_output/mass_histos.root";

  // Comb. conv. rej. via MVA cuts:
  TString infile4_filename = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/properPrefiltering/combConvRejMVA/massPlots/temp_output/mass_histos.root";

  TString infile_noMVAcuts_filename = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/properPrefiltering/combConvRejMVA/massPlots/temp_output/mass_histos.root";


  TString leg_label1 = "classical cuts";
  TString leg_label2 = "MVA 3 (track cuts: 2)";
  TString leg_label21 = "MVA 3 (track cuts: 1+2)";
  TString leg_label22 = "MVA 3 (track cuts: 1+2+3)";
  TString leg_label3 = "MVA 2";
  TString leg_label4 = "MVA 1";
  TString leg_label_noMVAcuts = "no cuts";
  TString leg_label_ideal = "ideal backgr. rejection";

  const Int_t kMarker_1 = 24;
  const Int_t kMarker_2 = 25;
  const Int_t kMarker_21 = 2;
  const Int_t kMarker_22 = 46;
  const Int_t kMarker_3 = 26;
  const Int_t kMarker_4 = 32;

  const Int_t kColor_1 = kBlack;
  const Int_t kColor_2 = kBlue;
  const Int_t kColor_21 = kAzure+6;
  const Int_t kColor_22 = kBlue;
  const Int_t kColor_3 = kRed;
  const Int_t kColor_4 = kGreen+2;
  const Int_t kColor_noMVAcuts = kGray+2;


  //////////////////////////////////////////////////////////////////////////////
  
  SetStyle();
  gROOT->ForceStyle();

  TFile *infile1 = new TFile(infile1_filename, "READ");
  TH1D *h_significance_class_1      = (TH1D*)infile1->Get("h_significance_class");
  TH1D *h_significance_class_1_norm = (TH1D*)infile1->Get("h_significance_class_norm");
  TH1D *h_significance_exp_1        = (TH1D*)infile1->Get("h_significance_exp");
  TH1D *h_significance_exp_1_norm   = (TH1D*)infile1->Get("h_significance_exp_norm");
  TH1D *h_SoverB_class_1            = (TH1D*)infile1->Get("h_SoverB_class");
  TH1D *h_SoverB_class_1_norm       = (TH1D*)infile1->Get("h_SoverB_class_norm");
  TH1D *h_SoverB_exp_1              = (TH1D*)infile1->Get("h_SoverB_exp");
  TH1D *h_SoverB_exp_1_norm         = (TH1D*)infile1->Get("h_SoverB_exp_norm");
  TH1D *h_eff_S_1                   = (TH1D*)infile1->Get("h_S_eff");
  TH1D *h_eff_CombiWithConvLeg_1    = (TH1D*)infile1->Get("h_CombiWithConvLeg_eff");
  TH1D *h_eff_CombiWithoutConvLeg_1 = (TH1D*)infile1->Get("h_CombiWithoutConvLeg_eff");
  TH1D *h_eff_HF_1                  = (TH1D*)infile1->Get("h_HF_eff");
  TH1D *h_eff_RPConv_1              = (TH1D*)infile1->Get("h_RPConv_eff");
  h_significance_class_1->SetName("h_significance_class_1");
  h_significance_exp_1->SetName("h_significance_exp_1");
  h_SoverB_class_1->SetName("h_SoverB_class_1");
  h_SoverB_exp_1->SetName("h_SoverB_exp_1");

  TFile *infile2 = new TFile(infile2_filename, "READ");
  TH1D *h_significance_class_2      = (TH1D*)infile2->Get("h_significance_class");
  TH1D *h_significance_class_2_norm = (TH1D*)infile2->Get("h_significance_class_norm");
  TH1D *h_significance_exp_2        = (TH1D*)infile2->Get("h_significance_exp");
  TH1D *h_significance_exp_2_norm   = (TH1D*)infile2->Get("h_significance_exp_norm");
  TH1D *h_SoverB_class_2            = (TH1D*)infile2->Get("h_SoverB_class");
  TH1D *h_SoverB_class_2_norm       = (TH1D*)infile2->Get("h_SoverB_class_norm");
  TH1D *h_SoverB_exp_2              = (TH1D*)infile2->Get("h_SoverB_exp");
  TH1D *h_SoverB_exp_2_norm         = (TH1D*)infile2->Get("h_SoverB_exp_norm");
  TH1D *h_eff_S_2                   = (TH1D*)infile2->Get("h_S_eff");
  TH1D *h_eff_CombiWithConvLeg_2    = (TH1D*)infile2->Get("h_CombiWithConvLeg_eff");
  TH1D *h_eff_CombiWithoutConvLeg_2 = (TH1D*)infile2->Get("h_CombiWithoutConvLeg_eff");
  TH1D *h_eff_HF_2                  = (TH1D*)infile2->Get("h_HF_eff");
  TH1D *h_eff_RPConv_2              = (TH1D*)infile2->Get("h_RPConv_eff");
  h_significance_class_2->SetName("h_significance_class_2");
  h_significance_exp_2->SetName("h_significance_exp_2");
  h_SoverB_class_2->SetName("h_SoverB_class_2");
  h_SoverB_exp_2->SetName("h_SoverB_exp_2");

  TFile *infile21 = new TFile(infile21_filename, "READ");
  TH1D *h_significance_class_21      = (TH1D*)infile21->Get("h_significance_class");
  TH1D *h_significance_class_21_norm = (TH1D*)infile21->Get("h_significance_class_norm");
  TH1D *h_significance_exp_21        = (TH1D*)infile21->Get("h_significance_exp");
  TH1D *h_significance_exp_21_norm   = (TH1D*)infile21->Get("h_significance_exp_norm");
  TH1D *h_SoverB_class_21            = (TH1D*)infile21->Get("h_SoverB_class");
  TH1D *h_SoverB_class_21_norm       = (TH1D*)infile21->Get("h_SoverB_class_norm");
  TH1D *h_SoverB_exp_21              = (TH1D*)infile21->Get("h_SoverB_exp");
  TH1D *h_SoverB_exp_21_norm         = (TH1D*)infile21->Get("h_SoverB_exp_norm");
  TH1D *h_eff_S_21                   = (TH1D*)infile21->Get("h_S_eff");
  TH1D *h_eff_CombiWithConvLeg_21    = (TH1D*)infile21->Get("h_CombiWithConvLeg_eff");
  TH1D *h_eff_CombiWithoutConvLeg_21 = (TH1D*)infile21->Get("h_CombiWithoutConvLeg_eff");
  TH1D *h_eff_HF_21                  = (TH1D*)infile21->Get("h_HF_eff");
  TH1D *h_eff_RPConv_21              = (TH1D*)infile21->Get("h_RPConv_eff");
  h_significance_class_21->SetName("h_significance_class_21");
  h_significance_exp_21->SetName("h_significance_exp_21");
  h_SoverB_class_21->SetName("h_SoverB_class_21");
  h_SoverB_exp_21->SetName("h_SoverB_exp_21");

  TFile *infile22 = new TFile(infile22_filename, "READ");
  TH1D *h_significance_class_22      = (TH1D*)infile22->Get("h_significance_class");
  TH1D *h_significance_class_22_norm = (TH1D*)infile22->Get("h_significance_class_norm");
  TH1D *h_significance_exp_22        = (TH1D*)infile22->Get("h_significance_exp");
  TH1D *h_significance_exp_22_norm   = (TH1D*)infile22->Get("h_significance_exp_norm");
  TH1D *h_SoverB_class_22            = (TH1D*)infile22->Get("h_SoverB_class");
  TH1D *h_SoverB_class_22_norm       = (TH1D*)infile22->Get("h_SoverB_class_norm");
  TH1D *h_SoverB_exp_22              = (TH1D*)infile22->Get("h_SoverB_exp");
  TH1D *h_SoverB_exp_22_norm         = (TH1D*)infile22->Get("h_SoverB_exp_norm");
  TH1D *h_eff_S_22                   = (TH1D*)infile22->Get("h_S_eff");
  TH1D *h_eff_CombiWithConvLeg_22    = (TH1D*)infile22->Get("h_CombiWithConvLeg_eff");
  TH1D *h_eff_CombiWithoutConvLeg_22 = (TH1D*)infile22->Get("h_CombiWithoutConvLeg_eff");
  TH1D *h_eff_HF_22                  = (TH1D*)infile22->Get("h_HF_eff");
  TH1D *h_eff_RPConv_22              = (TH1D*)infile22->Get("h_RPConv_eff");
  h_significance_class_22->SetName("h_significance_class_22");
  h_significance_exp_22->SetName("h_significance_exp_22");
  h_SoverB_class_22->SetName("h_SoverB_class_22");
  h_SoverB_exp_22->SetName("h_SoverB_exp_22");
  
  TFile *infile3 = new TFile(infile3_filename, "READ");
  TH1D *h_significance_class_3      = (TH1D*)infile3->Get("h_significance_class");
  TH1D *h_significance_class_3_norm = (TH1D*)infile3->Get("h_significance_class_norm");
  TH1D *h_significance_exp_3        = (TH1D*)infile3->Get("h_significance_exp");
  TH1D *h_significance_exp_3_norm   = (TH1D*)infile3->Get("h_significance_exp_norm");
  TH1D *h_SoverB_class_3            = (TH1D*)infile3->Get("h_SoverB_class");
  TH1D *h_SoverB_class_3_norm       = (TH1D*)infile3->Get("h_SoverB_class_norm");
  TH1D *h_SoverB_exp_3              = (TH1D*)infile3->Get("h_SoverB_exp");
  TH1D *h_SoverB_exp_3_norm         = (TH1D*)infile3->Get("h_SoverB_exp_norm");
  TH1D *h_eff_S_3                   = (TH1D*)infile3->Get("h_S_eff");
  TH1D *h_eff_CombiWithConvLeg_3    = (TH1D*)infile3->Get("h_CombiWithConvLeg_eff");
  TH1D *h_eff_CombiWithoutConvLeg_3 = (TH1D*)infile3->Get("h_CombiWithoutConvLeg_eff");
  TH1D *h_eff_HF_3                  = (TH1D*)infile3->Get("h_HF_eff");
  TH1D *h_eff_RPConv_3              = (TH1D*)infile3->Get("h_RPConv_eff");
  h_significance_class_3->SetName("h_significance_class_3");
  h_significance_exp_3->SetName("h_significance_exp_3");
  h_SoverB_class_3->SetName("h_SoverB_class_3");
  h_SoverB_exp_3->SetName("h_SoverB_exp_3");
  
  TFile *infile4 = new TFile(infile4_filename, "READ");
  TH1D *h_significance_class_4      = (TH1D*)infile4->Get("h_significance_class");
  TH1D *h_significance_class_4_norm = (TH1D*)infile4->Get("h_significance_class_norm");
  TH1D *h_significance_exp_4        = (TH1D*)infile4->Get("h_significance_exp");
  TH1D *h_significance_exp_4_norm   = (TH1D*)infile4->Get("h_significance_exp_norm");
  TH1D *h_SoverB_class_4            = (TH1D*)infile4->Get("h_SoverB_class");
  TH1D *h_SoverB_class_4_norm       = (TH1D*)infile4->Get("h_SoverB_class_norm");
  TH1D *h_SoverB_exp_4              = (TH1D*)infile4->Get("h_SoverB_exp");
  TH1D *h_SoverB_exp_4_norm         = (TH1D*)infile4->Get("h_SoverB_exp_norm");
  TH1D *h_eff_S_4                   = (TH1D*)infile4->Get("h_S_eff");
  TH1D *h_eff_CombiWithConvLeg_4    = (TH1D*)infile4->Get("h_CombiWithConvLeg_eff");
  TH1D *h_eff_CombiWithoutConvLeg_4 = (TH1D*)infile4->Get("h_CombiWithoutConvLeg_eff");
  TH1D *h_eff_HF_4                  = (TH1D*)infile4->Get("h_HF_eff");
  TH1D *h_eff_RPConv_4              = (TH1D*)infile4->Get("h_RPConv_eff");
  h_significance_class_4->SetName("h_significance_class_4");
  h_significance_exp_4->SetName("h_significance_exp_4");
  h_SoverB_class_4->SetName("h_SoverB_class_4");
  h_SoverB_exp_4->SetName("h_SoverB_exp_4");

  TFile *infile_noMVAcuts = new TFile(infile_noMVAcuts_filename, "READ");
  TH1D *h_significance_class_noMVAcuts = (TH1D*)infile_noMVAcuts->Get("h_significance_class_noCuts");
  TH1D *h_significance_exp_noMVAcuts = (TH1D*)infile_noMVAcuts->Get("h_significance_exp_noCuts");
  TH1D *h_significance_class_ideal = (TH1D*)infile_noMVAcuts->Get("h_significance_class_ideal");
  TH1D *h_significance_exp_ideal = (TH1D*)infile_noMVAcuts->Get("h_significance_exp_ideal");
  TH1D *h_SoverB_class_noMVAcuts = (TH1D*)infile_noMVAcuts->Get("h_SoverB_class_noCuts");
  TH1D *h_SoverB_exp_noMVAcuts = (TH1D*)infile_noMVAcuts->Get("h_SoverB_exp_noCuts");
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
  const Int_t kMarkerStyle_21 = 2;
  const Int_t kMarkerStyle_22 = 24;
  const Int_t kMarkerStyle_3 = 23;
  const Int_t kMarkerStyle_4 = 26;
  const Int_t kMarkerStyle_ideal = 7;
  const Int_t kMarkerStyle_noMVAcuts = 5;

  const Float_t kLineWidth_1 = 3;
  const Float_t kLineWidth_2 = kLineWidth_1;
  const Float_t kLineWidth_21 = kLineWidth_2;
  const Float_t kLineWidth_22 = kLineWidth_2;
  const Float_t kLineWidth_3 = kLineWidth_1;
  const Float_t kLineWidth_4 = kLineWidth_1;
  const Float_t kLineWidth_ideal = kLineWidth_1;
  const Float_t kLineWidth_noMVAcuts = kLineWidth_1;
  
  
  h_significance_class_noMVAcuts->SetLineColor(kColor_noMVAcuts);
  h_significance_class_noMVAcuts->SetLineWidth(kLineWidth_noMVAcuts);
  h_significance_class_noMVAcuts->SetMarkerColor(kColor_noMVAcuts);
  h_significance_class_noMVAcuts->SetMarkerStyle(kMarkerStyle_noMVAcuts);
  h_significance_exp_noMVAcuts->SetLineColor(kColor_noMVAcuts);
  h_significance_exp_noMVAcuts->SetLineWidth(kLineWidth_noMVAcuts);
  h_significance_exp_noMVAcuts->SetMarkerColor(kColor_noMVAcuts);
  h_significance_exp_noMVAcuts->SetMarkerStyle(kMarkerStyle_noMVAcuts);
  h_significance_class_ideal->SetLineColor(kColor_noMVAcuts);
  h_significance_class_ideal->SetLineWidth(kLineWidth_ideal);
  h_significance_class_ideal->SetMarkerColor(kColor_noMVAcuts);
  h_significance_class_ideal->SetMarkerStyle(kMarkerStyle_ideal);
  h_significance_exp_ideal->SetLineColor(kColor_noMVAcuts);
  h_significance_exp_ideal->SetLineWidth(kLineWidth_ideal);
  h_significance_exp_ideal->SetMarkerColor(kColor_noMVAcuts);
  h_significance_exp_ideal->SetMarkerStyle(kMarkerStyle_ideal);
  
  h_SoverB_class_noMVAcuts->SetLineColor(kColor_noMVAcuts);
  h_SoverB_class_noMVAcuts->SetLineWidth(kLineWidth_noMVAcuts);
  h_SoverB_class_noMVAcuts->SetMarkerColor(kColor_noMVAcuts);
  h_SoverB_class_noMVAcuts->SetMarkerStyle(kMarkerStyle_noMVAcuts);
  h_SoverB_exp_noMVAcuts->SetLineColor(kColor_noMVAcuts);
  h_SoverB_exp_noMVAcuts->SetLineWidth(kLineWidth_noMVAcuts);
  h_SoverB_exp_noMVAcuts->SetMarkerColor(kColor_noMVAcuts);
  h_SoverB_exp_noMVAcuts->SetMarkerStyle(kMarkerStyle_noMVAcuts);
  h_SoverB_exp_ideal->SetLineColor(kColor_noMVAcuts);
  h_SoverB_exp_ideal->SetLineWidth(kLineWidth_noMVAcuts);
  h_SoverB_exp_ideal->SetMarkerColor(kColor_noMVAcuts);
  h_SoverB_exp_ideal->SetMarkerStyle(kMarkerStyle_ideal);

  
  h_significance_class_1->SetLineColor(kColor_1);
  h_significance_class_1->SetLineWidth(kLineWidth_1);
  h_significance_class_1->SetMarkerColor(kColor_1);
  h_significance_class_1->SetMarkerStyle(kMarkerStyle_1);
  h_significance_exp_1->SetLineColor(kColor_1);
  h_significance_exp_1->SetLineWidth(kLineWidth_1);
  h_significance_exp_1->SetMarkerColor(kColor_1);
  h_significance_exp_1->SetMarkerStyle(kMarkerStyle_1);
  h_SoverB_class_1->SetLineColor(kColor_1);
  h_SoverB_class_1->SetLineWidth(kLineWidth_1);
  h_SoverB_class_1->SetMarkerColor(kColor_1);
  h_SoverB_class_1->SetMarkerStyle(kMarkerStyle_1);
  h_SoverB_exp_1->SetLineColor(kColor_1);
  h_SoverB_exp_1->SetLineWidth(kLineWidth_1);
  h_SoverB_exp_1->SetMarkerColor(kColor_1);
  h_SoverB_exp_1->SetMarkerStyle(kMarkerStyle_1);
  
  h_significance_class_2->SetLineColor(kColor_2);
  h_significance_class_2->SetLineWidth(kLineWidth_2);
  h_significance_class_2->SetMarkerColor(kColor_2);
  h_significance_class_2->SetMarkerStyle(kMarkerStyle_2);
  h_significance_exp_2->SetLineColor(kColor_2);
  h_significance_exp_2->SetLineWidth(kLineWidth_2);
  h_significance_exp_2->SetMarkerColor(kColor_2);
  h_significance_exp_2->SetMarkerStyle(kMarkerStyle_2);
  h_SoverB_class_2->SetLineColor(kColor_2);
  h_SoverB_class_2->SetLineWidth(kLineWidth_2);
  h_SoverB_class_2->SetMarkerColor(kColor_2);
  h_SoverB_class_2->SetMarkerStyle(kMarkerStyle_2);
  h_SoverB_exp_2->SetLineColor(kColor_2);
  h_SoverB_exp_2->SetLineWidth(kLineWidth_2);
  h_SoverB_exp_2->SetMarkerColor(kColor_2);
  h_SoverB_exp_2->SetMarkerStyle(kMarkerStyle_2);

  h_significance_class_21->SetLineColor(kColor_21);
  h_significance_class_21->SetLineWidth(kLineWidth_21);
  h_significance_class_21->SetMarkerColor(kColor_21);
  h_significance_class_21->SetMarkerStyle(kMarkerStyle_21);
  h_significance_exp_21->SetLineColor(kColor_21);
  h_significance_exp_21->SetLineWidth(kLineWidth_21);
  h_significance_exp_21->SetMarkerColor(kColor_21);
  h_significance_exp_21->SetMarkerStyle(kMarkerStyle_21);
  h_SoverB_class_21->SetLineColor(kColor_21);
  h_SoverB_class_21->SetLineWidth(kLineWidth_21);
  h_SoverB_class_21->SetMarkerColor(kColor_21);
  h_SoverB_class_21->SetMarkerStyle(kMarkerStyle_21);
  h_SoverB_exp_21->SetLineColor(kColor_21);
  h_SoverB_exp_21->SetLineWidth(kLineWidth_21);
  h_SoverB_exp_21->SetMarkerColor(kColor_21);
  h_SoverB_exp_21->SetMarkerStyle(kMarkerStyle_21);

  h_significance_class_22->SetLineColor(kColor_22);
  h_significance_class_22->SetLineWidth(kLineWidth_22);
  h_significance_class_22->SetMarkerColor(kColor_22);
  h_significance_class_22->SetMarkerStyle(kMarkerStyle_22);
  h_significance_exp_22->SetLineColor(kColor_22);
  h_significance_exp_22->SetLineWidth(kLineWidth_22);
  h_significance_exp_22->SetMarkerColor(kColor_22);
  h_significance_exp_22->SetMarkerStyle(kMarkerStyle_22);
  h_SoverB_class_22->SetLineColor(kColor_22);
  h_SoverB_class_22->SetLineWidth(kLineWidth_22);
  h_SoverB_class_22->SetMarkerColor(kColor_22);
  h_SoverB_class_22->SetMarkerStyle(kMarkerStyle_22);
  h_SoverB_exp_22->SetLineColor(kColor_22);
  h_SoverB_exp_22->SetLineWidth(kLineWidth_22);
  h_SoverB_exp_22->SetMarkerColor(kColor_22);
  h_SoverB_exp_22->SetMarkerStyle(kMarkerStyle_22);

  h_significance_class_3->SetLineColor(kColor_3);
  h_significance_class_3->SetLineWidth(kLineWidth_3);
  h_significance_class_3->SetMarkerColor(kColor_3);
  h_significance_class_3->SetMarkerStyle(kMarkerStyle_3);
  h_significance_exp_3->SetLineColor(kColor_3);
  h_significance_exp_3->SetLineWidth(kLineWidth_3);
  h_significance_exp_3->SetMarkerColor(kColor_3);
  h_significance_exp_3->SetMarkerStyle(kMarkerStyle_3);
  h_SoverB_class_3->SetLineColor(kColor_3);
  h_SoverB_class_3->SetLineWidth(kLineWidth_3);
  h_SoverB_class_3->SetMarkerColor(kColor_3);
  h_SoverB_class_3->SetMarkerStyle(kMarkerStyle_3);
  h_SoverB_exp_3->SetLineColor(kColor_3);
  h_SoverB_exp_3->SetLineWidth(kLineWidth_3);
  h_SoverB_exp_3->SetMarkerColor(kColor_3);
  h_SoverB_exp_3->SetMarkerStyle(kMarkerStyle_3);

  h_significance_class_4->SetLineColor(kColor_4);
  h_significance_class_4->SetLineWidth(kLineWidth_4);
  h_significance_class_4->SetMarkerColor(kColor_4);
  h_significance_class_4->SetMarkerStyle(kMarkerStyle_4);
  h_significance_exp_4->SetLineColor(kColor_4);
  h_significance_exp_4->SetLineWidth(kLineWidth_4);
  h_significance_exp_4->SetMarkerColor(kColor_4);
  h_significance_exp_4->SetMarkerStyle(kMarkerStyle_4);
  h_SoverB_class_4->SetLineColor(kColor_4);
  h_SoverB_class_4->SetLineWidth(kLineWidth_4);
  h_SoverB_class_4->SetMarkerColor(kColor_4);
  h_SoverB_class_4->SetMarkerStyle(kMarkerStyle_4);
  h_SoverB_exp_4->SetLineColor(kColor_4);
  h_SoverB_exp_4->SetLineWidth(kLineWidth_4);
  h_SoverB_exp_4->SetMarkerColor(kColor_4);
  h_SoverB_exp_4->SetMarkerStyle(kMarkerStyle_4);

  h_eff_S_1->SetLineColor(kColor_1);
  h_eff_S_1->SetLineWidth(kLineWidth_1);
  h_eff_S_1->SetMarkerColor(kColor_1);
  h_eff_S_2->SetMarkerColor(kColor_2);
  h_eff_S_2->SetLineColor(kColor_2);
  h_eff_S_2->SetLineWidth(kLineWidth_2);
  h_eff_S_21->SetMarkerColor(kColor_21);
  h_eff_S_21->SetLineColor(kColor_21);
  h_eff_S_21->SetLineWidth(kLineWidth_21);
  h_eff_S_22->SetMarkerColor(kColor_22);
  h_eff_S_22->SetLineColor(kColor_22);
  h_eff_S_22->SetLineWidth(kLineWidth_22);
  h_eff_S_3->SetMarkerColor(kColor_3);
  h_eff_S_3->SetLineColor(kColor_3);
  h_eff_S_3->SetLineWidth(kLineWidth_3);
  h_eff_S_4->SetMarkerColor(kColor_4);
  h_eff_S_4->SetLineColor(kColor_4);
  h_eff_S_4->SetLineWidth(kLineWidth_4);

  h_eff_CombiWithConvLeg_1->SetLineColor(kColor_1);
  h_eff_CombiWithConvLeg_1->SetLineWidth(kLineWidth_1);
  h_eff_CombiWithConvLeg_1->SetMarkerColor(kColor_1);
  h_eff_CombiWithConvLeg_2->SetLineColor(kColor_2);
  h_eff_CombiWithConvLeg_2->SetLineWidth(kLineWidth_2);
  h_eff_CombiWithConvLeg_2->SetMarkerColor(kColor_2);
  h_eff_CombiWithConvLeg_21->SetLineColor(kColor_21);
  h_eff_CombiWithConvLeg_21->SetLineWidth(kLineWidth_21);
  h_eff_CombiWithConvLeg_21->SetMarkerColor(kColor_21);
  h_eff_CombiWithConvLeg_22->SetLineColor(kColor_22);
  h_eff_CombiWithConvLeg_22->SetLineWidth(kLineWidth_22);
  h_eff_CombiWithConvLeg_22->SetMarkerColor(kColor_22);
  h_eff_CombiWithConvLeg_3->SetLineColor(kColor_3);
  h_eff_CombiWithConvLeg_3->SetLineWidth(kLineWidth_3);
  h_eff_CombiWithConvLeg_3->SetMarkerColor(kColor_3);
  h_eff_CombiWithConvLeg_4->SetLineColor(kColor_4);
  h_eff_CombiWithConvLeg_4->SetLineWidth(kLineWidth_4);
  h_eff_CombiWithConvLeg_4->SetMarkerColor(kColor_4);

  h_eff_CombiWithoutConvLeg_1->SetLineColor(kColor_1);
  h_eff_CombiWithoutConvLeg_1->SetLineWidth(kLineWidth_1);
  h_eff_CombiWithoutConvLeg_1->SetMarkerColor(kColor_1);
  h_eff_CombiWithoutConvLeg_2->SetLineColor(kColor_2);
  h_eff_CombiWithoutConvLeg_2->SetLineWidth(kLineWidth_2);
  h_eff_CombiWithoutConvLeg_2->SetMarkerColor(kColor_2);
  h_eff_CombiWithoutConvLeg_21->SetLineColor(kColor_21);
  h_eff_CombiWithoutConvLeg_21->SetLineWidth(kLineWidth_21);
  h_eff_CombiWithoutConvLeg_21->SetMarkerColor(kColor_21);
  h_eff_CombiWithoutConvLeg_22->SetLineColor(kColor_22);
  h_eff_CombiWithoutConvLeg_22->SetLineWidth(kLineWidth_22);
  h_eff_CombiWithoutConvLeg_22->SetMarkerColor(kColor_22);
  h_eff_CombiWithoutConvLeg_3->SetLineColor(kColor_3);
  h_eff_CombiWithoutConvLeg_3->SetLineWidth(kLineWidth_3);
  h_eff_CombiWithoutConvLeg_3->SetMarkerColor(kColor_3);
  h_eff_CombiWithoutConvLeg_4->SetLineColor(kColor_4);
  h_eff_CombiWithoutConvLeg_4->SetLineWidth(kLineWidth_4);
  h_eff_CombiWithoutConvLeg_4->SetMarkerColor(kColor_4);

  h_eff_HF_1->SetLineColor(kColor_1);
  h_eff_HF_1->SetLineWidth(kLineWidth_1);
  h_eff_HF_1->SetMarkerColor(kColor_1);
  h_eff_HF_2->SetLineColor(kColor_2);
  h_eff_HF_2->SetLineWidth(kLineWidth_2);
  h_eff_HF_2->SetMarkerColor(kColor_2);
  h_eff_HF_21->SetLineColor(kColor_21);
  h_eff_HF_21->SetLineWidth(kLineWidth_21);
  h_eff_HF_21->SetMarkerColor(kColor_21);
  h_eff_HF_22->SetLineColor(kColor_22);
  h_eff_HF_22->SetLineWidth(kLineWidth_22);
  h_eff_HF_22->SetMarkerColor(kColor_22);
  h_eff_HF_3->SetLineColor(kColor_3);
  h_eff_HF_3->SetLineWidth(kLineWidth_3);
  h_eff_HF_3->SetMarkerColor(kColor_3);
  h_eff_HF_4->SetLineColor(kColor_4);
  h_eff_HF_4->SetLineWidth(kLineWidth_4);
  h_eff_HF_4->SetMarkerColor(kColor_4);

  h_eff_RPConv_1->SetLineColor(kColor_1);
  h_eff_RPConv_1->SetLineWidth(kLineWidth_1);
  h_eff_RPConv_1->SetMarkerColor(kColor_1);
  h_eff_RPConv_2->SetLineColor(kColor_2);
  h_eff_RPConv_2->SetLineWidth(kLineWidth_2);
  h_eff_RPConv_2->SetMarkerColor(kColor_2);
  h_eff_RPConv_21->SetLineColor(kColor_21);
  h_eff_RPConv_21->SetLineWidth(kLineWidth_21);
  h_eff_RPConv_21->SetMarkerColor(kColor_21);
  h_eff_RPConv_22->SetLineColor(kColor_22);
  h_eff_RPConv_22->SetLineWidth(kLineWidth_22);
  h_eff_RPConv_22->SetMarkerColor(kColor_22);
  h_eff_RPConv_3->SetLineColor(kColor_3);
  h_eff_RPConv_3->SetLineWidth(kLineWidth_3);
  h_eff_RPConv_3->SetMarkerColor(kColor_3);
  h_eff_RPConv_4->SetLineColor(kColor_4);
  h_eff_RPConv_4->SetLineWidth(kLineWidth_4);
  h_eff_RPConv_4->SetMarkerColor(kColor_4);
  
  h_eff_S_1->SetMarkerStyle(kMarkerStyle_1);
  h_eff_S_2->SetMarkerStyle(kMarkerStyle_2);
  h_eff_S_21->SetMarkerStyle(kMarkerStyle_21);
  h_eff_S_22->SetMarkerStyle(kMarkerStyle_22);
  h_eff_S_3->SetMarkerStyle(kMarkerStyle_3);
  h_eff_S_4->SetMarkerStyle(kMarkerStyle_4);

  h_eff_CombiWithConvLeg_1->SetMarkerStyle(kMarkerStyle_1);
  h_eff_CombiWithConvLeg_2->SetMarkerStyle(kMarkerStyle_2);
  h_eff_CombiWithConvLeg_21->SetMarkerStyle(kMarkerStyle_21);
  h_eff_CombiWithConvLeg_22->SetMarkerStyle(kMarkerStyle_22);
  h_eff_CombiWithConvLeg_3->SetMarkerStyle(kMarkerStyle_3);
  h_eff_CombiWithConvLeg_4->SetMarkerStyle(kMarkerStyle_4);

  h_eff_CombiWithoutConvLeg_1->SetMarkerStyle(kMarkerStyle_1);
  h_eff_CombiWithoutConvLeg_2->SetMarkerStyle(kMarkerStyle_2);
  h_eff_CombiWithoutConvLeg_21->SetMarkerStyle(kMarkerStyle_21);
  h_eff_CombiWithoutConvLeg_22->SetMarkerStyle(kMarkerStyle_22);
  h_eff_CombiWithoutConvLeg_3->SetMarkerStyle(kMarkerStyle_3);
  h_eff_CombiWithoutConvLeg_4->SetMarkerStyle(kMarkerStyle_4);

  h_eff_HF_1->SetMarkerStyle(kMarkerStyle_1);
  h_eff_HF_2->SetMarkerStyle(kMarkerStyle_2);
  h_eff_HF_21->SetMarkerStyle(kMarkerStyle_21);
  h_eff_HF_22->SetMarkerStyle(kMarkerStyle_22);
  h_eff_HF_3->SetMarkerStyle(kMarkerStyle_3);
  h_eff_HF_4->SetMarkerStyle(kMarkerStyle_4);

  h_eff_RPConv_1->SetMarkerStyle(kMarkerStyle_1);
  h_eff_RPConv_2->SetMarkerStyle(kMarkerStyle_2);
  h_eff_RPConv_21->SetMarkerStyle(kMarkerStyle_21);
  h_eff_RPConv_22->SetMarkerStyle(kMarkerStyle_22);
  h_eff_RPConv_3->SetMarkerStyle(kMarkerStyle_3);
  h_eff_RPConv_4->SetMarkerStyle(kMarkerStyle_4);

  
  const Float_t axisTitleSize = .05;

  h_significance_class_ideal->SetXTitle("#it{m}_{ee} (GeV/#it{c}^{2})");
  h_significance_class_ideal->SetYTitle("#it{S}_{class} / #sqrt{#it{S}_{class} + #it{B}_{class}}");
  h_significance_class_ideal->SetTitleSize(axisTitleSize,"xyz");

  h_significance_class_1_norm->SetYTitle("Sign. / sign._{no cuts}");
  h_significance_class_ideal->SetTitleSize(axisTitleSize,"xyz");  

  h_significance_exp_ideal->SetXTitle("#it{m}_{ee} (GeV/#it{c}^{2})");
  h_significance_exp_ideal->SetYTitle("#it{S}_{exp} / #sqrt{#it{S}_{exp} + #it{B}_{exp}}");
  h_significance_exp_ideal->SetTitleSize(axisTitleSize,"xyz");
  h_significance_exp_ideal->GetYaxis()->SetRangeUser(1e-2,1e3);

  h_significance_exp_1_norm->SetYTitle("Sign. / sign._{no cuts}");

  h_SoverB_class_1->SetXTitle("#it{m}_{ee} (GeV/#it{c}^{2})");
  h_SoverB_class_1->SetYTitle("#it{S}_{class} / #it{B}_{class}");
  h_SoverB_class_1->SetTitleSize(axisTitleSize,"xyz");

  h_SoverB_class_1_norm->SetYTitle("(#it{S}/#it{B}) / (#it{S}/#it{B})_{no cuts}");

  h_SoverB_exp_1->SetXTitle("#it{m}_{ee} (GeV/#it{c}^{2})");
  h_SoverB_exp_1->SetYTitle("#it{S}_{exp} / #it{B}_{exp}");
  h_SoverB_exp_1->SetTitleSize(axisTitleSize,"xyz");

  h_SoverB_exp_1_norm->SetYTitle("(#it{S}/#it{B}) / (#it{S}/#it{B})_{no cuts}");


  h_eff_CombiWithConvLeg_1->SetXTitle("#it{m}_{ee} (GeV/#it{c}^{2})");
  h_eff_CombiWithConvLeg_1->SetYTitle("Efficiency");
  h_eff_CombiWithConvLeg_1->SetTitleSize(axisTitleSize,"xyz");


  TLegend *leg1 = new TLegend(.5, .5, .88, .85);
  leg1->AddEntry(h_significance_class_1, leg_label1, "p");
  leg1->AddEntry(h_significance_class_4, leg_label4, "p");
  leg1->AddEntry(h_significance_class_3, leg_label3, "p");
  leg1->AddEntry(h_significance_class_2, leg_label2, "p");
  leg1->AddEntry(h_significance_class_21, leg_label21, "p");
  leg1->AddEntry(h_significance_class_22, leg_label22, "p");
  leg1->AddEntry(h_significance_class_noMVAcuts, leg_label_noMVAcuts, "p");
  leg1->AddEntry(h_significance_class_ideal, leg_label_ideal, "p");

  TLegend *leg2 = new TLegend(.54, .5, .88, .85);
  leg2->AddEntry(h_significance_exp_1, leg_label1, "p");
  leg2->AddEntry(h_significance_exp_4, leg_label4, "p");
  leg2->AddEntry(h_significance_exp_3, leg_label3, "p");
  leg2->AddEntry(h_significance_exp_2, leg_label2, "p");
  leg2->AddEntry(h_significance_exp_21, leg_label21, "p");
  leg2->AddEntry(h_significance_exp_22, leg_label22, "p");
  leg2->AddEntry(h_significance_exp_noMVAcuts, leg_label_noMVAcuts, "p");
  leg2->AddEntry(h_significance_exp_ideal, leg_label_ideal, "p");

  TLegend *leg3 = new TLegend(.17, .4, .43, .75);
  leg3->AddEntry(h_SoverB_class_1, leg_label1, "p");
  leg3->AddEntry(h_SoverB_class_4, leg_label4, "p");
  leg3->AddEntry(h_SoverB_class_3, leg_label3, "p");
  leg3->AddEntry(h_SoverB_class_2, leg_label2, "p");
  leg3->AddEntry(h_SoverB_class_21, leg_label21, "p");
  leg3->AddEntry(h_SoverB_class_22, leg_label22, "p");
  leg3->AddEntry(h_SoverB_class_noMVAcuts, leg_label_noMVAcuts, "p");
  leg3->SetFillStyle(0);

  TLegend *leg4 = new TLegend(.5, .1, .75, .45);
  leg4->AddEntry(h_SoverB_exp_1, leg_label1, "p");
  leg4->AddEntry(h_SoverB_exp_4, leg_label4, "p");
  leg4->AddEntry(h_SoverB_exp_3, leg_label3, "p");
  leg4->AddEntry(h_SoverB_exp_2, leg_label2, "p");
  leg4->AddEntry(h_SoverB_exp_21, leg_label21, "p");
  leg4->AddEntry(h_SoverB_exp_22, leg_label22, "p");
  leg4->AddEntry(h_SoverB_exp_noMVAcuts, leg_label_noMVAcuts, "p");
  leg4->AddEntry(h_SoverB_exp_ideal, leg_label_ideal, "p");
  leg4->SetFillStyle(0);

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
  leg_eff->AddEntry(h_eff_S_21, leg_label21, "p");
  leg_eff->AddEntry(h_eff_S_22, leg_label22, "p");

  TLatex *l_ALICE = new TLatex();
  l_ALICE->SetTextSize(0.055);
  l_ALICE->SetTextFont(42);
  l_ALICE->SetNDC();

  TLatex *l_info = new TLatex();
  l_info->SetTextSize(.05);
  l_info->SetTextFont(42);
  l_info->SetNDC();

  TString drawOptions = "hist p e1 x0";
  TString drawOptions_same = drawOptions + " same";

  TString drawOptions_eff = "hist p x0 e1";
  TString drawOptions_eff_same = drawOptions_eff + " same";

  TString drawOptions_ratio = "hist p e1 x0";
  TString drawOptions_ratio_same = drawOptions_ratio + " same";
  
  TCanvas *c_significance_class = new TCanvas("c_significance_class","",1024,768);
  
  TPad *p_significance_class = new TPad("p_significance_class","p_significance_class",0,.4,1,1);
  p_significance_class->SetBottomMargin(0);
  p_significance_class->SetLogy();
  p_significance_class->Draw();
  p_significance_class->cd();

  h_significance_class_ideal->GetYaxis()->SetTitleSize(.065);
  h_significance_class_ideal->GetYaxis()->SetTitleOffset(.9);
  
  h_significance_class_ideal->Draw(drawOptions);
  h_significance_class_1->Draw(drawOptions_same);
  h_significance_class_2->Draw(drawOptions_same);
  h_significance_class_21->Draw(drawOptions_same);
  h_significance_class_22->Draw(drawOptions_same);
  h_significance_class_3->Draw(drawOptions_same);
  h_significance_class_4->Draw(drawOptions_same);
  h_significance_class_noMVAcuts->Draw(drawOptions_same);
  l_ALICE->DrawLatex(.19,.12,"ALICE work in progress");
  l_info->DrawLatex(.19,.05,"HIJING, Pb-Pb #sqrt{#it{s}_{NN}} = 5.5 TeV, 0-10%");
  leg1->Draw();
  
  c_significance_class->cd();
  TPad *p_significance_class_norm = new TPad("p_significance_class_norm","p_significance_class_norm", 0,.05,1,.4);
  p_significance_class_norm->SetTopMargin(0);
  p_significance_class_norm->SetBottomMargin(.3);
  p_significance_class_norm->Draw();
  p_significance_class_norm->cd();

  h_significance_class_1_norm->SetMinimum(.8);
  h_significance_class_1_norm->SetMaximum(1.35);
  h_significance_class_1_norm->SetTitle("");
  
  h_significance_class_1_norm->SetMarkerStyle(kMarkerStyle_1);
  h_significance_class_2_norm->SetMarkerStyle(kMarkerStyle_2);
  h_significance_class_21_norm->SetMarkerStyle(kMarkerStyle_21);
  h_significance_class_22_norm->SetMarkerStyle(kMarkerStyle_22);
  h_significance_class_3_norm->SetMarkerStyle(kMarkerStyle_3);
  h_significance_class_4_norm->SetMarkerStyle(kMarkerStyle_4);

  h_significance_class_1_norm->SetMarkerColor(kColor_1);
  h_significance_class_2_norm->SetMarkerColor(kColor_2);
  h_significance_class_21_norm->SetMarkerColor(kColor_21);
  h_significance_class_22_norm->SetMarkerColor(kColor_22);
  h_significance_class_3_norm->SetMarkerColor(kColor_3);
  h_significance_class_4_norm->SetMarkerColor(kColor_4);

  h_significance_class_1_norm->SetLineColor(kColor_1);
  h_significance_class_2_norm->SetLineColor(kColor_2);
  h_significance_class_21_norm->SetLineColor(kColor_21);
  h_significance_class_22_norm->SetLineColor(kColor_22);
  h_significance_class_3_norm->SetLineColor(kColor_3);
  h_significance_class_4_norm->SetLineColor(kColor_4);
  
  h_significance_class_1_norm->GetYaxis()->SetLabelSize(.07);
  h_significance_class_1_norm->GetYaxis()->SetTitleOffset(.5);
  h_significance_class_1_norm->GetYaxis()->SetTitleSize(.1);
  h_significance_class_1_norm->GetXaxis()->SetTitleOffset(.9);
  h_significance_class_1_norm->GetXaxis()->SetLabelSize(.1);
  h_significance_class_1_norm->GetXaxis()->SetTitleSize(.14);
  h_significance_class_1_norm->GetXaxis()->SetTitle("#it{m}_{ee} (GeV/#it{c}^{2})");

  h_significance_class_1_norm->Draw(drawOptions_ratio);
  h_significance_class_2_norm->Draw(drawOptions_ratio_same);
  h_significance_class_21_norm->Draw(drawOptions_ratio_same);
  h_significance_class_22_norm->Draw(drawOptions_ratio_same);
  h_significance_class_3_norm->Draw(drawOptions_ratio_same);
  h_significance_class_4_norm->Draw(drawOptions_ratio_same);

  gPad->Update();
  gPad->Modified();

  c_significance_class->SaveAs("temp_output/c_significance_class.pdf");
  c_significance_class->SaveAs("temp_output/c_significance_class.root");
  c_significance_class->SaveAs("temp_output/c_significance_class.png");


  
  TCanvas *c_significance_exp = new TCanvas("c_significance_exp","",1024,768);

  TPad *p_significance_exp = new TPad("p_significance_exp","p_significance_exp",0,.4,1,1);
  p_significance_exp->SetBottomMargin(0);
  p_significance_exp->SetLogy();
  p_significance_exp->Draw();
  p_significance_exp->cd();

  h_significance_exp_ideal->GetYaxis()->SetTitleSize(.065);
  h_significance_exp_ideal->GetYaxis()->SetTitleOffset(.9);
  h_significance_exp_ideal->SetMinimum(1.2e-2);
  
  h_significance_exp_ideal->Draw(drawOptions);
  h_significance_exp_1->Draw(drawOptions_same);
  h_significance_exp_2->Draw(drawOptions_same);
  h_significance_exp_21->Draw(drawOptions_same);
  h_significance_exp_22->Draw(drawOptions_same);
  h_significance_exp_3->Draw(drawOptions_same);
  h_significance_exp_4->Draw(drawOptions_same);
  h_significance_exp_noMVAcuts->Draw(drawOptions_same);
  l_ALICE->DrawLatex(.19,.82,"ALICE work in progress");
  l_info->DrawLatex(.19,.76,"HIJING, Pb-Pb #sqrt{#it{s}_{NN}} = 5.5 TeV, 0-10%");
  leg2->Draw();

  c_significance_exp->cd();
  TPad *p_significance_exp_norm = new TPad("p_significance_exp_norm","p_significance_exp_norm",0,.05,1,.4);
  p_significance_exp_norm->SetTopMargin(0);
  p_significance_exp_norm->SetBottomMargin(.3);
  p_significance_exp_norm->Draw();
  p_significance_exp_norm->cd();

  h_significance_exp_1_norm->SetMinimum(.8);
  h_significance_exp_1_norm->SetMaximum(1.35);
  h_significance_exp_1_norm->SetTitle("");

  h_significance_exp_1_norm->SetMarkerStyle(kMarkerStyle_1);
  h_significance_exp_2_norm->SetMarkerStyle(kMarkerStyle_2);
  h_significance_exp_21_norm->SetMarkerStyle(kMarkerStyle_21);
  h_significance_exp_22_norm->SetMarkerStyle(kMarkerStyle_22);
  h_significance_exp_3_norm->SetMarkerStyle(kMarkerStyle_3);
  h_significance_exp_4_norm->SetMarkerStyle(kMarkerStyle_4);

  h_significance_exp_1_norm->SetMarkerColor(kColor_1);
  h_significance_exp_2_norm->SetMarkerColor(kColor_2);
  h_significance_exp_21_norm->SetMarkerColor(kColor_21);
  h_significance_exp_22_norm->SetMarkerColor(kColor_22);
  h_significance_exp_3_norm->SetMarkerColor(kColor_3);
  h_significance_exp_4_norm->SetMarkerColor(kColor_4);

  h_significance_exp_1_norm->SetLineColor(kColor_1);
  h_significance_exp_2_norm->SetLineColor(kColor_2);
  h_significance_exp_21_norm->SetLineColor(kColor_21);
  h_significance_exp_22_norm->SetLineColor(kColor_22);
  h_significance_exp_3_norm->SetLineColor(kColor_3);
  h_significance_exp_4_norm->SetLineColor(kColor_4);

  h_significance_exp_1_norm->GetYaxis()->SetLabelSize(.07);
  h_significance_exp_1_norm->GetYaxis()->SetTitleOffset(.5);
  h_significance_exp_1_norm->GetYaxis()->SetTitleSize(.1);
  h_significance_exp_1_norm->GetXaxis()->SetTitleOffset(.9);
  h_significance_exp_1_norm->GetXaxis()->SetLabelSize(.1);
  h_significance_exp_1_norm->GetXaxis()->SetTitleSize(.14);
  h_significance_exp_1_norm->GetXaxis()->SetTitle("#it{m}_{ee} (GeV/#it{c}^{2})");

  h_significance_exp_1_norm->Draw(drawOptions_ratio);
  h_significance_exp_2_norm->Draw(drawOptions_ratio_same);
  h_significance_exp_21_norm->Draw(drawOptions_ratio_same);
  h_significance_exp_22_norm->Draw(drawOptions_ratio_same);
  h_significance_exp_3_norm->Draw(drawOptions_ratio_same);
  h_significance_exp_4_norm->Draw(drawOptions_ratio_same);

  gPad->Update();
  gPad->Modified();
  
  c_significance_exp->SaveAs("temp_output/c_significance_exp.pdf");
  c_significance_exp->SaveAs("temp_output/c_significance_exp.root");
  c_significance_exp->SaveAs("temp_output/c_significance_exp.png");


  TCanvas *c_SoverB_class = new TCanvas("c_SoverB_class","",1024,768);

  TPad *p_SoverB_class = new TPad("p_SoverB_class","p_SoverB_class",0,.4,1,1);
  p_SoverB_class->SetBottomMargin(0);
  p_SoverB_class->SetLogy();
  p_SoverB_class->Draw();
  p_SoverB_class->cd();

  h_SoverB_class_1->GetYaxis()->SetTitleSize(.065);
  h_SoverB_class_1->GetYaxis()->SetTitleOffset(.9);
  
  h_SoverB_class_1->Draw(drawOptions);
  h_SoverB_class_2->Draw(drawOptions_same);
  h_SoverB_class_21->Draw(drawOptions_same);
  h_SoverB_class_22->Draw(drawOptions_same);
  h_SoverB_class_3->Draw(drawOptions_same);
  h_SoverB_class_4->Draw(drawOptions_same);
  h_SoverB_class_noMVAcuts->Draw(drawOptions_same);
  l_ALICE->DrawLatex(.19,.82,"ALICE work in progress");
  l_info->DrawLatex(.19,.76,"HIJING, Pb-Pb #sqrt{#it{s}_{NN}} = 5.5 TeV, 0-10%");
  leg3->Draw();

  c_SoverB_class->cd();
  TPad *p_SoverB_class_norm = new TPad("p_SoverB_class_norm","p_SoverB_class_norm",0,.05,1,.4);
  p_SoverB_class_norm->SetTopMargin(0);
  p_SoverB_class_norm->SetBottomMargin(.3);
  p_SoverB_class_norm->Draw();
  p_SoverB_class_norm->cd();

  // h_SoverB_class_1_norm->SetMinimum(0.);
  // h_SoverB_class_1_norm->SetMaximum(2.05);
  h_SoverB_class_1_norm->SetTitle("");

  h_SoverB_class_1_norm->SetMarkerStyle(kMarkerStyle_1);
  h_SoverB_class_2_norm->SetMarkerStyle(kMarkerStyle_2);
  h_SoverB_class_21_norm->SetMarkerStyle(kMarkerStyle_21);
  h_SoverB_class_22_norm->SetMarkerStyle(kMarkerStyle_22);
  h_SoverB_class_3_norm->SetMarkerStyle(kMarkerStyle_3);
  h_SoverB_class_4_norm->SetMarkerStyle(kMarkerStyle_4);

  h_SoverB_class_1_norm->SetMarkerColor(kColor_1);
  h_SoverB_class_2_norm->SetMarkerColor(kColor_2);
  h_SoverB_class_21_norm->SetMarkerColor(kColor_21);
  h_SoverB_class_22_norm->SetMarkerColor(kColor_22);
  h_SoverB_class_3_norm->SetMarkerColor(kColor_3);
  h_SoverB_class_4_norm->SetMarkerColor(kColor_4);

  h_SoverB_class_1_norm->SetLineColor(kColor_1);
  h_SoverB_class_2_norm->SetLineColor(kColor_2);
  h_SoverB_class_21_norm->SetLineColor(kColor_21);
  h_SoverB_class_22_norm->SetLineColor(kColor_22);
  h_SoverB_class_3_norm->SetLineColor(kColor_3);
  h_SoverB_class_4_norm->SetLineColor(kColor_4);

  h_SoverB_class_1_norm->GetYaxis()->SetLabelSize(.07);
  h_SoverB_class_1_norm->GetYaxis()->SetTitleOffset(.5);
  h_SoverB_class_1_norm->GetYaxis()->SetTitleSize(.1);
  h_SoverB_class_1_norm->GetXaxis()->SetTitleOffset(.9);
  h_SoverB_class_1_norm->GetXaxis()->SetLabelSize(.1);
  h_SoverB_class_1_norm->GetXaxis()->SetTitleSize(.14);
  h_SoverB_class_1_norm->GetXaxis()->SetTitle("#it{m}_{ee} (GeV/#it{c}^{2})");

  h_SoverB_class_1_norm->Draw(drawOptions_ratio);
  h_SoverB_class_2_norm->Draw(drawOptions_ratio_same);
  h_SoverB_class_21_norm->Draw(drawOptions_ratio_same);
  h_SoverB_class_22_norm->Draw(drawOptions_ratio_same);
  h_SoverB_class_3_norm->Draw(drawOptions_ratio_same);
  h_SoverB_class_4_norm->Draw(drawOptions_ratio_same);

  gPad->Update();
  gPad->Modified();
  
  c_SoverB_class->SaveAs("temp_output/c_SoverB_class.pdf");
  c_SoverB_class->SaveAs("temp_output/c_SoverB_class.root");
  c_SoverB_class->SaveAs("temp_output/c_SoverB_class.png");


  TCanvas *c_SoverB_exp = new TCanvas("c_SoverB_exp","",1024,768);

  TPad *p_SoverB_exp = new TPad("p_SoverB_exp","p_SoverB_exp",0,.4,1,1);
  p_SoverB_exp->SetBottomMargin(0);
  p_SoverB_exp->SetLogy();
  p_SoverB_exp->Draw();
  p_SoverB_exp->cd();

  h_SoverB_exp_1->GetYaxis()->SetTitleSize(.065);
  h_SoverB_exp_1->GetYaxis()->SetTitleOffset(.9);
  h_SoverB_exp_1->SetMinimum(1.2e-5);
  
  h_SoverB_exp_1->Draw(drawOptions);
  h_SoverB_exp_2->Draw(drawOptions_same);
  h_SoverB_exp_21->Draw(drawOptions_same);
  h_SoverB_exp_22->Draw(drawOptions_same);
  h_SoverB_exp_3->Draw(drawOptions_same);
  h_SoverB_exp_4->Draw(drawOptions_same);
  h_SoverB_exp_noMVAcuts->Draw(drawOptions_same);
  h_SoverB_exp_ideal->Draw(drawOptions_same);
  l_ALICE->DrawLatex(.19,.82,"ALICE work in progress");
  l_info->DrawLatex(.19,.76,"HIJING, Pb-Pb #sqrt{#it{s}_{NN}} = 5.5 TeV, 0-10%");
  leg4->Draw();

  c_SoverB_exp->cd();
  TPad *p_SoverB_exp_norm = new TPad("p_SoverB_exp_norm","p_SoverB_exp_norm",0,.05,1,.4);
  p_SoverB_exp_norm->SetTopMargin(0);
  p_SoverB_exp_norm->SetBottomMargin(.3);
  p_SoverB_exp_norm->Draw();
  p_SoverB_exp_norm->cd();

  h_SoverB_exp_1_norm->SetMinimum(.8);
  h_SoverB_exp_1_norm->SetMaximum(1.75);
  h_SoverB_exp_1_norm->SetTitle("");

  h_SoverB_exp_1_norm->SetMarkerStyle(kMarkerStyle_1);
  h_SoverB_exp_2_norm->SetMarkerStyle(kMarkerStyle_2);
  h_SoverB_exp_21_norm->SetMarkerStyle(kMarkerStyle_21);
  h_SoverB_exp_22_norm->SetMarkerStyle(kMarkerStyle_22);
  h_SoverB_exp_3_norm->SetMarkerStyle(kMarkerStyle_3);
  h_SoverB_exp_4_norm->SetMarkerStyle(kMarkerStyle_4);

  h_SoverB_exp_1_norm->SetMarkerColor(kColor_1);
  h_SoverB_exp_2_norm->SetMarkerColor(kColor_2);
  h_SoverB_exp_21_norm->SetMarkerColor(kColor_21);
  h_SoverB_exp_22_norm->SetMarkerColor(kColor_22);
  h_SoverB_exp_3_norm->SetMarkerColor(kColor_3);
  h_SoverB_exp_4_norm->SetMarkerColor(kColor_4);

  h_SoverB_exp_1_norm->SetLineColor(kColor_1);
  h_SoverB_exp_2_norm->SetLineColor(kColor_2);
  h_SoverB_exp_21_norm->SetLineColor(kColor_21);
  h_SoverB_exp_22_norm->SetLineColor(kColor_22);
  h_SoverB_exp_3_norm->SetLineColor(kColor_3);
  h_SoverB_exp_4_norm->SetLineColor(kColor_4);

  h_SoverB_exp_1_norm->GetYaxis()->SetLabelSize(.07);
  h_SoverB_exp_1_norm->GetYaxis()->SetTitleOffset(.5);
  h_SoverB_exp_1_norm->GetYaxis()->SetTitleSize(.1);
  h_SoverB_exp_1_norm->GetXaxis()->SetTitleOffset(.9);
  h_SoverB_exp_1_norm->GetXaxis()->SetLabelSize(.1);
  h_SoverB_exp_1_norm->GetXaxis()->SetTitleSize(.14);
  h_SoverB_exp_1_norm->GetXaxis()->SetTitle("#it{m}_{ee} (GeV/#it{c}^{2})");

  h_SoverB_exp_1_norm->Draw(drawOptions_ratio);
  h_SoverB_exp_2_norm->Draw(drawOptions_ratio_same);
  h_SoverB_exp_21_norm->Draw(drawOptions_ratio_same);
  h_SoverB_exp_22_norm->Draw(drawOptions_ratio_same);
  h_SoverB_exp_3_norm->Draw(drawOptions_ratio_same);
  h_SoverB_exp_4_norm->Draw(drawOptions_ratio_same);

  gPad->Update();
  gPad->Modified();
  
  c_SoverB_exp->SaveAs("temp_output/c_SoverB_exp.pdf");
  c_SoverB_exp->SaveAs("temp_output/c_SoverB_exp.root");
  c_SoverB_exp->SaveAs("temp_output/c_SoverB_exp.png");
  



  l_ALICE->SetTextSize(.035);
  l_info->SetTextSize(.03);

  
  h_eff_S_1->SetXTitle("#it{m}_{ee} (GeV/#it{c}^{2})");
  h_eff_S_1->SetYTitle("Efficiency (S)");
  h_eff_S_1->GetXaxis()->SetTitleOffset(1.2);
  h_eff_S_1->GetYaxis()->SetTitleOffset(1.4);
  h_eff_S_1->GetYaxis()->SetRangeUser(0, 1.2);
  h_eff_S_1->SetTitleSize(axisTitleSize,"xyz");

  h_eff_CombiWithConvLeg_1->SetXTitle("#it{m}_{ee} (GeV/#it{c}^{2})");
  h_eff_CombiWithConvLeg_1->SetYTitle("Efficiency (Comb. w. conv. leg)");
  h_eff_CombiWithConvLeg_1->GetXaxis()->SetTitleOffset(1.2);
  h_eff_CombiWithConvLeg_1->GetYaxis()->SetTitleOffset(1.4);
  h_eff_CombiWithConvLeg_1->GetYaxis()->SetRangeUser(0, 1.2);
  h_eff_CombiWithConvLeg_1->SetTitleSize(axisTitleSize,"xyz");

  h_eff_CombiWithoutConvLeg_1->SetXTitle("#it{m}_{ee} (GeV/#it{c}^{2})");
  h_eff_CombiWithoutConvLeg_1->SetYTitle("Efficiency (Comb. w/o conv. leg)");
  h_eff_CombiWithoutConvLeg_1->GetXaxis()->SetTitleOffset(1.2);
  h_eff_CombiWithoutConvLeg_1->GetYaxis()->SetTitleOffset(1.4);
  h_eff_CombiWithoutConvLeg_1->GetYaxis()->SetRangeUser(0, 1.2);
  h_eff_CombiWithoutConvLeg_1->SetTitleSize(axisTitleSize,"xyz");

  h_eff_HF_1->SetXTitle("#it{m}_{ee} (GeV/#it{c}^{2})");
  h_eff_HF_1->SetYTitle("Efficiency (corr. HF)");
  h_eff_HF_1->GetXaxis()->SetTitleOffset(1.2);
  h_eff_HF_1->GetYaxis()->SetTitleOffset(1.4);
  h_eff_HF_1->GetYaxis()->SetRangeUser(0, 1.2);
  h_eff_HF_1->SetTitleSize(axisTitleSize,"xyz");

  h_eff_RPConv_1->SetXTitle("#it{m}_{ee} (GeV/#it{c}^{2})");
  h_eff_RPConv_1->SetYTitle("Efficiency (RP conv.)");
  h_eff_RPConv_1->GetXaxis()->SetTitleOffset(1.2);
  h_eff_RPConv_1->GetYaxis()->SetTitleOffset(1.4);
  h_eff_RPConv_1->GetYaxis()->SetRangeUser(0, 1.2);
  h_eff_RPConv_1->SetTitleSize(axisTitleSize,"xyz");


  
  
  TCanvas *c_effs_S = new TCanvas("c_effs_S","",1024,768);
  // c_effs_S->SetGridy();
  h_eff_S_1->Draw(drawOptions_eff);
  h_eff_S_2->Draw(drawOptions_eff_same);
  h_eff_S_21->Draw(drawOptions_eff_same);
  h_eff_S_22->Draw(drawOptions_eff_same);
  h_eff_S_3->Draw(drawOptions_eff_same);
  h_eff_S_4->Draw(drawOptions_eff_same);
  l_ALICE->DrawLatex(.19,.24,"ALICE work in progress");
  l_info->DrawLatex(.19,.2,"HIJING, Pb-Pb #sqrt{#it{s}_{NN}} = 5.5 TeV, 0-10%");
  leg_eff->Draw();
  c_effs_S->SaveAs("temp_output/c_effs_S.pdf");
  c_effs_S->SaveAs("temp_output/c_effs_S.root");
  c_effs_S->SaveAs("temp_output/c_effs_S.png");

  TCanvas *c_effs_CombiWithConvLeg = new TCanvas("c_effs_CombiWithConvLeg","",1024,768);
  // c_effs_CombiWithConvLeg->SetGridy();
  h_eff_CombiWithConvLeg_1->Draw(drawOptions_eff);
  h_eff_CombiWithConvLeg_2->Draw(drawOptions_eff_same);
  h_eff_CombiWithConvLeg_21->Draw(drawOptions_eff_same);
  h_eff_CombiWithConvLeg_22->Draw(drawOptions_eff_same);
  h_eff_CombiWithConvLeg_3->Draw(drawOptions_eff_same);
  h_eff_CombiWithConvLeg_4->Draw(drawOptions_eff_same);
  l_ALICE->DrawLatex(.19,.24,"ALICE work in progress");
  l_info->DrawLatex(.19,.2,"HIJING, Pb-Pb #sqrt{#it{s}_{NN}} = 5.5 TeV, 0-10%");
  leg_eff->Draw();
  c_effs_CombiWithConvLeg->SaveAs("temp_output/c_effs_CombiWithConvLeg.pdf");
  c_effs_CombiWithConvLeg->SaveAs("temp_output/c_effs_CombiWithConvLeg.root");
  c_effs_CombiWithConvLeg->SaveAs("temp_output/c_effs_CombiWithConvLeg.png");

  TCanvas *c_effs_CombiWithoutConvLeg = new TCanvas("c_effs_CombiWithoutConvLeg","",1024,768);
  // c_effs_CombiWithoutConvLeg->SetGridy();
  h_eff_CombiWithoutConvLeg_1->Draw(drawOptions_eff);
  h_eff_CombiWithoutConvLeg_2->Draw(drawOptions_eff_same);
  h_eff_CombiWithoutConvLeg_21->Draw(drawOptions_eff_same);
  h_eff_CombiWithoutConvLeg_22->Draw(drawOptions_eff_same);
  h_eff_CombiWithoutConvLeg_3->Draw(drawOptions_eff_same);
  h_eff_CombiWithoutConvLeg_4->Draw(drawOptions_eff_same);
  l_ALICE->DrawLatex(.19,.24,"ALICE work in progress");
  l_info->DrawLatex(.19,.2,"HIJING, Pb-Pb #sqrt{#it{s}_{NN}} = 5.5 TeV, 0-10%");
  leg_eff->Draw();
  c_effs_CombiWithoutConvLeg->SaveAs("temp_output/c_effs_CombiWithoutConvLeg.pdf");
  c_effs_CombiWithoutConvLeg->SaveAs("temp_output/c_effs_CombiWithoutConvLeg.root");
  c_effs_CombiWithoutConvLeg->SaveAs("temp_output/c_effs_CombiWithoutConvLeg.png");

  TCanvas *c_effs_HF = new TCanvas("c_effs_HF","",1024,768);
  // c_effs_HF->SetGridy();
  h_eff_HF_1->Draw(drawOptions_eff);
  h_eff_HF_2->Draw(drawOptions_eff_same);
  h_eff_HF_21->Draw(drawOptions_eff_same);
  h_eff_HF_22->Draw(drawOptions_eff_same);
  h_eff_HF_3->Draw(drawOptions_eff_same);
  h_eff_HF_4->Draw(drawOptions_eff_same);
  l_ALICE->DrawLatex(.19,.24,"ALICE work in progress");
  l_info->DrawLatex(.19,.2,"HIJING, Pb-Pb #sqrt{#it{s}_{NN}} = 5.5 TeV, 0-10%");
  leg_eff->Draw();
  c_effs_HF->SaveAs("temp_output/c_effs_HF.pdf");
  c_effs_HF->SaveAs("temp_output/c_effs_HF.root");
  c_effs_HF->SaveAs("temp_output/c_effs_HF.png");

  TCanvas *c_effs_RPConv = new TCanvas("c_effs_RPConv","",1024,768);
  // c_effs_RPConv->SetGridy();
  h_eff_RPConv_1->Draw(drawOptions_eff);
  h_eff_RPConv_2->Draw(drawOptions_eff_same);
  h_eff_RPConv_21->Draw(drawOptions_eff_same);
  h_eff_RPConv_22->Draw(drawOptions_eff_same);
  h_eff_RPConv_3->Draw(drawOptions_eff_same);
  h_eff_RPConv_4->Draw(drawOptions_eff_same);
  l_ALICE->DrawLatex(.19,.24,"ALICE work in progress");
  l_info->DrawLatex(.19,.2,"HIJING, Pb-Pb #sqrt{#it{s}_{NN}} = 5.5 TeV, 0-10%");
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

  h_eff_S_21->SetLineColor(kColor_4);
  h_eff_S_21->SetMarkerColor(kColor_4);
  h_eff_S_21->SetMarkerStyle(kMarker_21);
  h_eff_CombiWithConvLeg_21->SetLineColor(kColor_21);
  h_eff_CombiWithConvLeg_21->SetMarkerColor(kColor_21);
  h_eff_CombiWithConvLeg_21->SetMarkerStyle(kMarker_21);
  h_eff_CombiWithoutConvLeg_21->SetLineColor(kColor_3);
  h_eff_CombiWithoutConvLeg_21->SetMarkerColor(kColor_3);
  h_eff_CombiWithoutConvLeg_21->SetMarkerStyle(kMarker_21);
  h_eff_HF_21->SetLineColor(kOrange);
  h_eff_HF_21->SetMarkerColor(kOrange);
  h_eff_HF_21->SetMarkerStyle(kMarker_21);
  h_eff_RPConv_21->SetLineColor(kMagenta);
  h_eff_RPConv_21->SetMarkerColor(kMagenta);
  h_eff_RPConv_21->SetMarkerStyle(kMarker_21);

h_eff_S_22->SetLineColor(kColor_4);
  h_eff_S_22->SetMarkerColor(kColor_4);
  h_eff_S_22->SetMarkerStyle(kMarker_22);
  h_eff_CombiWithConvLeg_22->SetLineColor(kColor_22);
  h_eff_CombiWithConvLeg_22->SetMarkerColor(kColor_22);
  h_eff_CombiWithConvLeg_22->SetMarkerStyle(kMarker_22);
  h_eff_CombiWithoutConvLeg_22->SetLineColor(kColor_3);
  h_eff_CombiWithoutConvLeg_22->SetMarkerColor(kColor_3);
  h_eff_CombiWithoutConvLeg_22->SetMarkerStyle(kMarker_22);
  h_eff_HF_22->SetLineColor(kOrange);
  h_eff_HF_22->SetMarkerColor(kOrange);
  h_eff_HF_22->SetMarkerStyle(kMarker_22);
  h_eff_RPConv_22->SetLineColor(kMagenta);
  h_eff_RPConv_22->SetMarkerColor(kMagenta);
  h_eff_RPConv_22->SetMarkerStyle(kMarker_22);
  
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
  // c_effs_all->SetGridy();
  h_eff_S_1->Draw(drawOptions_eff);
  h_eff_S_2->Draw(drawOptions_eff_same);
  h_eff_S_21->Draw(drawOptions_eff_same);
  h_eff_S_22->Draw(drawOptions_eff_same);
  h_eff_S_3->Draw(drawOptions_eff_same);
  h_eff_S_4->Draw(drawOptions_eff_same);
  h_eff_CombiWithConvLeg_1->Draw(drawOptions_eff_same);
  h_eff_CombiWithConvLeg_2->Draw(drawOptions_eff_same);
  h_eff_CombiWithConvLeg_21->Draw(drawOptions_eff_same);
  h_eff_CombiWithConvLeg_22->Draw(drawOptions_eff_same);
  h_eff_CombiWithConvLeg_3->Draw(drawOptions_eff_same);
  h_eff_CombiWithConvLeg_4->Draw(drawOptions_eff_same);
  h_eff_CombiWithoutConvLeg_1->Draw(drawOptions_eff_same);
  h_eff_CombiWithoutConvLeg_2->Draw(drawOptions_eff_same);
  h_eff_CombiWithoutConvLeg_21->Draw(drawOptions_eff_same);
  h_eff_CombiWithoutConvLeg_22->Draw(drawOptions_eff_same);
  h_eff_CombiWithoutConvLeg_3->Draw(drawOptions_eff_same);
  h_eff_CombiWithoutConvLeg_4->Draw(drawOptions_eff_same);
  h_eff_HF_1->Draw(drawOptions_eff_same);
  h_eff_HF_2->Draw(drawOptions_eff_same);
  h_eff_HF_21->Draw(drawOptions_eff_same);
  h_eff_HF_22->Draw(drawOptions_eff_same);
  h_eff_HF_3->Draw(drawOptions_eff_same);
  h_eff_HF_4->Draw(drawOptions_eff_same);
  h_eff_RPConv_1->Draw(drawOptions_eff_same);
  h_eff_RPConv_2->Draw(drawOptions_eff_same);
  h_eff_RPConv_21->Draw(drawOptions_eff_same);
  h_eff_RPConv_22->Draw(drawOptions_eff_same);
  h_eff_RPConv_3->Draw(drawOptions_eff_same);
  h_eff_RPConv_4->Draw(drawOptions_eff_same);
  l_ALICE->DrawLatex(.19,.24,"ALICE work in progress");
  l_info->DrawLatex(.19,.2,"HIJING, Pb-Pb #sqrt{#it{s}_{NN}} = 5.5 TeV, 0-10%");
  leg5->Draw();
  c_effs_all->SaveAs("temp_output/c_effs_all.pdf");
  c_effs_all->SaveAs("temp_output/c_effs_all.root");
  c_effs_all->SaveAs("temp_output/c_effs_all.png");


  gApplication->Terminate();
}
