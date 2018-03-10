void PlotPIDeffs();
void PlotPhiv();
void PlotPhivMass();
void PlotMassVsPt();


void createThesisPlots() {
  
  gROOT->ForceStyle();
  
  PlotPIDeffs();
  PlotPhiv();
  PlotPhivMass();
  PlotMassVsPt();
  
  gSystem->Exit(0);
  
}


void PlotPIDeffs() {

  SetStyle();
  
  TFile *f = new TFile("/home/sebastian/analysis/data/finalAnalysis_FT2/inputdata/ITSU_PIDefficiency_lowB.root",
                       "READ");

  TH1D *PIDeffs = (TH1D*)f->Get("efficiencyLHC17d12_TPCandTOF3sigma");
  
  PIDeffs->GetXaxis()->SetRangeUser(0,5);

  PIDeffs->GetYaxis()->SetTitle("PID efficiency");
  PIDeffs->GetXaxis()->SetTitle("#it{p}_{T} / (GeV/#it{c})");

  PIDeffs->SetMarkerStyle(20);
  PIDeffs->SetMarkerSize(1);
  PIDeffs->SetMarkerColor(kBlue);
  PIDeffs->SetLineColor(kBlue);


  TCanvas *c_PIDeffs = new TCanvas("c","",1024,768);
  PIDeffs->Draw();

  c_PIDeffs->Modified();
  c_PIDeffs->Update();
  c_PIDeffs->SaveAs("temp_output/PIDeffs.root");
  c_PIDeffs->SaveAs("temp_output/PIDeffs.pdf");
  c_PIDeffs->SaveAs("temp_output/PIDeffs.png");
  
}


void PlotPhiv() {
  
  SetStyle();

  TFile *f = new TFile("/home/sebastian/analysis/data/finalAnalysis_FT2/workingdata/FT2_AnalysisResults_wLooseTracks_iGeo12_pairTree-us_analysis-0.8nEvents.root",
                       "READ");
  TTree *tree = (TTree*)f->Get("pairTree_us");

  Float_t phiv, mass;
  Int_t IsRP, IsConv, TrackCut1, TrackCut2; 
  tree->SetBranchAddress("phiv", &phiv);
  tree->SetBranchAddress("mass", &mass);
  tree->SetBranchAddress("IsRP", &IsRP);
  tree->SetBranchAddress("IsConv", &IsConv);
  tree->SetBranchAddress("TrackCut1", &TrackCut1);
  tree->SetBranchAddress("TrackCut2", &TrackCut2);

  TH1D *h_nonconvs = new TH1D("h_nonconvs","",100,0,TMath::Pi());
  TH1D *h_convs = new TH1D("h_convs","",100,0,TMath::Pi());

  for(Long64_t i=0; i<1e7; i++) {
    tree->GetEntry(i);

    if( !(IsRP==1 && IsConv==1) && mass<0.05 && TrackCut1==2 && TrackCut2==2 ) {
      h_nonconvs->Fill(phiv);
    } else if( (IsRP==1 && IsConv==1) && mass<0.05 && TrackCut1==2 && TrackCut2==2 ) {
      h_convs->Fill(phiv);
    }
  }

  h_convs->SetXTitle("#varphi_{V}");
  h_convs->SetYTitle("Yield / (arb. units)");
  h_convs->GetYaxis()->SetTitleOffset(1.3);

  h_nonconvs->SetLineColor(kGreen+2);
  h_convs->SetLineColor(kRed);

  TCanvas *c_phiv = new TCanvas("c_phiv","",1024, 768);

  h_convs->Draw("hist");
  h_nonconvs->Draw("hist same");

  TLatex *l_info = new TLatex(.19,.5,"HIJING, Pb-Pb #sqrt{#it{s}_{NN}} = 5.5 TeV, 0-10%, #it{m}_{ee} < 50 MeV/#it{c}^{2}");
  l_info->SetTextSize(0.03);
  l_info->SetTextFont(42);
  l_info->SetNDC();
  l_info->Draw();

  TLegend *leg_phiv = new TLegend(.19,.35,.7,.48);
  leg_phiv->SetFillStyle(0);
  leg_phiv->AddEntry(h_convs, "e^{+}e^{-} pairs from secondary conversions", "l");
  leg_phiv->AddEntry(h_nonconvs, "other e^{+}e^{-} pairs", "l");
  leg_phiv->Draw();
  
  
  c_phiv->Modified();
  c_phiv->Update();

  c_phiv->SaveAs("temp_output/phiv_firstMassBin.root");
  c_phiv->SaveAs("temp_output/phiv_firstMassBin.pdf");
  c_phiv->SaveAs("temp_output/phiv_firstMassBin.png");

  f->Close();
  
}


