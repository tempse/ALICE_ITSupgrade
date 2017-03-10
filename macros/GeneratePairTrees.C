//gSystem.Load("libPhysics.so");

// Abbreviations:
// rp <-> real pair
// us <-> unlike sign
// ls <-> like sign

#include <iostream>
#include <cstdlib>

#include <TROOT.h>
#include <TApplication.h>
#include <TFile.h>
#include <TTree.h>
#include <TParticle.h>
#include <TMath.h>
#include <TRandom.h>
#include <TStopwatch.h>

Bool_t isCharm(Int_t );
Bool_t isBottom(Int_t );

// The following functions use the globally defined output tree variables. Make
// sure that these variables have the correct values when calling them!
void calculateMomenta();
void calculatePhiv();
void calculateOpang();
void calculateMass();
void calculateDiffz();
void calculateSumz();
void calculateHF();


// MVA cut value (for identifying conversion tracks):
const Float_t MVAcut_convTrack = -0.4218;


const Bool_t doRandPairSwap = kFALSE; // do random pair swapping?

Bool_t doSwapCurrentPair = kFALSE;


// some counting variables:
Int_t cnt_ls = 0, cnt_us = 0, cnt_us_ls = 0, cnt_rp = 0;


// output variables (1,2 <-> 1st leg, 2nd leg):
Int_t TrackID1, TrackID2;
Int_t EventID1, EventID2;
Int_t IsRP;                  // real pairs: 1, combinatorial pairs: 0
Int_t IsUS;                  // pair with unlike sign (regardless of IsRP)
Int_t IsConv;                // both mother particles are gammas (can still be comb. pairs)
Int_t IsHF;                  // HF = "heavy flavor"
Int_t IsCorrCharm;           // correlated charmed pair: 1, else: 0
Int_t IsCorrBottom;          // correlated bottom pair: 1, else: 0
Int_t IsCorrCharmFromBottom; // correlated charmed pair originating from a bottom: 1, else: 0
Int_t ChargeSign;            // unlike sign: 0, like sign (++): 1, like sign (--): -1
Int_t IsTaggedRPConv;        // tagged as real pair conversion event: 1, otherwise: 0
Int_t IsTaggedConvTrack1,    // tagged as conversion track (by preceding...
  IsTaggedConvTrack2;        // ...MVA classification): 1, otherwise: 0
Float_t opang;
Float_t diffz;
Float_t mass;
Float_t sumz;
Float_t phiv;
Float_t px1, px2;
Float_t py1, py2;
Float_t pz1, pz2;
Float_t mcPx1, mcPx2;       // }
Float_t mcPy1, mcPy2;       // } momenta calculated from MC data
Float_t mcPz1, mcPz2;       // }
Int_t pdg1, pdg2;
Int_t motherPdg1, motherPdg2;
Int_t firstMotherPdg1, firstMotherPdg2;
Int_t motherLabel1, motherLabel2;
Int_t firstMotherLabel1, firstMotherLabel2;
Int_t firstMotherLabel1_min, firstMotherLabel2_min;
Int_t firstMotherLabel1_max, firstMotherLabel2_max;
Float_t DCAxy1, DCAxy2;
Float_t DCAz1, DCAz2;
Int_t nITS1, nITS2;
Float_t nITSshared1, nITSshared2;
Int_t nTPC1, nTPC2;
// Float_t chi2g1, chi2g2;
Float_t ITSchi21, ITSchi22;
Float_t TPCchi21, TPCchi22;
Float_t phi1, phi2;
Float_t eta1, eta2;
Float_t pt1, pt2;

TVector3 pv1, pv2;
TVector3 z(0,0,1);
TVector3 u,v,w,ua;          // for phiv calculation
TVector3 temp;

// program control parameters:
bool isPairTree_rp = false;      // }
bool isPairTree_us = true;       // } set/unset the
bool isPairTree_ls = false;      // } output trees here
bool isPairTree_us_ls = false;   // }



