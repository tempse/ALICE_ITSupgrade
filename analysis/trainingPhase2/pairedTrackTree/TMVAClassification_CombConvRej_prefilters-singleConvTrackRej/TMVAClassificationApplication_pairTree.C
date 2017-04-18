/**********************************************************************************
 * Project   : TMVA - a Root-integrated toolkit for multivariate data analysis    *
 * Package   : TMVA                                                               *
 * Exectuable: TMVAClassificationApplication                                      *
 *                                                                                *
 * This macro provides a simple example on how to use the trained classifiers     *
 * within an analysis module                                                      *
 **********************************************************************************/

#include <cstdlib>
#include <vector>
#include <iostream>
#include <map>
#include <string>

#include "TApplication.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TMath.h"
#include "TStopwatch.h"

#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include "TMVA/MethodCuts.h"

using namespace TMVA;

void TMVAClassificationApplication_pairTree( TString myMethodList = "" ) 
{   

  TString problematicRootVersion = "6.06/04";
  if(gROOT->GetVersion() == problematicRootVersion) {
    std::cout << std::endl << "!!!!!  WARNING: You are using ROOT "
	      << gROOT->GetVersion() << " - errors might be encountered with this version!"
	      << std::endl << std::endl;
  }
  
   //---------------------------------------------------------------

   // This loads the library
   TMVA::Tools::Instance();

   // Default MVA methods to be trained + tested
   std::map<std::string,int> Use;

   // --- Cut optimisation
   Use["Cuts"]            = 0;
   Use["CutsD"]           = 0;
   Use["CutsPCA"]         = 0;
   Use["CutsGA"]          = 0;
   Use["CutsSA"]          = 0;
   // 
   // --- 1-dimensional likelihood ("naive Bayes estimator")
   Use["Likelihood"]      = 0;
   Use["LikelihoodD"]     = 0; // the "D" extension indicates decorrelated input variables (see option strings)
   Use["LikelihoodPCA"]   = 0; // the "PCA" extension indicates PCA-transformed input variables (see option strings)
   Use["LikelihoodKDE"]   = 0;
   Use["LikelihoodMIX"]   = 0;
   //
   // --- Mutidimensional likelihood and Nearest-Neighbour methods
   Use["PDERS"]           = 0;
   Use["PDERSD"]          = 0;
   Use["PDERSPCA"]        = 0;
   Use["PDEFoam"]         = 0;
   Use["PDEFoamBoost"]    = 0; // uses generalised MVA method boosting
   Use["KNN"]             = 0; // k-nearest neighbour method
   //
   // --- Linear Discriminant Analysis
   Use["LD"]              = 0; // Linear Discriminant identical to Fisher
   Use["Fisher"]          = 0;
   Use["FisherG"]         = 0;
   Use["BoostedFisher"]   = 0; // uses generalised MVA method boosting
   Use["HMatrix"]         = 0;
   //
   // --- Function Discriminant analysis
   Use["FDA_GA"]          = 0; // minimisation of user-defined function using Genetics Algorithm
   Use["FDA_SA"]          = 0;
   Use["FDA_MC"]          = 0;
   Use["FDA_MT"]          = 0;
   Use["FDA_GAMT"]        = 0;
   Use["FDA_MCMT"]        = 0;
   //
   // --- Neural Networks (all are feed-forward Multilayer Perceptrons)
   Use["MLP"]             = 0; // Recommended ANN
   Use["MLPBFGS"]         = 0; // Recommended ANN with optional training method
   Use["MLPBNN"]          = 0; // Recommended ANN with BFGS training method and bayesian regulator
   Use["CFMlpANN"]        = 0; // Depreciated ANN from ALEPH
   Use["TMlpANN"]         = 0; // ROOT's own ANN
   //
   // --- Support Vector Machine 
   Use["SVM"]             = 0;
   // 
   // --- Boosted Decision Trees
   Use["BDT"]             = 1; // uses Adaptive Boost
   Use["BDTG"]            = 0; // uses Gradient Boost
   Use["BDTB"]            = 0; // uses Bagging
   Use["BDTD"]            = 0; // decorrelation + Adaptive Boost
   // 
   // --- Friedman's RuleFit method, ie, an optimised series of cuts ("rules")
   Use["RuleFit"]         = 0;
   // ---------------------------------------------------------------
   Use["Plugin"]          = 0;
   Use["Category"]        = 0;
   Use["SVM_Gauss"]       = 0;
   Use["SVM_Poly"]        = 0;
   Use["SVM_Lin"]         = 0;

   std::cout << std::endl;
   std::cout << "==> Start TMVAClassificationApplication" << std::endl;

   // Select methods (don't look at this code - not of interest)
   if (myMethodList != "") {
      for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) it->second = 0;

      std::vector<TString> mlist = gTools().SplitString( myMethodList, ',' );
      for (UInt_t i=0; i<mlist.size(); i++) {
         std::string regMethod(mlist[i]);

         if (Use.find(regMethod) == Use.end()) {
            std::cout << "Method \"" << regMethod 
                      << "\" not known in TMVA under this name. Choose among the following:" << std::endl;
            for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) {
               std::cout << it->first << " ";
            }
            std::cout << std::endl;
            return;
         }
         Use[regMethod] = 1;
      }
   }

   // --------------------------------------------------------------------------------------------------

   // --- Create the Reader object

   TMVA::Reader *reader = new TMVA::Reader( "Color:!Silent" );    

   // Create a set of variables and declare them to the reader
   // - the variable names MUST corresponds in name and type to those given in the weight file(s) used
   Float_t var_p, var_phiv, var_pz_diff, var_diffz, var_sumz, var_opang;
   Float_t var_DCAxy1, var_DCAxy2;
   Float_t var_DCAz1, var_DCAz2;
   Float_t var_mass;
   Float_t var_nITS1, var_nITS2;
   Float_t var_nITSshared1, var_nITSshared2;
   Float_t var_nTPC1, var_nTPC2;
   Float_t var_ITSchi21, var_ITSchi22;
   Float_t var_TPCchi21, var_TPCchi22;
   Float_t var_pt1, var_pt2;
   Float_t var_eta1, var_eta2;
   Float_t var_phi1, var_phi2;

   reader->AddVariable( "var_p := sqrt((px1+px2)*(px1+px2)+(py1+py2)*(py1+py2)+(pz1+pz2)*(pz1+pz2))", &var_p );
   reader->AddVariable( "var_phiv := phiv-1.57", &var_phiv );
   reader->AddVariable( "var_mass := mass", &var_mass );
   reader->AddVariable( "var_pz_diff := pz1/sqrt(px1*px1+py1*py1)-pz2/sqrt(px2*px2+py2*py2)", &var_pz_diff );
   reader->AddVariable( "var_sumz := sumz", &var_sumz );
   reader->AddVariable( "var_diffz := diffz-1.57", &var_diffz );
   reader->AddVariable( "var_opang := opang", &var_opang );
   reader->AddVariable( "var_nITS1 := nITS1", &var_nITS1 );
   reader->AddVariable( "var_nITS2 := nITS2", &var_nITS2 );
   reader->AddVariable( "var_nITSshared1 := nITSshared1", &var_nITSshared1 );
   reader->AddVariable( "var_nITSshared2 := nITSshared2", &var_nITSshared2 );
   reader->AddVariable( "var_nTPC1 := nTPC1", &var_nTPC1 );
   reader->AddVariable( "var_nTPC2 := nTPC2", &var_nTPC2 );
   reader->AddVariable( "var_DCAxy1 := log(abs(DCAxy1))", &var_DCAxy1 );
   reader->AddVariable( "var_DCAxy2 := log(abs(DCAxy2))", &var_DCAxy2 );
   reader->AddVariable( "var_DCAz1 := log(abs(DCAz1))", &var_DCAz1 );
   reader->AddVariable( "var_DCAz2 := log(abs(DCAz2))", &var_DCAz2 );
   // reader->AddVariable( "var_chi2g1 := chi2g1", &var_chi2g1 ); 
   // reader->AddVariable( "var_chi2g2 := chi2g2", &var_chi2g2 );
   reader->AddVariable( "var_ITSchi21 := ITSchi21", &var_ITSchi21 );
   reader->AddVariable( "var_ITSchi22 := ITSchi22", &var_ITSchi22 );
   reader->AddVariable( "var_TPCchi21 := TPCchi21", &var_TPCchi21 );
   reader->AddVariable( "var_TPCchi22 := TPCchi22", &var_TPCchi22 );
   reader->AddVariable( "var_pt1 := pt1", &var_pt1 );
   reader->AddVariable( "var_pt2 := pt2", &var_pt2 );
   reader->AddVariable( "var_eta1 := eta1", &var_eta1 );
   reader->AddVariable( "var_eta2 := eta2", &var_eta2 );
   reader->AddVariable( "var_phi1 := phi1", &var_phi1 ); 
   reader->AddVariable( "var_phi2 := phi2", &var_phi2 );

   
   // Spectator variables declared in the training have to be added to the reader, too
   Float_t IsRP, IsHF, IsConv;
   Float_t pdg1, pdg2, motherPdg1, motherPdg2;
   Int_t IsTaggedRPConv_classicalCuts;
   Int_t IsRPConv_MVAoutput;
   Int_t IsTaggedRPConv_classicalCuts_prefilter, IsTaggedRPConv_MVAcuts_prefilter;
   Int_t IsTaggedConvTrack1, IsTaggedConvTrack2;
   reader->AddSpectator( "IsRP", &IsRP );
   reader->AddSpectator( "IsHF", &IsHF );
   reader->AddSpectator( "IsConv", &IsConv );
   reader->AddSpectator( "IsTaggedRPConv_classicalCuts", &IsTaggedRPConv_classicalCuts );
   reader->AddSpectator( "IsTaggedRPConv_classicalCuts_prefilter", &IsTaggedRPConv_classicalCuts_prefilter );
   reader->AddSpectator( "IsRPConv_MVAoutput", &IsRPConv_MVAoutput );
   reader->AddSpectator( "IsTaggedRPConv_MVAcuts_prefilter", &IsTaggedRPConv_MVAcuts_prefilter );
   reader->AddSpectator( "IsTaggedConvTrack1", &IsTaggedConvTrack1 );
   reader->AddSpectator( "IsTaggedConvTrack2", &IsTaggedConvTrack2 );
   reader->AddSpectator( "pdg1", &pdg1 );
   reader->AddSpectator( "pdg2", &pdg2 );
   reader->AddSpectator( "motherPdg1", &motherPdg1 );
   reader->AddSpectator( "motherPdg2", &motherPdg2 );
   

   
   // reader->AddVariable( "var_p := sqrt((px1+px2)*(px1+px2)+(py1+py2)*(py1+py2)+(pz1+pz2)*(pz1+pz2))", &var_p );
   // reader->AddVariable( "var_phiv := abs(phiv-1.57)", &var_phiv );
   // reader->AddVariable( "var_mass := mass", &var_mass );
   // reader->AddVariable( "var_pz_diff := abs(pz1/sqrt(px1*px1+py1*py1)-pz2/sqrt(px2*px2+py2*py2))", &var_pz_diff );
   // reader->AddVariable( "var_diffz := abs(diffz-1.57)", &var_diffz );
   // reader->AddVariable( "var_opang := abs(opang)", &var_opang );
   // reader->AddVariable( "var_nITS1 := nITS1", &var_nITS1 );
   // reader->AddVariable( "var_nITS2 := nITS2", &var_nITS2 );
   // reader->AddVariable( "var_DCAxy1 := log(abs(DCAxy1))", &var_DCAxy1 );
   // reader->AddVariable( "var_DCAxy2 := log(abs(DCAxy2))", &var_DCAxy2 );
   

   // // Spectator variables declared in the training have to be added to the reader, too
   // Float_t IsRP, IsHF, IsConv, pdg1, pdg2, motherPdg1, motherPdg2;
   // reader->AddSpectator( "IsRP", &IsRP );
   // reader->AddSpectator( "IsHF", &IsHF );
   // reader->AddSpectator( "IsConv", &IsConv );
   // reader->AddSpectator( "pdg1", &pdg1 );
   // reader->AddSpectator( "pdg2", &pdg2 );
   // reader->AddSpectator( "motherPdg1", &motherPdg1 );
   // reader->AddSpectator( "motherPdg2", &motherPdg2 );


   

   // Float_t Category_cat1, Category_cat2, Category_cat3;
   // if (Use["Category"]){
   //    // Add artificial spectators for distinguishing categories
   //    reader->AddSpectator( "Category_cat1 := var3<=0",             &Category_cat1 );
   //    reader->AddSpectator( "Category_cat2 := (var3>0)&&(var4<0)",  &Category_cat2 );
   //    reader->AddSpectator( "Category_cat3 := (var3>0)&&(var4>=0)", &Category_cat3 );
   // }

   // --- Book the MVA methods

   TString dir    = "dataset/weights/";
   TString prefix = "TMVAClassification";

   // Book method(s)
   for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) {
      if (it->second) {
         TString methodName = TString(it->first) + TString(" method");
         TString weightfile = dir + prefix + TString("_") + TString(it->first) + TString(".weights.xml");
         reader->BookMVA( methodName, weightfile ); 
      }
   }



   // Prepare input tree (this must be replaced by your data source)
   // in this example, there is a toy tree with signal and one with background events
   // we'll later on use only the "signal" events for the test in this example.
   //   
   TFile *input(0);
   TString fname = "../../../applicationPhase2/pairedTrackTree/FT2_AnalysisResults_Upgrade_addFeat_pairtree_us_part3_1-1-8-split.root";  
   if (!gSystem->AccessPathName( fname )) 
     input = TFile::Open( fname, "READ" ); // check if file in local directory exists
   
   if (!input) {
      std::cout << "ERROR: could not open data file" << std::endl;
      exit(1);
   }
   std::cout << "--- TMVAClassificationApp    : Using input file: " << input->GetName() << std::endl;
   
   // --- Event loop

   // Prepare the event tree
   // - here the variable names have to corresponds to your tree
   // - you can use the same variables as above which is slightly faster,
   //   but of course you can use different ones and copy the values inside the event loop
   //
   std::cout << "--- Select signal sample" << std::endl;
   TTree* theTree = (TTree*)input->Get("pairTree_us");

   
   // Int_t app_TrackID1, app_TrackID2;
   // Int_t app_EventID1, app_EventID2;
   // Int_t app_IsRP, app_IsUS, app_IsConv, app_IsHF;
   // Int_t app_IsCorrCharm, app_IsCorrBottom, app_IsCorrCharmFromBottom;
   // Int_t app_ChargeSign;
   Int_t app_IsTaggedConvTrack1, app_IsTaggedConvTrack2;
   Float_t app_opang, app_diffz, app_mass;
   Float_t app_sumz;
   Float_t app_phiv;
   Float_t app_px1, app_px2, app_py1, app_py2, app_pz1, app_pz2;
   // Float_t app_mcPx1, app_mcPx2, app_mcPy1, app_mcPy2, app_mcPz1, app_mcPz2;
   // Int_t app_motherPdg1, app_motherPdg2;
   // Int_t app_pdg1, app_pdg2;
   Float_t app_DCAxy1, app_DCAxy2;
   Float_t app_DCAz1, app_DCAz2;
   Int_t app_nITS1, app_nITS2;
   Float_t app_nITSshared1, app_nITSshared2;
   Int_t app_nTPC1, app_nTPC2;
   Float_t app_ITSchi21, app_ITSchi22;
   Float_t app_TPCchi21, app_TPCchi22;
   Float_t app_phi1, app_phi2;
   Float_t app_eta1, app_eta2;
   Float_t app_pt1, app_pt2;
   // theTree->SetBranchAddress( "TrackID1", &app_TrackID1 );
   // theTree->SetBranchAddress( "TrackID2", &app_TrackID2 );
   // theTree->SetBranchAddress( "EventID1", &app_EventID1 );
   // theTree->SetBranchAddress( "EventID2", &app_EventID2 );
   // theTree->SetBranchAddress( "IsRP", &app_IsRP );
   // theTree->SetBranchAddress( "IsUS", &app_IsUS );
   // theTree->SetBranchAddress( "IsConv", &app_IsConv );
   // theTree->SetBranchAddress( "IsHF", &app_IsHF );
   // theTree->SetBranchAddress( "IsCorrCharm", &app_IsCorrCharm );
   // theTree->SetBranchAddress( "IsCorrBottom", &app_IsCorrBottom );
   // theTree->SetBranchAddress( "IsCorrCharmFromBottom", &app_IsCorrCharmFromBottom );
   // theTree->SetBranchAddress( "ChargeSign", &app_ChargeSign );
   theTree->SetBranchAddress( "IsTaggedConvTrack1", &app_IsTaggedConvTrack1 );
   theTree->SetBranchAddress( "IsTaggedConvTrack2", &app_IsTaggedConvTrack2 );
   theTree->SetBranchAddress( "opang", &app_opang );
   theTree->SetBranchAddress( "diffz", &app_diffz );
   theTree->SetBranchAddress( "mass", &app_mass );
   theTree->SetBranchAddress( "sumz", &app_sumz );
   theTree->SetBranchAddress( "phiv", &app_phiv );
   theTree->SetBranchAddress( "px1", &app_px1 );
   theTree->SetBranchAddress( "px2", &app_px2 );
   theTree->SetBranchAddress( "py1", &app_py1 );
   theTree->SetBranchAddress( "py2", &app_py2 );
   theTree->SetBranchAddress( "pz1", &app_pz1 );
   theTree->SetBranchAddress( "pz2", &app_pz2 );
   // theTree->SetBranchAddress( "mcPx1", &app_mcPx1 );
   // theTree->SetBranchAddress( "mcPx2", &app_mcPx2 );
   // theTree->SetBranchAddress( "mcPy1", &app_mcPy1 );
   // theTree->SetBranchAddress( "mcPy2", &app_mcPy2 );
   // theTree->SetBranchAddress( "mcPz1", &app_mcPz1 );
   // theTree->SetBranchAddress( "mcPz2", &app_mcPz2 );
   // theTree->SetBranchAddress( "motherPdg1", &app_motherPdg1 );
   // theTree->SetBranchAddress( "motherPdg2", &app_motherPdg2 );
   // theTree->SetBranchAddress( "pdg1", &app_pdg1 );
   // theTree->SetBranchAddress( "pdg2", &app_pdg2 );
   theTree->SetBranchAddress( "DCAxy1", &app_DCAxy1 );
   theTree->SetBranchAddress( "DCAxy2", &app_DCAxy2 );
   theTree->SetBranchAddress( "DCAz1", &app_DCAz1 );
   theTree->SetBranchAddress( "DCAz2", &app_DCAz2 );
   theTree->SetBranchAddress( "nITS1", &app_nITS1 );
   theTree->SetBranchAddress( "nITS2", &app_nITS2 );
   theTree->SetBranchAddress( "nITSshared1", &app_nITSshared1 );
   theTree->SetBranchAddress( "nITSshared2", &app_nITSshared2 );
   theTree->SetBranchAddress( "nTPC1", &app_nTPC1 );
   theTree->SetBranchAddress( "nTPC2", &app_nTPC2 );
   theTree->SetBranchAddress( "phi1", &app_phi1 );
   theTree->SetBranchAddress( "phi2", &app_phi2 );
   theTree->SetBranchAddress( "eta1", &app_eta1 );
   theTree->SetBranchAddress( "eta2", &app_eta2 );
   theTree->SetBranchAddress( "pt1", &app_pt1 );
   theTree->SetBranchAddress( "pt2", &app_pt2 );

   // output tree:
   TFile *target  = new TFile( "TMVApp.root","RECREATE" );
   TTree *targetTree = new TTree("pairTree_MVAoutput","MVA output");
   // Float_t MLP;
   Float_t BDT;
   // targetTree->Branch("MLP", &MLP, "MLP/F");
   targetTree->Branch("BDT", &BDT, "BDT/F");
   
   // Efficiency calculator for cut method
   Int_t    nSelCutsGA = 0;
   Double_t effS       = 0.7;

   std::vector<Float_t> vecVar(4); // vector for EvaluateMVA tests

   std::cout << "--- Processing: " << theTree->GetEntries() << " events" << std::endl;
   TStopwatch sw;
   sw.Start();
   Long64_t nIevt = theTree->GetEntries();
   for (Long64_t ievt=0; ievt<nIevt;ievt++) {

      if (ievt%1000 == 0) std::cout << "\r--- ... Processing event: " << ievt
				    << " (" << ievt*100/nIevt << " %)";

      theTree->GetEntry(ievt);

      
      var_p = TMath::Sqrt((app_px1+app_px2)*(app_px1+app_px2)
			  + (app_py1+app_py2)*(app_py1+app_py2)
			  + (app_pz1+app_pz2)*(app_pz1+app_pz2));
      var_phiv = app_phiv-1.57;
      var_mass = app_mass;
      var_pz_diff = app_pz1/TMath::Sqrt(app_px1*app_px1+app_py1*app_py1)-app_pz2/TMath::Sqrt(app_px2*app_px2+app_py2*app_py2);
      var_sumz = app_sumz;
      var_diffz = app_diffz-1.57;
      var_opang = app_opang;
      var_nITS1 = app_nITS1;
      var_nITS2 = app_nITS2;
      var_nITSshared1 = app_nITSshared1;
      var_nITSshared2 = app_nITSshared2;
      var_nTPC1 = app_nTPC1;
      var_nTPC2 = app_nTPC2;
      var_DCAxy1 = TMath::Log(TMath::Abs(app_DCAxy1));
      var_DCAxy2 = TMath::Log(TMath::Abs(app_DCAxy2));
      var_DCAz1 = TMath::Log(TMath::Abs(app_DCAz1));
      var_DCAz2 = TMath::Log(TMath::Abs(app_DCAz2));
      var_ITSchi21 = app_ITSchi21;
      var_ITSchi22 = app_ITSchi22;
      var_TPCchi21 = app_TPCchi21;
      var_TPCchi22 = app_TPCchi22;
      var_pt1 = app_pt1;
      var_pt2 = app_pt2;
      var_eta1 = app_eta1;
      var_eta2 = app_eta2;
      var_phi1 = app_phi1;
      var_phi2 = app_phi2;
      
      // MLP = reader->EvaluateMVA( "MLP method" );
      if(app_IsTaggedConvTrack1==1 || app_IsTaggedConvTrack2==1) {
	BDT = 999;
      }else if(app_mass < .1) {
	BDT = 99;
      }else {
	BDT = reader->EvaluateMVA( "BDT method" );
      }
      // bMLP->Fill();
      targetTree->Fill();
      
      // --- Return the MVA outputs and fill into histograms

      if (Use["CutsGA"]) {
         // Cuts is a special case: give the desired signal efficienciy
         Bool_t passed = reader->EvaluateMVA( "CutsGA method", effS );
         if (passed) nSelCutsGA++;
      }

   }

   // Get elapsed time
   sw.Stop();
   std::cout << "--- End of event loop: "; sw.Print();

   // Get efficiency for cuts classifier
   if (Use["CutsGA"]) std::cout << "--- Efficiency for CutsGA method: " << double(nSelCutsGA)/theTree->GetEntries()
                                << " (for a required signal efficiency of " << effS << ")" << std::endl;

   if (Use["CutsGA"]) {

      // test: retrieve cuts for particular signal efficiency
      // CINT ignores dynamic_casts so we have to use a cuts-secific Reader function to acces the pointer  
      TMVA::MethodCuts* mcuts = reader->FindCutsMVA( "CutsGA method" ) ;

      if (mcuts) {      
         std::vector<Double_t> cutsMin;
         std::vector<Double_t> cutsMax;
         mcuts->GetCuts( 0.7, cutsMin, cutsMax );
         std::cout << "--- -------------------------------------------------------------" << std::endl;
         std::cout << "--- Retrieve cut values for signal efficiency of 0.7 from Reader" << std::endl;
         for (UInt_t ivar=0; ivar<cutsMin.size(); ivar++) {
            std::cout << "... Cut: " 
                      << cutsMin[ivar] 
                      << " < \"" 
                      << mcuts->GetInputVar(ivar)
                      << "\" <= " 
                      << cutsMax[ivar] << std::endl;
         }
         std::cout << "--- -------------------------------------------------------------" << std::endl;
      }
   }

   // --- Write tree
   targetTree->Write();
   
   target->Close();

   std::cout << "--- Created root file: \"TMVApp.root\" containing the MVA output histograms" << std::endl;
  
   delete reader;
    
   std::cout << "==> TMVAClassificationApplication is done!" << std::endl << std::endl;

  
  if(gROOT->GetVersion() == problematicRootVersion) {
    std::cout << std::endl << "!!!!!  WARNING: You are using ROOT "
	      << gROOT->GetVersion() << " - errors might be encountered with this version!"
	      << std::endl << std::endl;
  }

  gApplication->Terminate();
  
} 



int main( int argc, char** argv )
{
   TString methodList; 
   for (int i=1; i<argc; i++) {
      TString regMethod(argv[i]);
      if(regMethod=="-b" || regMethod=="--batch") continue;
      if (!methodList.IsNull()) methodList += TString(","); 
      methodList += regMethod;
   }
   TMVAClassificationApplication_pairTree(methodList); 
   return 0; 
}