void PlotPhivMass() {

  SetStyle();

  TFile *f = new TFile("/home/sebastian/analysis/data/finalAnalysis_FT2/workingdata/FT2_AnalysisResults_wLooseTracks_iGeo12_pairTree-us_analysis-0.8nEvents.root",
                       "READ");
  TTree *tree = (TTree*)f->Get("pairTree_us");

  Float_t phiv, mass;
  Int_t IsRP, IsConv, TrackCut1, TrackCut2; 
  tree->SetBranchAddress("phiv", &phiv);
  tree->SetBranchAddress("mass", &mass);
  tree->SetBranchAddress("IsRP", &IsRP);
  tree->SetBranchAddress("IsConv", &IsConv);
  tree->SetBranchAddress("TrackCut1", &TrackCut1);
  tree->SetBranchAddress("TrackCut2", &TrackCut2);

  TH2D *h_phivmass = new TH2D("h_phivmass","",300,0,0.1,300,0,TMath::Pi());

  for(Long64_t i=0; i<1e8; i++) {
    tree->GetEntry(i);

    if( IsRP==1 && IsConv==1 && TrackCut1==2 && TrackCut2==2 ) {
      h_phivmass->Fill(mass, phiv);
    }
  }

  std::cout << h_phivmass->GetEntries() << std::endl;

  h_phivmass->SetXTitle("#it{m}_{ee} / (GeV/#it{c}^{2})");
  h_phivmass->SetYTitle("#varphi_{V}");

  TCanvas *c_phivmass = new TCanvas("c_phivmass","",1024,768);

  gStyle->SetOptStat(0);
  h_phivmass->Draw("colz");

  TLatex *l_info = new TLatex(.43,.25,"HIJING, Pb-Pb #sqrt{#it{s}_{NN}} = 5.5 TeV, 0-10%");
  l_info->SetTextSize(0.035);
  l_info->SetTextFont(42);
  l_info->SetNDC();
  l_info->Draw();

  c_phivmass->SetRightMargin(0.13);
  
  c_phivmass->Modified();
  c_phivmass->Update();

  c_phivmass->SaveAs("temp_output/phiv_mass.root");
  c_phivmass->SaveAs("temp_output/phiv_mass.pdf");
  c_phivmass->SaveAs("temp_output/phiv_mass.png");

  f->Close();

}


