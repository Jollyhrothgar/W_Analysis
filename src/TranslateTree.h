#ifndef __TRANSLATE_TREE_H__
#define __TRANSLATE_TREE_H__

#include "SingleMuonsConsts.h"
#include "BasicCutTreeVariables.h"
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include "TTree.h"
#include "TBranch.h"
#include "TLeaf.h"
#include "TFile.h"


int TranslateTree(int analyzerChoice, std::string inputFile, std::string inputTree, std::string outputFile, std::string outputTree);
#endif
