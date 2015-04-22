#include "../src/SingleMuonsConsts.h"
#include <iostream>
#include <iomanip>
// Option 1: Read Histos and Fit
// Option 0: Make Histos and Fit (do for new trees, at least once)
// setPDFCondition: use -1 for standard running, and positive numbers for debugging (see class def: dw23VsWness)
void Run_dw23WnessFitter(int option = 0,int setPDFCondition = -1, int smooth = 6)
{
	std::string lib = "libSingleMuonsWAnalysis.so";
	gSystem->Load(lib.c_str());
	gStyle->SetOptFit();
	std::string directory = baseDirectory + macrosDirectory + rootTreesDirectory;
	std::string figsDirectory = baseDirectory + figuresDirectory; 
	std::string treeFile = directory + "Run13WnessTreeWithFvtxAndRpc_Data.root";
	std::string histoFile = "Run13_dw23FittingPlots.root";
	std::string muBakSetupLog = baseDirectory + logDirectory + "MuonBackgroundSetup.txt";
	
	// Scale Factors
	//double efficiency_factor = 0.407;
  double efficiency_factor = 1.0;
	double dimuon_factor[NUMBER_OF_MUON_BACKGROUNDS];
	double muon_bk_xsection[NUMBER_OF_MUON_BACKGROUNDS];
	double muon_bk_generated_events[NUMBER_OF_MUON_BACKGROUNDS];
	double run13_luminosity = 228.;
	double scale_factor[NUMBER_OF_MUON_BACKGROUNDS];

	dimuon_factor[ DIRECT_PHOTON_INDEX ] = 1.4;
	dimuon_factor[ ONIUM_INDEX         ] = 1.3;
	dimuon_factor[ ONLY_Z_INDEX        ] = 2.2; // was 1 wp
	dimuon_factor[ OPEN_BOTTOM_INDEX   ] = .77;
	dimuon_factor[ OPEN_CHARM_INDEX    ] = 3.4;
	dimuon_factor[ W_HAD_INDEX         ] = 1  ;
	dimuon_factor[ W_TAU_INDEX         ] = 1  ;
	dimuon_factor[ Z_INDEX             ] = 2.2;

	muon_bk_xsection[ DIRECT_PHOTON_INDEX ] = pow(10,7)*5.3200 ;
	muon_bk_xsection[ ONIUM_INDEX         ] = pow(10,8)*1.3500 ;
	muon_bk_xsection[ ONLY_Z_INDEX        ] = pow(10,2)*(-3.37);  // was 1.33 wp
	muon_bk_xsection[ OPEN_BOTTOM_INDEX   ] = pow(10,6)*7.3000 ;
	muon_bk_xsection[ OPEN_CHARM_INDEX    ] = pow(10,8)*5.7100 ;
	muon_bk_xsection[ W_HAD_INDEX         ] = pow(10,3)*1.6600 ;
	muon_bk_xsection[ W_TAU_INDEX         ] = pow(10,3)*1.6600 ;
	muon_bk_xsection[ Z_INDEX             ] = pow(10,4)*1.5900 ;

    // Old values
	/*
	muon_bk_generated_events[ DIRECT_PHOTON_INDEX ] = 6400000  ;
	muon_bk_generated_events[ ONIUM_INDEX         ] = 55470000 ;
	muon_bk_generated_events[ ONLY_Z_INDEX        ] = 106500   ;
	muon_bk_generated_events[ OPEN_BOTTOM_INDEX   ] = 4003000  ;
	muon_bk_generated_events[ OPEN_CHARM_INDEX    ] = 134000000;
	muon_bk_generated_events[ W_HAD_INDEX         ] = 81000    ;
	muon_bk_generated_events[ W_TAU_INDEX         ] = 82000    ;
	muon_bk_generated_events[ Z_INDEX             ] = 245200   ;
	*/

	// New Values
	muon_bk_generated_events[ DIRECT_PHOTON_INDEX ] = 58420000  ;//dy         205000    ; //
	muon_bk_generated_events[ ONIUM_INDEX         ] = 150420000 ;//onium      15264000  ; //
	muon_bk_generated_events[ ONLY_Z_INDEX        ] = 173100    ;//onlyz      552000    ; //
	muon_bk_generated_events[ OPEN_BOTTOM_INDEX   ] = 7363000   ;//openbottom 8292000   ; //
	muon_bk_generated_events[ OPEN_CHARM_INDEX    ] = 584940000 ;//opencharm  15156000  ; // 
	muon_bk_generated_events[ W_HAD_INDEX         ] = 342000    ;//whad       200000    ; //
	muon_bk_generated_events[ W_TAU_INDEX         ] = 343000    ;//wtau       197000    ; //
	muon_bk_generated_events[ Z_INDEX             ] = 292900    ;//z_sum      1887000   ; //
	
	// Daniel Example:	eff_scaling_factor*mu_bg_dimuon_factor[0]*mu_bg_cross_section[0]/(1000*mu_bg_generated_events[0])*228/1.4
	scale_factor[ DIRECT_PHOTON_INDEX ] = efficiency_factor*dimuon_factor[ DIRECT_PHOTON_INDEX ] * ( muon_bk_xsection[ DIRECT_PHOTON_INDEX ]/(1000.0*muon_bk_generated_events[ DIRECT_PHOTON_INDEX ]) ) * ( run13_luminosity/1.4 );
	scale_factor[ ONIUM_INDEX         ] = efficiency_factor*dimuon_factor[ ONIUM_INDEX         ] * ( muon_bk_xsection[ ONIUM_INDEX         ]/(1000.0*muon_bk_generated_events[ ONIUM_INDEX         ]) ) * ( run13_luminosity/1.4 );
	scale_factor[ ONLY_Z_INDEX        ] = efficiency_factor*dimuon_factor[ ONLY_Z_INDEX        ] * ( muon_bk_xsection[ ONLY_Z_INDEX        ]/(1000.0*muon_bk_generated_events[ ONLY_Z_INDEX        ]) ) * ( run13_luminosity/1.4 );
	scale_factor[ OPEN_BOTTOM_INDEX   ] = efficiency_factor*dimuon_factor[ OPEN_BOTTOM_INDEX   ] * ( muon_bk_xsection[ OPEN_BOTTOM_INDEX   ]/(1000.0*muon_bk_generated_events[ OPEN_BOTTOM_INDEX   ]) ) * ( run13_luminosity/1.4 );
	scale_factor[ OPEN_CHARM_INDEX    ] = efficiency_factor*dimuon_factor[ OPEN_CHARM_INDEX    ] * ( muon_bk_xsection[ OPEN_CHARM_INDEX    ]/(1000.0*muon_bk_generated_events[ OPEN_CHARM_INDEX    ]) ) * ( run13_luminosity/1.4 );
	scale_factor[ W_HAD_INDEX         ] = efficiency_factor*dimuon_factor[ W_HAD_INDEX         ] * ( muon_bk_xsection[ W_HAD_INDEX         ]/(1000.0*muon_bk_generated_events[ W_HAD_INDEX         ]) ) * ( run13_luminosity/1.4 );
	scale_factor[ W_TAU_INDEX         ] = efficiency_factor*dimuon_factor[ W_TAU_INDEX         ] * ( muon_bk_xsection[ W_TAU_INDEX         ]/(1000.0*muon_bk_generated_events[ W_TAU_INDEX         ]) ) * ( run13_luminosity/1.4 );
	scale_factor[ Z_INDEX             ] = efficiency_factor*dimuon_factor[ Z_INDEX             ] * ( muon_bk_xsection[ Z_INDEX             ]/(1000.0*muon_bk_generated_events[ Z_INDEX             ]) ) * ( run13_luminosity/1.4 );

	std::ofstream logfile(muBakSetupLog.c_str());
	logfile << std::setw(15) << "#Muon_Type" << std::setw(15) << "scale_fac"                         << std::setw(15) << "xsect"                                 << std::setw(15) << "nevents"                                       << std::setw(15) << "dimu_fac"                           << std::endl;
	logfile << std::setw(15) << "dy"         << std::setw(15) << scale_factor[ DIRECT_PHOTON_INDEX ] << std::setw(15) << muon_bk_xsection[ DIRECT_PHOTON_INDEX ] << std::setw(15) << muon_bk_generated_events[ DIRECT_PHOTON_INDEX ] << std::setw(15) << dimuon_factor[ DIRECT_PHOTON_INDEX ] << std::endl;
	logfile << std::setw(15) << "onium"      << std::setw(15) << scale_factor[ ONIUM_INDEX         ] << std::setw(15) << muon_bk_xsection[ ONIUM_INDEX         ] << std::setw(15) << muon_bk_generated_events[ ONIUM_INDEX         ] << std::setw(15) << dimuon_factor[ ONIUM_INDEX         ] << std::endl;
	logfile << std::setw(15) << "onlyz"      << std::setw(15) << scale_factor[ ONLY_Z_INDEX        ] << std::setw(15) << muon_bk_xsection[ ONLY_Z_INDEX        ] << std::setw(15) << muon_bk_generated_events[ ONLY_Z_INDEX        ] << std::setw(15) << dimuon_factor[ ONLY_Z_INDEX        ] << std::endl;
	logfile << std::setw(15) << "bottom"     << std::setw(15) << scale_factor[ OPEN_BOTTOM_INDEX   ] << std::setw(15) << muon_bk_xsection[ OPEN_BOTTOM_INDEX   ] << std::setw(15) << muon_bk_generated_events[ OPEN_BOTTOM_INDEX   ] << std::setw(15) << dimuon_factor[ OPEN_BOTTOM_INDEX   ] << std::endl;
	logfile << std::setw(15) << "charm"      << std::setw(15) << scale_factor[ OPEN_CHARM_INDEX    ] << std::setw(15) << muon_bk_xsection[ OPEN_CHARM_INDEX    ] << std::setw(15) << muon_bk_generated_events[ OPEN_CHARM_INDEX    ] << std::setw(15) << dimuon_factor[ OPEN_CHARM_INDEX    ] << std::endl;
	logfile << std::setw(15) << "whad"       << std::setw(15) << scale_factor[ W_HAD_INDEX         ] << std::setw(15) << muon_bk_xsection[ W_HAD_INDEX         ] << std::setw(15) << muon_bk_generated_events[ W_HAD_INDEX         ] << std::setw(15) << dimuon_factor[ W_HAD_INDEX         ] << std::endl;
	logfile << std::setw(15) << "wtau"       << std::setw(15) << scale_factor[ W_TAU_INDEX         ] << std::setw(15) << muon_bk_xsection[ W_TAU_INDEX         ] << std::setw(15) << muon_bk_generated_events[ W_TAU_INDEX         ] << std::setw(15) << dimuon_factor[ W_TAU_INDEX         ] << std::endl;
	logfile << std::setw(15) << "z"          << std::setw(15) << scale_factor[ Z_INDEX             ] << std::setw(15) << muon_bk_xsection[ Z_INDEX             ] << std::setw(15) << muon_bk_generated_events[ Z_INDEX             ] << std::setw(15) << dimuon_factor[ Z_INDEX             ] << std::endl;
	logfile << "# Overall Efficiency Factor " << std::endl;
	logfile << efficiency_factor << std::endl;
	logfile << "# Run 13 Luminosity" << std::endl;
	logfile << run13_luminosity << std::endl;
	logfile << "# Lumoninosity Scale Factor" << std::endl;
	logfile << 1.4 << std::endl;
	logfile << "# Example Calc:" << std::endl;
	logfile << "# eff_fac * dimu_fac * xsect / (1000 * nevents) * run13_lumi / 1.4";
	logfile.close();
	
	// Data Plots - Do Loop Over Tree
	if( option == 0 )
	{
		// Hadronic Background Extrapolation
		dw23VsWness fitter;
		//fitter.SetNoDestruct();

		fitter.SelectPdfGroup(setPDFCondition);
		//fitter.SetWnessBinning(0.1,0.9,10);
    fitter.SetPDFSmoothing(smooth);
		fitter.dw23Extrapolate_LoopAndFit(treeFile, histoFile);
		fitter.Write();

		// Signal To Background Extraction
		fitter.AddMuonBackground("dy"         , "/phenix/u/beaumim/W_Analysis/macros/rootTrees/Run13WnessTree_DirectPhoton.root", scale_factor[ DIRECT_PHOTON_INDEX ]);
		fitter.AddMuonBackground("onium"      , "/phenix/u/beaumim/W_Analysis/macros/rootTrees/Run13WnessTree_Onium.root"       , scale_factor[ ONIUM_INDEX         ]);
		fitter.AddMuonBackground("onlyz"      , "/phenix/u/beaumim/W_Analysis/macros/rootTrees/Run13WnessTree_OnlyZSum.root"    , scale_factor[ ONLY_Z_INDEX        ]);
		fitter.AddMuonBackground("openbottom" , "/phenix/u/beaumim/W_Analysis/macros/rootTrees/Run13WnessTree_OpenBottom.root"  , scale_factor[ OPEN_BOTTOM_INDEX   ]);
		fitter.AddMuonBackground("opencharm"  , "/phenix/u/beaumim/W_Analysis/macros/rootTrees/Run13WnessTree_OpenCharm.root"   , scale_factor[ OPEN_CHARM_INDEX    ]);
		fitter.AddMuonBackground("whad"       , "/phenix/u/beaumim/W_Analysis/macros/rootTrees/Run13WnessTree_WHad.root"        , scale_factor[ W_HAD_INDEX         ]);
		fitter.AddMuonBackground("wtau"       , "/phenix/u/beaumim/W_Analysis/macros/rootTrees/Run13WnessTree_WTau.root"        , scale_factor[ W_TAU_INDEX         ]);
		fitter.AddMuonBackground("z"          , "/phenix/u/beaumim/W_Analysis/macros/rootTrees/Run13WnessTree_ZSum.root"        , scale_factor[ Z_INDEX             ]);
		
		fitter.AddWSignal("/phenix/u/beaumim/W_Analysis/macros/rootTrees/Run13WnessTreeWithFvtxAndRpc_Simu393888.root");
		fitter.GeneratePDFs();
		fitter.SaveCanvases("/direct/phenix+spin2/beaumim/WAnalysisRun13/AnalysisCode/figs/SBR_Fits/");
	}
	else if( option == 1 )
	{
		// Hadronic Background Extrapolation
		dw23VsWness fitter;
		fitter.SelectPdfGroup(setPDFCondition);
		//fitter.SetWnessBinning(0.1,0.9,10);
		//fitter.SetNoDestruct();
    fitter.SetPDFSmoothing(smooth);
		fitter.dw23Extrapolate_ReadAndFit(treeFile, histoFile);
		
		// Signal To Background Extraction
		fitter.AddMuonBackground("dy"         ,"/phenix/u/beaumim/W_Analysis/macros/rootTrees/Run13WnessTree_DirectPhoton.root", scale_factor[ DIRECT_PHOTON_INDEX ]);
		fitter.AddMuonBackground("onium"      ,"/phenix/u/beaumim/W_Analysis/macros/rootTrees/Run13WnessTree_Onium.root"       , scale_factor[ ONIUM_INDEX         ]);
		fitter.AddMuonBackground("onlyz"      ,"/phenix/u/beaumim/W_Analysis/macros/rootTrees/Run13WnessTree_OnlyZSum.root"    , scale_factor[ ONLY_Z_INDEX        ]);
		fitter.AddMuonBackground("openbottom" ,"/phenix/u/beaumim/W_Analysis/macros/rootTrees/Run13WnessTree_OpenBottom.root"  , scale_factor[ OPEN_BOTTOM_INDEX   ]);
		fitter.AddMuonBackground("opencharm"  ,"/phenix/u/beaumim/W_Analysis/macros/rootTrees/Run13WnessTree_OpenCharm.root"   , scale_factor[ OPEN_CHARM_INDEX    ]);
		fitter.AddMuonBackground("whad"       ,"/phenix/u/beaumim/W_Analysis/macros/rootTrees/Run13WnessTree_WHad.root"        , scale_factor[ W_HAD_INDEX         ]);
		fitter.AddMuonBackground("wtau"       ,"/phenix/u/beaumim/W_Analysis/macros/rootTrees/Run13WnessTree_WTau.root"        , scale_factor[ W_TAU_INDEX         ]);
		fitter.AddMuonBackground("z"          ,"/phenix/u/beaumim/W_Analysis/macros/rootTrees/Run13WnessTree_ZSum.root"        , scale_factor[ Z_INDEX             ]);
		
		fitter.AddWSignal("/phenix/u/beaumim/W_Analysis/macros/rootTrees/Run13WnessTreeWithFvtxAndRpc_Simu393888.root");
		fitter.GeneratePDFs();
		fitter.SaveCanvases("/direct/phenix+spin2/beaumim/WAnalysisRun13/AnalysisCode/figs/SBR_Fits/");
	}
}
