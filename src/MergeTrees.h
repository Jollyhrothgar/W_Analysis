#ifndef __MERGE_TREES__
#define __MERGE_TREES__

#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "RooRealVar.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"
#include "RooProdPdf.h"
#include "RooFormulaVar.h"

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

#include "SingleMuonsConsts.h"
#include "HelperFunctions.h"
#include "SingleMuons.h"
#include "SingleMuonsLikelihood.h"
#include "BasicCutTreeVariables.h"

// This function takes a tree list of the reduced tree type in this analysis
// and merges them all into one tree. This is done because sometimes, using
// hadd can give unexpected results.
int MergeTrees(std::string treeList, std::string outTree);

#endif
