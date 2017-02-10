void ExtractDcaxyHistos() {
  
  TFile *infile = new TFile("../pairTrees/FT2_AnalysisResults_Upgrade_all-Ev_pairtree_us/FT2_AnalysisResults_Upgrade_all-Ev_pairtree_us.root", "READ");
  TTree *tree = (TTree*)infile->Get("pairTree_us");

  Float_t DCAxy1, DCAxy2;
  Int_t IsCorrCharm, IsCorrBottom;
  tree->SetBranchAddress("DCAxy1", &DCAxy1);
  tree->SetBranchAddress("DCAxy2", &DCAxy2);
  tree->SetBranchAddress("IsCorrCharm", &IsCorrCharm);
  tree->SetBranchAddress("IsCorrBottom", &IsCorrBottom);


  TFile *outfile = new TFile("DCAxy_histograms.root","RECREATE");
  Int_t num_bins = 1000,
    min = -.5,
    max = .5;
  TH1F *h_DCAxy1_ccbar = new TH1F("h_DCAxy1_ccbar", "", num_bins, min, max);
  TH1F *h_DCAxy2_ccbar = new TH1F("h_DCAxy2_ccbar", "", num_bins, min, max);
  TH1F *h_DCAxy1_bbbar = new TH1F("h_DCAxy1_bbbar", "", num_bins, min,max);
  TH1F *h_DCAxy2_bbbar = new TH1F("h_DCAxy2_bbbar", "", num_bins, min,max);


  Int_t num_entries = tree->GetEntries()/10;

  std::cout << std::endl;
  for(Int_t entry=0; entry<num_entries; entry++) {

    if((entry%1000)==0) {
      std::cout << "\rProcessing event " << entry << " of " << num_entries
		<< " (" << entry*100/num_entries << " %)";
    }
    
    tree->GetEntry(entry);

    if (IsCorrCharm == 1) {
      h_DCAxy1_ccbar->Fill(DCAxy1);
      h_DCAxy2_ccbar->Fill(DCAxy2);
    }

    if (IsCorrBottom == 1) {
      h_DCAxy1_bbbar->Fill(DCAxy1);
      h_DCAxy2_bbbar->Fill(DCAxy2);
    }
    
  }
  std::cout << "\rProcessing event " << num_entries << " of " << num_entries
	    << " (100 %)" << std::endl;

  
  
  TF1 *gausfit_DCAxy1_ccbar = new TF1("gausfit_DCAxy1_ccbar",
				      "[0]*exp(-.5*((x-[1])/[2])**2)",
				      min,
				      max);
  TF1 *gausfit_DCAxy2_ccbar = new TF1("gausfit_DCAxy2_ccbar",
				      "[0]*exp(-.5*((x-[1])/[2])**2)",
				      min,
				      max);
  TF1 *gausfit_DCAxy1_bbbar = new TF1("gausfit_DCAxy1_bbbar",
				      "[0]*exp(-.5*((x-[1])/[2])**2)",
				      min,
				      max);
  TF1 *gausfit_DCAxy2_bbbar = new TF1("gausfit_DCAxy2_bbbar",
				      "[0]*exp(-.5*((x-[1])/[2])**2)",
				      min,
				      max);

  gausfit_DCAxy1_ccbar->SetParameters(1., 0., 1.);
  gausfit_DCAxy2_ccbar->SetParameters(1., 0., 1.);
  gausfit_DCAxy1_bbbar->SetParameters(1., 0., 1.);
  gausfit_DCAxy2_bbbar->SetParameters(1., 0., 1.);
  
  h_DCAxy1_ccbar->Fit("gausfit_DCAxy1_ccbar");
  h_DCAxy2_ccbar->Fit("gausfit_DCAxy2_ccbar");
  h_DCAxy1_bbbar->Fit("gausfit_DCAxy1_bbbar");
  h_DCAxy2_bbbar->Fit("gausfit_DCAxy2_bbbar");

  
  outfile->Write();
  

  // TCanvas *c_DCAxy1_ccbar = new TCanvas();
  // h_DCAxy1_ccbar->Draw("hist e");

  // TCanvas *c_DCAxy1_bbbar = new TCanvas();
  // h_DCAxy1_bbbar->Draw("hist e");
  
}
