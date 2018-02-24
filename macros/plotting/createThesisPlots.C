void PlotPIDeffs();
void PlotPhiv();
void PlotPhivMass();


void createThesisPlots() {
  
  gROOT->ForceStyle();
  
  PlotPIDeffs();
  PlotPhiv();
  PlotPhivMass();
  
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

}
