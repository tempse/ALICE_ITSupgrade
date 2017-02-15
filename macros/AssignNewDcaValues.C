// This macro copies an input tree containing pairTree data and adds additional
// leafs with "fake DCAxy" values for heavy-flavor events. The fake values are
// chosen randomly and accordingly to DCAxy values from an external data file.


#include <iostream>

#include <TString.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TGraph.h>
#include <TRandom.h>
#include <TStopwatch.h>



void AssignNewDcaValues() {

  TString infile_name = "../pairTrees/FT2_AnalysisResults_Upgrade_all-Ev_pairtree_us/FT2_AnalysisResults_Upgrade_all-Ev_pairtree_us.root";

  TString infile_DCAdata_name = "../inputData/FT2_AnalysisResults_DCAresults_DCAresultsFit.root";

  
  TFile *infile_DCAdata = new TFile(infile_DCAdata_name, "READ");

  // stores start values and ranges of the different pt intervals:
  Float_t pt_binStart[20];
  pt_binStart[0] = 0.;
  Float_t pt_binWidth[20];
  
  TGraph *gDCA = (TGraph*)infile_DCAdata->Get("gDCA");
  Float_t temp_sum = 0.;
  for(unsigned int i=0; i<20; i++) {
    pt_binWidth[i] = 2 * gDCA->GetErrorX(i);

    pt_binStart[i] = temp_sum;
    temp_sum += pt_binWidth[i];
  }
  
  
  TH1F *h_DCA_0 = (TH1F*)infile_DCAdata->Get("hDCA1_0"); // DCA distribution for pt_binStart[0] <= pt < pt_binStart[1]
  TH1F *h_DCA_1 = (TH1F*)infile_DCAdata->Get("hDCA1_1"); // DCA distribution for pt_binStart[1] <= pt < pt_binStart[2]
  TH1F *h_DCA_2 = (TH1F*)infile_DCAdata->Get("hDCA1_2"); // ...
  TH1F *h_DCA_3 = (TH1F*)infile_DCAdata->Get("hDCA1_3");
  TH1F *h_DCA_4 = (TH1F*)infile_DCAdata->Get("hDCA1_4");
  TH1F *h_DCA_5 = (TH1F*)infile_DCAdata->Get("hDCA1_5");
  TH1F *h_DCA_6 = (TH1F*)infile_DCAdata->Get("hDCA1_6");
  TH1F *h_DCA_7 = (TH1F*)infile_DCAdata->Get("hDCA1_7");
  TH1F *h_DCA_8 = (TH1F*)infile_DCAdata->Get("hDCA1_8");
  TH1F *h_DCA_9 = (TH1F*)infile_DCAdata->Get("hDCA1_9");
  TH1F *h_DCA_10 = (TH1F*)infile_DCAdata->Get("hDCA1_10");
  TH1F *h_DCA_11 = (TH1F*)infile_DCAdata->Get("hDCA1_11");
  TH1F *h_DCA_12 = (TH1F*)infile_DCAdata->Get("hDCA1_12");
  TH1F *h_DCA_13 = (TH1F*)infile_DCAdata->Get("hDCA1_13");
  TH1F *h_DCA_14 = (TH1F*)infile_DCAdata->Get("hDCA1_14");
  TH1F *h_DCA_15 = (TH1F*)infile_DCAdata->Get("hDCA1_15");
  TH1F *h_DCA_16 = (TH1F*)infile_DCAdata->Get("hDCA1_16");
  TH1F *h_DCA_17 = (TH1F*)infile_DCAdata->Get("hDCA1_17");
  TH1F *h_DCA_18 = (TH1F*)infile_DCAdata->Get("hDCA1_18");
  TH1F *h_DCA_19 = (TH1F*)infile_DCAdata->Get("hDCA1_19");


  
  TString outfile_name = "temp_fakeDcaTree.root";//"../pairTrees/FT2_AnalysisResults_Upgrade_all-Ev_pairtree_us/FT2_AnalysisResults_Upgrade_all-Ev_pairtree_us_train_1-100-split_fakeDcaValues.root";

  
  TFile *infile = new TFile(infile_name, "READ");
  TTree *input_tree = (TTree*)infile->Get("pairTree_us");

  Int_t in_trackid1, in_trackid2;
  Int_t in_eventid1, in_eventid2;
  Int_t in_isrp;
  Int_t in_isus;
  Int_t in_isconv;
  Int_t in_ishf;
  Int_t in_iscorrcharm;
  Int_t in_iscorrbottom;
  Int_t in_iscorrcharmfrombottom;
  Int_t in_chargesign;
  Float_t in_opang;
  Float_t in_diffz;
  Float_t in_mass;
  Float_t in_sumz;
  Float_t in_phiv;
  Float_t in_px1, in_px2;
  Float_t in_py1, in_py2;
  Float_t in_pz1, in_pz2;
  Float_t in_mcpx1, in_mcpx2;
  Float_t in_mcpy1, in_mcpy2;
  Float_t in_mcpz1, in_mcpz2;
  Int_t in_pdg1, in_pdg2;
  Int_t in_motherpdg1, in_motherpdg2;
  Float_t in_dcaxy1, in_dcaxy2;
  Float_t in_dcaz1, in_dcaz2;
  Int_t in_nits1, in_nits2;
  Float_t in_phi1, in_phi2;
  Float_t in_eta1, in_eta2;
  Float_t in_pt1, in_pt2;
  input_tree->SetBranchAddress("TrackID1", &in_trackid1);
  input_tree->SetBranchAddress("TrackID2", &in_trackid2);
  input_tree->SetBranchAddress("EventID1", &in_eventid1);
  input_tree->SetBranchAddress("EventID2", &in_eventid2);
  input_tree->SetBranchAddress("IsRP", &in_isrp);
  input_tree->SetBranchAddress("IsUS", &in_isus);
  input_tree->SetBranchAddress("IsConv", &in_isconv);
  input_tree->SetBranchAddress("IsHF", &in_ishf);
  input_tree->SetBranchAddress("IsCorrCharm", &in_iscorrcharm);
  input_tree->SetBranchAddress("IsCorrBottom", &in_iscorrbottom);
  input_tree->SetBranchAddress("IsCorrCharmFromBottom", &in_iscorrcharmfrombottom);
  input_tree->SetBranchAddress("ChargeSign", &in_chargesign);
  input_tree->SetBranchAddress("opang", &in_opang);
  input_tree->SetBranchAddress("diffz", &in_diffz);
  input_tree->SetBranchAddress("mass", &in_mass);
  input_tree->SetBranchAddress("sumz", &in_sumz);
  input_tree->SetBranchAddress("phiv", &in_phiv);
  input_tree->SetBranchAddress("px1", &in_px1);
  input_tree->SetBranchAddress("px2", &in_px2);
  input_tree->SetBranchAddress("py1", &in_py1);
  input_tree->SetBranchAddress("py2", &in_py2);
  input_tree->SetBranchAddress("pz1", &in_pz1);
  input_tree->SetBranchAddress("pz2", &in_pz2);
  input_tree->SetBranchAddress("mcPx1", &in_mcpx1);
  input_tree->SetBranchAddress("mcPx2", &in_mcpx2);
  input_tree->SetBranchAddress("mcPy1", &in_mcpy1);
  input_tree->SetBranchAddress("mcPy2", &in_mcpy2);
  input_tree->SetBranchAddress("mcPz1", &in_mcpz1);
  input_tree->SetBranchAddress("mcPz2", &in_mcpz2);
  input_tree->SetBranchAddress("pdg1", &in_pdg1);
  input_tree->SetBranchAddress("pdg2", &in_pdg2);
  input_tree->SetBranchAddress("motherPdg1", &in_motherpdg1);
  input_tree->SetBranchAddress("motherPdg2", &in_motherpdg2);
  input_tree->SetBranchAddress("DCAxy1", &in_dcaxy1);
  input_tree->SetBranchAddress("DCAxy2", &in_dcaxy2);
  input_tree->SetBranchAddress("DCAz1", &in_dcaz1);
  input_tree->SetBranchAddress("DCAz2", &in_dcaz2);
  input_tree->SetBranchAddress("nITS1", &in_nits1);
  input_tree->SetBranchAddress("nITS2", &in_nits2);
  input_tree->SetBranchAddress("phi1", &in_phi1);
  input_tree->SetBranchAddress("phi2", &in_phi2);
  input_tree->SetBranchAddress("eta1", &in_eta1);
  input_tree->SetBranchAddress("eta2", &in_eta2);
  input_tree->SetBranchAddress("pt1", &in_pt1);
  input_tree->SetBranchAddress("pt2", &in_pt2);


  
  TFile *outfile = new TFile(outfile_name, "RECREATE");
  TTree *output_tree = new TTree("pairTree_us","pairTree_us");
  
  Int_t out_trackid1, out_trackid2;
  Int_t out_eventid1, out_eventid2;
  Int_t out_isrp;
  Int_t out_isus;
  Int_t out_isconv;
  Int_t out_ishf;
  Int_t out_iscorrcharm;
  Int_t out_iscorrbottom;
  Int_t out_iscorrcharmfrombottom;
  Int_t out_chargesign;
  Float_t out_opang;
  Float_t out_diffz;
  Float_t out_mass;
  Float_t out_sumz;
  Float_t out_phiv;
  Float_t out_px1, out_px2;
  Float_t out_py1, out_py2;
  Float_t out_pz1, out_pz2;
  Float_t out_mcpx1, out_mcpx2;
  Float_t out_mcpy1, out_mcpy2;
  Float_t out_mcpz1, out_mcpz2;
  Int_t out_pdg1, out_pdg2;
  Int_t out_motherpdg1, out_motherpdg2;
  Float_t out_dcaxy1, out_dcaxy2;
  Float_t out_dcaxy1_fake, out_dcaxy2_fake;
  Float_t out_dcaz1, out_dcaz2;
  Int_t out_nits1, out_nits2;
  Float_t out_phi1, out_phi2;
  Float_t out_eta1, out_eta2;
  Float_t out_pt1, out_pt2;
  output_tree->Branch("TrackID1", &out_trackid1);
  output_tree->Branch("TrackID2", &out_trackid2);
  output_tree->Branch("EventID1", &out_eventid1);
  output_tree->Branch("EventID2", &out_eventid2);
  output_tree->Branch("IsRP", &out_isrp);
  output_tree->Branch("IsUS", &out_isus);
  output_tree->Branch("IsConv", &out_isconv);
  output_tree->Branch("IsHF", &out_ishf);
  output_tree->Branch("IsCorrCharm", &out_iscorrcharm);
  output_tree->Branch("IsCorrBottom", &out_iscorrbottom);
  output_tree->Branch("IsCorrCharmFromBottom", &out_iscorrcharmfrombottom);
  output_tree->Branch("ChargeSign", &out_chargesign);
  output_tree->Branch("opang", &out_opang);
  output_tree->Branch("diffz", &out_diffz);
  output_tree->Branch("mass", &out_mass);
  output_tree->Branch("sumz", &out_sumz);
  output_tree->Branch("phiv", &out_phiv);
  output_tree->Branch("px1", &out_px1);
  output_tree->Branch("px2", &out_px2);
  output_tree->Branch("py1", &out_py1);
  output_tree->Branch("py2", &out_py2);
  output_tree->Branch("pz1", &out_pz1);
  output_tree->Branch("pz2", &out_pz2);
  output_tree->Branch("mcPx1", &out_mcpx1);
  output_tree->Branch("mcPx2", &out_mcpx2);
  output_tree->Branch("mcPy1", &out_mcpy1);
  output_tree->Branch("mcPy2", &out_mcpy2);
  output_tree->Branch("mcPz1", &out_mcpz1);
  output_tree->Branch("mcPz2", &out_mcpz2);
  output_tree->Branch("pdg1", &out_pdg1);
  output_tree->Branch("pdg2", &out_pdg2);
  output_tree->Branch("motherPdg1", &out_motherpdg1);
  output_tree->Branch("motherPdg2", &out_motherpdg2);
  output_tree->Branch("DCAxy1", &out_dcaxy1);
  output_tree->Branch("DCAxy2", &out_dcaxy2);
  output_tree->Branch("DCAxy1_fakeHF", &out_dcaxy1_fake);
  output_tree->Branch("DCAxy2_fakeHF", &out_dcaxy2_fake);
  output_tree->Branch("DCAz1", &out_dcaz1);
  output_tree->Branch("DCAz2", &out_dcaz2);
  output_tree->Branch("nITS1", &out_nits1);
  output_tree->Branch("nITS2", &out_nits2);
  output_tree->Branch("phi1", &out_phi1);
  output_tree->Branch("phi2", &out_phi2);
  output_tree->Branch("eta1", &out_eta1);
  output_tree->Branch("eta2", &out_eta2);
  output_tree->Branch("pt1", &out_pt1);
  output_tree->Branch("pt2", &out_pt2);

  
  
  Long64_t num_entries = input_tree->GetEntries();

  TStopwatch *watch = new TStopwatch();
  
  std::cout << std::endl;
  watch->Start();
  for(Long64_t entry=0; entry<num_entries; entry++) {
    if((entry%10000)==0) {
      std::cout << "\rProcessing entry " << entry << " of "
				  << num_entries << " (" << entry*100/num_entries << " %)...";
    }
    input_tree->GetEntry(entry);

    out_trackid1 = in_trackid1;
    out_trackid2 = in_trackid2;
    out_eventid1 = in_eventid1;
    out_eventid2 = in_eventid2;
    out_isrp = in_isrp;
    out_isus = in_isus;
    out_isconv = in_isconv;
    out_ishf = in_ishf;
    out_iscorrcharm = in_iscorrcharm;
    out_iscorrbottom = in_iscorrbottom;
    out_iscorrcharmfrombottom = in_iscorrcharmfrombottom;
    out_chargesign = in_chargesign;
    out_opang = in_opang;
    out_diffz = in_diffz;
    out_mass = in_mass;
    out_sumz = in_sumz;
    out_phiv = in_phiv;
    out_px1 = in_px1;
    out_px2 = in_px2;
    out_py1 = in_py1;
    out_py2 = in_py2;
    out_pz1 = in_pz1;
    out_pz2 = in_pz2;
    out_mcpx1 = in_mcpx1;
    out_mcpx2 = in_mcpx2;
    out_mcpy1 = in_mcpy1;
    out_mcpy2 = in_mcpy2;
    out_mcpz1 = in_mcpz1;
    out_mcpz2 = in_mcpz2;
    out_pdg1 = in_pdg1;
    out_pdg2 = in_pdg2;
    out_motherpdg1 = in_motherpdg1;
    out_motherpdg2 = in_motherpdg2;
    out_dcaxy1 = in_dcaxy1;
    out_dcaxy2 = in_dcaxy2;
    out_dcaz1 = in_dcaz1;
    out_dcaz2 = in_dcaz2;
    out_nits1 = in_nits1;
    out_nits2 = in_nits2;
    out_eta1 = in_eta1;
    out_eta2 = in_eta2;
    out_pt1 = in_pt1;
    out_pt2 = in_pt2;


    
    // assign fake DCAxy values to HF events:
    if(in_ishf == 1) {

      // first leg:
      if(in_pt1 < pt_binStart[1])                     out_dcaxy1_fake = h_DCA_0->GetRandom();
      if(pt_binStart[1] <= in_pt1 < pt_binStart[2])   out_dcaxy1_fake = h_DCA_1->GetRandom();
      if(pt_binStart[2] <= in_pt1 < pt_binStart[3])   out_dcaxy1_fake = h_DCA_2->GetRandom();
      if(pt_binStart[3] <= in_pt1 < pt_binStart[4])   out_dcaxy1_fake = h_DCA_3->GetRandom();
      if(pt_binStart[4] <= in_pt1 < pt_binStart[5])   out_dcaxy1_fake = h_DCA_4->GetRandom();
      if(pt_binStart[5] <= in_pt1 < pt_binStart[6])   out_dcaxy1_fake = h_DCA_5->GetRandom();
      if(pt_binStart[6] <= in_pt1 < pt_binStart[7])   out_dcaxy1_fake = h_DCA_6->GetRandom();
      if(pt_binStart[7] <= in_pt1 < pt_binStart[8])   out_dcaxy1_fake = h_DCA_7->GetRandom();
      if(pt_binStart[8] <= in_pt1 < pt_binStart[9])   out_dcaxy1_fake = h_DCA_8->GetRandom();
      if(pt_binStart[9] <= in_pt1 < pt_binStart[10])  out_dcaxy1_fake = h_DCA_9->GetRandom();
      if(pt_binStart[10] <= in_pt1 < pt_binStart[11]) out_dcaxy1_fake = h_DCA_10->GetRandom();
      if(pt_binStart[11] <= in_pt1 < pt_binStart[12]) out_dcaxy1_fake = h_DCA_11->GetRandom();
      if(pt_binStart[12] <= in_pt1 < pt_binStart[13]) out_dcaxy1_fake = h_DCA_12->GetRandom();
      if(pt_binStart[13] <= in_pt1 < pt_binStart[14]) out_dcaxy1_fake = h_DCA_13->GetRandom();
      if(pt_binStart[14] <= in_pt1 < pt_binStart[15]) out_dcaxy1_fake = h_DCA_14->GetRandom();
      if(pt_binStart[15] <= in_pt1 < pt_binStart[16]) out_dcaxy1_fake = h_DCA_15->GetRandom();
      if(pt_binStart[16] <= in_pt1 < pt_binStart[17]) out_dcaxy1_fake = h_DCA_16->GetRandom();
      if(pt_binStart[17] <= in_pt1 < pt_binStart[18]) out_dcaxy1_fake = h_DCA_17->GetRandom();
      if(pt_binStart[18] <= in_pt1 < pt_binStart[19]) out_dcaxy1_fake = h_DCA_18->GetRandom();
      if(pt_binStart[19] <= in_pt1)                   out_dcaxy1_fake = h_DCA_19->GetRandom();

      // second leg:
      if(in_pt2 < pt_binStart[1])                     out_dcaxy2_fake = h_DCA_0->GetRandom();
      if(pt_binStart[1] <= in_pt2 < pt_binStart[2])   out_dcaxy2_fake = h_DCA_1->GetRandom();
      if(pt_binStart[2] <= in_pt2 < pt_binStart[3])   out_dcaxy2_fake = h_DCA_2->GetRandom();
      if(pt_binStart[3] <= in_pt2 < pt_binStart[4])   out_dcaxy2_fake = h_DCA_3->GetRandom();
      if(pt_binStart[4] <= in_pt2 < pt_binStart[5])   out_dcaxy2_fake = h_DCA_4->GetRandom();
      if(pt_binStart[5] <= in_pt2 < pt_binStart[6])   out_dcaxy2_fake = h_DCA_5->GetRandom();
      if(pt_binStart[6] <= in_pt2 < pt_binStart[7])   out_dcaxy2_fake = h_DCA_6->GetRandom();
      if(pt_binStart[7] <= in_pt2 < pt_binStart[8])   out_dcaxy2_fake = h_DCA_7->GetRandom();
      if(pt_binStart[8] <= in_pt2 < pt_binStart[9])   out_dcaxy2_fake = h_DCA_8->GetRandom();
      if(pt_binStart[9] <= in_pt2 < pt_binStart[10])  out_dcaxy2_fake = h_DCA_9->GetRandom();
      if(pt_binStart[10] <= in_pt2 < pt_binStart[11]) out_dcaxy2_fake = h_DCA_10->GetRandom();
      if(pt_binStart[11] <= in_pt2 < pt_binStart[12]) out_dcaxy2_fake = h_DCA_11->GetRandom();
      if(pt_binStart[12] <= in_pt2 < pt_binStart[13]) out_dcaxy2_fake = h_DCA_12->GetRandom();
      if(pt_binStart[13] <= in_pt2 < pt_binStart[14]) out_dcaxy2_fake = h_DCA_13->GetRandom();
      if(pt_binStart[14] <= in_pt2 < pt_binStart[15]) out_dcaxy2_fake = h_DCA_14->GetRandom();
      if(pt_binStart[15] <= in_pt2 < pt_binStart[16]) out_dcaxy2_fake = h_DCA_15->GetRandom();
      if(pt_binStart[16] <= in_pt2 < pt_binStart[17]) out_dcaxy2_fake = h_DCA_16->GetRandom();
      if(pt_binStart[17] <= in_pt2 < pt_binStart[18]) out_dcaxy2_fake = h_DCA_17->GetRandom();
      if(pt_binStart[18] <= in_pt2 < pt_binStart[19]) out_dcaxy2_fake = h_DCA_18->GetRandom();
      if(pt_binStart[19] <= in_pt2)                   out_dcaxy2_fake = h_DCA_19->GetRandom();
      
    }else { // copy real DCA values for non-HF events
      
      out_dcaxy1_fake = out_dcaxy1;
      out_dcaxy2_fake = out_dcaxy2;
      
    }
    

    output_tree->Fill();
  }
  
  watch->Stop();
  
  std::cout << "\rProcessing entry " << num_entries << " of " << num_entries
	    << " (100 %)" << std::endl << std::endl;

  std::cout << "Time passed since begin of processing: ";
  watch->Print();
  std::cout << std::endl;

  output_tree->Write();

  std::cout << "Created file " << outfile->GetName() << std::endl;
  
}
