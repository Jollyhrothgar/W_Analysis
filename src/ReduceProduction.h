#ifndef __REDUCE_PRODUCTION_H__
#define __REDUCE_PRODUCTION_H__

#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <time.h>
#include <map>
#include <iomanip>

#include "TString.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2.h"
#include "TTree.h"

#include "SingleMuons.h"
#include "SingleMuonsConsts.h"

int ReduceProduction( std::string inTreeFile = "", std::string inTreeName = "", std::string outTreeFile = "" , std::string outTreeName = "");

#endif
