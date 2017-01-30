#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <bitset>
#include <vector>

#include <stdio.h>
#include <iomanip>
#include <stdlib.h>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TNtuple.h>
#include <TMath.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include "TGraph.h"
#include <TLegend.h>
#include <TAttLine.h>
#include "TVector.h"

gSystem.Load("libPhysics.so");


using namespace std;


void getmomenta();
void getphiv();
void getopen();
void getdiffz();
void getmass();
void getsumz();
void getpdgs();
void getcharges();

TVector3 tempv;  
TVector3 diff;    
TVector3 sum; 
TVector3 u;             //used for phiv calculation
TVector3 v;
TVector3 w;
TVector3 z;
TVector3 ua;


// ST (=single track) variables

int eventwpair=0;
int tempevwpair=-1;

int tracc1=0;
int tracc2=0;
int tracc3=0;

int evpairs=0;
TH1F *fQAHist = new TH1F("h1", "h1 title", 100, 0.0, -10);

Int_t firsttr;
Int_t tempev;       //used to detect new event
Int_t tracks;       //used to know how many tracks in given event
Int_t trackstemp;   //used to hold number of tracks in previous event (in event mixing)        

Int_t pdg1;          //to store variables of first leg
Int_t ev1;
Int_t trid1; 
Int_t mpdg1;
Int_t charge1;
Int_t mlbl1;
Double_t eta1;
Double_t phi1;
Double_t pt1;
Double_t mvaBDT1;
Bool_t mvaCut1[11];
Double_t dcaxy1;
Double_t dcaz1;
Double_t chi2g1;
Double_t chi2ITS1;
Double_t chi2TPC1;
Double_t nTPC1;
Double_t nTOF1;
Int_t nITS1;
Double_t nITSshare1;
Int_t HasM1;
Double_t NCrossedRowsTPC1;
Int_t NClustersTPC1;
Bool_t HasSPDfirstHit1; 
Double_t RatioCrossedRowsFindableClusters1;  
Int_t NTPCSignal1; 


Int_t pdg2;         //to store variables of second leg
Int_t ev2;
Int_t trid2; 
Int_t mpdg2;
Int_t charge2;
Int_t mlbl2;
Double_t eta2;
Double_t phi2;
Double_t pt2;
Double_t mvaBDT2;
Bool_t mvaCut2[11];
Double_t dcaxy2;
Double_t dcaz2;
Double_t chi2g2;
Double_t chi2ITS2;
Double_t chi2TPC2;
Double_t nTPC2;
Double_t nTOF2;
Int_t nITS2;
Double_t nITSshare2;
Int_t HasM2;
Double_t NCrossedRowsTPC2;
Int_t NClustersTPC2;
Bool_t HasSPDfirstHit2; 
Double_t RatioCrossedRowsFindableClusters2;  
Int_t NTPCSignal2; 
// cartesian momentum 3-vector of ST

Double_t mom][6];       //px1,py1,pz1,px2,py2,pz3

// P (=pair) variables

Int_t IsRP;         //realpairs: 1; combis: 0
Int_t IsUS;         //pair with unlike sign (no matter if combi or rp)
Int_t IsConv;       // both mothers == 22 - can still be combi
Int_t ChSig;         // Charge Sign: 0 for US, 1 for LS++,  -1
Bool_t PassAlbCut_leg1;     // IF alberto track cuts were passed
Bool_t PassAlbCut_leg2;
Double_t opang;
Double_t diffz;
Double_t mass;
Double_t sumz;
Int_t trid_leg1;
Int_t trid_leg2;
Int_t evid_leg1;
Int_t evid_leg2;
Int_t mpdg_leg1;
Int_t mpdg_leg2;
Int_t pdg_leg1;
Int_t pdg_leg2;
Int_t charge_leg1;
Int_t charge_leg2;
Double_t Pmva1;
Bool_t PmvaCut1[11];
Double_t Pmva2;
Bool_t PmvaCut2[11];
Double_t phiv;
Double_t px1;
Double_t py1;
Double_t pz1;
Double_t px2;
Double_t py2;
Double_t pz2;
Double_t dcaxy_leg1;
Double_t dcaz_leg1;
Double_t chi2g_leg1;
Int_t nITS_leg1;
Double_t nITSshare_leg1;
Double_t dcaxy_leg2;
Double_t dcaz_leg2;
Double_t chi2g_leg2;
Int_t nITS_leg2;
Double_t nITSshare_leg2;
Double_t eta1;
Double_t phi1;
Double_t eta2;
Double_t phi2;
Double_t nTPC_leg1;
Double_t nTPC_leg2;
Double_t nTOF_leg1;
Double_t nTOF_leg2;
Double_t chi2ITS_leg1;
Double_t chi2ITS_leg2;
Double_t chi2TPC_leg1;
Double_t chi2TPC_leg2;

Double_t NCrossedRowsTPC_leg1;
Int_t NClustersTPC_leg1;
Bool_t HasSPDfirstHit_leg1; 
Double_t RatioCrossedRowsFindableClusters_leg1;  
Int_t NTPCSignal_leg1;

Double_t NCrossedRowsTPC_leg2;
Int_t NClustersTPC_leg2;
Bool_t HasSPDfirstHit_leg2; 
Double_t RatioCrossedRowsFindableClusters_leg2;  
Int_t NTPCSignal_leg2; 

Double_t eta_temp;
Double_t phi_temp;
Double_t pt_temp;

Double_t px_temp;
Double_t py_temp;
Double_t pz_temp;

