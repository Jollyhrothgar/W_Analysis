#include<iostream>
#include "TPRegexp.h"
#include "TString.h"
#include<vector>

// Run over muon backgrounds
int Run_InteractiveBasicCut(int doStream = -1, int runOver = -1)
{
	const int DIRECTORY = 14;
	const int FILELIST  = 14;

	std::string base = "/phenix/spin2/beaumim/WAnalysisRun13/AnalysisCode/macros/rootTrees/unmerged_trees/";
	std::string listBase = "/phenix/spin2/beaumim/WAnalysisRun13/AnalysisCode/runlists/";
	std::string folder  [DIRECTORY];
	std::string filelist[FILELIST ];

	// folder stores the output area. The input area comes from each line in teh filelist
	folder[ 0] = base+"no_rpc1/";                       filelist[ 0] = "run13_production_missing_rpc1.txt"; 
	folder[ 1] = base+"rpc1_uniqmissing_single_runs/";  filelist[ 1] = "run13_uniqRpc1Events.txt";                      
	folder[ 2] = base+"w_had/";                         filelist[ 2] = "run12_WHadSum_MuonBkgd_Run367593.txt";
	folder[ 3] = base+"open_charm/";                    filelist[ 3] = "run12_OpenCharmSum_MuonBkgd_Run367593.txt";    
	folder[ 4] = base+"open_bottom/";                   filelist[ 4] = "run12_OpenBottomSum_MuonBkgd_Run367593.txt";     
	folder[ 5] = base+"only_z/";                        filelist[ 5] = "run12_OnlyZSum_MuonBkgd_Run367593.txt";
	folder[ 6] = base+"onium_sum/";                     filelist[ 6] = "run12_OniumSum_MuonBkgd_Run367593.txt";   
	folder[ 7] = base+"direct_photon/";                 filelist[ 7] = "run12_DirectPhoton_MuonBkgd_Run367593.txt";       
	folder[ 8] = base+"light_sum/";                     filelist[ 8] = "run12_LightSum_MuonBkgd_Run367593.txt";   
	folder[ 9] = base+"w_jet/";                         filelist[ 9] = "run12_WJetSum_MuonBkgd_Run367593.txt";
	folder[10] = base+"w_tau/";                         filelist[10] = "run12_WTauSum_MuonBkgd_Run367593.txt";
	folder[11] = base+"z_sum/";                         filelist[11] = "run12_ZSum_MuonBkgd_Run367593.txt";
	folder[12] = base+"z_jet/";                         filelist[12] = "run12_ZJetSum_MuonBkgd_Run367593.txt";
	folder[13] = base+"w_sim/";                         filelist[13] = "run12_pDSTs_Simulation_367593.txt";
	
	std::string lib = "libSingleMuonsWAnalysis.so";
	gSystem->Load(lib.c_str());

	int start_stream = 0;
	int stop_stream = DIRECTORY;
	if(doStream >= 0 ) 
	{
		start_stream = doStream; 
		stop_stream = start_stream+1;
	}

	if (start_stream >= DIRECTORY)
	{
		cout << "out of range" << std::endl;
		return = -1;
	}

	for(int stream_i = start_stream; stream_i < stop_stream; stream_i++)
	{
		if(stream_i < 2) continue; // we run the production on files through condor.
		
		// Make a to-do list
		std::string thisFileList = listBase+filelist[stream_i];
		
		// What are we doing:
		std::cout << "Loading pDSTs to apply basic cut from: "
			<< std::endl << "\t" <<  thisFileList << std::endl 
			<< "Sending unmerged reduced trees to: " << std::endl 
			<< "\t" << folder[stream_i] << std::endl;

		std::vector<std::string> files;
		LoadFileListToVector(thisFileList,files);

		// Decide how many things on our list to do
		int number_of_files = 0;
		if     ( runOver < 0 ) number_of_files = files.size();
		else if( runOver > 0 ) number_of_files = runOver;
		if( static_cast<unsigned int> (number_of_files) > files.size() )
		{
			std::cout << "out of range" << std::endl;
			return -1;
		}
		for(int i = 0; i < number_of_files; i++)
		{
			//std::cout << files[i] << std::endl;

			// Defintiion for reading the root file + extracting the directory
			
			TString str(files[i].c_str());
			TObjArray *matchCheck = TPRegexp("([\\w+/]+)(\\d{6}_\\d{1}\.root)").MatchS(str);
			TObjArray *matchCheck = TPRegexp("(/.*/)(.*)").MatchS(str);
			const Int_t nrSubStr = matchCheck->GetLast()+1;
			if (nrSubStr > 2) // we found a directory like: /subdir1/subdir2/.../subdirn and a file like 123456-1.root
			{
				const TString File = ((TObjString *)matchCheck->At(2))->GetString();
				const TString Dir  = ((TObjString *)matchCheck->At(1))->GetString();
				//cout << "Dir: " << Dir << "  File: " << File << endl;

				std::string fname = File;
				std::string dname = Dir;

				std::cout << Dir << ", " << File << std::endl;

				// ReduceProduction args: directory to find files, directory to put files, name of file
				// Structured this way so that the output file has the same name as the input file.
				ReduceProduction(dname, folder[stream_i], fname);  
			}
			ReduceProduction(dname, folder[stream_i], fname);  

			// clear the to-do list (this should be done automatically)
			//files.resize(0);
			//files.erase();
		}
	}
	std::cout << "Done" << std::endl;
	return 0;
}
