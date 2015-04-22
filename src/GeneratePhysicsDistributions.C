#include "GeneratePhysicsDistributions.h"

int Generate( TFile* f, SingleMuonsLikelihood& sm_vars, int STREAM_INDEX, bool checkBounds);

// usage such as: "Ntuplebak.root", "Ntuplesig.root", "Run12_bak"
int GeneratePhysicsDistributions( std::string nTuplebakFile, std::string nTupleSignalFile, std::string PDFFileName , bool checkBounds = false)
{

  nTuplebakFile       = baseDirectory+macrosDirectory+rootTreesDirectory+nTuplebakFile;
  nTupleSignalFile    = baseDirectory+macrosDirectory+rootTreesDirectory+nTupleSignalFile;


  TFile* rootBakFile = new TFile(nTuplebakFile.c_str(),"READ");
  TFile* rootSigFile = new TFile(nTupleSignalFile.c_str(),"READ");
  if(!(rootBakFile&&rootSigFile))
  {
    std::cout << "Invalid signal/background file path or file" << std::endl;
    std::cout << "  SigFile: " << nTupleSignalFile << std::endl;
    std::cout << "  BakFile: " << nTuplebakFile << std::endl;
    return -1;
  }
  
  SingleMuonsLikelihood sm_vars;	
  sm_vars.InitHistograms(); 

  // Generate hadronic background PDFS and Signal PDFs
  std::cout << "Generating Kinematic Distributions for " << nTuplebakFile << std::endl;
  Generate(rootBakFile, sm_vars, BACKGROUND_INDEX, checkBounds);
  std::cout << "Generating Kinematic Distributions for " << nTupleSignalFile << std::endl;
  Generate(rootSigFile, sm_vars, SIGNAL_INDEX, checkBounds);

  std::string pdfOut = baseDirectory+macrosDirectory+rootTreesDirectory+PDFFileName;
  TFile * fout = new TFile(pdfOut.c_str(), "RECREATE");
  sm_vars.Write(fout);
  fout->Close();
  return 0;
}

int Generate(TFile* readFile, SingleMuonsLikelihood& sm_vars, int STREAM_INDEX, bool checkBounds)
{
  BasicCutTreeVariables vars;
  TTree* readTree = (TTree*)readFile->Get(wness_tree.c_str());
  vars.LinkTree(readTree, READ);
  
  int tEntries = readTree->GetEntries();
  std::cout << "  Entries found: " << tEntries << std::endl;
  if(!readTree)
  {
    std::cout << "Invalid tree" << std::endl;
    std::cout << "Tried to get tree: " << wness_tree << std::endl;
    return -1;
  }
  for ( Long64_t i = 0; i < tEntries; i++)
  {
    readTree->GetEntry(i);
    if(i%1000 == 0) 
    {
      double pct = 100.0*static_cast<double>(i)/static_cast<double>(tEntries);
      std::cout << "Progress: " << std::setprecision(4) <<  pct << "%     \r" << std::flush;
    }

    int arm_i    = GetArmIndex(vars.pz); 
    int charge_i = GetChargeIndex(vars.charge);
    
    // First Step - Simultaneously check that DCA_r && DG0 && DDG0 && Chi2 are ALL in range
    //   Second Step - For each event for which step 1 is true, then we must also have a good RPC1 value
    //     Third Step - Fill Only the correct RPC PDF
    //       Fourth Step - Add FVTX if good

    // First Step - check if primary vars are in or out of range
    if(   (( 0.0 > vars.DG0  ) || ( vars.DG0   > BASIC_CUT_DG0   ))
       || (( 0.0 > vars.DDG0 ) || ( vars.DDG0  > BASIC_CUT_DDG0  ))
       || (( 0.0 > vars.chi2 ) || ( vars.chi2  > BASIC_CUT_chi2  )) )
       //|| (( 0.0 > vars.DCA_r) || ( vars.DCA_r > BASIC_CUT_DCA_r )) ) 
    {
      continue;
    }
    // Second Step - Make Sure There is a good RPC value
    int rpc_condition =  GetLikelihoodIndex(vars.Rpc1dca, 
                                            vars.Rpc3dca, 
                                            vars.fvtx_dtheta, 
                                            vars.fvtx_dr,
                                            fabs(vars.fvtx_dphi) );
    // Third Step - Fill only the correct RPC condition
    if( rpc_condition < 0 ) continue;
    if( ( rpc_condition == RPC13_AND_FVTX ) || ( rpc_condition == RPC13 ) )
    {
      // fill rpc 1 and 3
      sm_vars.Fill( RPC1DCA_INDEX        , arm_i, charge_i, STREAM_INDEX, vars.Rpc1dca, checkBounds);
      sm_vars.Fill( RPC3DCA_INDEX        , arm_i, charge_i, STREAM_INDEX, vars.Rpc3dca, checkBounds);
    }
    else
    {
      if( ( rpc_condition == RPC1_AND_FVTX  ) || ( rpc_condition ==  RPC1 ) )
      {
        // fill rpc 1
        sm_vars.Fill( RPC1DCA_INDEX        , arm_i, charge_i, STREAM_INDEX, vars.Rpc1dca, checkBounds);
      }
      else if( ( rpc_condition == RPC3_AND_FVTX  ) || ( rpc_condition ==  RPC3 ) )
      {
        // fill rpc 3
        sm_vars.Fill( RPC3DCA_INDEX        , arm_i, charge_i, STREAM_INDEX, vars.Rpc3dca, checkBounds);
      }
      else
      {
        continue;
      }
    }
    
    // We only get here if Step 2 succeeded. If Step 2 or Step 1 fail, we never get here.
    sm_vars.Fill( DG0_INDEX            , arm_i, charge_i, STREAM_INDEX, vars.DG0  , checkBounds);
    sm_vars.Fill( DDG0_INDEX           , arm_i, charge_i, STREAM_INDEX, vars.DDG0 , checkBounds);
    sm_vars.Fill( CHI2_INDEX           , arm_i, charge_i, STREAM_INDEX, vars.chi2 , checkBounds);
    sm_vars.Fill( DCA_R_INDEX          , arm_i, charge_i, STREAM_INDEX, vars.DCA_r, checkBounds);
    sm_vars.Fill( DG0_DDG0_INDEX       , arm_i, charge_i, STREAM_INDEX, vars.DG0         , vars.DDG0 , checkBounds);
    sm_vars.Fill( DCA_R_CHI2_INDEX     , arm_i, charge_i, STREAM_INDEX, vars.DCA_r       , vars.chi2 , checkBounds);
    // Finally, we only fill fvtx for good fvtx.
    if( rpc_condition > 2 )
    {
      sm_vars.Fill( FVTX_DRxDTHETA_INDEX , arm_i, charge_i, STREAM_INDEX, vars.fvtx_dr*vars.fvtx_dtheta, checkBounds);
      sm_vars.Fill( FVTX_DPHI_INDEX      , arm_i, charge_i, STREAM_INDEX, fabs(vars.fvtx_dphi), checkBounds);
      sm_vars.Fill( FVTX_CONE_INDEX      , arm_i, charge_i, STREAM_INDEX, vars.fvtx_cone, checkBounds);
    }
    
  }
  std::cout << std::endl;
  return 0;
}
