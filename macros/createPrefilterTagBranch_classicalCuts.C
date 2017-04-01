#include <iostream>

#include <TROOT.h>
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

  bool operator < (const particlePair &rhs) const {
    return (EventID < rhs.EventID);
  }
};

// store all relevant information of a particle track:
struct particleTrack {
  Int_t EventID;
  Int_t TrackID;

  bool operator < (const particleTrack &rhs) const {
    return (EventID < rhs.EventID);
  }
};



void createPrefilterTagBranch_classicalCuts(TString updatefilename,
					    TString treename_updatefile,
					    TString branchname_read,
					    TString branchname_add,
					    TString signalRegion = "+") {
  
  Int_t var;
  Int_t isAccepted;
  
  
  TFile *updatefile = new TFile(updatefilename, "UPDATE");
  TTree *tree_updatefile = (TTree*)updatefile->Get(treename_updatefile);
  Int_t EventID, TrackID1, TrackID2;
  tree_updatefile->SetBranchAddress("EventID1", &EventID);
  tree_updatefile->SetBranchAddress("TrackID1", &TrackID1);
  tree_updatefile->SetBranchAddress("TrackID2", &TrackID2);
  tree_updatefile->SetBranchAddress(branchname_read, &var);
  if(tree_updatefile->GetListOfBranches()->FindObject(branchname_add) != NULL) {
    std::cout << "  ERROR: A branch named " << branchname_add
	      << " already exists in file " << updatefile->GetName() << std::endl;
    gApplication->Terminate();
  }
  TString branchname_add_vartype = branchname_add + "/I";
  TBranch *newBranch = tree_updatefile->Branch(branchname_add,
					       &isAccepted,
					       branchname_add_vartype);


  // Collection of all particle pairs (of all events):
  std::vector<particlePair> allPairs;

  // Collection of all accepted particle tracks (of all events):
  std::vector<particleTrack> tracksTaggedAccepted;

  
  const Long64_t nentries = tree_updatefile->GetEntries();

  std::cout << "Tagging real pairs based on cut values...";
  
  for(Long64_t j=0; j<nentries; j++) {
    tree_updatefile->GetEntry(j);
    
    particlePair currentPair;
    currentPair.EventID = EventID;
    currentPair.TrackID1 = TrackID1;
    currentPair.TrackID2 = TrackID2;
    
    if(signalRegion == "+") {
      if(var == 1) {
	currentPair.IsTaggedAccepted = 1.;

	particleTrack currentTrack1 = {EventID, TrackID1};
	particleTrack currentTrack2 = {EventID, TrackID2};
	tracksTaggedAccepted.push_back(currentTrack1);
	tracksTaggedAccepted.push_back(currentTrack2);
      }
      else currentPair.IsTaggedAccepted = 0.;
    }else if(signalRegion == "-") {
      if(var == 0) {
	currentPair.IsTaggedAccepted = 1.;

	particleTrack currentTrack1 = {EventID, TrackID1};
	particleTrack currentTrack2 = {EventID, TrackID2};
	tracksTaggedAccepted.push_back(currentTrack1);
	tracksTaggedAccepted.push_back(currentTrack2);
      }
      else currentPair.IsTaggedAccepted = 0.;
    }else {
      std::cout << "  ERROR: 'signalRegion' definition is wrong. "
		<< "(It should be either '+' (default) or '-'.)" << std::endl;
      std::cout << "  Abort." << std::endl << std::endl;
      gApplication->Terminate();
    }

    allPairs.push_back(currentPair);
  }
  std::cout << " DONE" << std::endl;

  std::cout << "Number of pairs: " << allPairs.size() << std::endl;
  std::cout << "Number of accepted pairs: " << tracksTaggedAccepted.size()/2 << std::endl;
  std::cout << std::endl;


  // sort pairs and tracks by event ID:
  std::sort(allPairs.begin(), allPairs.end());
  std::sort(tracksTaggedAccepted.begin(), tracksTaggedAccepted.end());

  // store start positions of new events:
  std::map<Long64_t, Long64_t> eventID_startPos;
  Long64_t EventID_prev = -1;
  for(Long64_t i=0; i<(Long64_t)tracksTaggedAccepted.size(); i++) {
    if(tracksTaggedAccepted[i].EventID != EventID_prev) {
      eventID_startPos[tracksTaggedAccepted[i].EventID] = i;
    }
    EventID_prev = tracksTaggedAccepted[i].EventID;
  }
  

  std::cout << "Propagate tag information to other pairs..." << std::endl;
  
  TStopwatch *watch = new TStopwatch();
  watch->Start();

  for(Long64_t i=0; i<nentries; i++) {
    if((i%1000)==0) {
      std::cout << "\r  Processing event " << i << " of " << nentries
		<< " (" << i/((Float_t)nentries)*100 << " %)...";
    }
    
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
  
  

  std::cout << "Fill new branch with appropriate tags...";
  for(Long64_t i=0; i<nentries; i++) {
    isAccepted = allPairs[i].IsTaggedAccepted;
    newBranch->Fill();
  }
  std::cout << " DONE" << std::endl << std::endl;

  
  updatefile->cd();
  tree_updatefile->Write("", TObject::kOverwrite);
  std::cout << "File " << updatefile->GetName() << " updated." << std::endl;

  gApplication->Terminate();
  
}
