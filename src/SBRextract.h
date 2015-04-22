#ifndef __SBREXTRACT_H__
#define __SBREXTRACT_H__

//STL
#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <map>
#include <cmath>
#include <utility>
#include <string>

// Root
#include "TROOT.h"
#include "TTree.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TGraph2D.h"
#include "TGraph2DErrors.h"
#include "TCanvas.h"
#include "TProfile.h"
#include "TLatex.h"
#include "TFile.h"
#include "TF1.h"
#include "TF2.h"
#include "TCut.h"
#include "THStack.h"

// RooFit
#include "RooDataHist.h"
#include "RooHistPdf.h"
#include "RooProdPdf.h"
#include "RooAbsPdf.h"
#include "RooAddPdf.h"

#ifndef __CINT__
#include "RooGlobalFunc.h"
#else
// Refer to a class implemented in libRooFit to force its loading
// via the autoloader.
class Roo2DKeysPdf;
#endif

#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "RooPlot.h"
#include "TMath.h"
#include "TF1.h"
#include "Math/DistFunc.h"

#ifndef __CINT__
#include "RooCFunction1Binding.h" 
#include "RooCFunction2Binding.h"
#include "RooCFunction3Binding.h"
#include "RooCFunction4Binding.h"
#endif

#include "RooTFnBinding.h" 

// Custom
#include "SingleMuonsConsts.h"
#include "HelperFunctions.h"
#include "BasicCutTreeVariables.h"

using namespace RooFit;

class SBRextract
{
 public:
  SBRextract( std::string openFile, std::string treeFile );
  ~SBRextract();
  void Run(); // Do everything related to SBR extraction
  void SetSBROutFileNameStd(std::string fName);
  void SetSBROutFileNameCsv(std::string fName);

 private:
  // PDF SETUP
  std::string pdfSeedFileName;
  TFile* rootFilePdfSeeds;
  std::string MakePdfName(const std::string& var, const std::string& type, int arm_i, int charge_i);
  
  // PDF SEED
  TH1F*  h_dw23_pdfSeed [NUMBER_OF_MUON_STREAMS][NUMBER_OF_ARM_CONFIGS][NUMBER_OF_CHARGE_CONFIGS];
  TH1F*  h_eta_pdfSeed  [NUMBER_OF_MUON_STREAMS][NUMBER_OF_ARM_CONFIGS][NUMBER_OF_CHARGE_CONFIGS];

  // RooFit - PDFs
  
  // FITTING SETUP
  std::string unbinnedDataFileName;
  int LoadUnbinnedData();
  TFile* unbinnedDataFile;
  TTree* unbinnedData;
  TTree* subTree[NUMBER_OF_ARM_CONFIGS][NUMBER_OF_CHARGE_CONFIGS];
  int SetupSubTrees();
  int totalMuons[NUMBER_OF_ARM_CONFIGS][NUMBER_OF_CHARGE_CONFIGS];

  int LoadPdfs();

  int MakeRooVariables();
  std::string MakeRooVarName(const std::string& stub, int arm_i , int charge_i);
  // Define Fitting Variables - pdf variables
  RooRealVar* v_dw23 ; 
  RooRealVar* v_eta  ;
  RooRealVar* v_Wness;
  // Defining the Fitting Variables - Number of muons
  RooRealVar* v_Nmu_HadBak;
  RooRealVar* v_Nmu_WSig  ;
  RooRealVar* v_Nmu_MuBak ;
  // Definine unbinned data set
  RooDataSet* DataSet[NUMBER_OF_ARM_CONFIGS][NUMBER_OF_CHARGE_CONFIGS]; 
  RooAddPdf * Model  [NUMBER_OF_ARM_CONFIGS][NUMBER_OF_CHARGE_CONFIGS];

  RooDataHist* rooHist_dw23 [NUMBER_OF_MUON_STREAMS][NUMBER_OF_ARM_CONFIGS][NUMBER_OF_CHARGE_CONFIGS];
  RooDataHist* rooHist_eta  [NUMBER_OF_MUON_STREAMS][NUMBER_OF_ARM_CONFIGS][NUMBER_OF_CHARGE_CONFIGS];
  RooHistPdf*  pdf_dw23     [NUMBER_OF_MUON_STREAMS][NUMBER_OF_ARM_CONFIGS][NUMBER_OF_CHARGE_CONFIGS];
  RooHistPdf*  pdf_eta      [NUMBER_OF_MUON_STREAMS][NUMBER_OF_ARM_CONFIGS][NUMBER_OF_CHARGE_CONFIGS];
  RooProdPdf*  prodPdf      [NUMBER_OF_MUON_STREAMS][NUMBER_OF_ARM_CONFIGS][NUMBER_OF_CHARGE_CONFIGS];

  // Signal To Background Ratio
  int Num_HadBak_Muons[NUMBER_OF_ARM_CONFIGS][NUMBER_OF_CHARGE_CONFIGS];
  int Num_MuBak_Muons [NUMBER_OF_ARM_CONFIGS][NUMBER_OF_CHARGE_CONFIGS];
  int Num_WSig_Muons   [NUMBER_OF_ARM_CONFIGS][NUMBER_OF_CHARGE_CONFIGS];
  double Err_Hi_HadBak_Muons [NUMBER_OF_ARM_CONFIGS][NUMBER_OF_CHARGE_CONFIGS];
  double Err_Lo_HadBak_Muons [NUMBER_OF_ARM_CONFIGS][NUMBER_OF_CHARGE_CONFIGS];
  double Err_Hi_WSig_Muons [NUMBER_OF_ARM_CONFIGS][NUMBER_OF_CHARGE_CONFIGS];
  double Err_Lo_WSig_Muons [NUMBER_OF_ARM_CONFIGS][NUMBER_OF_CHARGE_CONFIGS];
  double SBR[NUMBER_OF_ARM_CONFIGS][NUMBER_OF_CHARGE_CONFIGS];
  double Err_Hi_SBR[NUMBER_OF_ARM_CONFIGS][NUMBER_OF_CHARGE_CONFIGS];
  double Err_Lo_SBR[NUMBER_OF_ARM_CONFIGS][NUMBER_OF_CHARGE_CONFIGS];

  int MakeRooWSignalPdfs();
  int MakeRooHadronicBackgroundPdfs();
  int MakeRooMuonBackgroundPdfs();
  int MakeRooModel();
  int MakeRooDataSet();
  int FitModelToDataSet();

  // Output
  std::string SbrOutFileName;
  std::string sbrCSVName;

};
#endif
