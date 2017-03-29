#include <iostream>

#include <TROOT.h>
#include <TApplication.h>
#include <TFile.h>
#include <TTree.h>


// stores all relevant information of a particle pair:
struct particlePair {
  Int_t EventID;
  Int_t TrackID1;
  Int_t TrackID2;
  Int_t IsTaggedAccepted;
};

// store all relevant information of a particle track:
struct particleTrack {
  Int_t EventID;
  Int_t TrackID;
};



void mergeProcessedMVAoutputs(TString branchfilename,
			      TString treename_branchfile,
			      TString branchname_branchfile,
			      TString updatefilename,
			      TString treename_updatefile,
			      TString branchname_updatefile,
			      Float_t MVAcut,
			      TString signalRegion = "+") {

  Float_t var;
  Int_t isAccepted;
  
  
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
  
  std::cout << "Tagging pairs based on MVA output...";
  for(Long64_t j=0; j<nentries; j++) {
    tree_updatefile->GetEntry(j);
    tree_branchfile->GetEntry(j);

    particlePair currentPair;
    currentPair.EventID = EventID;
    currentPair.TrackID1 = TrackID1;
    currentPair.TrackID2 = TrackID2;
    
    if(signalRegion == "+") {
      if(var > MVAcut) {
	currentPair.IsTaggedAccepted = 1.;

	particleTrack currentTrack1 = {EventID, TrackID1};
	particleTrack currentTrack2 = {EventID, TrackID2};
	tracksTaggedAccepted.push_back(currentTrack1);
	tracksTaggedAccepted.push_back(currentTrack2);
      }
      else currentPair.IsTaggedAccepted = 0.;
    }else if(signalRegion == "-") {
      if(var < MVAcut) {
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
  std::cout << "Number of accepted tracks: " << tracksTaggedAccepted.size() << std::endl;
  std::cout << std::endl;


  // std::vector<Long64_t> newEventsStartPos;
  // Long64_t EventID_temp = -1;
  // for(Long64_t i=0; i<tracksTaggedAccepted.size(); i++) {
  //   if(EventID_temp != tracksTaggedAccepted[i].EventID) {
  //     newEventsStartPos.push_back(i);
  //     EventID_temp = tracksTaggedAccepted[i].EventID;
  //   }
  // }

  // std::cout << "DEBUGINFO: newEventsStartPos.size() = " << newEventsStartPos.size() << std::endl;

  TStopwatch *watch = new TStopwatch();
  watch->Start();

  std::cout << "Propagate tag information to other pairs..." << std::endl;
  for(Long64_t i=0; i<nentries; i++) {
    if((i%1000)==0) {
      std::cout << "\r  Processing event " << i << " of " << nentries
		<< " (" << i/((Float_t)nentries)*100 << " %)...";
    }

    for(Long64_t j=0; j<tracksTaggedAccepted.size(); j++) {
      if( (allPairs[i].TrackID1==tracksTaggedAccepted[j].TrackID ||
	   allPairs[i].TrackID2==tracksTaggedAccepted[j].TrackID) &&
	  allPairs[i].EventID==tracksTaggedAccepted[j].EventID ) {
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


  // for(Long64_t j=0; j<tracksTaggedAccepted.size(); j++) {
  //   for(Long64_t i=0; i<nentries; i++) {
  //     if( allPairs[i].TrackID1==tracksTaggedAccepted[j] ||
  // 	  allPairs[i].TrackID2==tracksTaggedAccepted[j] ) {
  // 	allPairs[i].IsTaggedAccepted = 1.;
  //     }
  //   }
  // }
  // std::cout << " DONE" << std::endl << std::endl;


  
  // std::cout << "Fill new branch...";
  // for(Long64_t i=0; i<nentries; i++) {
  //   isAccepted = allPairs[i].IsTaggedAccepted;
  //   newBranch->Fill();
  // }
  // std::cout << " DONE" << std::endl << std::endl;
  
  
  /*
    for(Long64_t i=0; i<nentries; i++) {
    tree_branchfile->GetEntry(i);
    if(signalRegion == "+") {
    if(var > MVAcut) isAccepted = 1.;
    else isAccepted = 0.;
    }else if(signalRegion == "-") {
    if(var < MVAcut) isAccepted = 1.;
    else isAccepted = 0.;
    }else {
    std::cout << "  ERROR: 'signalRegion' definition is wrong or not provided. "
    << "(It should be either '+' (default) or '-'.)" << std::endl;
    std::cout << "  Abort." << std::endl << std::endl;
    gApplication->Terminate();
    }
    
    newBranch->Fill();
    }
  */
  
  updatefile->cd();
  tree_updatefile->Write("", TObject::kOverwrite);
  std::cout << "File " << updatefile->GetName() << " updated." << std::endl;

  
  gApplication->Terminate();
  
}
