//gSystem.Load("libPhysics.so");

// Abbreviations:
// rp <-> real pair
// us <-> unlike sign
// ls <-> like sign

#include <iostream>
#include <cstdlib>

#include <TROOT.h>
#include <TApplication.h>
#include <TDirectory.h>
#include <TSystemDirectory.h>
#include <TSystemFile.h>
#include <TList.h>
#include <TFile.h>
#include <TChain.h>
#include <TTree.h>
#include <TH1D.h>
#include <TParticle.h>
#include <TMath.h>
#include <TRandom.h>
#include <TStopwatch.h>

Bool_t isCharm(Int_t );
Bool_t isBottom(Int_t );

Float_t getPIDefficiency(TH1D&, Float_t);

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
const Bool_t doConsiderMVAinfo_convTrack = kFALSE;
const Float_t MVAcut_convTrack = .41; // MVA output<MVAcut_convTrack <-> conversion track

// // MVA cut value (For identifying real pair conversions):
// const Float_t MVAcut_RPConv = 0.; // MVA output>MVAcut_RPConv <-> RP conv track


// cut values for classical RP conv. cuts:
Float_t phiv_cutValue = 2.4;  // RP convs. at phiv>phiv_cutValue
Float_t mass_cutValue = .01; // RP convs. at mass<mass_cutValue

// Loose track cuts settings: set kTRUE if the tree also contains tracks with looser cuts:
const Bool_t doContainLooseTracks = kFALSE;

// Online prefilter setting: kTRUE if tree contains branches with online prefilter tags
const Bool_t doContainOnlinePrefilter = kTRUE;

const Bool_t doRandPairSwap = kFALSE; // do random pair swapping?

Bool_t doSwapCurrentPair = kFALSE;


// some counting variables:
Int_t cnt_ls = 0, cnt_us = 0, cnt_us_ls = 0, cnt_rp = 0;


