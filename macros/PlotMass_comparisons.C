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


void PlotMass_comparisons() {

  // optimized MVA cut
  TFile *infile1 = new TFile("/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/analysis_singleConvTrackRejMVAcuts/plots/singleConvTrackRejMVAcuts/mass_histos_prefilterCuts.root", "READ");
  TH1D *h_SB_1                  = (TH1D*)infile1->Get("h_SB_prefilterCut");
  TH1D *h_S_1                   = (TH1D*)infile1->Get("h_S_prefilterCut");
  TH1D *h_CombiWithConvLeg_1    = (TH1D*)infile1->Get("h_CombiWithConvLeg_prefilterCut");
  TH1D *h_CombiWithoutConvLeg_1 = (TH1D*)infile1->Get("h_CombiWithoutConvLeg_prefilterCut");
  TH1D *h_HF_1                  = (TH1D*)infile1->Get("h_HF_prefilterCut");
  TH1D *h_RPConv_1              = (TH1D*)infile1->Get("h_RPConv_prefilterCut");

  h_SB_1->SetName("h_SB_1");
  h_S_1->SetName("h_S_1");
  h_CombiWithConvLeg_1->SetName("h_CombiWithConvLeg_1");
  h_CombiWithoutConvLeg_1->SetName("h_CombiWithoutConvLeg_1");
  h_HF_1->SetName("h_HF_1");
  h_RPConv_1->SetName("h_RPConv_1");
  
  // MVA cut +10%
  TFile *infile2 = new TFile("/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/analysis_singleConvTrackRejMVAcuts/plots/singleConvTrackRejMVAcuts-cutVariations/MVAcutPlus10Percent/mass_histos_prefilterCuts.root", "READ");
  TH1D *h_SB_2                  = (TH1D*)infile2->Get("h_SB_prefilterCut");
  TH1D *h_S_2                   = (TH1D*)infile2->Get("h_S_prefilterCut");
  TH1D *h_CombiWithConvLeg_2    = (TH1D*)infile2->Get("h_CombiWithConvLeg_prefilterCut");
  TH1D *h_CombiWithoutConvLeg_2 = (TH1D*)infile2->Get("h_CombiWithoutConvLeg_prefilterCut");
  TH1D *h_HF_2                  = (TH1D*)infile2->Get("h_HF_prefilterCut");
  TH1D *h_RPConv_2              = (TH1D*)infile2->Get("h_RPConv_prefilterCut");

  h_SB_2->SetName("h_SB_2");
  h_S_2->SetName("h_S_2");
  h_CombiWithConvLeg_2->SetName("h_CombiWithConvLeg_2");
  h_CombiWithoutConvLeg_2->SetName("h_CombiWithoutConvLeg_2");
  h_HF_2->SetName("h_HF_2");
  h_RPConv_2->SetName("h_RPConv_2");
  

  // MVA cut -10%
  TFile *infile3 = new TFile("/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/analysis_singleConvTrackRejMVAcuts/plots/singleConvTrackRejMVAcuts-cutVariations/MVAcutMinus10Percent/mass_histos_prefilterCuts.root", "READ");
  TH1D *h_SB_3                  = (TH1D*)infile3->Get("h_SB_prefilterCut");
  TH1D *h_S_3                   = (TH1D*)infile3->Get("h_S_prefilterCut");
  TH1D *h_CombiWithConvLeg_3    = (TH1D*)infile3->Get("h_CombiWithConvLeg_prefilterCut");
  TH1D *h_CombiWithoutConvLeg_3 = (TH1D*)infile3->Get("h_CombiWithoutConvLeg_prefilterCut");
  TH1D *h_HF_3                  = (TH1D*)infile3->Get("h_HF_prefilterCut");
  TH1D *h_RPConv_3              = (TH1D*)infile3->Get("h_RPConv_prefilterCut");

  h_SB_3->SetName("h_SB_3");
  h_S_3->SetName("h_S_3");
  h_CombiWithConvLeg_3->SetName("h_CombiWithConvLeg_3");
  h_CombiWithoutConvLeg_3->SetName("h_CombiWithoutConvLeg_3");
  h_HF_3->SetName("h_HF_3");
  h_RPConv_3->SetName("h_RPConv_3");
  
  /*
  // MVA cut +5%
  TFile *infile4 = new TFile("/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/analysis_singleConvTrackRejMVAcuts/plots/RPConvRejMVAcuts-varied_prefilter/RPConvRejMVAcuts_plus5percent_prefilter/mass_histos_prefilterCuts.root", "READ");
  TH1D *h_SB_4                  = (TH1D*)infile4->Get("h_SB_prefilterCut");
  TH1D *h_S_4                   = (TH1D*)infile4->Get("h_S_prefilterCut");
  TH1D *h_CombiWithConvLeg_4    = (TH1D*)infile4->Get("h_CombiWithConvLeg_prefilterCut");
  TH1D *h_CombiWithoutConvLeg_4 = (TH1D*)infile4->Get("h_CombiWithoughConvLeg_prefilterCut");
  TH1D *h_HF_4                  = (TH1D*)infile4->Get("h_HF_prefilterCut");
  TH1D *h_RPConv_4              = (TH1D*)infile4->Get("h_RPConv_prefilterCut");
  */
  
  // h_SB_4->SetName("h_SB_4");
  // h_S_4->SetName("h_S_4");
  // h_CombiWithConvLeg_4->SetName("h_CombiWithConvLeg_4");
  // h_CombiWithoutConvLeg_4->SetName("h_CombiWithoutConvLeg_4");
  // h_HF_4->SetName("h_HF_4");
  // h_RPConv_4->SetName("h_RPConv_4");
  
  /*
  // MVA cut -5%
  TFile *infile5 = new TFile("/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/analysis_singleConvTrackRejMVAcuts/plots/RPConvRejMVAcuts-varied_prefilter/RPConvRejMVAcuts_minus5percent_prefilter/mass_histos_prefilterCuts.root", "READ");
  TH1D *h_SB_5                  = (TH1D*)infile5->Get("h_SB_prefilterCut");
  TH1D *h_S_5                   = (TH1D*)infile5->Get("h_S_prefilterCut");
  TH1D *h_CombiWithConvLeg_5    = (TH1D*)infile5->Get("h_CombiWithConvLeg_prefilterCut");
  TH1D *h_CombiWithoutConvLeg_5 = (TH1D*)infile5->Get("h_CombiWithoughConvLeg_prefilterCut");
  TH1D *h_HF_5                  = (TH1D*)infile5->Get("h_HF_prefilterCut");
  TH1D *h_RPConv_5              = (TH1D*)infile5->Get("h_RPConv_prefilterCut");
  */
  
  // h_SB_5->SetName("h_SB_5");
  // h_S_5->SetName("h_S_5");
  // h_CombiWithConvLeg_5->SetName("h_CombiWithConvLeg_5");
  // h_CombiWithoutConvLeg_5->SetName("h_CombiWithoutConvLeg_5");
  // h_HF_5->SetName("h_HF_5");
  // h_RPConv_5->SetName("h_RPConv_5");


  /*
    h_SB_1->Sumw2();
    h_S_1->Sumw2();
    h_CombiWithConvLeg_1->Sumw2();
    h_CombiWithoutConvLeg_1->Sumw2();
    h_HF_1->Sumw2();
    h_RPConv_1->Sumw2();

    h_SB_2->Sumw2();
    h_S_2->Sumw2();
    h_CombiWithConvLeg_2->Sumw2();
    h_CombiWithoutConvLeg_2->Sumw2();
    h_HF_2->Sumw2();
    h_RPConv_2->Sumw2();

    h_SB_3->Sumw2();
    h_S_3->Sumw2();
    h_CombiWithConvLeg_3->Sumw2();
    h_CombiWithoutConvLeg_3->Sumw2();
    h_HF_3->Sumw2();
    h_RPConv_3->Sumw2();

    h_SB_4->Sumw2();
    h_S_4->Sumw2();
    h_CombiWithConvLeg_4->Sumw2();
    h_CombiWithoutConvLeg_4->Sumw2();
    h_HF_4->Sumw2();
    h_RPConv_4->Sumw2();

    h_SB_5->Sumw2();
    h_S_5->Sumw2();
    h_CombiWithConvLeg_5->Sumw2();
    h_CombiWithoutConvLeg_5->Sumw2();
    h_HF_5->Sumw2();
    h_RPConv_5->Sumw2();
  */
  



  
  //////////////////////////////////////// SIGNIFICANCES:


  TH1D *h_significance_S_1 = new TH1D();
  h_significance_S_1 = (TH1D*)h_S_1->Clone("h_significance_S_1");
  h_significance_S_1->Add(h_CombiWithoutConvLeg_1);
  h_significance_S_1->Add(h_HF_1);
    
  TH1D *h_significance_B_1 = new TH1D();
  h_significance_B_1 = (TH1D*)h_RPConv_1->Clone("h_significance_B_1");
  h_significance_B_1->Add(h_CombiWithConvLeg_1);

  
  TH1D *h_significance_S_2 = new TH1D();
  h_significance_S_2 = (TH1D*)h_S_2->Clone("h_significance_S_2");
  h_significance_S_2->Add(h_CombiWithoutConvLeg_2);
  h_significance_S_2->Add(h_HF_2);
    
  TH1D *h_significance_B_2 = new TH1D();
  h_significance_B_2 = (TH1D*)h_RPConv_2->Clone("h_significance_B_2");
  h_significance_B_2->Add(h_CombiWithConvLeg_2);

  
  TH1D *h_significance_S_3 = new TH1D();
  h_significance_S_3 = (TH1D*)h_S_3->Clone("h_significance_S_3");
  h_significance_S_3->Add(h_CombiWithoutConvLeg_3);
  h_significance_S_3->Add(h_HF_3);
    
  TH1D *h_significance_B_3 = new TH1D();
  h_significance_B_3 = (TH1D*)h_RPConv_3->Clone("h_significance_B_3");
  h_significance_B_3->Add(h_CombiWithConvLeg_3);

  /*
  TH1D *h_significance_S_4 = new TH1D();
  h_significance_S_4 = (TH1D*)h_S_4->Clone("h_significance_S_4");
  h_significance_S_4->Add(h_CombiWithoutConvLeg_4);
  h_significance_S_4->Add(h_HF_4);
    
  TH1D *h_significance_B_4 = new TH1D();
  h_significance_B_4 = (TH1D*)h_RPConv_4->Clone("h_significance_B_4");
  h_significance_B_4->Add(h_CombiWithConvLeg_4);


  TH1D *h_significance_S_5 = new TH1D();
  h_significance_S_5 = (TH1D*)h_S_5->Clone("h_significance_S_5");
  h_significance_S_5->Add(h_CombiWithoutConvLeg_5);
  h_significance_S_5->Add(h_HF_5);
    
  TH1D *h_significance_B_5 = new TH1D();
  h_significance_B_5 = (TH1D*)h_RPConv_5->Clone("h_significance_B_5");
  h_significance_B_5->Add(h_CombiWithConvLeg_5);
  */
  

  Int_t nBins = h_significance_S_1->GetNbinsX(),
    min = 0,
    max = h_significance_S_1->GetNbinsX();

  TH1D *h_significance_1 = new TH1D("h_significance_1","",nBins,min,max);
  TH1D *h_significance_2 = new TH1D("h_significance_2","",nBins,min,max);
  TH1D *h_significance_3 = new TH1D("h_significance_3","",nBins,min,max);
  // TH1D *h_significance_4 = new TH1D("h_significance_4","",nBins,min,max);
  // TH1D *h_significance_5 = new TH1D("h_significance_5","",nBins,min,max);


  Double_t significance_int_1 = 0,
    significance_int_2 = 0,
    significance_int_3 = 0;
    // significance_int_4 = 0,
    // significance_int_5 = 0;
  
  
  for(Int_t i=0; i<nBins; i++) {
    if(h_significance_S_1->GetBinContent(i) != 0) {
      h_significance_1->SetBinContent(i,
				      h_significance_S_1->GetBinContent(i) /
				      TMath::Sqrt(h_significance_S_1->GetBinContent(i) +
						  h_significance_B_1->GetBinContent(i)));
      significance_int_1 += h_significance_1->GetBinContent(i);
    }else {
      h_significance_1->SetBinContent(i, 0.);
    }

    if(h_significance_S_2->GetBinContent(i) != 0) {
      h_significance_2->SetBinContent(i,
				      h_significance_S_2->GetBinContent(i) /
				      TMath::Sqrt(h_significance_S_2->GetBinContent(i) +
						  h_significance_B_2->GetBinContent(i)));
      significance_int_2 += h_significance_2->GetBinContent(i);
    }else {
      h_significance_2->SetBinContent(i, 0.);
    }

    if(h_significance_S_3->GetBinContent(i) != 0) {
      h_significance_3->SetBinContent(i,
				      h_significance_S_3->GetBinContent(i) /
				      TMath::Sqrt(h_significance_S_3->GetBinContent(i) +
						  h_significance_B_3->GetBinContent(i)));
      significance_int_3 += h_significance_3->GetBinContent(i);
    }else {
      h_significance_3->SetBinContent(i, 0.);
    }

    /*
    if(h_significance_S_4->GetBinContent(i) != 0) {
      h_significance_4->SetBinContent(i,
				      h_significance_S_4->GetBinContent(i) /
				      TMath::Sqrt(h_significance_S_4->GetBinContent(i) +
						  h_significance_B_4->GetBinContent(i)));
      significance_int_4 += h_significance_4->GetBinContent(i);
    }else {
      h_significance_S_4->SetBinContent(i, 0.);
    }

    if(h_significance_S_5->GetBinContent(i) != 0) {
      h_significance_5->SetBinContent(i,
				      h_significance_S_5->GetBinContent(i) /
				      TMath::Sqrt(h_significance_S_5->GetBinContent(i) +
						  h_significance_B_5->GetBinContent(i)));
      significance_int_5 += h_significance_5->GetBinContent(i);
    }else {
      h_significance_S_5->SetBinContent(i, 0.);
    }
    */
  }


  h_significance_1->SetLineColor(kBlack);
  h_significance_1->SetMarkerColor(kBlack);
  h_significance_2->SetLineColor(kGreen+2);
  h_significance_2->SetMarkerColor(kGreen+2);
  h_significance_3->SetLineColor(kRed);
  h_significance_3->SetMarkerColor(kRed);
  // h_significance_4->SetLineColor(kBlue);
  // h_significance_4->SetMarkerColor(kBlue);
  // h_significance_5->SetLineColor(kBlue);
  // h_significance_5->SetMarkerColor(kBlue);

  h_significance_1->SetMarkerStyle(20);
  h_significance_1->SetMarkerSize(1);
  h_significance_2->SetMarkerStyle(26);
  h_significance_2->SetMarkerSize(1);
  h_significance_3->SetMarkerStyle(32);
  h_significance_3->SetMarkerSize(1);
  // h_significance_4->SetMarkerStyle(26);
  // h_significance_4->SetMarkerSize(1);
  // h_significance_5->SetMarkerStyle(32);
  // h_significance_5->SetMarkerSize(1);


  h_significance_1->SetXTitle("Mass bins");
  h_significance_1->SetYTitle("S/#sqrt{S+B}");
  h_significance_1->GetXaxis()->SetTitleOffset(1.2);
  h_significance_1->GetYaxis()->SetTitleOffset(1.5);


  gStyle->SetOptStat(0);
  
  TCanvas *c1 = new TCanvas("c1","",1024,768);
  c1->SetGridy();
  h_significance_1->Draw("p0");
  h_significance_2->Draw("p0 same");
  h_significance_3->Draw("p0 same");
  // h_significance_4->Draw("p0 same");
  // h_significance_5->Draw("p0 same");


  TString leg_label_1 = "TMVA-optimized MVA cut (sign. int. = ";
  leg_label_1 += significance_int_1;
  leg_label_1 += ")";

  TString leg_label_2 = "MVA cut: +10% (sign. int. = ";
  leg_label_2 += significance_int_2;
  leg_label_2 += ")";
  
  TString leg_label_3 = "MVA cut: -10% (sign. int. = ";
  leg_label_3 += significance_int_3;
  leg_label_3 += ")";
  
  // TString leg_label_4 = "MVA cut: +5% (sign. int. = ";
  // leg_label_4 += significance_int_4;
  // leg_label_4 += ")";
  
  // TString leg_label_5 = "MVA cut: -5% (sign. int. = ";
  // leg_label_5 += significance_int_5;
  // leg_label_5 += ")";

  

  TLegend *leg = new TLegend(.5,.7,.95,.95);
  leg->AddEntry(h_significance_1, leg_label_1, "p");
  leg->AddEntry(h_significance_2, leg_label_2, "p");
  leg->AddEntry(h_significance_3, leg_label_3, "p");
  // leg->AddEntry(h_significance_4, leg_label_4, "p");
  // leg->AddEntry(h_significance_5, leg_label_5, "p");

  leg->Draw();


  c1->SaveAs("temp_output/mass_significanceComparison.pdf");
  c1->SaveAs("temp_output/mass_significanceComparison.root");
  c1->SaveAs("temp_output/mass_significanceComparison.png");
  

  infile1->Close();
  infile2->Close();
  infile3->Close();
  // infile4->Close();
  // infile5->Close();



  
  //////////////////////////////////////// EFFICIENCIES:

    // optimized MVA cut
  TFile *infile1_eff = new TFile("/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/analysis_singleConvTrackRejMVAcuts/plots/singleConvTrackRejMVAcuts/mass_histos_prefilterCuts.root", "READ");
  TH1D *h_SB_eff_1                  = (TH1D*)infile1_eff->Get("h_SB_eff");
  TH1D *h_S_eff_1                   = (TH1D*)infile1_eff->Get("h_S_eff");
  TH1D *h_CombiWithConvLeg_eff_1    = (TH1D*)infile1_eff->Get("h_CombiWithConvLeg_eff");
  TH1D *h_CombiWithoutConvLeg_eff_1 = (TH1D*)infile1_eff->Get("h_CombiWithoutConvLeg_eff");
  TH1D *h_HF_eff_1                  = (TH1D*)infile1_eff->Get("h_HF_eff");
  TH1D *h_RPConv_eff_1              = (TH1D*)infile1_eff->Get("h_RPConv_eff");
  
  h_SB_eff_1->SetName("h_SB_eff_1");
  h_S_eff_1->SetName("h_S_eff_1");
  h_CombiWithConvLeg_eff_1->SetName("h_CombiWithConvLeg_eff_1");
  h_CombiWithoutConvLeg_eff_1->SetName("h_CombiWithoutConvLeg_eff_1");
  h_HF_eff_1->SetName("h_HF_eff_1");
  h_RPConv_eff_1->SetName("h_RPConv_eff_1");
  
  
  // MVA cut +10%
  TFile *infile2_eff = new TFile("/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/analysis_singleConvTrackRejMVAcuts/plots/singleConvTrackRejMVAcuts-cutVariations/MVAcutPlus10Percent/mass_histos_prefilterCuts.root", "READ");
  TH1D *h_SB_eff_2                  = (TH1D*)infile2_eff->Get("h_SB_eff");
  TH1D *h_S_eff_2                   = (TH1D*)infile2_eff->Get("h_S_eff");
  TH1D *h_CombiWithConvLeg_eff_2    = (TH1D*)infile2_eff->Get("h_CombiWithConvLeg_eff");
  TH1D *h_CombiWithoutConvLeg_eff_2 = (TH1D*)infile2_eff->Get("h_CombiWithoutConvLeg_eff");
  TH1D *h_HF_eff_2                  = (TH1D*)infile2_eff->Get("h_HF_eff");
  TH1D *h_RPConv_eff_2              = (TH1D*)infile2_eff->Get("h_RPConv_eff");

  
  h_SB_eff_2->SetName("h_SB_eff_2");
  h_S_eff_2->SetName("h_S_eff_2");
  h_CombiWithConvLeg_eff_2->SetName("h_CombiWithConvLeg_eff_2");
  h_CombiWithoutConvLeg_eff_2->SetName("h_CombiWithoutConvLeg_eff_2");
  h_HF_eff_2->SetName("h_HF_eff_2");
  h_RPConv_eff_2->SetName("h_RPConv_eff_2");


  // MVA cut -10%
  TFile *infile3_eff = new TFile("/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/analysis_singleConvTrackRejMVAcuts/plots/singleConvTrackRejMVAcuts-cutVariations/MVAcutMinus10Percent/mass_histos_prefilterCuts.root", "READ");
  TH1D *h_SB_eff_3                  = (TH1D*)infile3_eff->Get("h_SB_eff");
  TH1D *h_S_eff_3                   = (TH1D*)infile3_eff->Get("h_S_eff");
  TH1D *h_CombiWithConvLeg_eff_3    = (TH1D*)infile3_eff->Get("h_CombiWithConvLeg_eff");
  TH1D *h_CombiWithoutConvLeg_eff_3 = (TH1D*)infile3_eff->Get("h_CombiWithoutConvLeg_eff");
  TH1D *h_HF_eff_3                  = (TH1D*)infile3_eff->Get("h_HF_eff");
  TH1D *h_RPConv_eff_3              = (TH1D*)infile3_eff->Get("h_RPConv_eff");

  
  h_SB_eff_3->SetName("h_SB_eff_3");
  h_S_eff_3->SetName("h_S_eff_3");
  h_CombiWithConvLeg_eff_3->SetName("h_CombiWithConvLeg_eff_3");
  h_CombiWithoutConvLeg_eff_3->SetName("h_CombiWithoutConvLeg_eff_3");
  h_HF_eff_3->SetName("h_HF_eff_3");
  h_RPConv_eff_3->SetName("h_RPConv_eff_3");

  
  h_SB_eff_1->SetLineColor(kBlack);
  h_SB_eff_1->SetMarkerColor(kBlack);
  h_S_eff_1->SetLineColor(kGreen+1);
  h_S_eff_1->SetMarkerColor(kGreen+1);
  h_CombiWithConvLeg_eff_1->SetLineColor(kRed);
  h_CombiWithConvLeg_eff_1->SetMarkerColor(kRed);
  h_CombiWithoutConvLeg_eff_1->SetLineColor(kBlue);
  h_CombiWithoutConvLeg_eff_1->SetMarkerColor(kBlue);
  h_HF_eff_1->SetLineColor(kOrange);
  h_HF_eff_1->SetMarkerColor(kOrange);
  h_RPConv_eff_1->SetLineColor(kMagenta);
  h_RPConv_eff_1->SetMarkerColor(kMagenta);

  h_SB_eff_2->SetLineColor(kBlack);
  h_SB_eff_2->SetMarkerColor(kBlack);
  h_S_eff_2->SetLineColor(kGreen+1);
  h_S_eff_2->SetMarkerColor(kGreen+1);
  h_CombiWithConvLeg_eff_2->SetLineColor(kRed);
  h_CombiWithConvLeg_eff_2->SetMarkerColor(kRed);
  h_CombiWithoutConvLeg_eff_2->SetLineColor(kBlue);
  h_CombiWithoutConvLeg_eff_2->SetMarkerColor(kBlue);
  h_HF_eff_2->SetLineColor(kOrange);
  h_HF_eff_2->SetMarkerColor(kOrange);
  h_RPConv_eff_2->SetLineColor(kMagenta);
  h_RPConv_eff_2->SetMarkerColor(kMagenta);

  h_SB_eff_3->SetLineColor(kBlack);
  h_SB_eff_3->SetMarkerColor(kBlack);
  h_S_eff_3->SetLineColor(kGreen+1);
  h_S_eff_3->SetMarkerColor(kGreen+1);
  h_CombiWithConvLeg_eff_3->SetLineColor(kRed);
  h_CombiWithConvLeg_eff_3->SetMarkerColor(kRed);
  h_CombiWithoutConvLeg_eff_3->SetLineColor(kBlue);
  h_CombiWithoutConvLeg_eff_3->SetMarkerColor(kBlue);
  h_HF_eff_3->SetLineColor(kOrange);
  h_HF_eff_3->SetMarkerColor(kOrange);
  h_RPConv_eff_3->SetLineColor(kMagenta);
  h_RPConv_eff_3->SetMarkerColor(kMagenta);

  
  h_SB_eff_1->SetMarkerStyle(7);
  h_S_eff_1->SetMarkerStyle(7);
  h_CombiWithConvLeg_eff_1->SetMarkerStyle(7);
  h_CombiWithoutConvLeg_eff_1->SetMarkerStyle(7);
  h_HF_eff_1->SetMarkerStyle(7);
  h_RPConv_eff_1->SetMarkerStyle(7);
  
  h_SB_eff_2->SetMarkerStyle(26);
  h_S_eff_2->SetMarkerStyle(26);
  h_CombiWithConvLeg_eff_2->SetMarkerStyle(26);
  h_CombiWithoutConvLeg_eff_2->SetMarkerStyle(26);
  h_HF_eff_2->SetMarkerStyle(26);
  h_RPConv_eff_2->SetMarkerStyle(26);
  
  h_SB_eff_3->SetMarkerStyle(32);
  h_S_eff_3->SetMarkerStyle(32);
  h_CombiWithConvLeg_eff_3->SetMarkerStyle(32);
  h_CombiWithoutConvLeg_eff_3->SetMarkerStyle(32);
  h_HF_eff_3->SetMarkerStyle(32);
  h_RPConv_eff_3->SetMarkerStyle(32);



  h_CombiWithConvLeg_eff_1->SetXTitle("M_{ee} / (GeV/c^{2})");
  h_CombiWithConvLeg_eff_1->SetYTitle("Efficiency");
  h_CombiWithConvLeg_eff_1->GetXaxis()->SetTitleOffset(1.2);
  h_CombiWithConvLeg_eff_1->GetYaxis()->SetTitleOffset(1.3);

  
  TCanvas *c2 = new TCanvas("c2","",1024,768);
  c2->SetGridy();

  TString h_drawOptions = "e1 x0";
  TString h_drawOptions_same = h_drawOptions;
  h_drawOptions_same += " same";
  
  h_CombiWithConvLeg_eff_1->Draw(h_drawOptions);
  h_CombiWithoutConvLeg_eff_1->Draw(h_drawOptions_same);

  h_CombiWithConvLeg_eff_2->Draw(h_drawOptions_same);
  h_CombiWithoutConvLeg_eff_2->Draw(h_drawOptions_same);

  h_CombiWithConvLeg_eff_3->Draw(h_drawOptions_same);
  h_CombiWithoutConvLeg_eff_3->Draw(h_drawOptions_same);


  TLegend *leg2 = new TLegend(.6,.8,.95,.95);
  leg2->AddEntry(h_CombiWithConvLeg_eff_1, "Comb. w. conv. leg", "p");
  leg2->AddEntry(h_CombiWithConvLeg_eff_2, "   MVA cut var.: +10%", "p");
  leg2->AddEntry(h_CombiWithConvLeg_eff_3, "   MVA cut var.: -10%", "p");
  leg2->AddEntry(h_CombiWithoutConvLeg_eff_1, "Comb. w/o conv. leg", "p");
  leg2->AddEntry(h_CombiWithoutConvLeg_eff_2, "   MVA cut var.: +10%", "p");
  leg2->AddEntry(h_CombiWithoutConvLeg_eff_3, "   MVA cut var.: -10%", "p");
  leg2->Draw();


  c2->SaveAs("temp_output/mass_efficiencyComparison.pdf");
  c2->SaveAs("temp_output/mass_efficiencyComparison.root");
  c2->SaveAs("temp_output/mass_efficiencyComparison.png");



  infile1_eff->Close();
  infile2_eff->Close();
  infile3_eff->Close();

  // gApplication->Terminate();
}
