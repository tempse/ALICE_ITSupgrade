#include <iostream>

#include <TROOT.h>
#include <TSystem.h>
#include <TDirectory.h>
#include <TSystemDirectory.h>
#include <TList.h>
#include <TString.h>
#include <TFile.h>
#include <TChain.h>
#include <TTree.h>

void CountEvents() {

  // directory containing the input ROOT files
  const char *input_dirname = "~/analysis/data/FT2_AnalysisResults_Upgrade/workingData/DNNAnalysis/";
  const char *file_ext = "FT2_ITSup_singleTree_part2_1-9-split.root";

  // tree name
  TString treeName = "tracks";

  // branch name with event IDs in case test_sample_size is type int
  TString ev_id_branchname = "EventID1_unique";

  // branch name with information about the looseness of the track cuts
  TString trackCut_branchname = "isTrackCut";
  
  TSystemDirectory input_dir(input_dirname, input_dirname);
  TList *input_files = input_dir.GetListOfFiles();
  
  TChain *infileTree = new TChain(treeName);
  if(input_files) {
    std::cout << "Reading \"*" << file_ext << "\" files from \""
	      << input_dirname << "\"..." << std::endl;
    TSystemFile *file;
    TString fname;
    TIter next(input_files);
    while((file = (TSystemFile*)next())) {
      fname = file->GetName();
      if(!file->IsDirectory() && fname.EndsWith(file_ext)) {
	infileTree->Add(input_dirname + fname);
	std::cout << "  Imported file " << fname << std::endl;
      }
    }
  }
  
  
  // TFile *infile = new TFile("~/analysis/code/FT2_AnalysisResults_Upgrade/macros/dataManipulation/temp_output/output_splitTree_train.root","READ");
  // TTree *infileTree = (TTree*)infile->Get(treeName);


  Int_t ev_id;
  Int_t TrackCut;
  infileTree->SetBranchAddress(ev_id_branchname, &ev_id);
  //infileTree->SetBranchAddress(trackCut_branchname, &TrackCut);
  
  Int_t infileTree_nEntries = infileTree->GetEntries();
  std::cout << "Processing " << infileTree_nEntries << " entries..." << std::endl;

  Int_t infileTree_nEntries_1percent = infileTree_nEntries / 100.;

  Long64_t cnt_percent = 0;

  Long64_t ev_id_prev = -99;
  Long64_t cnt_passedEvents = -1;
  Long64_t cnt_descendingID = 0, cnt_largeIncrement = -1, cnt_largerIncrement = -1;

  std::cout << "Counting events..." << std::endl;

  for(Long64_t en=0; en<infileTree_nEntries; en++) {
    if((en%infileTree_nEntries_1percent)==0) {
      std::cout << "\r  Processing... (" << cnt_percent << "%)";
      cnt_percent++;
    }

    infileTree->GetEntry(en);

    if(ev_id != ev_id_prev) {
      /*if(TrackCut == 2)*/ cnt_passedEvents++;
      if(ev_id < ev_id_prev) cnt_descendingID++;
      if(ev_id != ev_id_prev+1) cnt_largeIncrement++;
      if(ev_id > ev_id_prev+2) cnt_largerIncrement++;
      
      ev_id_prev = ev_id;
    }
  }
  std::cout << std::endl;
  
  std::cout << "Number of (standard-cut) events: " << cnt_passedEvents << std::endl;
  std::cout << "Occurrences of descending event IDs: " << cnt_descendingID << std::endl;
  std::cout << "Occurrences of ID increments != 1: " << cnt_largeIncrement << std::endl;
  std::cout << "Occurrences of ID increments > 2: " << cnt_largerIncrement << std::endl;

  gSystem->Exit(0);

}
