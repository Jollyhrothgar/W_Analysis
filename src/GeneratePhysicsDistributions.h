#ifndef __GENERATE_PHYSICS_DISTRIBUTIONS__
#define __GENERATE_PHYSICS_DISTRIBUTIONS__

#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <time.h>
#include <map>
#include <iomanip>

#include "TPRegexp.h"
#include "TString.h"
#include "TObjArray.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2.h"
#include "TObjString.h"
#include "TTree.h"

#include "SingleMuonsConsts.h"
#include "SingleMuons.h"
#include "HelperFunctions.h"
#include "SingleMuonsLikelihood.h"

static const double degToRad = 0.0174532925;


int GeneratePhysicsDistributions( std::string nTupleDataFile, std::string nTupleSimulationFile , std::string PDFFileTag, bool checkBounds);

#endif
