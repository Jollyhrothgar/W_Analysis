#ifndef __REDUCE_PRODUCTION_CHAIN_H__
#define __REDUCE_PRODUCTION_CHAIN_H__

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
#include "TChain.h"

#include "SingleMuons.h"
#include "SingleMuonsConsts.h"

int ReduceProductionChain(TChain* ch, std::string outTree);

#endif
