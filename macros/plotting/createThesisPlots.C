void PlotPIDeffs();


void createThesisPlots() {
  PlotPIDeffs();
}


void PlotPIDeffs() {

  gROOT->ForceStyle();
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


  TCanvas *c = new TCanvas("c","",1024,768);
  PIDeffs->Draw();

  c->Modified();
  c->Update();
  c->SaveAs("temp_output/PIDeffs.root");
  c->SaveAs("temp_output/PIDeffs.pdf");
  c->SaveAs("temp_output/PIDeffs.png");

  gSystem->Exit(0);
}
