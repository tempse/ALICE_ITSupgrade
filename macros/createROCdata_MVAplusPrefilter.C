#include <iostream>
#include <algorithm>

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
  Int_t IsTrueConv;
  Int_t IsTrueRP;
  Long64_t originalPosition;
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
};

bool sortTracksByEventID(const particleTrack &lhs, const particleTrack &rhs) {
  return lhs.EventID < rhs.EventID;
}



void createROCdata_MVAplusPrefilter(TString MVAoutput_filename,
				    TString treename_MVAoutput_file,
				    TString branchname_MVAoutput_file,
				    TString MCdatafilename,
				    TString treename_MCdatafile,
				    TString signalRegion = "+",
				    Int_t   num_steps = 10,
				    Float_t MVAoutputRange_min = -1.,
				    Float_t MVAoutputRange_max = 1.) {

  Float_t var;
  Int_t isAccepted;
  
  
  std::cout << "Reading file...";
  TFile *MCdatafile = new TFile(MCdatafilename, "READ");
  TTree *tree_MCdatafile = (TTree*)MCdatafile->Get(treename_MCdatafile);
  Int_t EventID, TrackID1, TrackID2;
  Int_t IsConv, IsRP;
  tree_MCdatafile->SetBranchAddress("EventID1", &EventID);
  tree_MCdatafile->SetBranchAddress("TrackID1", &TrackID1);
  tree_MCdatafile->SetBranchAddress("TrackID2", &TrackID2);
  tree_MCdatafile->SetBranchAddress("IsConv", &IsConv);
  tree_MCdatafile->SetBranchAddress("IsRP", &IsRP);
  
  // if(tree_MCdatafile->GetListOfBranches()->FindObject(branchname_MCdatafile) != NULL) {
  //   std::cout << "  ERROR: A branch named " << branchname_MCdatafile
  // 	      << " already exists in file " << MCdatafile->GetName() << std::endl;
  //   gApplication->Terminate();
  // }
  std::cout << " DONE." << std::endl;


  TFile *MVAoutput_file = new TFile(MVAoutput_filename, "READ");
  TTree *tree_MVAoutput_file = (TTree*)MVAoutput_file->Get(treename_MVAoutput_file);
  tree_MVAoutput_file->SetBranchAddress(branchname_MVAoutput_file, &var);
  

  if(tree_MCdatafile->GetEntries() != tree_MVAoutput_file->GetEntries()) {
    std::cout << "  ERROR: Trees have different sizes." << std::endl
	      << "    Size of tree " << treename_MCdatafile << " in "
	      << MCdatafilename << ": " << tree_MCdatafile->GetEntries()
	      << std::endl
	      << "    Size of tree " << treename_MVAoutput_file << " in "
	      << MVAoutput_filename << ": " << tree_MVAoutput_file->GetEntries()
	      << std::endl << std::endl;
    gApplication->Terminate();
  }

  TFile *outfile = new TFile("temp_output/ROCdata_MVAplusPrefilter.root", "RECREATE");
  TTree *outtree = new TTree("ROCdata","ROCdata");
  Float_t tpr, fpr;
  Float_t currentMVAcut;
  Int_t nTaggedAccepted;
  Int_t nIsConv, nIsRP;
  outtree->Branch("tpr", &tpr, "tpr/F");
  outtree->Branch("fpr", &fpr, "fpr/F");
  outtree->Branch("MVAcut", &currentMVAcut, "MVAcut/F");
  outtree->Branch("nTaggedAccepted", &nTaggedAccepted, "nTaggedAccepted/I");
  outtree->Branch("nIsConv", &nIsConv, "nIsConv/I");
  outtree->Branch("nIsRP", &nIsRP, "nIsRP/I");
  

  
  const Long64_t nentries = tree_MCdatafile->GetEntries()/10;


  TStopwatch *watch_overall = new TStopwatch();
  watch_overall->Start();

  
  for(Int_t scan=0; scan<num_steps; scan++) {

    TStopwatch *watch_step = new TStopwatch();
    watch_step->Start();
    
    // Collection of all particle pairs (of all events):
    std::vector<particlePair> allPairs;

    // Collection of all accepted particle tracks (of all events):
    std::vector<particleTrack> tracksTaggedAccepted;

    
    Float_t MVAcut = scan/(num_steps*1.0)*(MVAoutputRange_max-MVAoutputRange_min) + MVAoutputRange_min;


    std::cout << std::endl << "---------- Step " << scan+1 << " of " << num_steps
	      << " (MVA cut: " << MVAcut << "): ----------" << std::endl;

    
    std::cout << "Tagging pairs based on MVA output..." << std::endl;
  
    for(Long64_t j=0; j<nentries; j++) {
      if((j%10000)==0) {
	std::cout << "\r  Processing event " << j << " of " << nentries
		  << " (" << j/((Float_t)nentries)*100 << " %)...";
      }


      tree_MCdatafile->GetEntry(j);
      tree_MVAoutput_file->GetEntry(j);
    
      particlePair currentPair;
      currentPair.EventID = EventID;
      currentPair.TrackID1 = TrackID1;
      currentPair.TrackID2 = TrackID2;
      currentPair.IsTrueConv = IsConv;
      currentPair.IsTrueRP = IsRP;
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
  

    for(Long64_t i=0; i<nentries; i++) {
      if((i%10000)==0) {
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
	      << " (100 %)... DONE." << std::endl;


    // restore original order of allPairs vector:
    std::cout << "Restoring original order of data...";
    std::sort(allPairs.begin(), allPairs.end(), sortByOriginalPosition);
    std::cout << " DONE." << std::endl;


    std::cout << "Calculating true and false positive rates...";

    nTaggedAccepted = 0;
    nIsConv = 0;
    nIsRP = 0;
    Long64_t tp=0, fp=0, fn=0, tn=0;
    
    for(Long64_t i=0; i<nentries; i++) {
      if(allPairs[i].IsTaggedAccepted!=0 && allPairs[i].IsTaggedAccepted!=1)
	continue;

      if(allPairs[i].IsTaggedAccepted) nTaggedAccepted++;
      if(allPairs[i].IsTrueConv) nIsConv++;
      if(allPairs[i].IsTrueRP) nIsRP++;
      
      if(allPairs[i].IsTaggedAccepted && allPairs[i].IsTrueConv) tn++;
      if(allPairs[i].IsTaggedAccepted && !allPairs[i].IsTrueConv) fn++;
      if(!allPairs[i].IsTaggedAccepted && allPairs[i].IsTrueConv) fp++;
      if(!allPairs[i].IsTaggedAccepted && !allPairs[i].IsTrueConv) tp++;
    }
    
    tpr = tp/((Float_t)tp+fn);
    fpr = fp/((Float_t)fp+tn);
    currentMVAcut = MVAcut;
    
    std::cout << " DONE" << std::endl;
    std::cout << "  (TPR = " << tpr << ",\tFPR = " << fpr
	      << ")" << std::endl;
    outtree->Fill();


    std::cout << std::endl << "Time elapsed: " << watch_step->RealTime()
	      << " seconds." << std::endl;
  }

  std::cout << std::endl << "Overall time elapsed: " << watch_overall->RealTime() << " seconds."
	    << std::endl << std::endl;
  
  std::cout << "Write data...";
  outfile->cd();
  outfile->Write("", TObject::kOverwrite);
  std::cout << " DONE." << std::endl;
  std::cout << "File " << outfile->GetName() << " recreated." << std::endl;

  
  gApplication->Terminate();
  
}
