#ifndef __CALCULATE_LIKELIHOOD_FUNCTIONS__
#define __CALCULATE_LIKELIHOOD_FUNCTIONS__

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

int CalculateLikelihoodFunctions(std::string kinematicDistributions, std::string runOverTree, std::string writeToThisFile);
#endif
