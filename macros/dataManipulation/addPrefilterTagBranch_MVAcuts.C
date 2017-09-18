#include <iostream>
#include <algorithm>

#include <TROOT.h>
#include <TMath.h>
#include <TApplication.h>
#include <TFile.h>
#include <TTree.h>
#include <TStopwatch.h>


// stores all relevant information of a particle pair:
struct particlePair {
  Long64_t EventID;
  Int_t TrackID1;
  Int_t TrackID2;
  Int_t IsTaggedAccepted;
};

bool sortPairsByEventID(const particlePair &lhs, const particlePair &rhs) {
  return lhs.EventID < rhs.EventID;
}

// store all relevant information of a particle track:
struct particleTrack {
  Long64_t EventID;
  Int_t TrackID;
};

bool sortTracksByEventID(const particleTrack &lhs, const particleTrack &rhs) {
  return lhs.EventID < rhs.EventID;
}



void addPrefilterTagBranch_MVAcuts(TString branchfilename,
				   TString treename_branchfile,
				   TString branchname_branchfile,
				   TString updatefilename,
				   TString treename_updatefile,
				   TString branchname_updatefile,
				   Float_t MVAcut,
				   TString signalRegion = "+") {

  Float_t var;
  Int_t isAccepted;
  

  std::cout << "Reading file...";
  TFile *updatefile = new TFile(updatefilename, "UPDATE");
  TTree *tree_updatefile = (TTree*)updatefile->Get(treename_updatefile);
  Long64_t EventID;
  Int_t TrackID1, TrackID2;
  tree_updatefile->SetBranchAddress("EventID1_unique", &EventID);
  tree_updatefile->SetBranchAddress("TrackID1", &TrackID1);
  tree_updatefile->SetBranchAddress("TrackID2", &TrackID2);
  if(tree_updatefile->GetListOfBranches()->FindObject(branchname_updatefile) != NULL) {
    std::cout << "  ERROR: A branch named " << branchname_updatefile
        << " already exists in file " << updatefile->GetName() << std::endl;
    gApplication->Terminate();
  }
  std::cout << " DONE." << std::endl;
  
  TString branchname_updatefile_vartype = branchname_updatefile + "/I";
  TBranch *newBranch = tree_updatefile->Branch(branchname_updatefile,
  				       &isAccepted,
  				       branchname_updatefile_vartype);
  
  
  TFile *branchfile = new TFile(branchfilename, "READ");
  TTree *tree_branchfile = (TTree*)branchfile->Get(treename_branchfile);
  tree_branchfile->SetBranchAddress(branchname_branchfile, &var);
  

  if(tree_updatefile->GetEntries() != tree_branchfile->GetEntries()) {
    std::cout << "  ERROR: Trees have different sizes." << std::endl
	      << "    Size of tree " << treename_updatefile << " in "
	      << updatefilename << ": " << tree_updatefile->GetEntries()
	      << std::endl
	      << "    Size of tree " << treename_branchfile << " in "
	      << branchfilename << ": " << tree_branchfile->GetEntries()
	      << std::endl << std::endl;
    gApplication->Terminate();
  }


  // TFile *outfile = new TFile("output_prefilterTagBranchMVA.root","RECREATE");
  // TTree *tree_outfile = new TTree("prefilterTags","prefilterTags");
  // tree_outfile->Branch(branchname_updatefile, &isAccepted, branchname_updatefile + "/I");


  // Collection of all particle pairs (of all events):
  std::vector<particlePair> allPairs;

  // Collection of all accepted particle tracks (of all events):
  std::vector<particleTrack> tracksTaggedAccepted;

  
  const Long64_t nentries = tree_updatefile->GetEntries();

  std::cout << "Tagging pairs based on MVA output..." << std::endl;
  
  for(Long64_t j=0; j<nentries; j++) {
    if((j%1000)==0) {
      std::cout << "\r  Processing event " << j << " of " << nentries
		<< " (" << j/((Float_t)nentries)*100 << " %)...";
    }


    tree_updatefile->GetEntry(j);
    tree_branchfile->GetEntry(j);
    
    particlePair currentPair;
    currentPair.EventID = EventID;
    currentPair.TrackID1 = TrackID1;
    currentPair.TrackID2 = TrackID2;
    
    if(signalRegion == "+") {
      if(var < -1 || var > 1) { // check if previously tagged as non-accepted
	currentPair.IsTaggedAccepted = -999;
      }else if(var > MVAcut) {
	currentPair.IsTaggedAccepted = 1.;

	particleTrack currentTrack1 = {EventID, TrackID1};
	particleTrack currentTrack2 = {EventID, TrackID2};
	tracksTaggedAccepted.push_back(currentTrack1);
	tracksTaggedAccepted.push_back(currentTrack2);
      }else currentPair.IsTaggedAccepted = 0.;
    }else if(signalRegion == "-") {
      if(var < -1 || var > 1) { // check if previously tagged as non-accepted
	currentPair.IsTaggedAccepted = -999;
      }else if(var < MVAcut) {
	currentPair.IsTaggedAccepted = 1.;

	particleTrack currentTrack1 = {EventID, TrackID1};
	particleTrack currentTrack2 = {EventID, TrackID2};
	tracksTaggedAccepted.push_back(currentTrack1);
	tracksTaggedAccepted.push_back(currentTrack2);
      }else currentPair.IsTaggedAccepted = 0.;
    }else {
      std::cout << "  ERROR: 'signalRegion' definition is wrong. "
		<< "(It should be either '+' (default) or '-'.)" << std::endl;
      std::cout << "  Abort." << std::endl << std::endl;
      gApplication->Terminate();
    }

    allPairs.push_back(currentPair);
  }
  std::cout << "\r  Processing event " << nentries << " of " << nentries
	    << " (100 %)... DONE" << std::endl;

  std::cout << "Number of pairs: " << allPairs.size() << std::endl;
  std::cout << "Number of accepted tracks: " << tracksTaggedAccepted.size() << std::endl;
  std::cout << std::endl;


  Int_t EventID_prev = -1;

  Int_t pairs_startPos = 0, accTracks_startPos = 0;
  Int_t accTracks_nextStartPos;


  std::cout << "Propagating tag information to other pairs in the respective event..."
	    << std::endl;
  
  while(pairs_startPos < nentries) {
    std::cout << "\r  (" << pairs_startPos << " / " << nentries << ")";

    while(allPairs[pairs_startPos].EventID == EventID_prev) {

      for(Int_t i=accTracks_startPos; i<tracksTaggedAccepted.size(); i++) {
	if(tracksTaggedAccepted[i].EventID != allPairs[pairs_startPos].EventID) {
	  accTracks_nextStartPos = i;
	  break;
	}else if(allPairs[pairs_startPos].TrackID1 == tracksTaggedAccepted[i].TrackID ||
		 allPairs[pairs_startPos].TrackID2 == tracksTaggedAccepted[i].TrackID) {
	  allPairs[pairs_startPos].IsTaggedAccepted = 1;
	}
      }

      pairs_startPos++;
    }

    accTracks_startPos = accTracks_nextStartPos;
    EventID_prev = allPairs[pairs_startPos].EventID;
    
  }


  std::cout << "Fill new branch with appropriate tags...";
  for(Long64_t i=0; i<nentries; i++) {
    isAccepted = allPairs[i].IsTaggedAccepted;
    newBranch->Fill();
  }
  std::cout << " DONE" << std::endl << std::endl;

  
  updatefile->cd();
  std::cout << "Write data...";
  tree_updatefile->Write("", TObject::kOverwrite);
  std::cout << " DONE." << std::endl;
  std::cout << "File " << updatefile->GetName() << " updated." << std::endl;

  
  gSystem->Exit(0);
  
}
