//gSystem.Load("libPhysics.so");

// Abbreviations:
// rp <-> real pair
// us <-> unlike sign
// ls <-> like sign

void calculateMomenta();
void calculatePhiv();
void calculateOpang();
void calculateMass();
void calculateDiffz();
void calculateSumz();

// variables of output tree (1,2 <-> 1st leg, 2nd leg):
Int_t TrackID1, TrackID2;
Int_t EventID1, EventID2;
Int_t IsRP;                  // real pairs: 1, combinatorial pairs: 0
Int_t IsUS;                  // pair with unlike sign (regardless of IsRP)
Int_t IsConv;                // both mother particles are gammas (can still be comb. pairs)
Int_t ChargeSign;            // unlike sign: 0, like sign (++): 1, like sign (--): -1
Double_t opang;
Double_t diffz;
Double_t mass;
Double_t sumz;
Double_t phiv;
Double_t px1, px2;
Double_t py1, py2;
Double_t pz1, pz2;
Double_t MCpx1, MCpx2;       // }
Double_t MCpy1, MCpy2;       // } momenta calculated from MC data
Double_t MCpz1, MCpz2;       // }
Int_t mpdg_leg1, mpdg_leg2;
Int_t pdg_leg1, pdg_leg2;
Int_t mlabel1, mlabel2;
Double_t DCAxy1, DCAxy2;
Double_t DCAz1, DCAz2;
Int_t nITS1, nITS2;
Double_t chi2g1, chi2g2;
Double_t phi1, phi2;
Double_t eta1, eta2;
Double_t pt1, pt2;

TVector3 pv1, pv2;
TVector3 z(0,0,1);
TVector3 u,v,w,ua;          // for phiv calculation
TVector3 temp;

// program control parameters:
bool isPairTree_rp = true;       // }
bool isPairTree_us = true;       // } set/unset the
bool isPairTree_ls = false;      // } output trees here
bool isPairTree_us_ls = false;   // }



