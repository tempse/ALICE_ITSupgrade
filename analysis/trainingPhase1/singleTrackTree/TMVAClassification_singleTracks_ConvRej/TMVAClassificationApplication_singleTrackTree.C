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

void TMVAClassificationApplication_singleTrackTree( TString myMethodList = "" ) 
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
   Float_t var_eta, var_phi, var_pt, var_dcaR, var_dcaZ, var_p, var_nITS, var_nTPC,
     var_nITSshared, var_ITSchi2, var_TPCchi2;

   reader->AddVariable( "var_eta := eta", &var_eta );
   reader->AddVariable( "var_phi := phi", &var_phi);
   reader->AddVariable( "var_pt := pt", &var_pt );
   reader->AddVariable( "var_dcaR := log(abs(dcaR))", &var_dcaR );
   reader->AddVariable( "var_dcaZ := log(abs(dcaZ))", &var_dcaZ);
   reader->AddVariable( "var_p := sqrt(particle.fPx*particle.fPx + particle.fPy*particle.fPy + particle.fPz*particle.fPz)", &var_p );
   reader->AddVariable( "var_nITS := nITS", &var_nITS );
   reader->AddVariable( "var_nTPC := nTPC", &var_nTPC );
   reader->AddVariable( "var_nITSshared := nITSshared", &var_nITSshared );
   reader->AddVariable( "var_ITSchi2 := ITSchi2", &var_ITSchi2 );
   reader->AddVariable( "var_TPCchi2 := TPCchi2", &var_TPCchi2 );
   
   

   // Spectator variables declared in the training have to be added to the reader, too
   Float_t pdg, pdgMother, pdgFirstMother;
   // reader->AddSpectator( "pdg", &pdg );
   // reader->AddSpectator( "pdgMother", &pdgMother );
   // reader->AddSpectator( "pdgFirstMother", &pdgFirstMother );
   // reader->AddSpectator( "IsRP" );
   // reader->AddSpectator( "IsHF" );
   // reader->AddSpectator( "IsConv" );
   // reader->AddSpectator( "pdg1" );
   // reader->AddSpectator( "pdg2" );
   // reader->AddSpectator( "motherPdg1" );
   // reader->AddSpectator( "motherPdg2" );

   

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
   // TString fname = "../../../trainingPhase2/singleTrackTree/FT2_AnalysisResults_Upgrade_addFeat_part2_1-1-8-split.root";
   TString fname = "../../../applicationPhase2/singleTrackTree/FT2_AnalysisResults_Upgrade_addFeat_part3_1-1-8-split.root";
   // TString fname = "../../../applicationPhase_final/FT2_AnalysisResults_Upgrade_addFeat_part3_1-1-8-split.root";
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
   TTree* theTree = (TTree*)input->Get("tracks");

   Int_t app_event;
   Int_t app_global;
   Int_t app_label;
   Int_t app_labelFirstMother, app_labelMinFirstMother, app_labelMaxFirstMother;
   Int_t app_pdg, app_pdgMother;
   Int_t app_pdgFirstMother;
   Int_t app_nITS;
   Double_t app_nITSshared;
   Double_t app_ITSchi2, app_TPCchi2;
   Int_t app_nTPC;
   Double_t app_IP;
   Double_t app_eta, app_phi;
   Double_t app_pt;
   Double_t app_dcaR;
   Double_t app_dcaZ;
   Double_t app_mcEta, app_mcPhi, app_mcPt;
   
   

   TParticle* app_particle = NULL;  
   theTree->SetBranchAddress("event",&app_event);
   theTree->SetBranchAddress("IP",&app_IP);
   theTree->SetBranchAddress("global",&app_global);
   theTree->SetBranchAddress("eta",&app_eta);
   theTree->SetBranchAddress("phi",&app_phi);
   theTree->SetBranchAddress("pt",&app_pt);
   theTree->SetBranchAddress("dcaR",&app_dcaR);
   theTree->SetBranchAddress("dcaZ",&app_dcaZ);
   theTree->SetBranchAddress("label",&app_label);
   theTree->SetBranchAddress("particle.",&app_particle);
   theTree->SetBranchAddress("labelFirstMother",&app_labelFirstMother);
   theTree->SetBranchAddress("labelMinFirstMother",&app_labelMinFirstMother);
   theTree->SetBranchAddress("labelMaxFirstMother",&app_labelMaxFirstMother);
   theTree->SetBranchAddress("mcEta",&app_mcEta);
   theTree->SetBranchAddress("mcPhi",&app_mcPhi);
   theTree->SetBranchAddress("mcPt",&app_mcPt);
   theTree->SetBranchAddress("pdg",&app_pdg);
   theTree->SetBranchAddress("pdgMother",&app_pdgMother);
   theTree->SetBranchAddress("pdgFirstMother",&app_pdgFirstMother);
   theTree->SetBranchAddress("nITS",&app_nITS);
   theTree->SetBranchAddress("nITSshared",&app_nITSshared);
   theTree->SetBranchAddress("nTPC",&app_nTPC);
   theTree->SetBranchAddress("ITSchi2",&app_ITSchi2);
   theTree->SetBranchAddress("TPCchi2",&app_TPCchi2);



   // output tree:
   TFile *target  = new TFile( "TMVApp.root","RECREATE" );
   TTree *targetTree = new TTree("tracks_MVAoutput","MVA output");
   Float_t MVAoutput;
   if(Use["MLP"]) targetTree->Branch("MLP", &MVAoutput, "MLP/F");
   if(Use["BDT"]) targetTree->Branch("BDT", &MVAoutput, "BDT/F");
   
   
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

      var_eta = app_eta;
      var_phi = app_phi;
      var_pt = app_pt;
      var_dcaR = TMath::Log(TMath::Abs(app_dcaR));
      var_p = TMath::Sqrt(app_particle->Px()*app_particle->Px() +
			  app_particle->Py()*app_particle->Py() +
			  app_particle->Pz()*app_particle->Pz());
      var_nITS = app_nITS;
      var_nTPC = app_nTPC;
      var_nITSshared = app_nITSshared;
      var_ITSchi2 = app_ITSchi2;
      var_TPCchi2 = app_TPCchi2;

      
      if(Use["MLP"]) MVAoutput = reader->EvaluateMVA( "MLP method" );
      if(Use["BDT"]) MVAoutput = reader->EvaluateMVA( "BDT method" );
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
   TMVAClassificationApplication_singleTrackTree(methodList); 
   return 0; 
}
