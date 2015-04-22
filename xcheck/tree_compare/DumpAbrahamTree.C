{
  gSystem->Load("libSingleMuonsWAnalysis.so");
  TFile* f = new TFile("/phenix/u/beaumim/W_Analysis/macros/rootTrees/compare/abraham/Run13WnessTreeWithFvtxAndRpc_Data.root","READ");
  TTree* t = (TTree*)f->Get("newsngmuons_basic_cut");

  BasicCutTreeVariables v;
  v.LinkTree(t,0);
  for(Long64_t i = 0; i < t->GetEntries(); i++)
  {
    t->GetEntry(i);
    std::cout << v.Run_Number << " " << v.Evt_Number << " " << v.DG0 << " " << v.DDG0 << " " << v.Rpc1dca << " " << v.Rpc3dca << " " << v.DCA_r << " "  << v.Wness << std::endl;
  }
}
