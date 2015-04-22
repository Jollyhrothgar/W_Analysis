#include "../src/SingleMuonsConsts.h"
void Run_SBRextract()
{
  std::string sbr_file_name = baseDirectory + macrosDirectory + rootTreesDirectory + "Run13SbrPdfs.root";
  std::string unbinned_data = baseDirectory + macrosDirectory + rootTreesDirectory + "Run13WnessTreeWithFvtxAndRpc_Data.root";
  std::string lib = "libSingleMuonsWAnalysis.so";
  gSystem->Load(lib.c_str());
  SBRextract sbr(sbr_file_name, unbinned_data);
  sbr.SetSBROutFileNameStd("/direct/phenix+spin2/beaumim/WAnalysisRun13/AnalysisCode/logs/SBR_Yields_Whole_Eta_Range.txt");
  sbr.SetSBROutFileNameCsv("/direct/phenix+spin2/beaumim/WAnalysisRun13/AnalysisCode/logs/SBR.csv");
  sbr.Run();
}
