#include "SingleMuonsLikelihood.h"
SingleMuonsLikelihood::SingleMuonsLikelihood()
{
  // State is determed by which 
  usingData         = false;
  usingSimulation   = false;
  loadedHistograms  = false;
  definedHistograms = false;
  manualDelete      = false;
}

SingleMuonsLikelihood::~SingleMuonsLikelihood()
{

}

int SingleMuonsLikelihood::Init2DHistograms()
{
  for(int var_i = 0; var_i < NUMBER_OF_CORRELATED_VARS; var_i++) // Variable Loop
  {
    for( int arm_i = 0; arm_i < NUMBER_OF_ARM_CONFIGS; arm_i++) // Arm Config Loop
    {                 
      for( int charge_i = 0; charge_i < NUMBER_OF_CHARGE_CONFIGS; charge_i++)  // Charge Config Loop
      {
        for( int data_i = 0; data_i < NUMBER_OF_DATA_STREAMS; data_i++) // Data Stream Loop
        {
          std::string name = Make2DVarName(var_i,arm_i,charge_i,data_i);
          float max1 = 0.0;
          float max2 = 0.0;
          GetCorrVarMax(var_i, max1, max2); 
          int bins = 100;

          PDFseed2D[var_i][arm_i][charge_i][data_i] = new TH2F
              (name.c_str(), name.c_str(), bins, 0.0, max1, bins,0.0,max2);

          // std::cout << PDFseed2D[var_i][arm_i][charge_i][data_i] 
          // <<", "  << var_i << ", " << arm_i << ", " 
          // << charge_i << ", " << data_i  << ", " 
          // << "(" << max1 << "," << max2 << ")" << ", " 
          // << PDFseed2D[var_i][arm_i][charge_i][data_i]->GetName() 
          // << std::endl;
          //  Print out ranges for Ralf:
          if( arm_i == 0 && charge_i == 0 && data_i == 0)
          {
            std::cout << PDFseed2D[var_i][arm_i][charge_i][0]->GetName() 
                << ", Bins X: " << bins << ", Min_x: 0.0, Max_x: " << max1 
                << ", Bins Y: " << bins << ", Min_y: 0.0, Max_y: " << max2 
                << std::endl;
          }
          name = "";
          max1 = 0.0;
          max2 = 0.0;
          bins = 0;
        }
      }
    }
  }
  return 0;
}

int SingleMuonsLikelihood::Init1DHistograms()
{
  for(int var_i = 0; var_i < NUMBER_OF_VARS; var_i++) // Variable Loop
  {
    for( int arm_i = 0; arm_i < NUMBER_OF_ARM_CONFIGS; arm_i++) // Arm Config Loop
    {                 
      for( int charge_i = 0; charge_i < NUMBER_OF_CHARGE_CONFIGS; charge_i++)  // Charge Config Loop
      {
        for ( int data_i = 0; data_i < NUMBER_OF_DATA_STREAMS; data_i++)
        {
          std::string name = Make1DVarName(var_i,arm_i,charge_i,data_i);
          float max = GetVarMax(var_i);
          float min = 0.0;
          int bins = 100;

          // fvtx_cone needs special treatment - one int per bin
          if(var_i == FVTX_CONE_INDEX)
          {
            max = FVTX_CONE_MAX_VALUE;
            min = 0;
            bins = 60.0;
          }

          PDFseed1D[var_i][arm_i][charge_i][data_i] = new TH1F
              (name.c_str(), name.c_str(), bins, min, max);

          // std::cout << PDFseed1D[var_i][arm_i][charge_i][data_i] << ", "  << var_i << ", " << arm_i 
          // << ", " << charge_i << ", " << data_i  << ", " << "(" << max << ")" 
          // << ", " << PDFseed1D[var_i][arm_i][charge_i][data_i]->GetName() 
          // << std::endl;
          // Print out ranges for Ralf:
          if( arm_i == 0 && charge_i == 0 && data_i == 0)
          {
            std::cout << PDFseed1D[var_i][arm_i][charge_i][0]->GetName() << ", Bins: " << bins << ", Min: " << min << ", Max: " << max << std::endl;
          }
          name = "";
          max = 0;
          bins = 0;
        }
      }
    }
  }
  return 0;
}


