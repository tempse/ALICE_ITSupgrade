void plot_mass() {
  TString fileName = "../TMVA/trainData/328k-Ev_us_MLP_BDT_0mass_largeTestTree/TMVA.root";
  TString h_text = "Combinatorial MLP";

  //// optimal MVA cuts for "signal = CombWithConvLegs":
  //const float MVAcut = .42; // highest significance for pairtree_us_MLP_classifier-CombWithConvLegs
  //const float MVAcut = .40; // highest significance for pairtree_us_MLP_classifier-CombWithConvLegs-0.1mass
  //// optimal MVA cuts for "signal = physical signal":
  const float MVAcut = .4; // highest significance for pairtree_us_MLP_classifier-CombWithConvLegs-0.1mass
  
  //const float MVAcut = .40; // highest significance for pairtree_us_MLP_classifier-S
  //const float MVAcut = .28; // highest significance for pairtree_us_MLP_classifier-S-0.1mass
  
  TFile *f = new TFile(fileName,"READ");
  TTree *TestTree = (TTree*)f->Get("TestTree");
  float MLP;
  float IsRP, IsConv, IsHF, mpdg_leg1, mpdg_leg2, mass;
  TestTree->SetBranchAddress("MLP",&MLP);
  TestTree->SetBranchAddress("IsRP",&IsRP);
  TestTree->SetBranchAddress("IsConv",&IsConv);
  TestTree->SetBranchAddress("IsHF",&IsHF);
  TestTree->SetBranchAddress("mpdg_leg1",&mpdg_leg1);
  TestTree->SetBranchAddress("mpdg_leg2",&mpdg_leg2);
  TestTree->SetBranchAddress("mass",&mass);

  const unsigned int min=0, max=2, nBins=20;
  TH1F *h_SB = new TH1F("h_SB","",nBins,min,max);
  TH1F *h_S = new TH1F("h_S","",nBins,min,max);
  TH1F *h_CombiWithConvLeg = new TH1F("h_CombiWithConvLeg","",nBins,min,max);
  TH1F *h_CombiWithoutConvLeg = new TH1F("h_CombiWithoughConvLeg","",nBins,min,max);
  TH1F *h_HF = new TH1F("h_HF","",nBins,min,max);
  TH1F *h_RPConv = new TH1F("h_RPConv","",nBins,min,max);
  TH1F *h_SB_MVAcut = new TH1F("h_SB_MVAcut","",nBins,min,max);
  TH1F *h_S_MVAcut = new TH1F("h_S_MVAcut","",nBins,min,max);
  TH1F *h_CombiWithConvLeg_MVAcut = new TH1F("h_CombiWithConvLeg_MVAcut","",nBins,min,max);
  TH1F *h_CombiWithoutConvLeg_MVAcut = new TH1F("h_CombiWithoutConvLeg_MVAcut","",nBins,min,max);
  TH1F *h_HF_MVAcut = new TH1F("h_HF_MVAcut","",nBins,min,max);
  TH1F *h_RPConv_MVAcut = new TH1F("h_RPConv_MVAcut","",nBins,min,max);

  TH1F *h_S_currentMVAcut = new TH1F("h_S_currentMVAcut","",nBins,min,max);
  TH1F *h_SB_currentMVAcut = new TH1F("h_SB_currentMVAcut","",nBins,min,max);
  TH1F *h_CombiWithConvLeg_currentMVAcut = new TH1F("h_CombiWithConvLeg_currentMVAcut","",nBins,min,max);
  TH1F *h_CombiWithoutConvLeg_currentMVAcut = new TH1F("h_CombiWithoutConvLeg_currentMVAcut","",nBins,min,max);
  TH1F *h_HF_currentMVAcut = new TH1F("h_HF_currentMVAcut","",nBins,min,max);
  TH1F *h_RPConv_currentMVAcut = new TH1F("h_RPConv_currentMVAcut","",nBins,min,max);
    
  const float stepSize = .1, nSteps = 10; // Note: 1/(nSteps)==stepSize must apply!
  TH2F *h_significance_MVAcutScan = new TH2F("h_significance_MVAcutScan","",nBins,min,max,nSteps,0,1);
  TH2F *h_signalOverBackground_MVAcutScan = new TH2F("h_signalOverBackground_MVAcutScan","",nBins,min,max,nSteps,0,1);
  float binContents_significance_MVAcutScan[nBins][nSteps];
  float binContents_signalOverBackground_MVAcutScan[nBins][nSteps];
  

  unsigned int nEv = TestTree->GetEntries();
  for(unsigned int i=1; i<=nSteps; i++) {
    std::cout << std::endl;
    for(unsigned int ev=0; ev<nEv; ev++) {
      if((ev%10000)==0) std::cout << "\rRun " << i << " of " << nSteps << ":  Processing event " << ev << " of " << nEv
				  << " (" << ev*100/nEv << "%)...";
      TestTree->GetEvent(ev);
      
      if(MLP <= stepSize*i) { // "<=" instead of ">=" in case the network is trained on the (physical) background
	h_SB_currentMVAcut->Fill(mass);
	if(IsRP==1 && IsConv==0) {
	  h_S_currentMVAcut->Fill(mass);
	}
	if(IsRP==0 && (mpdg_leg1==22 || mpdg_leg2==22)) {
	  h_CombiWithConvLeg_currentMVAcut->Fill(mass);
	}
	if(IsRP==0 && !(mpdg_leg1==22 || mpdg_leg2==22)) {
	  h_CombiWithoutConvLeg_currentMVAcut->Fill(mass);
	}
	if(IsRP==0 && IsHF==1) {
	  h_HF_currentMVAcut->Fill(mass);
	}
	if(IsRP==1 && IsConv==1) {
	  h_RPConv_currentMVAcut->Fill(mass);
	}
      }
      // create plots for a custom MVA cut value (but not for each step in the scan):
      if(stepSize*(i-1)<MVAcut && MVAcut<=stepSize*i) {
	h_SB->Fill(mass);
	if(IsRP==1 && IsConv==0) {
	  h_S->Fill(mass);
	}
	if(IsRP==0 && (mpdg_leg1==22 || mpdg_leg2==22)) {
	  h_CombiWithConvLeg->Fill(mass);
	}
	if(IsRP==0 && !(mpdg_leg1==22 || mpdg_leg2==22)) {
	  h_CombiWithoutConvLeg->Fill(mass);
	}
	if(IsRP==0 && IsHF==1) {
	  h_HF->Fill(mass);
	}
	if(IsRP==1 && IsConv==1) {
	  h_RPConv->Fill(mass);
	}

	if(MLP <= MVAcut) { // "<=" instead of ">=" in case the network is trained on the (physical) background
	  h_SB_MVAcut->Fill(mass);
	  if(IsRP==1 && IsConv==0) {
	    h_S_MVAcut->Fill(mass);
	  }
	  if(IsRP==0 && (mpdg_leg1==22 || mpdg_leg2==22)) {
	    h_CombiWithConvLeg_MVAcut->Fill(mass);
	  }
	  if(IsRP==0 && !(mpdg_leg1==22 || mpdg_leg2==22)) {
	    h_CombiWithoutConvLeg_MVAcut->Fill(mass);
	  }
	  if(IsRP==0 && IsHF==1) {
	    h_HF_MVAcut->Fill(mass);
	  }
	  if(IsRP==1 && IsConv==1) {
	    h_RPConv_MVAcut->Fill(mass);
	  }
	}
      }
    }
    std::cout << "\rRun " << i << " of " << nSteps << ":  Processing event "
	      << nEv << " of " << nEv << " (100%)... DONE.";
    
    // generating the 2D/3D histograms for the MVA cut scan:
    for(unsigned int j=1; j<=nBins; j++) {
      Int_t bin_S = h_S_currentMVAcut->GetBin(j),
	bin_SB = h_SB_currentMVAcut->GetBin(j),
	bin_CombiWithConvLeg = h_CombiWithConvLeg_currentMVAcut->GetBin(j),
	bin_CombiWithoutConvLeg = h_CombiWithoutConvLeg_currentMVAcut->GetBin(j),
	bin_HF = h_HF_currentMVAcut->GetBin(j),
	bin_RPConv = h_RPConv_currentMVAcut->GetBin(j),
	bin_MVAcutScan = h_significance_MVAcutScan->GetBin(j,i);
      // define your "signal"/"background" here:
      float S = h_S_currentMVAcut->GetBinContent(bin_S) + 
	h_HF_currentMVAcut->GetBinContent(bin_HF); + 
        h_RPConv_currentMVAcut->GetBinContent(bin_RPConv);
      float B = h_CombiWithConvLeg_currentMVAcut->GetBinContent(bin_CombiWithConvLeg) + 
	h_CombiWithoutConvLeg_currentMVAcut->GetBinContent(bin_CombiWithoutConvLeg);
      float significance = (B==0) ? 0 : S/TMath::Sqrt(S+B);
      float signalOverBackground = (B==0) ? 0 : S/B;
      h_significance_MVAcutScan->SetBinContent(bin_MVAcutScan,
					       significance);
      h_signalOverBackground_MVAcutScan->SetBinContent(bin_MVAcutScan,
						       signalOverBackground);
      binContents_significance_MVAcutScan[j-1][i-1] = significance;
      binContents_signalOverBackground_MVAcutScan[j-1][i-1] = signalOverBackground;
    }
    h_S_currentMVAcut->Reset();
    h_SB_currentMVAcut->Reset();
  }
  
  float AUCs_significance[nSteps], AUCs_signalOverBackground[nSteps];
  float AUC_significance_max = -1, AUC_significance_max_pos;
  float AUC_signalOverBackground_max = -1, AUC_signalOverBackground_max_pos;
  for(unsigned int k=0; k<nSteps; k++) {
    AUCs_significance[k] = 0.;
    AUCs_signalOverBackground[k] = 0.;
    for(unsigned int m=0; m<nBins; m++) {
      AUCs_significance[k] += binContents_significance_MVAcutScan[m][k];
      AUCs_signalOverBackground[k] += binContents_signalOverBackground_MVAcutScan[m][k];
    }
    if(AUCs_significance[k] > AUC_significance_max) {
      AUC_significance_max = AUCs_significance[k];
      AUC_significance_max_pos = k;
    }
    if(AUCs_signalOverBackground[k] > AUC_signalOverBackground_max) {
      AUC_signalOverBackground_max = AUCs_signalOverBackground[k];
      AUC_signalOverBackground_max_pos = k;
    }
  }
  std::cout << std::endl;
  std::cout << "Maximum significance AUC = " << AUC_significance_max
	    << " for an MVA cut value of "
	    << stepSize*(AUC_significance_max_pos+1) << std::endl;
  std::cout << "Maximum signal over background AUC = "
	    << AUC_signalOverBackground_max << " for an MVA cut value of "
	    << stepSize*(AUC_signalOverBackground_max_pos+1) << std::endl;
  
  
  gStyle->SetOptStat(0);
  
  TCanvas *c_significance_scan = new TCanvas("c_significance_scan","",800,600);
  c_significance_scan->SetGridy();
  c_significance_scan->SetLogz();
  h_significance_MVAcutScan->SetXTitle("M_{ee} / (GeV/c^{2})");
  h_significance_MVAcutScan->SetYTitle("MVA cut");
  h_significance_MVAcutScan->SetZTitle("S/#sqrt{S+B}");
  gStyle->SetPalette(54);
  // gStyle->SetPaintTextFormat("A %.2f");
  h_significance_MVAcutScan->Draw("colz");
  TLatex l_AUC_significance_max;
  l_AUC_significance_max.SetTextSize(.025);
  TString text_AUC_significance_max = "max(S/#sqrt{S+B}) for MVA cut at ";
  text_AUC_significance_max += stepSize*(AUC_significance_max_pos+1);
  l_AUC_significance_max.DrawLatex(.05,.95,text_AUC_significance_max);
  c_significance_scan->SaveAs("temp_output/mass_signScan.pdf");
  c_significance_scan->SaveAs("temp_output/mass_signScan.root");
  TCanvas *c_significance_scan_3D = new TCanvas("c_significance_scan_3D","",800,600);
  c_significance_scan_3D->SetLogz();
  h_significance_MVAcutScan->Draw("lego2");

  TCanvas *c_signalOverBackground_scan = new TCanvas("c_signalOverBackground_scan","",800,600);
  c_signalOverBackground_scan->SetGridy();
  c_signalOverBackground_scan->SetLogz();
  h_signalOverBackground_MVAcutScan->SetXTitle("M_{ee} / (GeV/c^{2})");
  h_signalOverBackground_MVAcutScan->SetYTitle("MVA cut");
  h_signalOverBackground_MVAcutScan->SetZTitle("S/B");
  gStyle->SetPalette(54);
  // gStyle->SetPaintTextFormat("A %.2f");
  h_signalOverBackground_MVAcutScan->Draw("colz");
  TLatex l_AUC_signalOverBackground_max;
  l_AUC_signalOverBackground_max.SetTextSize(.025);
  TString text_AUC_signalOverBackground_max = "max(S/B) for MVA cut at ";
  text_AUC_signalOverBackground_max += stepSize*(AUC_signalOverBackground_max_pos+1);
  l_AUC_signalOverBackground_max.DrawLatex(.05,.95,text_AUC_signalOverBackground_max);
  c_signalOverBackground_scan->SaveAs("temp_output/mass_signScan.pdf");
  c_signalOverBackground_scan->SaveAs("temp_output/mass_signScan.root");
  TCanvas *c_signalOverBackground_scan_3D = new TCanvas("c_signalOverBackground_scan_3D","",800,600);
  c_signalOverBackground_scan_3D->SetLogz();
  h_signalOverBackground_MVAcutScan->Draw("lego2");

  
  TH1F *h_SB_eff = (TH1F*)h_SB_MVAcut->Clone();
  TH1F *h_S_eff = (TH1F*)h_S_MVAcut->Clone();
  TH1F *h_CombiWithConvLeg_eff = (TH1F*)h_CombiWithConvLeg_MVAcut->Clone();
  TH1F *h_CombiWithoutConvLeg_eff = (TH1F*)h_CombiWithoutConvLeg_MVAcut->Clone();
  TH1F *h_HF_eff = (TH1F*)h_HF_MVAcut->Clone();
  TH1F *h_RPConv_eff = (TH1F*)h_RPConv_MVAcut->Clone();
  h_SB_eff->Sumw2();
  h_SB->Sumw2();
  h_SB_eff->Divide(h_SB);
  h_S_eff->Sumw2();
  h_S->Sumw2();
  h_S_eff->Divide(h_S);
  h_CombiWithConvLeg_eff->Sumw2();
  h_CombiWithConvLeg->Sumw2();
  h_CombiWithConvLeg_eff->Divide(h_CombiWithConvLeg);
  h_CombiWithoutConvLeg_eff->Sumw2();
  h_CombiWithoutConvLeg->Sumw2();
  h_CombiWithoutConvLeg_eff->Divide(h_CombiWithoutConvLeg);
  h_HF_eff->Sumw2();
  h_HF->Sumw2();
  h_HF_eff->Divide(h_HF);
  h_RPConv_eff->Sumw2();
  h_RPConv->Sumw2();
  h_RPConv_eff->Divide(h_RPConv);



  //////////////////// SETTING UP THE PLOTS:  
  
  h_SB->SetLineColor(kBlack);
  h_S->SetLineColor(kGreen+1);
  h_CombiWithConvLeg->SetLineColor(kRed);
  h_CombiWithoutConvLeg->SetLineColor(kBlue);
  h_HF->SetLineColor(kOrange);
  h_RPConv->SetLineColor(13);

  h_SB_MVAcut->SetLineColor(kBlack);
  h_SB_MVAcut->SetMarkerColor(kBlack);
  h_S_MVAcut->SetLineColor(kGreen+1);
  h_S_MVAcut->SetMarkerColor(kGreen+1);
  h_CombiWithConvLeg_MVAcut->SetLineColor(kRed);
  h_CombiWithConvLeg_MVAcut->SetMarkerColor(kRed);
  h_CombiWithoutConvLeg_MVAcut->SetLineColor(kBlue);
  h_CombiWithoutConvLeg_MVAcut->SetMarkerColor(kBlue);
  h_HF_MVAcut->SetLineColor(kOrange);
  h_HF_MVAcut->SetMarkerColor(kOrange);
  h_RPConv_MVAcut->SetLineColor(13);
  h_RPConv_MVAcut->SetMarkerColor(13);
  
  h_SB_MVAcut->SetMarkerStyle(7);
  h_S_MVAcut->SetMarkerStyle(7);
  h_CombiWithConvLeg_MVAcut->SetMarkerStyle(7);
  h_CombiWithoutConvLeg_MVAcut->SetMarkerStyle(7);
  h_HF_MVAcut->SetMarkerStyle(7);
  h_RPConv_MVAcut->SetMarkerStyle(7);

  h_SB->GetYaxis()->SetRangeUser(1e-1,1e6);
  h_SB->SetXTitle("M_{ee} / (GeV/c^{2})");
  h_SB->SetYTitle("Entries");
  h_SB->GetXaxis()->SetTitleOffset(1.2);
  h_SB->GetYaxis()->SetTitleOffset(1.3);

  h_SB_eff->SetLineColor(kBlack);
  h_S_eff->SetLineColor(kGreen+1);
  h_CombiWithConvLeg_eff->SetLineColor(kRed);
  h_CombiWithoutConvLeg_eff->SetLineColor(kBlue);
  h_HF_eff->SetLineColor(kOrange);
  h_RPConv_eff->SetLineColor(13);

  h_SB_eff->SetMarkerStyle(7);
  h_S_eff->SetMarkerStyle(7);
  h_CombiWithConvLeg_eff->SetMarkerStyle(7);
  h_CombiWithoutConvLeg_eff->SetMarkerStyle(7);
  h_HF_eff->SetMarkerStyle(7);
  h_RPConv_eff->SetMarkerStyle(7);
  
  h_SB_eff->SetMarkerColor(kBlack);
  h_S_eff->SetMarkerColor(kGreen+1);
  h_CombiWithConvLeg_eff->SetMarkerColor(kRed);
  h_CombiWithoutConvLeg_eff->SetMarkerColor(kBlue);
  h_HF_eff->SetMarkerColor(kOrange);
  h_RPConv_eff->SetMarkerColor(13);

  TCanvas *c = new TCanvas("c","",800,600);
  c->SetLogy();
  c->SetGridy();
  h_SB->Draw("hist e x0");
  h_S->Draw("hist e x0 same");
  h_CombiWithConvLeg->Draw("hist e x0 same");
  h_CombiWithoutConvLeg->Draw("hist e x0 same");
  h_HF->Draw("hist e x0 same");
  h_RPConv->Draw("hist e x0 same");

  h_SB_MVAcut->Draw("e1 x0 same");
  h_S_MVAcut->Draw("e1 x0 same");
  h_CombiWithConvLeg_MVAcut->Draw("e1 x0 same");
  h_CombiWithoutConvLeg_MVAcut->Draw("e1 x0 same");
  h_HF_MVAcut->Draw("e1 x0 same");
  h_RPConv_MVAcut->Draw("e1 x0 same");

  
  TLegend *leg = new TLegend(.6,.8,.95,.95);
  leg->AddEntry(h_SB,"S+B","l");
  leg->AddEntry(h_S,"S","l");
  leg->AddEntry(h_CombiWithConvLeg,"Comb. w. conv. leg","l");
  leg->AddEntry(h_CombiWithoutConvLeg,"Comb. w/o conv. leg","l");
  leg->AddEntry(h_HF,"Comb. HF","l");
  leg->AddEntry(h_RPConv,"RP conv.","l");
  leg->Draw();

  TLatex l;
  l.SetTextSize(.025);
  l.DrawLatex(.1,1e6+1e4,h_text);

  c->SaveAs("temp_output/mass.pdf");
  c->SaveAs("temp_output/mass.root");

  TCanvas *c_eff = new TCanvas("c_eff","",800,600);
  c_eff->SetGridy();
  h_SB_eff->SetXTitle("M_{ee} / (GeV/c^{2})");
  h_SB_eff->SetYTitle("Efficiency");
  h_SB_eff->GetXaxis()->SetTitleOffset(1.2);
  h_SB_eff->GetYaxis()->SetTitleOffset(1.3);
  h_SB_eff->GetYaxis()->SetRangeUser(0,1.1);
  h_SB_eff->Draw("e1 x0");
  h_HF_eff->Draw("e1 x0 same");
  h_S_eff->Draw("e1 x0 same");
  h_CombiWithConvLeg_eff->Draw("e1 x0 same");
  h_CombiWithoutConvLeg_eff->Draw("e1 x0 same");
  h_RPConv_eff->Draw("e1 x0 same");
  leg->Draw();
  l.DrawLatex(.1,1.125,h_text);

  c_eff->SaveAs("temp_output/mass_eff.pdf");
  c_eff->SaveAs("temp_output/mass_eff.root");
}
