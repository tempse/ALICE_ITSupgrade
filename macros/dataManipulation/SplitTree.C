#include <iostream>

#include <TROOT.h>
#include <TSystem.h>
#include <TDirectory.h>
#include <TSystemDirectory.h>
#include <TSystemFile.h>
#include <TList.h>
#include <TString.h>
#include <TFile.h>
#include <TChain.h>
#include <TTree.h>

void SplitTree() {
  
  // directory containing the input ROOT files
  const char *input_dirname = "~/analysis/data/FT2_AnalysisResults_Upgrade/inputData/FT2_AnalysisResults_wLooseTrackCuts_iGeo12.root/";
  const char *file_ext = ".root";

  // tree name
  TString treeName = "outputITSup/tracks";

  // size of test sample (int/float)  
  TString test_sample_size = "538273";

  // choose what to write to disk ("train"/"test"/"train+test")
  TString write_samples = "train+test";

  // path to output folder
  TString output_prefix = "temp_output/";

  // branch name with event IDs in case test_sample_size is type int
  TString ev_id_branchname = "event";

  // branch name with information about the looseness of the track cuts
  TString trackCut_branchname = "isTrackCut";

  

  if(test_sample_size.IsNull() || !test_sample_size.IsFloat()) {
    std::cout << "  Error: Invalid format or "
	      << "missing value of variable 'test_sample_size'."
	      << std::endl;
    gSystem->Exit(1);
  }

  Long64_t num_events_testsample;
  Float_t frac_events_testsample;
  Bool_t doSplitByEvent;
  
  if(!test_sample_size.IsNull()) {
    if(test_sample_size.IsDigit()) {
      doSplitByEvent = kTRUE;
      num_events_testsample = test_sample_size.Atoi();
    }else if(test_sample_size.IsFloat()) {
      doSplitByEvent = kFALSE;
      frac_events_testsample = test_sample_size.Atof();
    }else {
      std::cout << "  Error: invalid format of variable 'test_sample_size', "
		<< "  has to be either int or float." << std::endl;
      gSystem->Exit(1);
    }
  }else {
    std::cout << "  Error: Missing argument: 'test_sample_size.'"
	      << std::endl;
    gSystem->Exit(1);
  }

  if(doSplitByEvent && ev_id_branchname.IsNull()) {
    std::cout << "  Error: Missing argument: 'ev_id_branchname'." << std::endl;
    gSystem->Exit(1);
  }


  TSystemDirectory input_dir(input_dirname, input_dirname);
  TList *input_files = input_dir.GetListOfFiles();
  
  TChain *infileTree = new TChain(treeName);
  if(input_files) {
    std::cout << "Reading \"*" << file_ext << "\" files from \""
	      << input_dirname << "\"...";
    TSystemFile *file;
    TString fname;
    TIter next(input_files);
    while((file = (TSystemFile*)next())) {
      fname = file->GetName();
      if(!file->IsDirectory() && fname.EndsWith(file_ext)) {
	infileTree->Add(input_dirname + fname);
      }
    }
  }
  std::cout << "DONE" << std::endl;
  
  
  Long64_t ev_id;
  Int_t isTrackCut;
  if(doSplitByEvent) infileTree->SetBranchAddress(ev_id_branchname, &ev_id);
  if(doSplitByEvent) infileTree->SetBranchAddress(trackCut_branchname, &isTrackCut);

  
  TFile *outfile_part1 = new TFile(output_prefix + "output_splitTree_train.root", "RECREATE");
  TTree *splitTree_part1 = infileTree->CloneTree(0);

  TFile *outfile_part2 = new TFile(output_prefix + "output_splitTree_test.root", "RECREATE");
  TTree *splitTree_part2 = infileTree->CloneTree(0);


  Long64_t infileTree_nEntries = infileTree->GetEntries();

  Bool_t doTrainSplit = (write_samples.Contains("train")) ? kTRUE : kFALSE;
  Bool_t doTestSplit = (write_samples.Contains("test")) ? kTRUE : kFALSE;
  
  
  if(!doSplitByEvent) {

    std::cout << "Split criterion: fraction (test_sample_size: "
	      << frac_events_testsample << ")" << std::endl;
    
    Long64_t splitTree_part2_nEntries = (Long64_t)(frac_events_testsample * infileTree_nEntries);
    Long64_t splitTree_part1_nEntries = infileTree_nEntries - splitTree_part2_nEntries;
  
    // create first tree:
    if(doTrainSplit) {
      std::cout << std::endl << "Processing train split..." << std::endl;
      for(Long64_t ev=0;
	  ev<splitTree_part1_nEntries;
	  ev++) {
	if((ev%1000)==0) {
	  std::cout << "\rProcessing event " << ev << " of "
		    << splitTree_part1_nEntries << " ("
		    << ev*100/splitTree_part1_nEntries << "%)...";
	}

	infileTree->GetEntry(ev);

	splitTree_part1->Fill();
      }
      std::cout << "\rProcessing event " << splitTree_part1_nEntries
		<< " of " << splitTree_part1_nEntries << " (100%)... DONE."
		<< std::endl << std::endl;
    }

    outfile_part1->cd();
    splitTree_part1->Write(0, TObject::kOverwrite);

    
    // create second tree:
    
    if(doTestSplit) {
      std::cout << "Processing test split..." << std::endl;
      for(Long64_t ev=splitTree_part1_nEntries + 1;
	  ev<splitTree_part1_nEntries + splitTree_part2_nEntries;
	  ev++) {
	if((ev%1000)==0) {
	  std::cout << "\rProcessing event " << ev << " of "
		    << infileTree_nEntries << " ("
		    << (ev - splitTree_part1_nEntries)*100/splitTree_part2_nEntries
		    << "%)...";
	}

	infileTree->GetEntry(ev);

	splitTree_part2->Fill();
      }

      outfile_part2->cd();
      splitTree_part2->Write(0, TObject::kOverwrite);
      
      std::cout << "\rProcessing event " << splitTree_part2_nEntries
		<< " of " << splitTree_part2_nEntries << " (100%)... DONE."
		<< std::endl;
      std::cout << std::endl;
    }
    
  }else { // <-> if(doSplitByEvent)

    std::cout << "Split criterion: event number (test sample event number: "
	      << num_events_testsample << ")" << std::endl;
    
    Long64_t splitTree_testSample_nEvents = num_events_testsample;


    if(splitTree_testSample_nEvents >= infileTree_nEntries) {
      std::cout << "  Error: Event number in test sample (" << num_events_testsample
		<< ") exceeds total number of entries (" << infileTree_nEntries << ")."
		<< std::endl;
      gSystem->Exit(1);
    }

    Long64_t infileTree_nEntries_1percent = (Long64_t)(infileTree_nEntries/100);

    Long64_t cnt_percent = 0;

    Long64_t ev_id_prev = -1;
    Long64_t cnt_passedEvents = 0;
    Long64_t cnt_trainEntries = 0, cnt_testEntries = 0;

    Bool_t doCountEvent = kFALSE;

    std::cout << "Splitting the data..." << std::endl;
    
    for(Long64_t en=0; en<infileTree_nEntries; en++) {
      if((en%infileTree_nEntries_1percent)==0) {
	std::cout << "\r  Processing... (" << cnt_percent << "%)";
	cnt_percent++;
      }

      infileTree->GetEntry(en);

      // monitor whether there is a "standard-cut" track in the event
      if(isTrackCut == 2) doCountEvent = kTRUE;

      if(ev_id != ev_id_prev) {
	if(doCountEvent) cnt_passedEvents++;
	ev_id_prev = ev_id;
	doCountEvent = kFALSE;
      }

      if(cnt_passedEvents < splitTree_testSample_nEvents) {
	if(doTestSplit) splitTree_part2->Fill();
	cnt_testEntries++;
      }else {
	if(doTrainSplit) splitTree_part1->Fill();
	cnt_trainEntries++;
      }
    }
    std::cout << std::endl;
    
    std::cout << "Number of entries in training sample: " << cnt_trainEntries << std::endl
	      << "Number of entries in test sample:     " << cnt_testEntries << std::endl;
    

    
    outfile_part1->cd();
    splitTree_part1->Write(0, TObject::kOverwrite);

    outfile_part2->cd();
    splitTree_part2->Write(0, TObject::kOverwrite);

  }
    

  gSystem->Exit(1);
}