void PlotMassVsPt() {

  SetStyle();
  gStyle->SetTitleOffset(1.6, "y");

  TFile *f = new TFile("/home/sebastian/analysis/data/HFstudy/workingData/CA_AnalysisResults_Upgrade_GP_pairTree-us_part2_0.8nEvents.root",
                       "READ");
  
  TTree *t = (TTree*)f->Get("pairTree_us");
  
  Int_t IsRP, IsConv;
  Float_t px1, px2, py1, py2, mass;
  Float_t pair_pT;
  Float_t MVAoutput_track1_wKinFeats, MVAoutput_track2_wKinFeats;
  Float_t MVAoutput_track1_woKinFeats, MVAoutput_track2_woKinFeats;

  t->SetBranchAddress("IsRP", &IsRP);
  t->SetBranchAddress("IsConv", &IsConv);
  t->SetBranchAddress("mass", &mass);
  t->SetBranchAddress("px1", &px1);
  t->SetBranchAddress("px2", &px2);
  t->SetBranchAddress("py1", &py1);
  t->SetBranchAddress("py2", &py2);
  t->SetBranchAddress("MVAoutput__HF_2__DNN_5__track1", &MVAoutput_track1_wKinFeats);
  t->SetBranchAddress("MVAoutput__HF_2__DNN_5__track2", &MVAoutput_track2_wKinFeats);
  t->SetBranchAddress("MVAoutput__HF_2__DNN_11__track1", &MVAoutput_track1_woKinFeats);
  t->SetBranchAddress("MVAoutput__HF_2__DNN_11__track2", &MVAoutput_track2_woKinFeats);

  Float_t MVAcut_wKinFeats = 0.28;
  Float_t MVAcut_woKinFeats = 0.23;

  TString xtitle = "#it{p}_{T} / (GeV/#it{c})";
  TString ytitle = "#it{m}_{ee} / (GeV/#it{c}^{2})";
  
  Int_t nbinsx = 300, nbinsy = 300;
  Float_t binx_min_1 = 0., binx_max_1 = 1.4;
  Float_t biny_min_1 = 0., biny_max_1 = 1.4;
  Float_t binx_min_2 = 0., binx_max_2 = 1.4;
  Float_t biny_min_2 = 0., biny_max_2 = 1.4;
  
  TH2D *h_S_beforeCut_wKinFeats = new TH2D("h_S_beforeCut_wKinFeats","h_S_beforeCut_wKinFeats",
                                           nbinsx, binx_min_1, binx_max_1,
                                           nbinsy, biny_min_1, biny_max_1);
  TH2D *h_S_afterCut_wKinFeats = new TH2D("h_S_afterCut_wKinFeats","",
                                          nbinsx, binx_min_1, binx_max_1,
                                          nbinsy, biny_min_1, biny_max_1);
  TH2D *h_CombNonConv_beforeCut_wKinFeats = new TH2D("h_CombNonConv_beforeCut_wKinFeats","",
                                                     nbinsx, binx_min_2, binx_max_2,
                                                     nbinsy, biny_min_2, biny_max_2);
  TH2D *h_CombNonConv_afterCut_wKinFeats = new TH2D("h_CombNonConv_afterCut_wKinFeats","",
                                                    nbinsx, binx_min_2, binx_max_2,
                                                    nbinsy, biny_min_2, biny_max_2);
  TH2D *h_S_beforeCut_woKinFeats = new TH2D("h_S_beforeCut_woKinFeats","",
                                            nbinsx, binx_min_1, binx_max_1,
                                            nbinsy, biny_min_1, biny_max_1);
  TH2D *h_S_afterCut_woKinFeats = new TH2D("h_S_afterCut_woKinFeats","",
                                           nbinsx, binx_min_1, binx_max_1,
                                           nbinsy, biny_min_1, biny_max_1);
  TH2D *h_CombNonConv_beforeCut_woKinFeats = new TH2D("h_CombNonConv_beforeCut_woKinFeats","",
                                                      nbinsx, binx_min_2, binx_max_2,
                                                      nbinsy, biny_min_2, biny_max_2);
  TH2D *h_CombNonConv_afterCut_woKinFeats = new TH2D("h_CombNonConv_afterCut_woKinFeats","",
                                                     nbinsx, binx_min_2, binx_max_2,
                                                     nbinsy, biny_min_2, biny_max_2);

  Long64_t nentries = t->GetEntries();

  // process wKinFeats:
  for(Long64_t i=0; i<nentries; i++) {

    t->GetEntry(i);

    if(MVAoutput_track1_wKinFeats < 0 || MVAoutput_track1_wKinFeats > 1 ||
       MVAoutput_track2_wKinFeats < 0 || MVAoutput_track2_wKinFeats > 1)
      continue;

    pair_pT = TMath::Sqrt((px1+px2)*(px1+px2) + (py1+py2)*(py1+py2));

    if(IsRP==1 && IsConv==0) {
      h_S_beforeCut_wKinFeats->Fill(pair_pT, mass);
      if(MVAoutput_track1_wKinFeats >= MVAcut_wKinFeats &&
         MVAoutput_track2_wKinFeats >= MVAcut_wKinFeats) {
        h_S_afterCut_wKinFeats->Fill(pair_pT, mass);
      }
    }

    if(IsRP==0 && IsConv==0) {
      h_CombNonConv_beforeCut_wKinFeats->Fill(pair_pT, mass);
      if(MVAoutput_track1_wKinFeats >= MVAcut_wKinFeats &&
         MVAoutput_track2_wKinFeats >= MVAcut_wKinFeats) {
        h_CombNonConv_afterCut_wKinFeats->Fill(pair_pT, mass);
      }
    }

  }
    
  // process woKinFeats:
  for(Long64_t i=0; i<nentries; i++) {
    
    t->GetEntry(i);
    
    if(MVAoutput_track1_woKinFeats < 0 || MVAoutput_track1_woKinFeats > 1 ||
       MVAoutput_track2_woKinFeats < 0 || MVAoutput_track2_woKinFeats > 1)
      continue;
    
    pair_pT = TMath::Sqrt((px1+px2)*(px1+px2) + (py1+py2)*(py1+py2));

    if(IsRP==1 && IsConv==0) {
      h_S_beforeCut_woKinFeats->Fill(pair_pT, mass);
      if(MVAoutput_track1_woKinFeats >= MVAcut_woKinFeats &&
         MVAoutput_track2_woKinFeats >= MVAcut_woKinFeats) {
        h_S_afterCut_woKinFeats->Fill(pair_pT, mass);
      }
    }

    if(IsRP==0 && IsConv==0) {
      h_CombNonConv_beforeCut_woKinFeats->Fill(pair_pT, mass);
      if(MVAoutput_track1_woKinFeats >= MVAcut_woKinFeats &&
         MVAoutput_track2_woKinFeats >= MVAcut_woKinFeats) {
        h_CombNonConv_afterCut_woKinFeats->Fill(pair_pT, mass);
      }
    } 
    
  }

  h_S_beforeCut_wKinFeats->SetXTitle(xtitle);
  h_S_afterCut_wKinFeats->SetXTitle(xtitle);
  h_CombNonConv_beforeCut_wKinFeats->SetXTitle(xtitle);
  h_CombNonConv_afterCut_wKinFeats->SetXTitle(xtitle);
  h_S_beforeCut_woKinFeats->SetXTitle(xtitle);
  h_S_afterCut_woKinFeats->SetXTitle(xtitle);
  h_CombNonConv_beforeCut_woKinFeats->SetXTitle(xtitle);
  h_CombNonConv_afterCut_woKinFeats->SetXTitle(xtitle);

  h_S_beforeCut_wKinFeats->SetYTitle(ytitle);
  h_S_afterCut_wKinFeats->SetYTitle(ytitle);
  h_CombNonConv_beforeCut_wKinFeats->SetYTitle(ytitle);
  h_CombNonConv_afterCut_wKinFeats->SetYTitle(ytitle);
  h_S_beforeCut_woKinFeats->SetYTitle(ytitle);
  h_S_afterCut_woKinFeats->SetYTitle(ytitle);
  h_CombNonConv_beforeCut_woKinFeats->SetYTitle(ytitle);
  h_CombNonConv_afterCut_woKinFeats->SetYTitle(ytitle);


  TFile *f_outputHistograms = new TFile("temp_output/hist_massVsPt.root",
                                        "RECREATE");
  
  TCanvas *c = new TCanvas("c","c");
  c->SetCanvasSize(4096,8192);
  c->SetRightMargin(0.13);
  c->Divide(2,4);

  c->cd(1);
  h_S_beforeCut_wKinFeats->Draw("colz");
  // c->SaveAs("temp_output/HFstudy_S_beforeCut_wKinFeats.root");
  // c->SaveAs("temp_output/HFstudy_S_beforeCut_wKinFeats.pdf");
  // c->SaveAs("temp_output/HFstudy_S_beforeCut_wKinFeats.png");

  c->cd(2);
  h_S_afterCut_wKinFeats->Draw("colz");
  // c->SaveAs("temp_output/HFstudy_S_afterCut_wKinFeats.root");
  // c->SaveAs("temp_output/HFstudy_S_afterCut_wKinFeats.pdf");
  // c->SaveAs("temp_output/HFstudy_S_afterCut_wKinFeats.png");

  c->cd(5);
  h_CombNonConv_beforeCut_wKinFeats->Draw("colz");
  // c->SaveAs("temp_output/HFstudy_CombNonConv_beforeCut_wKinFeats.root");
  // c->SaveAs("temp_output/HFstudy_CombNonConv_beforeCut_wKinFeats.pdf");
  // c->SaveAs("temp_output/HFstudy_CombNonConv_beforeCut_wKinFeats.png");

  c->cd(6);
  h_CombNonConv_afterCut_wKinFeats->Draw("colz");
  // c->SaveAs("temp_output/HFstudy_CombNonConv_afterCut_wKinFeats.root");
  // c->SaveAs("temp_output/HFstudy_CombNonConv_afterCut_wKinFeats.pdf");
  // c->SaveAs("temp_output/HFstudy_CombNonConv_afterCut_wKinFeats.png");

  c->cd(3);
  h_S_beforeCut_woKinFeats->Draw("colz");
  // c->SaveAs("temp_output/HFstudy_S_beforeCut_woKinFeats.root");
  // c->SaveAs("temp_output/HFstudy_S_beforeCut_woKinFeats.pdf");
  // c->SaveAs("temp_output/HFstudy_S_beforeCut_woKinFeats.png");

  c->cd(4);
  h_S_afterCut_woKinFeats->Draw("colz");
  // c->SaveAs("temp_output/HFstudy_S_afterCut_woKinFeats.root");
  // c->SaveAs("temp_output/HFstudy_S_afterCut_woKinFeats.pdf");
  // c->SaveAs("temp_output/HFstudy_S_afterCut_woKinFeats.png");

  c->cd(7);
  h_CombNonConv_beforeCut_woKinFeats->Draw("colz");
  // c->SaveAs("temp_output/HFstudy_CombNonConv_beforeCut_woKinFeats.root");
  // c->SaveAs("temp_output/HFstudy_CombNonConv_beforeCut_woKinFeats.pdf");
  // c->SaveAs("temp_output/HFstudy_CombNonConv_beforeCut_woKinFeats.png");

  c->cd(8);
  h_CombNonConv_afterCut_woKinFeats->Draw("colz");
  // c->SaveAs("temp_output/HFstudy_CombNonConv_afterCut_woKinFeats.root");
  // c->SaveAs("temp_output/HFstudy_CombNonConv_afterCut_woKinFeats.pdf");
  // c->SaveAs("temp_output/HFstudy_CombNonConv_afterCut_woKinFeats.png");

  c->SaveAs("temp_output/HFstudy_massVsPt.root");
  c->SaveAs("temp_output/HFstudy_massVsPt.pdf");
  c->SaveAs("temp_output/HFstudy_massVsPt.png");

  f->Close();
  
  }
