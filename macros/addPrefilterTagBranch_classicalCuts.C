#include <iostream>

#include <TROOT.h>
#include <TMath.h> //debug
#include <TApplication.h>
#include <TFile.h>
#include <TTree.h>
#include <TStopwatch.h>
#include <TH1F.h>
#include <TCanvas.h>


// stores all relevant information of a particle pair:
struct particlePair {
  Int_t EventID;
  Int_t TrackID1;
  Int_t TrackID2;
  Int_t IsRP, IsConv; //debug
  Float_t phiv, mass; //debug
  Int_t IsTaggedAccepted;
  Int_t num_associatedPairs;
  Long64_t originalPosition; //debug

  bool operator < (const particlePair &rhs) const {
    return (EventID < rhs.EventID);
  }
};

//debug:
bool sortByOriginalPosition(const particlePair &lhs, const particlePair &rhs) {
  return lhs.originalPosition < rhs.originalPosition;
}

// store all relevant information of a particle track:
struct particleTrack {
  Int_t EventID;
  Int_t TrackID;
  Int_t IsConvLeg; //debug

  bool operator < (const particleTrack &rhs) const {
    return (EventID < rhs.EventID);
  }
};



void addPrefilterTagBranch_classicalCuts(TString updatefilename,
					 TString treename_updatefile,
					 TString branchname_read,
					 TString branchname_add,
					 TString signalRegion = "+") {
  
  Int_t var;
  Int_t isAccepted;
  
  
  TFile *updatefile = new TFile(updatefilename, "UPDATE");
  TTree *tree_updatefile = (TTree*)updatefile->Get(treename_updatefile);
  Int_t EventID, TrackID1, TrackID2;
  Int_t IsRP, IsConv, motherPdg1, motherPdg2; //debug
  Float_t phiv, mass; //debug
  tree_updatefile->SetBranchAddress("EventID1", &EventID);
  tree_updatefile->SetBranchAddress("TrackID1", &TrackID1);
  tree_updatefile->SetBranchAddress("TrackID2", &TrackID2);
  tree_updatefile->SetBranchAddress("IsRP", &IsRP); //debug
  tree_updatefile->SetBranchAddress("IsConv", &IsConv); //debug
  tree_updatefile->SetBranchAddress("motherPdg1", &motherPdg1); //debug
  tree_updatefile->SetBranchAddress("motherPdg2", &motherPdg2); //debug
  tree_updatefile->SetBranchAddress("phiv", &phiv); //debug
  tree_updatefile->SetBranchAddress("mass", &mass); //debug
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
    currentPair.IsRP = IsRP; //debug
    currentPair.IsConv = IsConv; //debug
    currentPair.phiv = phiv; //debug
    currentPair.mass = mass; //debug
    currentPair.originalPosition = j; //debug
    currentPair.num_associatedPairs = 0;
    
    if(signalRegion == "+") {
      if(var == 1) {
	currentPair.IsTaggedAccepted = 1.;

	particleTrack currentTrack1 = {EventID, TrackID1, (TMath::Abs(motherPdg1)==22) ? 1 : 0 /* debug */};
	particleTrack currentTrack2 = {EventID, TrackID2, (TMath::Abs(motherPdg2)==22) ? 1 : 0 /* debug */};
	tracksTaggedAccepted.push_back(currentTrack1);
	tracksTaggedAccepted.push_back(currentTrack2);
      }
      else currentPair.IsTaggedAccepted = 0.;
    }else if(signalRegion == "-") {
      if(var == 0) {
	currentPair.IsTaggedAccepted = 1.;

	particleTrack currentTrack1 = {EventID, TrackID1, (TMath::Abs(motherPdg1)==22) ? 1 : 0 /* debug */};
	particleTrack currentTrack2 = {EventID, TrackID2, (TMath::Abs(motherPdg1)==22) ? 1 : 0 /* debug */};
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

  // //debug:
  // Int_t cnt_allPairs = 0, cnt_allPairs_prefiltered = 0;
  // std::cout << std::endl;
  // std::cout << "########## DEBUG OUTPUT: allPairs ##########" << std::endl << std::endl;
  // for(Long64_t i=0; i<nentries; i++) {
  //   std::cout << "EventID = " << allPairs[i].EventID
  // 	      << ", TrackID1 = " << allPairs[i].TrackID1
  // 	      << ", TrackID2 = " << allPairs[i].TrackID2
  // 	      << ", IsRP = " << allPairs[i].IsRP
  // 	      << ", IsConv = " << allPairs[i].IsConv
  // 	      << ", IsTaggedAcepted = " << allPairs[i].IsTaggedAccepted
  // 	      << ", phiv = " << allPairs[i].phiv
  // 	      << ", mass = " << allPairs[i].mass
  // 	      << std::endl;
  //   if(allPairs[i].IsTaggedAccepted==1) cnt_allPairs++;
  // }
  // std::cout << "#IsTaggedAccepted: " << cnt_allPairs << std::endl;

  // //debug:
  // std::cout << std::endl;
  // std::cout << "########## DEBUG OUTPUT: tracksTaggedAccepted ##########" << std::endl << std::endl;
  // for(Long64_t i=0; i<(Long64_t)tracksTaggedAccepted.size(); i++) {
  //   std::cout << "EventID = " << tracksTaggedAccepted[i].EventID
  // 	      << ", TrackID = " << tracksTaggedAccepted[i].TrackID
  // 	      << ", IsConvLeg = " << tracksTaggedAccepted[i].IsConvLeg
  // 	      << std::endl;
  // }
  // std::cout << std::endl;
  
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
    if((i%2500)==0) {
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
	allPairs[i].num_associatedPairs = allPairs[i].num_associatedPairs + 1;
      }
    }
  }
  std::cout << "\r  Processing event " << nentries << " of " << nentries
	    << " (100 %)... DONE.";
  std::cout << " Time elapsed: " << watch->RealTime() << " seconds."
	    << std::endl << std::endl;


  // //debug:
  // std::cout << std::endl << "########## DEBUG OUTPUT: allPairs (prefiltered) ##########" << std::endl << std::endl;
  // for(Long64_t i=0; i<nentries; i++) {
  //   std::cout << "EventID = " << allPairs[i].EventID
  // 	      << ", TrackID1 = " << allPairs[i].TrackID1
  // 	      << ", TrackID2 = " << allPairs[i].TrackID2
  // 	      << ", IsRP = " << allPairs[i].IsRP
  // 	      << ", IsConv = " << allPairs[i].IsConv
  // 	      << ", IsTaggedAcepted = " << allPairs[i].IsTaggedAccepted
  // 	      << ", phiv = " << allPairs[i].phiv
  // 	      << ", mass = " << allPairs[i].mass
  // 	      << std::endl;
  //   if(allPairs[i].IsTaggedAccepted==1) cnt_allPairs_prefiltered++;
  // }
  // std::cout << "#IsTaggedAccepted: " << cnt_allPairs_prefiltered << std::endl;

  // //debug:
  // std::cout << std::endl;
  // std::cout << "########## DEBUG OUTPUT: tracksTaggedAccepted (prefiltered) ##########" << std::endl << std::endl;
  // for(Long64_t i=0; i<(Long64_t)tracksTaggedAccepted.size(); i++) {
  //   std::cout << "EventID = " << tracksTaggedAccepted[i].EventID
  // 	      << ", TrackID = " << tracksTaggedAccepted[i].TrackID
  // 	      << ", IsConvLeg = " << tracksTaggedAccepted[i].IsConvLeg
  // 	      << std::endl;
  // }
  // std::cout << std::endl;
  

  // create histogram with the distribution of the associated track number:
  TH1F *hist_associatedPairs_temp = new TH1F("","",nentries,0,nentries);
  for(Long64_t i=0; i<nentries; i++) {
    if(allPairs[i].IsTaggedAccepted == 1) {
      hist_associatedPairs_temp->SetBinContent(i, allPairs[i].num_associatedPairs);
    }
  }
  TH1F *hist_associatedPairs = new TH1F("","",100,0,hist_associatedPairs_temp->GetMaximum());
  for(Long64_t i=0; i<nentries; i++) {
    hist_associatedPairs->Fill(hist_associatedPairs_temp->GetBinContent(i));
  }  
  hist_associatedPairs->SetXTitle("number of associated pairs");
  hist_associatedPairs->SetYTitle("counts");
  hist_associatedPairs->GetYaxis()->SetTitleOffset(1.2);
  TCanvas *c = new TCanvas();
  c->SetLogy();
  hist_associatedPairs->Draw();
  c->SaveAs("temp_hist_numberOfAssociatedPairs.root");



  //debug:
  std::sort(allPairs.begin(), allPairs.end(), sortByOriginalPosition);
  
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