int SingleMuonsLikelihood::InitHistograms()
{
  Init1DHistograms();
  Init2DHistograms();
  return 0;
}

int SingleMuonsLikelihood::Write(TFile* f)
{
  if(!f)
  {
    std::cerr << "Root file does not exist." << std::endl;
    return -1;
  }
  f->cd();
  for(int var_i = 0; var_i < NUMBER_OF_VARS; var_i++) // Variable Loop
  {
    for( int arm_i = 0; arm_i < NUMBER_OF_ARM_CONFIGS; arm_i++) // Arm Config Loop
    {                 
      for( int charge_i = 0; charge_i < NUMBER_OF_CHARGE_CONFIGS; charge_i++)  // Charge Config Loop
      {
        for( int data_i = 0; data_i < NUMBER_OF_DATA_STREAMS; data_i++)
        {
          PDFseed1D[var_i][arm_i][charge_i][data_i]->Write();
        }
      }
    }
  }
  for(int var_i = 0; var_i < NUMBER_OF_CORRELATED_VARS; var_i++) // Variable Loop
  {
    for( int arm_i = 0; arm_i < NUMBER_OF_ARM_CONFIGS; arm_i++) // Arm Config Loop
    {                 
      for( int charge_i = 0; charge_i < NUMBER_OF_CHARGE_CONFIGS; charge_i++)  // Charge Config Loop
      {
        for( int data_i = 0; data_i < NUMBER_OF_DATA_STREAMS; data_i++)
        {
          PDFseed2D[var_i][arm_i][charge_i][data_i]->Write();
        }
      }
    }
  }
  return 0;
}

int SingleMuonsLikelihood::ReadHistograms( TFile* f )
{
  for(int var_i = 0; var_i < NUMBER_OF_VARS; var_i++) // Variable Loop
  {
    for( int arm_i = 0; arm_i < NUMBER_OF_ARM_CONFIGS; arm_i++) // Arm Config Loop
    {                 
      for( int charge_i = 0; charge_i < NUMBER_OF_CHARGE_CONFIGS; charge_i++)  // Charge Config Loop
      {
        for( int data_i = 0; data_i < NUMBER_OF_DATA_STREAMS; data_i++)
        {
          std::string histoName1D = Make1DVarName(var_i,arm_i,charge_i,data_i);
          PDFseed1D[var_i][arm_i][charge_i][data_i] = (TH1F*) f->Get(histoName1D.c_str());

          //std::cout << PDFseed1D[var_i][arm_i][charge_i][data_i]->GetName() << std::endl;

          if( var_i < NUMBER_OF_CORRELATED_VARS )
          {
            std::string histoName2D = Make2DVarName(var_i, arm_i, charge_i, data_i);
            PDFseed2D[var_i][arm_i][charge_i][data_i] = (TH2F*) f->Get(histoName2D.c_str());
            // std::cout << PDFseed2D[var_i][arm_i][charge_i][data_i]->GetName() << std::endl;
          }
        }
      }
    }
  }
  InitLikelihoods();
  GenerateProdPdfs();
  return 0;
}

int SingleMuonsLikelihood::InitRooRealVars()
{
  RooVar[DG0_INDEX        ] = new RooRealVar     ("RooVar_DG0"           , "RooVar_DG0"            , 0.0     , BASIC_CUT_DG0  );
  RooVar[DDG0_INDEX       ] = new RooRealVar     ("RooVar_DDG0"          , "RooVar_DDG0"           , 0.0     , BASIC_CUT_DDG0 );
  RooVar[CHI2_INDEX       ] = new RooRealVar     ("RooVar_Chi2"          , "RooVar_Chi2"           , 0.0     , BASIC_CUT_chi2 );
  RooVar[DCA_R_INDEX      ] = new RooRealVar     ("RooVar_DCA_r"         , "RooVar_DCA_r"          , 0.0     , BASIC_CUT_DCA_r );
  RooVar[RPC1DCA_INDEX    ] = new RooRealVar     ("RooVar_Rpc1Dca"       , "RooVar_Rpc1Dca"        , 0.0     , RPC_MAX_VALUE );
  RooVar[RPC3DCA_INDEX    ] = new RooRealVar     ("RooVar_Rpc3Dca"       , "RooVar_Rpc3Dca"        , 0.0     , RPC_MAX_VALUE );
  RooVar[FVTX_DRxDTHETA_INDEX ] = new RooRealVar ("RooVar_Fvtx_drxdtheta", "RooVar_Fvtx_drxdtheta" , 0.0     , FVTX_DRxDTHETA_MAX_VALUE );
  RooVar[FVTX_DPHI_INDEX  ] = new RooRealVar     ("RooVar_Fvtx_dphi"     , "RooVar_Fvtx_dphi"      , 0.0     , FVTX_DPHI_MAX_VALUE);
  RooVar[FVTX_CONE_INDEX  ] = new RooRealVar     ("RooVar_Fvtx_cone"     , "RooVar_fvtx_cone"      , 0.0     , FVTX_CONE_MAX_VALUE);
  return 0;
}

