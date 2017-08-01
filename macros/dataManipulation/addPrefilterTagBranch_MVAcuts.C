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
  Int_t EventID;
  Int_t TrackID1;
  Int_t TrackID2;
  Int_t IsTaggedAccepted;
  Long64_t originalPosition;

  // bool operator < (const particlePair &rhs) const {
  //   return (EventID < rhs.EventID);
  // }
};

bool sortPairsByEventID(const particlePair &lhs, const particlePair &rhs) {
  return lhs.EventID < rhs.EventID;
}

bool sortByOriginalPosition(const particlePair &lhs, const particlePair &rhs) {
  return lhs.originalPosition < rhs.originalPosition;
}

// store all relevant information of a particle track:
struct particleTrack {
  Int_t EventID;
  Int_t TrackID;

  // bool operator < (const particleTrack &rhs) const {
  //   return (EventID < rhs.EventID);
  // }
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
  Int_t EventID, TrackID1, TrackID2;
  tree_updatefile->SetBranchAddress("EventID1", &EventID);
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
    currentPair.originalPosition = j;
    
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


  // sort pairs and tracks by event ID:
  std::cout << "Sorting data...";
  std::sort(allPairs.begin(), allPairs.end(), sortPairsByEventID);
  std::sort(tracksTaggedAccepted.begin(), tracksTaggedAccepted.end(), sortTracksByEventID);
  std::cout << " DONE." << std::endl;
  

  // store start positions of new events:
  std::cout << "Finding new event start positions...";
  std::map<Long64_t, Long64_t> eventID_startPos;
  Long64_t EventID_prev = -1;
  for(Long64_t i=0; i<(Long64_t)tracksTaggedAccepted.size(); i++) {
    if(tracksTaggedAccepted[i].EventID != EventID_prev) {
      eventID_startPos[tracksTaggedAccepted[i].EventID] = i;
    }
    EventID_prev = tracksTaggedAccepted[i].EventID;
  }
  std::cout << " DONE." << std::endl;
  


  std::cout << "Propagate tag information to other pairs..." << std::endl;
  
  TStopwatch *watch = new TStopwatch();
  watch->Start();

  for(Long64_t i=0; i<nentries; i++) {
    if((i%1000)==0) {
      std::cout << "\r  Processing event " << i << " of " << nentries
		<< " (" << i/((Float_t)nentries)*100 << " %)...";
    }
    
    if( allPairs[i].IsTaggedAccepted == -999 ) continue; // skip irrelevant entries
    
    for(Long64_t j=eventID_startPos[allPairs[i].EventID];
	j<(Long64_t)tracksTaggedAccepted.size();
	j++) {
      if( allPairs[i].EventID != tracksTaggedAccepted[j].EventID ) {
	break;
      }else if(allPairs[i].TrackID1==tracksTaggedAccepted[j].TrackID ||
	       allPairs[i].TrackID2==tracksTaggedAccepted[j].TrackID) {
	allPairs[i].IsTaggedAccepted = 1.;
      }
    }
  }
  std::cout << "\r  Processing event " << nentries << " of " << nentries
	    << " (100 %)... DONE.";
  std::cout << " Time elapsed: " << watch->RealTime() << " seconds."
	    << std::endl << std::endl;


  // restore original order of allPairs vector:
  std::cout << "Restoring original order of data...";
  std::sort(allPairs.begin(), allPairs.end(), sortByOriginalPosition);
  std::cout << " DONE." << std::endl;
  

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

  
  gApplication->Terminate();
  
}