#include <iostream>

#include <TROOT.h>
#include <TStyle.h>
#include <TString.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLatex.h>

void PlotMvaOutput() {
  TString fileName = "../pairTrees/FT2_AnalysisResults_Upgrade_all-Ev_pairtree_us/FT2_AnalysisResults_Upgrade_all-Ev_pairtree_us_test_1-100-split.root";
  
  TString fileName_MVAoutput = "../pairTrees/FT2_AnalysisResults_Upgrade_all-Ev_pairtree_us/TMVApp_test.root";

  TString outfileName = "temp_output/MVAoutput.root";
  
  TString h_text = "Combinatorial MLP";

  
  const Float_t massCut_lower=0.1;
  // const Float_t massCut_upper=1.1;
  
  TFile *infile = new TFile(fileName,"READ");
  TTree *TestTree = (TTree*)infile->Get("pairTree_us");
  
  Float_t mass;
  Int_t motherPdg1, motherPdg2;
  Int_t IsRP, IsConv, IsHF;
  TestTree->SetBranchAddress("IsRP",&IsRP);
  TestTree->SetBranchAddress("IsConv",&IsConv);
  TestTree->SetBranchAddress("IsHF",&IsHF);
  TestTree->SetBranchAddress("motherPdg1",&motherPdg1);
  TestTree->SetBranchAddress("motherPdg2",&motherPdg2);
  TestTree->SetBranchAddress("mass",&mass);


  Float_t MLP;
  
  TFile *infile_MVAoutput = new TFile(fileName_MVAoutput,"READ");
  TTree *MVAoutputTree = (TTree*)infile_MVAoutput->Get("pairTree_MVAoutput");
  MVAoutputTree->SetBranchAddress("MLP",&MLP);


    
  if(TestTree->GetEntries() != MVAoutputTree->GetEntries()) {
    std::cout << "   ERROR: The trees of the input files have different sizes."
	      << std::endl;
    std::cout << "   Size of tree in file " << fileName << ": "
	      << TestTree->GetEntries() << std::endl;
    std::cout << "   Size of tree in file " << fileName_MVAoutput << ": "
	      << MVAoutputTree->GetEntries() << std::endl;
    return;
  }

  
  unsigned int min=0, max=1, nBins=100;
  
  TH1F *h_SB = new TH1F("h_SB","",nBins,min,max);
  
  TH1F *h_S = new TH1F("h_S","",nBins,min,max);
  
  TH1F *h_CombiWithConvLeg = new TH1F("h_CombiWithConvLeg","",nBins,min,max);
  
  TH1F *h_CombiWithoutConvLeg =
    new TH1F("h_CombiWithoutConvLeg","",nBins,min,max);
  
  TH1F *h_RPConv = new TH1F("h_RPConv","",nBins,min,max);

  TH1F *h_HF = new TH1F("h_HF","",nBins,min,max);


  
  Long64_t nEv = TestTree->GetEntries();
  
  for(Long64_t ev=0; ev<nEv; ev++) {
    TestTree->GetEvent(ev);
    MVAoutputTree->GetEvent(ev);

    if((ev%10000)==0) std::cout << "\rProcessing entry " << ev << " of "
				<< nEv << " (" << ev*100/nEv << "%)...";

    if(mass < massCut_lower) continue;
    
    h_SB->Fill(MLP);
    if(IsRP==1 && IsConv==0) {
      h_S->Fill(MLP);
    }
    if(IsRP==0 && (motherPdg1==22 || motherPdg2==22)) {
      h_CombiWithConvLeg->Fill(MLP);
    }
    if(IsRP==0 && !(motherPdg1==22 || motherPdg2==22)) {
      h_CombiWithoutConvLeg->Fill(MLP);
    }
    if(IsRP==0 && IsHF==1) {
      h_HF->Fill(MLP);
    }
    if(IsRP==1 && IsConv==1) {
      h_RPConv->Fill(MLP);
    }
  }

  std::cout << "\rProcessing entry "
	    << nEv << " of " << nEv << " (100%)... DONE." << std::endl;

  
  h_SB->SetLineColor(kBlack);
  h_S->SetLineColor(kGreen+1);
  h_CombiWithConvLeg->SetLineColor(kRed);
  h_CombiWithoutConvLeg->SetLineColor(kBlue);
  h_HF->SetLineColor(kOrange);
  h_RPConv->SetLineColor(13);

  h_SB->SetXTitle("MLP");
  h_SB->SetYTitle("Entries");
  h_SB->GetXaxis()->SetTitleOffset(1.2);
  h_SB->GetYaxis()->SetTitleOffset(1.3);

  h_S->SetXTitle("MLP");
  h_S->SetYTitle("Entries");
  h_S->GetXaxis()->SetTitleOffset(1.2);
  h_S->GetYaxis()->SetTitleOffset(1.3);

  h_CombiWithConvLeg->SetXTitle("MLP");
  h_CombiWithConvLeg->SetYTitle("Entries");
  h_CombiWithConvLeg->GetXaxis()->SetTitleOffset(1.2);
  h_CombiWithConvLeg->GetYaxis()->SetTitleOffset(1.3);

  h_CombiWithoutConvLeg->SetXTitle("MLP");
  h_CombiWithoutConvLeg->SetYTitle("Entries");
  h_CombiWithoutConvLeg->GetXaxis()->SetTitleOffset(1.2);
  h_CombiWithoutConvLeg->GetYaxis()->SetTitleOffset(1.3);

  h_HF->SetXTitle("MLP");
  h_HF->SetYTitle("Entries");
  h_HF->GetXaxis()->SetTitleOffset(1.2);
  h_HF->GetYaxis()->SetTitleOffset(1.3);

  h_RPConv->SetXTitle("MLP");
  h_RPConv->SetYTitle("Entries");
  h_RPConv->GetXaxis()->SetTitleOffset(1.2);
  h_RPConv->GetYaxis()->SetTitleOffset(1.3);


  TFile *outfile = new TFile(outfileName, "RECREATE");  


  // write histograms to output file:
  h_SB->SetDirectory(outfile);
  h_S->SetDirectory(outfile);
  h_CombiWithConvLeg->SetDirectory(outfile);
  h_CombiWithoutConvLeg->SetDirectory(outfile);
  h_HF->SetDirectory(outfile);
  h_RPConv->SetDirectory(outfile);
  
  outfile->Write();

    
  TCanvas *c = new TCanvas("c","",800,600);
  gStyle->SetOptStat(0);
  c->SetLogy();
  c->SetGridy();
  h_SB->GetYaxis()->SetRangeUser(.5,1e9);
  h_SB->Draw("hist");
  h_S->Draw("hist same");
  h_CombiWithConvLeg->Draw("hist same");
  h_CombiWithoutConvLeg->Draw("hist same");
  h_HF->Draw("hist same");
  h_RPConv->Draw("hist same");

  gStyle->SetOptStat(0);
  
  TLegend *leg = new TLegend(.55,.8,.95,.95);
  leg->AddEntry(h_SB,"S+B","l");
  leg->AddEntry(h_S,"S","l");
  leg->AddEntry(h_CombiWithConvLeg,"Comb. w. conv. leg");
  leg->AddEntry(h_CombiWithoutConvLeg,"Comb. w/o conv. leg");
  leg->AddEntry(h_HF,"Comb. HF","l");
  leg->AddEntry(h_RPConv,"RP conv.","l");
  leg->Draw();

  TLatex l;
  l.SetTextSize(.025);
  l.DrawLatex(.1,1e8+1e3,h_text);

  c->Write();

  outfile->Close();
  
}