int SingleMuonsLikelihood::InitLikelihoods()
{
  // Create a likelihood funciton when needed - is this too slow, or is it fast enough?
  // if it were fast, then the likelihood can be created, called with the indicies of the desired
  // distribution, a value can be called from it, and it can be destroyed.

  InitRooRealVars();

  // For now - we need to to create PDF function for each variable.
  for ( int var_i = 0; var_i < NUMBER_OF_VARS; var_i++)
  {
    for( int arm_i = 0; arm_i < NUMBER_OF_ARM_CONFIGS; arm_i++) 
    {
      for( int charge_i = 0; charge_i < NUMBER_OF_CHARGE_CONFIGS; charge_i++)
      {
        for(int data_i = 0; data_i < NUMBER_OF_DATA_STREAMS; data_i++)
        {
          std::string name_base = Make1DVarName(var_i,arm_i,charge_i,data_i);
          std::string name = "";

          // RooDataHist - Data
          name = "rooDataHist_"+name_base; 
          RooHist1D[var_i][arm_i][charge_i][data_i] = new RooDataHist(
              name.c_str(), 
              name.c_str(), 
              *(RooVar     [var_i]), 
              PDFseed1D[var_i][arm_i][charge_i][data_i]  
              );
          name = "";

          // RooHistPdf 
          name = "rooPdf_" + name_base;
          RooPdf1D[var_i][arm_i][charge_i][data_i] = new RooHistPdf(
              name.c_str(),
              name.c_str(),
              *(RooVar         [var_i]),
              *(RooHist1D[var_i][arm_i][charge_i][data_i])
              );
          name = "";

        }
      }
    }
  }

  for(int var_i = 0; var_i < NUMBER_OF_CORRELATED_VARS; var_i++) // Variable Loop
  {
    for( int arm_i = 0; arm_i < NUMBER_OF_ARM_CONFIGS; arm_i++) // Arm Config Loop
    {                 
      for( int charge_i = 0; charge_i < NUMBER_OF_CHARGE_CONFIGS; charge_i++)  // Charge Config Loop
      {
        for( int data_i = 0; data_i < NUMBER_OF_DATA_STREAMS; data_i++)
        {
          std::string name_base = Make1DVarName(var_i, arm_i, charge_i, data_i);
          int index_x = -1;
          int index_y = -1;
          GetCorrIndicies(var_i,  index_x, index_y );
          std::string name = "";
          //std::cout << PDFseed2D[var_i][arm_i][charge_i][data_i]->GetName() << std::endl;

          name = "rooHist_" + name_base;
          RooHist2D[var_i][arm_i][charge_i][data_i] = new RooDataHist(
              name.c_str(),
              name.c_str(),
              RooArgList( *(RooVar[index_x]), *(RooVar[index_y]) ),
              PDFseed2D[var_i][arm_i][charge_i][data_i]
              );
          name = "";

          //std::cout << name.c_str() << " xname : " << RooVar[index_x]->GetName() << " yname : " << RooVar[index_y]->GetName() << std::endl;
          name = "rooPdf_" + name_base;
          RooPdf2D[var_i][arm_i][charge_i][data_i] = new RooHistPdf(
              name.c_str(),
              name.c_str(),
              RooArgList( *(RooVar[index_x]), *(RooVar[index_y]) ),
              *(RooHist2D[var_i][arm_i][charge_i][data_i])
              );
          name = "";
        }
      }
    }
  }
  return 0;
}