void GeneratePairTrees() {
  TFile *infile = TFile::Open("../inputData/FT2_AnalysisResults_Upgrade_addFeat_test_1-10-split.root","READ");
  TTree *singleTree = (TTree*)infile->Get("tracks");

  TFile *infile_MVAoutputs = TFile::Open("../TMVA/TMVAClassification_singleTrackTree/TMVApp.root");
  TTree *singleTree_MVAoutputs = (TTree*)infile_MVAoutputs->Get("tracks_MVAoutput");
  
  TFile *outfile = TFile::Open("temp_output/output_pairtrees.root","RECREATE");
  TTree *pairTree_rp = new TTree("pairTree_rp","pairTree_rp");
  TTree *pairTree_us = new TTree("pairTree_us","pairTree_us");
  TTree *pairTree_ls = new TTree("pairTree_ls","pairTree_ls");
  TTree *pairTree_us_ls = new TTree("pairTree_us_ls","pairTree_us_ls");


  if(singleTree->GetEntries() != singleTree_MVAoutputs->GetEntries()) {
    std::cout << "  ERROR: The trees in the input files have different sizes."
	      << std::endl;
    std::cout << "    Size of tree in file " << infile->GetName() << ": "
	      << singleTree->GetEntries() << std::endl;
    std::cout << "    Size of tree in file " << infile_MVAoutputs->GetName() << ": "
	      << singleTree_MVAoutputs->GetEntries() << std::endl;
    gApplication->Terminate();
  }


  TRandom rand;



  
  // variables of input tree ("ST" <-> "single tree"):
  Int_t ST_event;
  Double_t ST_IP;
  Int_t ST_global;
  Double_t ST_eta;
  Double_t ST_phi;
  Double_t ST_pt;
  Double_t ST_dcaR;
  Double_t ST_dcaZ;
  Int_t ST_label;
  Int_t ST_labelFirstMother;
  Int_t ST_labelMinFirstMother;
  Int_t ST_labelMaxFirstMother;
  Double_t ST_mcEta;
  Double_t ST_mcPhi;
  Double_t ST_mcPt;
  Int_t ST_pdg;
  Int_t ST_pdgMother;
  Int_t ST_pdgFirstMother;
  Int_t ST_nITS;
  Double_t ST_nITSshared;
  Int_t ST_nTPC;
  Double_t ST_ITSchi2;
  Double_t ST_TPCchi2;

  
  TParticle* ST_particle = NULL;
  
  
  singleTree->SetBranchAddress("event",&ST_event);
  singleTree->SetBranchAddress("IP",&ST_IP);
  singleTree->SetBranchAddress("global",&ST_global);
  singleTree->SetBranchAddress("eta",&ST_eta);
  singleTree->SetBranchAddress("phi",&ST_phi);
  singleTree->SetBranchAddress("pt",&ST_pt);
  singleTree->SetBranchAddress("dcaR",&ST_dcaR);
  singleTree->SetBranchAddress("dcaZ",&ST_dcaZ);
  singleTree->SetBranchAddress("label",&ST_label);
  singleTree->SetBranchAddress("particle.",&ST_particle);
  singleTree->SetBranchAddress("labelFirstMother",&ST_labelFirstMother);
  singleTree->SetBranchAddress("labelMinFirstMother",&ST_labelMinFirstMother);
  singleTree->SetBranchAddress("labelMaxFirstMother",&ST_labelMaxFirstMother);
  singleTree->SetBranchAddress("mcEta",&ST_mcEta);
  singleTree->SetBranchAddress("mcPhi",&ST_mcPhi);
  singleTree->SetBranchAddress("mcPt",&ST_mcPt);
  singleTree->SetBranchAddress("pdg",&ST_pdg);
  singleTree->SetBranchAddress("pdgMother",&ST_pdgMother);
  singleTree->SetBranchAddress("pdgFirstMother",&ST_pdgFirstMother);
  singleTree->SetBranchAddress("nITS",&ST_nITS);
  singleTree->SetBranchAddress("nITSshared",&ST_nITSshared);
  singleTree->SetBranchAddress("nTPC",&ST_nTPC);
  singleTree->SetBranchAddress("ITSchi2",&ST_ITSchi2);
  singleTree->SetBranchAddress("TPCchi2",&ST_TPCchi2);


  if(isPairTree_rp) {
    pairTree_rp->Branch("TrackID1",&TrackID1);
    pairTree_rp->Branch("TrackID2",&TrackID2);
    pairTree_rp->Branch("EventID1",&EventID1);
    pairTree_rp->Branch("EventID2",&EventID2);
    pairTree_rp->Branch("IsRP",&IsRP);
    pairTree_rp->Branch("IsUS",&IsUS);
    pairTree_rp->Branch("IsConv",&IsConv);
    pairTree_rp->Branch("IsHF",&IsHF);
    pairTree_rp->Branch("IsCorrCharm",&IsCorrCharm);
    pairTree_rp->Branch("IsCorrBottom",&IsCorrBottom);
    pairTree_rp->Branch("IsCorrCharmFromBottom",&IsCorrCharmFromBottom);
    pairTree_rp->Branch("IsTaggedRPConv",&IsTaggedRPConv);
    pairTree_rp->Branch("IsTaggedConvTrack1",&IsTaggedConvTrack1);
    pairTree_rp->Branch("IsTaggedConvTrack2",&IsTaggedConvTrack2);
    pairTree_rp->Branch("ChargeSign",&ChargeSign);
    pairTree_rp->Branch("opang",&opang);
    pairTree_rp->Branch("diffz",&diffz);
    pairTree_rp->Branch("mass",&mass);
    pairTree_rp->Branch("sumz",&sumz);
    pairTree_rp->Branch("phiv",&phiv);
    pairTree_rp->Branch("px1",&px1);
    pairTree_rp->Branch("px2",&px2);
    pairTree_rp->Branch("py1",&py1);
    pairTree_rp->Branch("py2",&py2);
    pairTree_rp->Branch("pz1",&pz1);
    pairTree_rp->Branch("pz2",&pz2);
    pairTree_rp->Branch("mcPx1",&mcPx1);
    pairTree_rp->Branch("mcPx2",&mcPx2);
    pairTree_rp->Branch("mcPy1",&mcPy1);
    pairTree_rp->Branch("mcPy2",&mcPy2);
    pairTree_rp->Branch("mcPz1",&mcPz1);
    pairTree_rp->Branch("mcPz2",&mcPz2);
    pairTree_rp->Branch("motherPdg1",&motherPdg1);
    pairTree_rp->Branch("motherPdg2",&motherPdg2);
    pairTree_rp->Branch("pdg1",&pdg1);
    pairTree_rp->Branch("pdg2",&pdg2);
    pairTree_rp->Branch("DCAxy1",&DCAxy1);
    pairTree_rp->Branch("DCAxy2",&DCAxy2);
    pairTree_rp->Branch("DCAz1",&DCAz1);
    pairTree_rp->Branch("DCAz2",&DCAz2);
    pairTree_rp->Branch("nITS1",&nITS1);
    pairTree_rp->Branch("nITS2",&nITS2);
    pairTree_rp->Branch("nITSshared1",&nITSshared1);
    pairTree_rp->Branch("nITSshared2",&nITSshared2);
    pairTree_rp->Branch("nTPC1",&nTPC1);
    pairTree_rp->Branch("nTPC2",&nTPC2);
    pairTree_rp->Branch("ITSchi21",&ITSchi21);
    pairTree_rp->Branch("ITSchi22",&ITSchi22);
    pairTree_rp->Branch("TPCchi21",&TPCchi21);
    pairTree_rp->Branch("TPCchi22",&TPCchi22);
    pairTree_rp->Branch("phi1",&phi1);
    pairTree_rp->Branch("phi2",&phi2);
    pairTree_rp->Branch("eta1",&eta1);
    pairTree_rp->Branch("eta2",&eta2);
    pairTree_rp->Branch("pt1",&pt1);
    pairTree_rp->Branch("pt2",&pt2);
  }

  if(isPairTree_us) {
    pairTree_us->Branch("TrackID1",&TrackID1);
    pairTree_us->Branch("TrackID2",&TrackID2);
    pairTree_us->Branch("EventID1",&EventID1);
    pairTree_us->Branch("EventID2",&EventID2);
    pairTree_us->Branch("IsRP",&IsRP);
    pairTree_us->Branch("IsUS",&IsUS);
    pairTree_us->Branch("IsConv",&IsConv);
    pairTree_us->Branch("IsHF",&IsHF);
    pairTree_us->Branch("IsCorrCharm",&IsCorrCharm);
    pairTree_us->Branch("IsCorrBottom",&IsCorrBottom);
    pairTree_us->Branch("IsCorrCharmFromBottom",&IsCorrCharmFromBottom);
    pairTree_us->Branch("IsTaggedRPConv",&IsTaggedRPConv);
    pairTree_us->Branch("IsTaggedConvTrack1",&IsTaggedConvTrack1);
    pairTree_us->Branch("IsTaggedConvTrack2",&IsTaggedConvTrack2);
    pairTree_us->Branch("ChargeSign",&ChargeSign);
    pairTree_us->Branch("opang",&opang);
    pairTree_us->Branch("diffz",&diffz);
    pairTree_us->Branch("mass",&mass);
    pairTree_us->Branch("sumz",&sumz);
    pairTree_us->Branch("phiv",&phiv);
    pairTree_us->Branch("px1",&px1);
    pairTree_us->Branch("px2",&px2);
    pairTree_us->Branch("py1",&py1);
    pairTree_us->Branch("py2",&py2);
    pairTree_us->Branch("pz1",&pz1);
    pairTree_us->Branch("pz2",&pz2);
    pairTree_us->Branch("mcPx1",&mcPx1);
    pairTree_us->Branch("mcPx2",&mcPx2);
    pairTree_us->Branch("mcPy1",&mcPy1);
    pairTree_us->Branch("mcPy2",&mcPy2);
    pairTree_us->Branch("mcPz1",&mcPz1);
    pairTree_us->Branch("mcPz2",&mcPz2);
    pairTree_us->Branch("motherPdg1",&motherPdg1);
    pairTree_us->Branch("motherPdg2",&motherPdg2);
    pairTree_us->Branch("pdg1",&pdg1);
    pairTree_us->Branch("pdg2",&pdg2);
    pairTree_us->Branch("DCAxy1",&DCAxy1);
    pairTree_us->Branch("DCAxy2",&DCAxy2);
    pairTree_us->Branch("DCAz1",&DCAz1);
    pairTree_us->Branch("DCAz2",&DCAz2);
    pairTree_us->Branch("nITS1",&nITS1);
    pairTree_us->Branch("nITS2",&nITS2);
    pairTree_us->Branch("nITSshared1",&nITSshared1);
    pairTree_us->Branch("nITSshared2",&nITSshared2);
    pairTree_us->Branch("nTPC1",&nTPC1);
    pairTree_us->Branch("nTPC2",&nTPC2);
    pairTree_us->Branch("ITSchi21",&ITSchi21);
    pairTree_us->Branch("ITSchi22",&ITSchi22);
    pairTree_us->Branch("TPCchi21",&TPCchi21);
    pairTree_us->Branch("TPCchi22",&TPCchi22);
    pairTree_us->Branch("phi1",&phi1);
    pairTree_us->Branch("phi2",&phi2);
    pairTree_us->Branch("eta1",&eta1);
    pairTree_us->Branch("eta2",&eta2);
    pairTree_us->Branch("pt1",&pt1);
    pairTree_us->Branch("pt2",&pt2);
  }

  if(isPairTree_ls) {
    pairTree_ls->Branch("TrackID1",&TrackID1);
    pairTree_ls->Branch("TrackID2",&TrackID2);
    pairTree_ls->Branch("EventID1",&EventID1);
    pairTree_ls->Branch("EventID2",&EventID2);
    pairTree_ls->Branch("IsRP",&IsRP);
    pairTree_ls->Branch("IsUS",&IsUS);
    pairTree_ls->Branch("IsConv",&IsConv);
    pairTree_ls->Branch("IsHF",&IsHF);
    pairTree_ls->Branch("IsCorrCharm",&IsCorrCharm);
    pairTree_ls->Branch("IsCorrBottom",&IsCorrBottom);
    pairTree_ls->Branch("IsCorrCharmFromBottom",&IsCorrCharmFromBottom);
    pairTree_ls->Branch("IsTaggedRPConv",&IsTaggedRPConv);
    pairTree_ls->Branch("IsTaggedConvTrack1",&IsTaggedConvTrack1);
    pairTree_ls->Branch("IsTaggedConvTrack2",&IsTaggedConvTrack2);
    pairTree_ls->Branch("ChargeSign",&ChargeSign);
    pairTree_ls->Branch("opang",&opang);
    pairTree_ls->Branch("diffz",&diffz);
    pairTree_ls->Branch("mass",&mass);
    pairTree_ls->Branch("sumz",&sumz);
    pairTree_ls->Branch("phiv",&phiv);
    pairTree_ls->Branch("px1",&px1);
    pairTree_ls->Branch("px2",&px2);
    pairTree_ls->Branch("py1",&py1);
    pairTree_ls->Branch("py2",&py2);
    pairTree_ls->Branch("pz1",&pz1);
    pairTree_ls->Branch("pz2",&pz2);
    pairTree_ls->Branch("mcPx1",&mcPx1);
    pairTree_ls->Branch("mcPx2",&mcPx2);
    pairTree_ls->Branch("mcPy1",&mcPy1);
    pairTree_ls->Branch("mcPy2",&mcPy2);
    pairTree_ls->Branch("mcPz1",&mcPz1);
    pairTree_ls->Branch("mcPz2",&mcPz2);
    pairTree_ls->Branch("motherPdg1",&motherPdg1);
    pairTree_ls->Branch("motherPdg2",&motherPdg2);
    pairTree_ls->Branch("pdg1",&pdg1);
    pairTree_ls->Branch("pdg2",&pdg2);
    pairTree_ls->Branch("DCAxy1",&DCAxy1);
    pairTree_ls->Branch("DCAxy2",&DCAxy2);
    pairTree_ls->Branch("DCAz1",&DCAz1);
    pairTree_ls->Branch("DCAz2",&DCAz2);
    pairTree_ls->Branch("nITS1",&nITS1);
    pairTree_ls->Branch("nITS2",&nITS2);
    pairTree_ls->Branch("nITSshared1",&nITSshared1);
    pairTree_ls->Branch("nITSshared2",&nITSshared2);
    pairTree_ls->Branch("nTPC1",&nTPC1);
    pairTree_ls->Branch("nTPC2",&nTPC2);
    pairTree_ls->Branch("ITSchi21",&ITSchi21);
    pairTree_ls->Branch("ITSchi22",&ITSchi22);
    pairTree_ls->Branch("TPCchi21",&TPCchi21);
    pairTree_ls->Branch("TPCchi22",&TPCchi22);
    pairTree_ls->Branch("phi1",&phi1);
    pairTree_ls->Branch("phi2",&phi2);
    pairTree_ls->Branch("eta1",&eta1);
    pairTree_ls->Branch("eta2",&eta2);
    pairTree_ls->Branch("pt1",&pt1);
    pairTree_ls->Branch("pt2",&pt2);
  }

  if(isPairTree_us_ls) {
    pairTree_us_ls->Branch("TrackID1",&TrackID1);
    pairTree_us_ls->Branch("TrackID2",&TrackID2);
    pairTree_us_ls->Branch("EventID1",&EventID1);
    pairTree_us_ls->Branch("EventID2",&EventID2);
    pairTree_us_ls->Branch("IsRP",&IsRP);
    pairTree_us_ls->Branch("IsUS",&IsUS);
    pairTree_us_ls->Branch("IsConv",&IsConv);
    pairTree_us_ls->Branch("IsHF",&IsHF);
    pairTree_us_ls->Branch("IsCorrCharm",&IsCorrCharm);
    pairTree_us_ls->Branch("IsCorrBottom",&IsCorrBottom);
    pairTree_us_ls->Branch("IsCorrCharmFromBottom",&IsCorrCharmFromBottom);
    pairTree_us_ls->Branch("IsTaggedRPConv",&IsTaggedRPConv);
    pairTree_us_ls->Branch("IsTaggedConvTrack1",&IsTaggedConvTrack1);
    pairTree_us_ls->Branch("IsTaggedConvTrack2",&IsTaggedConvTrack2);
    pairTree_us_ls->Branch("ChargeSign",&ChargeSign);
    pairTree_us_ls->Branch("opang",&opang);
    pairTree_us_ls->Branch("diffz",&diffz);
    pairTree_us_ls->Branch("mass",&mass);
    pairTree_us_ls->Branch("sumz",&sumz);
    pairTree_us_ls->Branch("phiv",&phiv);
    pairTree_us_ls->Branch("px1",&px1);
    pairTree_us_ls->Branch("px2",&px2);
    pairTree_us_ls->Branch("py1",&py1);
    pairTree_us_ls->Branch("py2",&py2);
    pairTree_us_ls->Branch("pz1",&pz1);
    pairTree_us_ls->Branch("pz2",&pz2);
    pairTree_us_ls->Branch("mcPx1",&mcPx1);
    pairTree_us_ls->Branch("mcPx2",&mcPx2);
    pairTree_us_ls->Branch("mcPy1",&mcPy1);
    pairTree_us_ls->Branch("mcPy2",&mcPy2);
    pairTree_us_ls->Branch("mcPz1",&mcPz1);
    pairTree_us_ls->Branch("mcPz2",&mcPz2);
    pairTree_us_ls->Branch("motherPdg1",&motherPdg1);
    pairTree_us_ls->Branch("motherPdg2",&motherPdg2);
    pairTree_us_ls->Branch("pdg1",&pdg1);
    pairTree_us_ls->Branch("pdg2",&pdg2);
    pairTree_us_ls->Branch("DCAxy1",&DCAxy1);
    pairTree_us_ls->Branch("DCAxy2",&DCAxy2);
    pairTree_us_ls->Branch("DCAz1",&DCAz1);
    pairTree_us_ls->Branch("DCAz2",&DCAz2);
    pairTree_us_ls->Branch("nITS1",&nITS1);
    pairTree_us_ls->Branch("nITS2",&nITS2);
    pairTree_us_ls->Branch("nITSshared1",&nITSshared1);
    pairTree_us_ls->Branch("nITSshared2",&nITSshared2);
    pairTree_us_ls->Branch("nTPC1",&nTPC1);
    pairTree_us_ls->Branch("nTPC2",&nTPC2);
    pairTree_us_ls->Branch("ITSchi21",&ITSchi21);
    pairTree_us_ls->Branch("ITSchi22",&ITSchi22);
    pairTree_us_ls->Branch("TPCchi21",&TPCchi21);
    pairTree_us_ls->Branch("TPCchi22",&TPCchi22);
    pairTree_us_ls->Branch("phi1",&phi1);
    pairTree_us_ls->Branch("phi2",&phi2);
    pairTree_us_ls->Branch("eta1",&eta1);
    pairTree_us_ls->Branch("eta2",&eta2);
    pairTree_us_ls->Branch("pt1",&pt1);
    pairTree_us_ls->Branch("pt2",&pt2);
  }


  Float_t MVAoutput_convTrack;
  singleTree_MVAoutputs->SetBranchAddress("BDT", &MVAoutput_convTrack);
  

  // Int_t ev_temp = -1; // used to detect new events
  Int_t firstTrack; // first track number in given event
  Int_t nTracks; // total number of tracks in given event

  
  Long64_t singleTree_nEvents = singleTree->GetEntries();

  
  std::cout << std::endl << "Start event processing...";
  
  TStopwatch *watch = new TStopwatch();
  watch->Start();



  for(Long64_t tr1=0; tr1<singleTree_nEvents; tr1++) { // first track loop
    if((tr1%1000)==0) std::cout << "\rProcessing event " << tr1 << " of " << singleTree_nEvents
				<< " (" << tr1*100/singleTree_nEvents << "%)...";
    singleTree->GetEntry(tr1);
    singleTree_MVAoutputs->GetEntry(tr1);

    
    // do not look for partner tracks of the last event in the tree:
    if(tr1 == singleTree_nEvents-1) break;

    
    // detect new events:
    // if(ev_temp != ST_event) {
    //   ev_temp = ST_event;
    //   firstTrack = tr1;
    // }
    Int_t ev_temp = ST_event; // marks the current event number

    // pdg cut:
    if(abs(ST_pdg) != 11) continue; // keep electrons/positrons only

    // cut to ignore unphysical DCAz values (specific to prior analysis):
    if(ST_dcaZ == 999) continue;


    // MVA cut tagging (conversion track identification):
    if(MVAoutput_convTrack < MVAcut_convTrack) {
      if(!doSwapCurrentPair) IsTaggedConvTrack1 = 1;
      else IsTaggedConvTrack2 = 1;
    }else {
      if(!doSwapCurrentPair) IsTaggedConvTrack1 = 0;
      else IsTaggedConvTrack2 = 0;
    }
    
    // prepare random pair swapping if doRandPairSwap==kTRUE:
    if(doRandPairSwap) {
      doSwapCurrentPair = (rand.Uniform() < .5) ? kTRUE : kFALSE;
    }else {
      doSwapCurrentPair = kFALSE;
    }

    
    // already available, pairing-independent variables of 1st leg for output tree:
    if(!doSwapCurrentPair) {
      EventID1 = ST_event;
      TrackID1 = tr1;
      mcPx1 = ST_particle->Px();
      mcPy1 = ST_particle->Py();
      mcPz1 = ST_particle->Pz();
      pdg1 = ST_pdg;
      motherPdg1 = ST_pdgMother;
      firstMotherPdg1 = ST_pdgFirstMother;
      motherLabel1 = ST_particle->GetMother(0); // Note: That's not the first mother of the track.
      firstMotherLabel1 = ST_labelFirstMother;
      firstMotherLabel1_min = ST_labelMinFirstMother;
      firstMotherLabel1_max = ST_labelMaxFirstMother;
      DCAxy1 = ST_dcaR;
      DCAz1 = ST_dcaZ;
      nITS1 = ST_nITS;
      nITSshared1 = ST_nITSshared;
      nTPC1 = ST_nTPC;
      ITSchi21 = ST_ITSchi2;
      TPCchi21 = ST_TPCchi2;
      phi1 = ST_phi;
      eta1 = ST_eta;
      pt1 = ST_pt;
    }else {
      EventID2 = ST_event;
      TrackID2 = tr1;
      mcPx2 = ST_particle->Px();
      mcPy2 = ST_particle->Py();
      mcPz2 = ST_particle->Pz();
      pdg2 = ST_pdg;
      motherPdg2 = ST_pdgMother;
      firstMotherPdg2 = ST_pdgFirstMother;
      motherLabel2 = ST_particle->GetMother(0); // Note: That's not the first mother of the track.
      firstMotherLabel2 = ST_labelFirstMother;
      firstMotherLabel2_min = ST_labelMinFirstMother;
      firstMotherLabel2_max = ST_labelMaxFirstMother;
      DCAxy2 = ST_dcaR;
      DCAz2 = ST_dcaZ;
      nITS2 = ST_nITS;
      nITSshared2 = ST_nITSshared;
      nTPC2 = ST_nTPC;
      ITSchi22 = ST_ITSchi2;
      TPCchi22 = ST_TPCchi2;
      phi2 = ST_phi;
      eta2 = ST_eta;
      pt2 = ST_pt;
    }

    
    Int_t tr2 = tr1+1;
    while(true) { // loop over remaining tracks of the same event
      singleTree->GetEntry(tr2);
      singleTree_MVAoutputs->GetEntry(tr2);

      // exit loop over remaining tracks if event number changes or if the last
      // event of the tree is reached:
      if(ev_temp != ST_event || tr2 == singleTree_nEvents-1) break;

      // pdg cut:
      if(abs(ST_pdg) != 11) { tr2++; continue; }
      
      // cut to ignore unphysical DCAz values (specific to prior analysis):
      if(ST_dcaZ == 999) { tr2++; continue; }

      // MVA cut tagging (conversion track identification):
      if(MVAoutput_convTrack < MVAcut_convTrack) {
	if(!doSwapCurrentPair) IsTaggedConvTrack2 = 1;
	else IsTaggedConvTrack1 = 1;
      }else {
	if(!doSwapCurrentPair) IsTaggedConvTrack2 = 0;
	else IsTaggedConvTrack1 = 0;
      }

      // already available, pairing-independent variables of 2nd leg for output tree:
      if(!doSwapCurrentPair) {
	EventID2 = ST_event;
	TrackID2 = tr2;
	mcPx2 = ST_particle->Px();
	mcPy2 = ST_particle->Py();
	mcPz2 = ST_particle->Pz();
	pdg2 = ST_pdg;
	motherPdg2 = ST_pdgMother;
	firstMotherPdg2 = ST_pdgFirstMother;
	motherLabel2 = ST_particle->GetMother(0); // Note: That's not the first mother of the track.
	firstMotherLabel2 = ST_labelFirstMother;
	firstMotherLabel2_min = ST_labelMinFirstMother;
	firstMotherLabel2_max = ST_labelMaxFirstMother;
	DCAxy2 = ST_dcaR;
	DCAz2 = ST_dcaZ;
	nITS2 = ST_nITS;
	nITSshared2 = ST_nITSshared;
	nTPC2 = ST_nTPC;
	ITSchi22 = ST_ITSchi2;
	TPCchi22 = ST_TPCchi2;
	phi2 = ST_phi;
	eta2 = ST_eta;
	pt2 = ST_pt;
      }else {
	EventID1 = ST_event;
	TrackID1 = tr2;
	mcPx1 = ST_particle->Px();
	mcPy1 = ST_particle->Py();
	mcPz1 = ST_particle->Pz();
	pdg1 = ST_pdg;
	motherPdg1 = ST_pdgMother;
	firstMotherPdg1 = ST_pdgFirstMother;
	motherLabel1 = ST_particle->GetMother(0); // Note: That's not the first mother of the track.
	firstMotherLabel1 = ST_labelFirstMother;
	firstMotherLabel1_min = ST_labelMinFirstMother;
	firstMotherLabel1_max = ST_labelMaxFirstMother;
	DCAxy1 = ST_dcaR;
	DCAz1 = ST_dcaZ;
	nITS1 = ST_nITS;
	nITSshared1 = ST_nITSshared;
	nTPC1 = ST_nTPC;
	ITSchi21 = ST_ITSchi2;
	TPCchi21 = ST_TPCchi2;
	phi1 = ST_phi;
	eta1 = ST_eta;
	pt1 = ST_pt;
      }

      IsRP = 0; // default value
      
      if(pdg1*pdg2 < 0) { // <-> unlike sign pair (via MC info!)
	IsUS = 1;
	
	ChargeSign = 0;

	calculateMomenta(); // has to be called before other methods that need momentum variables!
	calculateMass();
	calculatePhiv();
	calculateOpang();
        calculateDiffz();
	calculateSumz();
	calculateHF();

	if(motherPdg1==22 || motherPdg2==22) { // IsConv==1 for every pair that contains a conversion leg
	  IsConv = 1;
	}else {
	  IsConv = 0;
	}

	// prefilter cuts - tagging RP conversions:
	if(phiv<TMath::PiOver2() && mass<.05) {
	  IsTaggedRPConv = 1;
	}else {
	  IsTaggedRPConv = 0;
	}

	
	if(motherLabel1==motherLabel2 && abs(pdg1)==11 && abs(pdg2)==11) {
	  IsRP = 1;
	  if(isPairTree_rp) {
	    pairTree_rp->Fill();
	    cnt_rp++;
	  }
	}
	if(isPairTree_us) {
	  pairTree_us->Fill();
	  cnt_us++;
	}
	if(isPairTree_us_ls) {
	  pairTree_us_ls->Fill();
	  cnt_us_ls++;
	}
      }else { // <-> like sign pair
	IsUS = 0;
	
	if(pdg1>0) ChargeSign = 1;
	else if(pdg1<0) ChargeSign = -1;

	calculateMomenta(); // has to be called before other methods that need momentum variables!
	calculateMass();
	calculatePhiv();
	calculateOpang();
        calculateDiffz();
	calculateSumz();
	calculateHF();
	
	if(motherPdg1==22 || motherPdg2==22) {
	  IsConv = 1;
	}else {
	  IsConv = 0;
	}

	
	// prefilter cuts - tagging RP conversions:
	if(phiv<TMath::PiOver2() && mass<.05) {
	  IsTaggedRPConv = 1;
	}else {
	  IsTaggedRPConv = 0;
	}
	

	if(motherLabel1==motherLabel2 && abs(pdg1)==11 && abs(pdg2)==11) {
	  IsRP = 1;
	  if(isPairTree_rp) {
	    pairTree_rp->Fill();
	    cnt_rp++;
	  }
	}
	if(isPairTree_ls) {
	  pairTree_ls->Fill();
	  cnt_ls++;
	}
	if(isPairTree_us_ls) {
	  pairTree_us_ls->Fill();
	  cnt_us_ls++;
	}
      }
      
      tr2++;
    }
  }
  std::cout << "\rProcessing event " << singleTree_nEvents << " of " << singleTree_nEvents
	    << " (100%)... DONE." << std::endl;
  std::cout << "Time elapsed since begin of event processing: " << std::endl;
  std::cout << "\t";
  watch->Print();
  std::cout << std::endl;
  watch->Stop();

  infile->Close();
  
  if(isPairTree_rp) pairTree_rp->Write();
  if(isPairTree_us) pairTree_us->Write();
  if(isPairTree_ls) pairTree_ls->Write();
  if(isPairTree_us_ls) pairTree_us_ls->Write();

  outfile->Close();

  std::cout << std::endl << "---------- SUMMARY: ----------" << std::endl;
  std::cout << "Filled the histograms with the following number of entries:" << std::endl;
  std::cout << "  us: " << "\t" << cnt_us << std::endl
	    << "  ls: " << "\t" << cnt_ls << std::endl
	    << "  ls+us: " << "\t" << cnt_us_ls << std::endl << std::endl;

  // histDiagnosis1->SaveAs("histDiagnosis1.root");
  // histDiagnosis2->SaveAs("histDiagnosis2.root");

  
  gApplication->Terminate();
}




