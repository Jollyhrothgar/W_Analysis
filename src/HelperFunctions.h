#ifndef __HELPER_FUNCTIONS_H__
#define __HELPER_FUNCTIONS_H__
#include <sstream>
#include <iostream>
#include <string>
#include <fstream> 
#include <vector>
#include <map>
#include <cmath>
#include <fstream>
#include <TTree.h>
#include <TFile.h>
#include "BasicCutTreeVariables.h"
#include "SingleMuonsConsts.h"

unsigned int StringSplit(const std::string& , std::vector<std::string>& );

// splits the arg vector in half, and returns two halves. v1 is first half, v2
// is last half. in the case of odd# vector, the second gets more entries by 1.
/*
   template<typename T> 
   void VectorSplit(std::vector<T> &in, std::vector<T> &v1, std::vector<T> &v2)
   {
   typename std::vector<T>::iterator midpoint = in.begin() + in.size() / 2; 
   std::vector<T> vect1(in.begin(), midpoint);
   std::vector<T> vect2(midpoint, in.end());
   v1 = vect1;
   v2 = vect2;
   }
   template<typename T>
   void SerializeVector(std::vector<T> &in,const std::string &filename)
   {
   ofstream outFile(filename.c_str());
   for(int i=0; i<in.size(); i++)
   {
   outFile << in[i] << std::endl;
   }
   outFile.close();
   }
 */

template<typename T> // repeat for all other vector containers, potentially!
void MakeArmChargeSpinEtaContainer(std::vector< std::vector < std::vector < std::vector< T> > > > & v, int eta_bins) {
  v.resize(NUMBER_OF_ARM_CONFIGS);
  for(auto i = v.begin(); i != v.end(); ++i) {
    i->resize(NUMBER_OF_CHARGE_CONFIGS);
    for(auto j = (*i).begin(); j != (*i).end(); ++j) {
      j->resize(NUMBER_OF_SPIN_PATTERNS);
      for(auto k = (*j).begin(); k != (*j).end(); ++k) {
        k->resize(eta_bins, 0);
      }
    }
  }
}

template<typename T> // repeat for all other vector containers, potentially!
void MakeArmChargeEtaContainer(std::vector< std::vector < std::vector< T> > > & v, int eta_bins) {
  v.resize(NUMBER_OF_ARM_CONFIGS);
  for(auto i = v.begin(); i != v.end(); ++i) {
    i->resize(NUMBER_OF_CHARGE_CONFIGS);
    for(auto j = (*i).begin(); j != (*i).end(); ++j) {
      j->resize(eta_bins, 0);
    }
  }
}

template<typename T> 
void MakeArmChargeSpinContainer(std::vector< std::vector< std::vector < T> > > &v) {
  v.resize(NUMBER_OF_ARM_CONFIGS);
  for(auto i = v.begin(); i != v.end(); ++i) {
    i->resize(NUMBER_OF_CHARGE_CONFIGS);
    for(auto j = (*i).begin(); j != (*i).end(); ++j) {
      j->resize(NUMBER_OF_SPIN_PATTERNS, 0);
    }
  }
}

bool IsSubstring(std::string,std::string);

// First argument is a file-handle string to a file with one file per line
// Second argument is an empty vector which is filled in the function, with one
// file handle per entry bin
//
// Returns the number of lines in the first argument file list (which should 
// correspond to the number of lines in the file)
unsigned int LoadFileListToVector(const std::string, std::vector<std::string>& );


// Returns the root name of a 1D physics variable distribution
std::string Make1DVarName(int var_i, int arm_i, int charge_i, int datatype_i);

// Returns the root name of a 2D physics variable distribution
std::string Make2DVarName(int var_i, int arm_i, int charge_i, int datatype_i);

// Returns the maximum value of a physics variable distribution.
float GetVarMax(int var_i);

// Updates the value max1 and max2 with the associated maximum set for the correllated distributions
// Order matters. For example, if you want to get the maximum values for DG0_vs_DDG0 (a 2D distribution)
// then you max1 is set to DG0 maximum and max2 is set to DDG0 maximum.
void GetCorrVarMax(int var_i, float& max1, float& max2);

// Returns the approprite index for each 1D correlated variable making a 2D distribution. For example,
// want to give the right RooRealVars to a RooDataHist or RooProdPdf variable. Say we are working with
// DG0_vs_DDG0 as represented by a RooDataHist - in this case, index_x is the index set to the DG0 variable
// and index_y is the index set to DDG0 RooRealVariable.
void GetCorrIndicies( int var_i, int& index_x, int& index_y );

// This is used mainly for plotting macros. You give the root-name stub (containing arm, charge and data index)
// as the fourth argument, and a root-title stub as the fifth argumen. Internally, these strings are set to the
// appropriate stub value, according to the configuration indicies (arm_i, charge_i, datatype_i).
// Example: arm_i=0, charge_1=0, datatype=0 : name_str = "Arm0_Charge0_data" and title_str is set to: "North Arm #mu+ Data"
void MakeNameTitleStubPair(int arm_i, int charge_i, int datatype_i, std::string& name_str, std::string& title_str);

// gets arm index based on z momentum for accessing right plots
int GetArmIndex   (float z_momentum  );

// gets charge index based on charge value
int GetChargeIndex(float charge_value);

// gets likelihood index based on RPC condition
int GetLikelihoodIndex(float rpc1dca, float rpc3dca, float fvtxdtheta, float fvtxdr, float fvtxdphi);

// Returns var name associated with index
std::string GetVarName(int var_i);

// Loops through simulation sources and makes a table of total events generated, 
// and calculates correction needed.
void Count_Generated_Events();

// Loops over a tree and checks for "nan" entries in Wness.
int CheckNan(const std::string& inFile, const std::string& logFile);
#endif