int SingleMuonsLikelihood::GenerateProdPdfs()
{
  for( int likelihood_i = 0; likelihood_i < NUMBER_OF_LIKELIHOODS; likelihood_i++)
  {
    for( int arm_i = 0; arm_i < NUMBER_OF_ARM_CONFIGS; arm_i++)
    {
      for( int charge_i = 0; charge_i < NUMBER_OF_CHARGE_CONFIGS; charge_i++)
      {
        for( int data_i = 0; data_i < NUMBER_OF_DATA_STREAMS; data_i++)
        {
          std::stringstream name;
          std::string name_stub = "";

          switch(data_i)
          { 
            case BACKGROUND_INDEX:
              name_stub = "_bak";
              break;
            case SIGNAL_INDEX:
              name_stub = "_sig";
              break;
            default:
              name_stub = "_ERROR";
              std::cerr << "You are asking for a likelihood for bak stream index " << data_i << " which does not exist." << std::endl;
          }

          switch(likelihood_i)
          {
            case RPC1:
              name << "likelihood_RPC1" <<  "_Arm" << arm_i << "_Charge" << charge_i << name_stub;

              Likelihood[likelihood_i][arm_i][charge_i][data_i] = new RooProdPdf( 
                  name.str().c_str(),
                  name.str().c_str(),
                  RooArgList(
                      *(RooPdf2D[DG0_DDG0_INDEX  ][arm_i][charge_i][data_i]),
                      *(RooPdf2D[DCA_R_CHI2_INDEX  ][arm_i][charge_i][data_i]),
                      *(RooPdf1D[ RPC1DCA_INDEX  ][arm_i][charge_i][data_i])
                      ),
                  0
                  );
              name.str("");
              break;
            case RPC3:
              name << "likelihood_RPC3" << "_Arm" << arm_i << "_Charge" << charge_i << name_stub;

              Likelihood[likelihood_i][arm_i][charge_i][data_i] = new RooProdPdf( 
                  name.str().c_str(),
                  name.str().c_str(),
                  RooArgList(
                      *(RooPdf2D[DG0_DDG0_INDEX  ][arm_i][charge_i][data_i]),
                      *(RooPdf2D[DCA_R_CHI2_INDEX  ][arm_i][charge_i][data_i]),
                      *(RooPdf1D[ RPC3DCA_INDEX  ][arm_i][charge_i][data_i])
                      ),
                  0
                  );
              name.str("");
              break;
            case RPC13:
              name << "likelihood_RPC13" << "_Arm" <<  arm_i << "_Charge" << charge_i << name_stub;

              Likelihood[likelihood_i][arm_i][charge_i][data_i] = new RooProdPdf( 
                  name.str().c_str(),
                  name.str().c_str(),
                  RooArgList(
                      *(RooPdf2D[DG0_DDG0_INDEX  ][arm_i][charge_i][data_i]),
                      *(RooPdf2D[DCA_R_CHI2_INDEX  ][arm_i][charge_i][data_i]),
                      *(RooPdf1D[ RPC1DCA_INDEX  ][arm_i][charge_i][data_i]),
                      *(RooPdf1D[ RPC3DCA_INDEX  ][arm_i][charge_i][data_i])
                      ),
                  0
                  );
              name.str("");
              break;
            case RPC1_AND_FVTX:
              name << "likelihood_RPC1_AND_FVTX" << "_Arm" <<  arm_i << "_Charge" << charge_i << name_stub;

              Likelihood[likelihood_i][arm_i][charge_i][data_i] = new RooProdPdf( 
                  name.str().c_str(),
                  name.str().c_str(),
                  RooArgList(
                      *(RooPdf2D[ DG0_DDG0_INDEX ][arm_i][charge_i][data_i]),
                      *(RooPdf2D[ DCA_R_CHI2_INDEX  ][arm_i][charge_i][data_i]),
                      *(RooPdf1D[ RPC1DCA_INDEX  ][arm_i][charge_i][data_i]),
                      *(RooPdf1D[ FVTX_DRxDTHETA_INDEX ][arm_i][charge_i][data_i]),
                      *(RooPdf1D[ FVTX_DPHI_INDEX      ][arm_i][charge_i][data_i]),
                      *(RooPdf1D[ FVTX_CONE_INDEX      ][arm_i][charge_i][data_i])
                      ),
                  0
                  );
              name.str("");
              break;
            case RPC3_AND_FVTX:
              name << "likelihood_RPC3_AND_FVTX" << "_Arm" <<  arm_i << "_Charge" << charge_i << name_stub;

              Likelihood[likelihood_i][arm_i][charge_i][data_i] = new RooProdPdf( 
                  name.str().c_str(),
                  name.str().c_str(),
                  RooArgList(
                      *(RooPdf2D[ DG0_DDG0_INDEX ][arm_i][charge_i][data_i]),
                      *(RooPdf2D[DCA_R_CHI2_INDEX  ][arm_i][charge_i][data_i]),
                      *(RooPdf1D[ RPC3DCA_INDEX  ][arm_i][charge_i][data_i]),
                      *(RooPdf1D[ FVTX_DRxDTHETA_INDEX ][arm_i][charge_i][data_i]),
                      *(RooPdf1D[ FVTX_DPHI_INDEX      ][arm_i][charge_i][data_i]),
                      *(RooPdf1D[ FVTX_CONE_INDEX      ][arm_i][charge_i][data_i])
                      ),
                  0
                  );
              name.str("");
              break;
            case RPC13_AND_FVTX:
              name << "likelihood_RPC13_AND_FVTX" << "_Arm" <<  arm_i << "_Charge" << charge_i << name_stub;

              Likelihood[likelihood_i][arm_i][charge_i][data_i] = new RooProdPdf( 
                  name.str().c_str(),
                  name.str().c_str(),
                  RooArgList(
                      *(RooPdf2D[ DG0_DDG0_INDEX ][arm_i][charge_i][data_i]),
                      *(RooPdf2D[DCA_R_CHI2_INDEX  ][arm_i][charge_i][data_i]),
                      *(RooPdf1D[ RPC1DCA_INDEX  ][arm_i][charge_i][data_i]),
                      *(RooPdf1D[ RPC3DCA_INDEX  ][arm_i][charge_i][data_i]),
                      *(RooPdf1D[ FVTX_DRxDTHETA_INDEX ][arm_i][charge_i][data_i]),
                      *(RooPdf1D[ FVTX_DPHI_INDEX      ][arm_i][charge_i][data_i]), 
                      *(RooPdf1D[ FVTX_CONE_INDEX      ][arm_i][charge_i][data_i])
                      ),
                  0
                  );
              name.str("");
              break;

            default:
              std::cerr << "The likelihood index " << likelihood_i << " does not exist" << std::endl;
          }
          name.str("");
        }
      }
    }
  }
  return 0;
}

