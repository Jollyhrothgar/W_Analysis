
int Run_PatternComparison() {
  gSystem->Load("libHelicityCheck.so");
  ComparePatterns pat(
      "/direct/phenix+spin2/beaumim/WAnalysisRun13/AnalysisCode/xcheck/spin_compare/data/mike_XC_000.txt",
      "/direct/phenix+spin2/beaumim/WAnalysisRun13/AnalysisCode/xcheck/spin_compare/data/francesca_XC_000.txt",
      0.001,
      "/direct/phenix+spin2/beaumim/WAnalysisRun13/AnalysisCode/xcheck/spin_compare/logs"
      );
  pat.Run();
}