Bool_t isCharm(Int_t pdg) {
  if(TMath::Abs(pdg) == 4) return kTRUE;           // charmed quark
  if(TMath::Abs(pdg)/1000 == 4) return kTRUE;      // charmed baryon
  if((TMath::Abs(pdg)/100)%100 == 4) return kTRUE; // charmed meson
  return kFALSE;
}


Bool_t isBottom(Int_t pdg) {
  if(TMath::Abs(pdg) == 5) return kTRUE;           // charmed quark
  if(TMath::Abs(pdg)/1000 == 5) return kTRUE;      // charmed baryon
  if((TMath::Abs(pdg)/100)%100 == 5) return kTRUE; // charmed meson
  return kFALSE;
}


void calculateMomenta() { // has to be called before other methods that need momentum variables!
  if(!doSwapCurrentPair) { // <-> do not swap pairs
    px1 = pt1*TMath::Cos(phi1);
    py1 = pt1*TMath::Sin(phi1);
    pz1 = pt1*TMath::SinH(eta1);

    px2 = pt2*TMath::Cos(phi2);
    py2 = pt2*TMath::Sin(phi2);
    pz2 = pt2*TMath::SinH(eta2);

    pv1.SetXYZ(px1, py1, pz1);
    pv2.SetXYZ(px2, py2, pz2);
  } else { // <-> swap pairs
    px2 = pt1*TMath::Cos(phi1);
    py2 = pt1*TMath::Sin(phi1);
    pz2 = pt1*TMath::SinH(eta1);

    px1 = pt2*TMath::Cos(phi2);
    py1 = pt2*TMath::Sin(phi2);
    pz1 = pt2*TMath::SinH(eta2);

    pv2.SetXYZ(px1, py1, pz1);
    pv1.SetXYZ(px2, py2, pz2);
  }
}


