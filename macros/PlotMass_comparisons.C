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

  TString infile1_filename = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/analysis_singleConvTrackRejMVAcuts/plots/RPConvRejMVAcuts_prefilter/mass_histos_prefilterCuts.root";
  TString infile2_filename = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/analysis_singleConvTrackRejMVAcuts/plots/RPConvRejMVAcuts-cutVariations_prefilter/RPConvRejMVAcuts_plus10percent_prefilter/mass_histos_prefilterCuts.root";
  TString infile3_filename = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/analysis_singleConvTrackRejMVAcuts/plots/RPConvRejMVAcuts-cutVariations_prefilter/RPConvRejMVAcuts_minus10percent_prefilter/mass_histos_prefilterCuts.root";
  
  TString infile1_eff_filename = infile1_filename;
  TString infile2_eff_filename = infile2_filename;
  TString infile3_eff_filename = infile3_filename;

  TString outfile_filename = "temp_output/mass_histos_comparison.root";
  
  
  // optimized MVA cut
  TFile *infile1 = new TFile(infile1_filename, "READ");
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
  TFile *infile2 = new TFile(infile2_filename, "READ");
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
  TFile *infile3 = new TFile(infile3_filename, "READ");
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


  std::cout << "All files read succesfully..." << std::endl;
  



  
  //////////////////////////////////////// SIGNIFICANCE, SIGNAL OVER BACKGROUND:


  TH1D *h_plot_S_1 = new TH1D();
  h_plot_S_1 = (TH1D*)h_S_1->Clone("h_plot_S_1");
  h_plot_S_1->Add(h_CombiWithoutConvLeg_1);
  h_plot_S_1->Add(h_HF_1);
  // h_plot_S_1->Add(h_RPConv_1);
  h_plot_S_1->Sumw2();
    
  TH1D *h_plot_B_1 = new TH1D();
  // h_plot_B_1 = (TH1D*)h_CombiWithConvLeg_1->Clonse("h_plot_B_1");
  h_plot_B_1 = (TH1D*)h_RPConv_1->Clone("h_plot_B_1");
  h_plot_B_1->Add(h_CombiWithConvLeg_1);
  h_plot_B_1->Sumw2();

  
  TH1D *h_plot_S_2 = new TH1D();
  h_plot_S_2 = (TH1D*)h_S_2->Clone("h_plot_S_2");
  h_plot_S_2->Add(h_CombiWithoutConvLeg_2);
  h_plot_S_2->Add(h_HF_2);
  // h_plot_S_2->Add(h_RPConv_2);
  h_plot_S_2->Sumw2();
    
  TH1D *h_plot_B_2 = new TH1D();
  // h_plot_B_2 = (TH1D*)h_CombiWithConvLeg_2->Clone("h_plot_B_2");
  h_plot_B_2 = (TH1D*)h_RPConv_2->Clone("h_plot_B_2");
  h_plot_B_2->Add(h_CombiWithConvLeg_2);
  h_plot_B_2->Sumw2();

  
  TH1D *h_plot_S_3 = new TH1D();
  h_plot_S_3 = (TH1D*)h_S_3->Clone("h_plot_S_3");
  h_plot_S_3->Add(h_CombiWithoutConvLeg_3);
  h_plot_S_3->Add(h_HF_3);
  // h_plot_S_3->Add(h_RPConv_3);
  h_plot_S_3->Sumw2();
    
  TH1D *h_plot_B_3 = new TH1D();
  // h_plot_B_3 = (TH1D*)h_CombiWithConvLeg_3->Clone("h_plot_B_3");
  h_plot_B_3 = (TH1D*)h_RPConv_3->Clone("h_plot_B_3");
  h_plot_B_3->Add(h_CombiWithConvLeg_3);
  h_plot_B_3->Sumw2();



  TH1D *h_plot_S_exp_1 = new TH1D();
  h_plot_S_exp_1 = (TH1D*)h_S_1->Clone("h_plot_S_exp_1");
  h_plot_S_exp_1->Add(h_HF_1);
  h_plot_S_exp_1->Sumw2();

  TH1D *h_plot_B_exp_1 = new TH1D();
  h_plot_B_exp_1 = (TH1D*)h_CombiWithConvLeg_1->Clone("h_plot_B_exp_1");
  h_plot_B_exp_1->Add(h_CombiWithoutConvLeg_1);
  h_plot_B_exp_1->Add(h_RPConv_1);
  h_plot_B_exp_1->Sumw2();

  TH1D *h_plot_S_exp_2 = new TH1D();
  h_plot_S_exp_2 = (TH1D*)h_S_2->Clone("h_plot_S_exp_2");
  h_plot_S_exp_2->Add(h_HF_2);
  h_plot_S_exp_2->Sumw2();

  TH1D *h_plot_B_exp_2 = new TH1D();
  h_plot_B_exp_2 = (TH1D*)h_CombiWithConvLeg_2->Clone("h_plot_B_exp_2");
  h_plot_B_exp_2->Add(h_CombiWithoutConvLeg_2);
  h_plot_B_exp_2->Add(h_RPConv_2);
  h_plot_B_exp_2->Sumw2();

  TH1D *h_plot_S_exp_3 = new TH1D();
  h_plot_S_exp_3 = (TH1D*)h_S_3->Clone("h_plot_S_exp_3");
  h_plot_S_exp_3->Add(h_HF_3);
  h_plot_S_exp_3->Sumw2();

  TH1D *h_plot_B_exp_3 = new TH1D();
  h_plot_B_exp_3 = (TH1D*)h_CombiWithConvLeg_3->Clone("h_plot_B_exp_3");
  h_plot_B_exp_3->Add(h_CombiWithoutConvLeg_3);
  h_plot_B_exp_3->Add(h_RPConv_3);
  h_plot_B_exp_3->Sumw2();


  

  Int_t nBins = h_plot_S_1->GetNbinsX(),
    min = 0,
    max = h_plot_S_1->GetNbinsX();

  TH1D *h_significance_1 = new TH1D("h_significance_1", "", nBins, min, max);
  TH1D *h_significance_2 = new TH1D("h_significance_2", "", nBins, min, max);
  TH1D *h_significance_3 = new TH1D("h_significance_3", "", nBins, min, max);

  
  TH1D *h_SoverB_1 = new TH1D("h_SoverB_1", "", nBins, min, max);
  TH1D *h_SoverB_2 = new TH1D("h_SoverB_2", "", nBins, min, max);
  TH1D *h_SoverB_3 = new TH1D("h_SoverB_3", "", nBins, min, max);


  TH1D *h_significance_exp_1 = new TH1D("h_significance_exp_1", "", nBins, min, max);
  TH1D *h_significance_exp_2 = new TH1D("h_significance_exp_2", "", nBins, min, max);
  TH1D *h_significance_exp_3 = new TH1D("h_significance_exp_3", "", nBins, min, max);

  TH1D *h_SoverB_exp_1 = new TH1D("h_SoverB_exp_1", "", nBins, min, max);
  TH1D *h_SoverB_exp_2 = new TH1D("h_SoverB_exp_2", "", nBins, min, max);
  TH1D *h_SoverB_exp_3 = new TH1D("h_SoverB_exp_3", "", nBins, min, max);

  

  Double_t significance_int_1 = 0,
    significance_int_2 = 0,
    significance_int_3 = 0;

  Double_t significance_exp_int_1 = 0,
    significance_exp_int_2 = 0,
    significance_exp_int_3 = 0;
  
  
  
  for(Int_t i=0; i<nBins; i++) {

    // significance:
    if(h_plot_S_1->GetBinContent(i) != 0) {
      h_significance_1->SetBinContent(i,
				      h_plot_S_1->GetBinContent(i) /
				      TMath::Sqrt(h_plot_S_1->GetBinContent(i) +
						  h_plot_B_1->GetBinContent(i)));
      significance_int_1 += h_significance_1->GetBinContent(i);
    }else {
      h_significance_1->SetBinContent(i, 0.);
    }

    if(h_plot_S_2->GetBinContent(i) != 0) {
      h_significance_2->SetBinContent(i,
				      h_plot_S_2->GetBinContent(i) /
				      TMath::Sqrt(h_plot_S_2->GetBinContent(i) +
						  h_plot_B_2->GetBinContent(i)));
      significance_int_2 += h_significance_2->GetBinContent(i);
    }else {
      h_significance_2->SetBinContent(i, 0.);
    }

    if(h_plot_S_3->GetBinContent(i) != 0) {
      h_significance_3->SetBinContent(i,
				      h_plot_S_3->GetBinContent(i) /
				      TMath::Sqrt(h_plot_S_3->GetBinContent(i) +
						  h_plot_B_3->GetBinContent(i)));
      significance_int_3 += h_significance_3->GetBinContent(i);
    }else {
      h_significance_3->SetBinContent(i, 0.);
    }


    // significance (exp.):
    if(h_plot_S_exp_1->GetBinContent(i) != 0) {
      h_significance_exp_1->SetBinContent(i,
					  h_plot_S_exp_1->GetBinContent(i) /
					  TMath::Sqrt(h_plot_S_exp_1->GetBinContent(i) +
						      h_plot_B_exp_1->GetBinContent(i)));
      significance_exp_int_1 += h_significance_exp_1->GetBinContent(i);
    }else {
      h_significance_exp_1->SetBinContent(i, 0.);
    }

    if(h_plot_S_exp_2->GetBinContent(i) != 0) {
      h_significance_exp_2->SetBinContent(i,
				      h_plot_S_exp_2->GetBinContent(i) /
				      TMath::Sqrt(h_plot_S_exp_2->GetBinContent(i) +
						  h_plot_B_exp_2->GetBinContent(i)));
      significance_exp_int_2 += h_significance_exp_2->GetBinContent(i);
    }else {
      h_significance_exp_2->SetBinContent(i, 0.);
    }

    if(h_plot_S_exp_3->GetBinContent(i) != 0) {
      h_significance_exp_3->SetBinContent(i,
				      h_plot_S_exp_3->GetBinContent(i) /
				      TMath::Sqrt(h_plot_S_exp_3->GetBinContent(i) +
						  h_plot_B_exp_3->GetBinContent(i)));
      significance_exp_int_3 += h_significance_exp_3->GetBinContent(i);
    }else {
      h_significance_exp_3->SetBinContent(i, 0.);
    }
    

    // signal over background:
    if(h_plot_B_1->GetBinContent(i) != 0) {
      h_SoverB_1->SetBinContent(i,
				h_plot_S_1->GetBinContent(i) /
				h_plot_B_1->GetBinContent(i));
    }else {
      h_SoverB_1->SetBinContent(i, 0);
    }

    if(h_plot_B_2->GetBinContent(i) != 0) {
      h_SoverB_2->SetBinContent(i,
				h_plot_S_2->GetBinContent(i) /
				h_plot_B_2->GetBinContent(i));
    }else {
      h_SoverB_2->SetBinContent(i, 0);
    }

    if(h_plot_B_3->GetBinContent(i) != 0) {
      h_SoverB_3->SetBinContent(i,
				h_plot_S_3->GetBinContent(i) /
				h_plot_B_3->GetBinContent(i));
    }else {
      h_SoverB_3->SetBinContent(i, 0);
    }


   // signal over background (exp.):
    if(h_plot_B_exp_1->GetBinContent(i) != 0) {
      h_SoverB_exp_1->SetBinContent(i,
				h_plot_S_exp_1->GetBinContent(i) /
				h_plot_B_exp_1->GetBinContent(i));
    }else {
      h_SoverB_exp_1->SetBinContent(i, 0);
    }

    if(h_plot_B_exp_2->GetBinContent(i) != 0) {
      h_SoverB_exp_2->SetBinContent(i,
				h_plot_S_exp_2->GetBinContent(i) /
				h_plot_B_exp_2->GetBinContent(i));
    }else {
      h_SoverB_exp_2->SetBinContent(i, 0);
    }

    if(h_plot_B_exp_3->GetBinContent(i) != 0) {
      h_SoverB_exp_3->SetBinContent(i,
				h_plot_S_exp_3->GetBinContent(i) /
				h_plot_B_exp_3->GetBinContent(i));
    }else {
      h_SoverB_exp_3->SetBinContent(i, 0);
    }
    
  }
  


  h_significance_1->SetLineColor(kBlack);
  h_significance_1->SetMarkerColor(kBlack);
  h_significance_2->SetLineColor(kGreen+2);
  h_significance_2->SetMarkerColor(kGreen+2);
  h_significance_3->SetLineColor(kRed);
  h_significance_3->SetMarkerColor(kRed);

  h_significance_exp_1->SetLineColor(kBlack);
  h_significance_exp_1->SetMarkerColor(kBlack);
  h_significance_exp_2->SetLineColor(kGreen+2);
  h_significance_exp_2->SetMarkerColor(kGreen+2);
  h_significance_exp_3->SetLineColor(kRed);
  h_significance_exp_3->SetMarkerColor(kRed);


  h_significance_1->SetMarkerStyle(20);
  h_significance_1->SetMarkerSize(1);
  h_significance_2->SetMarkerStyle(26);
  h_significance_2->SetMarkerSize(1);
  h_significance_3->SetMarkerStyle(32);
  h_significance_3->SetMarkerSize(1);

  h_significance_exp_1->SetMarkerStyle(20);
  h_significance_exp_1->SetMarkerSize(1);
  h_significance_exp_2->SetMarkerStyle(26);
  h_significance_exp_2->SetMarkerSize(1);
  h_significance_exp_3->SetMarkerStyle(32);
  h_significance_exp_3->SetMarkerSize(1);
  
  

  h_SoverB_1->SetLineColor(kBlack);
  h_SoverB_1->SetMarkerColor(kBlack);
  h_SoverB_2->SetLineColor(kGreen+2);
  h_SoverB_2->SetMarkerColor(kGreen+2);
  h_SoverB_3->SetLineColor(kRed);
  h_SoverB_3->SetMarkerColor(kRed);

  h_SoverB_exp_1->SetLineColor(kBlack);
  h_SoverB_exp_1->SetMarkerColor(kBlack);
  h_SoverB_exp_2->SetLineColor(kGreen+2);
  h_SoverB_exp_2->SetMarkerColor(kGreen+2);
  h_SoverB_exp_3->SetLineColor(kRed);
  h_SoverB_exp_3->SetMarkerColor(kRed);
  

  h_SoverB_1->SetMarkerStyle(20);
  h_SoverB_1->SetMarkerSize(1);
  h_SoverB_2->SetMarkerStyle(26);
  h_SoverB_2->SetMarkerSize(1);
  h_SoverB_3->SetMarkerStyle(32);
  h_SoverB_3->SetMarkerSize(1);

  h_SoverB_exp_1->SetMarkerStyle(20);
  h_SoverB_exp_1->SetMarkerSize(1);
  h_SoverB_exp_2->SetMarkerStyle(26);
  h_SoverB_exp_2->SetMarkerSize(1);
  h_SoverB_exp_3->SetMarkerStyle(32);
  h_SoverB_exp_3->SetMarkerSize(1);


  
  h_significance_1->SetXTitle("Mass bins");
  h_significance_1->SetYTitle("S/#sqrt{S+B}");
  h_significance_1->GetXaxis()->SetTitleOffset(1.2);
  h_significance_1->GetYaxis()->SetTitleOffset(1.5);

  h_significance_exp_1->SetXTitle("Mass bins");
  h_significance_exp_1->SetYTitle("S_{exp}/#sqrt{S_{exp}+B_{exp}}");
  h_significance_exp_1->GetXaxis()->SetTitleOffset(1.2);
  h_significance_exp_1->GetYaxis()->SetTitleOffset(1.5);


  h_SoverB_1->SetXTitle("Mass bins");
  h_SoverB_1->SetYTitle("S/B");
  h_SoverB_1->GetXaxis()->SetTitleOffset(1.2);
  h_SoverB_1->GetYaxis()->SetTitleOffset(1.5);

  h_SoverB_exp_1->SetXTitle("Mass bins");
  h_SoverB_exp_1->SetYTitle("S_{exp}/B_{exp}");
  h_SoverB_exp_1->GetXaxis()->SetTitleOffset(1.2);
  h_SoverB_exp_1->GetYaxis()->SetTitleOffset(1.5);


  gStyle->SetOptStat(0);


  TCanvas *c_significance = new TCanvas("c_significance","",1024,768);
  c_significance->SetGridy();
  h_significance_1->Draw("p0");
  h_significance_2->Draw("p0 same");
  h_significance_3->Draw("p0 same");


  TString leg_label_1 = "TMVA-optimized MVA cut (sign. int. = ";
  leg_label_1 += significance_int_1;
  leg_label_1 += ")";

  TString leg_label_2 = "MVA cut: +10% (sign. int. = ";
  leg_label_2 += significance_int_2;
  leg_label_2 += ")";
  
  TString leg_label_3 = "MVA cut: -10% (sign. int. = ";
  leg_label_3 += significance_int_3;
  leg_label_3 += ")";

  

  TLegend *leg_significance = new TLegend(.5,.7,.95,.95);
  leg_significance->AddEntry(h_significance_1, leg_label_1, "p");
  leg_significance->AddEntry(h_significance_2, leg_label_2, "p");
  leg_significance->AddEntry(h_significance_3, leg_label_3, "p");
  leg_significance->Draw();


  c_significance->SaveAs("temp_output/mass_significanceComparison.pdf");
  c_significance->SaveAs("temp_output/mass_significanceComparison.root");
  c_significance->SaveAs("temp_output/mass_significanceComparison.png");


  


  TCanvas *c_significance_exp = new TCanvas("c_significance_exp","",1024,768);
  c_significance_exp->SetGridy();
  h_significance_exp_1->Draw("p0 e1 x0");
  h_significance_exp_2->Draw("p0 same");
  h_significance_exp_3->Draw("p0 same");


  TString leg_label_exp_1 = "TMVA-optimized MVA cut (sign. int. = ";
  leg_label_exp_1 += significance_exp_int_1;
  leg_label_exp_1 += ")";

  TString leg_label_exp_2 = "MVA cut: +10% (sign. int. = ";
  leg_label_exp_2 += significance_exp_int_2;
  leg_label_exp_2 += ")";
  
  TString leg_label_exp_3 = "MVA cut: -10% (sign. int. = ";
  leg_label_exp_3 += significance_exp_int_3;
  leg_label_exp_3 += ")";

  

  TLegend *leg_significance_exp = new TLegend(.5,.7,.95,.95);
  leg_significance_exp->AddEntry(h_significance_exp_1, leg_label_exp_1, "p");
  leg_significance_exp->AddEntry(h_significance_exp_2, leg_label_exp_2, "p");
  leg_significance_exp->AddEntry(h_significance_exp_3, leg_label_exp_3, "p");
  leg_significance_exp->Draw();


  c_significance_exp->SaveAs("temp_output/mass_significance_exp_Comparison.pdf");
  c_significance_exp->SaveAs("temp_output/mass_significance_exp_Comparison.root");
  c_significance_exp->SaveAs("temp_output/mass_significance_exp_Comparison.png");
  



  
  
  TCanvas *c_SoverB = new TCanvas("c_SoverB","",1024,768);
  c_SoverB->SetGridy();
  h_SoverB_1->Draw("p0");
  h_SoverB_2->Draw("p0 same");
  h_SoverB_3->Draw("p0 same");

  TLegend *leg_SoverB = new TLegend(.6,.75,.95,.95);
  leg_SoverB->AddEntry(h_SoverB_1, "TMVA-optimized MVA cut", "p");
  leg_SoverB->AddEntry(h_SoverB_2, "MVA cut: +10%", "p");
  leg_SoverB->AddEntry(h_SoverB_3, "MVA cut: -10%", "p");
  leg_SoverB->Draw();

  
  c_SoverB->SaveAs("temp_output/mass_SoverBComparison.pdf");
  c_SoverB->SaveAs("temp_output/mass_SoverBComparison.root");
  c_SoverB->SaveAs("temp_output/mass_SoverBComparison.png");



  TCanvas *c_SoverB_exp = new TCanvas("c_SoverB_exp","",1024,768);
  c_SoverB_exp->SetGridy();
  c_SoverB_exp->SetLogy();
  h_SoverB_exp_1->Draw("p0 e1 x0");
  h_SoverB_exp_2->Draw("p0 same");
  h_SoverB_exp_3->Draw("p0 same");

  TLegend *leg_SoverB_exp = new TLegend(.6,.75,.95,.95);
  leg_SoverB_exp->AddEntry(h_SoverB_exp_1, "TMVA-optimized MVA cut", "p");
  leg_SoverB_exp->AddEntry(h_SoverB_exp_2, "MVA cut: +10%", "p");
  leg_SoverB_exp->AddEntry(h_SoverB_exp_3, "MVA cut: -10%", "p");
  leg_SoverB_exp->Draw();

  
  c_SoverB_exp->SaveAs("temp_output/mass_SoverB_exp_Comparison.pdf");
  c_SoverB_exp->SaveAs("temp_output/mass_SoverB_exp_Comparison.root");
  c_SoverB_exp->SaveAs("temp_output/mass_SoverB_exp_Comparison.png");
  

  

  // infile1->Close();
  // infile2->Close();
  // infile3->Close();


  
  
  //////////////////////////////////////// EFFICIENCIES:

  // optimized MVA cut
  TFile *infile1_eff = new TFile(infile1_eff_filename, "READ");
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
  TFile *infile2_eff = new TFile(infile2_eff_filename, "READ");
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
  TFile *infile3_eff = new TFile(infile3_eff_filename, "READ");
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

  
  TCanvas *c_eff = new TCanvas("c_eff","",1024,768);
  c_eff->SetGridy();

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


  c_eff->SaveAs("temp_output/mass_efficiencyComparison.pdf");
  c_eff->SaveAs("temp_output/mass_efficiencyComparison.root");
  c_eff->SaveAs("temp_output/mass_efficiencyComparison.png");



  // infile1_eff->Close();
  // infile2_eff->Close();
  // infile3_eff->Close();



  TFile *outfile = new TFile(outfile_filename, "RECREATE");
  
  h_SB_1->Write(0, TObject::kOverwrite);
  h_S_1->Write(0, TObject::kOverwrite);
  h_CombiWithConvLeg_1->Write(0, TObject::kOverwrite);
  h_CombiWithoutConvLeg_1->Write(0, TObject::kOverwrite);
  h_HF_1->Write(0, TObject::kOverwrite);
  h_RPConv_1->Write(0, TObject::kOverwrite);

  h_SB_eff_1->Write(0, TObject::kOverwrite);
  h_S_eff_1->Write(0, TObject::kOverwrite);
  h_CombiWithConvLeg_eff_1->Write(0, TObject::kOverwrite);
  h_CombiWithoutConvLeg_eff_1->Write(0, TObject::kOverwrite);
  h_HF_eff_1->Write(0, TObject::kOverwrite);
  h_RPConv_eff_1->Write(0, TObject::kOverwrite);

  h_plot_S_1->Write(0, TObject::kOverwrite);
  h_plot_B_1->Write(0, TObject::kOverwrite);

  h_plot_S_exp_1->Write(0, TObject::kOverwrite);
  h_plot_B_exp_1->Write(0, TObject::kOverwrite);

  h_significance_1->Write(0, TObject::kOverwrite);
  h_SoverB_1->Write(0, TObject::kOverwrite);

  h_significance_exp_1->Write(0, TObject::kOverwrite);
  h_SoverB_exp_1->Write(0, TObject::kOverwrite);

  
  h_SB_2->Write(0, TObject::kOverwrite);
  h_S_2->Write(0, TObject::kOverwrite);
  h_CombiWithConvLeg_2->Write(0, TObject::kOverwrite);
  h_CombiWithoutConvLeg_2->Write(0, TObject::kOverwrite);
  h_HF_2->Write(0, TObject::kOverwrite);
  h_RPConv_2->Write(0, TObject::kOverwrite);

  h_SB_eff_2->Write(0, TObject::kOverwrite);
  h_S_eff_2->Write(0, TObject::kOverwrite);
  h_CombiWithConvLeg_eff_2->Write(0, TObject::kOverwrite);
  h_CombiWithoutConvLeg_eff_2->Write(0, TObject::kOverwrite);
  h_HF_eff_2->Write(0, TObject::kOverwrite);
  h_RPConv_eff_2->Write(0, TObject::kOverwrite);

  h_plot_S_2->Write(0, TObject::kOverwrite);
  h_plot_B_2->Write(0, TObject::kOverwrite);

  h_plot_S_exp_2->Write(0, TObject::kOverwrite);
  h_plot_B_exp_2->Write(0, TObject::kOverwrite);

  h_significance_2->Write(0, TObject::kOverwrite);
  h_SoverB_2->Write(0, TObject::kOverwrite);

  h_significance_exp_2->Write(0, TObject::kOverwrite);
  h_SoverB_exp_2->Write(0, TObject::kOverwrite);


  h_SB_3->Write(0, TObject::kOverwrite);
  h_S_3->Write(0, TObject::kOverwrite);
  h_CombiWithConvLeg_3->Write(0, TObject::kOverwrite);
  h_CombiWithoutConvLeg_3->Write(0, TObject::kOverwrite);
  h_HF_3->Write(0, TObject::kOverwrite);
  h_RPConv_3->Write(0, TObject::kOverwrite);

  h_SB_eff_3->Write(0, TObject::kOverwrite);
  h_S_eff_3->Write(0, TObject::kOverwrite);
  h_CombiWithConvLeg_eff_3->Write(0, TObject::kOverwrite);
  h_CombiWithoutConvLeg_eff_3->Write(0, TObject::kOverwrite);
  h_HF_eff_3->Write(0, TObject::kOverwrite);
  h_RPConv_eff_3->Write(0, TObject::kOverwrite);

  h_plot_S_3->Write(0, TObject::kOverwrite);
  h_plot_B_3->Write(0, TObject::kOverwrite);

  h_plot_S_exp_3->Write(0, TObject::kOverwrite);
  h_plot_B_exp_3->Write(0, TObject::kOverwrite);

  h_significance_3->Write(0, TObject::kOverwrite);
  h_SoverB_3->Write(0, TObject::kOverwrite);

  h_significance_exp_3->Write(0, TObject::kOverwrite);
  h_SoverB_exp_3->Write(0, TObject::kOverwrite);


  outfile->Close();


  

  gApplication->Terminate();
}
