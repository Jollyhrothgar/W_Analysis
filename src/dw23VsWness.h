#ifndef __DW23_VS_WNESS_H__
#define __DW23_VS_WNESS_H__

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

double double_gaus_2d( double* x, double* par);

class dw23VsWness
{
 public:
  dw23VsWness();
  ~dw23VsWness();

  /* ANALYSIS INTERFACE  */
  // dw23Extrapolate_ operates in two modes, one, where the tree for hadronic background (data)
  // is looped over, and filtered into histograms, which are then fit for the extrapolation, 
  // and the other mode, where a file is read, containing prefilled histograms, which are then
  // fit.

  // dw23Extrapolate_LoopAndFit
  // Usage: Reads in a tree, runs over the tree, fits histograms for dw23 data 
  // extrapolation, writes histograms to file. Called instead of dw23Extrapolate_ReadAndFit
  // Args:
  //  1: string - a string containing the record name of the basic cut tree
  //  2: string - a string containing the record name of the root file to serialize this class'
  //              root objects. histoFile is created, treeFile is read and looped over to create
  //              histogram objects to store in histoFile.
  int dw23Extrapolate_LoopAndFit(std::string treeFile, std::string histoFile);

  // dw23Extrapolate_LoopAndFit
  // Usage: Reads in a tree, runs over the tree, fits histograms for dw23 data 
  // extrapolation, writes histograms to file. Called instead of dw23Extrapolate_ReadAndFit
  // Args:
  //  1: string - a string containing the record name of the basic cut tree
  //  2: string - a string containing the record name of the root file to serialize this class'
  //              root objects. histoFile is read, treeFile is read
  int dw23Extrapolate_ReadAndFit(std::string treeFile, std::string histoFile); // Reads in a histogram file, fits histograms for dw23 data extrapolation

  /* IO OPERATIONS */
  int Write();     // Loops over TObject vector writing to file
  int Print(); // Loops Over all objects in root object registry + binning.
  int SetNoDestruct(); // Will not delete objects in Registry. Introduces memory leak so root objects stay in scope.
  int SetHighVerbosity(); // turns on most diagnostic output 
  int GeneratePDFs();

  /* CUSTOMIZATION OF ANALYSIS */
  int SetWnessBinning(float min_range, float max_range, int bins); // default binning is used until this member is called.
  int SetPDFSmoothing(int smooth);

  /* PLOTS */
  int SaveCanvases(std::string directory); // saves canvases to the directory 
  int ShowHadronicExtrapolationPlots();    // turns on plotting for hadronic background extrapolation // wp

  /* PUBLIC DEVELOPMENT AREA */
  int SelectPdfGroup(int PDFSelect);
  int pdfSelect;
  // Nothing is here, because nothing is being publically developed

  /* UNBINNED LIKELIHOOD FITTING */
  // Setup for Muon Background
  //int SetupRooVars(); // initializes roo fit variables
  int SetPDFOutputFile(std::string);
  int AddMuonBackground(const std::string bkgd_key , const std::string bkgd_file_name, double weight); // add a muon background tree directory listing to vector 
  int AddWSignal(const std::string WSig_file_name); // add the W signal tree directory listing to member

  int FormHadBackgroundPdfSeeds();
  int FormWSignalPdfSeeds();
  int FormMuonBackgroundPdfSeeds();

  // Directory listings / Trees for Muon Backgrounds
  typedef std::map< std::string, std::pair< std::string, double> > MuBakMap;
  MuBakMap MuBkgMap; // wp modify Add Muon background to use simple key like "dy" to get to file name and weight.
  std::map<std::string,int> MuIndex;

  TTree* t_mu_bak[9]; // for some reason "NUMBER_OF_MUON_BACKGROUNDS" throws an error.
  // NUMBER_OF_MUON_BACKGROUNDS is defined in SingleMuonsConsts.h
  std::string WSignalFile;

  //int ExtractPDFs(); // Does unbinned likelihood fitting
  int Num_HadBack_Muons[NUMBER_OF_ARM_CONFIGS][NUMBER_OF_CHARGE_CONFIGS]; // number of muons from the fit
  int Num_WSig_Muons   [NUMBER_OF_ARM_CONFIGS][NUMBER_OF_CHARGE_CONFIGS];
  int Num_MuBack_Muons [NUMBER_OF_ARM_CONFIGS][NUMBER_OF_CHARGE_CONFIGS];