void calculatePhiv() {
  if(pv1.Mag()==0 || pv2.Mag()==0) {
    std::cout << "Error in 'calculatePhiv()': Variable pv1 and/or pv2 empty." << std::endl;
    return;
  }
  
  if(pdg1 < 0) {
    temp = pv1;
    pv1 = pv2;
    pv2 = temp;
  }
  
  u = pv1 + pv2;
  u = u.Unit();

  v = pv1.Cross(pv2);
  // if(doSwapCurrentPair) v -= v;

  w = u.Cross(v);

  ua = u.Cross(z);
  ua = ua.Unit();

  phiv = w.Angle(ua);
  if(motherPdg1==22 && motherPdg2==22 && motherLabel1==motherLabel2
     && abs(pdg1)==11 && abs(pdg2)==11 && phiv>TMath::PiOver2()) {
    // For RP conversion, make phiv<pi/2 if phiv is much larger than pi/2
    // (then the B field was wrong):
    phiv = TMath::Pi() - phiv;
  }
}


void calculateOpang() {
  opang = pv1.Angle(pv2);
}


void calculateMass() {
  if(pv1.Mag()==0 || pv2.Mag()==0) {
    std::cout << "Error in 'calculateMass()': Variable pv1 and/or pv2 empty." << std::endl;
    return;
  }
  
  mass = 2*pv1.Mag()*pv2.Mag()*(1-TMath::Cos(pv1.Angle(pv2)));
  mass = TMath::Sqrt(TMath::Abs(mass));
}


