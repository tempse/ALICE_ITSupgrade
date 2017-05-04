#include <iostream>
#include <cstdlib>

#include <TROOT.h>
#include <TApplication.h>
#include <TStyle.h>
#include <TString.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TStopwatch.h>
#include <TMath.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLatex.h>


void PlotMass_prefilterCuts() {

  // File containing the input pairtree (test) data:
  TString fileName_testData = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/analysis_singleConvTrackRejMVAcuts/applicationPhase1/FT2_AnalysisResults_Upgrade_addFeat_pairtree_us_part2_1-9-split.root";
  
  TString h_text = "prefilter: RP conv. rej. via MVA cuts";

  // two tags are combined via
  // (tag1 == wantedPrefilterTagValue1 && tag2 == wantedPrefilterTagValue2)
  // if the following variable is set to kTRUE:
  Bool_t useTwoTags = kFALSE;

  TString prefilterTagName1 = "IsTaggedRPConv_MVAcuts_prefilter";
  TString prefilterTagName2 = "IsTaggedConvTrack2";

  // After prefiltering, use events with this tag value only:
  const Int_t wantedPrefilterTagValue1 = 0.;
  const Int_t wantedPrefilterTagValue2 = 0.;
  
  // set the used MVA method:
  const Bool_t isMLP = kFALSE;
  const Bool_t isBDT = kTRUE;

  
  // ROOT file name containing all created histograms:
  TString outfileName = "temp_output/mass_histos_prefilterCuts.root";

  
  /////////////////////////////////////////////////////////////////////////

  
  if( !(wantedPrefilterTagValue1==0 || wantedPrefilterTagValue1==1) ) {
    std::cout << "   ERROR: Variable 'wantedPrefilterTagValue1' is required to have values "
	      << " 0 or 1, but has value " << wantedPrefilterTagValue1 << " instead."
	      << std::endl;
    gApplication->Terminate();
  }
  
  if(isMLP & isBDT) {
    std::cout << "  ERROR: Cannot use both MLP and BDT output." << std::endl;
    exit(1);
  }


  TFile *f = new TFile(fileName_testData,"READ");
  TTree *TestTree = (TTree*)f->Get("pairTree_us");

  Float_t mass;
  Int_t IsRP, IsConv, IsHF;
  Int_t IsTag1, IsTag2; // generic variables whose values depend on the particular analysis
  TestTree->SetBranchAddress("mass", &mass);
  TestTree->SetBranchAddress("IsRP", &IsRP);
  TestTree->SetBranchAddress("IsConv", &IsConv);
  TestTree->SetBranchAddress("IsHF", &IsHF);
  if(TestTree->GetListOfBranches()->FindObject(prefilterTagName1) != NULL) {
    TestTree->SetBranchAddress(prefilterTagName1, &IsTag1);
  }else {
    std::cout << "  ERROR: The branch " << prefilterTagName1
	      << " does not exist in the file " << f->GetName() << std::endl;
    gApplication->Terminate();
  }
  if(useTwoTags) {
    if(TestTree->GetListOfBranches()->FindObject(prefilterTagName2) != NULL) {
      TestTree->SetBranchAddress(prefilterTagName2, &IsTag2);
    }else {
      std::cout << "  ERROR: The branch " << prefilterTagName2
		<< " does not exist in the file " << f->GetName() << std::endl;
      gApplication->Terminate();
    }
  }

  
  const unsigned int min=0, max=5, nBins=50;

  
  TH1D *h_SB = new TH1D("h_SB","",nBins,min,max);
  
  TH1D *h_S = new TH1D("h_S","",nBins,min,max);
  
  TH1D *h_CombiWithConvLeg = new TH1D("h_CombiWithConvLeg","",nBins,min,max);
  
  TH1D *h_CombiWithoutConvLeg =
    new TH1D("h_CombiWithouthConvLeg","",nBins,min,max);
  
  TH1D *h_HF = new TH1D("h_HF","",nBins,min,max);
  
  TH1D *h_RPConv = new TH1D("h_RPConv","",nBins,min,max);
  

  TH1D *h_S_prefilterCut = new TH1D("h_S_prefilterCut","",nBins,min,max);
  
  TH1D *h_SB_prefilterCut = new TH1D("h_SB_prefilterCut","",nBins,min,max);
  
  TH1D *h_CombiWithConvLeg_prefilterCut =
    new TH1D("h_CombiWithConvLeg_prefilterCut","",nBins,min,max);
  
  TH1D *h_CombiWithoutConvLeg_prefilterCut =
    new TH1D("h_CombiWithoutConvLeg_prefilterCut","",nBins,min,max);
  
  TH1D *h_HF_prefilterCut = new TH1D("h_HF_prefilterCut","",nBins,min,max);
  
  TH1D *h_RPConv_prefilterCut =
    new TH1D("h_RPConv_prefilterCut","",nBins,min,max);



  Long64_t nEv = TestTree->GetEntries();

  
  Float_t passed_seconds_prev = 0.;
  
  TStopwatch *watch = new TStopwatch();

  watch->Start();


  for(Long64_t ev=0; ev<nEv; ev++) {

    if((ev%10000)==0) std::cout << "\rProcessing entry " << ev << " of "
				<< nEv << " (" << ev*100/nEv << "%)...";
      
    TestTree->GetEvent(ev);
    
    h_SB->Fill(mass);
    if(IsRP==1 && IsConv==0) {
      h_S->Fill(mass);
    }
    if(IsRP==0 && IsConv==1) {
      h_CombiWithConvLeg->Fill(mass);
    }
    if(IsRP==0 && IsConv==0) {
      h_CombiWithoutConvLeg->Fill(mass);
    }
    if(IsRP==0 && IsHF==1) {
      h_HF->Fill(mass);
    }
    if(IsRP==1 && IsConv==1) {
      h_RPConv->Fill(mass);
    }


    
    // (IsTag == 0) if "unwanted tag" corresponds to 1
    // in the input file, (IsTag == 1) otherwise:

    if(!useTwoTags) {
      if(IsTag1 == wantedPrefilterTagValue1) {
	h_SB_prefilterCut->Fill(mass);
	if(IsRP==1 && IsConv==0) {
	  h_S_prefilterCut->Fill(mass);
	}
	if(IsRP==0 && IsConv==1) {
	  h_CombiWithConvLeg_prefilterCut->Fill(mass);
	}
	if(IsRP==0 && IsConv==0) {
	  h_CombiWithoutConvLeg_prefilterCut->Fill(mass);
	}
	if(IsRP==0 && IsHF==1) {
	  h_HF_prefilterCut->Fill(mass);
	}
	if(IsRP==1 && IsConv==1) {
	  h_RPConv_prefilterCut->Fill(mass);
	}
      }
    }
    
    if(useTwoTags) {
      if(IsTag1 == wantedPrefilterTagValue1 &&
	 IsTag2 == wantedPrefilterTagValue2) {
	h_SB_prefilterCut->Fill(mass);
	if(IsRP==1 && IsConv==0) {
	  h_S_prefilterCut->Fill(mass);
	}
	if(IsRP==0 && IsConv==1) {
	  h_CombiWithConvLeg_prefilterCut->Fill(mass);
	}
	if(IsRP==0 && IsConv==0) {
	  h_CombiWithoutConvLeg_prefilterCut->Fill(mass);
	}
	if(IsRP==0 && IsHF==1) {
	  h_HF_prefilterCut->Fill(mass);
	}
	if(IsRP==1 && IsConv==1) {
	  h_RPConv_prefilterCut->Fill(mass);
	}
      }
    }
    
  } // end ev loop

   
  std::cout << std::endl << std::endl;
  std::cout << "Time elapsed since begin of processing: " << std::endl;
  std::cout << "\t";
  watch->Print();
  std::cout << std::endl;
  watch->Stop();



  
  //////////////////// SETTING UP THE PLOTS:  

  
  gStyle->SetOptStat(0);

  TH1D *h_SB_eff = (TH1D*)h_SB_prefilterCut->Clone();
  TH1D *h_S_eff = (TH1D*)h_S_prefilterCut->Clone();
  TH1D *h_CombiWithConvLeg_eff = (TH1D*)h_CombiWithConvLeg_prefilterCut->Clone();
  TH1D *h_CombiWithoutConvLeg_eff = (TH1D*)h_CombiWithoutConvLeg_prefilterCut->Clone();
  TH1D *h_HF_eff = (TH1D*)h_HF_prefilterCut->Clone();
  TH1D *h_RPConv_eff = (TH1D*)h_RPConv_prefilterCut->Clone();
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


  h_SB->SetLineColor(kBlack);
  h_S->SetLineColor(kGreen+1);
  h_CombiWithConvLeg->SetLineColor(kRed);
  h_CombiWithoutConvLeg->SetLineColor(kBlue);
  h_HF->SetLineColor(kOrange);
  h_RPConv->SetLineColor(kMagenta);

  h_SB_prefilterCut->SetLineColor(kBlack);
  h_SB_prefilterCut->SetMarkerColor(kBlack);
  h_S_prefilterCut->SetLineColor(kGreen+1);
  h_S_prefilterCut->SetMarkerColor(kGreen+1);
  h_CombiWithConvLeg_prefilterCut->SetLineColor(kRed);
  h_CombiWithConvLeg_prefilterCut->SetMarkerColor(kRed);
  h_CombiWithoutConvLeg_prefilterCut->SetLineColor(kBlue);
  h_CombiWithoutConvLeg_prefilterCut->SetMarkerColor(kBlue);
  h_HF_prefilterCut->SetLineColor(kOrange);
  h_HF_prefilterCut->SetMarkerColor(kOrange);
  h_RPConv_prefilterCut->SetLineColor(kMagenta);
  h_RPConv_prefilterCut->SetMarkerColor(kMagenta);
  
  h_SB_prefilterCut->SetMarkerStyle(7);
  h_S_prefilterCut->SetMarkerStyle(7);
  h_CombiWithConvLeg_prefilterCut->SetMarkerStyle(7);
  h_CombiWithoutConvLeg_prefilterCut->SetMarkerStyle(7);
  h_HF_prefilterCut->SetMarkerStyle(7);
  h_RPConv_prefilterCut->SetMarkerStyle(7);

  h_SB->GetYaxis()->SetRangeUser(1e-1,1e8);
  h_SB->SetXTitle("M_{ee} / (GeV/c^{2})");
  h_SB->SetYTitle("Entries");
  h_SB->GetXaxis()->SetTitleOffset(1.2);
  h_SB->GetYaxis()->SetTitleOffset(1.3);

  h_SB_eff->SetLineColor(kBlack);
  h_S_eff->SetLineColor(kGreen+1);
  h_CombiWithConvLeg_eff->SetLineColor(kRed);
  h_CombiWithoutConvLeg_eff->SetLineColor(kBlue);
  h_HF_eff->SetLineColor(kOrange);
  h_RPConv_eff->SetLineColor(kMagenta);

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
  h_RPConv_eff->SetMarkerColor(kMagenta);


  TCanvas *c = new TCanvas("c","",800,600);
  c->SetLogy();
  c->SetGridy();
  h_SB->Draw("hist e x0");
  h_S->Draw("hist e x0 same");
  h_CombiWithConvLeg->Draw("hist e x0 same");
  h_CombiWithoutConvLeg->Draw("hist e x0 same");
  h_HF->Draw("hist e x0 same");
  h_RPConv->Draw("hist e x0 same");

  h_SB_prefilterCut->Draw("e1 x0 same");
  h_S_prefilterCut->Draw("e1 x0 same");
  h_CombiWithConvLeg_prefilterCut->Draw("e1 x0 same");
  h_CombiWithoutConvLeg_prefilterCut->Draw("e1 x0 same");
  h_HF_prefilterCut->Draw("e1 x0 same");
  h_RPConv_prefilterCut->Draw("e1 x0 same");

  
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
  l.DrawLatex(.1,1.5e7,h_text);


  c->SaveAs("temp_output/mass_prefilterCuts.pdf");
  c->SaveAs("temp_output/mass_prefilterCuts.root");
  c->SaveAs("temp_output/mass_prefilterCuts.png");



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

  c_eff->SaveAs("temp_output/mass_eff_prefilterCuts.pdf");
  c_eff->SaveAs("temp_output/mass_eff_prefilterCuts.root");
  c_eff->SaveAs("temp_output/mass_eff_prefilterCuts.png");



  // store all created histograms in a ROOT file:
  TFile *outfile = new TFile(outfileName, "RECREATE");

  h_SB->Write(0, TObject::kOverwrite);
  h_S->Write(0, TObject::kOverwrite);
  h_CombiWithConvLeg->Write(0, TObject::kOverwrite);
  h_CombiWithoutConvLeg->Write(0, TObject::kOverwrite);
  h_HF->Write(0, TObject::kOverwrite);
  h_RPConv->Write(0, TObject::kOverwrite);

  h_SB_prefilterCut->Write(0, TObject::kOverwrite);
  h_S_prefilterCut->Write(0, TObject::kOverwrite);
  h_CombiWithConvLeg_prefilterCut->Write(0, TObject::kOverwrite);
  h_CombiWithoutConvLeg_prefilterCut->Write(0, TObject::kOverwrite);
  h_HF_prefilterCut->Write(0, TObject::kOverwrite);
  h_RPConv_prefilterCut->Write(0, TObject::kOverwrite);

  h_SB_eff->Write(0, TObject::kOverwrite);
  h_S_eff->Write(0, TObject::kOverwrite);
  h_CombiWithConvLeg_eff->Write(0, TObject::kOverwrite);
  h_CombiWithoutConvLeg_eff->Write(0, TObject::kOverwrite);
  h_HF_eff->Write(0, TObject::kOverwrite);
  h_RPConv_eff->Write(0, TObject::kOverwrite);
  
  outfile->Close();
  
  // gApplication->Terminate();
}
