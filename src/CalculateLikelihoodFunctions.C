#include "CalculateLikelihoodFunctions.h"

int CalculateLikelihoodFunctions(std::string kinematicDistributions,std::string runOverFile, std::string outRoot)
{
  std::string rootFile = baseDirectory+macrosDirectory+rootTreesDirectory+kinematicDistributions;
  outRoot  = baseDirectory+macrosDirectory+rootTreesDirectory+outRoot;
  runOverFile = baseDirectory+macrosDirectory+rootTreesDirectory+runOverFile;
  std::cout << "Running over: " << runOverFile << std::endl;
  std::cout << "Sending to: " << outRoot << std::endl;

  std::cout << "opening " << rootFile << std::endl;
  TFile * f = new TFile(rootFile.c_str(),"READ");

  SingleMuonsLikelihood test;
  test.ReadHistograms(f); // likelihoods are also initialized here

  TFile * treeFile = new TFile(runOverFile.c_str(), "READ");

  TTree* bakTree = (TTree*)treeFile->Get(wness_tree.c_str());
  BasicCutTreeVariables readVars;
  readVars.LinkTree(bakTree, READ);

  BasicCutTreeVariables writeVars;
  TFile * writeFile = new TFile(outRoot.c_str(), "RECREATE");
  TTree * writeTree = new TTree(wness_tree.c_str(), "Wness For Events with RPC_1||3 and FVTX. DCA_r is not correllated to chi2");
  writeVars.LinkTree(writeTree, WRITE);

  int skippedEvents = 0;

  for( Long64_t i = 0; i < bakTree->GetEntries(); i++)
  {
    bakTree->GetEntry(i);
    double pct = 100.0*static_cast<double>(i)/static_cast<double>(bakTree->GetEntries());
    std::cout << "Progress: " << std::setprecision(4) <<  pct << "%     \r" << std::flush;


    // Set the FVTX variables
    test.SetVariables(
        readVars.DG0, 
        readVars.DDG0, 
        readVars.DCA_r, 
        readVars.chi2, 
        readVars.Rpc1dca, 
        readVars.Rpc3dca, 
        readVars.fvtx_dr*readVars.fvtx_dtheta, 
        fabs(readVars.fvtx_dphi),
        readVars.fvtx_cone
        );


    float signal = 0.0;
    float background = 0.0;
    float ratio = -100;

    int arm_i        = GetArmIndex       (readVars.pz); 
    int charge_i     = GetChargeIndex    (readVars.charge);
    int likelihood_i = GetLikelihoodIndex(
        readVars.Rpc1dca, 
        readVars.Rpc3dca, 
        readVars.fvtx_dtheta, 
        readVars.fvtx_dr, 
        fabs(readVars.fvtx_dphi));
    if(likelihood_i < 0) continue;

    signal     = test.GetLikelihood( likelihood_i, arm_i, charge_i, SIGNAL_INDEX);
    background = test.GetLikelihood( likelihood_i, arm_i, charge_i, BACKGROUND_INDEX);
    ratio      = signal/(signal + background);

    if( std::isnan(ratio) )
    {
      skippedEvents++;
      continue;
    }
    // operator=(LHS,RHS) defined for structs.
    writeVars       = readVars;

    double new_eta = fabs(readVars.eta);
    writeVars.eta = new_eta;

    writeVars.Wness = ratio;
    writeTree->Fill();
  }

  double totalTracks = static_cast<double>(bakTree->GetEntries() );

  std::cout << std::endl;
  std::cout << "Percent of events skipped because Wness condition was not met: " 
      << std::setprecision(4) << 100.0*(static_cast<double>(skippedEvents)/totalTracks) << " %" 
      << std::endl;
  writeFile->Write();
  writeFile->Close();
  treeFile->Close();
  f->Close();
  return 0;
}