float SingleMuonsLikelihood::GetLikelihood( int likelihood_i, int arm_i, int charge_i, int data_i )
{
  float likelihood = -999.9;

  if( data_i >= NUMBER_OF_DATA_STREAMS)
  {
    std::cerr << "The likelihood stream for data stream index " << data_i << " does not exist" << std::endl;
    return likelihood;
  }
  if( arm_i >= NUMBER_OF_ARM_CONFIGS )
  {
    std::cerr << "The arm index " << arm_i << " does not exist" << std::endl;
    return likelihood;
  }
  if( charge_i >= NUMBER_OF_CHARGE_CONFIGS )
  { 
    std::cerr << "The charge index " << charge_i << " does not exist" << std::endl; 
    return likelihood;
  }
  // give the correct list of variables to each possible likelihood. Order is important

  RooArgSet* arg_set = new RooArgSet();
  switch(likelihood_i)
  {
    case RPC1:
      arg_set = new RooArgSet(
          *(RooVar[ DG0_INDEX      ]),
          *(RooVar[ DDG0_INDEX     ]),
          *(RooVar[ DCA_R_INDEX    ]),
          *(RooVar[ CHI2_INDEX     ]),
          *(RooVar[ RPC1DCA_INDEX  ]),
          *(RooVar[ FVTX_CONE_INDEX     ])
          );
      break;
    case RPC3:
      arg_set = new RooArgSet(
          *(RooVar[ DG0_INDEX      ]),
          *(RooVar[ DDG0_INDEX     ]),
          *(RooVar[ DCA_R_INDEX    ]),
          *(RooVar[ CHI2_INDEX     ]),
          *(RooVar[ RPC3DCA_INDEX  ]),
          *(RooVar[ FVTX_CONE_INDEX     ])
          );
      break;
    case RPC13:
      arg_set = new RooArgSet(
          *(RooVar[ DG0_INDEX      ]),
          *(RooVar[ DDG0_INDEX     ]),
          *(RooVar[ DCA_R_INDEX    ]),
          *(RooVar[ CHI2_INDEX     ]),
          *(RooVar[ RPC1DCA_INDEX  ]),
          *(RooVar[ RPC3DCA_INDEX  ]),
          *(RooVar[ FVTX_CONE_INDEX     ])
          );
      break;
    case RPC1_AND_FVTX:
      arg_set = new RooArgSet(
          *(RooVar[ DG0_INDEX         ]),
          *(RooVar[ DDG0_INDEX        ]),
          *(RooVar[ DCA_R_INDEX       ]),
          *(RooVar[ CHI2_INDEX        ]),
          *(RooVar[ RPC1DCA_INDEX     ]),
          *(RooVar[ FVTX_DPHI_INDEX   ]),
          *(RooVar[ FVTX_DRxDTHETA_INDEX     ]),
          *(RooVar[ FVTX_CONE_INDEX     ])
          );
      break; 
    case RPC3_AND_FVTX:
      arg_set = new RooArgSet(
          *(RooVar[ DG0_INDEX      ]),
          *(RooVar[ DDG0_INDEX     ]),
          *(RooVar[ DCA_R_INDEX    ]),
          *(RooVar[ CHI2_INDEX     ]),
          *(RooVar[ RPC3DCA_INDEX  ]),
          *(RooVar[ FVTX_DPHI_INDEX   ]),
          *(RooVar[ FVTX_DRxDTHETA_INDEX     ]),
          *(RooVar[ FVTX_CONE_INDEX     ])
          );
      break;
    case RPC13_AND_FVTX:
      arg_set = new RooArgSet(
          *(RooVar[ DG0_INDEX      ]),
          *(RooVar[ DDG0_INDEX     ]),
          *(RooVar[ DCA_R_INDEX    ]),
          *(RooVar[ CHI2_INDEX     ]),
          *(RooVar[ RPC1DCA_INDEX  ]),
          *(RooVar[ RPC3DCA_INDEX  ]),
          *(RooVar[ FVTX_DPHI_INDEX   ]),
          *(RooVar[ FVTX_DRxDTHETA_INDEX     ]),
          *(RooVar[ FVTX_CONE_INDEX     ])
          );
      break;
    default:
      std::cerr << "The likelihood index " << likelihood_i << " does not exist" << std::endl;
  }
  likelihood = (Likelihood[likelihood_i][arm_i][charge_i][data_i])->getVal(arg_set); // RCF
  //likelihood = (Likelihood[likelihood_i][arm_i][charge_i][data_i])->getValV(arg_set); // LOCAL
  delete arg_set;
  return likelihood;
}

