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


void PlotSignificanceErrorDiff() {

  gROOT->ForceStyle();
  SetStyle();

  TString output_prefix = "temp_output/";
  if(!output_prefix.EndsWith("/")) output_prefix += "/";
  
  UInt_t num_files = 100;
  UInt_t num_files_start = 1;
  UInt_t num_files_end = 100;
  UInt_t step_size = 1;
  UInt_t num_massBins = 50;

  std::vector<UInt_t> project_bins;
  for(UInt_t i=0; i<num_massBins; i++) project_bins.push_back(i);

  TString filename_1 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_1subsamples/mass_histos.root";
  TString filename_2 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_2subsamples/mass_histos.root";
  TString filename_3 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_3subsamples/mass_histos.root";
  TString filename_4 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_4subsamples/mass_histos.root";
  TString filename_5 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_5subsamples/mass_histos.root";
  TString filename_6 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_6subsamples/mass_histos.root";
  TString filename_7 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_7subsamples/mass_histos.root";
  TString filename_8 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_8subsamples/mass_histos.root";
  TString filename_9 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_9subsamples/mass_histos.root";
  TString filename_10 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_10subsamples/mass_histos.root";
  TString filename_11 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_11subsamples/mass_histos.root";
  TString filename_12 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_12subsamples/mass_histos.root";
  TString filename_13 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_13subsamples/mass_histos.root";
  TString filename_14 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_14subsamples/mass_histos.root";
  TString filename_15 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_15subsamples/mass_histos.root";
  TString filename_16 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_16subsamples/mass_histos.root";
  TString filename_17 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_17subsamples/mass_histos.root";
  TString filename_18 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_18subsamples/mass_histos.root";
  TString filename_19 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_19subsamples/mass_histos.root";
  TString filename_20 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_20subsamples/mass_histos.root";
  TString filename_21 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_21subsamples/mass_histos.root";
  TString filename_22 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_22subsamples/mass_histos.root";
  TString filename_23 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_23subsamples/mass_histos.root";
  TString filename_24 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_24subsamples/mass_histos.root";
  TString filename_25 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_25subsamples/mass_histos.root";
  TString filename_26 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_26subsamples/mass_histos.root";
  TString filename_27 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_27subsamples/mass_histos.root";
  TString filename_28 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_28subsamples/mass_histos.root";
  TString filename_29 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_29subsamples/mass_histos.root";
  TString filename_30 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_30subsamples/mass_histos.root";
  TString filename_31 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_31subsamples/mass_histos.root";
  TString filename_32 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_32subsamples/mass_histos.root";
  TString filename_33 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_33subsamples/mass_histos.root";
  TString filename_34 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_34subsamples/mass_histos.root";
  TString filename_35 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_35subsamples/mass_histos.root";
  TString filename_36 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_36subsamples/mass_histos.root";
  TString filename_37 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_37subsamples/mass_histos.root";
  TString filename_38 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_38subsamples/mass_histos.root";
  TString filename_39 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_39subsamples/mass_histos.root";
  TString filename_40 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_40subsamples/mass_histos.root";
  TString filename_41 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_41subsamples/mass_histos.root";
  TString filename_42 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_42subsamples/mass_histos.root";
  TString filename_43 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_43subsamples/mass_histos.root";
  TString filename_44 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_44subsamples/mass_histos.root";
  TString filename_45 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_45subsamples/mass_histos.root";
  TString filename_46 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_46subsamples/mass_histos.root";
  TString filename_47 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_47subsamples/mass_histos.root";
  TString filename_48 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_48subsamples/mass_histos.root";
  TString filename_49 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_49subsamples/mass_histos.root";
  TString filename_50 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_50subsamples/mass_histos.root";
  TString filename_51 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_51subsamples/mass_histos.root";
  TString filename_52 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_52subsamples/mass_histos.root";
  TString filename_53 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_53subsamples/mass_histos.root";
  TString filename_54 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_54subsamples/mass_histos.root";
  TString filename_55 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_55subsamples/mass_histos.root";
  TString filename_56 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_56subsamples/mass_histos.root";
  TString filename_57 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_57subsamples/mass_histos.root";
  TString filename_58 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_58subsamples/mass_histos.root";
  TString filename_59 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_59subsamples/mass_histos.root";
  TString filename_60 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_60subsamples/mass_histos.root";
  TString filename_61 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_61subsamples/mass_histos.root";
  TString filename_62 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_62subsamples/mass_histos.root";
  TString filename_63 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_63subsamples/mass_histos.root";
  TString filename_64 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_64subsamples/mass_histos.root";
  TString filename_65 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_65subsamples/mass_histos.root";
  TString filename_66 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_66subsamples/mass_histos.root";
  TString filename_67 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_67subsamples/mass_histos.root";
  TString filename_68 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_68subsamples/mass_histos.root";
  TString filename_69 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_69subsamples/mass_histos.root";
  TString filename_70 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_70subsamples/mass_histos.root";
  TString filename_71 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_71subsamples/mass_histos.root";
  TString filename_72 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_72subsamples/mass_histos.root";
  TString filename_73 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_73subsamples/mass_histos.root";
  TString filename_74 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_74subsamples/mass_histos.root";
  TString filename_75 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_75subsamples/mass_histos.root";
  TString filename_76 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_76subsamples/mass_histos.root";
  TString filename_77 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_77subsamples/mass_histos.root";
  TString filename_78 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_78subsamples/mass_histos.root";
  TString filename_79 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_79subsamples/mass_histos.root";
  TString filename_80 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_80subsamples/mass_histos.root";
  TString filename_81 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_81subsamples/mass_histos.root";
  TString filename_82 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_82subsamples/mass_histos.root";
  TString filename_83 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_83subsamples/mass_histos.root";
  TString filename_84 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_84subsamples/mass_histos.root";
  TString filename_85 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_85subsamples/mass_histos.root";
  TString filename_86 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_86subsamples/mass_histos.root";
  TString filename_87 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_87subsamples/mass_histos.root";
  TString filename_88 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_88subsamples/mass_histos.root";
  TString filename_89 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_89subsamples/mass_histos.root";
  TString filename_90 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_90subsamples/mass_histos.root";
  TString filename_91 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_91subsamples/mass_histos.root";
  TString filename_92 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_92subsamples/mass_histos.root";
  TString filename_93 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_93subsamples/mass_histos.root";
  TString filename_94 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_94subsamples/mass_histos.root";
  TString filename_95 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_95subsamples/mass_histos.root";
  TString filename_96 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_96subsamples/mass_histos.root";
  TString filename_97 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_97subsamples/mass_histos.root";
  TString filename_98 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_98subsamples/mass_histos.root";
  TString filename_99 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_99subsamples/mass_histos.root";
  TString filename_100 = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/plots/significanceError_sampleSizeScan/data/new_correctedIntDiv/woBootstrapping/woBootstrapping_100subsamples/mass_histos.root";


  TFile *file_1 = new TFile(filename_1, "READ");
  TH1D *h_errorDiff_class_1 = (TH1D*)file_1->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_1 = (TH1D*)file_1->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_1 = (TH2D*)file_1->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_1 = (TH2D*)file_1->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_1 = (TH2D*)file_1->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_1 = (TH2D*)file_1->Get("h_backgr_exp_subsample");

  TFile *file_2 = new TFile(filename_2, "READ");
  TH1D *h_errorDiff_class_2 = (TH1D*)file_2->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_2 = (TH1D*)file_2->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_2 = (TH2D*)file_2->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_2 = (TH2D*)file_2->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_2 = (TH2D*)file_2->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_2 = (TH2D*)file_2->Get("h_backgr_exp_subsample");

  TFile *file_3 = new TFile(filename_3, "READ");
  TH1D *h_errorDiff_class_3 = (TH1D*)file_3->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_3 = (TH1D*)file_3->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_3 = (TH2D*)file_3->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_3 = (TH2D*)file_3->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_3 = (TH2D*)file_3->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_3 = (TH2D*)file_3->Get("h_backgr_exp_subsample");

  TFile *file_4 = new TFile(filename_4, "READ");
  TH1D *h_errorDiff_class_4 = (TH1D*)file_4->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_4 = (TH1D*)file_4->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_4 = (TH2D*)file_4->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_4 = (TH2D*)file_4->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_4 = (TH2D*)file_4->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_4 = (TH2D*)file_4->Get("h_backgr_exp_subsample");

  TFile *file_5 = new TFile(filename_5, "READ");
  TH1D *h_errorDiff_class_5 = (TH1D*)file_5->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_5 = (TH1D*)file_5->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_5 = (TH2D*)file_5->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_5 = (TH2D*)file_5->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_5 = (TH2D*)file_5->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_5 = (TH2D*)file_5->Get("h_backgr_exp_subsample");

  TFile *file_6 = new TFile(filename_6, "READ");
  TH1D *h_errorDiff_class_6 = (TH1D*)file_6->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_6 = (TH1D*)file_6->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_6 = (TH2D*)file_6->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_6 = (TH2D*)file_6->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_6 = (TH2D*)file_6->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_6 = (TH2D*)file_6->Get("h_backgr_exp_subsample");

  TFile *file_7 = new TFile(filename_7, "READ");
  TH1D *h_errorDiff_class_7 = (TH1D*)file_7->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_7 = (TH1D*)file_7->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_7 = (TH2D*)file_7->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_7 = (TH2D*)file_7->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_7 = (TH2D*)file_7->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_7 = (TH2D*)file_7->Get("h_backgr_exp_subsample");

  TFile *file_8 = new TFile(filename_8, "READ");
  TH1D *h_errorDiff_class_8 = (TH1D*)file_8->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_8 = (TH1D*)file_8->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_8 = (TH2D*)file_8->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_8 = (TH2D*)file_8->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_8 = (TH2D*)file_8->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_8 = (TH2D*)file_8->Get("h_backgr_exp_subsample");

  TFile *file_9 = new TFile(filename_9, "READ");
  TH1D *h_errorDiff_class_9 = (TH1D*)file_9->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_9 = (TH1D*)file_9->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_9 = (TH2D*)file_9->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_9 = (TH2D*)file_9->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_9 = (TH2D*)file_9->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_9 = (TH2D*)file_9->Get("h_backgr_exp_subsample");

  TFile *file_10 = new TFile(filename_10, "READ");
  TH1D *h_errorDiff_class_10 = (TH1D*)file_10->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_10 = (TH1D*)file_10->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_10 = (TH2D*)file_10->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_10 = (TH2D*)file_10->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_10 = (TH2D*)file_10->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_10 = (TH2D*)file_10->Get("h_backgr_exp_subsample");

  TFile *file_11 = new TFile(filename_11, "READ");
  TH1D *h_errorDiff_class_11 = (TH1D*)file_11->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_11 = (TH1D*)file_11->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_11 = (TH2D*)file_11->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_11 = (TH2D*)file_11->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_11 = (TH2D*)file_11->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_11 = (TH2D*)file_11->Get("h_backgr_exp_subsample");

  TFile *file_12 = new TFile(filename_12, "READ");
  TH1D *h_errorDiff_class_12 = (TH1D*)file_12->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_12 = (TH1D*)file_12->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_12 = (TH2D*)file_12->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_12 = (TH2D*)file_12->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_12 = (TH2D*)file_12->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_12 = (TH2D*)file_12->Get("h_backgr_exp_subsample");

  TFile *file_13 = new TFile(filename_13, "READ");
  TH1D *h_errorDiff_class_13 = (TH1D*)file_13->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_13 = (TH1D*)file_13->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_13 = (TH2D*)file_13->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_13 = (TH2D*)file_13->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_13 = (TH2D*)file_13->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_13 = (TH2D*)file_13->Get("h_backgr_exp_subsample");

  TFile *file_14 = new TFile(filename_14, "READ");
  TH1D *h_errorDiff_class_14 = (TH1D*)file_14->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_14 = (TH1D*)file_14->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_14 = (TH2D*)file_14->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_14 = (TH2D*)file_14->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_14 = (TH2D*)file_14->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_14 = (TH2D*)file_14->Get("h_backgr_exp_subsample");

  TFile *file_15 = new TFile(filename_15, "READ");
  TH1D *h_errorDiff_class_15 = (TH1D*)file_15->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_15 = (TH1D*)file_15->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_15 = (TH2D*)file_15->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_15 = (TH2D*)file_15->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_15 = (TH2D*)file_15->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_15 = (TH2D*)file_15->Get("h_backgr_exp_subsample");

  TFile *file_16 = new TFile(filename_16, "READ");
  TH1D *h_errorDiff_class_16 = (TH1D*)file_16->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_16 = (TH1D*)file_16->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_16 = (TH2D*)file_16->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_16 = (TH2D*)file_16->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_16 = (TH2D*)file_16->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_16 = (TH2D*)file_16->Get("h_backgr_exp_subsample");

  TFile *file_17 = new TFile(filename_17, "READ");
  TH1D *h_errorDiff_class_17 = (TH1D*)file_17->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_17 = (TH1D*)file_17->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_17 = (TH2D*)file_17->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_17 = (TH2D*)file_17->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_17 = (TH2D*)file_17->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_17 = (TH2D*)file_17->Get("h_backgr_exp_subsample");

  TFile *file_18 = new TFile(filename_18, "READ");
  TH1D *h_errorDiff_class_18 = (TH1D*)file_18->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_18 = (TH1D*)file_18->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_18 = (TH2D*)file_18->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_18 = (TH2D*)file_18->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_18 = (TH2D*)file_18->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_18 = (TH2D*)file_18->Get("h_backgr_exp_subsample");

  TFile *file_19 = new TFile(filename_19, "READ");
  TH1D *h_errorDiff_class_19 = (TH1D*)file_19->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_19 = (TH1D*)file_19->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_19 = (TH2D*)file_19->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_19 = (TH2D*)file_19->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_19 = (TH2D*)file_19->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_19 = (TH2D*)file_19->Get("h_backgr_exp_subsample");

  TFile *file_20 = new TFile(filename_20, "READ");
  TH1D *h_errorDiff_class_20 = (TH1D*)file_20->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_20 = (TH1D*)file_20->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_20 = (TH2D*)file_20->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_20 = (TH2D*)file_20->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_20 = (TH2D*)file_20->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_20 = (TH2D*)file_20->Get("h_backgr_exp_subsample");

  TFile *file_21 = new TFile(filename_21, "READ");
  TH1D *h_errorDiff_class_21 = (TH1D*)file_21->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_21 = (TH1D*)file_21->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_21 = (TH2D*)file_21->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_21 = (TH2D*)file_21->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_21 = (TH2D*)file_21->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_21 = (TH2D*)file_21->Get("h_backgr_exp_subsample");

  TFile *file_22 = new TFile(filename_22, "READ");
  TH1D *h_errorDiff_class_22 = (TH1D*)file_22->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_22 = (TH1D*)file_22->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_22 = (TH2D*)file_22->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_22 = (TH2D*)file_22->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_22 = (TH2D*)file_22->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_22 = (TH2D*)file_22->Get("h_backgr_exp_subsample");

  TFile *file_23 = new TFile(filename_23, "READ");
  TH1D *h_errorDiff_class_23 = (TH1D*)file_23->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_23 = (TH1D*)file_23->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_23 = (TH2D*)file_23->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_23 = (TH2D*)file_23->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_23 = (TH2D*)file_23->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_23 = (TH2D*)file_23->Get("h_backgr_exp_subsample");

  TFile *file_24 = new TFile(filename_24, "READ");
  TH1D *h_errorDiff_class_24 = (TH1D*)file_24->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_24 = (TH1D*)file_24->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_24 = (TH2D*)file_24->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_24 = (TH2D*)file_24->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_24 = (TH2D*)file_24->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_24 = (TH2D*)file_24->Get("h_backgr_exp_subsample");

  TFile *file_25 = new TFile(filename_25, "READ");
  TH1D *h_errorDiff_class_25 = (TH1D*)file_25->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_25 = (TH1D*)file_25->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_25 = (TH2D*)file_25->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_25 = (TH2D*)file_25->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_25 = (TH2D*)file_25->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_25 = (TH2D*)file_25->Get("h_backgr_exp_subsample");

  TFile *file_26 = new TFile(filename_26, "READ");
  TH1D *h_errorDiff_class_26 = (TH1D*)file_26->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_26 = (TH1D*)file_26->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_26 = (TH2D*)file_26->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_26 = (TH2D*)file_26->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_26 = (TH2D*)file_26->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_26 = (TH2D*)file_26->Get("h_backgr_exp_subsample");

  TFile *file_27 = new TFile(filename_27, "READ");
  TH1D *h_errorDiff_class_27 = (TH1D*)file_27->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_27 = (TH1D*)file_27->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_27 = (TH2D*)file_27->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_27 = (TH2D*)file_27->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_27 = (TH2D*)file_27->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_27 = (TH2D*)file_27->Get("h_backgr_exp_subsample");

  TFile *file_28 = new TFile(filename_28, "READ");
  TH1D *h_errorDiff_class_28 = (TH1D*)file_28->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_28 = (TH1D*)file_28->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_28 = (TH2D*)file_28->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_28 = (TH2D*)file_28->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_28 = (TH2D*)file_28->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_28 = (TH2D*)file_28->Get("h_backgr_exp_subsample");

  TFile *file_29 = new TFile(filename_29, "READ");
  TH1D *h_errorDiff_class_29 = (TH1D*)file_29->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_29 = (TH1D*)file_29->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_29 = (TH2D*)file_29->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_29 = (TH2D*)file_29->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_29 = (TH2D*)file_29->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_29 = (TH2D*)file_29->Get("h_backgr_exp_subsample");

  TFile *file_30 = new TFile(filename_30, "READ");
  TH1D *h_errorDiff_class_30 = (TH1D*)file_30->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_30 = (TH1D*)file_30->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_30 = (TH2D*)file_30->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_30 = (TH2D*)file_30->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_30 = (TH2D*)file_30->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_30 = (TH2D*)file_30->Get("h_backgr_exp_subsample");

  TFile *file_31 = new TFile(filename_31, "READ");
  TH1D *h_errorDiff_class_31 = (TH1D*)file_31->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_31 = (TH1D*)file_31->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_31 = (TH2D*)file_31->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_31 = (TH2D*)file_31->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_31 = (TH2D*)file_31->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_31 = (TH2D*)file_31->Get("h_backgr_exp_subsample");

  TFile *file_32 = new TFile(filename_32, "READ");
  TH1D *h_errorDiff_class_32 = (TH1D*)file_32->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_32 = (TH1D*)file_32->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_32 = (TH2D*)file_32->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_32 = (TH2D*)file_32->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_32 = (TH2D*)file_32->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_32 = (TH2D*)file_32->Get("h_backgr_exp_subsample");

  TFile *file_33 = new TFile(filename_33, "READ");
  TH1D *h_errorDiff_class_33 = (TH1D*)file_33->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_33 = (TH1D*)file_33->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_33 = (TH2D*)file_33->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_33 = (TH2D*)file_33->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_33 = (TH2D*)file_33->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_33 = (TH2D*)file_33->Get("h_backgr_exp_subsample");

  TFile *file_34 = new TFile(filename_34, "READ");
  TH1D *h_errorDiff_class_34 = (TH1D*)file_34->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_34 = (TH1D*)file_34->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_34 = (TH2D*)file_34->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_34 = (TH2D*)file_34->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_34 = (TH2D*)file_34->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_34 = (TH2D*)file_34->Get("h_backgr_exp_subsample");

  TFile *file_35 = new TFile(filename_35, "READ");
  TH1D *h_errorDiff_class_35 = (TH1D*)file_35->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_35 = (TH1D*)file_35->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_35 = (TH2D*)file_35->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_35 = (TH2D*)file_35->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_35 = (TH2D*)file_35->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_35 = (TH2D*)file_35->Get("h_backgr_exp_subsample");

  TFile *file_36 = new TFile(filename_36, "READ");
  TH1D *h_errorDiff_class_36 = (TH1D*)file_36->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_36 = (TH1D*)file_36->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_36 = (TH2D*)file_36->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_36 = (TH2D*)file_36->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_36 = (TH2D*)file_36->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_36 = (TH2D*)file_36->Get("h_backgr_exp_subsample");

  TFile *file_37 = new TFile(filename_37, "READ");
  TH1D *h_errorDiff_class_37 = (TH1D*)file_37->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_37 = (TH1D*)file_37->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_37 = (TH2D*)file_37->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_37 = (TH2D*)file_37->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_37 = (TH2D*)file_37->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_37 = (TH2D*)file_37->Get("h_backgr_exp_subsample");

  TFile *file_38 = new TFile(filename_38, "READ");
  TH1D *h_errorDiff_class_38 = (TH1D*)file_38->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_38 = (TH1D*)file_38->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_38 = (TH2D*)file_38->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_38 = (TH2D*)file_38->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_38 = (TH2D*)file_38->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_38 = (TH2D*)file_38->Get("h_backgr_exp_subsample");

  TFile *file_39 = new TFile(filename_39, "READ");
  TH1D *h_errorDiff_class_39 = (TH1D*)file_39->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_39 = (TH1D*)file_39->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_39 = (TH2D*)file_39->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_39 = (TH2D*)file_39->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_39 = (TH2D*)file_39->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_39 = (TH2D*)file_39->Get("h_backgr_exp_subsample");

  TFile *file_40 = new TFile(filename_40, "READ");
  TH1D *h_errorDiff_class_40 = (TH1D*)file_40->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_40 = (TH1D*)file_40->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_40 = (TH2D*)file_40->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_40 = (TH2D*)file_40->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_40 = (TH2D*)file_40->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_40 = (TH2D*)file_40->Get("h_backgr_exp_subsample");

  TFile *file_41 = new TFile(filename_41, "READ");
  TH1D *h_errorDiff_class_41 = (TH1D*)file_41->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_41 = (TH1D*)file_41->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_41 = (TH2D*)file_41->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_41 = (TH2D*)file_41->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_41 = (TH2D*)file_41->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_41 = (TH2D*)file_41->Get("h_backgr_exp_subsample");

  TFile *file_42 = new TFile(filename_42, "READ");
  TH1D *h_errorDiff_class_42 = (TH1D*)file_42->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_42 = (TH1D*)file_42->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_42 = (TH2D*)file_42->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_42 = (TH2D*)file_42->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_42 = (TH2D*)file_42->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_42 = (TH2D*)file_42->Get("h_backgr_exp_subsample");

  TFile *file_43 = new TFile(filename_43, "READ");
  TH1D *h_errorDiff_class_43 = (TH1D*)file_43->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_43 = (TH1D*)file_43->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_43 = (TH2D*)file_43->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_43 = (TH2D*)file_43->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_43 = (TH2D*)file_43->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_43 = (TH2D*)file_43->Get("h_backgr_exp_subsample");

  TFile *file_44 = new TFile(filename_44, "READ");
  TH1D *h_errorDiff_class_44 = (TH1D*)file_44->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_44 = (TH1D*)file_44->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_44 = (TH2D*)file_44->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_44 = (TH2D*)file_44->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_44 = (TH2D*)file_44->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_44 = (TH2D*)file_44->Get("h_backgr_exp_subsample");

  TFile *file_45 = new TFile(filename_45, "READ");
  TH1D *h_errorDiff_class_45 = (TH1D*)file_45->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_45 = (TH1D*)file_45->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_45 = (TH2D*)file_45->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_45 = (TH2D*)file_45->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_45 = (TH2D*)file_45->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_45 = (TH2D*)file_45->Get("h_backgr_exp_subsample");

  TFile *file_46 = new TFile(filename_46, "READ");
  TH1D *h_errorDiff_class_46 = (TH1D*)file_46->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_46 = (TH1D*)file_46->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_46 = (TH2D*)file_46->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_46 = (TH2D*)file_46->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_46 = (TH2D*)file_46->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_46 = (TH2D*)file_46->Get("h_backgr_exp_subsample");

  TFile *file_47 = new TFile(filename_47, "READ");
  TH1D *h_errorDiff_class_47 = (TH1D*)file_47->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_47 = (TH1D*)file_47->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_47 = (TH2D*)file_47->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_47 = (TH2D*)file_47->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_47 = (TH2D*)file_47->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_47 = (TH2D*)file_47->Get("h_backgr_exp_subsample");

  TFile *file_48 = new TFile(filename_48, "READ");
  TH1D *h_errorDiff_class_48 = (TH1D*)file_48->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_48 = (TH1D*)file_48->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_48 = (TH2D*)file_48->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_48 = (TH2D*)file_48->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_48 = (TH2D*)file_48->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_48 = (TH2D*)file_48->Get("h_backgr_exp_subsample");

  TFile *file_49 = new TFile(filename_49, "READ");
  TH1D *h_errorDiff_class_49 = (TH1D*)file_49->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_49 = (TH1D*)file_49->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_49 = (TH2D*)file_49->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_49 = (TH2D*)file_49->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_49 = (TH2D*)file_49->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_49 = (TH2D*)file_49->Get("h_backgr_exp_subsample");

  TFile *file_50 = new TFile(filename_50, "READ");
  TH1D *h_errorDiff_class_50 = (TH1D*)file_50->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_50 = (TH1D*)file_50->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_50 = (TH2D*)file_50->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_50 = (TH2D*)file_50->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_50 = (TH2D*)file_50->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_50 = (TH2D*)file_50->Get("h_backgr_exp_subsample");

  TFile *file_51 = new TFile(filename_51, "READ");
  TH1D *h_errorDiff_class_51 = (TH1D*)file_51->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_51 = (TH1D*)file_51->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_51 = (TH2D*)file_51->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_51 = (TH2D*)file_51->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_51 = (TH2D*)file_51->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_51 = (TH2D*)file_51->Get("h_backgr_exp_subsample");

  TFile *file_52 = new TFile(filename_52, "READ");
  TH1D *h_errorDiff_class_52 = (TH1D*)file_52->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_52 = (TH1D*)file_52->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_52 = (TH2D*)file_52->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_52 = (TH2D*)file_52->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_52 = (TH2D*)file_52->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_52 = (TH2D*)file_52->Get("h_backgr_exp_subsample");

  TFile *file_53 = new TFile(filename_53, "READ");
  TH1D *h_errorDiff_class_53 = (TH1D*)file_53->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_53 = (TH1D*)file_53->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_53 = (TH2D*)file_53->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_53 = (TH2D*)file_53->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_53 = (TH2D*)file_53->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_53 = (TH2D*)file_53->Get("h_backgr_exp_subsample");

  TFile *file_54 = new TFile(filename_54, "READ");
  TH1D *h_errorDiff_class_54 = (TH1D*)file_54->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_54 = (TH1D*)file_54->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_54 = (TH2D*)file_54->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_54 = (TH2D*)file_54->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_54 = (TH2D*)file_54->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_54 = (TH2D*)file_54->Get("h_backgr_exp_subsample");

  TFile *file_55 = new TFile(filename_55, "READ");
  TH1D *h_errorDiff_class_55 = (TH1D*)file_55->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_55 = (TH1D*)file_55->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_55 = (TH2D*)file_55->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_55 = (TH2D*)file_55->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_55 = (TH2D*)file_55->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_55 = (TH2D*)file_55->Get("h_backgr_exp_subsample");

  TFile *file_56 = new TFile(filename_56, "READ");
  TH1D *h_errorDiff_class_56 = (TH1D*)file_56->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_56 = (TH1D*)file_56->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_56 = (TH2D*)file_56->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_56 = (TH2D*)file_56->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_56 = (TH2D*)file_56->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_56 = (TH2D*)file_56->Get("h_backgr_exp_subsample");

  TFile *file_57 = new TFile(filename_57, "READ");
  TH1D *h_errorDiff_class_57 = (TH1D*)file_57->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_57 = (TH1D*)file_57->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_57 = (TH2D*)file_57->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_57 = (TH2D*)file_57->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_57 = (TH2D*)file_57->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_57 = (TH2D*)file_57->Get("h_backgr_exp_subsample");

  TFile *file_58 = new TFile(filename_58, "READ");
  TH1D *h_errorDiff_class_58 = (TH1D*)file_58->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_58 = (TH1D*)file_58->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_58 = (TH2D*)file_58->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_58 = (TH2D*)file_58->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_58 = (TH2D*)file_58->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_58 = (TH2D*)file_58->Get("h_backgr_exp_subsample");

  TFile *file_59 = new TFile(filename_59, "READ");
  TH1D *h_errorDiff_class_59 = (TH1D*)file_59->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_59 = (TH1D*)file_59->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_59 = (TH2D*)file_59->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_59 = (TH2D*)file_59->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_59 = (TH2D*)file_59->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_59 = (TH2D*)file_59->Get("h_backgr_exp_subsample");

  TFile *file_60 = new TFile(filename_60, "READ");
  TH1D *h_errorDiff_class_60 = (TH1D*)file_60->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_60 = (TH1D*)file_60->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_60 = (TH2D*)file_60->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_60 = (TH2D*)file_60->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_60 = (TH2D*)file_60->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_60 = (TH2D*)file_60->Get("h_backgr_exp_subsample");

  TFile *file_61 = new TFile(filename_61, "READ");
  TH1D *h_errorDiff_class_61 = (TH1D*)file_61->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_61 = (TH1D*)file_61->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_61 = (TH2D*)file_61->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_61 = (TH2D*)file_61->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_61 = (TH2D*)file_61->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_61 = (TH2D*)file_61->Get("h_backgr_exp_subsample");

  TFile *file_62 = new TFile(filename_62, "READ");
  TH1D *h_errorDiff_class_62 = (TH1D*)file_62->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_62 = (TH1D*)file_62->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_62 = (TH2D*)file_62->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_62 = (TH2D*)file_62->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_62 = (TH2D*)file_62->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_62 = (TH2D*)file_62->Get("h_backgr_exp_subsample");

  TFile *file_63 = new TFile(filename_63, "READ");
  TH1D *h_errorDiff_class_63 = (TH1D*)file_63->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_63 = (TH1D*)file_63->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_63 = (TH2D*)file_63->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_63 = (TH2D*)file_63->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_63 = (TH2D*)file_63->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_63 = (TH2D*)file_63->Get("h_backgr_exp_subsample");

  TFile *file_64 = new TFile(filename_64, "READ");
  TH1D *h_errorDiff_class_64 = (TH1D*)file_64->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_64 = (TH1D*)file_64->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_64 = (TH2D*)file_64->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_64 = (TH2D*)file_64->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_64 = (TH2D*)file_64->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_64 = (TH2D*)file_64->Get("h_backgr_exp_subsample");

  TFile *file_65 = new TFile(filename_65, "READ");
  TH1D *h_errorDiff_class_65 = (TH1D*)file_65->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_65 = (TH1D*)file_65->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_65 = (TH2D*)file_65->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_65 = (TH2D*)file_65->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_65 = (TH2D*)file_65->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_65 = (TH2D*)file_65->Get("h_backgr_exp_subsample");

  TFile *file_66 = new TFile(filename_66, "READ");
  TH1D *h_errorDiff_class_66 = (TH1D*)file_66->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_66 = (TH1D*)file_66->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_66 = (TH2D*)file_66->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_66 = (TH2D*)file_66->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_66 = (TH2D*)file_66->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_66 = (TH2D*)file_66->Get("h_backgr_exp_subsample");

  TFile *file_67 = new TFile(filename_67, "READ");
  TH1D *h_errorDiff_class_67 = (TH1D*)file_67->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_67 = (TH1D*)file_67->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_67 = (TH2D*)file_67->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_67 = (TH2D*)file_67->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_67 = (TH2D*)file_67->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_67 = (TH2D*)file_67->Get("h_backgr_exp_subsample");

  TFile *file_68 = new TFile(filename_68, "READ");
  TH1D *h_errorDiff_class_68 = (TH1D*)file_68->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_68 = (TH1D*)file_68->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_68 = (TH2D*)file_68->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_68 = (TH2D*)file_68->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_68 = (TH2D*)file_68->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_68 = (TH2D*)file_68->Get("h_backgr_exp_subsample");

  TFile *file_69 = new TFile(filename_69, "READ");
  TH1D *h_errorDiff_class_69 = (TH1D*)file_69->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_69 = (TH1D*)file_69->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_69 = (TH2D*)file_69->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_69 = (TH2D*)file_69->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_69 = (TH2D*)file_69->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_69 = (TH2D*)file_69->Get("h_backgr_exp_subsample");

  TFile *file_70 = new TFile(filename_70, "READ");
  TH1D *h_errorDiff_class_70 = (TH1D*)file_70->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_70 = (TH1D*)file_70->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_70 = (TH2D*)file_70->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_70 = (TH2D*)file_70->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_70 = (TH2D*)file_70->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_70 = (TH2D*)file_70->Get("h_backgr_exp_subsample");

  TFile *file_71 = new TFile(filename_71, "READ");
  TH1D *h_errorDiff_class_71 = (TH1D*)file_71->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_71 = (TH1D*)file_71->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_71 = (TH2D*)file_71->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_71 = (TH2D*)file_71->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_71 = (TH2D*)file_71->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_71 = (TH2D*)file_71->Get("h_backgr_exp_subsample");

  TFile *file_72 = new TFile(filename_72, "READ");
  TH1D *h_errorDiff_class_72 = (TH1D*)file_72->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_72 = (TH1D*)file_72->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_72 = (TH2D*)file_72->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_72 = (TH2D*)file_72->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_72 = (TH2D*)file_72->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_72 = (TH2D*)file_72->Get("h_backgr_exp_subsample");

  TFile *file_73 = new TFile(filename_73, "READ");
  TH1D *h_errorDiff_class_73 = (TH1D*)file_73->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_73 = (TH1D*)file_73->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_73 = (TH2D*)file_73->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_73 = (TH2D*)file_73->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_73 = (TH2D*)file_73->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_73 = (TH2D*)file_73->Get("h_backgr_exp_subsample");

  TFile *file_74 = new TFile(filename_74, "READ");
  TH1D *h_errorDiff_class_74 = (TH1D*)file_74->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_74 = (TH1D*)file_74->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_74 = (TH2D*)file_74->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_74 = (TH2D*)file_74->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_74 = (TH2D*)file_74->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_74 = (TH2D*)file_74->Get("h_backgr_exp_subsample");

  TFile *file_75 = new TFile(filename_75, "READ");
  TH1D *h_errorDiff_class_75 = (TH1D*)file_75->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_75 = (TH1D*)file_75->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_75 = (TH2D*)file_75->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_75 = (TH2D*)file_75->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_75 = (TH2D*)file_75->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_75 = (TH2D*)file_75->Get("h_backgr_exp_subsample");

  TFile *file_76 = new TFile(filename_76, "READ");
  TH1D *h_errorDiff_class_76 = (TH1D*)file_76->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_76 = (TH1D*)file_76->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_76 = (TH2D*)file_76->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_76 = (TH2D*)file_76->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_76 = (TH2D*)file_76->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_76 = (TH2D*)file_76->Get("h_backgr_exp_subsample");

  TFile *file_77 = new TFile(filename_77, "READ");
  TH1D *h_errorDiff_class_77 = (TH1D*)file_77->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_77 = (TH1D*)file_77->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_77 = (TH2D*)file_77->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_77 = (TH2D*)file_77->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_77 = (TH2D*)file_77->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_77 = (TH2D*)file_77->Get("h_backgr_exp_subsample");

  TFile *file_78 = new TFile(filename_78, "READ");
  TH1D *h_errorDiff_class_78 = (TH1D*)file_78->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_78 = (TH1D*)file_78->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_78 = (TH2D*)file_78->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_78 = (TH2D*)file_78->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_78 = (TH2D*)file_78->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_78 = (TH2D*)file_78->Get("h_backgr_exp_subsample");

  TFile *file_79 = new TFile(filename_79, "READ");
  TH1D *h_errorDiff_class_79 = (TH1D*)file_79->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_79 = (TH1D*)file_79->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_79 = (TH2D*)file_79->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_79 = (TH2D*)file_79->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_79 = (TH2D*)file_79->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_79 = (TH2D*)file_79->Get("h_backgr_exp_subsample");

  TFile *file_80 = new TFile(filename_80, "READ");
  TH1D *h_errorDiff_class_80 = (TH1D*)file_80->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_80 = (TH1D*)file_80->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_80 = (TH2D*)file_80->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_80 = (TH2D*)file_80->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_80 = (TH2D*)file_80->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_80 = (TH2D*)file_80->Get("h_backgr_exp_subsample");

  TFile *file_81 = new TFile(filename_81, "READ");
  TH1D *h_errorDiff_class_81 = (TH1D*)file_81->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_81 = (TH1D*)file_81->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_81 = (TH2D*)file_81->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_81 = (TH2D*)file_81->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_81 = (TH2D*)file_81->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_81 = (TH2D*)file_81->Get("h_backgr_exp_subsample");

  TFile *file_82 = new TFile(filename_82, "READ");
  TH1D *h_errorDiff_class_82 = (TH1D*)file_82->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_82 = (TH1D*)file_82->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_82 = (TH2D*)file_82->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_82 = (TH2D*)file_82->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_82 = (TH2D*)file_82->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_82 = (TH2D*)file_82->Get("h_backgr_exp_subsample");

  TFile *file_83 = new TFile(filename_83, "READ");
  TH1D *h_errorDiff_class_83 = (TH1D*)file_83->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_83 = (TH1D*)file_83->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_83 = (TH2D*)file_83->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_83 = (TH2D*)file_83->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_83 = (TH2D*)file_83->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_83 = (TH2D*)file_83->Get("h_backgr_exp_subsample");

  TFile *file_84 = new TFile(filename_84, "READ");
  TH1D *h_errorDiff_class_84 = (TH1D*)file_84->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_84 = (TH1D*)file_84->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_84 = (TH2D*)file_84->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_84 = (TH2D*)file_84->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_84 = (TH2D*)file_84->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_84 = (TH2D*)file_84->Get("h_backgr_exp_subsample");

  TFile *file_85 = new TFile(filename_85, "READ");
  TH1D *h_errorDiff_class_85 = (TH1D*)file_85->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_85 = (TH1D*)file_85->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_85 = (TH2D*)file_85->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_85 = (TH2D*)file_85->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_85 = (TH2D*)file_85->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_85 = (TH2D*)file_85->Get("h_backgr_exp_subsample");

  TFile *file_86 = new TFile(filename_86, "READ");
  TH1D *h_errorDiff_class_86 = (TH1D*)file_86->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_86 = (TH1D*)file_86->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_86 = (TH2D*)file_86->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_86 = (TH2D*)file_86->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_86 = (TH2D*)file_86->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_86 = (TH2D*)file_86->Get("h_backgr_exp_subsample");

  TFile *file_87 = new TFile(filename_87, "READ");
  TH1D *h_errorDiff_class_87 = (TH1D*)file_87->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_87 = (TH1D*)file_87->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_87 = (TH2D*)file_87->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_87 = (TH2D*)file_87->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_87 = (TH2D*)file_87->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_87 = (TH2D*)file_87->Get("h_backgr_exp_subsample");

  TFile *file_88 = new TFile(filename_88, "READ");
  TH1D *h_errorDiff_class_88 = (TH1D*)file_88->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_88 = (TH1D*)file_88->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_88 = (TH2D*)file_88->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_88 = (TH2D*)file_88->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_88 = (TH2D*)file_88->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_88 = (TH2D*)file_88->Get("h_backgr_exp_subsample");

  TFile *file_89 = new TFile(filename_89, "READ");
  TH1D *h_errorDiff_class_89 = (TH1D*)file_89->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_89 = (TH1D*)file_89->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_89 = (TH2D*)file_89->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_89 = (TH2D*)file_89->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_89 = (TH2D*)file_89->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_89 = (TH2D*)file_89->Get("h_backgr_exp_subsample");

  TFile *file_90 = new TFile(filename_90, "READ");
  TH1D *h_errorDiff_class_90 = (TH1D*)file_90->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_90 = (TH1D*)file_90->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_90 = (TH2D*)file_90->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_90 = (TH2D*)file_90->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_90 = (TH2D*)file_90->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_90 = (TH2D*)file_90->Get("h_backgr_exp_subsample");

  TFile *file_91 = new TFile(filename_91, "READ");
  TH1D *h_errorDiff_class_91 = (TH1D*)file_91->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_91 = (TH1D*)file_91->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_91 = (TH2D*)file_91->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_91 = (TH2D*)file_91->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_91 = (TH2D*)file_91->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_91 = (TH2D*)file_91->Get("h_backgr_exp_subsample");

  TFile *file_92 = new TFile(filename_92, "READ");
  TH1D *h_errorDiff_class_92 = (TH1D*)file_92->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_92 = (TH1D*)file_92->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_92 = (TH2D*)file_92->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_92 = (TH2D*)file_92->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_92 = (TH2D*)file_92->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_92 = (TH2D*)file_92->Get("h_backgr_exp_subsample");

  TFile *file_93 = new TFile(filename_93, "READ");
  TH1D *h_errorDiff_class_93 = (TH1D*)file_93->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_93 = (TH1D*)file_93->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_93 = (TH2D*)file_93->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_93 = (TH2D*)file_93->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_93 = (TH2D*)file_93->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_93 = (TH2D*)file_93->Get("h_backgr_exp_subsample");

  TFile *file_94 = new TFile(filename_94, "READ");
  TH1D *h_errorDiff_class_94 = (TH1D*)file_94->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_94 = (TH1D*)file_94->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_94 = (TH2D*)file_94->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_94 = (TH2D*)file_94->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_94 = (TH2D*)file_94->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_94 = (TH2D*)file_94->Get("h_backgr_exp_subsample");

  TFile *file_95 = new TFile(filename_95, "READ");
  TH1D *h_errorDiff_class_95 = (TH1D*)file_95->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_95 = (TH1D*)file_95->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_95 = (TH2D*)file_95->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_95 = (TH2D*)file_95->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_95 = (TH2D*)file_95->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_95 = (TH2D*)file_95->Get("h_backgr_exp_subsample");

  TFile *file_96 = new TFile(filename_96, "READ");
  TH1D *h_errorDiff_class_96 = (TH1D*)file_96->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_96 = (TH1D*)file_96->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_96 = (TH2D*)file_96->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_96 = (TH2D*)file_96->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_96 = (TH2D*)file_96->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_96 = (TH2D*)file_96->Get("h_backgr_exp_subsample");

  TFile *file_97 = new TFile(filename_97, "READ");
  TH1D *h_errorDiff_class_97 = (TH1D*)file_97->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_97 = (TH1D*)file_97->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_97 = (TH2D*)file_97->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_97 = (TH2D*)file_97->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_97 = (TH2D*)file_97->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_97 = (TH2D*)file_97->Get("h_backgr_exp_subsample");

  TFile *file_98 = new TFile(filename_98, "READ");
  TH1D *h_errorDiff_class_98 = (TH1D*)file_98->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_98 = (TH1D*)file_98->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_98 = (TH2D*)file_98->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_98 = (TH2D*)file_98->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_98 = (TH2D*)file_98->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_98 = (TH2D*)file_98->Get("h_backgr_exp_subsample");

  TFile *file_99 = new TFile(filename_99, "READ");
  TH1D *h_errorDiff_class_99 = (TH1D*)file_99->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_99 = (TH1D*)file_99->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_99 = (TH2D*)file_99->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_99 = (TH2D*)file_99->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_99 = (TH2D*)file_99->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_99 = (TH2D*)file_99->Get("h_backgr_exp_subsample");

  TFile *file_100 = new TFile(filename_100, "READ");
  TH1D *h_errorDiff_class_100 = (TH1D*)file_100->Get("h_errorDiff_significance_class");
  TH1D *h_errorDiff_exp_100 = (TH1D*)file_100->Get("h_errorDiff_significance_exp");
  TH2D *h_signal_class_subsample_100 = (TH2D*)file_100->Get("h_signal_class_subsample");
  TH2D *h_backgr_class_subsample_100 = (TH2D*)file_100->Get("h_backgr_class_subsample");
  TH2D *h_signal_exp_subsample_100 = (TH2D*)file_100->Get("h_signal_exp_subsample");
  TH2D *h_backgr_exp_subsample_100 = (TH2D*)file_100->Get("h_backgr_exp_subsample");



 
  
  Float_t nBins = h_errorDiff_class_1->GetNbinsX(),
    min = 0.,
    max = h_errorDiff_class_1->GetXaxis()->GetBinUpEdge(h_errorDiff_class_1->GetNbinsX());

  
  TH2D *h_errorDiff_class_compare = new TH2D("h_errorDiff_class_compare","",
					     num_files,num_files_start,num_files_end*step_size,
					     nBins,min,max);
  TH2D *h_errorDiff_exp_compare = new TH2D("h_errorDiff_exp_compare","",
					   num_files,num_files_start,num_files_end*step_size,
					   nBins,min,max);
  
  std::vector<TH1D*> v_errorDiff_class_compare, v_errorDiff_exp_compare;
  v_errorDiff_class_compare.push_back(h_errorDiff_class_1);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_1);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_2);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_2);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_3);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_3);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_4);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_4);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_5);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_5);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_6);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_6);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_7);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_7);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_8);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_8);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_9);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_9);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_10);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_10);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_11);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_11);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_12);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_12);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_13);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_13);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_14);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_14);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_15);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_15);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_16);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_16);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_17);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_17);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_18);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_18);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_19);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_19);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_20);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_20);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_21);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_21);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_22);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_22);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_23);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_23);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_24);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_24);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_25);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_25);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_26);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_26);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_27);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_27);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_28);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_28);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_29);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_29);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_30);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_30);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_31);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_31);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_32);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_32);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_33);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_33);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_34);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_34);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_35);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_35);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_36);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_36);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_37);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_37);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_38);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_38);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_39);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_39);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_40);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_40);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_41);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_41);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_42);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_42);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_43);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_43);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_44);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_44);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_45);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_45);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_46);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_46);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_47);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_47);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_48);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_48);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_49);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_49);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_50);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_50);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_51);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_51);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_52);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_52);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_53);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_53);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_54);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_54);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_55);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_55);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_56);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_56);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_57);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_57);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_58);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_58);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_59);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_59);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_60);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_60);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_61);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_61);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_62);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_62);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_63);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_63);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_64);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_64);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_65);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_65);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_66);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_66);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_67);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_67);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_68);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_68);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_69);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_69);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_70);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_70);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_71);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_71);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_72);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_72);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_73);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_73);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_74);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_74);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_75);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_75);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_76);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_76);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_77);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_77);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_78);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_78);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_79);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_79);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_80);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_80);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_81);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_81);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_82);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_82);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_83);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_83);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_84);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_84);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_85);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_85);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_86);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_86);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_87);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_87);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_88);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_88);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_89);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_89);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_90);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_90);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_91);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_91);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_92);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_92);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_93);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_93);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_94);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_94);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_95);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_95);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_96);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_96);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_97);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_97);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_98);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_98);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_99);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_99);
  v_errorDiff_class_compare.push_back(h_errorDiff_class_100);
  v_errorDiff_exp_compare.push_back(h_errorDiff_exp_100);

 
  if(v_errorDiff_class_compare.size() != num_files) {
    std::cout << "  ERROR: v_errorDiff_class_compare.size() != num_files"
	      << " (" << v_errorDiff_class_compare.size() << " vs. " << num_files << ")"
	      << std::endl;
    gApplication->Terminate();
  }
  if(v_errorDiff_exp_compare.size() != num_files) {
    std::cout << "  ERROR: v_errorDiff_exp_compare.size() != num_files"
	      << " (" << v_errorDiff_exp_compare.size() << " vs. " << num_files << ")"
	      << std::endl;
    gApplication->Terminate();
  }
  
  for(UInt_t i=1; i<=nBins; i++) {
    for(UInt_t j=0; j<num_files; j++) {

      h_errorDiff_class_compare->SetBinContent(j+1, i, v_errorDiff_class_compare[j]->GetBinContent(i));
      h_errorDiff_exp_compare->SetBinContent(j+1, i, v_errorDiff_exp_compare[j]->GetBinContent(i));

    }
  }
  

  TH1D *h_errorDiff_class_compare_projectX = new TH1D("h_errorDiff_class_compare_projectX","",num_files,num_files_start,num_files_end);
  h_errorDiff_class_compare_projectX = (TH1D*)h_errorDiff_class_compare->ProjectionX();

  TH1D *h_errorDiff_exp_compare_projectX = new TH1D("h_errorDiff_exp_compare_projectX","",num_files_start,0,num_files_end);
  h_errorDiff_exp_compare_projectX = (TH1D*)h_errorDiff_exp_compare->ProjectionX();

  h_errorDiff_class_compare_projectX->SetXTitle("number of subsamples");
  h_errorDiff_class_compare_projectX->SetYTitle("mass-integrated error diff. (class.)");
  h_errorDiff_class_compare_projectX->SetMarkerStyle(20);
  h_errorDiff_class_compare_projectX->SetMarkerSize(1);

  h_errorDiff_exp_compare_projectX->SetXTitle("number of subsamples");
  h_errorDiff_exp_compare_projectX->SetYTitle("mass-integrated error diff. (exp.)");
  h_errorDiff_exp_compare_projectX->SetMarkerStyle(20);
  h_errorDiff_exp_compare_projectX->SetMarkerSize(1);
  

  h_errorDiff_class_compare->SetYTitle("#it{m}_{ee} (GeV/#it{c}^{2})");
  h_errorDiff_class_compare->SetXTitle("number of subsamples");
  h_errorDiff_class_compare->SetZTitle("(error_{uncorr} - error_{est}) / error_{uncorr} (class.)");
  // h_errorDiff_class_compare->SetMaximum(1.);

  h_errorDiff_exp_compare->SetYTitle("#it{m}_{ee} (GeV/#it{c}^{2})");
  h_errorDiff_exp_compare->SetXTitle("number of subsamples");
  h_errorDiff_exp_compare->SetZTitle("(error_{uncorr} - error_{est}) / error_{uncorr} (exp.)");
  // h_errorDiff_exp_compare->SetMaximum(1.);
  
  
  TCanvas *c_errorDiff_class_compare = new TCanvas("c_errorDiff_class_compare","",1024,768);
  c_errorDiff_class_compare->SetRightMargin(.17);
  h_errorDiff_class_compare->Draw("colz");

  TCanvas *c_errorDiff_exp_compare = new TCanvas("c_errorDiff_exp_compare","",1024,768);
  c_errorDiff_exp_compare->SetRightMargin(.17);
  h_errorDiff_exp_compare->Draw("colz");


  TCanvas *c_errorDiff_class_compare_projectX = new TCanvas("c_errorDiff_class_compare_projectX","",1024,768);
  h_errorDiff_class_compare_projectX->Draw("hist");

  TCanvas *c_errorDiff_exp_compare_projectX = new TCanvas("c_errorDiff_exp_compare_projectX","",1024,768);
  h_errorDiff_exp_compare_projectX->Draw("hist");


  c_errorDiff_class_compare->SaveAs(output_prefix + "significanceErrorDiff_class.pdf");
  c_errorDiff_class_compare->SaveAs(output_prefix + "significanceErrorDiff_class.png");
  c_errorDiff_class_compare->SaveAs(output_prefix + "significanceErrorDiff_class.root");

  c_errorDiff_exp_compare->SaveAs(output_prefix + "significanceErrorDiff_exp.pdf");
  c_errorDiff_exp_compare->SaveAs(output_prefix + "significanceErrorDiff_exp.png");
  c_errorDiff_exp_compare->SaveAs(output_prefix + "significanceErrorDiff_exp.root");

  c_errorDiff_class_compare_projectX->SaveAs(output_prefix + "significanceErrorDiff_class_projectX.pdf");
  c_errorDiff_class_compare_projectX->SaveAs(output_prefix + "significanceErrorDiff_class_projectX.png");
  c_errorDiff_class_compare_projectX->SaveAs(output_prefix + "significanceErrorDiff_class_projectX.root");

  c_errorDiff_exp_compare_projectX->SaveAs(output_prefix + "significanceErrorDiff_exp_projectX.pdf");
  c_errorDiff_exp_compare_projectX->SaveAs(output_prefix + "significanceErrorDiff_exp_projectX.png");
  c_errorDiff_exp_compare_projectX->SaveAs(output_prefix + "significanceErrorDiff_exp_projectX.root");




  
  for(UInt_t i=0; i<project_bins.size(); i++) {

    TH1D *h_project_massBin = new TH1D("h_project_massBin","",50,0,1);
    
    for(UInt_t j=0; j<num_files; j++) {
      h_project_massBin->Fill(h_errorDiff_class_compare->GetBinContent(j+1, project_bins[i]+1));
    }
    
    TString temp_xtitle = "Significances in mass bin ";
    temp_xtitle += project_bins[i]+1;
    TString temp_ytitle = "Entries";
    h_project_massBin->SetXTitle(temp_xtitle);
    h_project_massBin->SetYTitle(temp_ytitle);

    TCanvas *c_project_massBin = new TCanvas("c_project_massBin","",1024,768);
    h_project_massBin->Draw();
    
    TString temp_filename = output_prefix + "significanceErrorDiff_massBin";
    temp_filename += project_bins[i]+1;
    c_project_massBin->SaveAs(temp_filename + ".pdf");
    c_project_massBin->SaveAs(temp_filename + ".png");
    c_project_massBin->SaveAs(temp_filename + ".root");

    h_project_massBin->Delete();
  }

  gApplication->Terminate();
  
}