Double_t mpdg_leg_temp;
Double_t pdg_leg_temp;

Int_t IsEnh = 0;


TVector3 pv1;
TVector3 pv2;
 
void getpairtrees_mc(){             

  cout<<"EventID dependent charge ordering in phiv calculation!!! watch out!!!!!"<<endl;
    
  z.SetXYZ(0.,0.,1.);     
    
  TFile *infile= TFile::Open("./FT2_AnalysisResults_Upgrade.root","READ");

  TTree *singleTree = (TTree*)infile->Get("outputITSup/tracks");

  TFile *outfile= TFile::Open("./pairtrees.root","RECREATE");
  TTree *pairTree_rp = new TTree("Pair_tree_rp","Pair_tree_rp");
  TTree *pairTree_us = new TTree("Pair_tree_us","Pair_tree_us"); 
  TTree *pairTree_ls = new TTree("Pair_tree_ls","Pair_tree_ls"); 
  TTree *pairTree_usnls = new TTree("Pair_tree_us_n_ls","Pair_tree_us_n_ls"); 

  // ST Branches

  TBranch *btrx = 0;       
  TBranch *btrid = 0;   
  TBranch *bpdg1 = 0;
  TBranch *bch1 = 0;
  TBranch *bev1 = 0;
  TBranch *bmpdg1 = 0;
  TBranch *bmlbl1 = 0;
  TBranch *beta1 = 0;
  TBranch *bphi1 = 0;
  TBranch *bpt1 = 0;
  TBranch *bdcaxy1=0;
  TBranch *bdcaz1=0;
  TBranch *bchi2g1=0;
  TBranch *bnITS1=0;
  TBranch *bnITSshare1=0;
  TBranch *bhasm1=0;
  TBranch *bnTPC1=0;
  TBranch *bNCrossedRowsTPC1=0;
  TBranch *bNClustersTPC1=0;
  TBranch *bHasSPDfirstHit1=0;
  TBranch *bRatioCrossedRowsFindableClusters1=0;
  TBranch *bNTPCSignal1=0;
  TBranch *bchi2TPC1=0;
  TBranch *bchi2ITS1=0;
  TBranch *bnTOF1=0;


  singleTree->SetBranchAddress("#tracks",&tracks,&btrx);
  singleTree->SetBranchAddress("TrackID",&trid1,&btrid);
  singleTree->SetBranchAddress("Pdg",&pdg1,&bpdg1);
  //singleTree->SetBranchAddress("charge",&charge1,&bch1);
  singleTree->SetBranchAddress("EventID",&ev1,&bev1);
  singleTree->SetBranchAddress("Pdg_Mother",&mpdg1,&bmpdg1);
  singleTree->SetBranchAddress("Mother_label",&mlbl1,&bmlbl1);
  singleTree->SetBranchAddress("eta",&eta1,&beta1);
  singleTree->SetBranchAddress("phi",&phi1,&bphi1);
  singleTree->SetBranchAddress("pt",&pt1,&bpt1);
  singleTree->SetBranchAddress("DCAxy",&dcaxy1,&bdcaxy1 );
  singleTree->SetBranchAddress("DCAz",&dcaz1,&bdcaz1 );
  singleTree->SetBranchAddress("chi2GlobalvsTPC",&chi2g1,&bchi2g1);
  singleTree->SetBranchAddress("chi2TPC",&chi2TPC1,&bchi2TPC1);
  singleTree->SetBranchAddress("chi2ITS",&chi2ITS1,&bchi2ITS1);
  singleTree->SetBranchAddress("nITS",&nITS1,&bnITS1);
  singleTree->SetBranchAddress("nITSshared_frac",&nITSshare1,&bnITSshare1);
  singleTree->SetBranchAddress("Has_Mother",&HasM1,&bhasm1);
  singleTree->SetBranchAddress("EsigTPC",&nTPC1,&bnTPC1);
  singleTree->SetBranchAddress("EsigTOF",&nTOF1,&bnTOF1);
  singleTree->SetBranchAddress("NCrossedRowsTPC", &NCrossedRowsTPC1, &bNCrossedRowsTPC1);
  singleTree->SetBranchAddress("NClustersTPC", &NClustersTPC1, &bNClustersTPC1);
  singleTree->SetBranchAddress("HasSPDfirstHit", &HasSPDfirstHit1, &bHasSPDfirstHit1);        
  singleTree->SetBranchAddress("RatioCrossedRowsFindableClusters",&RatioCrossedRowsFindableClusters1, &bRatioCrossedRowsFindableClusters1);
  singleTree->SetBranchAddress("NTPCSignal",&NTPCSignal1, &bNTPCSignal1);
  //TBranch *bdcaxy1=0;
  //TBranch *bdcaz1=0;
  //TBranch *bchi2g1=0;
  //TBranch *bnITS1=0;
 
  // Pair Branches RP

  pairTree_rp->Branch("TrackID1",&trid_leg1);
  pairTree_rp->Branch("TrackID2",&trid_leg2);
  pairTree_rp->Branch("EventID1",&evid_leg1);
  pairTree_rp->Branch("EventID2",&evid_leg2);
  pairTree_rp->Branch("PassAlbertoCuts1",&PassAlbCut_leg1);
  pairTree_rp->Branch("PassAlbertoCuts2",&PassAlbCut_leg2);
  pairTree_rp->Branch("IsUS",&IsUS);
  pairTree_rp->Branch("IsRP",&IsRP);
  pairTree_rp->Branch("IsConv",&IsConv);
  pairTree_rp->Branch("ChargeSign",&ChSig);
  pairTree_rp->Branch("opang",&opang);
  pairTree_rp->Branch("diffz",&diffz);
  pairTree_rp->Branch("mass",&mass);
  pairTree_rp->Branch("sumz",&sumz);
  pairTree_rp->Branch("phiv",&phiv);
  pairTree_rp->Branch("px1",&px1);
  pairTree_rp->Branch("py1",&py1);
  pairTree_rp->Branch("pz1",&pz1);
  pairTree_rp->Branch("px2",&px2);
  pairTree_rp->Branch("py2",&py2);
  pairTree_rp->Branch("pz2",&pz2);
  pairTree_rp->Branch("mpdg_leg1",&mpdg_leg1);
  pairTree_rp->Branch("mpdg_leg2",&mpdg_leg2);
  pairTree_rp->Branch("pdg_leg1",&pdg_leg1);
  pairTree_rp->Branch("pdg_leg2",&pdg_leg2);
  //pairTree_rp->Branch("ChargeLeg1",&charge_leg1);
  //pairTree_rp->Branch("ChargeLeg2",&charge_leg2);
  pairTree_rp->Branch("Cut10_2",&PmvaCut2[10]);
  pairTree_rp->Branch("DCAxy1",&dcaxy_leg1);
  pairTree_rp->Branch("DCAxy2",&dcaxy_leg2);
  pairTree_rp->Branch("DCAz1",&dcaz_leg1);
  pairTree_rp->Branch("DCAz2",&dcaz_leg2);
  pairTree_rp->Branch("nITS1",&nITS_leg1);
  pairTree_rp->Branch("nITS2",&nITS_leg2);
  pairTree_rp->Branch("nITSshared_frac1",&nITSshare_leg1);
  pairTree_rp->Branch("nITSshared_frac2",&nITSshare_leg2);
  pairTree_rp->Branch("chi2g1",&chi2g_leg1);
  pairTree_rp->Branch("chi2g2",&chi2g_leg2);
  pairTree_rp->Branch("chi2ITS1",&chi2ITS_leg1);
  pairTree_rp->Branch("chi2ITS2",&chi2ITS_leg2);
  pairTree_rp->Branch("chi2TPC1",&chi2TPC_leg1);
  pairTree_rp->Branch("chi2TPC2",&chi2TPC_leg2);
  pairTree_rp->Branch("phi1",&phi1);
  pairTree_rp->Branch("phi2",&phi2);
  pairTree_rp->Branch("eta1",&eta1);
  pairTree_rp->Branch("eta2",&eta2);
  pairTree_rp->Branch("pt1",&pt1);
  pairTree_rp->Branch("pt2",&pt2);
  pairTree_rp->Branch("EsigTPC1",&nTPC_leg1);
  pairTree_rp->Branch("EsigTPC2",&nTPC_leg2);
  pairTree_rp->Branch("EsigTOF1",&nTOF_leg1);
  pairTree_rp->Branch("EsigTOF2",&nTOF_leg2);
  pairTree_rp->Branch("NCrossedRowsTPC_leg1", &NCrossedRowsTPC_leg1);
  pairTree_rp->Branch("NClustersTPC_leg1", &NClustersTPC_leg1);
  pairTree_rp->Branch("HasSPDfirstHit_leg1", &HasSPDfirstHit_leg1);        
  pairTree_rp->Branch("RatioCrossedRowsFindableClusters_leg1",&RatioCrossedRowsFindableClusters_leg1);
  pairTree_rp->Branch("NTPCSignal_leg1",&NTPCSignal_leg1);
  pairTree_rp->Branch("NCrossedRowsTPC_leg2", &NCrossedRowsTPC_leg2);
  pairTree_rp->Branch("NClustersTPC_leg2", &NClustersTPC_leg2);
  pairTree_rp->Branch("HasSPDfirstHit_leg2", &HasSPDfirstHit_leg2);        
  pairTree_rp->Branch("RatioCrossedRowsFindableClusters_leg2",&RatioCrossedRowsFindableClusters_leg2);
  pairTree_rp->Branch("NTPCSignal_leg2",&NTPCSignal_leg2);
  pairTree_rp->Branch("IsEnh",&IsEnh);
  // Pair Branches US

  pairTree_us->Branch("TrackID1",&trid_leg1);
  pairTree_us->Branch("TrackID2",&trid_leg2);
  pairTree_us->Branch("EventID1",&evid_leg1);
  pairTree_us->Branch("EventID2",&evid_leg2);
  pairTree_us->Branch("PassAlbertoCuts1",&PassAlbCut_leg1);
  pairTree_us->Branch("PassAlbertoCuts2",&PassAlbCut_leg2);
  pairTree_us->Branch("IsRP",&IsRP);
  pairTree_us->Branch("IsUS",&IsUS);
  pairTree_us->Branch("IsConv",&IsConv);
  pairTree_us->Branch("ChargeSign",&ChSig);
  pairTree_us->Branch("opang",&opang);
  pairTree_us->Branch("diffz",&diffz);
  pairTree_us->Branch("mass",&mass);
  pairTree_us->Branch("sumz",&sumz);
  pairTree_us->Branch("phiv",&phiv);
  pairTree_us->Branch("px1",&px1);
  pairTree_us->Branch("py1",&py1);
  pairTree_us->Branch("pz1",&pz1);
  pairTree_us->Branch("px2",&px2);
  pairTree_us->Branch("py2",&py2);
  pairTree_us->Branch("pz2",&pz2);
  pairTree_us->Branch("mpdg_leg1",&mpdg_leg1);
  pairTree_us->Branch("mpdg_leg2",&mpdg_leg2);
  pairTree_us->Branch("pdg_leg1",&pdg_leg1);
  pairTree_us->Branch("pdg_leg2",&pdg_leg2);
  //pairTree_us->Branch("ChargeLeg1",&charge_leg1);
  //pairTree_us->Branch("ChargeLeg2",&charge_leg2);
  pairTree_us->Branch("DCAxy1",&dcaxy_leg1);
  pairTree_us->Branch("DCAxy2",&dcaxy_leg2);
  pairTree_us->Branch("DCAz1",&dcaz_leg1);
  pairTree_us->Branch("DCAz2",&dcaz_leg2);
  pairTree_us->Branch("nITSshared_frac1",&nITSshare_leg1);
  pairTree_us->Branch("nITSshared_frac2",&nITSshare_leg2);
  pairTree_us->Branch("nITS1",&nITS_leg1);
  pairTree_us->Branch("nITS2",&nITS_leg2);
  pairTree_us->Branch("chi2g1",&chi2g_leg1);
  pairTree_us->Branch("chi2g2",&chi2g_leg2);
  pairTree_us->Branch("chi2ITS1",&chi2ITS_leg1);
  pairTree_us->Branch("chi2ITS2",&chi2ITS_leg2);
  pairTree_us->Branch("chi2TPC1",&chi2TPC_leg1);
  pairTree_us->Branch("chi2TPC2",&chi2TPC_leg2);
  pairTree_us->Branch("phi1",&phi1);
  pairTree_us->Branch("phi2",&phi2);
  pairTree_us->Branch("eta1",&eta1);
  pairTree_us->Branch("eta2",&eta2);
  pairTree_us->Branch("pt1",&pt1);
  pairTree_us->Branch("pt2",&pt2);
  pairTree_us->Branch("EsigTPC1",&nTPC_leg1);
  pairTree_us->Branch("EsigTPC2",&nTPC_leg2);
  pairTree_us->Branch("EsigTOF1",&nTOF_leg1);
  pairTree_us->Branch("EsigTOF2",&nTOF_leg2);
  pairTree_us->Branch("NCrossedRowsTPC_leg1", &NCrossedRowsTPC_leg1);
  pairTree_us->Branch("NClustersTPC_leg1", &NClustersTPC_leg1);
  pairTree_us->Branch("HasSPDfirstHit_leg1", &HasSPDfirstHit_leg1);        
  pairTree_us->Branch("RatioCrossedRowsFindableClusters_leg1",&RatioCrossedRowsFindableClusters_leg1);
  pairTree_us->Branch("NTPCSignal_leg1",&NTPCSignal_leg1);
  pairTree_us->Branch("NCrossedRowsTPC_leg2", &NCrossedRowsTPC_leg2);
  pairTree_us->Branch("NClustersTPC_leg2", &NClustersTPC_leg2);
  pairTree_us->Branch("HasSPDfirstHit_leg2", &HasSPDfirstHit_leg2);        
  pairTree_us->Branch("RatioCrossedRowsFindableClusters_leg2",&RatioCrossedRowsFindableClusters_leg2);
  pairTree_us->Branch("NTPCSignal_leg2",&NTPCSignal_leg2);
  pairTree_us->Branch("IsEnh",&IsEnh);
  // Pair Branches LS

  pairTree_ls->Branch("TrackID1",&trid_leg1);
  pairTree_ls->Branch("TrackID2",&trid_leg2);
  pairTree_ls->Branch("EventID1",&evid_leg1);
  pairTree_ls->Branch("EventID2",&evid_leg2);
  pairTree_ls->Branch("PassAlbertoCuts1",&PassAlbCut_leg1);
  pairTree_ls->Branch("PassAlbertoCuts2",&PassAlbCut_leg2);
  pairTree_ls->Branch("IsRP",&IsRP);
  pairTree_ls->Branch("IsUS",&IsUS);
  pairTree_ls->Branch("IsConv",&IsConv);
  pairTree_ls->Branch("ChargeSign",&ChSig);
  pairTree_ls->Branch("opang",&opang);
  pairTree_ls->Branch("diffz",&diffz);
  pairTree_ls->Branch("mass",&mass);
  pairTree_ls->Branch("sumz",&sumz);
  pairTree_ls->Branch("phiv",&phiv);
  pairTree_ls->Branch("px1",&px1);
  pairTree_ls->Branch("py1",&py1);
  pairTree_ls->Branch("pz1",&pz1);
  pairTree_ls->Branch("px2",&px2);
  pairTree_ls->Branch("py2",&py2);
  pairTree_ls->Branch("pz2",&pz2);
  pairTree_ls->Branch("mpdg_leg1",&mpdg_leg1);
  pairTree_ls->Branch("mpdg_leg2",&mpdg_leg2);
  pairTree_ls->Branch("pdg_leg1",&pdg_leg1);
  pairTree_ls->Branch("pdg_leg2",&pdg_leg2);
  //pairTree_ls->Branch("ChargeLeg1",&charge_leg1);
  //pairTree_ls->Branch("ChargeLeg2",&charge_leg2);
  pairTree_ls->Branch("DCAxy1",&dcaxy_leg1);
  pairTree_ls->Branch("DCAxy2",&dcaxy_leg2);
  pairTree_ls->Branch("DCAz1",&dcaz_leg1);
  pairTree_ls->Branch("DCAz2",&dcaz_leg2);
  pairTree_ls->Branch("nITS1",&nITS_leg1);
  pairTree_ls->Branch("nITS2",&nITS_leg2);
  pairTree_ls->Branch("nITSshared_frac1",&nITSshare_leg1);
  pairTree_ls->Branch("nITSshared_frac2",&nITSshare_leg2);
  pairTree_ls->Branch("chi2g1",&chi2g_leg1);
  pairTree_ls->Branch("chi2g2",&chi2g_leg2);
  pairTree_ls->Branch("chi2ITS1",&chi2ITS_leg1);
  pairTree_ls->Branch("chi2ITS2",&chi2ITS_leg2);
  pairTree_ls->Branch("chi2TPC1",&chi2TPC_leg1);
  pairTree_ls->Branch("chi2TPC2",&chi2TPC_leg2);
  pairTree_ls->Branch("phi1",&phi1);
  pairTree_ls->Branch("phi2",&phi2);
  pairTree_ls->Branch("eta1",&eta1);
  pairTree_ls->Branch("eta2",&eta2);
  pairTree_ls->Branch("pt1",&pt1);
  pairTree_ls->Branch("pt2",&pt2);
  pairTree_ls->Branch("EsigTPC1",&nTPC_leg1);
  pairTree_ls->Branch("EsigTPC2",&nTPC_leg2);
  pairTree_ls->Branch("EsigTOF1",&nTOF_leg1);
  pairTree_ls->Branch("EsigTOF2",&nTOF_leg2);
  pairTree_ls->Branch("NCrossedRowsTPC_leg1", &NCrossedRowsTPC_leg1);
  pairTree_ls->Branch("NClustersTPC_leg1", &NClustersTPC_leg1);
  pairTree_ls->Branch("HasSPDfirstHit_leg1", &HasSPDfirstHit_leg1);        
  pairTree_ls->Branch("RatioCrossedRowsFindableClusters_leg1",&RatioCrossedRowsFindableClusters_leg1);
  pairTree_ls->Branch("NTPCSignal_leg1",&NTPCSignal_leg1);
  pairTree_ls->Branch("NCrossedRowsTPC_leg2", &NCrossedRowsTPC_leg2);
  pairTree_ls->Branch("NClustersTPC_leg2", &NClustersTPC_leg2);
  pairTree_ls->Branch("HasSPDfirstHit_leg2", &HasSPDfirstHit_leg2);        
  pairTree_ls->Branch("RatioCrossedRowsFindableClusters_leg2",&RatioCrossedRowsFindableClusters_leg2);
  pairTree_ls->Branch("NTPCSignal_leg2",&NTPCSignal_leg2);
  pairTree_ls->Branch("IsEnh",&IsEnh);

  //Pair Branches USnLS
  pairTree_usnls->Branch("TrackID1",&trid_leg1);
  pairTree_usnls->Branch("TrackID2",&trid_leg2);
  pairTree_usnls->Branch("EventID1",&evid_leg1);
  pairTree_usnls->Branch("EventID2",&evid_leg2);
  pairTree_usnls->Branch("PassAlbertoCuts1",&PassAlbCut_leg1);
  pairTree_usnls->Branch("PassAlbertoCuts2",&PassAlbCut_leg2);
  pairTree_usnls->Branch("IsRP",&IsRP);
  pairTree_usnls->Branch("IsUS",&IsUS);
  pairTree_usnls->Branch("IsConv",&IsConv);
  pairTree_usnls->Branch("ChargeSign",&ChSig);
  pairTree_usnls->Branch("opang",&opang);
  pairTree_usnls->Branch("diffz",&diffz);
  pairTree_usnls->Branch("mass",&mass);
  pairTree_usnls->Branch("sumz",&sumz);
  pairTree_usnls->Branch("phiv",&phiv);
  pairTree_usnls->Branch("px1",&px1);
  pairTree_usnls->Branch("py1",&py1);
  pairTree_usnls->Branch("pz1",&pz1);
  pairTree_usnls->Branch("px2",&px2);
  pairTree_usnls->Branch("py2",&py2);
  pairTree_usnls->Branch("pz2",&pz2);
  pairTree_usnls->Branch("mpdg_leg1",&mpdg_leg1);
  pairTree_usnls->Branch("mpdg_leg2",&mpdg_leg2);
  pairTree_usnls->Branch("pdg_leg1",&pdg_leg1);
  pairTree_usnls->Branch("pdg_leg2",&pdg_leg2);
  //pairTree_usnls->Branch("ChargeLeg1",&charge_leg1);
  //pairTree_usnls->Branch("ChargeLeg2",&charge_leg2);
  pairTree_usnls->Branch("DCAxy1",&dcaxy_leg1);
  pairTree_usnls->Branch("DCAxy2",&dcaxy_leg2);
  pairTree_usnls->Branch("DCAz1",&dcaz_leg1);
  pairTree_usnls->Branch("DCAz2",&dcaz_leg2);
  pairTree_usnls->Branch("nITS1",&nITS_leg1);
  pairTree_usnls->Branch("nITS2",&nITS_leg2);
  pairTree_usnls->Branch("nITSshared_frac1",&nITSshare_leg1);
  pairTree_usnls->Branch("nITSshared_frac2",&nITSshare_leg2);
  pairTree_usnls->Branch("chi2g1",&chi2g_leg1);
  pairTree_usnls->Branch("chi2g2",&chi2g_leg2);
  pairTree_usnls->Branch("chi2ITS1",&chi2ITS_leg1);
  pairTree_usnls->Branch("chi2ITS2",&chi2ITS_leg2);
  pairTree_usnls->Branch("chi2TPC1",&chi2TPC_leg1);
  pairTree_usnls->Branch("chi2TPC2",&chi2TPC_leg2);
  pairTree_usnls->Branch("phi1",&phi1);
  pairTree_usnls->Branch("phi2",&phi2);
  pairTree_usnls->Branch("eta1",&eta1);
  pairTree_usnls->Branch("eta2",&eta2);
  pairTree_usnls->Branch("pt1",&pt1);
  pairTree_usnls->Branch("pt2",&pt2);
  pairTree_usnls->Branch("EsigTPC1",&nTPC_leg1);
  pairTree_usnls->Branch("EsigTPC2",&nTPC_leg2);
  pairTree_usnls->Branch("EsigTOF1",&nTOF_leg1);
  pairTree_usnls->Branch("EsigTOF2",&nTOF_leg2);
  pairTree_usnls->Branch("NCrossedRowsTPC_leg1", &NCrossedRowsTPC_leg1);
  pairTree_usnls->Branch("NClustersTPC_leg1", &NClustersTPC_leg1);
  pairTree_usnls->Branch("HasSPDfirstHit_leg1", &HasSPDfirstHit_leg1);        
  pairTree_usnls->Branch("RatioCrossedRowsFindableClusters_leg1",&RatioCrossedRowsFindableClusters_leg1);
  pairTree_usnls->Branch("NTPCSignal_leg1",&NTPCSignal_leg1);
  pairTree_usnls->Branch("NCrossedRowsTPC_leg2", &NCrossedRowsTPC_leg2);
  pairTree_usnls->Branch("NClustersTPC_leg2", &NClustersTPC_leg2);
  pairTree_usnls->Branch("HasSPDfirstHit_leg2", &HasSPDfirstHit_leg2);        
  pairTree_usnls->Branch("RatioCrossedRowsFindableClusters_leg2",&RatioCrossedRowsFindableClusters_leg2);
  pairTree_usnls->Branch("NTPCSignal_leg2",&NTPCSignal_leg2);
  pairTree_usnls->Branch("IsEnh",&IsEnh);
  tempev=-1; 

  Int_t tr1;        
  Int_t tr2;
  TRandom3 r; // generate a number in interval ]0,1] (0 is excluded)


  Int_t singleTree_nEntries = singleTree->GetEntries();
  for (tr1=0; tr1<singleTree_nEntries; tr1++) {        //1st Track loop

    if (!(tr1%100000))cout<<"current Event: "<<tr1<<"  of  "<<singleTree->GetEntries()<<endl;
  
    singleTree->GetEntry(tr1);

    // if(tracks<2 || tracks>100) continue;

    if(tempev!=ev1) {
      
      tempev=ev1;       //get new ev number if it changed
      firsttr=tr1;      //get pos of first
    }

    PassAlbCut_leg1=kTRUE;
    PassAlbCut_leg2=kTRUE; 
  
    //pdg cut
    if(abs(pdg1)!=11) {tracc1++; continue;} 
    //kin
    if(pt1<0.4 || pt1>5) {tracc2++; continue;}
    //PID
    if(nTPC1<-3*exp(-pt1*cosh(eta1)) || chi2ITS1/nITS1>5 || nITSshare1>0.4 || chi2g1>10 ) {tracc3++; continue;}     //p dependent TPC cut 
  
    //  if(nTPC1<-3*exp(-pt1*cosh(eta1))) continue;        //p dependent TPC cut 

    // track cuts
    if(dcaxy1*0.01>0.00515869+0.0101668/pow(pt1,1.34489)) PassAlbCut_leg1 = kFALSE;
    if(dcaz1*0.01>0.1) PassAlbCut_leg1 = kFALSE;  
    if(nITS1<4) PassAlbCut_leg1 = kFALSE;
    if(NCrossedRowsTPC1<100) PassAlbCut_leg1 = kFALSE;
    if(NClustersTPC1<70) PassAlbCut_leg1 = kFALSE;
    if(!HasSPDfirstHit1) PassAlbCut_leg1 = kFALSE;
    if(NTPCSignal1<50) PassAlbCut_leg1 = kFALSE;
    if(RatioCrossedRowsFindableClusters1<0.6) PassAlbCut_leg1 = kFALSE;
    if(chi2TPC1/NClustersTPC1>4) PassAlbCut_leg1 = kFALSE;

    PassAlbCut_leg2 = PassAlbCut_leg1;
    pdg2=pdg1;
    ev2=ev1;
    mpdg2=mpdg1;
    mlbl2=mlbl1;
    eta2=eta1;
    phi2=phi1;
    pt2=pt1;
    trid2=trid1; 
    dcaxy2=dcaxy1;
    dcaz2=dcaz1;
    chi2g2=chi2g1;
    nITS2=nITS1;
    nITSshare2=nITSshare1;
    HasM2=HasM1;
    nTPC2=nTPC1;
    nTOF2=nTOF1;
    NCrossedRowsTPC2=NCrossedRowsTPC1;
    NClustersTPC2=NClustersTPC1;
    HasSPDfirstHit2=HasSPDfirstHit1; 
    RatioCrossedRowsFindableClusters2=RatioCrossedRowsFindableClusters1;  
    NTPCSignal2=NTPCSignal1; 
    chi2TPC2=chi2TPC1;
    chi2ITS2=chi2ITS1; 
    //  charge2=charge1;
    //    cout<<tr1+1<<" "<<firsttr+tracks<<endl;
    for (tr2=tr1+1; tr2<firsttr+tracks; tr2++) {        //same event loop  
      singleTree->GetEntry(tr2);

      //pdg
      if(abs(pdg1)!=11) continue;
      //kin
      if(pt1<0.4 || pt1>5) continue; 
      //PID
      if(nTPC1<-3*exp(-pt1*cosh(eta1)) || chi2ITS1/nITS1>5 || nITSshare1>0.4 || chi2g1>10 ) continue;        //p dependent TPC cut 
        
      //  if(nTPC1<-3*exp(-pt1*cosh(eta1))) continue;        //p dependent TPC cut 
      
      // track cuts
      if(dcaxy1*0.01>0.00515869+0.0101668/pow(pt1,1.34489)) PassAlbCut_leg1 = kFALSE;
      if(dcaz1*0.01>0.1) PassAlbCut_leg1 = kFALSE;  
      if(nITS1<4) PassAlbCut_leg1 = kFALSE;
      if(NCrossedRowsTPC1<100) PassAlbCut_leg1 = kFALSE;
      if(NClustersTPC1<70) PassAlbCut_leg1 = kFALSE;
      if(!HasSPDfirstHit1) PassAlbCut_leg1 = kFALSE;
      if(NTPCSignal1<50) PassAlbCut_leg1 = kFALSE;
      if(RatioCrossedRowsFindableClusters1<0.6) PassAlbCut_leg1 = kFALSE;
      if(chi2TPC1/NClustersTPC1>4) PassAlbCut_leg1 = kFALSE;
  
  
      //        if (ev1==ev2) cout<<tracks<<" "<<ev1<<" "<<ev2<<endl;
        
      if (ev1!=ev2) cout<<"event error!! not same event as first track"<<ev1<<" "<<ev2<<endl;
        
      if(tempevwpair != ev1) {
	eventwpair++;
	//            cout<<evpairs<<endl;
	fQAHist->Fill(evpairs);
	evpairs=1;      //start counting with 1 because first pair has to count too
	tempevwpair = ev1;
      }
      else evpairs++;
        
        
      IsRP=0;
         
        
      if (pdg1 * pdg2 < 0 ){               //US via MC info - has to be changed!!!
	//                        if (charge1 * charge2 < 0 ){
	IsUS = 1; 
	ChSig=0;
                            
	getmomenta();
	getmass();
	getphiv();
	getopang();
	getdiffz();
	getsumz();
	getpdgs();
	//                            getcharges();
                            
	//                        if(r.Rndm()>0.5){
	if(1){
	  //                            cout<<"not swap"<<endl;
	  trid_leg1=trid1;
	  trid_leg2=trid2;
	  evid_leg1=ev1;
	  evid_leg2=ev2;                           
	  dcaxy_leg1=dcaxy1;
	  dcaz_leg1=dcaz1;
	  chi2g_leg1=chi2g1;
	  nITS_leg1=nITS1;
	  nITSshare_leg1=nITSshare1;
	  dcaxy_leg2=dcaxy2;
	  dcaz_leg2=dcaz2;
	  chi2g_leg2=chi2g2;
	  chi2TPC_leg1=chi2TPC1;
	  chi2TPC_leg2=chi2TPC2;
	  chi2ITS_leg1=chi2ITS1;
	  chi2ITS_leg2=chi2ITS2;                            
	  nITS_leg2=nITS2;
	  nITSshare_leg2=nITSshare2;
	  nTPC_leg1=nTPC1;
	  nTPC_leg2=nTPC2;
	  nTOF_leg1=nTOF1;
	  nTOF_leg2=nTOF2;
	  NCrossedRowsTPC_leg1=NCrossedRowsTPC1;
	  NClustersTPC_leg1=NClustersTPC1;
	  HasSPDfirstHit_leg1=HasSPDfirstHit1; 
	  RatioCrossedRowsFindableClusters_leg1=RatioCrossedRowsFindableClusters1;  
	  NTPCSignal_leg1=NTPCSignal1; 
	  NCrossedRowsTPC_leg2=NCrossedRowsTPC2;
	  NClustersTPC_leg2=NClustersTPC2;
	  HasSPDfirstHit_leg2=HasSPDfirstHit2; 
	  RatioCrossedRowsFindableClusters_leg2=RatioCrossedRowsFindableClusters2;  
	  NTPCSignal_leg2=NTPCSignal2; 
                       
	}
                            

                            
	if(mpdg1==22 || mpdg2==22 )    IsConv = 1;         //IsConv is every pair that contains a conversion leg 
	else              IsConv = 0;  
                            
	if (mlbl1 == mlbl2 && abs(pdg1)==11 && abs(pdg2)==11 ){           

	  IsRP = 1; 
	  pairTree_rp->Fill();    
	}

	pairTree_us->Fill(); 
	pairTree_usnls->Fill();
                            
       
      }
                    
      else {               //LS via MC info - has to be changed!!!
               
	IsUS = 0;  
                            
	if(pdg1>0 ) ChSig=1;
	else if(pdg1<0) ChSig=-1;
	else if(pdg1 * pdg2 == 0) ChSig=0;
                            
	//                                     if(charge1>0 ) ChSig=1;
	//                            else if(charge1<0) ChSig=-1;
	//                            else if(charge1 * charge2 == 0) ChSig=0;
                            
	getmomenta();
	getmass();
	getphiv();
	getopang();
	getdiffz();
	getsumz();
	getpdgs();
	//                            getcharges();  
                            
	//                         if(r.Rndm()>0.5){
                            
	if(1){
	  //                             cout<<"not swap"<<endl;
	  trid_leg1=trid1;
	  trid_leg2=trid2;
	  evid_leg1=ev1;
	  evid_leg2=ev2;                           
	  dcaxy_leg1=dcaxy1;
	  dcaz_leg1=dcaz1;
	  chi2g_leg1=chi2g1;
	  nITS_leg1=nITS1;
	  nITSshare_leg1=nITSshare1;
	  dcaxy_leg2=dcaxy2;
	  dcaz_leg2=dcaz2;
	  chi2g_leg2=chi2g2;
	  chi2TPC_leg1=chi2TPC1;
	  chi2TPC_leg2=chi2TPC2;
	  chi2ITS_leg1=chi2ITS1;
	  chi2ITS_leg2=chi2ITS2;                            
	  nITS_leg2=nITS2;
	  nITSshare_leg2=nITSshare2;
	  nTPC_leg1=nTPC1;
	  nTPC_leg2=nTPC2;
	  nTOF_leg1=nTOF1;
	  nTOF_leg2=nTOF2;                            
	  NCrossedRowsTPC_leg1=NCrossedRowsTPC1;
	  NClustersTPC_leg1=NClustersTPC1;
	  HasSPDfirstHit_leg1=HasSPDfirstHit1; 
	  RatioCrossedRowsFindableClusters_leg1=RatioCrossedRowsFindableClusters1;  
	  NTPCSignal_leg1=NTPCSignal1; 
	  NCrossedRowsTPC_leg2=NCrossedRowsTPC2;
	  NClustersTPC_leg2=NClustersTPC2;
	  HasSPDfirstHit_leg2=HasSPDfirstHit2; 
	  RatioCrossedRowsFindableClusters_leg2=RatioCrossedRowsFindableClusters2;  
	  NTPCSignal_leg2=NTPCSignal2; 
	}
                            
                     


	if(mpdg1==22 || mpdg2==22 )    IsConv = 1;          //IsConv is every pair that contains a conversion leg
	else              IsConv = 0; 
                            
	if (mlbl1 == mlbl2 && abs(pdg1)==11 && abs(pdg2)==11){               
	  IsRP = 1;
	  pairTree_rp->Fill();    
	}
	pairTree_ls->Fill();    
	pairTree_usnls->Fill(); 


      }
        
                  
    } 
    if(!(tr1%100000)) cout<<"trees are not written!"<<endl;
  } 

  pairTree_rp->Write();  
  pairTree_us->Write(); 
  pairTree_ls->Write(); 
  pairTree_usnls->Write();
         
  cout<<"events with at least one pair: "<<eventwpair<<endl;
  cout<<tracc1<<" "<<tracc2<<" "<<tracc3<<endl;
  fQAHist->Draw("");
}






