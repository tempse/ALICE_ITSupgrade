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

void SetStyle(Bool_t graypalette=kFALSE);

void PlotPhiv() {

  gROOT->ForceStyle();
  SetStyle();
  
  TString filename = "~/analysis/data/FT2_AnalysisResults_Upgrade/workingData/DNNAnalysis/FT2_ITSup_pairTree-us_part2_1-9-split.root";
  TString treename = "pairTree_us";
  
  TFile *infile = new TFile(filename, "READ");
  TTree *tree = (TTree*)infile->Get(treename);

  TH1D *convs = new TH1D("convs","",100,0,TMath::Pi());
  TH1D *nonconvs = new TH1D("nonconvs","",100,0,TMath::Pi());


  Float_t lineWidth = 3;
  TString XTitle = "#phi_{V} (rad)";
  TString YTitle = "Yield (arb. units)";
  
  TCanvas *c = new TCanvas("c","",768,768);
  tree->Draw("phiv>>convs","IsRP&&IsConv","");
  convs->SetLineColor(kRed);
  convs->SetLineWidth(lineWidth);
  convs->SetXTitle(XTitle);
  convs->SetYTitle(YTitle);
  
  tree->Draw("phiv>>nonconvs","IsRP&&!IsConv","same");
  nonconvs->SetLineColor(kGreen+1);
  nonconvs->SetLineWidth(lineWidth);
  nonconvs->SetXTitle(XTitle);
  nonconvs->SetYTitle(YTitle);

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
  leg->Draw();

  c->SaveAs("temp_output/phiv_contributions.pdf");
  c->SaveAs("temp_output/phiv_contributions.png");
  c->SaveAs("temp_output/phiv_contributions.root");

  // gApplication->Terminate();
}


void SetStyle(Bool_t graypalette) {
  
  gStyle->Reset("Plain");
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  if(graypalette) gStyle->SetPalette(8,0);
  else gStyle->SetPalette(1);
  gStyle->SetCanvasColor(10);
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetFrameLineWidth(1);
  gStyle->SetFrameFillColor(kWhite);
  gStyle->SetPadColor(10);
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  gStyle->SetPadBottomMargin(0.15);
  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetHistLineWidth(1);
  gStyle->SetHistLineColor(kRed);
  gStyle->SetFuncWidth(2);
  gStyle->SetFuncColor(kGreen);
  gStyle->SetLineWidth(2);
  gStyle->SetLabelSize(0.045,"xyz");
  gStyle->SetLabelOffset(0.01,"y");
  gStyle->SetLabelOffset(0.01,"x");
  gStyle->SetLabelColor(kBlack,"xyz");
  gStyle->SetTitleSize(0.05,"xyz");
  gStyle->SetTitleOffset(1.4,"y");
  gStyle->SetTitleOffset(1.2,"x");
  gStyle->SetTitleFillColor(kWhite);
  gStyle->SetTextSizePixels(26);
  gStyle->SetTextFont(42);
  //  gStyle->SetTickLength(0.04,"X");  gStyle->SetTickLength(0.04,"Y"); 

  gStyle->SetLegendBorderSize(0);
  gStyle->SetLegendFillColor(kWhite);
  //  gStyle->SetFillColor(kWhite);
  gStyle->SetLegendFont(42);

}