// 1D Method
int SingleMuonsLikelihood::Fill(int var, int arm, int charge, int data_i, float fillval, bool checkBounds)
{
  if( data_i >= NUMBER_OF_DATA_STREAMS)
  {
    std::cerr << " bad data-type index!  Index: " << data_i << std::endl;
    return -4;
  }

  if( var >= NUMBER_OF_VARS )
  {
    std::cerr << " bad variable index! Index: " << var << std::endl;
    return -1;
  }
  if ( arm >= NUMBER_OF_ARM_CONFIGS )
  {
    std::cerr << " bad arm index! Index: " << arm << std::endl;
    return -2;
  }
  if ( charge >= NUMBER_OF_CHARGE_CONFIGS )
  {
    std::cerr << " bad charge index! Index:  "<< charge  << std::endl;
    return -3;
  }
  // first bin# = 1, last bin number = nbins
  int nbins  = PDFseed1D[var][arm][charge][data_i]->GetNbinsX();
  float xlow = PDFseed1D[var][arm][charge][data_i]->GetXaxis()->GetBinLowEdge(1);       // low edge of first bin
  float xhi =  PDFseed1D[var][arm][charge][data_i]->GetXaxis()->GetBinUpEdge(nbins);    // upper edge of last bin
  
  if(checkBounds)
  {
    if( ( xlow < fillval ) && ( fillval < xhi ) )
    {
      PDFseed1D[var][arm][charge][data_i]->Fill(fillval);
    }
  }
  else
  {
    PDFseed1D[var][arm][charge][data_i]->Fill(fillval);
  }
  return 0;
}

