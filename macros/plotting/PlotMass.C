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

Float_t significanceError(TH2D&, TH2D&, Int_t, Int_t);

Float_t significanceError_uncorrelated(Float_t S, Float_t B) {
  return 0.5*TMath::Sqrt((S*(S+4*B))/(S+B)/(S+B)); // neglects correlations
}

Float_t SoverBError(Float_t S, Float_t B) {
  return TMath::Sqrt(S*(S+B)/TMath::Power(B,3));
}


void PlotMass() {

  // File containing the input pairtree (test) data:
  TString fileName_testData = "~/analysis/data/FT2_AnalysisResults_Upgrade/workingData/DNNAnalysis/FT2_ITSup_pairTree-us_part2_1-9-split.root";
  TString treeName_TestTree = "pairTree_us";

  // File containing the corresponding MVA output values:
  TString fileName_MVAoutput = "~/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/CombConvRejMVA_DNN_0.05mass/output/predictions_DNN.root";
  TString treeName_MVAoutputTree = "pairTree_MVAoutput";
  
  TString h_text = "";//Single-track conv. rejection via MVA cuts";

  // two variables are combined, e.g., via
  // (tag1 >= wantedPrefilterTagValue1 && tag2 >= wantedPrefilterTagValue2)
  // if the following variable is set to kTRUE:
  Bool_t useTwoVars = kFALSE;

  Bool_t useTags = kFALSE; // kTRUE...use tag variables, kFALSE...use MVA cut defined below
  Float_t variable1, variable2; // choose variable type accordingly (tags: Int_t, MVAcut: Float_t)!
  
  Float_t MVAcut = 0.21;
  
  TString signalRegion = "+"; // "+"/"-"... accept values greater/smaller than MVAcut
  
  TString variableName1 = "DNN";
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
  
  // number of subsamples which are created for significance error estimation:
  const Int_t num_subsamples = 20;

  // enable bootstrapping for significance error estimation:
  const Bool_t doBootstrap = kTRUE;

  // Output file path prefix:
  TString output_prefix = "temp_output/";
  if(!output_prefix.EndsWith("/")) output_prefix += "/";
  
  // ROOT file name containing all created histograms:
  TString outfileName = output_prefix + "mass_histos.root";

  
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
  TString infile_PIDefficiencies_name = "~/analysis/data/FT2_AnalysisResults_Upgrade/inputData/ITSU_PIDefficiency_lowB.root";
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

  
  TH2D *h_S_subsample =
    new TH2D("h_S_subsample","",nBins,min,max,num_subsamples,0,num_subsamples);

  TH2D *h_SB_subsample =
    new TH2D("h_SB_subsample","",nBins,min,max,num_subsamples,0,num_subsamples);

  TH2D *h_CombiWithConvLeg_subsample =
    new TH2D("h_CombiWithConvLeg_subsample","",nBins,min,max,num_subsamples,0,num_subsamples);

  TH2D *h_CombiWithoutConvLeg_subsample =
    new TH2D("h_CombiWithoutConvLeg_subsample","",nBins,min,max,num_subsamples,0,num_subsamples);

  TH2D *h_HF_subsample =
    new TH2D("h_HF_subsample","",nBins,min,max,num_subsamples,0,num_subsamples);

  TH2D *h_RPConv_subsample =
    new TH2D("h_RPConv_subsample","",nBins,min,max,num_subsamples,0,num_subsamples);

  

  TH1D *h_S_afterCut = new TH1D("h_S_afterCut","",nBins,min,max);
  
  TH1D *h_SB_afterCut = new TH1D("h_SB_afterCut","",nBins,min,max);
  
  TH1D *h_CombiWithConvLeg_afterCut =
    new TH1D("h_CombiWithConvLeg_afterCut","",nBins,min,max);
  
  TH1D *h_CombiWithoutConvLeg_afterCut =
    new TH1D("h_CombiWithoutConvLeg_afterCut","",nBins,min,max);
  
  TH1D *h_HF_afterCut = new TH1D("h_HF_afterCut","",nBins,min,max);
  
  TH1D *h_RPConv_afterCut =
    new TH1D("h_RPConv_afterCut","",nBins,min,max);


  TH2D *h_S_afterCut_subsample =
    new TH2D("h_S_afterCut_subsample","",nBins,min,max,num_subsamples,0,num_subsamples);

  TH2D *h_SB_afterCut_subsample =
    new TH2D("h_SB_afterCut_subsample","",nBins,min,max,num_subsamples,0,num_subsamples);

  TH2D *h_CombiWithConvLeg_afterCut_subsample =
    new TH2D("h_CombiWithConvLeg_afterCut_subsample","",nBins,min,max,num_subsamples,0,num_subsamples);

  TH2D *h_CombiWithoutConvLeg_afterCut_subsample =
    new TH2D("h_CombiWithoutConvLeg_afterCut_subsample","",nBins,min,max,num_subsamples,0,num_subsamples);

  TH2D *h_HF_afterCut_subsample =
    new TH2D("h_HF_afterCut_subsample","",nBins,min,max,num_subsamples,0,num_subsamples);

  TH2D *h_RPConv_afterCut_subsample =
    new TH2D("h_RPConv_afterCut_subsample","",nBins,min,max,num_subsamples,0,num_subsamples);

  for(Int_t i=1; i<=nBins; i++) {
    for(Int_t j=1; j<=num_subsamples; j++) {
      h_S_afterCut_subsample->SetBinContent(i, j, 0.);
      h_SB_afterCut_subsample->SetBinContent(i, j, 0.);
      h_CombiWithConvLeg_afterCut_subsample->SetBinContent(i, j, 0.);
      h_CombiWithoutConvLeg_afterCut_subsample->SetBinContent(i, j, 0.);
      h_HF_afterCut_subsample->SetBinContent(i, j, 0.);
      h_RPConv_afterCut_subsample->SetBinContent(i, j, 0.);
    }
  }

  TRandom *rand_subsample = new TRandom();
  
  const Int_t b_max = doBootstrap ? num_subsamples : 1;
  


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
    for(Int_t b=0; b<b_max; b++) {
      h_SB_subsample->Fill(mass, rand_subsample->Integer(num_subsamples), sample_weight);
    }
    if(IsRP==1 && IsConv==0) {
      h_S->Fill(mass, sample_weight);
      for(Int_t b=0; b<b_max; b++) {
	h_S_subsample->Fill(mass, rand_subsample->Integer(num_subsamples), sample_weight);
      }
    }
    if(IsRP==0 && IsConv==1) {
      h_CombiWithConvLeg->Fill(mass, sample_weight);
      for(Int_t b=0; b<b_max; b++) {
	h_CombiWithConvLeg_subsample->Fill(mass, rand_subsample->Integer(num_subsamples), sample_weight);
      }
    }
    if(IsRP==0 && IsConv==0) {
      h_CombiWithoutConvLeg->Fill(mass, sample_weight);
      for(Int_t b=0; b<b_max; b++) {
	h_CombiWithoutConvLeg_subsample->Fill(mass, rand_subsample->Integer(num_subsamples), sample_weight);
      }
    }
    if(IsRP==0 && IsHF==1) {
      h_HF->Fill(mass, sample_weight);
      for(Int_t b=0; b<b_max; b++) {
	h_HF_subsample->Fill(mass, rand_subsample->Integer(num_subsamples), sample_weight);
      }
    }
    if(IsRP==1 && IsConv==1) {
      h_RPConv->Fill(mass, sample_weight);
      for(Int_t b=0; b<b_max; b++) {
	h_RPConv_subsample->Fill(mass, rand_subsample->Integer(num_subsamples), sample_weight);
      }
    }


    // Only process entries which pass the cut:
    if(useTags) {
      
      if(!useTwoVars) {
	if(variable1 == wantedPrefilterTagValue1) {
	  h_SB_afterCut->Fill(mass, sample_weight);
	  for(Int_t b=0; b<b_max; b++) {
	    h_SB_afterCut_subsample->Fill(mass, rand_subsample->Integer(num_subsamples), sample_weight);
	  }
	  if(IsRP==1 && IsConv==0) {
	    h_S_afterCut->Fill(mass, sample_weight);
	    for(Int_t b=0; b<b_max; b++) {
	      h_S_afterCut_subsample->Fill(mass, rand_subsample->Integer(num_subsamples), sample_weight);
	    }
	  }
	  if(IsRP==0 && IsConv==1) {
	    h_CombiWithConvLeg_afterCut->Fill(mass, sample_weight);
	    for(Int_t b=0; b<b_max; b++) {
	      h_CombiWithConvLeg_afterCut_subsample->Fill(mass, rand_subsample->Integer(num_subsamples), sample_weight);
	    }
	  }
	  if(IsRP==0 && IsConv==0) {
	    h_CombiWithoutConvLeg_afterCut->Fill(mass, sample_weight);
	    for(Int_t b=0; b<b_max; b++) {
	      h_CombiWithoutConvLeg_afterCut_subsample->Fill(mass, rand_subsample->Integer(num_subsamples), sample_weight);
	    }
	  }
	  if(IsRP==0 && IsHF==1) {
	    h_HF_afterCut->Fill(mass, sample_weight);
	    for(Int_t b=0; b<b_max; b++) {
	      h_HF_afterCut_subsample->Fill(mass, rand_subsample->Integer(num_subsamples), sample_weight);
	    }
	  }
	  if(IsRP==1 && IsConv==1) {
	    h_RPConv_afterCut->Fill(mass, sample_weight);
	    for(Int_t b=0; b<b_max; b++) {
	      h_RPConv_afterCut_subsample->Fill(mass, rand_subsample->Integer(num_subsamples), sample_weight);
	    }
	  }
	}
      }else { // if(useTwoVars)
	if(variable1 == wantedPrefilterTagValue1 &&
	   variable2 == wantedPrefilterTagValue2) {
	  h_SB_afterCut->Fill(mass, sample_weight);
	  for(Int_t b=0; b<b_max; b++) {
	    h_SB_afterCut_subsample->Fill(mass, rand_subsample->Integer(num_subsamples), sample_weight);
	  }
	  if(IsRP==1 && IsConv==0) {
	    h_S_afterCut->Fill(mass, sample_weight);
	    for(Int_t b=0; b<b_max; b++) {
	      h_S_afterCut_subsample->Fill(mass, rand_subsample->Integer(num_subsamples), sample_weight);
	    }
	  }
	  if(IsRP==0 && IsConv==1) {
	    h_CombiWithConvLeg_afterCut->Fill(mass, sample_weight);
	    for(Int_t b=0; b<b_max; b++) {
	      h_CombiWithConvLeg_afterCut_subsample->Fill(mass, rand_subsample->Integer(num_subsamples), sample_weight);
	    }
	  }
	  if(IsRP==0 && IsConv==0) {
	    h_CombiWithoutConvLeg_afterCut->Fill(mass, sample_weight);
	    for(Int_t b=0; b<b_max; b++) {
	      h_CombiWithoutConvLeg_afterCut_subsample->Fill(mass, rand_subsample->Integer(num_subsamples), sample_weight);
	    }
	  }
	  if(IsRP==0 && IsHF==1) {
	    h_HF_afterCut->Fill(mass, sample_weight);
	    for(Int_t b=0; b<b_max; b++) {
	      h_HF_afterCut_subsample->Fill(mass, rand_subsample->Integer(num_subsamples), sample_weight);
	    }
	  }
	  if(IsRP==1 && IsConv==1) {
	    h_RPConv_afterCut->Fill(mass, sample_weight);
	    for(Int_t b=0; b<b_max; b++) {
	      h_RPConv_afterCut_subsample->Fill(mass, rand_subsample->Integer(num_subsamples), sample_weight);
	    }
	  }
	}
      }
      
    }else { // if(!useTags)
      
      if(!useTwoVars) {
	if(signalRegion == "+" && variable1 >= MVAcut) {
	  h_SB_afterCut->Fill(mass, sample_weight);
	  for(Int_t b=0; b<b_max; b++) {
	    h_SB_afterCut_subsample->Fill(mass, rand_subsample->Integer(num_subsamples), sample_weight);
	  }
	  if(IsRP==1 && IsConv==0) {
	    h_S_afterCut->Fill(mass, sample_weight);
	    for(Int_t b=0; b<b_max; b++) {
	      h_S_afterCut_subsample->Fill(mass, rand_subsample->Integer(num_subsamples), sample_weight);
	    }
	  }
	  if(IsRP==0 && IsConv==1) {
	    h_CombiWithConvLeg_afterCut->Fill(mass, sample_weight);
	    for(Int_t b=0; b<b_max; b++) {
	      h_CombiWithConvLeg_afterCut_subsample->Fill(mass, rand_subsample->Integer(num_subsamples), sample_weight);
	    }
	  }
	  if(IsRP==0 && IsConv==0) {
	    h_CombiWithoutConvLeg_afterCut->Fill(mass, sample_weight);
	    for(Int_t b=0; b<b_max; b++) {
	      h_CombiWithoutConvLeg_afterCut_subsample->Fill(mass, rand_subsample->Integer(num_subsamples), sample_weight);
	    }
	  }
	  if(IsRP==0 && IsHF==1) {
	    h_HF_afterCut->Fill(mass, sample_weight);
	    for(Int_t b=0; b<b_max; b++) {
	      h_HF_afterCut_subsample->Fill(mass, rand_subsample->Integer(num_subsamples), sample_weight);
	    }
	  }
	  if(IsRP==1 && IsConv==1) {
	    h_RPConv_afterCut->Fill(mass, sample_weight);
	    for(Int_t b=0; b<b_max; b++) {
	      h_RPConv_afterCut_subsample->Fill(mass, rand_subsample->Integer(num_subsamples), sample_weight);
	    }
	  }
	}
	if(signalRegion == "-" && variable1 <= MVAcut) {
	  h_SB_afterCut->Fill(mass, sample_weight);
	  for(Int_t b=0; b<b_max; b++) {
	    h_SB_afterCut_subsample->Fill(mass, rand_subsample->Integer(num_subsamples), sample_weight);
	  }
	  if(IsRP==1 && IsConv==0) {
	    h_S_afterCut->Fill(mass, sample_weight);
	    for(Int_t b=0; b<b_max; b++) {
	      h_S_afterCut_subsample->Fill(mass, rand_subsample->Integer(num_subsamples), sample_weight);
	    }
	  }
	  if(IsRP==0 && IsConv==1) {
	    h_CombiWithConvLeg_afterCut->Fill(mass, sample_weight);
	    for(Int_t b=0; b<b_max; b++) {
	      h_CombiWithConvLeg_afterCut_subsample->Fill(mass, rand_subsample->Integer(num_subsamples), sample_weight);
	    }
	  }
	  if(IsRP==0 && IsConv==0) {
	    h_CombiWithoutConvLeg_afterCut->Fill(mass, sample_weight);
	    for(Int_t b=0; b<b_max; b++) {
	      h_CombiWithoutConvLeg_afterCut_subsample->Fill(mass, rand_subsample->Integer(num_subsamples), sample_weight);
	    }
	  }
	  if(IsRP==0 && IsHF==1) {
	    h_HF_afterCut->Fill(mass, sample_weight);
	    for(Int_t b=0; b<b_max; b++) {
	      h_HF_afterCut_subsample->Fill(mass, rand_subsample->Integer(num_subsamples), sample_weight);
	    }
	  }
	  if(IsRP==1 && IsConv==1) {
	    h_RPConv_afterCut->Fill(mass, sample_weight);
	    for(Int_t b=0; b<b_max; b++) {
	      h_RPConv_afterCut_subsample->Fill(mass, rand_subsample->Integer(num_subsamples), sample_weight);
	    }
	  }
	}
      }else { // if(useTwoVars)
	if(signalRegion == "+" &&
	   variable1 >= MVAcut &&
	   variable2 >= MVAcut) {
	  h_SB_afterCut->Fill(mass, sample_weight);
	  for(Int_t b=0; b<b_max; b++) {
	    h_SB_afterCut_subsample->Fill(mass, rand_subsample->Integer(num_subsamples), sample_weight);
	  }
	  if(IsRP==1 && IsConv==0) {
	    h_S_afterCut->Fill(mass, sample_weight);
	    for(Int_t b=0; b<b_max; b++) {
	      h_S_afterCut_subsample->Fill(mass, rand_subsample->Integer(num_subsamples), sample_weight);
	    }
	  }
	  if(IsRP==0 && IsConv==1) {
	    h_CombiWithConvLeg_afterCut->Fill(mass, sample_weight);
	    for(Int_t b=0; b<b_max; b++) {
	      h_CombiWithConvLeg_afterCut_subsample->Fill(mass, rand_subsample->Integer(num_subsamples), sample_weight);
	    }
	  }
	  if(IsRP==0 && IsConv==0) {
	    h_CombiWithoutConvLeg_afterCut->Fill(mass, sample_weight);
	    for(Int_t b=0; b<b_max; b++) {
	      h_CombiWithoutConvLeg_afterCut_subsample->Fill(mass, rand_subsample->Integer(num_subsamples), sample_weight);
	    }
	  }
	  if(IsRP==0 && IsHF==1) {
	    h_HF_afterCut->Fill(mass, sample_weight);
	    for(Int_t b=0; b<b_max; b++) {
	      h_HF_afterCut_subsample->Fill(mass, rand_subsample->Integer(num_subsamples), sample_weight);
	    }
	  }
	  if(IsRP==1 && IsConv==1) {
	    h_RPConv_afterCut->Fill(mass, sample_weight);
	    for(Int_t b=0; b<b_max; b++) {
	      h_RPConv_afterCut_subsample->Fill(mass, rand_subsample->Integer(num_subsamples), sample_weight);
	    }
	  }
	}
	if(signalRegion == "-" &&
	   variable1 <= MVAcut &&
	   variable2 <= MVAcut) {
	  h_SB_afterCut->Fill(mass, sample_weight);
	  for(Int_t b=0; b<b_max; b++) {
	    h_SB_afterCut_subsample->Fill(mass, rand_subsample->Integer(num_subsamples), sample_weight);
	  }
	  if(IsRP==1 && IsConv==0) {
	    h_S_afterCut->Fill(mass, sample_weight);
	    for(Int_t b=0; b<b_max; b++) {
	      h_S_afterCut_subsample->Fill(mass, rand_subsample->Integer(num_subsamples), sample_weight);
	    }
	  }
	  if(IsRP==0 && IsConv==1) {
	    h_CombiWithConvLeg_afterCut->Fill(mass, sample_weight);
	    for(Int_t b=0; b<b_max; b++) {
	      h_CombiWithConvLeg_afterCut_subsample->Fill(mass, rand_subsample->Integer(num_subsamples), sample_weight);
	    }
	  }
	  if(IsRP==0 && IsConv==0) {
	    h_CombiWithoutConvLeg_afterCut->Fill(mass, sample_weight);
	    for(Int_t b=0; b<b_max; b++) {
	      h_CombiWithoutConvLeg_afterCut_subsample->Fill(mass, rand_subsample->Integer(num_subsamples), sample_weight);
	    }
	  }
	  if(IsRP==0 && IsHF==1) {
	    h_HF_afterCut->Fill(mass, sample_weight);
	    for(Int_t b=0; b<b_max; b++) {
	      h_HF_afterCut_subsample->Fill(mass, rand_subsample->Integer(num_subsamples), sample_weight);
	    }
	  }
	  if(IsRP==1 && IsConv==1) {
	    h_RPConv_afterCut->Fill(mass, sample_weight);
	    for(Int_t b=0; b<b_max; b++) {
	      h_RPConv_afterCut_subsample->Fill(mass, rand_subsample->Integer(num_subsamples), sample_weight);
	    }
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


  if(!TEfficiency::CheckConsistency(*h_SB_afterCut, *h_SB) ||
     !TEfficiency::CheckConsistency(*h_S_afterCut, *h_S) ||
     !TEfficiency::CheckConsistency(*h_CombiWithConvLeg_afterCut, *h_CombiWithConvLeg) ||
     !TEfficiency::CheckConsistency(*h_CombiWithoutConvLeg_afterCut, *h_CombiWithoutConvLeg) ||
     !TEfficiency::CheckConsistency(*h_HF_afterCut, *h_HF) ||
     !TEfficiency::CheckConsistency(*h_RPConv_afterCut, *h_RPConv)) {
    std::cout << "  ERROR: Histograms are inconsistent for TEfficiency object initialization." << std::endl;
    gApplication->Terminate();
  }

  TEfficiency *pEff_SB = new TEfficiency(*h_SB_afterCut, *h_SB);
  TEfficiency *pEff_S = new TEfficiency(*h_S_afterCut, *h_S);
  TEfficiency *pEff_CombiWithConvLeg = new TEfficiency(*h_CombiWithConvLeg_afterCut, *h_CombiWithConvLeg);
  TEfficiency *pEff_CombiWithoutConvLeg = new TEfficiency(*h_CombiWithoutConvLeg_afterCut, *h_CombiWithoutConvLeg);
  TEfficiency *pEff_HF = new TEfficiency(*h_HF_afterCut, *h_HF);
  TEfficiency *pEff_RPConv = new TEfficiency(*h_RPConv_afterCut, *h_RPConv);
  
  
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

  h_SB_afterCut->SetLineWidth(lineWidth);
  h_S_afterCut->SetLineWidth(lineWidth);
  h_CombiWithConvLeg_afterCut->SetLineWidth(lineWidth);
  h_CombiWithoutConvLeg_afterCut->SetLineWidth(lineWidth);
  h_HF_afterCut->SetLineWidth(lineWidth);
  h_RPConv_afterCut->SetLineWidth(lineWidth);

  h_SB_afterCut->SetLineColor(kBlack);
  h_SB_afterCut->SetMarkerColor(kBlack);
  h_S_afterCut->SetLineColor(kGreen+1);
  h_S_afterCut->SetMarkerColor(kGreen+1);
  h_CombiWithConvLeg_afterCut->SetLineColor(kRed);
  h_CombiWithConvLeg_afterCut->SetMarkerColor(kRed);
  h_CombiWithoutConvLeg_afterCut->SetLineColor(kBlue);
  h_CombiWithoutConvLeg_afterCut->SetMarkerColor(kBlue);
  h_HF_afterCut->SetLineColor(kOrange);
  h_HF_afterCut->SetMarkerColor(kOrange);
  h_RPConv_afterCut->SetLineColor(kMagenta);
  h_RPConv_afterCut->SetMarkerColor(kMagenta);

  Int_t markerStyle = 20;
  h_SB_afterCut->SetMarkerStyle(markerStyle);
  h_S_afterCut->SetMarkerStyle(markerStyle);
  h_CombiWithConvLeg_afterCut->SetMarkerStyle(markerStyle);
  h_CombiWithoutConvLeg_afterCut->SetMarkerStyle(markerStyle);
  h_HF_afterCut->SetMarkerStyle(markerStyle);
  h_RPConv_afterCut->SetMarkerStyle(markerStyle);

  Float_t markerSize = 1;
  h_SB_afterCut->SetMarkerSize(markerSize);
  h_S_afterCut->SetMarkerSize(markerSize);
  h_CombiWithConvLeg_afterCut->SetMarkerSize(markerSize);
  h_CombiWithoutConvLeg_afterCut->SetMarkerSize(markerSize);
  h_HF->SetMarkerSize(markerSize);
  h_RPConv_afterCut->SetMarkerSize(markerSize);

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

  TString drawOptions_mass_afterCut = "e1 x0 same";

  TCanvas *c = new TCanvas("c","",1024,768);
  c->SetLogy();
  h_SB->Draw(drawOptions_mass);
  h_S->Draw(drawOptions_mass_same);
  h_CombiWithConvLeg->Draw(drawOptions_mass_same);
  h_CombiWithoutConvLeg->Draw(drawOptions_mass_same);
  h_HF->Draw(drawOptions_mass_same);
  h_RPConv->Draw(drawOptions_mass_same);

  h_SB_afterCut->Draw(drawOptions_mass_afterCut);
  h_S_afterCut->Draw(drawOptions_mass_afterCut);
  h_CombiWithConvLeg_afterCut->Draw(drawOptions_mass_afterCut);
  h_CombiWithoutConvLeg_afterCut->Draw(drawOptions_mass_afterCut);
  h_HF_afterCut->Draw(drawOptions_mass_afterCut);
  h_RPConv_afterCut->Draw(drawOptions_mass_afterCut);

  
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

  c->SaveAs(output_prefix + "mass.pdf");
  c->SaveAs(output_prefix + "mass.root");
  c->SaveAs(output_prefix + "mass.png");


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

  c_eff->SaveAs(output_prefix + "mass_eff.pdf");
  c_eff->SaveAs(output_prefix + "mass_eff.root");
  c_eff->SaveAs(output_prefix + "mass_eff.png");



  

  //////////////////////////////////////// SIGNIFICANCE, SIGNAL OVER BACKGROUND:

  TH1D *h_signal_class = new TH1D();
  h_signal_class = (TH1D*)h_S_afterCut->Clone("h_signal_afterCut");
  h_signal_class->Add(h_CombiWithoutConvLeg_afterCut);
  h_signal_class->Add(h_HF_afterCut);
  h_signal_class->Sumw2();

  TH2D *h_signal_class_subsample = new TH2D();
  h_signal_class_subsample = (TH2D*)h_S_afterCut_subsample->Clone("h_signal_class_subsample");
  h_signal_class_subsample->Add(h_CombiWithoutConvLeg_afterCut_subsample);
  h_signal_class_subsample->Add(h_HF_afterCut_subsample);

  TH1D *h_backgr_class = new TH1D();
  h_backgr_class = (TH1D*)h_CombiWithConvLeg_afterCut->Clone("h_backgr_afterCut");
  h_backgr_class->Add(h_RPConv_afterCut);
  h_backgr_class->Sumw2();

  TH2D *h_backgr_class_subsample = new TH2D();
  h_backgr_class_subsample = (TH2D*)h_CombiWithConvLeg_afterCut_subsample->Clone("h_backgr_class_subsample");
  h_backgr_class_subsample->Add(h_RPConv_afterCut_subsample);


  TH1D *h_signal_class_noCuts = new TH1D();
  h_signal_class_noCuts = (TH1D*)h_S->Clone("h_signal_class_noCuts");
  h_signal_class_noCuts->Add(h_CombiWithoutConvLeg);
  h_signal_class_noCuts->Add(h_HF);
  h_signal_class_noCuts->Sumw2();

  TH2D *h_signal_class_noCuts_subsample = new TH2D();
  h_signal_class_noCuts_subsample = (TH2D*)h_S_subsample->Clone("h_signal_class_noCuts_subsample");
  h_signal_class_noCuts_subsample->Add(h_CombiWithoutConvLeg_subsample);
  h_signal_class_noCuts_subsample->Add(h_HF_subsample);

  TH1D *h_backgr_class_noCuts = new TH1D();
  h_backgr_class_noCuts = (TH1D*)h_CombiWithConvLeg->Clone("h_backgr_class_noCuts");
  h_backgr_class_noCuts->Add(h_RPConv);
  h_backgr_class_noCuts->Sumw2();

  TH2D *h_backgr_class_noCuts_subsample = new TH2D();
  h_backgr_class_noCuts_subsample = (TH2D*)h_CombiWithConvLeg_subsample->Clone("h_backgr_class_noCuts_subsample");
  h_backgr_class_noCuts_subsample->Add(h_RPConv_subsample);


  TH1D *h_signal_class_ideal = new TH1D();
  h_signal_class_ideal = (TH1D*)h_S->Clone("h_signal_class_ideal");
  h_signal_class_ideal->Add(h_CombiWithoutConvLeg);
  h_CombiWithoutConvLeg->Add(h_HF);
  h_CombiWithoutConvLeg->Sumw2();

  TH2D *h_signal_class_ideal_subsample = new TH2D();
  h_signal_class_ideal_subsample = (TH2D*)h_S_subsample->Clone("h_signal_class_ideal_subsample");
  h_signal_class_ideal_subsample->Add(h_CombiWithConvLeg_subsample);
  h_signal_class_ideal_subsample->Add(h_HF_subsample);

  TH1D *h_backgr_class_ideal = new TH1D("h_backgr_class_ideal","",nBins,min,max);
  for(Int_t i=1; i<=nBins; i++) h_backgr_class_ideal->SetBinContent(i, 0.);

  TH2D *h_backgr_class_ideal_subsample = new TH2D("h_backgr_class_ideal_subsample","",nBins,min,max,num_subsamples,0,num_subsamples);
  for(Int_t i=1; i<=nBins*num_subsamples; i++) h_backgr_class_ideal_subsample->SetBinContent(i, 0.);


  TH1D *h_signal_exp = new TH1D();
  h_signal_exp = (TH1D*)h_S_afterCut->Clone("h_signal_exp");
  h_signal_exp->Add(h_HF_afterCut);
  h_signal_exp->Sumw2();

  TH2D *h_signal_exp_subsample = new TH2D();
  h_signal_exp_subsample = (TH2D*)h_S_afterCut_subsample->Clone("h_signal_exp_subsample");
  h_signal_exp_subsample->Add(h_HF_afterCut_subsample);

  TH1D *h_backgr_exp = new TH1D();
  h_backgr_exp = (TH1D*)h_CombiWithConvLeg_afterCut->Clone("h_backgr_exp");
  h_backgr_exp->Add(h_CombiWithoutConvLeg_afterCut);
  h_backgr_exp->Add(h_RPConv_afterCut);
  h_backgr_exp->Sumw2();

  TH2D *h_backgr_exp_subsample = new TH2D();
  h_backgr_exp_subsample = (TH2D*)h_CombiWithConvLeg_afterCut_subsample->Clone("h_backgr_exp_subsample");
  h_backgr_exp_subsample->Add(h_CombiWithoutConvLeg_afterCut_subsample);
  h_backgr_exp_subsample->Add(h_RPConv_afterCut_subsample);


  TH1D *h_signal_exp_noCuts = new TH1D();
  h_signal_exp_noCuts = (TH1D*)h_S->Clone("h_signal_exp_noCuts");
  h_signal_exp_noCuts->Add(h_HF);
  h_signal_exp_noCuts->Sumw2();

  TH2D *h_signal_exp_noCuts_subsample = new TH2D();
  h_signal_exp_noCuts_subsample = (TH2D*)h_S_subsample->Clone("h_signal_exp_noCuts_subsample");
  h_signal_exp_noCuts_subsample->Add(h_HF_subsample);

  TH1D *h_backgr_exp_noCuts = new TH1D();
  h_backgr_exp_noCuts = (TH1D*)h_CombiWithConvLeg->Clone("h_backgr_exp_noCuts");
  h_backgr_exp_noCuts->Add(h_CombiWithoutConvLeg);
  h_CombiWithoutConvLeg->Add(h_RPConv);
  h_CombiWithoutConvLeg->Sumw2();

  TH2D *h_backgr_exp_noCuts_subsample = new TH2D();
  h_backgr_exp_noCuts_subsample = (TH2D*)h_CombiWithConvLeg_subsample->Clone("h_backgr_exp_noCuts_subsample");
  h_backgr_exp_noCuts_subsample->Add(h_CombiWithoutConvLeg_subsample);
  h_backgr_exp_noCuts_subsample->Add(h_RPConv_subsample);


  TH1D *h_signal_exp_ideal = new TH1D();
  h_signal_exp_ideal = (TH1D*)h_S->Clone("h_signal_exp_ideal");
  h_signal_exp_ideal->Add(h_HF);
  h_signal_exp_ideal->Sumw2();

  TH2D *h_signal_exp_ideal_subsample = new TH2D();
  h_signal_exp_ideal_subsample = (TH2D*)h_S_subsample->Clone("h_signal_exp_ideal_subsample");
  h_signal_exp_ideal_subsample->Add(h_HF_subsample);

  TH1D *h_backgr_exp_ideal = new TH1D();
  h_backgr_exp_ideal = (TH1D*)h_CombiWithoutConvLeg->Clone("h_backgr_exp_ideal");
  h_backgr_exp_ideal->Sumw2();

  TH2D *h_backgr_exp_ideal_subsample = new TH2D();
  h_backgr_exp_ideal_subsample = (TH2D*)h_CombiWithoutConvLeg_subsample->Clone("h_backgr_exp_ideal_subsample");
  



  TH1D *h_significance_class = new TH1D("h_significance_class", "", nBins, min, max);
  TH1D *h_significance_class_norm = new TH1D("h_significance_class_norm", "", nBins, min, max);
  TH1D *h_significance_class_noCuts = new TH1D("h_significance_class_noCuts", "", nBins, min, max);
  TH1D *h_significance_class_noCuts_norm = new TH1D("h_significance_class_noCuts_norm", "", nBins, min, max);
  TH1D *h_significance_class_ideal = new TH1D("h_significance_class_ideal", "", nBins, min, max);
  TH1D *h_significance_class_ideal_norm = new TH1D("h_significance_class_ideal_norm", "", nBins, min, max);

  TH1D *h_SoverB_class = new TH1D("h_SoverB_class", "", nBins, min, max);
  TH1D *h_SoverB_class_norm = new TH1D("h_SoverB_class_norm", "", nBins, min, max);
  TH1D *h_SoverB_class_noCuts = new TH1D("h_SoverB_class_noCuts", "", nBins, min, max);
  TH1D *h_SoverB_class_noCuts_norm = new TH1D("h_SoverB_class_noCuts_norm", "", nBins, min, max);
  TH1D *h_SoverB_class_ideal = new TH1D("h_SoverB_class_ideal", "", nBins, min, max);

  TH1D *h_significance_exp = new TH1D("h_significance_exp", "", nBins, min, max);
  TH1D *h_significance_exp_norm = new TH1D("h_significance_exp_norm", "", nBins, min, max);
  TH1D *h_significance_exp_noCuts = new TH1D("h_significance_exp_noCuts", "", nBins, min, max);
  TH1D *h_significance_exp_noCuts_norm = new TH1D("h_significance_exp_noCuts_norm", "", nBins, min, max);
  TH1D *h_significance_exp_ideal = new TH1D("h_significance_exp_ideal", "", nBins, min, max);
  TH1D *h_significance_exp_ideal_norm = new TH1D("h_significance_exp_ideal_norm", "", nBins, min, max);

  TH1D *h_SoverB_exp = new TH1D("h_SoverB_exp", "", nBins, min, max);
  TH1D *h_SoverB_exp_norm = new TH1D("h_SoverB_exp_norm", "", nBins, min, max);
  TH1D *h_SoverB_exp_noCuts = new TH1D("h_SoverB_exp_noCuts", "", nBins, min, max);
  TH1D *h_SoverB_exp_noCuts_norm = new TH1D("h_SoverB_exp_noCuts_norm", "", nBins, min, max);
  TH1D *h_SoverB_exp_ideal = new TH1D("h_SoverB_exp_ideal", "", nBins, min, max);
  TH1D *h_SoverB_exp_ideal_norm = new TH1D("h_SoverB_exp_ideal_norm", "", nBins, min, max);


  TH1D *h_error_uncorr_significance_class = new TH1D("h_error_uncorr_significance_class","",nBins,min,max);
  TH1D *h_error_uncorr_significance_class_noCuts = new TH1D("h_error_uncorr_significance_class_noCuts","",nBins,min,max);
  TH1D *h_error_uncorr_significance_class_ideal = new TH1D("h_error_uncorr_significance_class_ideal","",nBins,min,max);
  TH1D *h_error_uncorr_significance_exp = new TH1D("h_error_uncorr_significance_exp","",nBins,min,max);
  TH1D *h_error_uncorr_significance_exp_noCuts = new TH1D("h_error_uncorr_significance_exp_noCuts","",nBins,min,max);
  TH1D *h_error_uncorr_significance_exp_ideal = new TH1D("h_error_uncorr_significance_exp_ideal","",nBins,min,max);

  TH1D *h_error_est_significance_class = new TH1D("h_error_est_significance_class","",nBins,min,max);
  TH1D *h_error_est_significance_class_noCuts = new TH1D("h_error_est_significance_class_noCuts","",nBins,min,max);
  TH1D *h_error_est_significance_class_ideal = new TH1D("h_error_est_significance_class_ideal","",nBins,min,max);
  TH1D *h_error_est_significance_exp = new TH1D("h_error_est_significance_exp","",nBins,min,max);
  TH1D *h_error_est_significance_exp_noCuts = new TH1D("h_error_est_significance_exp_noCuts","",nBins,min,max);
  TH1D *h_error_est_significance_exp_ideal = new TH1D("h_error_est_significance_exp_ideal","",nBins,min,max);
  
  TH1D *h_errorDiff_significance_class = new TH1D("h_errorDiff_significance_class","",nBins,min,max);
  TH1D *h_errorDiff_significance_class_noCuts = new TH1D("h_errorDiff_significance_class_noCuts","",nBins,min,max);
  TH1D *h_errorDiff_significance_class_ideal = new TH1D("h_errorDiff_significance_class_ideal","",nBins,min,max);
  TH1D *h_errorDiff_significance_exp = new TH1D("h_errorDiff_significance_exp","",nBins,min,max);
  TH1D *h_errorDiff_significance_exp_noCuts = new TH1D("h_errorDiff_significance_exp_noCuts","",nBins,min,max);
  TH1D *h_errorDiff_significance_exp_ideal = new TH1D("h_errorDiff_significance_exp_ideal","",nBins,min,max);

  Double_t temp_error_uncorr;


  Double_t significance_class_integral = 0.,
    significance_class_noCuts_integral = 0.,
    significance_class_ideal_integral = 0.;

  Double_t significance_exp_integral = 0.,
    significance_exp_noCuts_integral = 0.,
    significance_exp_ideal_integral = 0.;



  for(Int_t i=1; i<=nBins; i++) {

    // classifier significance
    if((h_signal_class->GetBinContent(i) + h_backgr_class->GetBinContent(i)) != 0) {
      h_significance_class->SetBinContent(i,
					  h_signal_class->GetBinContent(i) /
					  TMath::Sqrt(h_signal_class->GetBinContent(i) +
						      h_backgr_class->GetBinContent(i)));
      h_significance_class->SetBinError(i, significanceError(*h_signal_class_subsample, *h_backgr_class_subsample,
       							     i, num_subsamples));
      significance_class_integral += h_significance_class->GetBinContent(i);
    }else {
      h_significance_class->SetBinContent(i, 0.);
      h_significance_class->SetBinError(i, 0.);
    }

    temp_error_uncorr = significanceError_uncorrelated(h_signal_class->GetBinContent(i),
						       h_backgr_class->GetBinContent(i));
    if(temp_error_uncorr != 0) {
      h_errorDiff_significance_class->SetBinContent(i,
						    (temp_error_uncorr -
						     h_significance_class->GetBinError(i)) /
						    temp_error_uncorr);
    }
    h_error_est_significance_class->SetBinContent(i, h_significance_class->GetBinError(i));
    h_error_uncorr_significance_class->SetBinContent(i, temp_error_uncorr);

    
    if((h_signal_class_noCuts->GetBinContent(i) + h_backgr_class_noCuts->GetBinContent(i)) != 0) {
      h_significance_class_noCuts->SetBinContent(i,
						 h_signal_class_noCuts->GetBinContent(i) /
						 TMath::Sqrt(h_signal_class_noCuts->GetBinContent(i) +
							     h_backgr_class_noCuts->GetBinContent(i)));
      h_significance_class_noCuts->SetBinError(i, significanceError(*h_signal_class_noCuts_subsample, *h_backgr_class_noCuts_subsample,
								    i, num_subsamples));
      significance_class_noCuts_integral += h_significance_class_noCuts->GetBinContent(i);
    }else {
      h_significance_class_noCuts->SetBinContent(i, 0.);
      h_significance_class_noCuts->SetBinContent(i, 0.);
    }

    temp_error_uncorr = significanceError_uncorrelated(h_signal_class_noCuts->GetBinContent(i),
						       h_backgr_class_noCuts->GetBinContent(i));
    if(temp_error_uncorr != 0) {
      h_errorDiff_significance_class_noCuts->SetBinContent(i,
							   (temp_error_uncorr -
							    h_significance_class_noCuts->GetBinError(i)) /
							   temp_error_uncorr);
    }
    h_error_est_significance_class_noCuts->SetBinContent(i, h_significance_class_noCuts->GetBinError(i));
    h_error_uncorr_significance_class_noCuts->SetBinContent(i, temp_error_uncorr);
    

    h_significance_class_ideal->SetBinContent(i, TMath::Sqrt(h_signal_class_ideal->GetBinContent(i)));
    h_significance_class_ideal->SetBinError(i, significanceError(*h_signal_class_ideal_subsample, *h_backgr_class_ideal_subsample,
								 i, num_subsamples));
    significance_class_ideal_integral += h_significance_class_ideal->GetBinContent(i);

    temp_error_uncorr = significanceError_uncorrelated(h_signal_class_ideal->GetBinContent(i),
						       h_backgr_class_ideal->GetBinContent(i));
    if(temp_error_uncorr != 0) {
      h_errorDiff_significance_class_ideal->SetBinContent(i,
							  (temp_error_uncorr -
							   h_significance_class_ideal->GetBinError(i)) /
							  temp_error_uncorr);
    }
    h_error_est_significance_class_ideal->SetBinContent(i, h_significance_class_ideal->GetBinError(i));
    h_error_uncorr_significance_class_ideal->SetBinContent(i, temp_error_uncorr);

    
    // experimental significance
    if((h_signal_exp->GetBinContent(i) + h_backgr_exp->GetBinContent(i)) != 0) {
      h_significance_exp->SetBinContent(i,
					h_signal_exp->GetBinContent(i) /
					TMath::Sqrt(h_signal_exp->GetBinContent(i) +
						    h_backgr_exp->GetBinContent(i)));
      h_significance_exp->SetBinError(i, significanceError(*h_signal_exp_subsample, *h_backgr_exp_subsample,
							   i, num_subsamples));
      significance_exp_integral += h_significance_exp->GetBinContent(i);
    }else {
      h_significance_exp->SetBinContent(i, 0.);
      h_significance_exp->SetBinError(i, 0.);
    }

    temp_error_uncorr = significanceError_uncorrelated(h_signal_exp->GetBinContent(i),
						       h_backgr_exp->GetBinContent(i));
    if(temp_error_uncorr != 0) {
      h_errorDiff_significance_exp->SetBinContent(i,
						  (temp_error_uncorr -
						   h_significance_exp->GetBinError(i)) /
						  temp_error_uncorr);
    }
    h_error_est_significance_exp->SetBinContent(i, h_significance_exp->GetBinError(i));
    h_error_uncorr_significance_exp->SetBinContent(i, temp_error_uncorr);
    

    if((h_signal_exp_noCuts->GetBinContent(i) + h_backgr_exp_noCuts->GetBinContent(i))!= 0) {
      h_significance_exp_noCuts->SetBinContent(i,
					       h_signal_exp_noCuts->GetBinContent(i) /
					       TMath::Sqrt(h_signal_exp_noCuts->GetBinContent(i) +
							   h_backgr_exp_noCuts->GetBinContent(i)));
      h_significance_exp_noCuts->SetBinError(i, significanceError(*h_signal_exp_noCuts_subsample, *h_backgr_exp_noCuts_subsample,
								  i, num_subsamples));
      significance_exp_noCuts_integral += h_significance_exp_noCuts->GetBinContent(i);
    }else {
      h_significance_exp_noCuts->SetBinContent(i, 0.);
      h_significance_exp_noCuts->SetBinError(i, 0.);
    }

    temp_error_uncorr = significanceError_uncorrelated(h_signal_exp_noCuts->GetBinContent(i),
						       h_backgr_exp_noCuts->GetBinContent(i));
    if(temp_error_uncorr != 0) {
      h_errorDiff_significance_exp_noCuts->SetBinContent(i,
							 (temp_error_uncorr -
							  h_significance_exp_noCuts->GetBinError(i)) /
							 temp_error_uncorr);
    }
    h_error_est_significance_exp_noCuts->SetBinContent(i, h_significance_exp_noCuts->GetBinError(i));
    h_error_uncorr_significance_exp_noCuts->SetBinContent(i, temp_error_uncorr);
    

    if((h_signal_exp_ideal->GetBinContent(i) + h_backgr_exp_ideal->GetBinContent(i)) != 0) {
      h_significance_exp_ideal->SetBinContent(i,
					      h_signal_exp_ideal->GetBinContent(i) /
					      TMath::Sqrt(h_signal_exp_ideal->GetBinContent(i) +
							  h_backgr_exp_ideal->GetBinContent(i)));
      h_significance_exp_ideal->SetBinError(i, significanceError(*h_signal_exp_ideal_subsample, *h_backgr_exp_ideal_subsample,
								 i, num_subsamples));
      significance_exp_ideal_integral += h_significance_exp_ideal->GetBinContent(i);
    }else {
      h_significance_exp_ideal->SetBinContent(i, 0.);
      h_significance_exp_ideal->SetBinError(i, 0.);
    }

    temp_error_uncorr = significanceError_uncorrelated(h_signal_exp_ideal->GetBinContent(i),
						       h_backgr_exp_ideal->GetBinContent(i));
    if(temp_error_uncorr != 0) {
      h_errorDiff_significance_exp_ideal->SetBinContent(i,
							(temp_error_uncorr -
							 h_significance_exp_ideal->GetBinError(i)) /
							temp_error_uncorr);
    }
    h_error_est_significance_exp_ideal->SetBinContent(i, h_significance_exp_ideal->GetBinError(i));
    h_error_uncorr_significance_exp_ideal->SetBinContent(i, temp_error_uncorr);


    // classifier S/B
    if(h_backgr_class->GetBinContent(i) != 0) {
      h_SoverB_class->SetBinContent(i,
				    h_signal_class->GetBinContent(i) /
				    h_backgr_class->GetBinContent(i));
      h_SoverB_class->SetBinError(i, SoverBError(h_signal_class->GetBinContent(i),
						 h_backgr_class->GetBinContent(i)));
    }else {
      h_SoverB_class->SetBinContent(i, 0.);
      h_SoverB_class->SetBinError(i, 0.);
    }

    if(h_backgr_class_noCuts->GetBinContent(i) != 0) {
      h_SoverB_class_noCuts->SetBinContent(i,
					   h_signal_class_noCuts->GetBinContent(i) /
					   h_backgr_class_noCuts->GetBinContent(i));
      h_SoverB_class_noCuts->SetBinError(i, SoverBError(h_signal_class_noCuts->GetBinContent(i),
							h_backgr_class_noCuts->GetBinContent(i)));
    }else {
      h_SoverB_class_noCuts->SetBinContent(i, 0.);
      h_SoverB_class_noCuts->SetBinError(i, 0.);
    }

    h_SoverB_class_ideal->SetBinContent(i, h_signal_class_ideal->GetBinContent(i));
    h_SoverB_class_ideal->SetBinError(i, 0.);


    // experimental S/B
    if(h_backgr_exp->GetBinContent(i) != 0) {
      h_SoverB_exp->SetBinContent(i,
				  h_signal_exp->GetBinContent(i) /
				  h_backgr_exp->GetBinContent(i));
      h_SoverB_exp->SetBinError(i, SoverBError(h_signal_exp->GetBinContent(i),
					       h_backgr_exp->GetBinContent(i)));
    }else {
      h_SoverB_exp->SetBinContent(i, 0.);
      h_SoverB_exp->SetBinError(i, 0.);
    }

    if(h_backgr_exp_noCuts->GetBinContent(i) != 0) {
      h_SoverB_exp_noCuts->SetBinContent(i,
					 h_signal_exp_noCuts->GetBinContent(i) /
					 h_backgr_exp_noCuts->GetBinContent(i));
      h_SoverB_exp_noCuts->SetBinError(i, SoverBError(h_signal_exp_noCuts->GetBinContent(i),
						      h_backgr_exp_noCuts->GetBinContent(i)));
    }else {
      h_SoverB_exp_noCuts->SetBinContent(i, 0.);
      h_SoverB_exp_noCuts->SetBinError(i, 0.);
    }

    if(h_backgr_exp_ideal->GetBinContent(i) != 0) {
      h_SoverB_exp_ideal->SetBinContent(i,
					h_signal_exp_ideal->GetBinContent(i) /
					h_backgr_exp_ideal->GetBinContent(i));
      h_backgr_exp_ideal->SetBinError(i, SoverBError(h_signal_exp_ideal->GetBinContent(i),
						     h_backgr_exp_ideal->GetBinContent(i)));
    }else {
      h_SoverB_exp_ideal->SetBinContent(i, 0.);
      h_SoverB_exp_ideal->SetBinError(i, 0.);
    }
    
  }


  // create ratio histograms
  h_significance_class_norm = (TH1D*)h_significance_class->Clone("h_significance_class_norm");
  h_significance_class_norm->Sumw2();
  h_significance_class_noCuts->Sumw2();
  h_significance_class_norm->Divide(h_significance_class_noCuts);

  h_significance_exp_norm = (TH1D*)h_significance_exp->Clone("h_significance_exp_norm");
  h_significance_exp_norm->Sumw2();
  h_significance_exp_noCuts->Sumw2();
  h_significance_exp_norm->Divide(h_significance_exp_noCuts);

  h_SoverB_class_norm = (TH1D*)h_SoverB_exp->Clone("h_SoverB_class_norm");
  h_SoverB_class_norm->Sumw2();
  h_SoverB_class_noCuts->Sumw2();
  h_SoverB_class_norm->Divide(h_SoverB_class_noCuts);

  h_SoverB_exp_norm = (TH1D*)h_SoverB_exp->Clone("h_SoverB_exp_norm");
  h_SoverB_exp_norm->Sumw2();
  h_SoverB_exp_noCuts->Sumw2();
  h_SoverB_exp_norm->Divide(h_SoverB_exp_noCuts);


  const Int_t kColor_significance = kBlack;
  const Int_t kColor_significance_noCuts = kGray+2;
  const Int_t kColor_significance_ideal = kGray+2;

  const Int_t kMarkerStyle_significance = 20;
  const Int_t kMarkerStyle_significance_noCuts = 24;
  const Int_t kMarkerStyle_significance_ideal = 5;

  const Int_t kMarkerSize_significance = 1;
  const Int_t kMarkerSize_significance_noCuts = 1;
  const Int_t kMarkerSize_significance_ideal = 1;

  const Int_t kMarkerStyle_SoverB = 20;

  const Int_t kMarkerSize_SoverB = 1;
  
  h_significance_class->SetLineColor(kColor_significance);
  h_significance_class->SetMarkerColor(kColor_significance);
  h_significance_class_noCuts->SetLineColor(kColor_significance_noCuts);
  h_significance_class_noCuts->SetMarkerColor(kColor_significance_noCuts);
  h_significance_class_ideal->SetLineColor(kColor_significance_ideal);
  h_significance_class_ideal->SetMarkerColor(kColor_significance_ideal);

  h_significance_exp->SetLineColor(kColor_significance);
  h_significance_exp->SetMarkerColor(kColor_significance);
  h_significance_exp_noCuts->SetLineColor(kColor_significance_noCuts);
  h_significance_exp_noCuts->SetMarkerColor(kColor_significance_noCuts);
  h_significance_exp_ideal->SetLineColor(kColor_significance_ideal);
  h_significance_exp_ideal->SetMarkerColor(kColor_significance_ideal);

  h_significance_class->SetMarkerStyle(kMarkerStyle_significance);
  h_significance_class_noCuts->SetMarkerStyle(kMarkerStyle_significance_noCuts);
  h_significance_class_ideal->SetMarkerStyle(kMarkerStyle_significance_ideal);

  h_significance_class->SetMarkerSize(kMarkerSize_significance);
  h_significance_class_noCuts->SetMarkerSize(kMarkerSize_significance_noCuts);
  h_significance_class_ideal->SetMarkerSize(kMarkerSize_significance_ideal);

  h_significance_exp->SetMarkerStyle(kMarkerStyle_significance);
  h_significance_exp_noCuts->SetMarkerStyle(kMarkerStyle_significance_noCuts);
  h_significance_exp_ideal->SetMarkerStyle(kMarkerStyle_significance_ideal);

  h_significance_exp->SetMarkerSize(kMarkerSize_significance);
  h_significance_exp_noCuts->SetMarkerSize(kMarkerSize_significance_noCuts);
  h_significance_exp_ideal->SetMarkerSize(kMarkerSize_significance_ideal);


  h_SoverB_class->SetMarkerStyle(kMarkerStyle_SoverB);

  h_SoverB_class->SetMarkerSize(kMarkerSize_SoverB);

  h_SoverB_exp->SetMarkerStyle(kMarkerStyle_SoverB);

  h_SoverB_exp->SetMarkerSize(kMarkerSize_SoverB);



  TString XTitle = "#it{m}_{ee} / (GeV/#it{c}^{2})";
  
  TString YTitle_significance_class = "#it{S}_{class} / #sqrt{#it{S}_{class}+#it{B}_{class}}";
  TString YTitle_significance_class_noCuts = YTitle_significance_class;
  TString YTitle_significance_class_ideal = YTitle_significance_class;
  
  TString YTitle_significance_exp = "#it{S}_{exp} / #sqrt{#it{S}_{exp}+#it{B}_{exp}}";
  TString YTitle_significance_exp_noCuts = YTitle_significance_exp;
  TString YTitle_significance_exp_ideal = YTitle_significance_exp;

  TString YTitle_SoverB_class = "#it{S}_{class}/#it{B}_{class}";
  TString YTitle_SoverB_class_noCuts = YTitle_SoverB_class;
  TString YTitle_SoverB_class_ideal = YTitle_SoverB_class;

  TString YTitle_SoverB_exp = "#it{S}_{exp}/#it{B}_{exp}";
  TString YTitle_SoverB_exp_noCuts = YTitle_SoverB_exp;
  TString YTitle_SoverB_exp_ideal = YTitle_SoverB_exp;
  
  h_significance_class->SetXTitle(XTitle);
  h_significance_class->SetYTitle(YTitle_significance_class);

  h_significance_class_noCuts->SetXTitle(XTitle);
  h_significance_class_noCuts->SetYTitle(YTitle_significance_class_noCuts);

  h_significance_class_ideal->SetXTitle(XTitle);
  h_significance_class_ideal->SetYTitle(YTitle_significance_class_ideal);

  h_significance_exp->SetXTitle(XTitle);
  h_significance_exp->SetYTitle(YTitle_significance_exp);

  h_significance_exp_noCuts->SetXTitle(XTitle);
  h_significance_exp_noCuts->SetYTitle(YTitle_significance_exp_noCuts);

  h_significance_exp_ideal->SetXTitle(XTitle);
  h_significance_exp_ideal->SetYTitle(YTitle_significance_exp_ideal);

  h_SoverB_class->SetXTitle(XTitle);
  h_SoverB_class->SetYTitle(YTitle_SoverB_class);

  h_SoverB_class_noCuts->SetXTitle(XTitle);
  h_SoverB_class_noCuts->SetYTitle(YTitle_SoverB_class_noCuts);

  h_SoverB_class_ideal->SetXTitle(XTitle);
  h_SoverB_class_ideal->SetYTitle(YTitle_SoverB_class_ideal);

  h_SoverB_exp->SetXTitle(XTitle);
  h_SoverB_exp->SetYTitle(YTitle_SoverB_exp);

  h_SoverB_exp_noCuts->SetXTitle(XTitle);
  h_SoverB_exp_noCuts->SetYTitle(YTitle_SoverB_exp_noCuts);

  h_SoverB_exp_ideal->SetXTitle(XTitle);
  h_SoverB_exp_ideal->SetYTitle(YTitle_SoverB_exp_ideal);



  TString leg_baselabel_class = "MVA cut";
  TString leg_label_class = leg_baselabel_class;
  leg_label_class += " (sign. int. = ";
  leg_label_class += significance_class_integral;
  leg_label_class += ")";

  TString leg_baselabel_class_noCuts = "no backgr. rej.";
  TString leg_label_class_noCuts = leg_baselabel_class_noCuts;
  leg_label_class_noCuts += " (sign. int. = ";
  leg_label_class_noCuts += significance_class_noCuts_integral;
  leg_label_class_noCuts += ")";

  TString leg_baselabel_class_ideal = "ideal backgr. rej.";
  TString leg_label_class_ideal = leg_baselabel_class_ideal;
  leg_label_class_ideal += " (sign. int. = ";
  leg_label_class_ideal += significance_class_ideal_integral;
  leg_label_class_ideal += ")";

  TString leg_baselabel_exp = leg_baselabel_class;
  TString leg_label_exp = leg_baselabel_exp;
  leg_label_exp += " (sign. int. = ";
  leg_label_exp += significance_exp_integral;
  leg_label_exp += ")";

  TString leg_baselabel_exp_noCuts = leg_baselabel_class_noCuts;
  TString leg_label_exp_noCuts = leg_baselabel_exp_noCuts;
  leg_label_exp_noCuts += " (sign. int. = ";
  leg_label_exp_noCuts += significance_exp_noCuts_integral;
  leg_label_exp_noCuts += ")";

  TString leg_baselabel_exp_ideal = leg_baselabel_class_ideal;
  TString leg_label_exp_ideal = leg_baselabel_exp_ideal;
  leg_label_exp_ideal += " (sign. int. = ";
  leg_label_exp_ideal += significance_exp_ideal_integral;
  leg_label_exp_ideal += ")";


  TLegend *leg_significance_class = new TLegend(.5, .7,.95, .95);
  leg_significance_class->AddEntry(h_significance_class, leg_label_class, "p");
  leg_significance_class->AddEntry(h_significance_class_noCuts, leg_label_class_noCuts, "p");
  leg_significance_class->AddEntry(h_significance_class_ideal, leg_label_class_ideal, "p");

  TLegend *leg_significance_exp = new TLegend(.5, .7, .95, .95);
  leg_significance_exp->AddEntry(h_significance_exp, leg_label_exp, "p");
  leg_significance_exp->AddEntry(h_significance_exp_noCuts, leg_label_exp_noCuts, "p");
  leg_significance_exp->AddEntry(h_significance_exp_ideal, leg_label_exp_ideal, "p");

  TLegend *leg_SoverB_class = new TLegend(.6, .75, .95, .95);
  leg_SoverB_class->AddEntry(h_SoverB_class, leg_label_class, "p");
  leg_SoverB_class->AddEntry(h_SoverB_class_noCuts, leg_label_class_noCuts, "p");
  leg_SoverB_class->AddEntry(h_SoverB_class_ideal, leg_label_class_ideal, "p");

  TLegend *leg_SoverB_exp = new TLegend(.6, .75, .95, .95);
  leg_SoverB_exp->AddEntry(h_SoverB_exp, leg_label_exp, "p");
  leg_SoverB_exp->AddEntry(h_SoverB_exp_noCuts, leg_label_exp_noCuts, "p");
  leg_SoverB_exp->AddEntry(h_SoverB_exp_ideal, leg_label_exp_ideal, "p");
  


  TString drawOptions_significance_class = "p0 x0";

  TCanvas *c_significance_class = new TCanvas("c_significance_class", "", 1024, 768);
  h_significance_class->Draw(drawOptions_significance_class);
  h_significance_class_noCuts->Draw(drawOptions_significance_class + " same");
  h_significance_class_ideal->Draw(drawOptions_significance_class + " same");
  leg_significance_class->Draw();

  c_significance_class->SaveAs(output_prefix + "mass_significance_class.pdf");
  c_significance_class->SaveAs(output_prefix + "mass_significance_class.png");
  c_significance_class->SaveAs(output_prefix + "mass_significance_class.root");


  TString drawOptions_significance_class_norm = "p0 e1 x0";

  TCanvas *c_significance_class_norm = new TCanvas("c_significance_class_norm", "", 1024, 768);
  h_significance_class_norm->Draw(drawOptions_significance_class_norm);

  c_significance_class_norm->SaveAs(output_prefix + "mass_significance_class_norm.pdf");
  c_significance_class_norm->SaveAs(output_prefix + "mass_significance_class_norm.png");
  c_significance_class_norm->SaveAs(output_prefix + "mass_significance_class_norm.root");


  TString drawOptions_significance_exp = "p0 x0";

  TCanvas *c_significance_exp = new TCanvas("c_significance_exp", "", 1024, 768);
  c_significance_exp->SetLogy();
  h_significance_exp->Draw(drawOptions_significance_exp);
  h_significance_exp_noCuts->Draw(drawOptions_significance_exp + " same");
  h_significance_exp_ideal->Draw(drawOptions_significance_exp + " same");
  leg_significance_exp->Draw();

  c_significance_exp->SaveAs(output_prefix + "mass_significance_exp.pdf");
  c_significance_exp->SaveAs(output_prefix + "mass_significance_exp.png");
  c_significance_exp->SaveAs(output_prefix + "mass_significance_exp.root");


  TString drawOptions_significance_exp_norm = "p0 x0";

  TCanvas *c_significance_exp_norm = new TCanvas("c_significance_exp_norm", "", 1024, 768);
  h_significance_exp_norm->Draw(drawOptions_significance_exp_norm);

  c_significance_exp_norm->SaveAs(output_prefix + "mass_significance_exp_norm.pdf");
  c_significance_exp_norm->SaveAs(output_prefix + "mass_significance_exp_norm.png");
  c_significance_exp_norm->SaveAs(output_prefix + "mass_significance_exp_norm.root");


  TString drawOptions_SoverB_class = "p0 x0";

  TCanvas *c_SoverB_class = new TCanvas("c_SoverB_class", "", 1024, 768);
  h_SoverB_class->Draw(drawOptions_SoverB_class);
  h_SoverB_class_noCuts->Draw(drawOptions_SoverB_class + " same");
  h_SoverB_class_ideal->Draw(drawOptions_SoverB_class + " same");
  leg_SoverB_class->Draw();

  c_SoverB_class->SaveAs(output_prefix + "mass_SoverB_class.pdf");
  c_SoverB_class->SaveAs(output_prefix + "mass_SoverB_class.png");
  c_SoverB_class->SaveAs(output_prefix + "mass_SoverB_class.root");
  

  TString drawOptions_SoverB_class_norm = "p0 e1 x0";

  TCanvas *c_SoverB_class_norm = new TCanvas("c_SoverB_class_norm", "", 1024, 768);
  h_SoverB_class_norm->Draw(drawOptions_SoverB_class_norm);

  c_SoverB_class_norm->SaveAs(output_prefix + "mass_SoverB_class_norm.pdf");
  c_SoverB_class_norm->SaveAs(output_prefix + "mass_SoverB_class_norm.png");
  c_SoverB_class_norm->SaveAs(output_prefix + "mass_SoverB_class_norm.root");


  TString drawOptions_SoverB_exp = "p0 x0";

  TCanvas *c_SoverB_exp = new TCanvas("c_SoverB_exp", "", 1024, 768);
  c_SoverB_exp->SetLogy();
  h_SoverB_exp->Draw(drawOptions_SoverB_exp);
  h_SoverB_exp_noCuts->Draw(drawOptions_SoverB_exp + " same");
  h_SoverB_exp_ideal->Draw(drawOptions_SoverB_exp + " same");
  leg_SoverB_exp->Draw();

  c_SoverB_exp->SaveAs(output_prefix + "mass_SoverB_exp.pdf");
  c_SoverB_exp->SaveAs(output_prefix + "mass_SoverB_exp.png");
  c_SoverB_exp->SaveAs(output_prefix + "mass_SoverB_exp.root");

  TString drawOptions_SoverB_exp_norm = "p0 e1 x0";

  TCanvas *c_SoverB_exp_norm = new TCanvas("c_SoverB_exp_norm", "", 1024, 768);
  h_SoverB_exp_norm->Draw(drawOptions_SoverB_exp_norm);

  c_SoverB_exp_norm->SaveAs(output_prefix + "mass_SoverB_exp_norm.pdf");
  c_SoverB_exp_norm->SaveAs(output_prefix + "mass_SoverB_exp_norm.png");
  c_SoverB_exp_norm->SaveAs(output_prefix + "mass_SoverB_exp_norm.root");

  

  
  

  // store all created histograms in a ROOT file:
  TFile *outfile = new TFile(outfileName, "RECREATE");

  h_SB->Write(0, TObject::kOverwrite);
  h_S->Write(0, TObject::kOverwrite);
  h_CombiWithConvLeg->Write(0, TObject::kOverwrite);
  h_CombiWithoutConvLeg->Write(0, TObject::kOverwrite);
  h_HF->Write(0, TObject::kOverwrite);
  h_RPConv->Write(0, TObject::kOverwrite);

  h_SB_afterCut->Write(0, TObject::kOverwrite);
  h_S_afterCut->Write(0, TObject::kOverwrite);
  h_CombiWithConvLeg_afterCut->Write(0, TObject::kOverwrite);
  h_CombiWithoutConvLeg_afterCut->Write(0, TObject::kOverwrite);
  h_HF_afterCut->Write(0, TObject::kOverwrite);
  h_RPConv_afterCut->Write(0, TObject::kOverwrite);

  h_SB_afterCut_subsample->Write(0, TObject::kOverwrite);
  h_S_afterCut_subsample->Write(0, TObject::kOverwrite);
  h_CombiWithConvLeg_afterCut_subsample->Write(0, TObject::kOverwrite);
  h_CombiWithoutConvLeg_afterCut_subsample->Write(0, TObject::kOverwrite);
  h_HF_afterCut_subsample->Write(0, TObject::kOverwrite);
  h_RPConv_afterCut_subsample->Write(0, TObject::kOverwrite);
  
  h_SB_eff->Write(0, TObject::kOverwrite);
  h_S_eff->Write(0, TObject::kOverwrite);
  h_CombiWithConvLeg_eff->Write(0, TObject::kOverwrite);
  h_CombiWithoutConvLeg_eff->Write(0, TObject::kOverwrite);
  h_HF_eff->Write(0, TObject::kOverwrite);
  h_RPConv_eff->Write(0, TObject::kOverwrite);

  h_signal_class->Write(0, TObject::kOverwrite);
  h_signal_class_noCuts->Write(0, TObject::kOverwrite);
  h_signal_class_ideal->Write(0, TObject::kOverwrite);
  h_backgr_class->Write(0, TObject::kOverwrite);
  h_backgr_class_noCuts->Write(0, TObject::kOverwrite);

  h_signal_exp->Write(0, TObject::kOverwrite);
  h_signal_exp_noCuts->Write(0, TObject::kOverwrite);
  h_signal_exp_ideal->Write(0, TObject::kOverwrite);
  h_backgr_exp->Write(0, TObject::kOverwrite);
  h_backgr_exp_noCuts->Write(0, TObject::kOverwrite);
  h_backgr_exp_ideal->Write(0, TObject::kOverwrite);

  h_significance_class->Write(0, TObject::kOverwrite);
  h_significance_class_norm->Write(0, TObject::kOverwrite);
  h_significance_class_noCuts->Write(0, TObject::kOverwrite);
  h_significance_class_noCuts_norm->Write(0, TObject::kOverwrite);
  h_significance_class_ideal->Write(0, TObject::kOverwrite);
  h_significance_class_ideal_norm->Write(0, TObject::kOverwrite);

  h_significance_exp->Write(0, TObject::kOverwrite);
  h_significance_exp_norm->Write(0, TObject::kOverwrite);
  h_significance_exp_noCuts->Write(0, TObject::kOverwrite);
  h_significance_exp_noCuts_norm->Write(0, TObject::kOverwrite);
  h_significance_exp_ideal->Write(0, TObject::kOverwrite);
  h_significance_exp_ideal_norm->Write(0, TObject::kOverwrite);

  h_SoverB_class->Write(0, TObject::kOverwrite);
  h_SoverB_class_norm->Write(0, TObject::kOverwrite);
  h_SoverB_class_noCuts->Write(0, TObject::kOverwrite);
  h_SoverB_class_noCuts_norm->Write(0, TObject::kOverwrite);
  h_SoverB_class_ideal->Write(0, TObject::kOverwrite);

  h_SoverB_exp->Write(0, TObject::kOverwrite);
  h_SoverB_exp_norm->Write(0, TObject::kOverwrite);
  h_SoverB_exp_noCuts->Write(0, TObject::kOverwrite);
  h_SoverB_exp_noCuts_norm->Write(0, TObject::kOverwrite);
  h_SoverB_exp_ideal->Write(0, TObject::kOverwrite);
  h_SoverB_exp_ideal_norm->Write(0, TObject::kOverwrite);

  h_signal_class_subsample->Write(0, TObject::kOverwrite);
  h_signal_class_noCuts_subsample->Write(0, TObject::kOverwrite);
  h_signal_class_ideal_subsample->Write(0, TObject::kOverwrite);
  h_signal_exp_subsample->Write(0, TObject::kOverwrite);
  h_signal_exp_noCuts_subsample->Write(0, TObject::kOverwrite);
  h_signal_exp_ideal_subsample->Write(0, TObject::kOverwrite);

  h_backgr_class_subsample->Write(0, TObject::kOverwrite);
  h_backgr_class_noCuts_subsample->Write(0, TObject::kOverwrite);
  h_backgr_class_ideal_subsample->Write(0, TObject::kOverwrite);
  h_backgr_exp_subsample->Write(0, TObject::kOverwrite);
  h_backgr_exp_noCuts_subsample->Write(0, TObject::kOverwrite);
  h_backgr_exp_ideal_subsample->Write(0, TObject::kOverwrite);

  h_error_est_significance_class->Write(0, TObject::kOverwrite);
  h_error_est_significance_class_noCuts->Write(0, TObject::kOverwrite);
  h_error_est_significance_class_ideal->Write(0, TObject::kOverwrite);
  h_error_est_significance_exp->Write(0, TObject::kOverwrite);
  h_error_est_significance_exp_noCuts->Write(0, TObject::kOverwrite);
  h_error_est_significance_exp_ideal->Write(0, TObject::kOverwrite);

  h_error_uncorr_significance_class->Write(0, TObject::kOverwrite);
  h_error_uncorr_significance_class_noCuts->Write(0, TObject::kOverwrite);
  h_error_uncorr_significance_class_ideal->Write(0, TObject::kOverwrite);
  h_error_uncorr_significance_exp->Write(0, TObject::kOverwrite);
  h_error_uncorr_significance_exp_noCuts->Write(0, TObject::kOverwrite);
  h_error_uncorr_significance_exp_ideal->Write(0, TObject::kOverwrite);

  h_errorDiff_significance_class->Write(0, TObject::kOverwrite);
  h_errorDiff_significance_class_noCuts->Write(0, TObject::kOverwrite);
  h_errorDiff_significance_class_ideal->Write(0, TObject::kOverwrite);
  h_errorDiff_significance_exp->Write(0, TObject::kOverwrite);
  h_errorDiff_significance_exp_noCuts->Write(0, TObject::kOverwrite);
  h_errorDiff_significance_exp_ideal->Write(0, TObject::kOverwrite);
  
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


Float_t significanceError(TH2D &signal_subsamples, TH2D &backgr_subsamples,
			  Int_t bin_mass, Int_t num_subsamples) {
  
  Float_t significance_subsamples[num_subsamples];
  
  for(Int_t i=0; i<num_subsamples; i++) {
    significance_subsamples[i] =
      (signal_subsamples.GetBinContent(bin_mass, i+1) + backgr_subsamples.GetBinContent(bin_mass, i+1))==0 ?
      -1. : signal_subsamples.GetBinContent(bin_mass, i+1) /
      TMath::Sqrt(signal_subsamples.GetBinContent(bin_mass, i+1) +
		  backgr_subsamples.GetBinContent(bin_mass, i+1));
  }

  Float_t mean=0., stddev=0.;
  UInt_t cnt_invalidEntries = 0;
  for(Int_t i=0; i<num_subsamples; i++) {
    if(significance_subsamples[i] != -1) {
      mean += significance_subsamples[i];
    }else {
      cnt_invalidEntries++;
    }
  }
  mean /= num_subsamples - cnt_invalidEntries;

  for(Int_t i=0; i<num_subsamples; i++) {
    if(significance_subsamples[i] != -1) {
      stddev += (significance_subsamples[i]-mean)*(significance_subsamples[i]-mean);
    }
  }
  stddev = TMath::Sqrt(stddev/(num_subsamples-cnt_invalidEntries));

  return stddev;
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