 private:
  /* ANALYSIS OPERATIONS */
  int SetDataTreeFile            (std::string dataTreeFileName );
  int SetAnalysisObjectOutputFile (std::string outputFile);

  int Loop();      // Loop Over Tree, filling histograms.
  int RunFitter(); // Performs fits
  int MakePlots(); // make some plots to show off at meetings
  std::string NewPdfFileName;
  TFile* PdfOutFile;

  unsigned int RegisterObject(TObject* obj);
  /* UNBINNED MAXIMUM LIKELIHOOD FITTING */

  // Likelihood PDFs
  TF1*          f_dw23_HadBak_pdfSeed [NUMBER_OF_ARM_CONFIGS][NUMBER_OF_CHARGE_CONFIGS];
  TH1F*         h_dw23_pdfSeed [NUMBER_OF_MUON_STREAMS][NUMBER_OF_ARM_CONFIGS][NUMBER_OF_CHARGE_CONFIGS];
  TH1F*         h_eta_pdfSeed  [NUMBER_OF_MUON_STREAMS][NUMBER_OF_ARM_CONFIGS][NUMBER_OF_CHARGE_CONFIGS];
  int smoothing;

  // We fit to this model
  RooAddPdf*    Model      [NUMBER_OF_ARM_CONFIGS][NUMBER_OF_CHARGE_CONFIGS];

  double nMuBakMuons[NUMBER_OF_ARM_CONFIGS][NUMBER_OF_CHARGE_CONFIGS];
  std::ofstream MuonBackgroundLog;

  /* CONSTANTS, CONFIGURATION */

  /* WNESS BINNING */
  // Default option:
  //   Binning used in previous analyses
  //   bin# 0 -> Full Wness Range
  //   bin# 1 -> 0.1 < Wness < 0.3
  //   bin# 2 -> 0.3 < Wness < 0.5
  //   bin# 3 -> 0.5 < Wness < 0.7
  //   bin# 4 -> 0.7 < Wness < 0.9
  //   bin# 5 -> 0.92< Wness < 1.0 extrapolation region
  // User Defined Option: specify range and # of bins gets you:
  //   bin# 0 -> Full Wness Range
  //   bin# 1 -> WnessBin1 low_edge < Wness < low_edge+bin_width
  //   ...
  //   bin# n -> wnessBinN low_edge+n*bin_width < Wness < low_edge+(n+1)*bin_width
  //   Bins are all the same width, evenly  distributed within range of Wness.
  bool overrideDefaultWnessBinning; // use default values = false 
  bool readHistos; // do not delete histograms, instead, close root file.
  int NUMBER_OF_WNESS_BINS;
  int TOTAL_WNESS_BINS;
  float WNESS_UPPER;
  float WNESS_LOWER;
  float WNESS_RANGE;
  float WNESS_STEP;
  int WnessHistoBins;
  std::vector < std::pair<float,float> > WnessBins;
  int SetupWnessBins(); // fills the WnessBins vector with {bin_start, bin_end} values

  /* IO OPERATIONS */
  std::vector<TObject*> Registry;
  std::string dataTreeFile;       // name of input file  (expect root file) (contains tree)
  std::string histogramsFile;     // name of output file (expect root file)
  bool setDestruct;               // Default is false. Toggled to true if SetNoDestruct() called.
  bool showDiagnostics;    // Default is false. Toggled to true if SetLowVerbosity() is called
  TFile* histoFile; // this file contains histograms produced from running over the basic_cut_tree
  TFile* reducedTreeFile; // this is the file we pull the basic_cut_tree from
  std::string histoFileName; // name of histogram file for reading mode
  void Pause(); // Add a brief pause requiring user input to continue

  /* INITIALIZATION */
  int DataType; // HADRONIC_BACKGROUND, W_SIM, or MUONIC_BACKGROUND
  int InitTree();  // init tracks member TTree with tree from dataTreeFile root file
  int PrepareAnalysisObjects(); // Resizes the histogram vectors to the correct size
  int ReadHistograms();    // points histograms to file-resident histograms  
  int MakeAnalysisObjects(); // makes new histograms and functions that are empty.
  bool treeAlreadyInitialized; // tracks whether or not we can use the member tree, tracks.

  /* TREE DEFINITION */
  TTree* tracks; // Contains tracks for a the tree
  std::string treeName;
  BasicCutTreeVariables vars;

