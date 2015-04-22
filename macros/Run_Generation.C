#include <string>
#include "../src/SingleMuonsConsts.h"
int Run_Generation(std::string raw_data, std::string raw_simu, std::string pdfOutName ,bool checkBounds = 1)
{
	std::string lib = "libSingleMuonsWAnalysis.so";
	gSystem->Load(lib.c_str());
	GeneratePhysicsDistributions(raw_data,raw_simu,pdfOutName,checkBounds);
	return 0;
}