void generatePairTrees_mc() {
  TFile *infile = TFile::Open("FT2_AnalysisResults_Upgrade.root","READ");
  TTree *singleTree = (TTree*)infile->Get("outputITSup/tracks");

  TFile *outfile = TFile::Open("pairtrees.root","RECREATE");
  TTree *pairTree_rp = new TTree("pairTree_rp","pairTree_rp");
  TTree *pairTree_us = new TTree("pairTree_us","pairTree_us");
  TTree *pairTree_ls = new TTree("pairTree_ls","pairTree_ls");
  TTree *pairTree_us_ls = new TTree("pairTree_us_ls","pairTree_us_ls");

  // TH1F *histDiagnosis1 = new TH1F("histDiagnosis1","",1000,-500,500);
  // TH1F *histDiagnosis2 = new TH1F("histDiagnosis2","",1000,-500,500);

  
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
  Int_t ST_nTPC;

  
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
  singleTree->SetBranchAddress("nTPC",&ST_nTPC);


  if(isPairTree_rp) {
    pairTree_rp->Branch("TrackID1",&TrackID1);
    pairTree_rp->Branch("TrackID2",&TrackID2);
    pairTree_rp->Branch("EventID1",&EventID1);
    pairTree_rp->Branch("EventIS2",&EventID2);
    pairTree_rp->Branch("IsRP",&IsRP);
    pairTree_rp->Branch("IsUS",&IsUS);
    pairTree_rp->Branch("IsConv",&IsConv);
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
    pairTree_rp->Branch("MCpx1",&MCpx1);
    pairTree_rp->Branch("MCpx2",&MCpx2);
    pairTree_rp->Branch("MCpy1",&MCpy1);
    pairTree_rp->Branch("MCpy2",&MCpy2);
    pairTree_rp->Branch("MCpz1",&MCpz1);
    pairTree_rp->Branch("MCpz2",&MCpz2);
    pairTree_rp->Branch("mpdg_leg1",&mpdg_leg1);
    pairTree_rp->Branch("mpdg_leg2",&mpdg_leg2);
    pairTree_rp->Branch("pdg_leg1",&pdg_leg1);
    pairTree_rp->Branch("pdg_leg2",&pdg_leg2);
    pairTree_rp->Branch("DCAxy1",&DCAxy1);
    pairTree_rp->Branch("DCAxy2",&DCAxy2);
    pairTree_rp->Branch("DCAz1",&DCAz1);
    pairTree_rp->Branch("DCAz2",&DCAz2);
    pairTree_rp->Branch("nITS1",&nITS1);
    pairTree_rp->Branch("nITS2",&nITS2);
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
    pairTree_us->Branch("EventIS2",&EventID2);
    pairTree_us->Branch("IsRP",&IsRP);
    pairTree_us->Branch("IsUS",&IsUS);
    pairTree_us->Branch("IsConv",&IsConv);
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
    pairTree_us->Branch("MCpx1",&MCpx1);
    pairTree_us->Branch("MCpx2",&MCpx2);
    pairTree_us->Branch("MCpy1",&MCpy1);
    pairTree_us->Branch("MCpy2",&MCpy2);
    pairTree_us->Branch("MCpz1",&MCpz1);
    pairTree_us->Branch("MCpz2",&MCpz2);
    pairTree_us->Branch("mpdg_leg1",&mpdg_leg1);
    pairTree_us->Branch("mpdg_leg2",&mpdg_leg2);
    pairTree_us->Branch("pdg_leg1",&pdg_leg1);
    pairTree_us->Branch("pdg_leg2",&pdg_leg2);
    pairTree_us->Branch("DCAxy1",&DCAxy1);
    pairTree_us->Branch("DCAxy2",&DCAxy2);
    pairTree_us->Branch("DCAz1",&DCAz1);
    pairTree_us->Branch("DCAz2",&DCAz2);
    pairTree_us->Branch("nITS1",&nITS1);
    pairTree_us->Branch("nITS2",&nITS2);
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
    pairTree_ls->Branch("EventIS2",&EventID2);
    pairTree_ls->Branch("IsRP",&IsRP);
    pairTree_ls->Branch("IsUS",&IsUS);
    pairTree_ls->Branch("IsConv",&IsConv);
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
    pairTree_ls->Branch("MCpx1",&MCpx1);
    pairTree_ls->Branch("MCpx2",&MCpx2);
    pairTree_ls->Branch("MCpy1",&MCpy1);
    pairTree_ls->Branch("MCpy2",&MCpy2);
    pairTree_ls->Branch("MCpz1",&MCpz1);
    pairTree_ls->Branch("MCpz2",&MCpz2);
    pairTree_ls->Branch("mpdg_leg1",&mpdg_leg1);
    pairTree_ls->Branch("mpdg_leg2",&mpdg_leg2);
    pairTree_ls->Branch("pdg_leg1",&pdg_leg1);
    pairTree_ls->Branch("pdg_leg2",&pdg_leg2);
    pairTree_ls->Branch("DCAxy1",&DCAxy1);
    pairTree_ls->Branch("DCAxy2",&DCAxy2);
    pairTree_ls->Branch("DCAz1",&DCAz1);
    pairTree_ls->Branch("DCAz2",&DCAz2);
    pairTree_ls->Branch("nITS1",&nITS1);
    pairTree_ls->Branch("nITS2",&nITS2);
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
    pairTree_us_ls->Branch("EventIS2",&EventID2);
    pairTree_us_ls->Branch("IsRP",&IsRP);
    pairTree_us_ls->Branch("IsUS",&IsUS);
    pairTree_us_ls->Branch("IsConv",&IsConv);
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
    pairTree_us_ls->Branch("MCpx1",&MCpx1);
    pairTree_us_ls->Branch("MCpx2",&MCpx2);
    pairTree_us_ls->Branch("MCpy1",&MCpy1);
    pairTree_us_ls->Branch("MCpy2",&MCpy2);
    pairTree_us_ls->Branch("MCpz1",&MCpz1);
    pairTree_us_ls->Branch("MCpz2",&MCpz2);
    pairTree_us_ls->Branch("mpdg_leg1",&mpdg_leg1);
    pairTree_us_ls->Branch("mpdg_leg2",&mpdg_leg2);
    pairTree_us_ls->Branch("pdg_leg1",&pdg_leg1);
    pairTree_us_ls->Branch("pdg_leg2",&pdg_leg2);
    pairTree_us_ls->Branch("DCAxy1",&DCAxy1);
    pairTree_us_ls->Branch("DCAxy2",&DCAxy2);
    pairTree_us_ls->Branch("DCAz1",&DCAz1);
    pairTree_us_ls->Branch("DCAz2",&DCAz2);
    pairTree_us_ls->Branch("nITS1",&nITS1);
    pairTree_us_ls->Branch("nITS2",&nITS2);
    pairTree_us_ls->Branch("phi1",&phi1);
    pairTree_us_ls->Branch("phi2",&phi2);
    pairTree_us_ls->Branch("eta1",&eta1);
    pairTree_us_ls->Branch("eta2",&eta2);
    pairTree_us_ls->Branch("pt1",&pt1);
    pairTree_us_ls->Branch("pt2",&pt2);
  }
  



  Int_t ev_temp = -1; // used to detect new events
  Int_t firstTrack; // first track number in given event
  Int_t nTracks; // total number of tracks in given event
  
  Int_t singleTree_nEvents = singleTree->GetEntries()/100; //todo: loop over all events
  std::cout << std::endl;
  std::cout << "Start event processing...";
  for(Int_t tr1=0; tr1<singleTree_nEvents; tr1++) { // first track loop
    if((tr1%100)==0) std::cout << "\rProcessing event " << tr1 << " of " << singleTree_nEvents
			       << " (" << tr1*100/singleTree_nEvents << "%)...";
    singleTree->GetEntry(tr1);

    // detect new events:
    if(ev_temp != ST_event) {
      ev_temp = ST_event;
      firstTrack = tr1;
    }

    // pdg cut:
    if(abs(ST_pdg) != 11) continue; // keep electrons/positrons only

    // cut to ignore unphysical DCAz values (specific to prior analysis):
    if(ST_dcaZ == 999) continue;

    // pt cut:
    //if(pt1<.4 || pt1>5) continue;

    // already available, pairing-independent variables of 1st leg for output tree:
    EventID1 = ST_event;
    TrackID1 = tr1;
    MCpx1 = ST_particle->Px();
    MCpy1 = ST_particle->Py();
    MCpz1 = ST_particle->Pz();
    mpdg_leg1 = ST_pdgMother;
    pdg_leg1 = ST_pdg;
    mlabel1 = ST_particle->GetFirstMother(); // equiv. to ->GetMother(0); not the first mother of the track though!
    DCAxy1 = ST_dcaR;
    DCAz1 = ST_dcaZ;
    nITS1 = ST_nITS;
    phi1 = ST_phi;
    eta1 = ST_eta;
    pt1 = ST_pt;
    
    Int_t tr2 = tr1+1;
    while(true) { // loop over remaining tracks of the same event
      singleTree->GetEntry(tr2);
      if(ev_temp != ST_event) break; // exit loop over remaining tracks if event number changes

      // pdg cut:
      if(abs(ST_pdg) != 11) continue;
      
      // cut to ignore unphysical DCAz values (specific to prior analysis):
      if(ST_dcaZ == 999) {tr2++; continue;}
      
      // pt cut:
      //if(pt1<.4 || pt1>5) continue;

      // already available, pairing-independent variables of 2nd leg for output tree:
      EventID2 = ST_event;
      TrackID2 = tr2;
      MCpx2= ST_particle->Px();
      MCpy2 = ST_particle->Py();
      MCpz2 = ST_particle->Pz();
      mpdg_leg2 = ST_pdgMother;
      pdg_leg2 = ST_pdg;
      mlabel2 = ST_particle->GetFirstMother(); // equiv. to ->GetMother(0); not the first mother of the track though!
      DCAxy2 = ST_dcaR;
      DCAz2 = ST_dcaZ;
      nITS2 = ST_nITS;
      phi2 = ST_phi;
      eta2 = ST_eta;
      pt2 = ST_pt;

      IsRP = 0; // default value
      
      if(pdg_leg1*ST_pdg < 0) { // <-> unlike sign pair (via MC info!)
	IsUS = 1;
	
	ChargeSign = 0;

	calculateMomenta(); // has to be called before other methods that need momentum variables!
	calculateMass();
	calculatePhiv();
	calculateOpang();
        calculateDiffz();
	calculateSumz();

	if(mpdg_leg1==22 || mpdg_leg2==22) { // IsConv==1 for every pair that contains a conversion leg
	  IsConv = 1;
	}else {
	  IsConv = 0;
	}

	if(mlabel1==mlabel2 && abs(pdg_leg1)==11 && abs(pdg_leg2)==11) {
	  IsRP = 1;
	  if(isPairTree_rp) pairTree_rp->Fill();
	}
	if(isPairTree_us) pairTree_us->Fill();
	if(isPairTree_us_ls) pairTree_us_ls->Fill();
      }else { // <-> like sign pair
	IsUS = 0;
	
	if(pdg_leg1>0) ChargeSign = 1;
	else if(pdg_leg1<0) ChargeSign = -1;

	calculateMomenta(); // has to be called before other methods that need momentum variables!
	calculateMass();
	calculatePhiv();
	calculateOpang();
        calculateDiffz();
	calculateSumz();
	
	if(mpdg_leg1==22 || mpdg_leg2==22) {
	  IsConv = 1;
	}else {
	  IsConv = 0;
	}

	if(mlabel1==mlabel2 && abs(pdg_leg1)==11 && abs(pdg_leg2)==11) {
	  IsRP = 1;
	  if(isPairTree_rp) pairTree_rp->Fill();
	}
	if(isPairTree_ls) pairTree_ls->Fill();
	if(isPairTree_us_ls) pairTree_us_ls->Fill();
      }
      
      tr2++;
    }
  }
  std::cout << "\rProcessing event " << singleTree_nEvents << " of " << singleTree_nEvents
	    << " (100%)...";
  std::cout << " DONE." << std::endl << std::endl;

  infile->Close();
  
  if(isPairTree_rp) pairTree_rp->Write();
  if(isPairTree_us) pairTree_us->Write();
  if(isPairTree_ls) pairTree_ls->Write();
  if(isPairTree_us_ls) pairTree_us_ls->Write();

  outfile->Close();

  // histDiagnosis1->SaveAs("histDiagnosis1.root");
  // histDiagnosis2->SaveAs("histDiagnosis2.root");
  
}




