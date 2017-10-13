void printElement(TString name, Float_t num_total, Float_t num_noPrefilter, Float_t num_prefilter,
		  const Int_t &nameWidth, const Int_t &numWidth, const char &separator) {
  std::cout << std::left << std::setw(numWidth) << std::setfill(separator) << name;
  std::cout << std::right  << std::setw(numWidth) << std::setfill(separator) << num_total;
  std::cout << std::right << std::setw(numWidth) << std::setfill(separator) << num_noPrefilter;
  std::cout << std::left << " (" << num_noPrefilter/(num_total*1.0) << "%)";
  std::cout << std::right << std::setw(numWidth) << std::setfill(separator) << num_prefilter;
  std::cout << std::left << " (" << num_prefilter/(num_total*1.0) << "%)";
  std::cout << std::endl;
}

Float_t getPairPIDefficiency(Float_t, Float_t, TH1D&);


void GetNumberOfPairs_prefilterCutDependent(TString infile_name,
					    TString infile_tree_name,
					    Bool_t doConsiderPIDefficiencies = kTRUE) {

  TFile *infile = new TFile(infile_name, "READ");
  TTree *infile_tree = (TTree*)infile->Get(infile_tree_name);

  Int_t IsRP, IsConv, IsCorrHF;
  Int_t IsTaggedRPConv, IsTaggedRPConv_prefilter;
  Float_t pt1, pt2;
  Bool_t containsTrackCutInfo = kTRUE;
  Int_t TrackCut1, TrackCut2;

  infile_tree->SetBranchAddress("IsRP", &IsRP);
  infile_tree->SetBranchAddress("IsConv", &IsConv);
  infile_tree->SetBranchAddress("IsCorrHF", &IsCorrHF);
  infile_tree->SetBranchAddress("IsTaggedRPConv_classicalCuts", &IsTaggedRPConv);
  infile_tree->SetBranchAddress("IsTaggedRPConv_classicalCuts_prefilter", &IsTaggedRPConv_prefilter);
  infile_tree->SetBranchAddress("pt1", &pt1);
  infile_tree->SetBranchAddress("pt2", &pt2);
  if(infile_tree->GetListOfBranches()->FindObject("TrackCut1") != NULL &&
     infile_tree->GetListOfBranches()->FindObject("TrackCut2") != NULL) {
    infile_tree->SetBranchAddress("TrackCut1", &TrackCut1);
    infile_tree->SetBranchAddress("TrackCut2", &TrackCut2);
  }else {
    std::cout << "  Info: No branch holding track cut information found. "
	      << "All tracks will be processed." << std::endl;
    containsTrackCutInfo = kFALSE;
  }

  // File containing the pt-dependent PID efficiencies:
  TString infile_PIDefficiencies_name = "~/analysis/data/FT2_AnalysisResults_Upgrade/inputData/ITSU_PIDefficiency_lowB.root";
  TFile *infile_PIDefficiencies = new TFile(infile_PIDefficiencies_name, "READ");
  TH1D *h_PIDeff = (TH1D*)infile_PIDefficiencies->Get("efficiencyLHC17d12_TPCandTOF3sigma");
  h_PIDeff->GetXaxis()->SetRangeUser(0,5);

  TH1D *h_sample_weight = new TH1D("h_sample_weight","",1000,0,1);
  
  
  Double_t cnt_signal = 0.,
    cnt_combWithConv = 0.,
    cnt_combWithoutConv = 0.,
    cnt_RPConv = 0.,
    cnt_corrHF = 0.;
  Double_t cnt_signal_tagged = 0.,
    cnt_combWithConv_tagged = 0.,
    cnt_combWithoutConv_tagged = 0.,
    cnt_RPConv_tagged = 0.,
    cnt_corrHF_tagged = 0.;
  Double_t cnt_signal_prefilterTagged = 0.,
    cnt_combWithConv_prefilterTagged = 0.,
    cnt_combWithoutConv_prefilterTagged = 0.,
    cnt_RPConv_prefilterTagged = 0.,
    cnt_corrHF_prefilterTagged = 0.;
  
  
  Long64_t nentries = infile_tree->GetEntries();
    
  for(Long64_t i=0; i<nentries; i++) {
    if((i%10000)==0) {
      std::cout << "\rProcessing entry " << i << " of " << nentries
		<< " (" << i*100/(nentries*1.0) << "%)...";
    }
    
    infile_tree->GetEntry(i);

    if(TrackCut1 != 2 || TrackCut2 != 2) continue;
    
    Double_t pairWeight = doConsiderPIDefficiencies ? getPairPIDefficiency(pt1, pt2, *h_PIDeff) : 1.;

    if(IsRP==1 && IsConv==0) cnt_signal += pairWeight;
    if(IsRP==0 && IsConv==1) cnt_combWithConv += pairWeight;
    if(IsRP==0 && IsConv==0) cnt_combWithoutConv += pairWeight;
    if(IsRP==1 && IsConv==1) cnt_RPConv += pairWeight;
    if(IsCorrHF==1) cnt_corrHF += pairWeight;

    if(IsRP==1 && IsConv==0 && IsTaggedRPConv) cnt_signal_tagged += pairWeight;
    if(IsRP==0 && IsConv==1 && IsTaggedRPConv) cnt_combWithConv_tagged += pairWeight;
    if(IsRP==0 && IsConv==0 && IsTaggedRPConv) cnt_combWithoutConv_tagged += pairWeight;
    if(IsRP==1 && IsConv==1 && IsTaggedRPConv) cnt_RPConv_tagged += pairWeight;
    if(IsCorrHF==1 && IsTaggedRPConv) cnt_corrHF_tagged += pairWeight;
    
    if(IsRP==1 && IsConv==0 && IsTaggedRPConv_prefilter) cnt_signal_prefilterTagged += pairWeight;
    if(IsRP==0 && IsConv==1 && IsTaggedRPConv_prefilter) cnt_combWithConv_prefilterTagged += pairWeight;
    if(IsRP==0 && IsConv==0 && IsTaggedRPConv_prefilter) cnt_combWithoutConv_prefilterTagged += pairWeight;
    if(IsRP==1 && IsConv==1 && IsTaggedRPConv_prefilter) cnt_RPConv_prefilterTagged += pairWeight;
    if(IsCorrHF==1 && IsTaggedRPConv_prefilter) cnt_corrHF_prefilterTagged += pairWeight;
  }
  std::cout << "\rProcessing entry " << nentries << " of " << nentries
	    << " (100%)...";

  const char separator = ' ';
  const Int_t nameWidth = 2;
  const Int_t numWidth = 18;

  std::cout << std::endl;

  std::cout << std::right << std::setw(45) << std::setfill(separator)
	    << "total  |  " << "tagged before prefilter  |  " << "tagged after prefilter" << std::endl;
  printElement("Signal", cnt_signal, cnt_signal_tagged, cnt_signal_prefilterTagged, nameWidth, numWidth, separator);
  printElement("Comb. w/ conv.", cnt_combWithConv, cnt_combWithConv_tagged, cnt_combWithConv_prefilterTagged, nameWidth, numWidth, separator);
  printElement("Comb. w/o conv.", cnt_combWithoutConv, cnt_combWithoutConv_tagged, cnt_combWithoutConv_prefilterTagged, nameWidth, numWidth, separator);
  printElement("RP conv.", cnt_RPConv, cnt_RPConv_tagged, cnt_RPConv_prefilterTagged, nameWidth, numWidth, separator);
  printElement("Corr. HF", cnt_corrHF, cnt_corrHF_tagged, cnt_corrHF_prefilterTagged, nameWidth, numWidth, separator);

  /*
  std::cout << std::endl;
  // std::cout << std::setw(vspace*2) << "total" << std::setfill(' ') << std::setw(vspace) << "tagged \t\ttagged" << std::endl;
  
  std::cout << "\t\t\t\t(no prefilter)\t\t(prefilter)" << std::endl << std::endl;
  
  std::cout << "signal\t\t\t" << cnt_signal << "\t\t"
	    << cnt_signal_tagged << " (" << cnt_signal_tagged*100/(cnt_signal*1.0) << "%)\t\t"
	    << cnt_signal_prefilterTagged << " (" << cnt_signal_prefilterTagged*100/(cnt_signal*1.0) << "%)" << std::endl;
  
  std::cout << "comb. w/ conv.\t\t" << cnt_combWithConv << "\t\t"
	    << cnt_combWithConv_tagged << " (" << cnt_combWithConv_tagged*100/(cnt_combWithConv*1.0) << "%)\t\t"
	    << cnt_combWithConv_prefilterTagged << " (" << cnt_combWithConv_prefilterTagged*100/(cnt_combWithConv*1.0) << ")" << std::endl;
  
  std::cout << "comb. w/o conv.\t\t" << cnt_combWithoutConv << "\t\t"
	    << cnt_combWithoutConv_tagged << " (" << cnt_combWithoutConv_tagged*100/(cnt_combWithoutConv*1.0) << "%)\t\t"
	    << cnt_combWithoutConv_prefilterTagged << " (" << cnt_combWithoutConv_prefilterTagged*100/(cnt_combWithoutConv*1.0) << "%)" << std::endl;
  
  std::cout << "RP conv.\t\t" << cnt_RPConv << "\t\t"
	    << cnt_RPConv_tagged << " (" << cnt_RPConv_tagged*100/(cnt_RPConv*1.0) << "%)\t\t"
	    << cnt_RPConv_prefilterTagged << " (" << cnt_RPConv_prefilterTagged*100/(cnt_RPConv*1.0) << "%)" << std::endl;
  
  std::cout << "corr. HF\t\t" << cnt_corrHF << "\t\t"
	    << cnt_corrHF_tagged << " (" << cnt_corrHF_tagged*100/(cnt_corrHF*1.0) << "%)\t\t"
	    << cnt_corrHF_prefilterTagged << " (" << cnt_corrHF_prefilterTagged*100/(cnt_corrHF*1.0) << "%)" << std::endl;
  */

  infile->Close();
  gSystem->Exit(0);
}


Float_t getPairPIDefficiency(Float_t pt1, Float_t pt2, TH1D &h_PIDeff) {

  Float_t PIDeff1 = (pt1 >= h_PIDeff.GetBinLowEdge(h_PIDeff.GetNbinsX())) ?
    h_PIDeff.GetBinContent(h_PIDeff.GetNbinsX()) : h_PIDeff.GetBinContent(h_PIDeff.FindBin(pt1));

  Float_t PIDeff2 = (pt2 >= h_PIDeff.GetBinLowEdge(h_PIDeff.GetNbinsX())) ?
    h_PIDeff.GetBinContent(h_PIDeff.GetNbinsX()) : h_PIDeff.GetBinContent(h_PIDeff.FindBin(pt2));

  return PIDeff1 * PIDeff2;
  
}
