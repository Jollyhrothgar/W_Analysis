#include "ReduceProduction.h"
#include "BasicCutTreeVariables.h"
#include "HelperFunctions.h"

int ReduceProduction( std::string inTreeFile, std::string inTreeName, std::string outTreeFile, std::string outTreeName)
{
	const double PI = 3.1415926;

	std::map <std::string, long double> basicTrackCut;
	basicTrackCut["AcceptedTracks"]       = 0.0;
	basicTrackCut["TotalTracks"]          = 0.0;
	basicTrackCut["_RpcHits"]             = 0.0;
	basicTrackCut["pT_high"]              = 0.0;
	basicTrackCut["pT_low" ]              = 0.0;
	basicTrackCut["lastGap"]              = 0.0;
	basicTrackCut["chi2"]                 = 0.0;
	basicTrackCut["DG0"]                  = 0.0;
	basicTrackCut["DDG0"]                 = 0.0;
	basicTrackCut["Evt_Nmu"]              = 0.0;
	basicTrackCut["p_low"]                = 0.0;
	basicTrackCut["p_high"]               = 0.0;
	basicTrackCut["Rpc1or3OutofRange"]    = 0.0;

  std::map<int,int> Evt_Nmu_Counter;
  for(int i = 0; i < 100; i++) {
    Evt_Nmu_Counter[i] = 0;
  }

	std::string runOverFileName = inTreeFile; 
	SingleMuons sm;
	std::string outName =  outTreeFile;

	// Create out file first, to avoid memory resident root trees.
	TFile* outRoot = new TFile(outName.c_str(),"RECREATE");
	TTree* OutTree = new TTree(outTreeName.c_str(),"Basic Cut for newsngmuons");
	BasicCutTreeVariables vars;
	vars.LinkTree(OutTree, WRITE);
	TFile * treeFile = new TFile(runOverFileName.c_str(), "READ");
	treeFile->cd();
	TTree* t = (TTree*)treeFile->Get(inTreeName.c_str());

	if(t)
	{
		if( t->GetEntries() < 1 )
		{
			//std::cout << "OPTION1" << std::endl;
			std::cout << "    no events in this file: " << runOverFileName << std::endl;
			delete treeFile;
			delete OutTree;
			outRoot->Close();
			delete outRoot;
			return -2;
		}
	}
	else
	{
			//std::cout << "OPTION2" << std::endl;
			std::cout << "   invalid tree pointer in this file: " << runOverFileName << std::endl;
			delete treeFile;
			delete OutTree;
			outRoot->Close();
			delete outRoot;
			return -2;
	}
	
	std::cout << "reducing: "  << runOverFileName << std::endl;


	// Prepare to loop over events
	sm.Initialize(t);
	Long64_t entries = t->GetEntries();
	for(Long64_t i = 0; i < entries; i++)
	{
		sm.GetEvent(i);
		// Analyze Reduction power of cuts
		if(sm._RpcHits < 1 ) basicTrackCut["_RpcHits"] += 1.0;
		for( unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(sm._RecoTracks); Track_i++)
		{
			basicTrackCut["TotalTracks"] += 1.0;
			if( sm.pT[Track_i]      < BASIC_CUT_pT_low) basicTrackCut["pT_low" ] += 1.0;
			if( sm.pT[Track_i]      > BASIC_CUT_pT_high) basicTrackCut["pT_high"] += 1.0;
			if( sm.lastGap[Track_i] != 4.0             ) basicTrackCut["lastGap"] += 1.0;
			if( sm.chi2[Track_i]    > BASIC_CUT_chi2   ) basicTrackCut["chi2"   ] += 1.0;
			if( sm.DG0[Track_i]     > BASIC_CUT_DG0    ) basicTrackCut["DG0"    ] += 1.0;
			if( sm.DDG0[Track_i]    > BASIC_CUT_DDG0   ) basicTrackCut["DDG0"   ] += 1.0;
			if( ((int) sm.Evt_Nmu[Track_i]) != ((int) 1)               ) {
       basicTrackCut["Evt_Nmu"] += 1.0; 
       std::cout << sm.Evt_Nmu[Track_i] << ", " << 1 << std::endl;
      }
			if( sm.p[Track_i]       < BASIC_CUT_p_low  ) basicTrackCut["p_low"  ] += 1.0;
			if( sm.p[Track_i]       > BASIC_CUT_p_high ) basicTrackCut["p_high" ] += 1.0;
			if( (sm.RpcMatchSt1_Rpc1dca[Track_i] > RPC_MAX_VALUE ) && ( sm.RpcMatchSt3_Rpc3dca[Track_i] > RPC_MAX_VALUE ) ) basicTrackCut["Rpc1or3OutofRange"] += 1.0; // RPC info must be present to do analysis.
		// Apply Cuts
    }

		for( unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(sm._RecoTracks); Track_i++)
		{
      /* 12 Oct 2014 
			if( sm.pT[Track_i]      < BASIC_CUT_pT_low ) continue;
			if( sm.pT[Track_i]      > BASIC_CUT_pT_high) continue;
			if( sm.lastGap[Track_i] != 4.0             ) continue;
			if( sm.chi2[Track_i]    > BASIC_CUT_chi2   ) continue;
			if( sm.DG0[Track_i]     > BASIC_CUT_DG0    ) continue;
			if( sm.DDG0[Track_i]    > BASIC_CUT_DDG0   ) continue;
			if( ((int)sm.Evt_Nmu[Track_i]) != ((int)1)               ) continue;
			if( sm.p[Track_i]       < BASIC_CUT_p_low  ) continue;
			if( sm.p[Track_i]       > BASIC_CUT_p_high ) continue;
			if( (sm.RpcMatchSt1_Rpc1dca[Track_i] > RPC_MAX_VALUE ) && ( sm.RpcMatchSt3_Rpc3dca[Track_i] > RPC_MAX_VALUE ) ) continue; // RPC info must be present to do analysis.
			if( (sm.RpcMatchSt1_Rpc1dca[Track_i] < 0.0 ) || (sm.RpcMatchSt3_Rpc3dca[Track_i] < 0.0) ) continue;
			if( (sm.RpcMatchSt1_Rpc1dca[Track_i] > RPC_MAX_VALUE ) && ( sm.RpcMatchSt3_Rpc3dca[Track_i] > RPC_MAX_VALUE ) ) std::cout << "RPC1/3 fail" << std::endl; // RPC info must be present to do analysis.
      */ 
      Evt_Nmu_Counter[sm.Evt_Nmu[Track_i] ]++;
			// make dw23, dw13 and fill Tree 
			// These are copied from Ralf
			// phys_dist[10] (dphi23) = fmod( atan2( st3y[mt_track], st3x[mt_track] ) - atan2( st2y[mt_track], st2x[mt_track] ) + 3*PI, 2*PI) - PI;
			//                          fmod[ (               vars.phiSt3                ) - (               phSt2                 ) + 3*PI, 2*PI ] - PI        
			// phys_dist[11] (dw23)   = phys_dist[10]*pT[mt_track]*sin(atan(pT[mt_track]/fabs(pz[mt_track])));
			// phys_dist[10] = (fmod(atan2(st2y[mt_track],st2x[mt_track])-atan2(st3y[mt_track],st3x[mt_track])+3*PI,2*PI)-PI);
			vars.phiSt1      = atan2(sm.ySt1[Track_i],sm.xSt1[Track_i]); // in radians			
			vars.phiSt2      = atan2(sm.ySt2[Track_i],sm.xSt2[Track_i]); // in radians			
			vars.phiSt3      = atan2(sm.ySt3[Track_i],sm.xSt3[Track_i]); // in radians			

			// need to move phi difference into correct range (-PI,PI) // wp
			vars.dphi23 = vars.phiSt3-vars.phiSt2;
			vars.dphi13 = vars.phiSt3-vars.phiSt1;

			vars.dphi23 = fmod(vars.dphi23 + 3.0*PI, 2.0*PI) - PI; 
			vars.dphi13 = fmod(vars.dphi13 + 3.0*PI, 2.0*PI) - PI; 

			// double check that theta is calculated correctly... // wp
			vars.theta       = atan2(sm.pT[Track_i],sm.pz[Track_i]);
			vars.Run_Number  = sm.Run_Number;
			vars.Evt_Number  = sm.Evt_Number;
			vars.Evt_bbcZ    = sm.Evt_bbcZ;
			vars.charge      = sm.charge [Track_i];
			vars.pT          = sm.pT     [Track_i];
			vars.pz          = sm.pz     [Track_i];
			vars.phi         = sm.phi    [Track_i];
			vars.eta         = fabs(sm.eta    [Track_i]);
			vars.DG0         = sm.DG0    [Track_i];
			vars.DDG0        = sm.DDG0   [Track_i];
			vars.chi2        = sm.chi2   [Track_i];
			vars.DCA_z       = sm.DCA_z  [Track_i];
			vars.DCA_r       = sm.DCA_r  [Track_i];
			vars.Rpc1dca     = sm.RpcMatchSt1_Rpc1dca[Track_i];
			vars.Rpc3dca     = sm.RpcMatchSt3_Rpc3dca[Track_i];
			vars.fvtx_dtheta = sm.fvtx_dtheta[Track_i];
			vars.fvtx_dphi   = sm.fvtx_dphi  [Track_i];
			vars.fvtx_dr     = sm.fvtx_dr    [Track_i];
			vars.fvtx_cone   = sm.fvtx_cone  [Track_i];
			vars.dw23        = vars.dphi23*vars.pT*sin(vars.theta); 
			vars.dw13        = vars.dphi13*vars.pT*sin(vars.theta); 	
			vars.clockcross  = sm.clockcross;
			vars.triggerbit  = sm.triggerbit;
			vars.DG4         = sm.DG4[Track_i];

			basicTrackCut["AcceptedTracks"]+= 1.0;
			OutTree->Fill();
		}
	}

  for(int i = 0; i < 100; i++) {
    if( Evt_Nmu_Counter[i] == 0 ) {
      continue;
    } else {
      std::cout << "Evt_Nmu[" << i << "]: " << Evt_Nmu_Counter[i] << std::endl;
    }
  }
	//std::cout << "END" << std::endl;
	//std::cout << 1 << std::endl;
	delete treeFile; // owns t (reading tree)
	//std::cout << 2 << std::endl;
	outRoot->cd();
	//std::cout << 3 << std::endl;
	OutTree->Write();
	outRoot->Write();
	//std::cout << 4 << std::endl;
	delete OutTree; // owned by outRoot
	outRoot->Close();
	//std::cout << 5 << std::endl;
	delete outRoot;
	//std::cout << 6 << std::endl;

	// Since OutTree is owned by outRoot, it is deleted with outRoot when outRoot->Close() is called

	// Summary of Cuts
  std::cout << "There were " << basicTrackCut["TotalTracks"] << " tracks in the file. " << std::endl;
  std::cout << basicTrackCut["AcceptedTracks"] << " tracks were accepted." << std::endl;
	for( std::map < std::string, long double > ::iterator iter = basicTrackCut.begin(); iter != basicTrackCut.end(); ++iter)
	{
		long double total = basicTrackCut["TotalTracks"];
		std::string Key = iter->first;
		long double Val = iter->second;
		if (Key.compare("TotalTracks") == 0.0) continue;
		if (Key.compare("AcceptedTracks") == 0.0) continue;

		std::cout << std::setw(20) << Key << std::setw(12) << std::setprecision(4) << Val/total*100.0 << "%" << std::endl;
	}
	std::cout << std::endl << std::setw(20) << "Tracks Analyzed: " <<  basicTrackCut["AcceptedTracks"] << std::endl;

	return 0;
}
