#include <iostream>

#include <TROOT.h>
#include <TApplication.h>
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
  

  TString splitFraction = "1:1:8";
  if(frac.IsNull() || !frac.Contains(":")) {
    std::cout << "  Warning: Invalid format or missing value of third argument. "
	      << "  The default value \"1:1:8\" will be used." << std::endl;
  }else {
    splitFraction = frac;
  }

  TString temp1_str = splitFraction, temp2_str = splitFraction;
  TString str1 = splitFraction.Remove(splitFraction.First(':'),
				      splitFraction.Length());
  TString str2 = temp1_str.Remove(0,temp1_str.First(':')+1);
  str2.Remove(str2.Last(':'), str2.Length());
  TString str3 = temp2_str.Remove(0, temp2_str.Last(':')+1);
  Int_t part1, part2, part3;
  if(!frac.IsNull() && (!str1.IsDigit() || !str2.IsDigit() || !str3.IsDigit())) {
    std::cout << "  Warning: invalid format of third argument. "
	      << "  (You should enter something like \"1:1:8\".)" << std::endl
	      << "  Default values will be used." << std::endl;
    part1 = 1; // default value
    part2 = 1; // default value
    part3 = 8; // default value
  }else {
    part1 = str1.Atoi();
    part2 = str2.Atoi();
    part3 = str3.Atoi();
  }


  if(method.IsNull()) {
    method = "1+2+3"; // default value
  }

  
  TFile *infile = new TFile(infileName, "READ");
  TTree *infileTree = (TTree*)infile->Get(treeName);

  TFile *outfile_part1 = new TFile("temp_output/output_splitTree_part1.root", "RECREATE");
  TTree *splitTree_part1 = infileTree->CloneTree(0);

  TFile *outfile_part2 = new TFile("temp_output/output_splitTree_part2.root", "RECREATE");
  TTree *splitTree_part2 = infileTree->CloneTree(0);

  TFile *outfile_part3 = new TFile("temp_output/output_splitTree_part3.root", "RECREATe");
  TTree *splitTree_part3 = infileTree->CloneTree(0);
  
  
  Long64_t infileTree_nEvents = infileTree->GetEntries();
  Long64_t splitTree_part1_nEvents = part1 * infileTree_nEvents / (part1+part2+part3);
  Long64_t splitTree_part2_nEvents = part2 * infileTree_nEvents / (part1+part2+part3);
  Long64_t splitTree_part3_nEvents = part3 * infileTree_nEvents / (part1+part2+part3);


  std::cout << "Input file: " << infileName << std::endl << std::endl;
  
  // create first tree:
  if(method.Contains("1")) {
    std::cout << std::endl << "Processing first part of the tree..." << std::endl;
    for(Long64_t ev=0;
	ev<splitTree_part1_nEvents;
	ev++) {
      if((ev%1000)==0) {
	std::cout << "\rProcessing event " << ev << " of "
		  << splitTree_part1_nEvents << " ("
		  << ev*100/splitTree_part1_nEvents << "%)...";
      }

      infileTree->GetEntry(ev);

      splitTree_part1->Fill();
    }
    std::cout << "\rProcessing event " << splitTree_part1_nEvents
	      << " of " << splitTree_part1_nEvents << " (100%)... DONE."
	      << std::endl << std::endl;
  
    outfile_part1->Write();
    outfile_part1->Close();
  }

  
  // create second tree:
  if(method.Contains("2")) {
    std::cout << "Processing second part of the tree..." << std::endl;
    for(Long64_t ev=splitTree_part1_nEvents + 1;
	ev<splitTree_part1_nEvents + splitTree_part2_nEvents;
	ev++) {
      if((ev%1000)==0) {
	std::cout << "\rProcessing event " << ev << " of "
		  << infileTree_nEvents << " ("
		  << (ev - splitTree_part1_nEvents)*100/splitTree_part2_nEvents
		  << "%)...";
      }

      infileTree->GetEntry(ev);

      splitTree_part2->Fill();
    }
    std::cout << "\rProcessing event " << splitTree_part2_nEvents
	      << " of " << splitTree_part2_nEvents << " (100%)... DONE."
	      << std::endl;
    std::cout << std::endl;

    outfile_part2->Write();
    outfile_part2->Close();
  }


  // create third tree:
  if(method.Contains("3")) {
    std::cout << "Processing third part of the tree..." << std::endl;
    for(Long64_t ev=splitTree_part1_nEvents + splitTree_part2_nEvents + 1;
	ev<splitTree_part1_nEvents + splitTree_part2_nEvents + splitTree_part3_nEvents;
	ev++) {
      if((ev%1000)==0) {
	std::cout << "\rProcessing event " << ev << " of "
		  << infileTree_nEvents << " ("
		  << (ev - splitTree_part1_nEvents - splitTree_part2_nEvents)*100/splitTree_part3_nEvents
		  << "%)...";
      }

      infileTree->GetEntry(ev);

      splitTree_part3->Fill();
    }
    std::cout << "\rProcessing event " << splitTree_part3_nEvents
	      << " of " << splitTree_part3_nEvents << " (100%)... DONE."
	      << std::endl;
    std::cout << std::endl;

    outfile_part3->Write();
    outfile_part3->Close();
  }


  // Print summary:
  std::cout << "The following trees were created:" << std::endl;
  if(method.Contains("1"))
    std::cout << "  1st tree containing " << splitTree_part1_nEvents
	    << " events (" << splitTree_part1_nEvents/((Float_t)infileTree_nEvents)*100
	    << "% of input tree)" << std::endl;
  if(method.Contains("2"))
    std::cout << "  2nd tree containing " << splitTree_part2_nEvents
	    << " events (" << splitTree_part2_nEvents/((Float_t)infileTree_nEvents)*100
	    << "% of input tree)" << std::endl;
  if(method.Contains("3"))
    std::cout << "  3rd tree containing " << splitTree_part3_nEvents
	    << " events (" << splitTree_part3_nEvents/((Float_t)infileTree_nEvents)*100
	    << "% of input tree)" << std::endl;
  std::cout << std::endl;


  gApplication->Terminate();
}
