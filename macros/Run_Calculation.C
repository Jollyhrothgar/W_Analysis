#include "../src/SingleMuonsConsts.h"
#include <vector>
#include <string>

int Run_Calculation(std::string PDF, std::string input, std::string output)
{
	//std::string lib = baseDirectory + libDir + "libSingleMuonsWAnalysis.so";
	std::string lib = "libSingleMuonsWAnalysis.so";
	gSystem->Load(lib.c_str());
	CalculateLikelihoodFunctions(PDF, input, output );
	return 0;
}