// 2D Method
int SingleMuonsLikelihood::Fill(int var, int arm, int charge, int data_i, float fillvalX, float fillvalY, bool checkBounds)
{
  if( data_i >= NUMBER_OF_DATA_STREAMS)
  {
    std::cerr << " bad data-type index!  Index: " << data_i << std::endl;
    return -4;
  }

  if( var >= NUMBER_OF_VARS )
  {
    std::cerr << " bad variable index! Index: " << var << std::endl;
    return -1;
  }
  if ( arm >= NUMBER_OF_ARM_CONFIGS )
  {
    std::cerr << " bad arm index! Index: " << arm << std::endl;
    return -2;
  }
  if ( charge >= NUMBER_OF_CHARGE_CONFIGS )
  {
    std::cerr << " bad charge index! Index:  "<< charge  << std::endl;
    return -3;
  }
  // first bin# = 1, last bin number = nbins
  int nbinsX  = PDFseed2D[var][arm][charge][data_i]->GetNbinsX();
  int nbinsY  = PDFseed2D[var][arm][charge][data_i]->GetNbinsY();
  float xlow  = PDFseed2D[var][arm][charge][data_i]->GetXaxis()->GetBinLowEdge(1);        // low edge of first bin
  float xhi   = PDFseed2D[var][arm][charge][data_i]->GetXaxis()->GetBinUpEdge (nbinsX);   // upper edge of last bin
  float ylow  = PDFseed2D[var][arm][charge][data_i]->GetYaxis()->GetBinLowEdge(1);        // low edge of first bin
  float yhi   = PDFseed2D[var][arm][charge][data_i]->GetYaxis()->GetBinUpEdge (nbinsY); // upper edge of last bin
  
  // bounds check - do not fill if we do not want to fill!
  if(checkBounds)
  {
    if( ( xlow < fillvalX ) && ( fillvalX < xhi ) && ( ylow < fillvalY ) && ( fillvalY < yhi ) )
    {  
      PDFseed2D[var][arm][charge][data_i]->Fill(fillvalX,fillvalY);
    }
  }
  else
  {
    PDFseed2D[var][arm][charge][data_i]->Fill(fillvalX,fillvalY);
  }
  return 0;
}

int SingleMuonsLikelihood::SetVariables
(
    float v_DG0, 
    float v_DDG0, 
    float v_DCA_r, 
    float v_chi2, 
    float v_rpc1, 
    float v_rpc3, 
    float v_fvtx_drxdtheta, 
    float v_fvtx_dphi,
    float v_fvtx_cone
    )
{
  (RooVar[DG0_INDEX     ])->setVal(v_DG0  );
  (RooVar[DDG0_INDEX    ])->setVal(v_DDG0 );
  (RooVar[DCA_R_INDEX   ])->setVal(v_DCA_r);
  (RooVar[CHI2_INDEX    ])->setVal(v_chi2 );
  (RooVar[RPC1DCA_INDEX ])->setVal(v_rpc1 );
  (RooVar[RPC3DCA_INDEX ])->setVal(v_rpc3 );
  (RooVar[FVTX_DRxDTHETA_INDEX ]    )->setVal(v_fvtx_drxdtheta );
  (RooVar[FVTX_DPHI_INDEX ]  )->setVal(v_fvtx_dphi );
  (RooVar[FVTX_CONE_INDEX ]  )->setVal(v_fvtx_cone );
  return 0;
}
