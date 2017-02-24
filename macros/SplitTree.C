#include <iostream>

#include <TROOT.h>
#include <TString.h>
#include <TFile.h>
#include <TTree.h>

void SplitTree(TString fname, TString tname, TString frac, TString method) {

  // TString infileName = "../inputData/FT2_AnalysisResults_Upgrade_addFeat.root";
  TString infileName;
  if(fname.IsNull() || !fname.EndsWith(".root")) {
    std::cout << "  ERROR: You must provide a ROOT file name with the first argument."
	      << std::endl;
    exit(1);
  }else {
    infileName = fname;
  }


  TString treeName = "tracks";
  if(tname.IsNull()) {
    std::cout << "  Warning: No tree name provided. Default value (\""
	      << treeName << "\") will be used." << std::endl;
  }else {
    treeName = tname;
  }
  

  TString splitFraction = "1:1";
  if(frac.IsNull() || !frac.Contains(":")) {
    std::cout << "  Warning: Invalid format/value of second argument. "
	      << "  The default value \"1:1\" will be used." << std::endl;
  }else {
    splitFraction = frac;
  }

  TString temp_str = splitFraction;
  TString str1 = splitFraction.Remove(splitFraction.First(':'),
				      splitFraction.Length());
  TString str2 = temp_str.Remove(0,temp_str.Last(':')+1);
  Int_t part1, part2;
  if(!str1.IsDigit() || !str2.IsDigit()) {
    std::cout << "  Warning: invalid format of second argument. "
	      << "  (You should enter something like \"1:2\".)" << std::endl
	      << "  Default values will be used." << std::endl;
    part1 = 1; // default value
    part2 = 1; // default value
  }else {
    part1 = str1.Atoi();
    part2 = str2.Atoi();
  }

  if(method.IsNull()) {
    method = "train+test"; // default value
  }

  
  TFile *infile = new TFile(infileName, "READ");
  TTree *infileTree = (TTree*)infile->Get(treeName);

  TFile *outfile_train = new TFile("temp_output/output_splitTree_train.root", "RECREATE");
  TTree *splitTree_train = infileTree->CloneTree(0);

  TFile *outfile_test = new TFile("temp_output/output_splitTree_test.root", "RECREATE");
  TTree *splitTree_test = infileTree->CloneTree(0);
  
  
  Long64_t infileTree_nEvents = infileTree->GetEntries();
  Long64_t splitTree_train_nEvents = part1 * infileTree_nEvents / (part1+part2);
  Long64_t splitTree_test_nEvents = infileTree_nEvents - splitTree_train_nEvents;


  std::cout << "Input file: " << infileName << std::endl << std::endl;
  
  // create tree with training data:
  if(method.Contains("train")) {
    std::cout << std::endl << "Create tree with training data..." << std::endl;
    for(Long64_t ev=0; ev<splitTree_train_nEvents; ev++) {
      if((ev%1000)==0) {
	std::cout << "\rProcessing event " << ev << " of "
		  << splitTree_train_nEvents << " ("
		  << ev*100/splitTree_train_nEvents << "%)...";
      }

      infileTree->GetEntry(ev);

      splitTree_train->Fill();
    }
    std::cout << "\rProcessing event " << splitTree_train_nEvents
	      << " of " << splitTree_train_nEvents << " (100%)... DONE."
	      << std::endl << std::endl;
  
    outfile_train->Write();
  }

  
  // create tree with test data:
  if(method.Contains("test")) {
    std::cout << "Create tree with test data..." << std::endl;
    for(Int_t ev=splitTree_train_nEvents+1; ev<infileTree_nEvents; ev++) {
      if((ev%1000)==0) {
	std::cout << "\rProcessing event " << ev << " of "
		  << infileTree_nEvents << " ("
		  << (ev - splitTree_train_nEvents)*100/splitTree_test_nEvents
		  << "%)...";
      }

      infileTree->GetEntry(ev);

      splitTree_test->Fill();
    }
    std::cout << "\rProcessing event " << splitTree_test_nEvents
	      << " of " << splitTree_test_nEvents << " (100%)... DONE."
	      << std::endl;
    std::cout << std::endl;

    outfile_test->Write();
  }


  // Print summary:
  std::cout << "The following trees were created:" << std::endl;
  std::cout << "  Train tree containing " << splitTree_train_nEvents
	    << " events (" << splitTree_train_nEvents/((Float_t)infileTree_nEvents)
	    << "% of input tree)" << std::endl;
  std::cout << "  Test tree containing " << splitTree_test_nEvents
	    << " events (" << splitTree_test_nEvents/((Float_t)infileTree_nEvents)
	    << "% of input tree)" << std::endl;
  std::cout << std::endl;
  
}