void getmomenta(){
    
  px1=pt1*TMath::Cos(phi1);
  py1=pt1*TMath::Sin(phi1);
  pz1=pt1*TMath::SinH(eta1);


  px2=pt2*TMath::Cos(phi2);
  py2=pt2*TMath::Sin(phi2);
  pz2=pt2*TMath::SinH(eta2);

  pv1.SetXYZ(pt1*TMath::Cos(phi1),pt1*TMath::Sin(phi1),pt1*TMath::SinH(eta1));
  pv2.SetXYZ(pt2*TMath::Cos(phi2),pt2*TMath::Sin(phi2),pt2*TMath::SinH(eta2));

}


void getphiv(){
  

  if (pdg1<0){
    //if (charge1>0){    
    tempv = pv1;     
    pv1 = pv2;
    pv2 = tempv;}

  //if (ev1<290940){       //Change back if EventID with wrong PhiV
  ////if (charge1>0){    
  //    tempv = pv1;     
  //    pv1 = pv2;
  //    pv2 = tempv;}
  //  


  u = pv1 + pv2;
  u = u/u.Mag();

  v = pv1.Cross(pv2);
  //if (v.Z()<0) v = -v;

  w = u.Cross(v);

  ua = u.Cross(z);
  ua = ua/ua.Mag();
 
  phiv = w.Angle(ua);
        
  if(mpdg1==22 && mpdg2==22 && mlbl1 == mlbl2 && abs(pdg1)==11 && abs(pdg2)==11 && phiv>TMath::PiOver2())     phiv = TMath::Pi()-phiv;        //    For RP Conversion make phiv < pi/2 if phiv is much larger than pi/2 (then B field was wrong)

}

void getopang(){

  opang = pv1.Angle(pv2);
}

void getmass(){


  mass = pv1.Mag();
  mass = mass * pv2.Mag();
  mass = mass * 2 * (1-TMath::Cos(pv1.Angle(pv2)));
  mass = TMath::Abs(mass);
  mass = TMath::Sqrt(mass);

}

void getdiffz(){        // angle p1_norm - p2_norm wrt z axis
  diff = pv1.Unit();
  tempv = pv2.Unit();
  diff = diff - tempv;
  diffz = diff.Angle(z);

}

void getsumz(){
  sum = pv1+pv2;
  sumz = sum.Angle(z);
}

void getpdgs(){


  mpdg_leg1 = mpdg1;
  mpdg_leg2 = mpdg2;
  pdg_leg1 = pdg1;
  pdg_leg2 = pdg2;



}


void getcharges(){


  charge_leg1 = charge1;
  charge_leg2 = charge2;
}