void calculateMomenta() { // has to be called before other methods that need momentum variables!
  px1 = pt1*TMath::Cos(phi1);
  py1 = pt1*TMath::Sin(phi1);
  pz1 = pt1*TMath::SinH(eta1);


  px2 = pt2*TMath::Cos(phi2);
  py2 = pt2*TMath::Sin(phi2);
  pz2 = pt2*TMath::SinH(eta2);

  pv1.SetXYZ(px1, py1, pz1);
  pv2.SetXYZ(px2, py2, pz2);
}

void calculatePhiv() {
  if(pv1.Mag()==0 || pv2.Mag()==0) {
    std::cout << "Error in 'calculatePhiv()': Variable pv1 and/or pv2 empty." << std::endl;
    return;
  }
  if(pdg_leg1 < 0) {
    temp = pv1;
    pv1 = pv2;
    pv2 = temp;
  }
  u = pv1 + pv2;
  u = u.Unit();

  v = pv1.Cross(pv2);

  w = u.Cross(v);

  ua = u.Cross(z);
  ua = ua.Unit();

  phiv = w.Angle(ua);
  if(mpdg_leg1==22 && mpdg_leg2==22 && mlabel1==mlabel2 && abs(pdg_leg1)==11 && abs(pdg_leg2)==11 && phiv>TMath::PiOver2()) {
    phiv = TMath::Pi() - phiv; // For RP conversion, make phiv<pi/2 if phiv is much larger than pi/2 (then the B field was wrong)
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
