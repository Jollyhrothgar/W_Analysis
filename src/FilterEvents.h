#ifndef __BINARY_SEARCH_MERGE__
#define __BINARY_SEARCH_MERGE__

#include <TFile.h>
#include <TTree.h>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <iomanip>
#include <utility>
#include <map>
#include <algorithm>
#include "TPRegexp.h"
#include "TString.h"

#include "SingleMuonsConsts.h"
#include "BasicCutTreeVariables.h"
#include "HelperFunctions.h"
#include "Run13Triggers.h"

int FilterEvents( std::string filterTreeName, std::string uniqTreeName);
#endif
