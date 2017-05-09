
#include <iostream>
#include <cstdlib>

#include <TROOT.h>
#include "TApplication.h"
#include <TStyle.h>
#include <TString.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TStopwatch.h>
#include <TMath.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLatex.h>



void PlotMass() {
  // File containing the input pairtree (test) data:
  TString fileName_testData = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/analysis_singleConvTrackRejMVAcuts/applicationPhase1/FT2_AnalysisResults_Upgrade_addFeat_pairtree_us_part2_1-9-split.root";

  // File containing the corresponding MVA output values:
  TString fileName_MVAoutput = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/analysis_singleConvTrackRejMVAcuts/applicationPhase1/TMVApp_CombConvRejMVAcuts_part2_1-9-split.root";  

  TString h_text = "Comb. conv. rej. via MVA cuts (cut at -0.1)";

  // set the used MVA method:
  const Bool_t isMLP = kFALSE;
  const Bool_t isBDT = kTRUE;


  // Output ROOT file name containing all created histograms:
  TString outfileName = "temp_output/mass_histos.root";
  
  if(isMLP & isBDT) {
    std::cout << "  ERROR: Cannot use both MLP and BDT output." << std::endl;
    exit(1);
  }
  

  // set MVA cut value:
  float MVAcut = .1;
  
  const float stepSize = 1;
  const int nSteps = 1; // NB: 1/(nSteps)==stepSize must apply

  
  TFile *f = new TFile(fileName_testData,"READ");
  TTree *TestTree = (TTree*)f->Get("pairTree_us");
  Float_t mass;
  Int_t IsRP, IsConv, IsHF, motherPdg1, motherPdg2;
  TestTree->SetBranchAddress("IsRP",&IsRP);
  TestTree->SetBranchAddress("IsConv",&IsConv);
  TestTree->SetBranchAddress("IsHF",&IsHF);
  TestTree->SetBranchAddress("motherPdg1",&motherPdg1);
  TestTree->SetBranchAddress("motherPdg2",&motherPdg2);
  TestTree->SetBranchAddress("mass",&mass);

  
  // input MVA output information from file:
  TFile *f_MVAoutput = new TFile(fileName_MVAoutput,"READ");
  TTree *MVAoutputTree = (TTree*)f_MVAoutput->Get("pairTree_MVAoutput");
  Float_t MVAoutput;
  if(isMLP) MVAoutputTree->SetBranchAddress("MLP", &MVAoutput);
  if(isBDT) MVAoutputTree->SetBranchAddress("BDT", &MVAoutput);
  
  
  const unsigned int min=0, max=5, nBins=50;
  
  
  TH1D *h_SB = new TH1D("h_SB","",nBins,min,max);
  
  TH1D *h_S = new TH1D("h_S","",nBins,min,max);
  
  TH1D *h_CombiWithConvLeg = new TH1D("h_CombiWithConvLeg","",nBins,min,max);
  
  TH1D *h_CombiWithoutConvLeg =
    new TH1D("h_CombiWithoutConvLeg","",nBins,min,max);


  
  TH1D *h_HF = new TH1D("h_HF","",nBins,min,max);
  
  TH1D *h_RPConv = new TH1D("h_RPConv","",nBins,min,max);
  
  TH1D *h_SB_MVAcut = new TH1D("h_SB_MVAcut","",nBins,min,max);
  
  TH1D *h_S_MVAcut = new TH1D("h_S_MVAcut","",nBins,min,max);
  
  TH1D *h_CombiWithConvLeg_MVAcut =
    new TH1D("h_CombiWithConvLeg_MVAcut","",nBins,min,max);
  
  TH1D *h_CombiWithoutConvLeg_MVAcut =
    new TH1D("h_CombiWithoutConvLeg_MVAcut","",nBins,min,max);
  
  TH1D *h_HF_MVAcut = new TH1D("h_HF_MVAcut","",nBins,min,max);
  
  TH1D *h_RPConv_MVAcut = new TH1D("h_RPConv_MVAcut","",nBins,min,max);


  
  TH1D *h_S_currentMVAcut = new TH1D("h_S_currentMVAcut","",nBins,min,max);
  
  TH1D *h_SB_currentMVAcut = new TH1D("h_SB_currentMVAcut","",nBins,min,max);
  
  TH1D *h_CombiWithConvLeg_currentMVAcut =
    new TH1D("h_CombiWithConvLeg_currentMVAcut","",nBins,min,max);
  
  TH1D *h_CombiWithoutConvLeg_currentMVAcut =
    new TH1D("h_CombiWithoutConvLeg_currentMVAcut","",nBins,min,max);
  
  TH1D *h_HF_currentMVAcut = new TH1D("h_HF_currentMVAcut","",nBins,min,max);
  
  TH1D *h_RPConv_currentMVAcut =
    new TH1D("h_RPConv_currentMVAcut","",nBins,min,max);


  TH1D *h_signalOverBackground =
    new TH1D("h_signalOverBackground","",nBins,min,max);

  TH1D *h_significance =
    new TH1D("h_significance","",nBins,min,max);
  

  
  
  TH2D *h_significance_MVAcutScan =
    new TH2D("h_significance_MVAcutScan","",nBins,min,max,nSteps,0,1);
  
  TH2D *h_signalOverBackground_MVAcutScan =
    new TH2D("h_signalOverBackground_MVAcutScan","",nBins,min,max,nSteps,0,1);
  
  float binContents_significance_MVAcutScan[nBins][nSteps];

  float binContents_signalOverBackground_MVAcutScan[nBins][nSteps];

  for(unsigned int i=0; i<nSteps; i++) {
    for(unsigned int j=0; j<nBins; j++) {
      binContents_significance_MVAcutScan[j][i] = 0.;
      binContents_signalOverBackground_MVAcutScan[j][i] = 0.;
    }
  }

  
  if(TestTree->GetEntries() != MVAoutputTree->GetEntries()) {
    std::cout << "   ERROR: The trees of the input files have different sizes."
	      << std::endl;
    std::cout << "   Size of tree in file " << fileName_testData << ": "
	      << TestTree->GetEntries() << std::endl;
    std::cout << "   Size of tree in file " << fileName_MVAoutput << ": "
	      << MVAoutputTree->GetEntries() << std::endl;
    return;
  }
  
  
  Long64_t nEv = TestTree->GetEntries();

  Float_t passed_seconds_prev = 0.;
  
  TStopwatch *watch = new TStopwatch();

  watch->Start();

  
  // linear mapping of the MVA cut value: [-1,1] -> [0,1]:
  if(isBDT) MVAcut = (MVAcut+1)/2.;

  
  for(Int_t i=1; i<=nSteps; i++) {
    std::cout << std::endl;
    
    for(Long64_t ev=0; ev<nEv; ev++) {
      if((ev%10000)==0) std::cout << "\rRun " << i << " of " << nSteps
				  << ":  Processing entry " << ev << " of "
				  << nEv << " (" << ev*100/nEv << "%)...";
      
      TestTree->GetEvent(ev);
      MVAoutputTree->GetEvent(ev);

      // linear mapping of the BDT values: [-1,1] -> [0,1]:
      if(isBDT) MVAoutput = (MVAoutput+1)/2.;

      
      // Skip irrelevant events (tagged with MVAoutputs greater or smaller than
      // 0 or 1 (after BDT output transformation)):
      if(MVAoutput < 0 || MVAoutput > 1) continue;

      
      if(MVAoutput >= stepSize*i) {
	h_SB_currentMVAcut->Fill(mass);
	if(IsRP==1 && IsConv==0) {
	  h_S_currentMVAcut->Fill(mass);
	}
	if(IsRP==0 && (motherPdg1==22 || motherPdg2==22)) {
	  h_CombiWithConvLeg_currentMVAcut->Fill(mass);
	}
	if(IsRP==0 && !(motherPdg1==22 || motherPdg2==22)) {
	  h_CombiWithoutConvLeg_currentMVAcut->Fill(mass);
	}
	if(IsRP==0 && IsHF==1) {
	  h_HF_currentMVAcut->Fill(mass);
	}
	if(IsRP==1 && IsConv==1) {
	  h_RPConv_currentMVAcut->Fill(mass);
	}
      }
      
      // create plots for a custom MVA cut value
      // (but not for each step in the scan):
      if(stepSize*(i-1)<MVAcut && MVAcut<=stepSize*i) {
	h_SB->Fill(mass);
	if(IsRP==1 && IsConv==0) {
	  h_S->Fill(mass);
	}
	if(IsRP==0 && (motherPdg1==22 || motherPdg2==22)) {
	  h_CombiWithConvLeg->Fill(mass);
	}
	if(IsRP==0 && !(motherPdg1==22 || motherPdg2==22)) {
	  h_CombiWithoutConvLeg->Fill(mass);
	}
	if(IsRP==0 && IsHF==1) {
	  h_HF->Fill(mass);
	}
	if(IsRP==1 && IsConv==1) {
	  h_RPConv->Fill(mass);
	}
	

	if(MVAoutput >= MVAcut) {
	  h_SB_MVAcut->Fill(mass);
	  if(IsRP==1 && IsConv==0) {
	    h_S_MVAcut->Fill(mass);
	  }
	  if(IsRP==0 && (motherPdg1==22 || motherPdg2==22)) {
	    h_CombiWithConvLeg_MVAcut->Fill(mass);
	  }
	  if(IsRP==0 && !(motherPdg1==22 || motherPdg2==22)) {
	    h_CombiWithoutConvLeg_MVAcut->Fill(mass);
	  }
	  if(IsRP==0 && IsHF==1) {
	    h_HF_MVAcut->Fill(mass);
	  }
	  if(IsRP==1 && IsConv==1) {
	    h_RPConv_MVAcut->Fill(mass);
	  }
	}
	
      }
      
    } // end ev loop

    
    
    // generating the 2D/3D histograms for the MVA cut scan:
    for(unsigned int j=2; j<=nBins; j++) { //j>=2 cuts on the first bin (mass<=.1)
      
      Int_t bin_S = h_S_currentMVAcut->GetBin(j),
	bin_SB = h_SB_currentMVAcut->GetBin(j),
	bin_CombiWithConvLeg = h_CombiWithConvLeg_currentMVAcut->GetBin(j),
	bin_CombiWithoutConvLeg = h_CombiWithoutConvLeg_currentMVAcut->GetBin(j),
	bin_HF = h_HF_currentMVAcut->GetBin(j),
	bin_RPConv = h_RPConv_currentMVAcut->GetBin(j),
	bin_MVAcutScan = h_significance_MVAcutScan->GetBin(j,i);
      
      // define your "signal"/"background" here:
      float S = h_S_currentMVAcut->GetBinContent(bin_S) + 
	h_HF_currentMVAcut->GetBinContent(bin_HF) + 
        h_RPConv_currentMVAcut->GetBinContent(bin_RPConv) +
	h_CombiWithoutConvLeg_currentMVAcut->GetBinContent(bin_CombiWithoutConvLeg);
      
      float B =
	h_CombiWithConvLeg_currentMVAcut->GetBinContent(bin_CombiWithConvLeg);// + 
      //h_CombiWithoutConvLeg_currentMVAcut->GetBinContent(bin_CombiWithoutConvLeg);
      
      float significance = (B==0) ? 0 : S/TMath::Sqrt(S+B);
      float signalOverBackground = (B==0) ? 0 : S/B;
      
      binContents_significance_MVAcutScan[j-1][i-1] =
	significance;
      
      binContents_signalOverBackground_MVAcutScan[j-1][i-1] =
	signalOverBackground;

      
      if(stepSize*(i-1)<MVAcut && MVAcut<=stepSize*i) {
	h_signalOverBackground->SetBinContent(j, signalOverBackground);
	h_significance->SetBinContent(j, significance);
      }
    }
    
    h_S_currentMVAcut->Reset();
    h_SB_currentMVAcut->Reset();

    
    std::cout << "\rRun " << i << " of " << nSteps << ":  Processing entry "
	      << nEv << " of " << nEv << " (100%)... DONE.";
    Float_t passed_seconds = watch->RealTime();
    std::cout << "  (Time elapsed: " << passed_seconds-passed_seconds_prev << " seconds)";
    passed_seconds_prev = passed_seconds;
    watch->Continue();
    
  } // end i loop

  
  // f->Close();
  // f_MVAoutput->Close();


  
  std::cout << std::endl << std::endl;
  std::cout << "Time elapsed since begin of processing: " << std::endl;
  std::cout << "\t";
  watch->Print();
  std::cout << std::endl;
  watch->Stop();

  
  // normalize bins to the "no MVA cuts" case:
  for(unsigned int i=0; i<nSteps; i++) {
    for(unsigned int j=0; j<nBins; j++) {
      
      Float_t significance_norm = (binContents_significance_MVAcutScan[j][0] == 0.) ?
   	0. : binContents_significance_MVAcutScan[j][i] / binContents_significance_MVAcutScan[j][0];
     
      Float_t signalOverBackground_norm = (binContents_signalOverBackground_MVAcutScan[j][0] == 0.) ?
  	0. : binContents_signalOverBackground_MVAcutScan[j][i] / binContents_signalOverBackground_MVAcutScan[j][0];
      
      h_significance_MVAcutScan->SetBinContent(h_significance_MVAcutScan->GetBin(j+1,i+1),
 					       significance_norm);
      h_signalOverBackground_MVAcutScan->SetBinContent(h_signalOverBackground_MVAcutScan->GetBin(j+1,i+1),
  						       signalOverBackground_norm);
     
    }
  }

  
  float AUCs_significance[nSteps],
    AUCs_signalOverBackground[nSteps];
  
  float AUC_significance_max = -1,
    AUC_significance_max_pos;
  
  float AUC_signalOverBackground_max = -1,
    AUC_signalOverBackground_max_pos;
  
  for(unsigned int k=0; k<nSteps; k++) {
    AUCs_significance[k] = 0.;
    AUCs_signalOverBackground[k] = 0.;
    
    for(unsigned int m=0; m<nBins; m++) {
      AUCs_significance[k] +=
	binContents_significance_MVAcutScan[m][k];
      
      AUCs_signalOverBackground[k] +=
	binContents_signalOverBackground_MVAcutScan[m][k];
    }
    
    if(AUCs_significance[k] > AUC_significance_max) {
      AUC_significance_max = AUCs_significance[k];
      AUC_significance_max_pos = k;
    }
    if(AUCs_signalOverBackground[k] > AUC_signalOverBackground_max) {
      AUC_signalOverBackground_max = AUCs_signalOverBackground[k];
      AUC_signalOverBackground_max_pos = k;
    }
  }
  
  std::cout << std::endl;
  std::cout << "Maximum significance AUC = " << AUC_significance_max
	    << " for an MVA cut value of "
	    << stepSize*(AUC_significance_max_pos+1) << std::endl;
  std::cout << "Maximum signal over background AUC = "
	    << AUC_signalOverBackground_max << " for an MVA cut value of "
	    << stepSize*(AUC_signalOverBackground_max_pos+1)
	    << std::endl << std::endl;
  


  
  //////////////////// SETTING UP THE PLOTS:  

  
  gStyle->SetOptStat(0);
  
  TCanvas *c_significance_scan = new TCanvas("c_significance_scan","",1024,768);
  c_significance_scan->SetGridy();
  // c_significance_scan->SetLogz();
  h_significance_MVAcutScan->SetXTitle("M_{ee} / (GeV/c^{2})");
  h_significance_MVAcutScan->SetYTitle("MVA cut");
  h_significance_MVAcutScan->SetZTitle("(S/#sqrt{S+B} after MVA cut)/(S/#sqrt{S+B} before MVA cut)");
  gStyle->SetPalette(54);
  // gStyle->SetPaintTextFormat("A %.2f");
  h_significance_MVAcutScan->Draw("colz");
  TLatex l_AUC_significance_max;
  l_AUC_significance_max.SetTextSize(.025);
  TString text_AUC_significance_max = "max(S/#sqrt{S+B}) for MVA cut at ";
  text_AUC_significance_max += stepSize*(AUC_significance_max_pos);
  l_AUC_significance_max.DrawLatex(.05,.95,text_AUC_significance_max);
  c_significance_scan->SaveAs("temp_output/mass_significance_MVAscan.pdf");
  c_significance_scan->SaveAs("temp_output/mass_significance_MVAscan.root");
  c_significance_scan->SaveAs("temp_output/mass_significance_MVAscan.png");
  TCanvas *c_significance_scan_3D =
    new TCanvas("c_significance_scan_3D","",1024,768);
  // c_significance_scan_3D->SetLogz();
  h_significance_MVAcutScan->Draw("lego2");
  h_significance_MVAcutScan->SaveAs("temp_output/mass_significance_MVAscan_3D.root");

  TCanvas *c_signalOverBackground_scan =
    new TCanvas("c_signalOverBackground_scan","",1024,768);
  c_signalOverBackground_scan->SetGridy();
  c_signalOverBackground_scan->SetLogz();
  // h_signalOverBackground_MVAcutScan->SetMinimum(1e-6);
  h_signalOverBackground_MVAcutScan->SetXTitle("M_{ee} / (GeV/c^{2})");
  h_signalOverBackground_MVAcutScan->SetYTitle("MVA cut");
  h_signalOverBackground_MVAcutScan->SetZTitle("(S/B after MVA cut)/(S/B before MVA cut)");
  gStyle->SetPalette(54);
  // gStyle->SetPaintTextFormat("A %.2f");
  h_signalOverBackground_MVAcutScan->Draw("colz");
  TLatex l_AUC_signalOverBackground_max;
  l_AUC_signalOverBackground_max.SetTextSize(.025);
  TString text_AUC_signalOverBackground_max = "max(S/B) for MVA cut at ";
  text_AUC_signalOverBackground_max +=
    stepSize*(AUC_signalOverBackground_max_pos);
  l_AUC_signalOverBackground_max.DrawLatex(.05,.95,
					   text_AUC_signalOverBackground_max);
  c_signalOverBackground_scan->SaveAs("temp_output/mass_signalOverBackground_MVAscan.pdf");
  c_signalOverBackground_scan->SaveAs("temp_output/mass_signalOverBackground_MVAscan.root");
  c_signalOverBackground_scan->SaveAs("temp_output/mass_signalOverBackground_MVAscan.png");
  TCanvas *c_signalOverBackground_scan_3D =
    new TCanvas("c_signalOverBackground_scan_3D","",1024,768);
  c_signalOverBackground_scan_3D->SetLogz();
  h_signalOverBackground_MVAcutScan->Draw("lego2");
  h_signalOverBackground_MVAcutScan->SaveAs("temp_output/mass_signalOverBackground_MVAscan_3D.root");

  
  TH1D *h_SB_eff = (TH1D*)h_SB_MVAcut->Clone("h_SB_eff");
  TH1D *h_S_eff = (TH1D*)h_S_MVAcut->Clone("h_S_eff");
  TH1D *h_CombiWithConvLeg_eff = (TH1D*)h_CombiWithConvLeg_MVAcut->Clone("h_CombiWithConvLeg_eff");
  TH1D *h_CombiWithoutConvLeg_eff = (TH1D*)h_CombiWithoutConvLeg_MVAcut->Clone("h_CombiWithoutConvLeg_eff");
  TH1D *h_HF_eff = (TH1D*)h_HF_MVAcut->Clone("h_HF_eff");
  TH1D *h_RPConv_eff = (TH1D*)h_RPConv_MVAcut->Clone("h_RPConv_eff");
  h_SB_eff->Sumw2();
  h_SB->Sumw2();
  h_SB_eff->Divide(h_SB);
  h_S_eff->Sumw2();
  h_S->Sumw2();
  h_S_eff->Divide(h_S);
  h_CombiWithConvLeg_eff->Sumw2();
  h_CombiWithConvLeg->Sumw2();
  h_CombiWithConvLeg_eff->Divide(h_CombiWithConvLeg);
  h_CombiWithoutConvLeg_eff->Sumw2();
  h_CombiWithoutConvLeg->Sumw2();
  h_CombiWithoutConvLeg_eff->Divide(h_CombiWithoutConvLeg);
  h_HF_eff->Sumw2();
  h_HF->Sumw2();
  h_HF_eff->Divide(h_HF);
  h_RPConv_eff->Sumw2();
  h_RPConv->Sumw2();
  h_RPConv_eff->Divide(h_RPConv);



  
  h_SB->SetLineColor(kBlack);
  h_S->SetLineColor(kGreen+1);
  h_CombiWithConvLeg->SetLineColor(kRed);
  h_CombiWithoutConvLeg->SetLineColor(kBlue);
  h_HF->SetLineColor(kOrange);
  h_RPConv->SetLineColor(kMagenta);

  h_SB_MVAcut->SetLineColor(kBlack);
  h_SB_MVAcut->SetMarkerColor(kBlack);
  h_S_MVAcut->SetLineColor(kGreen+1);
  h_S_MVAcut->SetMarkerColor(kGreen+1);
  h_CombiWithConvLeg_MVAcut->SetLineColor(kRed);
  h_CombiWithConvLeg_MVAcut->SetMarkerColor(kRed);
  h_CombiWithoutConvLeg_MVAcut->SetLineColor(kBlue);
  h_CombiWithoutConvLeg_MVAcut->SetMarkerColor(kBlue);
  h_HF_MVAcut->SetLineColor(kOrange);
  h_HF_MVAcut->SetMarkerColor(kOrange);
  h_RPConv_MVAcut->SetLineColor(kMagenta);
  h_RPConv_MVAcut->SetMarkerColor(kMagenta);
  
  h_SB_MVAcut->SetMarkerStyle(7);
  h_S_MVAcut->SetMarkerStyle(7);
  h_CombiWithConvLeg_MVAcut->SetMarkerStyle(7);
  h_CombiWithoutConvLeg_MVAcut->SetMarkerStyle(7);
  h_HF_MVAcut->SetMarkerStyle(7);
  h_RPConv_MVAcut->SetMarkerStyle(7);

  h_SB->GetYaxis()->SetRangeUser(1e-1,1e8);
  h_SB->SetXTitle("M_{ee} / (GeV/c^{2})");
  h_SB->SetYTitle("Entries");
  h_SB->GetXaxis()->SetTitleOffset(1.2);
  h_SB->GetYaxis()->SetTitleOffset(1.3);

  h_SB_eff->SetLineColor(kBlack);
  h_S_eff->SetLineColor(kGreen+1);
  h_CombiWithConvLeg_eff->SetLineColor(kRed);
  h_CombiWithoutConvLeg_eff->SetLineColor(kBlue);
  h_HF_eff->SetLineColor(kOrange);
  h_RPConv_eff->SetLineColor(kMagenta);

  h_SB_eff->SetMarkerStyle(7);
  h_S_eff->SetMarkerStyle(7);
  h_CombiWithConvLeg_eff->SetMarkerStyle(7);
  h_CombiWithoutConvLeg_eff->SetMarkerStyle(7);
  h_HF_eff->SetMarkerStyle(7);
  h_RPConv_eff->SetMarkerStyle(7);
  
  h_SB_eff->SetMarkerColor(kBlack);
  h_S_eff->SetMarkerColor(kGreen+1);
  h_CombiWithConvLeg_eff->SetMarkerColor(kRed);
  h_CombiWithoutConvLeg_eff->SetMarkerColor(kBlue);
  h_HF_eff->SetMarkerColor(kOrange);
  h_RPConv_eff->SetMarkerColor(kMagenta);

  h_signalOverBackground->SetMarkerStyle(7);
  h_significance->SetMarkerStyle(7);

  

  TCanvas *c = new TCanvas("c","",1024,768);
  c->SetLogy();
  c->SetGridy();
  h_SB->Draw("hist e x0");
  h_S->Draw("hist e x0 same");
  h_CombiWithConvLeg->Draw("hist e x0 same");
  h_CombiWithoutConvLeg->Draw("hist e x0 same");
  h_HF->Draw("hist e x0 same");
  h_RPConv->Draw("hist e x0 same");

  h_SB_MVAcut->Draw("e1 x0 same");
  h_S_MVAcut->Draw("e1 x0 same");
  h_CombiWithConvLeg_MVAcut->Draw("e1 x0 same");
  h_CombiWithoutConvLeg_MVAcut->Draw("e1 x0 same");
  h_HF_MVAcut->Draw("e1 x0 same");
  h_RPConv_MVAcut->Draw("e1 x0 same");

  
  TLegend *leg = new TLegend(.6,.8,.95,.95);
  leg->AddEntry(h_SB,"S+B","l");
  leg->AddEntry(h_S,"S","l");
  leg->AddEntry(h_CombiWithConvLeg,"Comb. w. conv. leg","l");
  leg->AddEntry(h_CombiWithoutConvLeg,"Comb. w/o conv. leg","l");
  leg->AddEntry(h_HF,"Corr. HF","l");
  leg->AddEntry(h_RPConv,"RP conv.","l");
  leg->Draw();

  TLatex l;
  l.SetTextSize(.025);
  l.DrawLatex(.1,1e7+5e6,h_text);

  c->SaveAs("temp_output/mass.pdf");
  c->SaveAs("temp_output/mass.root");
  c->SaveAs("temp_output/mass.png");

  TCanvas *c_eff = new TCanvas("c_eff","",1024,768);
  c_eff->SetGridy();
  h_SB_eff->SetXTitle("M_{ee} / (GeV/c^{2})");
  h_SB_eff->SetYTitle("Efficiency");
  h_SB_eff->GetXaxis()->SetTitleOffset(1.2);
  h_SB_eff->GetYaxis()->SetTitleOffset(1.3);
  h_SB_eff->GetYaxis()->SetRangeUser(0,1.1);
  h_SB_eff->Draw("e1 x0");
  h_HF_eff->Draw("e1 x0 same");
  h_S_eff->Draw("e1 x0 same");
  h_CombiWithConvLeg_eff->Draw("e1 x0 same");
  h_CombiWithoutConvLeg_eff->Draw("e1 x0 same");
  h_RPConv_eff->Draw("e1 x0 same");
  leg->Draw();
  l.DrawLatex(.1,1.125,h_text);

  c_eff->SaveAs("temp_output/mass_eff.pdf");
  c_eff->SaveAs("temp_output/mass_eff.root");
  c_eff->SaveAs("temp_output/mass_eff.png");


  TCanvas *c_signalOverBackground =
    new TCanvas("c_signalOverBackground","",1024,768);
  c_signalOverBackground->SetLogy();
  c_signalOverBackground->SetGridy();
  h_signalOverBackground->SetXTitle("M_{ee} / (GeV/c^{2})");
  h_signalOverBackground->SetYTitle("Signal over background");
  h_signalOverBackground->GetXaxis()->SetTitleOffset(1.2);
  h_signalOverBackground->GetYaxis()->SetTitleOffset(1.3);
  h_signalOverBackground->GetYaxis()->SetRangeUser(1e-6,1.1);
  h_signalOverBackground->Draw("e1 x0");

  c_signalOverBackground->SaveAs("temp_output/mass_signalOverBackground.pdf");
  c_signalOverBackground->SaveAs("temp_output/mass_signalOverBackground.root");
  c_signalOverBackground->SaveAs("temp_output/mass_signalOverBackground.png");


  TCanvas *c_significance =
    new TCanvas("c_significance","",1024,768);
  // c_significance->SetLogy();
  c_significance->SetGridy();
  h_significance->SetXTitle("M_{ee} / (GeV/c^{2})");
  h_significance->SetYTitle("Significance");
  h_significance->GetXaxis()->SetTitleOffset(1.2);
  h_significance->GetYaxis()->SetTitleOffset(1.3);
  // h_significance->GetYaxis()->SetRangeUser(1e-6,1.1);
  h_significance->Draw("e1 x0");

  c_significance->SaveAs("temp_output/mass_significance.pdf");
  c_significance->SaveAs("temp_output/mass_significance.root");
  c_significance->SaveAs("temp_output/mass_significance.png");


  
  // store all created histograms in a ROOT file:
  TFile *outfile = new TFile(outfileName, "RECREATE");

  h_SB->Write(0, TObject::kOverwrite);
  h_S->Write(0, TObject::kOverwrite);
  h_CombiWithConvLeg->Write(0, TObject::kOverwrite);
  h_CombiWithoutConvLeg->Write(0, TObject::kOverwrite);
  h_HF->Write(0, TObject::kOverwrite);
  h_RPConv->Write(0, TObject::kOverwrite);

  h_SB_MVAcut->Write(0, TObject::kOverwrite);
  h_S_MVAcut->Write(0, TObject::kOverwrite);
  h_CombiWithConvLeg_MVAcut->Write(0, TObject::kOverwrite);
  h_CombiWithoutConvLeg_MVAcut->Write(0, TObject::kOverwrite);
  h_HF_MVAcut->Write(0, TObject::kOverwrite);
  h_RPConv_MVAcut->Write(0, TObject::kOverwrite);

  h_SB_eff->Write(0, TObject::kOverwrite);
  h_S_eff->Write(0, TObject::kOverwrite);
  h_CombiWithConvLeg_eff->Write(0, TObject::kOverwrite);
  h_CombiWithoutConvLeg_eff->Write(0, TObject::kOverwrite);
  h_HF_eff->Write(0, TObject::kOverwrite);
  h_RPConv_eff->Write(0, TObject::kOverwrite);
  
  outfile->Close();



  
  gApplication->Terminate();
}


// int main() {
//   PlotMass();
//   return 0;
// }
