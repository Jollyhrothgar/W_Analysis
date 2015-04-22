#include "Run13Triggers.h"

Run13Triggers::Run13Triggers()
{
	// Triggers are guaranteed to have a unique bit and name on a run-to-run
	// basis. However, this mapping has duplicate keys which reflects the
	// fact that trigger names can vary run to run and can get associated with
	// different bits.
	triggers.insert(std::pair<std::string, int>("MUON_N_SG1&BBCLL1(noVtx)"                      , 0x00800000));
	triggers.insert(std::pair<std::string, int>("MUON_N_SG1&BBCLL1(noVtx)"                      , 0x02000000));
	triggers.insert(std::pair<std::string, int>("(MUIDLL1_N1D||S1D)&BBCLL1(noVtx)"              , 0x00020000));
	triggers.insert(std::pair<std::string, int>("(MUIDLL1_N1D||S1D)&BBCLL1(noVtx)"              , 0x00004000));
	triggers.insert(std::pair<std::string, int>("(MUIDLL1_N1D||S1D)&BBCLL1(noVtx)"              , 0x00400000));
	triggers.insert(std::pair<std::string, int>("RPC1+RPC3_S"                                   , 0x00040000));
	triggers.insert(std::pair<std::string, int>("MUON_S_SG1&BBCLL1(noVtx)"                      , 0x01000000));
	triggers.insert(std::pair<std::string, int>("MUON_S_SG1&BBCLL1(noVtx)"                      , 0x00400000));
	triggers.insert(std::pair<std::string, int>("MUON_S_SG1_RPC3A&MUID_S1D"                     , 0x00400000));
	triggers.insert(std::pair<std::string, int>("((MUIDLL1_N2D||S2D)||(N1D&S1D))&BBCLL1(noVtx)" , 0x00001000));
	triggers.insert(std::pair<std::string, int>("((MUIDLL1_N2D||S2D)||(N1D&S1D))&BBCLL1(noVtx)" , 0x00010000));
	triggers.insert(std::pair<std::string, int>("MUON_N_SG1_RPC3A&MUID_N1D"                     , 0x00800000));
	triggers.insert(std::pair<std::string, int>("MUON_N_SG1_RPC3_1_B||C"                        , 0x08000000));
	triggers.insert(std::pair<std::string, int>("MUON_N_SG1_RPC3_1_B||C"                        , 0x02000000));
	triggers.insert(std::pair<std::string, int>("MUON_S_SG1_RPC3_1_B||C"                        , 0x04000000));
	triggers.insert(std::pair<std::string, int>("MUON_S_SG1_RPC3_1_B||C"                        , 0x08000000));
	triggers.insert(std::pair<std::string, int>("RPC1+RPC3_N"                                   , 0x00080000));
	triggers.insert(std::pair<std::string, int>("SG1+RPC1(C)&MUIDLL1_N||S"                      , 0x00200000));
}

Run13Triggers::~Run13Triggers()
{
	triggers.clear();
}


// this checks if one of the physics triggers fired, not if one trigger bit exclusively fired.
bool Run13Triggers::IsRun13Trigger(int triggerBit)
{
	bool FoundTrigger = false;
	TriggerBit::iterator itr;
	for(itr = triggers.begin(); itr != triggers.end(); ++itr)
	{
		// & returns decimal version of int if LHS int matches RHS int, or 0 otherwise.
		if((triggerBit&(itr->second)) > 0) 
		{
			FoundTrigger = true;
			break;
		}
	}
	return FoundTrigger;
}

void Run13Triggers::ShowTriggers()
{
	TriggerBit::iterator itr;
	for(itr = triggers.begin(); itr != triggers.end(); ++itr)
	{
		std::cout << std::setfill(' ') << std::setw(50) << (itr->first) << " 0x"  << std::setfill('0') << std::setw(8) << std::hex << (itr->second) << std::endl;
	}
}