void calculateDiffz() {
  if(pv1.Mag()==0 || pv2.Mag()==0) {
    std::cout << "Error in 'calculateDiffz()': Variable pv1 and/or pv2 empty." << std::endl;
    return;
  }
  
  temp = pv1.Unit() - pv2.Unit();
  if(doSwapCurrentPair) temp *= -1;
  diffz = temp.Angle(z);
}


void calculateSumz() {
  if(pv1.Mag()==0 || pv2.Mag()==0) {
    std::cout << "Error in 'calculateSumz()': Variable pv1 and/or pv2 empty." << std::endl;
    return;
  }
  
  temp = pv1 + pv2;
  sumz = temp.Angle(z);
}


void calculateHF() {
  IsHF = 0;                  // }
  IsCorrCharm = 0;           // } default
  IsCorrBottom = 0;          // } values
  IsCorrCharmFromBottom = 0; // }
  
  // check heavy flavor of first mothers:
  if((isCharm(firstMotherPdg1)||isBottom(firstMotherPdg1))
     && (isCharm(firstMotherPdg2)||isBottom(firstMotherPdg2))) {
    
    // check whether they are in the same first mother range (i.e., have the same origin):
    if(firstMotherLabel1>=firstMotherLabel2_min && firstMotherLabel1<=firstMotherLabel2_max) {
      
      if(!(firstMotherLabel2>=firstMotherLabel1_min && firstMotherLabel2<=firstMotherLabel1_max)) {
	std::cout << "Warning: firstMotherLabel1 is in firstMotherLabel2 range, but not vice versa." << std::endl;
      }
      
      // check heavy flavor of mothers:
      if((isCharm(motherPdg1)||isBottom(motherPdg1)) && (isCharm(motherPdg2)||isBottom(motherPdg2))) {
	
	if(isCharm(firstMotherPdg1) && isCharm(firstMotherPdg2)) {
	  IsCorrCharm = 1;
	}else if(isBottom(firstMotherPdg1) && isBottom(firstMotherPdg2)) {
	  IsCorrBottom = 1;
	  
	  if(isCharm(motherPdg1)||isCharm(motherPdg2)) {
	    IsCorrCharmFromBottom = 1;
	  }
	  
	}
	
      }
      
    }
    
  }
  
  if(IsCorrCharm==1 || IsCorrBottom==1) {
    IsHF = 1;
  }
  
}
