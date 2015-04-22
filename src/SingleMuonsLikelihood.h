#ifndef __SINGLE_MUONS_LIKELIHOOD_H__
#define __SINGLE_MUONS_LIKELIHOOD_H__

//STL
#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <map>

// Root
#include "TROOT.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TAxis.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TGraph2D.h"
#include "TGraph2DErrors.h"
#include "TCanvas.h"
#include "TProfile.h"
#include "TLatex.h"
#include "TFile.h"
#include "RVersion.h" // check root version so I don't have to keep changing things locally

// RooFit
#include "RooRealVar.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"
#include "RooProdPdf.h"

#include "SingleMuonsConsts.h"
#include "HelperFunctions.h"

class SingleMuonsLikelihood
{
 public:
  SingleMuonsLikelihood(); // implemented
  ~SingleMuonsLikelihood(); // need to handle memory
  int ReadHistograms(TFile*); // Reads in data and simulation histograms from an initialized TFile      
  int InitHistograms();
  int SetVariables(float, float, float, float, float, float, float, float, float); // call AFTER you have histograms. 

  // chose var, arm, charge indicies, final argument is fill value.

  int Fill(int, int, int, int, float        , bool); // 1D method, last arg is filled, first args are coordinates
  int Fill(int, int, int, int, float, float , bool); // 2D method, last two args are filled (x,y), first args are coordinates

  // GetLikelihood
  // Indicies are defined in SingleMuonsConsts.h
  // Arg1. Likelihood index 
  // Arg2. Arm index
  // Arg3. Charge Index
  // Arg4. Data index
  // 
  // Usage:
  // We wish to flag a given event with a likelihood value (likelihood ratio)
  // This member serves to select the correct likelihood ratio, based on the event
  // type - for example - positive muon in north arm, with no RPC3 data, but has RPC1
  // and FVTX data - would be called like:
  // --> instance of SingleMuonsLikelihood -> sm
  // --> sm.GetLikelihood( RPC1_AND_FVTX, ARM_NORTH, CHARGE_POS, HADRONIC_BACKGROUND )
  // This takes advantage of our indexing system, defined in SingleMuonsConsts.h
  //
  // Design Discussion:
  // This member does not make any checks to see if the Likelihood it tries to call
  // has been defined yet, but does make out of range index checks. However, if 
  // a likelihood is called (via index) which has not been defined, the program
  // WILL segfault...I guess this is better than not segfaulting and continuing without
  // error. In any case, we do not want to given default likelihoods outside of what
  // we want, because that will introduce a systematic error into the analysis - 
  // the philosophy of this code is that it either works, or it breaks - it does not
  // try to fix things for the user.
  float GetLikelihood(int, int, int, int); 

  int Write(TFile* );

 private:
  // State Tracking Variables
  bool usingData;         // True if we Initalize the class to read/fill simulation data
  bool usingSimulation;   // True if we Initialze the class to read/fill data
  bool loadedHistograms;  // True if we create the histograms
  bool definedHistograms; // True if we read the histograms
  bool manualDelete;      // True if we want to leave historams in memory when class goess out of scope

  // Data and simulation are done separately because the distributions are filled in separate
  // loops to avoid namespace collisions in root
  int Init1DHistograms(); // initialize data histograms
  int Init2DHistograms(); // initialize simulaiton histograms

  // GenerateProdPdfs
  // This Member automatically intitalizes every distinct likelihood we may wish
  // to assign to an event. All the configuration indicies are looped over (see 
  // SingleMuonConsts.h) and the appropriate 2D variables are correlated via the 
  // RooPdf2D object. Single variables are still fed into the likelihood.
  int GenerateProdPdfs();

  int InitRooRealVars(); // Initalize and create RooRealVars
  int InitLikelihoods();       // to do, use boolean to control the return result of this function.

  // It is expected that histograms for data and simulation are loaded from one root
  // file, and that these histograms have unique names. Therefore, it is NOT neccessary
  // to have separate methods to create roo hists and roo pdfs, etc.

  TH1F* PDFseed1D[NUMBER_OF_VARS           ][NUMBER_OF_ARM_CONFIGS][NUMBER_OF_CHARGE_CONFIGS][NUMBER_OF_DATA_STREAMS]; 
  TH2F* PDFseed2D[NUMBER_OF_CORRELATED_VARS][NUMBER_OF_ARM_CONFIGS][NUMBER_OF_CHARGE_CONFIGS][NUMBER_OF_DATA_STREAMS];

  RooRealVar* RooVar         [NUMBER_OF_VARS]; // All 1D variables 

  // RooFit - 1D RooHist + RooPDF
  RooDataHist* RooHist1D[NUMBER_OF_VARS][NUMBER_OF_ARM_CONFIGS][NUMBER_OF_CHARGE_CONFIGS][NUMBER_OF_DATA_STREAMS];
  RooHistPdf*  RooPdf1D [NUMBER_OF_VARS][NUMBER_OF_ARM_CONFIGS][NUMBER_OF_CHARGE_CONFIGS][NUMBER_OF_DATA_STREAMS];

  // RooFit - 2D RooHist + RooPDF
  RooDataHist* RooHist2D [NUMBER_OF_CORRELATED_VARS][NUMBER_OF_ARM_CONFIGS][NUMBER_OF_CHARGE_CONFIGS][NUMBER_OF_DATA_STREAMS];   
  RooHistPdf*  RooPdf2D  [NUMBER_OF_CORRELATED_VARS][NUMBER_OF_ARM_CONFIGS][NUMBER_OF_CHARGE_CONFIGS][NUMBER_OF_DATA_STREAMS];   

  // Various Likelihoods 
  RooProdPdf* Likelihood[NUMBER_OF_LIKELIHOODS][NUMBER_OF_ARM_CONFIGS][NUMBER_OF_CHARGE_CONFIGS][NUMBER_OF_DATA_STREAMS];
};

#endif
