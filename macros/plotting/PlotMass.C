
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
#include <TEfficiency.h>
#include <TStopwatch.h>
#include <TMath.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLatex.h>


Float_t getPairPIDefficiency(Float_t, Float_t, TH1D&);


void PlotMass() {
  // File containing the input pairtree (test) data:
  TString fileName_testData = "~/analysis/data/FT2_AnalysisResults_Upgrade/workingData/FT2_AnalysisResults_Upgrade_DCAvec_PIDeffs_pairtree_us_part2_1-9-split_correctedPIDeffs.root";

  // File containing the corresponding MVA output values:
  TString fileName_MVAoutput = "~/Downloads/CombConvRej_NN_testing/temp_output/ann/predictions_NN.root"; //"~/analysis/data/FT2_AnalysisResults_Upgrade/sklearn_BDT_analysis/randomForest/CombConvRej_woPIDeffs_noMassCuts/clf_predict/predictions_BDT.root";

  TString h_text = "Comb. conv. rej. via MVA cuts";

  // set the used MVA method:
  const Bool_t isNN  = kTRUE;
  const Bool_t isBDT = kFALSE;

  // MVA output range in the corresponding input file:
  const Float_t MVAoutputRange_min = 0.;
  const Float_t MVAoutputRange_max = 1.;

  const Bool_t doConsiderPIDefficiencies = kTRUE;

  // File containing the pt-dependent PID efficiencies:
  TString infile_PIDefficiencies_name = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/inputData/ITSU_PIDefficiency_lowB.root";
  TFile *infile_PIDefficiencies = new TFile(infile_PIDefficiencies_name, "READ");
  TH1D *h_PIDeff = (TH1D*)infile_PIDefficiencies->Get("efficiencyLHC17d12_TPCandTOF3sigma");
  h_PIDeff->GetXaxis()->SetRangeUser(0,5);

  // Output ROOT file name containing all created histograms:
  TString outfileName = "temp_output/mass_histos.root";
  
  if(isNN & isBDT) {
    std::cout << "  ERROR: Cannot use both MLP and BDT output." << std::endl;
    exit(1);
  }

  if(MVAoutputRange_min >= MVAoutputRange_max) {
    std::cout << "  ERROR: Ill-defined MVA output range (min>=max)." << std::endl;
    exit(1);
  }
  

  // set MVA cut value:
  float MVAcut = .21;
  
  const float stepSize = 1;
  const int nSteps = 1; // NB: 1/(nSteps)==stepSize must apply


  
  
  TFile *f = new TFile(fileName_testData,"READ");
  TTree *TestTree = (TTree*)f->Get("pairTree_us");
  Float_t mass;
  Float_t pt1, pt2;
  Int_t IsRP, IsConv, IsHF, motherPdg1, motherPdg2;
  TestTree->SetBranchAddress("IsRP",&IsRP);
  TestTree->SetBranchAddress("IsConv",&IsConv);
  TestTree->SetBranchAddress("IsHF",&IsHF);
  TestTree->SetBranchAddress("motherPdg1",&motherPdg1);
  TestTree->SetBranchAddress("motherPdg2",&motherPdg2);
  TestTree->SetBranchAddress("mass",&mass);
  TestTree->SetBranchAddress("pt1",&pt1);
  TestTree->SetBranchAddress("pt2",&pt2);

  
  // input MVA output information from file:
  TFile *f_MVAoutput = new TFile(fileName_MVAoutput,"READ");
  TTree *MVAoutputTree = (TTree*)f_MVAoutput->Get("pairTree_MVAoutput");
  Float_t MVAoutput;
  if(isNN) MVAoutputTree->SetBranchAddress("NN", &MVAoutput);
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

  TH1D *h_sample_weight = new TH1D("h_sample_weight","",1000,0,1);

  
  if(TestTree->GetEntries() != MVAoutputTree->GetEntries()) {
    std::cout << "   ERROR: The trees of the input files have different sizes."
	      << std::endl;
    std::cout << "   Size of tree in file " << fileName_testData << ": "
	      << TestTree->GetEntries() << std::endl;
    std::cout << "   Size of tree in file " << fileName_MVAoutput << ": "
	      << MVAoutputTree->GetEntries() << std::endl;
    // return;
  }
  
  
  Long64_t nEv = 100000000; //TestTree->GetEntries();
  std::cout << "Starting to process " << nEv << " entries..." << std::endl;

  Float_t passed_seconds_prev = 0.;
  
  TStopwatch *watch = new TStopwatch();

  watch->Start();

  
  // linear mapping of the MVA cut value to the range [0,1]:
  MVAcut = (MVAcut-MVAoutputRange_min)/(MVAoutputRange_max-MVAoutputRange_min);

  
  for(Int_t i=1; i<=nSteps; i++) {
    std::cout << std::endl;
    
    for(Long64_t ev=0; ev<nEv; ev++) {
      if((ev%10000)==0) std::cout << "\rRun " << i << " of " << nSteps
				  << ":  Processing entry " << ev << " of "
				  << nEv << " (" << ev*100/nEv << "%)...";
      
      TestTree->GetEvent(ev);
      MVAoutputTree->GetEvent(ev);

      // linear mapping of the MVA output values to the range [0,1]:
      MVAoutput = (MVAoutput-MVAoutputRange_min)/(MVAoutputRange_max-MVAoutputRange_min);
      
      // Skip irrelevant events (tagged with MVA outputs greater or smaller than
      // 0 or 1 (after MVA output transformation)):
      if(MVAoutput < 0 || MVAoutput > 1) continue;
      

      Float_t sample_weight = 1.;
      if(doConsiderPIDefficiencies) {
	sample_weight = getPairPIDefficiency(pt1, pt2, *h_PIDeff);
      }
      h_sample_weight->Fill(sample_weight);

      
      if(MVAoutput >= stepSize*i) {
	h_SB_currentMVAcut->Fill(mass, sample_weight);
	if(IsRP==1 && IsConv==0) {
	  h_S_currentMVAcut->Fill(mass, sample_weight);
	}
	if(IsRP==0 && (motherPdg1==22 || motherPdg2==22)) {
	  h_CombiWithConvLeg_currentMVAcut->Fill(mass, sample_weight);
	}
	if(IsRP==0 && !(motherPdg1==22 || motherPdg2==22)) {
	  h_CombiWithoutConvLeg_currentMVAcut->Fill(mass, sample_weight);
	}
	if(IsRP==0 && IsHF==1) {
	  h_HF_currentMVAcut->Fill(mass, sample_weight);
	}
	if(IsRP==1 && IsConv==1) {
	  h_RPConv_currentMVAcut->Fill(mass, sample_weight);
	}
      }
      
      // create plots for a custom MVA cut value
      // (but not for each step in the scan):
      if(stepSize*(i-1)<MVAcut && MVAcut<=stepSize*i) {
	h_SB->Fill(mass, sample_weight);
	if(IsRP==1 && IsConv==0) {
	  h_S->Fill(mass, sample_weight);
	}
	if(IsRP==0 && (motherPdg1==22 || motherPdg2==22)) {
	  h_CombiWithConvLeg->Fill(mass, sample_weight);
	}
	if(IsRP==0 && !(motherPdg1==22 || motherPdg2==22)) {
	  h_CombiWithoutConvLeg->Fill(mass, sample_weight);
	}
	if(IsRP==0 && IsHF==1) {
	  h_HF->Fill(mass, sample_weight);
	}
	if(IsRP==1 && IsConv==1) {
	  h_RPConv->Fill(mass, sample_weight);
	}
	

	if(MVAoutput >= MVAcut) {
	  h_SB_MVAcut->Fill(mass, sample_weight);
	  if(IsRP==1 && IsConv==0) {
	    h_S_MVAcut->Fill(mass, sample_weight);
	  }
	  if(IsRP==0 && (motherPdg1==22 || motherPdg2==22)) {
	    h_CombiWithConvLeg_MVAcut->Fill(mass, sample_weight);
	  }
	  if(IsRP==0 && !(motherPdg1==22 || motherPdg2==22)) {
	    h_CombiWithoutConvLeg_MVAcut->Fill(mass, sample_weight);
	  }
	  if(IsRP==0 && IsHF==1) {
	    h_HF_MVAcut->Fill(mass, sample_weight);
	  }
	  if(IsRP==1 && IsConv==1) {
	    h_RPConv_MVAcut->Fill(mass, sample_weight);
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

  TCanvas *c_sample_weight = new TCanvas("c_sample_weight","",1024,768);
  h_sample_weight->SetXTitle("Sample weight");
  h_sample_weight->SetYTitle("Entries");
  h_sample_weight->SaveAs("temp_output/mass_sampleWeight.root");
  h_sample_weight->Draw();
  c_sample_weight->SaveAs("temp_output/mass_sampleWeight.png");
  
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


  if(!TEfficiency::CheckConsistency(*h_SB_MVAcut, *h_SB) ||
     !TEfficiency::CheckConsistency(*h_S_MVAcut, *h_S) ||
     !TEfficiency::CheckConsistency(*h_CombiWithConvLeg_MVAcut, *h_CombiWithConvLeg) ||
     !TEfficiency::CheckConsistency(*h_CombiWithoutConvLeg_MVAcut, *h_CombiWithoutConvLeg) ||
     !TEfficiency::CheckConsistency(*h_HF_MVAcut, *h_HF) ||
     !TEfficiency::CheckConsistency(*h_RPConv_MVAcut, *h_RPConv)) {
    std::cout << "  ERROR: Histograms are inconsistent for TEfficiency object initialization." << std::endl;
    gApplication->Terminate();
  }

  TEfficiency *pEff_SB = new TEfficiency(*h_SB_MVAcut, *h_SB);
  TEfficiency *pEff_S = new TEfficiency(*h_S_MVAcut, *h_S);
  TEfficiency *pEff_CombiWithConvLeg = new TEfficiency(*h_CombiWithConvLeg_MVAcut, *h_CombiWithConvLeg);
  TEfficiency *pEff_CombiWithoutConvLeg = new TEfficiency(*h_CombiWithoutConvLeg_MVAcut, *h_CombiWithoutConvLeg);
  TEfficiency *pEff_HF = new TEfficiency(*h_HF_MVAcut, *h_HF);
  TEfficiency *pEff_RPConv = new TEfficiency(*h_RPConv_MVAcut, *h_RPConv);
  
  
  TH1D *h_SB_eff = new TH1D("h_SB_eff","",nBins,min,max);
  TH1D *h_S_eff = new TH1D("h_S_eff","",nBins,min,max);
  TH1D *h_CombiWithConvLeg_eff = new TH1D("h_CombiWithConvLeg_eff","",nBins,min,max);
  TH1D *h_CombiWithoutConvLeg_eff = new TH1D("h_CombiWithoutConvLeg_eff","",nBins,min,max);
  TH1D *h_HF_eff = new TH1D("h_HF_eff","",nBins,min,max);
  TH1D *h_RPConv_eff = new TH1D("h_RPConv_eff","",nBins,min,max);
  
  
  for(unsigned int i=1; i<=nBins; i++) {
    h_SB_eff->SetBinContent(i, pEff_SB->GetEfficiency(i));
    h_S_eff->SetBinContent(i, pEff_S->GetEfficiency(i));
    h_CombiWithConvLeg_eff->SetBinContent(i, pEff_CombiWithConvLeg->GetEfficiency(i));
    h_CombiWithoutConvLeg_eff->SetBinContent(i, pEff_CombiWithoutConvLeg->GetEfficiency(i));
    h_HF_eff->SetBinContent(i, pEff_HF->GetEfficiency(i));
    h_RPConv_eff->SetBinContent(i, pEff_RPConv->GetEfficiency(i));

    if(!TMath::IsNaN(pEff_SB->GetEfficiencyErrorLow(i)) && !TMath::IsNaN(pEff_SB->GetEfficiencyErrorUp(i))) {
      h_SB_eff->SetBinError(i, TMath::Max(pEff_SB->GetEfficiencyErrorLow(i), pEff_SB->GetEfficiencyErrorUp(i)));
    }else h_SB_eff->SetBinError(i, 0.);

    if(!TMath::IsNaN(pEff_S->GetEfficiencyErrorLow(i)) && !TMath::IsNaN(pEff_S->GetEfficiencyErrorUp(i))) {
      h_S_eff->SetBinError(i, TMath::Max(pEff_S->GetEfficiencyErrorLow(i), pEff_S->GetEfficiencyErrorUp(i)));
    }else h_S_eff->SetBinError(i, 0.);

    if(!TMath::IsNaN(pEff_CombiWithConvLeg->GetEfficiencyErrorLow(i)) && !TMath::IsNaN(pEff_CombiWithConvLeg->GetEfficiencyErrorUp(i))) {
      h_CombiWithConvLeg_eff->SetBinError(i, TMath::Max(pEff_CombiWithConvLeg->GetEfficiencyErrorLow(i), pEff_CombiWithConvLeg->GetEfficiencyErrorUp(i)));
    }else h_CombiWithConvLeg_eff->SetBinError(i, 0.);

    if(!TMath::IsNaN(pEff_CombiWithoutConvLeg->GetEfficiencyErrorLow(i)) && !TMath::IsNaN(pEff_CombiWithoutConvLeg->GetEfficiencyErrorUp(i))) {
      h_CombiWithoutConvLeg_eff->SetBinError(i, TMath::Max(pEff_CombiWithoutConvLeg->GetEfficiencyErrorLow(i), pEff_CombiWithoutConvLeg->GetEfficiencyErrorUp(i)));
    }else h_CombiWithoutConvLeg_eff->SetBinError(i, 0.);

    if(!TMath::IsNaN(pEff_HF->GetEfficiencyErrorLow(i)) && !TMath::IsNaN(pEff_HF->GetEfficiencyErrorUp(i))) {
      h_HF_eff->SetBinError(i, TMath::Max(pEff_HF->GetEfficiencyErrorLow(i), pEff_HF->GetEfficiencyErrorUp(i)));
    }else h_HF_eff->SetBinError(i, 0.);

    if(!TMath::IsNaN(pEff_RPConv->GetEfficiencyErrorLow(i)) && !TMath::IsNaN(pEff_RPConv->GetEfficiencyErrorUp(i))) {
      h_RPConv_eff->SetBinError(i, TMath::Max(pEff_RPConv->GetEfficiencyErrorLow(i), pEff_RPConv->GetEfficiencyErrorUp(i)));
    }else h_RPConv_eff->SetBinError(i, 0.);
  }
  
  
  
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

  h_SB_eff->SetMarkerStyle(8);
  h_S_eff->SetMarkerStyle(8);
  h_CombiWithConvLeg_eff->SetMarkerStyle(8);
  h_CombiWithoutConvLeg_eff->SetMarkerStyle(8);
  h_HF_eff->SetMarkerStyle(8);
  h_RPConv_eff->SetMarkerStyle(8);
  
  h_SB_eff->SetMarkerColor(kBlack);
  h_S_eff->SetMarkerColor(kGreen+1);
  h_CombiWithConvLeg_eff->SetMarkerColor(kRed);
  h_CombiWithoutConvLeg_eff->SetMarkerColor(kBlue);
  h_HF_eff->SetMarkerColor(kOrange);
  h_RPConv_eff->SetMarkerColor(kMagenta);

  h_SB_eff->SetMarkerSize(1.3);
  h_S_eff->SetMarkerSize(1.3);
  h_CombiWithConvLeg_eff->SetMarkerSize(1.3);
  h_CombiWithoutConvLeg_eff->SetMarkerSize(1.3);
  h_HF_eff->SetMarkerSize(1.3);
  h_RPConv_eff->SetMarkerSize(1.3);
  

  h_signalOverBackground->SetMarkerStyle(7);
  h_significance->SetMarkerStyle(7);


  TString drawOptions_mass_hist = "hist e x0";
  TString drawOptions_mass_hist_same = drawOptions_mass_hist + " same";

  TString drawOptions_mass_marker = "same";
  TString drawOptions_mass_marker_same = drawOptions_mass_marker + " same";

  TCanvas *c = new TCanvas("c","",1024,768);
  c->SetLogy();
  c->SetGridy();
  h_SB->Draw(drawOptions_mass_hist);
  h_S->Draw(drawOptions_mass_hist_same);
  h_CombiWithConvLeg->Draw(drawOptions_mass_hist_same);
  h_CombiWithoutConvLeg->Draw(drawOptions_mass_hist_same);
  h_HF->Draw(drawOptions_mass_hist_same);
  h_RPConv->Draw(drawOptions_mass_hist_same);

  h_SB_MVAcut->Draw(drawOptions_mass_marker_same);
  h_S_MVAcut->Draw(drawOptions_mass_marker_same);
  h_CombiWithConvLeg_MVAcut->Draw(drawOptions_mass_marker_same);
  h_CombiWithoutConvLeg_MVAcut->Draw(drawOptions_mass_marker_same);
  h_HF_MVAcut->Draw(drawOptions_mass_marker_same);
  h_RPConv_MVAcut->Draw(drawOptions_mass_marker_same);

  
  TLegend *leg_mass = new TLegend(.6,.7,.88,.88);
  leg_mass->AddEntry(h_SB_eff,"S+B","p");
  leg_mass->AddEntry(h_S_eff,"S","p");
  leg_mass->AddEntry(h_CombiWithConvLeg_eff,"Comb. w. conv. leg","p");
  leg_mass->AddEntry(h_CombiWithoutConvLeg_eff,"Comb. w/o conv. leg","p");
  leg_mass->AddEntry(h_HF_eff,"Corr. HF","p");
  leg_mass->AddEntry(h_RPConv_eff,"RP conv.","p");
  leg_mass->Draw();

  TLatex l;
  l.SetTextSize(.025);
  l.DrawLatex(.1,1e7+5e6,h_text);

  c->SaveAs("temp_output/mass.pdf");
  c->SaveAs("temp_output/mass.root");
  c->SaveAs("temp_output/mass.png");


  TString drawOptions_eff = "hist p x0 e1";
  TString drawOptions_eff_same = drawOptions_eff + " same";


  TLegend *leg_eff = new TLegend(.6,.15,.88,.33);
  leg_eff->AddEntry(h_SB_eff,"S+B","p");
  leg_eff->AddEntry(h_S_eff,"S","p");
  leg_eff->AddEntry(h_CombiWithConvLeg_eff,"Comb. w. conv. leg","p");
  leg_eff->AddEntry(h_CombiWithoutConvLeg_eff,"Comb. w/o conv. leg","p");
  leg_eff->AddEntry(h_HF_eff,"Corr. HF","p");
  leg_eff->AddEntry(h_RPConv_eff,"RP conv.","p");
  leg_eff->Draw();
  
  TCanvas *c_eff = new TCanvas("c_eff","",1024,768);
  c_eff->SetGridy();
  h_SB_eff->SetXTitle("M_{ee} / (GeV/c^{2})");
  h_SB_eff->SetYTitle("Efficiency");
  h_SB_eff->GetXaxis()->SetTitleOffset(1.2);
  h_SB_eff->GetYaxis()->SetTitleOffset(1.3);
  h_SB_eff->GetYaxis()->SetRangeUser(0,1.1);
  h_SB_eff->Draw(drawOptions_eff);
  h_HF_eff->Draw(drawOptions_eff_same);
  h_S_eff->Draw(drawOptions_eff_same);
  h_CombiWithConvLeg_eff->Draw(drawOptions_eff_same);
  h_CombiWithoutConvLeg_eff->Draw(drawOptions_eff_same);
  h_RPConv_eff->Draw(drawOptions_eff_same);
  leg_eff->Draw();
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



Float_t getPairPIDefficiency(Float_t pt1, Float_t pt2, TH1D &h_PIDeff) {

  Float_t PIDeff1 = (pt1 >= h_PIDeff.GetBinLowEdge(h_PIDeff.GetNbinsX())) ?
    h_PIDeff.GetBinContent(h_PIDeff.GetNbinsX()) : h_PIDeff.GetBinContent(h_PIDeff.FindBin(pt1));

  Float_t PIDeff2 = (pt2 >= h_PIDeff.GetBinLowEdge(h_PIDeff.GetNbinsX())) ?
    h_PIDeff.GetBinContent(h_PIDeff.GetNbinsX()) : h_PIDeff.GetBinContent(h_PIDeff.FindBin(pt2));

  return PIDeff1 * PIDeff2;
  
}
