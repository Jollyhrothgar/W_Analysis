#ifndef __PLOTTER_H__
#define __PLOTTER_H__

//STL
#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <map>
#include <cmath>

// Root
#include "TROOT.h"
#include "TTree.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TGraph2D.h"
#include "TGraph2DErrors.h"
#include "TCanvas.h"
#include "TProfile.h"
#include "TLatex.h"
#include "TFile.h"
#include "TF1.h"
#include "TF2.h"

// My Headers
#include "SingleMuonsConsts.h"
#include "HelperFunctions.h"

class Plotter
{
	public:
		Plotter();
		Plotter(std::string sigFile, std::string bakFile, std::string tree_name, std::string outArea);
		~Plotter();
		int SetSignalFile(std::string sigFile );
		int SetBackgroundFile(std::string bakFile );
		int SetTreeName(std::string treeName);
		int SetFigureOutputArea(std::string outArea);
		int Init();
		int Loop();
		int DrawPlots();
	private:
		/* Tree Vars */ 
		// dimension: NUMBER_OF_DATA_STREAMS (use BACKGROUND_INDEX and SIGNAL_INDEX)
		std::vector< TFile* > treeFile;
		std::vector< TTree* > plotTree;
		std::string signalFile;
		std::string backgroundFile;
		std::string treeName;
		std::string plotsDirectory;
    int PLOT_VARS;
		int   Run_Number  [NUMBER_OF_DATA_STREAMS];
		int   Evt_Number  [NUMBER_OF_DATA_STREAMS];
		float Evt_bbcZ    [NUMBER_OF_DATA_STREAMS];
		int   clockcross  [NUMBER_OF_DATA_STREAMS];
		float Wness       [NUMBER_OF_DATA_STREAMS];
		float charge      [NUMBER_OF_DATA_STREAMS];
		float pT          [NUMBER_OF_DATA_STREAMS];
		float pz          [NUMBER_OF_DATA_STREAMS];
		float phi         [NUMBER_OF_DATA_STREAMS];
		float eta         [NUMBER_OF_DATA_STREAMS];
		float DG0         [NUMBER_OF_DATA_STREAMS];
		float DDG0        [NUMBER_OF_DATA_STREAMS];
		float chi2        [NUMBER_OF_DATA_STREAMS];
		float DCA_z       [NUMBER_OF_DATA_STREAMS];
		float DCA_r       [NUMBER_OF_DATA_STREAMS];
		float phiSt1      [NUMBER_OF_DATA_STREAMS];
		float phiSt2      [NUMBER_OF_DATA_STREAMS];
		float phiSt3      [NUMBER_OF_DATA_STREAMS];
		float theta       [NUMBER_OF_DATA_STREAMS];
		//float dphi23      [NUMBER_OF_DATA_STREAMS];
		//float dphi13      [NUMBER_OF_DATA_STREAMS];
		float dw13        [NUMBER_OF_DATA_STREAMS];
		float dw23        [NUMBER_OF_DATA_STREAMS];
		float Rpc1dca     [NUMBER_OF_DATA_STREAMS];
		float Rpc3dca     [NUMBER_OF_DATA_STREAMS];
		float fvtx_dtheta [NUMBER_OF_DATA_STREAMS];
		float fvtx_dphi   [NUMBER_OF_DATA_STREAMS];
		float fvtx_dr     [NUMBER_OF_DATA_STREAMS];
		//float fvtx_cone   [NUMBER_OF_DATA_STREAMS];

		// All variables vs all other variables.
		// only plot the ones that we wish
		// Dimension: [var][arm][charge][data]
		// var: selects distribution to plot against vs"Var"
		// arm: selects north or south arm
		// charge: selects positive or negative charge
		std::vector< std::vector< std::vector< std::vector <TH2F*> > > > vsDG0;
		std::vector< std::vector< std::vector< std::vector <TH2F*> > > > vsDDG0;
		std::vector< std::vector< std::vector< std::vector <TH2F*> > > > vsRpc3Dca;
		std::vector< std::vector< std::vector< std::vector <TH2F*> > > > vsRpc1Dca;
		std::vector< std::vector< std::vector< std::vector <TH2F*> > > > vsFvtxDrDtheta;
		std::vector< std::vector< std::vector< std::vector <TH2F*> > > > vsFvtxDphi;
		std::vector< std::vector< std::vector< std::vector <TH2F*> > > > vsChi2;
		std::vector< std::vector< std::vector< std::vector <TH2F*> > > > vsDCA_r;

		TCanvas* varVsDG0;
		TCanvas* varVsDDG0;
		TCanvas* varVsRpc3Dca;
		TCanvas* varVsRpc1Dca;
		TCanvas* varVsFvtxDrDtheta;
		TCanvas* varVsFvtxDphi;
		TCanvas* varVsChi2;

		int GrowVectors();
		float GetVar(int var_i, int data_i);
		int RegisterObject( TObject* );
		std::vector<TObject*> ObjectRegistry;
		
		int InitTrees();
		int InitHistograms();
};

#endif
