#ifndef _ASYMMETRY_CALCULATOR_H_
#define _ASYMMETRY_CALCULATOR_H_
// Root
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TGraph.h"
#include "TGraphErrors.h"

// STL
#include <iostream>
#include <cmath>
#include <memory>
#include <iomanip>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>
#include <set>

// Custom
#include "W2eGetPol.h"
#include "BasicCutTreeVariables.h"
#include "SingleMuonsConsts.h"
#include "HelperFunctions.h"

class AsymmetryCalculator
{
 public:
  AsymmetryCalculator();
  int LoadData(const std::string& fileName, const std::string& badSpinDBRunsName); /** Loads data from newsngmuons_basic_cut */
  int Run(); /** Loads subroutines and runs the show */
  int SaveHistograms(const std::string& saveTo); /** Saves all histograms in registry to a file specified in the argument */
  int SaveHelicityCount(const std::string&); /** Saves all helicity counds to a .csv file and a raw text file */
  int SaveRawAsymmetries(const std::string& rawAsymFile); /** Saves raw single and double asymmetries to a file */
  int SetupEtaBins(int number_of_eta_bins); /** Divide the nominal eta range of 1.1 to 2.6 into number_of_eta_bins bins, equally spaced */
  ~AsymmetryCalculator();
 private:

  // Shared Pointers / Stuff that ROOT hates
#ifndef __CINT__
  // definitions used in this class - internal defintion
  typedef std::shared_ptr<TFile> sTFile;
  typedef std::shared_ptr<TH1F> sTH1F;
  typedef std::shared_ptr<TGraph> sTGraph;
  typedef std::shared_ptr<TGraphErrors> sTGraphErrors;
#else
  // making CINT happy with compiled code externally
  class sTFile;
  class TTree;
  class sTH1F;
  class sTGraphErrors;
#endif 

  // Sort the tree by run to avoid unneccessary calls to the database,
  // since each spin information is constnat over the course of a run.
  std::vector< std::pair <float, float> > AsymEtaBins;
  std::set<int> badSpinDBRuns;
  std::set<int> SortedRuns;
  int GetAsymEtaBin(float eta); /** Returns the index of the eta bin corresponding to the value of the argument, eta */

  Long64_t total_tracks; /** number of entries in newsngmuons_basic_cut */
  W2eGetPol pol; /** Database interface for extracting spin information for runs */

  int CalculateAsymmetries(); /** Calculates asymmetry from various spin pattern counts. */
  // Helicity Accounting
  std::vector< std::vector< std::vector < std::vector<int> > > > patternCount;  /**Dimensions: arm charge spin_pat eta_bin */  
  std::vector< std::vector< std::vector<float> > >  rawSingleAsymmetryPosEta;     /** Dimensions: arm charge eta_bin */
  std::vector< std::vector< std::vector<float> > >  rawSingleAsymmetryNegEta;     /** Dimensions: arm charge eta_bin */
  std::vector< std::vector< std::vector<float> > >  rawSingleAsymmetryPosEtaErr;  /** Dimensions: arm charge eta_bin */
  std::vector< std::vector< std::vector<float> > >  rawSingleAsymmetryNegEtaErr;  /** Dimensions: arm charge eta_bin */
  std::vector< std::vector< std::vector<float> > >  rawDoubleAsymmetry;           /** Dimensions: arm charge eta_bin */
  std::vector< std::vector< std::vector<float> > >  rawDoubleAsymmetryErr;        /** Dimensions: arm charge eta_bin */

 public:
  // wp implement filtering based on keys which are filled in HelicityCrossCheck, filter events for comparison w/fracesca
  int DumpHelicityInformation(const std::string& outFileNameStub, int arm, int charge, int pat); /** Must be Called BEFORE Run() 
    Dump helicity cross check info to file, for a specific arm, charge and spin pattern. */
  int XCheckPatternFile(const std::string& inFileName, int spin_pattern_check); /** Read in the event data from the pattern check file
    Then, dump out information (including spin pattern) from my tree. File should contain data formated by 
    "DumpHelicityInformation" only. MUST BE CALLED AFTER Run(). */
 private:
  std::map<std::string, std::ofstream*> HelicityCrossCheck; /** stores an output file for cross check; key generated according to: ARM_NORTH, CHARGE_NEG, spinPattern::PP -> "110" */
  int SplitKey(std::string key, int& arm, int& charge, int& pat); /** Takes a key for HelicityCrossCheck and extracts arm, charge and pattern index from the key-string */
  int Register(TObject* obj); /** Register each object that we want to write to a file */
  std::vector<TObject*> registry; /** Use for writing display objects to file */
  bool CutEvent(const BasicCutTreeVariables& v); /** Returns true if the event should be skipped */

  // Use forward declarations to reassure root.
  // Using std::shared_ptr to avoid having to deal with cleaning up memory manually
  sTFile dataFile; /** Run 13 pp510 GeV data + basic cut file container */
  TTree* dataTree; /** Run 13 pp510 GeV data + basic cut */

  // Histograms
  sTH1F eventsInRun; /** plots Wness > 0.92 events per run, converting run to run index - no reverse-mapping is done */
  sTH1F polarizationInRunBlue; 
  sTH1F polarizationInRunYellow;
  sTH1F crossingCount;

  // Graphs
  /** plots the asymmetry vs pseudorapidty (integrated over pseudorapidity) assuming yellow beam is polarized target. */
  std::vector< std::vector < std::vector < TGraphErrors > > > yellowTargetAsym; 
  
  /** "" "" assuming blue beam is polarized target */
  std::vector< std::vector < std::vector < TGraphErrors > > > blueTargetAsym;

  /** "" "" assuming yellow beam is polarized target, split into eta bins */
  std::vector< std::vector < std::vector < std::vector < TGraphErrors > > > > yellowTargetAsymEta; 
  
  /** "" "" assuming blue beam is polarized target, split into eta bins */
  std::vector< std::vector < std::vector < std::vector < TGraphErrors > > > > blueTargetAsymEta; 

  int SetupHistograms(); /** Makes all the histograms used in this analysis. 
                           Call only after eta bins have been defined for good.*/
  int SortTreeByRun(); /** maps each unique runnumber to a list of events in that run */
  int Loop(); /** Loops over dataTree, extracting crossing + asymmetry info */
  std::string GetPatternString(int pat_i);
};

#endif
