#include <iostream>
#include <cstdlib>

#include <TROOT.h>
#include <TApplication.h>
#include <TStyle.h>
#include <TString.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TEfficiency.h>
#include <TStopwatch.h>
#include <TMath.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLatex.h>


void SetStyle(Bool_t graypalette=kFALSE);

Float_t getPairPIDefficiency(Float_t, Float_t, TH1D&);



void PlotMass() {

  // File containing the input pairtree (test) data:
  TString fileName_testData = "~/analysis/data/FT2_AnalysisResults_Upgrade/workingData/DNNAnalysis/FT2_ITSup_pairTree-us_part2_1-9-split.root";
  TString treeName_TestTree = "pairTree_us";

  // File containing the corresponding MVA output values:
  TString fileName_MVAoutput = "~/analysis/data/FT2_AnalysisResults_Upgrade/workingData/DNNAnalysis/FT2_ITSup_pairTree-us_part2_1-9-split.root";
  TString treeName_MVAoutputTree = "pairTree_us";
  
  TString h_text = "";//Single-track conv. rejection via MVA cuts";

  // two variables are combined, e.g., via
  // (tag1 >= wantedPrefilterTagValue1 && tag2 >= wantedPrefilterTagValue2)
  // if the following variable is set to kTRUE:
  Bool_t useTwoVars = kTRUE;

  Bool_t useTags = kFALSE; // kTRUE...use tag variables, kFALSE...use MVA cut defined below
  Float_t variable1, variable2; // choose variable type accordingly (tags: Int_t, MVAcut: Float_t)!
  
  Float_t MVAcut = 0.41;
  
  TString signalRegion = "+"; // "+"/"-"... accept values greater/smaller than MVAcut
  
  TString variableName1 = "MVAoutput_convTrack1";
  TString variableName2 = "MVAoutput_convTrack2";

  // After prefiltering, use events with this tag value only (if useTags==kTRUE):
  const Int_t wantedPrefilterTagValue1 = 0.;
  const Int_t wantedPrefilterTagValue2 = 0.;
  
  // set the used MVA method:
  const Bool_t isMLP = kTRUE;
  const Bool_t isBDT = kFALSE;

  // MVA output range in the corresponding input file:
  const Float_t MVAoutputRange_min = 0.;
  const Float_t MVAoutputRange_max = 1.;

  const Bool_t doConsiderPIDefficiencies = kTRUE;
  
  // ROOT file name containing all created histograms:
  TString outfileName = "temp_output/mass_histos_prefilterCuts.root";

  
  /////////////////////////////////////////////////////////////////////////

  
  if( !(wantedPrefilterTagValue1==0 || wantedPrefilterTagValue1==1) ) {
    std::cout << "   ERROR: Variable 'wantedPrefilterTagValue1' is required to have values "
	      << " 0 or 1, but has value " << wantedPrefilterTagValue1 << " instead."
	      << std::endl;
    gApplication->Terminate();
  }
  
  if(isMLP & isBDT) {
    std::cout << "  ERROR: Cannot use both MLP and BDT output." << std::endl;
    exit(1);
  }


  gROOT->ForceStyle();
  SetStyle();
  

  TFile *f = new TFile(fileName_testData,"READ");
  TTree *TestTree = (TTree*)f->Get(treeName_TestTree);

  Float_t mass;
  Float_t pt1, pt2;
  Int_t IsRP, IsConv, IsHF, motherPdg1, motherPdg2;
  TestTree->SetBranchAddress("mass", &mass);
  TestTree->SetBranchAddress("pt1", &pt1);
  TestTree->SetBranchAddress("pt2", &pt2);
  TestTree->SetBranchAddress("motherPdg1",&motherPdg1);
  TestTree->SetBranchAddress("motherPdg2",&motherPdg2);
  TestTree->SetBranchAddress("IsRP", &IsRP);
  TestTree->SetBranchAddress("IsConv", &IsConv);
  TestTree->SetBranchAddress("IsCorrHF", &IsHF);
  // if(TestTree->GetListOfBranches()->FindObject(variableName1) != NULL) {
  //   TestTree->SetBranchAddress(variableName1, &variable1);
  // }else {
  //   std::cout << "  ERROR: The branch " << variableName1
  // 	      << " does not exist in the file " << f->GetName() << std::endl;
  //   gApplication->Terminate();
  // }
  // if(useTwoVars) {
  //   if(TestTree->GetListOfBranches()->FindObject(variableName2) != NULL) {
  //     TestTree->SetBranchAddress(variableName2, &variable2);
  //   }else {
  //     std::cout << "  ERROR: The branch " << variableName2
  // 		<< " does not exist in the file " << f->GetName() << std::endl;
  //     gApplication->Terminate();
  //   }
  // }

  // input MVA output information from file:
  TFile *f_MVAoutput = new TFile(fileName_MVAoutput,"READ");
  TTree *MVAoutputTree = (TTree*)f_MVAoutput->Get(treeName_MVAoutputTree);
  Float_t MVAoutput;

  if(TestTree->GetEntries() != MVAoutputTree->GetEntries()) {
    std::cout << "   ERROR: The trees of the input files have different sizes."
	      << std::endl;
    std::cout << "   Size of tree in file " << fileName_testData << ": "
	      << TestTree->GetEntries() << std::endl;
    std::cout << "   Size of tree in file " << fileName_MVAoutput << ": "
	      << MVAoutputTree->GetEntries() << std::endl;
    gApplication->Terminate();
  }

  if(MVAoutputTree->GetListOfBranches()->FindObject(variableName1) != NULL) {
    MVAoutputTree->SetBranchAddress(variableName1, &variable1);
  }else {
    std::cout << "  ERROR: The branch " << variableName1
	      << " does not exist in the file " << f_MVAoutput->GetName() << std::endl;
    gApplication->Terminate();
  }
  if(useTwoVars) {
    if(MVAoutputTree->GetListOfBranches()->FindObject(variableName2) != NULL) {
      MVAoutputTree->SetBranchAddress(variableName2, &variable2);
    }else {
      std::cout << "  ERROR: The branch " << variableName2
		<< " does not exist in the file " << f_MVAoutput->GetName() << std::endl;
    }
  }


  // File containing the pt-dependent PID efficiencies:
  TString infile_PIDefficiencies_name = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/inputData/ITSU_PIDefficiency_lowB.root";
  TFile *infile_PIDefficiencies = new TFile(infile_PIDefficiencies_name, "READ");
  TH1D *h_PIDeff = (TH1D*)infile_PIDefficiencies->Get("efficiencyLHC17d12_TPCandTOF3sigma");
  h_PIDeff->GetXaxis()->SetRangeUser(0,5);

  TH1D *h_sample_weight = new TH1D("h_sample_weight","",1000,0,1);
  
  
  const unsigned int min=0, max=5, nBins=50;

  
  TH1D *h_SB = new TH1D("h_SB","",nBins,min,max);
  
  TH1D *h_S = new TH1D("h_S","",nBins,min,max);
  
  TH1D *h_CombiWithConvLeg = new TH1D("h_CombiWithConvLeg","",nBins,min,max);
  
  TH1D *h_CombiWithoutConvLeg =
    new TH1D("h_CombiWithoutConvLeg","",nBins,min,max);
  
  TH1D *h_HF = new TH1D("h_HF","",nBins,min,max);
  
  TH1D *h_RPConv = new TH1D("h_RPConv","",nBins,min,max);
  

  TH1D *h_S_prefilterCut = new TH1D("h_S_prefilterCut","",nBins,min,max);
  
  TH1D *h_SB_prefilterCut = new TH1D("h_SB_prefilterCut","",nBins,min,max);
  
  TH1D *h_CombiWithConvLeg_prefilterCut =
    new TH1D("h_CombiWithConvLeg_prefilterCut","",nBins,min,max);
  
  TH1D *h_CombiWithoutConvLeg_prefilterCut =
    new TH1D("h_CombiWithoutConvLeg_prefilterCut","",nBins,min,max);
  
  TH1D *h_HF_prefilterCut = new TH1D("h_HF_prefilterCut","",nBins,min,max);
  
  TH1D *h_RPConv_prefilterCut =
    new TH1D("h_RPConv_prefilterCut","",nBins,min,max);



  Long64_t nEv = TestTree->GetEntries();
  std::cout << "Starting to process " << nEv << " entries..." << std::endl;

  
  Float_t passed_seconds_prev = 0.;
  
  TStopwatch *watch = new TStopwatch();

  watch->Start();

  // linear mapping of the MVA cut value to the range [0,1]:
  MVAcut = (MVAcut-MVAoutputRange_min)/(MVAoutputRange_max-MVAoutputRange_min);

  
  for(Long64_t ev=0; ev<nEv; ev++) {

    if((ev%10000)==0) std::cout << "\rProcessing entry " << ev << " of "
				<< nEv << " (" << ev*100/nEv << "%)...";
    
    TestTree->GetEvent(ev);
    MVAoutputTree->GetEvent(ev);

    // linear mapping of the MVA output values to the range [0,1]:
    MVAoutput = (MVAoutput-MVAoutputRange_min)/(MVAoutputRange_max-MVAoutputRange_min);

    Float_t sample_weight = 1.;
    if(doConsiderPIDefficiencies) {
      sample_weight = getPairPIDefficiency(pt1, pt2, *h_PIDeff);
    }
    h_sample_weight->Fill(sample_weight);
    

    // Process all entries:
    h_SB->Fill(mass, sample_weight);
    if(IsRP==1 && IsConv==0) {
      h_S->Fill(mass, sample_weight);
    }
    if(IsRP==0 && IsConv==1) {
      h_CombiWithConvLeg->Fill(mass, sample_weight);
    }
    if(IsRP==0 && IsConv==0) {
      h_CombiWithoutConvLeg->Fill(mass, sample_weight);
    }
    if(IsRP==0 && IsHF==1) {
      h_HF->Fill(mass, sample_weight);
    }
    if(IsRP==1 && IsConv==1) {
      h_RPConv->Fill(mass, sample_weight);
    }


    // Process selected entries only:
    if(useTags) {
      
      if(!useTwoVars) {
	if(variable1 == wantedPrefilterTagValue1) {
	  h_SB_prefilterCut->Fill(mass, sample_weight);
	  if(IsRP==1 && IsConv==0) {
	    h_S_prefilterCut->Fill(mass, sample_weight);
	  }
	  if(IsRP==0 && IsConv==1) {
	    h_CombiWithConvLeg_prefilterCut->Fill(mass, sample_weight);
	  }
	  if(IsRP==0 && IsConv==0) {
	    h_CombiWithoutConvLeg_prefilterCut->Fill(mass, sample_weight);
	  }
	  if(IsRP==0 && IsHF==1) {
	    h_HF_prefilterCut->Fill(mass, sample_weight);
	  }
	  if(IsRP==1 && IsConv==1) {
	    h_RPConv_prefilterCut->Fill(mass, sample_weight);
	  }
	}
      }else { // if(useTwoVars)
	if(variable1 == wantedPrefilterTagValue1 &&
	   variable2 == wantedPrefilterTagValue2) {
	  h_SB_prefilterCut->Fill(mass, sample_weight);
	  if(IsRP==1 && IsConv==0) {
	    h_S_prefilterCut->Fill(mass, sample_weight);
	  }
	  if(IsRP==0 && IsConv==1) {
	    h_CombiWithConvLeg_prefilterCut->Fill(mass, sample_weight);
	  }
	  if(IsRP==0 && IsConv==0) {
	    h_CombiWithoutConvLeg_prefilterCut->Fill(mass, sample_weight);
	  }
	  if(IsRP==0 && IsHF==1) {
	    h_HF_prefilterCut->Fill(mass, sample_weight);
	  }
	  if(IsRP==1 && IsConv==1) {
	    h_RPConv_prefilterCut->Fill(mass, sample_weight);
	  }
	}
      }
      
    }else { // if(!useTags)
      
      if(!useTwoVars) {
	if(signalRegion == "+" && variable1 >= MVAcut) {
	  h_SB_prefilterCut->Fill(mass, sample_weight);
	  if(IsRP==1 && IsConv==0) {
	    h_S_prefilterCut->Fill(mass, sample_weight);
	  }
	  if(IsRP==0 && IsConv==1) {
	    h_CombiWithConvLeg_prefilterCut->Fill(mass, sample_weight);
	  }
	  if(IsRP==0 && IsConv==0) {
	    h_CombiWithoutConvLeg_prefilterCut->Fill(mass, sample_weight);
	  }
	  if(IsRP==0 && IsHF==1) {
	    h_HF_prefilterCut->Fill(mass, sample_weight);
	  }
	  if(IsRP==1 && IsConv==1) {
	    h_RPConv_prefilterCut->Fill(mass, sample_weight);
	  }
	}
	if(signalRegion == "-" && variable1 <= MVAcut) {
	  h_SB_prefilterCut->Fill(mass, sample_weight);
	  if(IsRP==1 && IsConv==0) {
	    h_S_prefilterCut->Fill(mass, sample_weight);
	  }
	  if(IsRP==0 && IsConv==1) {
	    h_CombiWithConvLeg_prefilterCut->Fill(mass, sample_weight);
	  }
	  if(IsRP==0 && IsConv==0) {
	    h_CombiWithoutConvLeg_prefilterCut->Fill(mass, sample_weight);
	  }
	  if(IsRP==0 && IsHF==1) {
	    h_HF_prefilterCut->Fill(mass, sample_weight);
	  }
	  if(IsRP==1 && IsConv==1) {
	    h_RPConv_prefilterCut->Fill(mass, sample_weight);
	  }
	}
      }else { // if(useTwoVars)
	if(signalRegion == "+" &&
	   variable1 >= MVAcut &&
	   variable2 >= MVAcut) {
	  h_SB_prefilterCut->Fill(mass, sample_weight);
	  if(IsRP==1 && IsConv==0) {
	    h_S_prefilterCut->Fill(mass, sample_weight);
	  }
	  if(IsRP==0 && IsConv==1) {
	    h_CombiWithConvLeg_prefilterCut->Fill(mass, sample_weight);
	  }
	  if(IsRP==0 && IsConv==0) {
	    h_CombiWithoutConvLeg_prefilterCut->Fill(mass, sample_weight);
	  }
	  if(IsRP==0 && IsHF==1) {
	    h_HF_prefilterCut->Fill(mass, sample_weight);
	  }
	  if(IsRP==1 && IsConv==1) {
	    h_RPConv_prefilterCut->Fill(mass, sample_weight);
	  }
	}
	if(signalRegion == "-" &&
	   variable1 <= MVAcut &&
	   variable2 <= MVAcut) {
	  h_SB_prefilterCut->Fill(mass, sample_weight);
	  if(IsRP==1 && IsConv==0) {
	    h_S_prefilterCut->Fill(mass, sample_weight);
	  }
	  if(IsRP==0 && IsConv==1) {
	    h_CombiWithConvLeg_prefilterCut->Fill(mass, sample_weight);
	  }
	  if(IsRP==0 && IsConv==0) {
	    h_CombiWithoutConvLeg_prefilterCut->Fill(mass, sample_weight);
	  }
	  if(IsRP==0 && IsHF==1) {
	    h_HF_prefilterCut->Fill(mass, sample_weight);
	  }
	  if(IsRP==1 && IsConv==1) {
	    h_RPConv_prefilterCut->Fill(mass, sample_weight);
	  }
	}
      }
      
    }
    
  } // end ev loop

   
  std::cout << std::endl << std::endl;
  std::cout << "Time elapsed since begin of processing: " << std::endl;
  std::cout << "\t";
  watch->Print();
  std::cout << std::endl;
  watch->Stop();



  
  //////////////////// SETTING UP THE PLOTS:  


  if(!TEfficiency::CheckConsistency(*h_SB_prefilterCut, *h_SB) ||
     !TEfficiency::CheckConsistency(*h_S_prefilterCut, *h_S) ||
     !TEfficiency::CheckConsistency(*h_CombiWithConvLeg_prefilterCut, *h_CombiWithConvLeg) ||
     !TEfficiency::CheckConsistency(*h_CombiWithoutConvLeg_prefilterCut, *h_CombiWithoutConvLeg) ||
     !TEfficiency::CheckConsistency(*h_HF_prefilterCut, *h_HF) ||
     !TEfficiency::CheckConsistency(*h_RPConv_prefilterCut, *h_RPConv)) {
    std::cout << "  ERROR: Histograms are inconsistent for TEfficiency object initialization." << std::endl;
    gApplication->Terminate();
  }

  TEfficiency *pEff_SB = new TEfficiency(*h_SB_prefilterCut, *h_SB);
  TEfficiency *pEff_S = new TEfficiency(*h_S_prefilterCut, *h_S);
  TEfficiency *pEff_CombiWithConvLeg = new TEfficiency(*h_CombiWithConvLeg_prefilterCut, *h_CombiWithConvLeg);
  TEfficiency *pEff_CombiWithoutConvLeg = new TEfficiency(*h_CombiWithoutConvLeg_prefilterCut, *h_CombiWithoutConvLeg);
  TEfficiency *pEff_HF = new TEfficiency(*h_HF_prefilterCut, *h_HF);
  TEfficiency *pEff_RPConv = new TEfficiency(*h_RPConv_prefilterCut, *h_RPConv);
  
  
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

  Int_t lineWidth = 3;
  h_SB->SetLineWidth(lineWidth);
  h_S->SetLineWidth(lineWidth);
  h_CombiWithConvLeg->SetLineWidth(lineWidth);
  h_CombiWithoutConvLeg->SetLineWidth(lineWidth);
  h_HF->SetLineWidth(lineWidth);
  h_RPConv->SetLineWidth(lineWidth);

  h_SB_prefilterCut->SetLineWidth(lineWidth);
  h_S_prefilterCut->SetLineWidth(lineWidth);
  h_CombiWithConvLeg_prefilterCut->SetLineWidth(lineWidth);
  h_CombiWithoutConvLeg_prefilterCut->SetLineWidth(lineWidth);
  h_HF_prefilterCut->SetLineWidth(lineWidth);
  h_RPConv_prefilterCut->SetLineWidth(lineWidth);

  h_SB_prefilterCut->SetLineColor(kBlack);
  h_SB_prefilterCut->SetMarkerColor(kBlack);
  h_S_prefilterCut->SetLineColor(kGreen+1);
  h_S_prefilterCut->SetMarkerColor(kGreen+1);
  h_CombiWithConvLeg_prefilterCut->SetLineColor(kRed);
  h_CombiWithConvLeg_prefilterCut->SetMarkerColor(kRed);
  h_CombiWithoutConvLeg_prefilterCut->SetLineColor(kBlue);
  h_CombiWithoutConvLeg_prefilterCut->SetMarkerColor(kBlue);
  h_HF_prefilterCut->SetLineColor(kOrange);
  h_HF_prefilterCut->SetMarkerColor(kOrange);
  h_RPConv_prefilterCut->SetLineColor(kMagenta);
  h_RPConv_prefilterCut->SetMarkerColor(kMagenta);

  Int_t markerStyle = 20;
  h_SB_prefilterCut->SetMarkerStyle(markerStyle);
  h_S_prefilterCut->SetMarkerStyle(markerStyle);
  h_CombiWithConvLeg_prefilterCut->SetMarkerStyle(markerStyle);
  h_CombiWithoutConvLeg_prefilterCut->SetMarkerStyle(markerStyle);
  h_HF_prefilterCut->SetMarkerStyle(markerStyle);
  h_RPConv_prefilterCut->SetMarkerStyle(markerStyle);

  Float_t markerSize = 1;
  h_SB_prefilterCut->SetMarkerSize(markerSize);
  h_S_prefilterCut->SetMarkerSize(markerSize);
  h_CombiWithConvLeg_prefilterCut->SetMarkerSize(markerSize);
  h_CombiWithoutConvLeg_prefilterCut->SetMarkerSize(markerSize);
  h_HF->SetMarkerSize(markerSize);
  h_RPConv_prefilterCut->SetMarkerSize(markerSize);

  h_SB->GetYaxis()->SetRangeUser(1e-1,1e8);
  h_SB->SetXTitle("#it{m}_{ee} / (GeV/#it{c}^{2})");
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


  TString drawOptions_mass = "hist e x0";
  TString drawOptions_mass_same = drawOptions_mass + " same";

  TString drawOptions_mass_cut = "e1 x0 same";

  TCanvas *c = new TCanvas("c","",1024,768);
  c->SetLogy();
  h_SB->Draw(drawOptions_mass);
  h_S->Draw(drawOptions_mass_same);
  h_CombiWithConvLeg->Draw(drawOptions_mass_same);
  h_CombiWithoutConvLeg->Draw(drawOptions_mass_same);
  h_HF->Draw(drawOptions_mass_same);
  h_RPConv->Draw(drawOptions_mass_same);

  h_SB_prefilterCut->Draw(drawOptions_mass_cut);
  h_S_prefilterCut->Draw(drawOptions_mass_cut);
  h_CombiWithConvLeg_prefilterCut->Draw(drawOptions_mass_cut);
  h_CombiWithoutConvLeg_prefilterCut->Draw(drawOptions_mass_cut);
  h_HF_prefilterCut->Draw(drawOptions_mass_cut);
  h_RPConv_prefilterCut->Draw(drawOptions_mass_cut);

  
  TLegend *leg_mass_nocuts = new TLegend(.575,.6,.67,.83, "w/o,");
  leg_mass_nocuts->AddEntry(h_SB,"","l");
  leg_mass_nocuts->AddEntry(h_S,"","l");
  leg_mass_nocuts->AddEntry(h_CombiWithConvLeg,"","l");
  leg_mass_nocuts->AddEntry(h_CombiWithoutConvLeg,"","l");
  leg_mass_nocuts->AddEntry(h_HF,"","l");
  leg_mass_nocuts->AddEntry(h_RPConv,"","l");
  TLegendEntry *leg_mass_nocuts_header =
    (TLegendEntry*)leg_mass_nocuts->GetListOfPrimitives()->First();
  leg_mass_nocuts_header->SetTextSize(.033);
  leg_mass_nocuts->Draw();

  TLegend *leg_mass = new TLegend(.62,.6,.88,.83, "with MVA cuts");
  leg_mass->AddEntry(h_SB_eff,"S+B","p");
  leg_mass->AddEntry(h_S_eff,"S","p");
  leg_mass->AddEntry(h_CombiWithConvLeg_eff,"Comb. w. conv. leg","p");
  leg_mass->AddEntry(h_CombiWithoutConvLeg_eff,"Comb. w/o conv. leg","p");
  leg_mass->AddEntry(h_HF_eff,"Corr. HF","p");
  leg_mass->AddEntry(h_RPConv_eff,"RP conv.","p");
  TLegendEntry *leg_mass_header =
    (TLegendEntry*)leg_mass->GetListOfPrimitives()->First();
  leg_mass_header->SetTextSize(.033);
  leg_mass->Draw();
  

  TLatex l;
  l.SetTextSize(.025);
  l.DrawLatex(.1,1.5e7,h_text);

  TLatex *l_ALICE = new TLatex(.19,.82,"ALICE work in progress");
  l_ALICE->SetTextSize(.035);
  l_ALICE->SetTextFont(42);
  l_ALICE->SetNDC();
  l_ALICE->Draw();

  TLatex *l_info = new TLatex(.19,.78,"HIJING, Pb-Pb #sqrt{#it{s}_{NN}} = 5.5 TeV, 0-10%");
  l_info->SetTextSize(.03);
  l_info->SetTextFont(42);
  l_info->SetNDC();
  l_info->Draw();

  c->SaveAs("temp_output/mass_prefilterCuts.pdf");
  c->SaveAs("temp_output/mass_prefilterCuts.root");
  c->SaveAs("temp_output/mass_prefilterCuts.png");


  TString drawOptions_eff = "hist p x0 e1";
  TString drawOptions_eff_same = drawOptions_eff + " same";


  TLegend *leg_eff = new TLegend(.55,.2,.88,.43);
  leg_eff->AddEntry(h_SB_eff,"S+B","p");
  leg_eff->AddEntry(h_S_eff,"S","p");
  leg_eff->AddEntry(h_CombiWithConvLeg_eff,"Comb. w. conv. leg","p");
  leg_eff->AddEntry(h_CombiWithoutConvLeg_eff,"Comb. w/o conv. leg","p");
  leg_eff->AddEntry(h_HF_eff,"Corr. heavy flavor","p");
  leg_eff->AddEntry(h_RPConv_eff,"RP conv.","p");
  

  TCanvas *c_eff = new TCanvas("c_eff","",1024,768);
  h_SB_eff->SetXTitle("#it{m}_{ee} / (GeV/#it{c}^{2})");
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

  TLatex *l_eff_ALICE = new TLatex(.2,.26,"ALICE work in progress");
  l_eff_ALICE->SetTextSize(.035);
  l_eff_ALICE->SetTextFont(42);
  l_eff_ALICE->SetNDC();
  l_eff_ALICE->Draw();

  TLatex *l_eff_info = new TLatex(.2,.22,"HIJING, Pb-Pb #sqrt{#it{s}_{NN}} = 5.5 TeV, 0-10%");
  l_eff_info->SetTextSize(.03);
  l_eff_info->SetTextFont(42);
  l_eff_info->SetNDC();
  l_eff_info->Draw();

  c_eff->SaveAs("temp_output/mass_eff_prefilterCuts.pdf");
  c_eff->SaveAs("temp_output/mass_eff_prefilterCuts.root");
  c_eff->SaveAs("temp_output/mass_eff_prefilterCuts.png");



  // store all created histograms in a ROOT file:
  TFile *outfile = new TFile(outfileName, "RECREATE");

  h_SB->Write(0, TObject::kOverwrite);
  h_S->Write(0, TObject::kOverwrite);
  h_CombiWithConvLeg->Write(0, TObject::kOverwrite);
  h_CombiWithoutConvLeg->Write(0, TObject::kOverwrite);
  h_HF->Write(0, TObject::kOverwrite);
  h_RPConv->Write(0, TObject::kOverwrite);

  h_SB_prefilterCut->Write(0, TObject::kOverwrite);
  h_S_prefilterCut->Write(0, TObject::kOverwrite);
  h_CombiWithConvLeg_prefilterCut->Write(0, TObject::kOverwrite);
  h_CombiWithoutConvLeg_prefilterCut->Write(0, TObject::kOverwrite);
  h_HF_prefilterCut->Write(0, TObject::kOverwrite);
  h_RPConv_prefilterCut->Write(0, TObject::kOverwrite);

  h_SB_eff->Write(0, TObject::kOverwrite);
  h_S_eff->Write(0, TObject::kOverwrite);
  h_CombiWithConvLeg_eff->Write(0, TObject::kOverwrite);
  h_CombiWithoutConvLeg_eff->Write(0, TObject::kOverwrite);
  h_HF_eff->Write(0, TObject::kOverwrite);
  h_RPConv_eff->Write(0, TObject::kOverwrite);
  
  outfile->Close();
  
  // gApplication->Terminate();
}


Float_t getPairPIDefficiency(Float_t pt1, Float_t pt2, TH1D &h_PIDeff) {

  Float_t PIDeff1 = (pt1 >= h_PIDeff.GetBinLowEdge(h_PIDeff.GetNbinsX())) ?
    h_PIDeff.GetBinContent(h_PIDeff.GetNbinsX()) : h_PIDeff.GetBinContent(h_PIDeff.FindBin(pt1));

  Float_t PIDeff2 = (pt2 >= h_PIDeff.GetBinLowEdge(h_PIDeff.GetNbinsX())) ?
    h_PIDeff.GetBinContent(h_PIDeff.GetNbinsX()) : h_PIDeff.GetBinContent(h_PIDeff.FindBin(pt2));

  return PIDeff1 * PIDeff2;
  
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
  gStyle->SetTitleOffset(1.25,"y");
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