  /* FITTING METHODS, DEFINITIONS */
  int ApplyNormalization(); // Normalize the dw23 vs Wness spectrum + normalize dw23 distributions
  // this function fits dw23 vs Wness. pol4 describes Wness slices and coax-gaussian describes dw23 slices
  // Additionally, the parameters of the coax gaussian vary lineraly with Wness.
  int NumCoaxial2DGaussPars;

  // Extracts the value of the parameter par (as defined for the generalized coaxial double
  // gaussian from f2_double_gaus_2d). 
  double GetParameter(int par, double Wness, int arm_i, int charge_i );

  /* WNESS EXTRAPOLATION FITTING OBJECTS */
  // Dimension [NUMBER_OF_ARM_CONFIGS][NUMBER_OF_CHARGE_CONFIGS]
  std::vector< std::vector <TH1F*> > h_Wness_Fit_Seed; // for pol4 fit
  std::vector< std::vector <TH1F*> > h_eta;            // unused
  std::vector< std::vector <TH1F*> > h_eta_hadBak_pdfSeed;   // Wness: 0.1-0.7 - pdf seed
  std::vector< std::vector <TH1F*> > h_dw13;           // unused
  std::vector< std::vector <TH1F*> > h_dw23;           // for SBR
  std::vector< std::vector <TH2F*> > h_etaVsWness;     // for SBR
  std::vector< std::vector <TH2F*> > h_dw23VsEta;      // for SBR
  std::vector< std::vector <TH2F*> > h_dw23VsWness;    // for Extrapolation
  std::vector< std::vector <TF2* > > f2_double_gaus_2d;// fit to dw23VsWness
  std::vector< std::vector <TF1* > > f_Wness_Seed;

  /* Efficiency Correction */
 // eta_min  eta_max  | effi     | delta_effi effi_syst | effi     delta_effi effi_syst 
  struct eta_eff
  {
    double eta_min;   // minimum range of eta bin
    double eta_max;   // maximum range of eta bin
    double effi;      // efficiency
    double delta_effi;// efficiency uncertainty
    double syst_effi; // systematic efficiency
    eta_eff()
    {
      eta_min = 0;
      eta_max = 0;
      effi = 0;
      delta_effi = 0;
      syst_effi = 0;
    }
  };

  // [NUMBER_OF_ARM_CONFIGS][NUMBER_OF_CHARGE_CONFIGS][NUMBER_OF_ETA_BINS]
  std::vector< std::vector< std::vector< eta_eff > > > eta_eff;
  int LoadEtaEfficiencies();

  // Dimension [NUMBER_OF_ARM_CONFIGS][NUMBER_OF_CHARGE_CONFIGS][TOTAL_WNESS_BINS]
  std::vector< std::vector < std::vector<TH1F*> > > h_etaWness ;// slice in Wness
  std::vector< std::vector < std::vector<TH1F*> > > h_dw23Wness;// slice in Wness 
  std::vector< std::vector < std::vector<TH1F*> > > h_WnessBin; // Wness bin

  /* DISPLAY OBJECTS */
  TH1F* h_Wness; // for display purposes - the complete wness spectrum.
  unsigned int RegisterCanvas(TCanvas*); // puts a canvas in a vector
  bool plotHadronicExtrapolation; // default is false, set to true if SetNoHadronicExtrapolationPlots() is called.
  TCanvas* c_WnessVsdw23; // shows 2D dw23 vs Wness and fits
  TCanvas* c_dw23Slices; // shows dw23 in each Wness bin
  TCanvas* c_dw23SlicesCheck;
  TCanvas* c_WnessFit1D; // 1D Wness seed fits
  TCanvas* c_WnessLinearCheck; // check if gaus params are linear
  TCanvas* c_dw23_Eta_PDF[NUMBER_OF_ARM_CONFIGS][NUMBER_OF_CHARGE_CONFIGS]; // show final PDFs prior to fit
  TCanvas* c_RooPlotTest; // Plot rooPlot to this canvas.
  std::vector<TCanvas*> CanvasRegistry;

  /* HISTOGRAM BOUNDARIES */
  int eta_min; 
  float eta_max;
  float eta_bins;
  float dw23_min;
  float dw23_max;
  float pdf_dw23_min;
  float pdf_dw23_max;
  int dw23_bins;
};
#endif
