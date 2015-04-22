#ifndef _SINGLE_MUONS_CONSTS_H_
#define _SINGLE_MUONS_CONSTS_H_

#include <string>
#include <map>
// Environment
const std::string baseDirectory          = "/direct/phenix+spin2/beaumim/WAnalysisRun13/AnalysisCode/";
const std::string srcDirectory           = "src/";
const std::string macrosDirectory        = "macros/";
const std::string runlistsDirectory      = "runlists/";
const std::string rootPlotFilesDirectory = "rootPlotFiles/";
const std::string rootTreesDirectory     = "rootTrees/";
const std::string figuresDirectory       = "figs/";
const std::string logDirectory           = "logs/";
const std::string libDir                 = "$MYINSTALL/lib/";
const std::string wness_tree = "newsngmuons_basic_cut";

// Analyzers
const int MIKE_INDEX   = 0;
const int DANIEL_INDEX = 1;
const int RALF_INDEX   = 2;
const int ABE_INDEX    = 3;
const int ANALYZER_INDEX = MIKE_INDEX; // select analyzer here

// blue is left letter, yellow is right letter
// P = plus (+1),  M = Minus (-1)
// Asymmetry
const int NUMBER_OF_SPIN_PATTERNS = 5;
namespace spinPattern {
  enum Pattern{ PP = 0, MP = 1, PM = 2, MM = 3, EMPTY = 4 };
  enum Beam   { BLUE = 0, YELLOW = 1};
}

// For basic cut tree I/O
const int READ  = 0;
const int WRITE = 1;

// Basic Cuts
const float BASIC_CUT_DG0            =  20.0; // cm
const float BASIC_CUT_DDG0           =   9.0; // deg
const float BASIC_CUT_DCA_r          =  30.0; // cm - eliminate this cut
const float BASIC_CUT_chi2           =  20.0; // unitless
const float BASIC_CUT_p_low          =   5.0; // GeV
const float BASIC_CUT_p_high         = 250.0; // GeV 
const float BASIC_CUT_pT_low         = 16.0 ; // GeV 
const float BASIC_CUT_pT_high        = 60.0 ; // GeV 
const float RPC1_DIST_HIGH           = 20.0 ; // cm
const float RPC3_DIST_HIGH           = 40.0 ; // cm
const float RPC_MAX_VALUE            = 100.0; // cm
const float FVTX_DTHETA_MAX_VALUE    =   1.5; // radians
const float FVTX_DPHI_MAX_VALUE      =   1.5; // radians
const float FVTX_CONE_MAX_VALUE      = 60.0 ; // no unit
const float FVTX_DR_MAX_VALUE        = 100.0; // cm
const float FVTX_DRxDTHETA_MAX_VALUE = 150.0;

// Physics Distributions - correllated
const int NUMBER_OF_CORRELATED_VARS = 2;
const int DG0_DDG0_INDEX            = 0;
const int DCA_R_CHI2_INDEX          = 1;

// Physics Distributions - we use these indicies to index single roo variables
// in order to enforce the 1-1 correspondance of PDF histogram seeds to roo vars
const int NUMBER_OF_VARS = 9;
const int DG0_INDEX                = 0; 
const int DDG0_INDEX               = 1; 
const int CHI2_INDEX               = 2;
const int DCA_R_INDEX              = 3;
const int RPC1DCA_INDEX            = 4; // also use in Wness
const int RPC3DCA_INDEX            = 5; // also use in Wness
const int FVTX_DRxDTHETA_INDEX     = 6;
const int FVTX_DPHI_INDEX          = 7; // also use in Wness
const int FVTX_CONE_INDEX          = 8;

// access all variables by name (reuse other indicies, but continue counting here
// INDEX still accesses correct variable, but we want to access a different set here) 
const int NUMBER_OF_TREE_VARIABLES = 26;

// ongoing enumeration of variables
const int RUN_NUMBER_INDEX         = 9 ; 
const int EVT_NUMBER_INDEX         = 10; 
const int EVT_BBC_INDEX            = 11; 
const int CLOCKCROSS_INDEX         = 12; 
const int WNESS_INDEX              = 13; 
const int CHARGE_INDEX             = 14; 
const int PT_INDEX                 = 15; 
const int PZ_INDEX                 = 16; 
const int PHI_INDEX                = 17; 
const int ETA_INDEX                = 18; 
const int PHI_ST1_INDEX            = 19; 
const int PHI_ST2_INDEX            = 20; 
const int PHI_ST3_INDEX            = 21; 
const int THETA_INDEX              = 22; 
const int DW13_INDEX               = 23; 
const int DW23_INDEX               = 24; 
const int DCA_Z_INDEX              = 25;

// Muon Background Streams
const static int NUMBER_OF_MUON_BACKGROUNDS  =  8;
const int DIRECT_PHOTON_INDEX         =  0;
const int ONIUM_INDEX                 =  1;
const int ONLY_Z_INDEX                =  2;
const int OPEN_BOTTOM_INDEX           =  3;
const int OPEN_CHARM_INDEX            =  4;
const int W_HAD_INDEX                 =  5;
const int W_TAU_INDEX                 =  6;
const int Z_INDEX                     =  7;

const int NUMBER_OF_ETA_BINS = 15;

// Signal and Background indicies - SingleMuonsLikelihood
const int NUMBER_OF_DATA_STREAMS = 2;
const int BACKGROUND_INDEX       = 0; // hadronic bak
const int SIGNAL_INDEX           = 1; // W sim

// Signal and Background indicies - d23VsWness
const int NUMBER_OF_MUON_STREAMS = 3;
const int W_INDEX     = 0;
const int HAD_INDEX   = 1;
const int BAK_INDEX   = 2;

// Roo Variables - dw23VsWness
const int NUMBER_OF_EXTRAPOLATION_VARS = 2;
const int DW23_EXTRAP_INDEX = 0;
const int ETA_EXTRAP_INDEX  = 1;

// Roo Vars - dw23VsWness
const int NUMBER_OF_SBR_SEED_VARS = 2;
const int NUMBER_OF_SBR_VARS      = 6; 
const int DW23_SBR_INDEX          = 0;
const int ETA_SBR_INDEX           = 1;
const int WNESS_SBR_INDEX         = 2;
const int W_COUNT_INDEX           = 3;
const int FAKE_MU_COUNT_INDEX     = 4;
const int MU_BAK_COUNT_INDEX      = 5;
const int SBR_PDF_BINS            = 30;
const int SBR_SMOOTHING           = 6;

// Charge selection
const int NUMBER_OF_CHARGE_CONFIGS = 2;
const int CHARGE_POS = 1;
const int CHARGE_NEG = 0;

// Arm selection
const int NUMBER_OF_ARM_CONFIGS = 2;
const int ARM_NORTH = 1;
const int ARM_SOUTH = 0;

// Likelihood Selection - SingleMuonsLikelihood 
const int NUMBER_OF_LIKELIHOODS = 6;
const int RPC1           = 0; // fvtx is bad
const int RPC3           = 1; // fvtx is bad
const int RPC13          = 2; // fvtx is bad
const int RPC1_AND_FVTX  = 3; // fvtx is good
const int RPC3_AND_FVTX  = 4; // fvtx is good
const int RPC13_AND_FVTX = 5; // fvtx is good
// Add likelihoods to the end - index to NUMBER_OF_LIKELIHOODS

// RooVar setup
const int NUMBER_OF_CORRELATED_ROO_VARS = 2;
const int ROO_VAR_X = 0;
const int ROO_VAR_Y = 1;

// Math
static const float PI = 3.1415926;

#endif