// output variables (1,2 <-> 1st leg, 2nd leg):
Int_t TrackID1, TrackID2;
Int_t EventID1, EventID2;
Long64_t EventID1_unique, EventID2_unique; // continuously assigned, unique event IDs
Int_t IsRP;                         // real pairs: 1, combinatorial pairs: 0
Int_t IsUS;                         // pair with unlike sign (regardless of IsRP)
Int_t IsConv;                       // both mother particles are gammas (can still be comb. pairs)
Int_t IsCorrHF;                     // correlated heavy-flavor (HF) pair
Int_t IsCorrCharm;                  // correlated charmed pair: 1, else: 0
Int_t IsCorrBottom;                 // correlated bottom pair: 1, else: 0
Int_t IsCorrCharmFromBottom;        // correlated charmed pair originating from a bottom: 1, else: 0
Int_t IsCombHF;                     // combinatorial HF pair
Int_t ChargeSign;                   // unlike sign: 0, like sign (++): 1, like sign (--): -1
Int_t IsTaggedRPConv_classicalCuts; // tagged as real pair conversion event: 1, otherwise: 0
Int_t IsTaggedRPConv_classicalCuts_tight;  // RP conv. <-> (phiv>2.9 && mass<0.0035)
Int_t IsTaggedRPConv_classicalCuts_loose1; // RP conv. <-> (phiv>2 && mass<0.04)
Int_t IsTaggedRPConv_classicalCuts_loose2; // RP conv. <-> (phiv>pi/2 && mass<0.05)
//Int_t IsTaggedRPConv_MVAcuts;     // tagged as real pair conversion event: 1, otherwise: 0
Int_t IsTaggedConvTrack1;           // tagged as conversion track (by preceding...
Int_t IsTaggedConvTrack2;           // ...MVA classification): 1, otherwise: 0
Float_t MVAoutput_convTrack1;       // MVA output values of preceding application of...
Float_t MVAoutput_convTrack2;       // ...the trained classifier (if available)
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
Float_t PIDeff1, PIDeff2;
Int_t pdg1, pdg2;
Int_t motherPdg1, motherPdg2;
Int_t firstMotherPdg1, firstMotherPdg2;
Int_t motherLabel1, motherLabel2;
Int_t firstMotherLabel1, firstMotherLabel2;
Int_t firstMotherLabel1_min, firstMotherLabel2_min;
Int_t firstMotherLabel1_max, firstMotherLabel2_max;
Float_t DCAxy1_norm, DCAxy2_norm;
Float_t DCAz1_norm, DCAz2_norm;
Float_t DCAx1, DCAx2, DCAy1, DCAy2, DCAz1, DCAz2;
Int_t nITS1, nITS2;
Float_t nITSshared1, nITSshared2;
Int_t nTPC1, nTPC2;
// Float_t chi2g1, chi2g2;
Float_t ITSchi21, ITSchi22;
Float_t TPCchi21, TPCchi22;
Float_t phi1, phi2;
Float_t eta1, eta2;
Float_t pt1, pt2;
Int_t TrackCut1, TrackCut2; // 0: ITS standalone tracks, 1: loose track cuts, 2: normal (tight) track cuts
Int_t IsTaggedRPConv_classicalCuts_onlinePrefilter;
Int_t IsTaggedRPConv_classicalCuts_onlinePrefilter_tight;
Int_t IsTaggedRPConv_classicalCuts_onlinePrefilter_loose1;
Int_t IsTaggedRPConv_classicalCuts_onlinePrefilter_loose2;

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

  const char *input_dirname = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/workingData/DNNAnalysis/";
  const char *file_ext = "FT2_ITSup_onlinePrefilter-allTracks_singleTree_part2_538273events.root";
  TSystemDirectory input_dir(input_dirname, input_dirname);
  TList *input_files = input_dir.GetListOfFiles();
  
  TChain *singleTree = new TChain("tracks");
  if(input_files) {
    std::cout << "Reading \"*" << file_ext << "\" files from "
	      << input_dirname << "...";
    TSystemFile *file;
    TString fname;
    TIter next(input_files);
    while((file = (TSystemFile*)next())) {
      fname = file->GetName();
      if(!file->IsDirectory() && fname.EndsWith(file_ext)) {
	singleTree->Add(input_dirname + fname);
      }
    }
  }

  
  TString infile_MVAoutputs_name = "~/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/singleTrackConvRejMVA_DNN_noMassCuts/output/predictions_NN.root";
  
  TChain *singleTree_MVAoutputs = new TChain("singleTree_MVAoutput");
  if(doConsiderMVAinfo_convTrack) {
    singleTree_MVAoutputs->AddFile(infile_MVAoutputs_name);
  }

  TString infile_PIDefficiencies_name = "~/analysis/data/FT2_AnalysisResults_Upgrade/inputData/ITSU_PIDefficiency_lowB.root";
  TFile *infile_PIDefficiencies = new TFile(infile_PIDefficiencies_name, "READ");
  TH1D *h_PIDeff = (TH1D*)infile_PIDefficiencies->Get("efficiencyLHC17d12_TPCandTOF3sigma");
  h_PIDeff->GetXaxis()->SetRangeUser(0,5);
  
    
  
  TFile *outfile = TFile::Open("temp_output/output_pairtrees.root","RECREATE");
  TTree *pairTree_rp = new TTree("pairTree_rp","pairTree_rp");
  TTree *pairTree_us = new TTree("pairTree_us","pairTree_us");
  TTree *pairTree_ls = new TTree("pairTree_ls","pairTree_ls");
  TTree *pairTree_us_ls = new TTree("pairTree_us_ls","pairTree_us_ls");


  if(doConsiderMVAinfo_convTrack && (singleTree->GetEntries() != singleTree_MVAoutputs->GetEntries())) {
    std::cout << "  ERROR: The trees in the input files have different sizes."
	      << std::endl;
    std::cout << "    Size of tree in input files: "
	      << singleTree->GetEntries() << std::endl;
    std::cout << "    Size of tree in file " << infile_MVAoutputs_name << ": "
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
  Double_t ST_dcaR_norm;
  Double_t ST_dcaZ_norm;
  Double_t ST_dcaX, ST_dcaY, ST_dcaZ;
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
  Int_t ST_isTrackCut;
  Int_t ST_taggedByPrefilter;
  Int_t ST_taggedByPrefilter_tight;
  Int_t ST_taggedByPrefilter_loose1;
  Int_t ST_taggedByPrefilter_loose2;
  
  TParticle* ST_particle = NULL;
  
  Long64_t ST_eventID_unique = 0;
  
  
  singleTree->SetBranchAddress("event",&ST_event);
  singleTree->SetBranchAddress("IP",&ST_IP);
  singleTree->SetBranchAddress("global",&ST_global);
  singleTree->SetBranchAddress("eta",&ST_eta);
  singleTree->SetBranchAddress("phi",&ST_phi);
  singleTree->SetBranchAddress("pt",&ST_pt);
  singleTree->SetBranchAddress("dcaR_norm",&ST_dcaR_norm);
  singleTree->SetBranchAddress("dcaZ_norm",&ST_dcaZ_norm);
  singleTree->SetBranchAddress("dcaX", &ST_dcaX);
  singleTree->SetBranchAddress("dcaY", &ST_dcaY);
  singleTree->SetBranchAddress("dcaZ", &ST_dcaZ);
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
  if(doContainLooseTracks) singleTree->SetBranchAddress("isTrackCut",&ST_isTrackCut);
  if(doContainOnlinePrefilter) singleTree->SetBranchAddress("taggedByPrefilter_standardCuts",&ST_taggedByPrefilter);
  if(doContainOnlinePrefilter) singleTree->SetBranchAddress("taggedByPrefilter_tighterCuts",&ST_taggedByPrefilter_tight);
  if(doContainOnlinePrefilter) singleTree->SetBranchAddress("taggedByPrefilter_looserCuts1",&ST_taggedByPrefilter_loose1);
  if(doContainOnlinePrefilter) singleTree->SetBranchAddress("taggedByPrefilter_looserCuts2",&ST_taggedByPrefilter_loose2);
  


  if(isPairTree_rp) {
    pairTree_rp->Branch("TrackID1",&TrackID1);
    pairTree_rp->Branch("TrackID2",&TrackID2);
    pairTree_rp->Branch("EventID1",&EventID1);
    pairTree_rp->Branch("EventID2",&EventID2);
    pairTree_rp->Branch("EventID1_unique",&EventID1_unique);
    pairTree_rp->Branch("EventID2_unique",&EventID2_unique);
    pairTree_rp->Branch("IsRP",&IsRP);
    pairTree_rp->Branch("IsUS",&IsUS);
    pairTree_rp->Branch("IsConv",&IsConv);
    pairTree_rp->Branch("IsCorrHF",&IsCorrHF);
    pairTree_rp->Branch("IsCorrCharm",&IsCorrCharm);
    pairTree_rp->Branch("IsCorrBottom",&IsCorrBottom);
    pairTree_rp->Branch("IsCorrCharmFromBottom",&IsCorrCharmFromBottom);
    pairTree_rp->Branch("IsCombHF",&IsCombHF);
    pairTree_rp->Branch("IsTaggedRPConv_classicalCuts_tight",&IsTaggedRPConv_classicalCuts_tight);
    pairTree_rp->Branch("IsTaggedRPConv_classicalCuts",&IsTaggedRPConv_classicalCuts);
    pairTree_rp->Branch("IsTaggedRPConv_classicalCuts_loose1",&IsTaggedRPConv_classicalCuts_loose1);
    pairTree_rp->Branch("IsTaggedRPConv_classicalCuts_loose2",&IsTaggedRPConv_classicalCuts_loose2);
    if(doConsiderMVAinfo_convTrack)
      pairTree_rp->Branch("IsTaggedConvTrack1",&IsTaggedConvTrack1);
    if(doConsiderMVAinfo_convTrack)
      pairTree_rp->Branch("IsTaggedConvTrack2",&IsTaggedConvTrack2);
    if(doConsiderMVAinfo_convTrack)
      pairTree_rp->Branch("MVAoutput_convTrack1", &MVAoutput_convTrack1);
    if(doConsiderMVAinfo_convTrack)
      pairTree_rp->Branch("MVAoutput_convTrack2", &MVAoutput_convTrack2);
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
    pairTree_rp->Branch("PIDeff1",&PIDeff1);
    pairTree_rp->Branch("PIDeff2",&PIDeff2);
    pairTree_rp->Branch("DCAxy1_norm",&DCAxy1_norm);
    pairTree_rp->Branch("DCAxy2_norm",&DCAxy2_norm);
    pairTree_rp->Branch("DCAz1_norm",&DCAz1_norm);
    pairTree_rp->Branch("DCAz2_norm",&DCAz2_norm);
    pairTree_rp->Branch("DCAx1", &DCAx1);
    pairTree_rp->Branch("DCAx2", &DCAx2);
    pairTree_rp->Branch("DCAy1", &DCAy1);
    pairTree_rp->Branch("DCAy2", &DCAy2);
    pairTree_rp->Branch("DCAz1", &DCAz1);
    pairTree_rp->Branch("DCAz2", &DCAz2);
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
    if(doContainLooseTracks) pairTree_rp->Branch("TrackCut1",&TrackCut1);
    if(doContainLooseTracks) pairTree_rp->Branch("TrackCut2",&TrackCut2);
    if(doContainOnlinePrefilter) pairTree_rp->Branch("IsTaggedRPConv_classicalCuts_onlinePrefilter",&IsTaggedRPConv_classicalCuts_onlinePrefilter);
    if(doContainOnlinePrefilter) pairTree_rp->Branch("IsTaggedRPConv_classicalCuts_onlinePrefilter_tight",&IsTaggedRPConv_classicalCuts_onlinePrefilter_tight);
    if(doContainOnlinePrefilter) pairTree_rp->Branch("IsTaggedRPConv_classicalCuts_onlinePrefilter_loose1",&IsTaggedRPConv_classicalCuts_onlinePrefilter_loose1);
    if(doContainOnlinePrefilter) pairTree_rp->Branch("IsTaggedRPConv_classicalCuts_onlinePrefilter_loose2",&IsTaggedRPConv_classicalCuts_onlinePrefilter_loose2);
  }

  if(isPairTree_us) {
    pairTree_us->Branch("TrackID1",&TrackID1);
    pairTree_us->Branch("TrackID2",&TrackID2);
    pairTree_us->Branch("EventID1",&EventID1);
    pairTree_us->Branch("EventID2",&EventID2);
    pairTree_us->Branch("EventID1_unique",&EventID1_unique);
    pairTree_us->Branch("EventID2_unique",&EventID2_unique);
    pairTree_us->Branch("IsRP",&IsRP);
    pairTree_us->Branch("IsUS",&IsUS);
    pairTree_us->Branch("IsConv",&IsConv);
    pairTree_us->Branch("IsCorrHF",&IsCorrHF);
    pairTree_us->Branch("IsCorrCharm",&IsCorrCharm);
    pairTree_us->Branch("IsCorrBottom",&IsCorrBottom);
    pairTree_us->Branch("IsCorrCharmFromBottom",&IsCorrCharmFromBottom);
    pairTree_us->Branch("IsCombHF",&IsCombHF);
    pairTree_us->Branch("IsTaggedRPConv_classicalCuts_tight",&IsTaggedRPConv_classicalCuts_tight);
    pairTree_us->Branch("IsTaggedRPConv_classicalCuts",&IsTaggedRPConv_classicalCuts);
    pairTree_us->Branch("IsTaggedRPConv_classicalCuts_loose1",&IsTaggedRPConv_classicalCuts_loose1);
    pairTree_us->Branch("IsTaggedRPConv_classicalCuts_loose2",&IsTaggedRPConv_classicalCuts_loose2);
    if(doConsiderMVAinfo_convTrack)
      pairTree_us->Branch("IsTaggedConvTrack1",&IsTaggedConvTrack1);
    if(doConsiderMVAinfo_convTrack)
      pairTree_us->Branch("IsTaggedConvTrack2",&IsTaggedConvTrack2);
    if(doConsiderMVAinfo_convTrack)
      pairTree_us->Branch("MVAoutput_convTrack1", &MVAoutput_convTrack1);
    if(doConsiderMVAinfo_convTrack)
      pairTree_us->Branch("MVAoutput_convTrack2", &MVAoutput_convTrack2);
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
    pairTree_us->Branch("PIDeff1",&PIDeff1);
    pairTree_us->Branch("PIDeff2",&PIDeff2);
    pairTree_us->Branch("DCAxy1_norm",&DCAxy1_norm);
    pairTree_us->Branch("DCAxy2_norm",&DCAxy2_norm);
    pairTree_us->Branch("DCAz1_norm",&DCAz1_norm);
    pairTree_us->Branch("DCAz2_norm",&DCAz2_norm);
    pairTree_us->Branch("DCAx1", &DCAx1);
    pairTree_us->Branch("DCAx2", &DCAx2);
    pairTree_us->Branch("DCAy1", &DCAy1);
    pairTree_us->Branch("DCAy2", &DCAy2);
    pairTree_us->Branch("DCAz1", &DCAz1);
    pairTree_us->Branch("DCAz2", &DCAz2);
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
    if(doContainLooseTracks) pairTree_us->Branch("TrackCut1",&TrackCut1);
    if(doContainLooseTracks) pairTree_us->Branch("TrackCut2",&TrackCut2);
    if(doContainOnlinePrefilter) pairTree_us->Branch("IsTaggedRPConv_classicalCuts_onlinePrefilter",&IsTaggedRPConv_classicalCuts_onlinePrefilter);
    if(doContainOnlinePrefilter) pairTree_us->Branch("IsTaggedRPConv_classicalCuts_onlinePrefilter_tight",&IsTaggedRPConv_classicalCuts_onlinePrefilter_tight);
    if(doContainOnlinePrefilter) pairTree_us->Branch("IsTaggedRPConv_classicalCuts_onlinePrefilter_loose1",&IsTaggedRPConv_classicalCuts_onlinePrefilter_loose1);
    if(doContainOnlinePrefilter) pairTree_us->Branch("IsTaggedRPConv_classicalCuts_onlinePrefilter_loose2",&IsTaggedRPConv_classicalCuts_onlinePrefilter_loose2);
  }

  if(isPairTree_ls) {
    pairTree_ls->Branch("TrackID1",&TrackID1);
    pairTree_ls->Branch("TrackID2",&TrackID2);
    pairTree_ls->Branch("EventID1",&EventID1);
    pairTree_ls->Branch("EventID2",&EventID2);
    pairTree_ls->Branch("EventID1_unique",&EventID1_unique);
    pairTree_ls->Branch("EventID2_unique",&EventID2_unique);
    pairTree_ls->Branch("IsRP",&IsRP);
    pairTree_ls->Branch("IsUS",&IsUS);
    pairTree_ls->Branch("IsConv",&IsConv);
    pairTree_ls->Branch("IsCorrHF",&IsCorrHF);
    pairTree_ls->Branch("IsCorrCharm",&IsCorrCharm);
    pairTree_ls->Branch("IsCorrBottom",&IsCorrBottom);
    pairTree_ls->Branch("IsCorrCharmFromBottom",&IsCorrCharmFromBottom);
    pairTree_ls->Branch("IsCombHF",&IsCombHF);
    pairTree_ls->Branch("IsTaggedRPConv_classicalCuts_tight",&IsTaggedRPConv_classicalCuts_tight);
    pairTree_ls->Branch("IsTaggedRPConv_classicalCuts",&IsTaggedRPConv_classicalCuts);
    pairTree_ls->Branch("IsTaggedRPConv_classicalCuts_loose1",&IsTaggedRPConv_classicalCuts_loose1);
    pairTree_ls->Branch("IsTaggedRPConv_classicalCuts_loose2",&IsTaggedRPConv_classicalCuts_loose2);
    if(doConsiderMVAinfo_convTrack)
      pairTree_ls->Branch("IsTaggedConvTrack1",&IsTaggedConvTrack1);
    if(doConsiderMVAinfo_convTrack)
      pairTree_ls->Branch("IsTaggedConvTrack2",&IsTaggedConvTrack2);
    if(doConsiderMVAinfo_convTrack)
      pairTree_ls->Branch("MVAoutput_convTrack1", &MVAoutput_convTrack1);
    if(doConsiderMVAinfo_convTrack)
      pairTree_ls->Branch("MVAoutput_convTrack2", &MVAoutput_convTrack2);
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
    pairTree_ls->Branch("PIDeff1",&PIDeff1);
    pairTree_ls->Branch("PIDeff2",&PIDeff2);
    pairTree_ls->Branch("DCAxy1_norm",&DCAxy1_norm);
    pairTree_ls->Branch("DCAxy2_norm",&DCAxy2_norm);
    pairTree_ls->Branch("DCAz1_norm",&DCAz1_norm);
    pairTree_ls->Branch("DCAz2_norm",&DCAz2_norm);
    pairTree_ls->Branch("DCAx1", &DCAx1);
    pairTree_ls->Branch("DCAx2", &DCAx2);
    pairTree_ls->Branch("DCAy1", &DCAy1);
    pairTree_ls->Branch("DCAy2", &DCAy2);
    pairTree_ls->Branch("DCAz1", &DCAz1);
    pairTree_ls->Branch("DCAz2", &DCAz2);
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
    if(doContainLooseTracks) pairTree_ls->Branch("TrackCut1",&TrackCut1);
    if(doContainLooseTracks) pairTree_ls->Branch("TrackCut2",&TrackCut2);
    if(doContainOnlinePrefilter) pairTree_ls->Branch("IsTaggedRPConv_classicalCuts_onlinePrefilter",&IsTaggedRPConv_classicalCuts_onlinePrefilter);
    if(doContainOnlinePrefilter) pairTree_ls->Branch("IsTaggedRPConv_classicalCuts_onlinePrefilter_tight",&IsTaggedRPConv_classicalCuts_onlinePrefilter_tight);
    if(doContainOnlinePrefilter) pairTree_ls->Branch("IsTaggedRPConv_classicalCuts_onlinePrefilter_loose1",&IsTaggedRPConv_classicalCuts_onlinePrefilter_loose1);
    if(doContainOnlinePrefilter) pairTree_ls->Branch("IsTaggedRPConv_classicalCuts_onlinePrefilter_loose2",&IsTaggedRPConv_classicalCuts_onlinePrefilter_loose2);
  }

  if(isPairTree_us_ls) {
    pairTree_us_ls->Branch("TrackID1",&TrackID1);
    pairTree_us_ls->Branch("TrackID2",&TrackID2);
    pairTree_us_ls->Branch("EventID1",&EventID1);
    pairTree_us_ls->Branch("EventID2",&EventID2);
    pairTree_us_ls->Branch("EventID1_unique",&EventID1_unique);
    pairTree_us_ls->Branch("EventID2_unique",&EventID2_unique);
    pairTree_us_ls->Branch("IsRP",&IsRP);
    pairTree_us_ls->Branch("IsUS",&IsUS);
    pairTree_us_ls->Branch("IsConv",&IsConv);
    pairTree_us_ls->Branch("IsCorrHF",&IsCorrHF);
    pairTree_us_ls->Branch("IsCorrCharm",&IsCorrCharm);
    pairTree_us_ls->Branch("IsCorrBottom",&IsCorrBottom);
    pairTree_us_ls->Branch("IsCorrCharmFromBottom",&IsCorrCharmFromBottom);
    pairTree_us_ls->Branch("IsCombHF",&IsCombHF);
    pairTree_us_ls->Branch("IsTaggedRPConv_classicalCuts_tight",&IsTaggedRPConv_classicalCuts_tight);
    pairTree_us_ls->Branch("IsTaggedRPConv_classicalCuts",&IsTaggedRPConv_classicalCuts);
    pairTree_us_ls->Branch("IsTaggedRPConv_classicalCuts_loose1",&IsTaggedRPConv_classicalCuts_loose1);
			   pairTree_us_ls->Branch("IsTaggedRPConv_classicalCuts_loose2",&IsTaggedRPConv_classicalCuts_loose2);
    if(doConsiderMVAinfo_convTrack)
      pairTree_us_ls->Branch("IsTaggedConvTrack1",&IsTaggedConvTrack1);
    if(doConsiderMVAinfo_convTrack)
      pairTree_us_ls->Branch("IsTaggedConvTrack2",&IsTaggedConvTrack2);
    if(doConsiderMVAinfo_convTrack)
      pairTree_us_ls->Branch("MVAoutput_convTrack1", &MVAoutput_convTrack1);
    if(doConsiderMVAinfo_convTrack)
      pairTree_us_ls->Branch("MVAoutput_convTrack2", &MVAoutput_convTrack2);
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
    pairTree_us_ls->Branch("PIDeff1",&PIDeff1);
    pairTree_us_ls->Branch("PIDeff2",&PIDeff2);
    pairTree_us_ls->Branch("DCAxy1_norm",&DCAxy1_norm);
    pairTree_us_ls->Branch("DCAxy2_norm",&DCAxy2_norm);
    pairTree_us_ls->Branch("DCAz1_norm",&DCAz1_norm);
    pairTree_us_ls->Branch("DCAz2_norm",&DCAz2_norm);
    pairTree_us_ls->Branch("DCAx1", &DCAx1);
    pairTree_us_ls->Branch("DCAx2", &DCAx2);
    pairTree_us_ls->Branch("DCAy1", &DCAy1);
    pairTree_us_ls->Branch("DCAy2", &DCAy2);
    pairTree_us_ls->Branch("DCAz1", &DCAz1);
    pairTree_us_ls->Branch("DCAz2", &DCAz2);
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
    if(doContainLooseTracks) pairTree_us_ls->Branch("TrackCut1",&TrackCut1);
    if(doContainLooseTracks) pairTree_us_ls->Branch("TrackCut2",&TrackCut2);
    if(doContainOnlinePrefilter) pairTree_us_ls->Branch("IsTaggedRPConv_classicalCuts_onlinePrefilter",&IsTaggedRPConv_classicalCuts_onlinePrefilter);
    if(doContainOnlinePrefilter) pairTree_us_ls->Branch("IsTaggedRPConv_classicalCuts_onlinePrefilter_tight",&IsTaggedRPConv_classicalCuts_onlinePrefilter_tight);
    if(doContainOnlinePrefilter) pairTree_us_ls->Branch("IsTaggedRPConv_classicalCuts_onlinePrefilter_loose1",&IsTaggedRPConv_classicalCuts_onlinePrefilter_loose1);
    if(doContainOnlinePrefilter) pairTree_us_ls->Branch("IsTaggedRPConv_classicalCuts_onlinePrefilter_loose2",&IsTaggedRPConv_classicalCuts_onlinePrefilter_loose2);
  }


  Int_t IsTaggedRPConv_classicalCuts_onlinePrefilter1, IsTaggedRPConv_classicalCuts_onlinePrefilter2;
  Int_t IsTaggedRPConv_classicalCuts_onlinePrefilter1_tight, IsTaggedRPConv_classicalCuts_onlinePrefilter2_tight;
  Int_t IsTaggedRPConv_classicalCuts_onlinePrefilter1_loose1, IsTaggedRPConv_classicalCuts_onlinePrefilter2_loose1;
  Int_t IsTaggedRPConv_classicalCuts_onlinePrefilter1_loose2, IsTaggedRPConv_classicalCuts_onlinePrefilter2_loose2;
  

  Float_t MVAoutput_convTrack;
  if(doConsiderMVAinfo_convTrack) singleTree_MVAoutputs->SetBranchAddress("BDT", &MVAoutput_convTrack);
  

  Int_t firstTrack; // first track number in given event
  Int_t nTracks; // total number of tracks in given event

  
  Long64_t singleTree_nEvents = singleTree->GetEntries();

  Long64_t singleTree_nEvents_1percent = singleTree_nEvents/100;


  Long64_t ev_prev = -1;
  
  std::cout << std::endl << "Start event processing...";
  
  TStopwatch *watch = new TStopwatch();
  watch->Start();



  for(Long64_t tr1=0; tr1<singleTree_nEvents-1; tr1++) { // first track loop, do not search partner tracks in the last event of the tree
    if((tr1%singleTree_nEvents_1percent)==0)
      std::cout << "\rProcessing event " << tr1 << " of " << singleTree_nEvents
		<< " (" << tr1*100/singleTree_nEvents << "%)...";

    singleTree->GetEntry(tr1);
    if(doConsiderMVAinfo_convTrack) singleTree_MVAoutputs->GetEntry(tr1);

    
    Int_t ev_temp = ST_event; // marks the current event number

    if(ev_temp != ev_prev) {
      ST_eventID_unique++;
      ev_prev = ev_temp;
    }

    // pdg cut:
    if(abs(ST_pdg) != 11) continue; // keep electrons/positrons only

    // cut to ignore unphysical DCAz values (specific to prior analysis):
    if(ST_dcaZ == 999) continue;

    
    // prepare random pair swapping if doRandPairSwap==kTRUE:
    if(doRandPairSwap) {
      doSwapCurrentPair = (rand.Uniform() < .5) ? kTRUE : kFALSE;
    }else {
      doSwapCurrentPair = kFALSE;
    }


    // MVA cut tagging (conversion track identification):
    if(doConsiderMVAinfo_convTrack) {
      if(MVAoutput_convTrack < MVAcut_convTrack) {
	if(!doSwapCurrentPair) IsTaggedConvTrack1 = 1.;
	else IsTaggedConvTrack2 = 1.;
      }else {
	if(!doSwapCurrentPair) IsTaggedConvTrack1 = 0.;
	else IsTaggedConvTrack2 = 0.;
      }
    }


    // already available, pairing-independent variables of 1st leg for output tree:
    if(!doSwapCurrentPair) {
      EventID1 = ST_event;
      EventID1_unique = ST_eventID_unique;
      TrackID1 = tr1;
      MVAoutput_convTrack1 = MVAoutput_convTrack;
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
      DCAxy1_norm = ST_dcaR_norm;
      DCAz1_norm = ST_dcaZ_norm;
      DCAx1 = ST_dcaX;
      DCAy1 = ST_dcaY;
      DCAz1 = ST_dcaZ;
      nITS1 = ST_nITS;
      nITSshared1 = ST_nITSshared;
      nTPC1 = ST_nTPC;
      ITSchi21 = ST_ITSchi2;
      TPCchi21 = ST_TPCchi2;
      phi1 = ST_phi;
      eta1 = ST_eta;
      pt1 = ST_pt;
      PIDeff1 = getPIDefficiency(*h_PIDeff, ST_pt);
      if(doContainLooseTracks) TrackCut1 = ST_isTrackCut;
      if(doContainOnlinePrefilter) IsTaggedRPConv_classicalCuts_onlinePrefilter1 = ST_taggedByPrefilter;
      if(doContainOnlinePrefilter) IsTaggedRPConv_classicalCuts_onlinePrefilter1_tight = ST_taggedByPrefilter_tight;
      if(doContainOnlinePrefilter) IsTaggedRPConv_classicalCuts_onlinePrefilter1_loose1 = ST_taggedByPrefilter_loose1;
      if(doContainOnlinePrefilter) IsTaggedRPConv_classicalCuts_onlinePrefilter1_loose2 = ST_taggedByPrefilter_loose2;
    }else {
      EventID2 = ST_event;
      EventID2_unique = ST_eventID_unique;
      TrackID2 = tr1;
      MVAoutput_convTrack2 = MVAoutput_convTrack;
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
      DCAxy2_norm = ST_dcaR_norm;
      DCAz2_norm = ST_dcaZ_norm;
      DCAx2 = ST_dcaX;
      DCAy2 = ST_dcaY;
      DCAz2 = ST_dcaZ;
      nITS2 = ST_nITS;
      nITSshared2 = ST_nITSshared;
      nTPC2 = ST_nTPC;
      ITSchi22 = ST_ITSchi2;
      TPCchi22 = ST_TPCchi2;
      phi2 = ST_phi;
      eta2 = ST_eta;
      pt2 = ST_pt;
      PIDeff2 = getPIDefficiency(*h_PIDeff, ST_pt);
      if(doContainLooseTracks) TrackCut2 = ST_isTrackCut;
      if(doContainOnlinePrefilter) IsTaggedRPConv_classicalCuts_onlinePrefilter2 = ST_taggedByPrefilter;
      if(doContainOnlinePrefilter) IsTaggedRPConv_classicalCuts_onlinePrefilter2_tight = ST_taggedByPrefilter_tight;
      if(doContainOnlinePrefilter) IsTaggedRPConv_classicalCuts_onlinePrefilter2_loose1 = ST_taggedByPrefilter_loose1;
      if(doContainOnlinePrefilter) IsTaggedRPConv_classicalCuts_onlinePrefilter2_loose2 = ST_taggedByPrefilter_loose2;
    }

    
    Int_t tr2 = tr1+1;
    while(true) { // loop over remaining tracks of the same event
      singleTree->GetEntry(tr2);
      if(doConsiderMVAinfo_convTrack) singleTree_MVAoutputs->GetEntry(tr2);

      // exit loop over remaining tracks if event number changes or if the last
      // event of the tree is reached:
      if(ev_temp != ST_event || tr2 == singleTree_nEvents-1) break;

      // pdg cut:
      if(abs(ST_pdg) != 11) { tr2++; continue; }
      
      // cut to ignore unphysical DCAz values (specific to prior analysis):
      if(ST_dcaZ == 999) { tr2++; continue; }

      // MVA cut tagging (conversion track identification):
      if(doConsiderMVAinfo_convTrack) {
	if(MVAoutput_convTrack < MVAcut_convTrack) {
	  if(!doSwapCurrentPair) IsTaggedConvTrack2 = 1.;
	  else IsTaggedConvTrack1 = 1.;
	}else {
	  if(!doSwapCurrentPair) IsTaggedConvTrack2 = 0.;
	  else IsTaggedConvTrack1 = 0.;
	}
      }

      // already available, pairing-independent variables of 2nd leg for output tree:
      if(!doSwapCurrentPair) {
	EventID2 = ST_event;
	EventID2_unique = ST_eventID_unique;
	TrackID2 = tr2;
	MVAoutput_convTrack2 = MVAoutput_convTrack;
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
	DCAxy2_norm = ST_dcaR_norm;
	DCAz2_norm = ST_dcaZ_norm;
	DCAx2 = ST_dcaX;
	DCAy2 = ST_dcaY;
	DCAz2 = ST_dcaZ;
	nITS2 = ST_nITS;
	nITSshared2 = ST_nITSshared;
	nTPC2 = ST_nTPC;
	ITSchi22 = ST_ITSchi2;
	TPCchi22 = ST_TPCchi2;
	phi2 = ST_phi;
	eta2 = ST_eta;
	pt2 = ST_pt;
	PIDeff2 = getPIDefficiency(*h_PIDeff, ST_pt);
	if(doContainLooseTracks) TrackCut2 = ST_isTrackCut;
	if(doContainOnlinePrefilter) IsTaggedRPConv_classicalCuts_onlinePrefilter2 = ST_taggedByPrefilter;
	if(doContainOnlinePrefilter) IsTaggedRPConv_classicalCuts_onlinePrefilter2_tight = ST_taggedByPrefilter_tight;
	if(doContainOnlinePrefilter) IsTaggedRPConv_classicalCuts_onlinePrefilter2_loose1 = ST_taggedByPrefilter_loose1;
	if(doContainOnlinePrefilter) IsTaggedRPConv_classicalCuts_onlinePrefilter2_loose2 = ST_taggedByPrefilter_loose2;
      }else {
	EventID1 = ST_event;
	EventID1_unique = ST_eventID_unique;
	TrackID1 = tr2;
	MVAoutput_convTrack1 = MVAoutput_convTrack;
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
	DCAxy1_norm = ST_dcaR_norm;
	DCAz1_norm = ST_dcaZ_norm;
	DCAx1 = ST_dcaX;
	DCAy1 = ST_dcaY;
	DCAz1 = ST_dcaZ;
	nITS1 = ST_nITS;
	nITSshared1 = ST_nITSshared;
	nTPC1 = ST_nTPC;
	ITSchi21 = ST_ITSchi2;
	TPCchi21 = ST_TPCchi2;
	phi1 = ST_phi;
	eta1 = ST_eta;
	pt1 = ST_pt;
	PIDeff1 = getPIDefficiency(*h_PIDeff, ST_pt);
	if(doContainLooseTracks) TrackCut1 = ST_isTrackCut;
	if(doContainOnlinePrefilter) IsTaggedRPConv_classicalCuts_onlinePrefilter1 = ST_taggedByPrefilter;
	if(doContainOnlinePrefilter) IsTaggedRPConv_classicalCuts_onlinePrefilter1_tight = ST_taggedByPrefilter_tight;
	if(doContainOnlinePrefilter) IsTaggedRPConv_classicalCuts_onlinePrefilter1_loose1 = ST_taggedByPrefilter_loose1;
	if(doContainOnlinePrefilter) IsTaggedRPConv_classicalCuts_onlinePrefilter1_loose2 = ST_taggedByPrefilter_loose2;
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
	if(phiv>phiv_cutValue && mass<mass_cutValue) {
	  IsTaggedRPConv_classicalCuts = 1;
	}else {
	  IsTaggedRPConv_classicalCuts = 0;
	}
	
	// further (classical) prefilter cuts:
        if(phiv>2.9 && mass < .0035) {
          IsTaggedRPConv_classicalCuts_tight = 1;
        }else {
          IsTaggedRPConv_classicalCuts_tight = 0;
        }
        if(phiv>2 && mass<.04) {
          IsTaggedRPConv_classicalCuts_loose1 = 1;
        }else {
          IsTaggedRPConv_classicalCuts_loose1 = 0;
        }
        if(phiv>TMath::Pi()*.5 && mass<.05) {
          IsTaggedRPConv_classicalCuts_loose2 = 1;
        }else {
          IsTaggedRPConv_classicalCuts_loose2 = 0;
        }

	// online prefilter cuts:
	if(doContainOnlinePrefilter) {
	  if(IsTaggedRPConv_classicalCuts_onlinePrefilter1 == 1 ||
	     IsTaggedRPConv_classicalCuts_onlinePrefilter2 == 1)
	    IsTaggedRPConv_classicalCuts_onlinePrefilter = 1;
	  else
	    IsTaggedRPConv_classicalCuts_onlinePrefilter = 0;

	  if(IsTaggedRPConv_classicalCuts_onlinePrefilter1_tight == 1 ||
	     IsTaggedRPConv_classicalCuts_onlinePrefilter2_tight == 1)
	    IsTaggedRPConv_classicalCuts_onlinePrefilter_tight = 1;
	  else
	    IsTaggedRPConv_classicalCuts_onlinePrefilter_tight = 0;

	  if(IsTaggedRPConv_classicalCuts_onlinePrefilter1_loose1 == 1 ||
	     IsTaggedRPConv_classicalCuts_onlinePrefilter2_loose1 == 1)
	    IsTaggedRPConv_classicalCuts_onlinePrefilter_loose1 = 1;
	  else
	    IsTaggedRPConv_classicalCuts_onlinePrefilter_loose1 = 0;

	  if(IsTaggedRPConv_classicalCuts_onlinePrefilter1_loose2 == 1 ||
	     IsTaggedRPConv_classicalCuts_onlinePrefilter2_loose2 == 1)
	    IsTaggedRPConv_classicalCuts_onlinePrefilter_loose2 = 1;
	  else
	    IsTaggedRPConv_classicalCuts_onlinePrefilter_loose2 = 0;
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
        if(phiv>phiv_cutValue && mass<mass_cutValue) {
	  IsTaggedRPConv_classicalCuts = 1;
	}else {
	  IsTaggedRPConv_classicalCuts = 0;
	}

	// further (classical) prefilter cuts:
        if(phiv>2.9 && mass < .0035) {
          IsTaggedRPConv_classicalCuts_tight = 1;
        }else {
          IsTaggedRPConv_classicalCuts_tight = 0;
        }
        if(phiv>2 && mass<.04) {
          IsTaggedRPConv_classicalCuts_loose1 = 1;
        }else {
          IsTaggedRPConv_classicalCuts_loose1 = 0;
        }
        if(phiv>TMath::Pi()*.5 && mass<.05) {
          IsTaggedRPConv_classicalCuts_loose2 = 1;
        }else {
          IsTaggedRPConv_classicalCuts_loose2 = 0;
        }

	// online prefilter cuts:
	if(doContainOnlinePrefilter) {
	  if(IsTaggedRPConv_classicalCuts_onlinePrefilter1 == 1 ||
	     IsTaggedRPConv_classicalCuts_onlinePrefilter2 == 1)
	    IsTaggedRPConv_classicalCuts_onlinePrefilter = 1;
	  else
	    IsTaggedRPConv_classicalCuts_onlinePrefilter = 0;

	  if(IsTaggedRPConv_classicalCuts_onlinePrefilter1_tight == 1 ||
	     IsTaggedRPConv_classicalCuts_onlinePrefilter2_tight == 1)
	    IsTaggedRPConv_classicalCuts_onlinePrefilter_tight = 1;
	  else
	    IsTaggedRPConv_classicalCuts_onlinePrefilter_tight = 0;

	  if(IsTaggedRPConv_classicalCuts_onlinePrefilter1_loose1 == 1 ||
	     IsTaggedRPConv_classicalCuts_onlinePrefilter2_loose1 == 1)
	    IsTaggedRPConv_classicalCuts_onlinePrefilter_loose1 = 1;
	  else
	    IsTaggedRPConv_classicalCuts_onlinePrefilter_loose1 = 0;

	  if(IsTaggedRPConv_classicalCuts_onlinePrefilter1_loose2 == 1 ||
	     IsTaggedRPConv_classicalCuts_onlinePrefilter2_loose2 == 1)
	    IsTaggedRPConv_classicalCuts_onlinePrefilter_loose2 = 1;
	  else
	    IsTaggedRPConv_classicalCuts_onlinePrefilter_loose2 = 0;
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
  
  if(isPairTree_rp) pairTree_rp->Write(0, TObject::kOverwrite);
  if(isPairTree_us) pairTree_us->Write(0, TObject::kOverwrite);
  if(isPairTree_ls) pairTree_ls->Write(0, TObject::kOverwrite);
  if(isPairTree_us_ls) pairTree_us_ls->Write(0, TObject::kOverwrite);

  outfile->Close();

  std::cout << std::endl << "---------- SUMMARY: ----------" << std::endl;
  std::cout << "Filled the histograms with the following number of entries:" << std::endl;
  std::cout << "  us: " << "\t" << cnt_us << std::endl
	    << "  ls: " << "\t" << cnt_ls << std::endl
	    << "  ls+us: " << "\t" << cnt_us_ls << std::endl << std::endl;

  
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

  /*
  // adapted AliPhysics implementation:

  if(pdg1 < 0) {
    temp = pv1;
    pv1 = pv2;
    pv2 = temp;
  }
  
  /// Following the idea to use opening of collinear pairs in magnetic field from e.g. PHENIX
  /// to identify conversions. Angle between ee plane and magnetic field is calculated (0 to pi).
  /// Due to tracking to the primary vertex, conversions with no intrinsic opening angle 
  /// always end up as pair in "cowboy" configuration. The function as defined here then 
  /// returns values close to pi.
  /// Correlated Like Sign pairs (from double conversion / dalitz + conversion) may show up 
  /// at pi or at 0 depending on which leg has the higher momentum. (not checked yet)
  /// This expected ambiguity is not seen due to sorting of track arrays in this framework. 
  /// To reach the same result as for ULS (~pi), the legs are flipped for LS.

  // // Adapt variables for AliPhysics:
  Double_t px1 = pv1.x();
  Double_t px2 = pv2.x();
  Double_t py1 = pv1.y();
  Double_t py2 = pv2.y();
  Double_t pz1 = pv1.z();
  Double_t pz2 = pv2.z();
  
  Double_t px = px1+px2;
  Double_t py = py1+py2;
  Double_t pz = pz1+pz2;
  Double_t dppair = TMath::Sqrt(px*px+py*py+pz*pz);

    //unit vector of (pep+pem) 
  Double_t pl = dppair;
  Double_t ux = px/pl;
  Double_t uy = py/pl;
  Double_t uz = pz/pl;
  Double_t ax = uy/TMath::Sqrt(ux*ux+uy*uy);
  Double_t ay = -ux/TMath::Sqrt(ux*ux+uy*uy); 
  
  //momentum of e+ and e- in (ax,ay,az) axis. Note that az=0 by definition.
  //Double_t ptep = iep->Px()*ax + iep->Py()*ay; 
  //Double_t ptem = iem->Px()*ax + iem->Py()*ay; 
  
  Double_t pxep = px1;
  Double_t pyep = py1;
  Double_t pzep = pz1;
  Double_t pxem = px2;
  Double_t pyem = py2;
  Double_t pzem = pz2;
  
  //vector product of pep X pem 
  Double_t vpx = pyep*pzem - pzep*pyem; 
  Double_t vpy = pzep*pxem - pxep*pzem; 
  Double_t vpz = pxep*pyem - pyep*pxem; 
  Double_t vp = sqrt(vpx*vpx+vpy*vpy+vpz*vpz); 
  //Double_t thev = acos(vpz/vp); 
  
  //unit vector of pep X pem 
  Double_t vx = vpx/vp; 
  Double_t vy = vpy/vp; 
  Double_t vz = vpz/vp; 

  //The third axis defined by vector product (ux,uy,uz)X(vx,vy,vz) 
  Double_t wx = uy*vz - uz*vy; 
  Double_t wy = uz*vx - ux*vz; 
  //Double_t wz = ux*vy - uy*vx; 
  //Double_t wl = sqrt(wx*wx+wy*wy+wz*wz); 
  // by construction, (wx,wy,wz) must be a unit vector. 
  // measure angle between (wx,wy,wz) and (ax,ay,0). The angle between them 
  // should be small if the pair is conversion 
  // this function then returns values close to pi!
  Double_t cosPhiV = wx*ax + wy*ay; 
  phiv = TMath::ACos(cosPhiV);
  */
  
  
  
  
  // phiv implementation following the PHENIX approach
  // (PHYSICAL REVIEW C 81, 034911 (2010)):

  // if(pdg1 < 0) {
  //   temp = pv1;
  //   pv1 = pv2;
  //   pv2 = temp;
  // }
  
  u = pv1 + pv2;
  u = u.Unit();

  v = pv1.Cross(pv2);
  if(pdg1 < 0) v = -v; // equivalent to exchanging pv1 and pv2

  w = u.Cross(v);

  ua = u.Cross(z);
  ua = ua.Unit();

  phiv = w.Angle(ua);
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
  IsCorrHF = 0;              // }
  IsCorrCharm = 0;           // }
  IsCorrBottom = 0;          // } default values
  IsCorrCharmFromBottom = 0; // }
  IsCombHF = 0;              // }
  
  // check heavy flavor of first mothers:
  if((isCharm(firstMotherPdg1)||isBottom(firstMotherPdg1))
     && (isCharm(firstMotherPdg2)||isBottom(firstMotherPdg2))) {

    // // NB: IGNORE LABEL CHECKS FOR IGEO19
    
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
      
    }else { // <-> mothers have a different origin
      
      // check only heavy flavor of mothers:
      if((isCharm(motherPdg1)||isBottom(motherPdg1)) && (isCharm(motherPdg2)||isBottom(motherPdg2))) {
	IsCombHF = 1;
      }
      
    }
    
  }
  
  if(IsCorrCharm==1 || IsCorrBottom==1) {
    IsCorrHF = 1;
  }
  
}


Float_t getPIDefficiency(TH1D &h_effs, Float_t pT) {
  return pT>=h_effs.GetBinLowEdge(h_effs.GetNbinsX()) ?
    h_effs.GetBinContent(h_effs.GetNbinsX()) : h_effs.GetBinContent(h_effs.FindBin(pT));
}
