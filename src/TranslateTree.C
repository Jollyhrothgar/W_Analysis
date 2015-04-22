#include "TranslateTree.h"

int SetupTree(int analyzer, TTree* anaTree);
int SetVariables(int analyzer, BasicCutTreeVariables& vars);
// Checks if branch can be set
template <class T> int SafeSet(T& SetVal, TBranch* br, const std::string& varName) {
  if(br) {
    if(br->GetLeaf(varName.c_str())) {
      SetVal = static_cast< T >( br->GetLeaf(varName.c_str())->GetValue(0));
    } else {
      std::cerr << "Could not find leaf " << varName << " for the branch" << std::endl
          << "  Does this leaf exist?" << std::endl;
      return -2;
    }
  } else {
    std::cerr << " Cound not find branch for leaf " << varName << std::endl
        << "    Did you get the branch from the tree correctly? " << std::endl;
    return -1;
  }
  return 0;
}

TBranch* br_Run_Number    = NULL;
TBranch* br_Evt_Number    = NULL;
TBranch* br_Evt_bbcZ      = NULL;
TBranch* br_clockcross    = NULL;
TBranch* br_triggerbit    = NULL;
TBranch* br_Wness         = NULL;
TBranch* br_charge        = NULL;
TBranch* br_pT            = NULL;
TBranch* br_pz            = NULL;
TBranch* br_phi           = NULL;
TBranch* br_eta           = NULL;
TBranch* br_DG0           = NULL;
TBranch* br_DDG0          = NULL;
TBranch* br_DG4           = NULL;
TBranch* br_chi2          = NULL;
TBranch* br_DCA_z         = NULL;
TBranch* br_DCA_r         = NULL;
TBranch* br_phiSt1        = NULL;
TBranch* br_phiSt2        = NULL;
TBranch* br_phiSt3        = NULL;
TBranch* br_theta         = NULL;
TBranch* br_dphi13        = NULL;
TBranch* br_dphi23        = NULL;
TBranch* br_dw13          = NULL;
TBranch* br_dw23          = NULL;
TBranch* br_Rpc1dca       = NULL;
TBranch* br_Rpc3dca       = NULL;
TBranch* br_fvtx_dtheta   = NULL;
TBranch* br_fvtx_dphi     = NULL;
TBranch* br_fvtx_dr       = NULL;
TBranch* br_fvtx_cone     = NULL;
TBranch* br_fvtx_tracklet = NULL;

int TranslateTree(int analyzerChoice, std::string inputFile, std::string inputTree, std::string outputFile, std::string outputTree)
{
  // All my leaves - common leaves will be initialized in the switch statement in SetupTree.

  TFile* readFile = new TFile(inputFile.c_str(), "READ");
  TTree* readTree = (TTree*)readFile->Get(inputTree.c_str());
  SetupTree(analyzerChoice, readTree);

  TFile* writeFile = new TFile(outputFile.c_str(), "RECREATE");
  std::stringstream analyzer_name;
  analyzer_name << "Analyzer_" << analyzerChoice;
  TTree* outTree = new TTree(outputTree.c_str(),analyzer_name.str().c_str());
  BasicCutTreeVariables v;
  v.LinkTree(outTree,WRITE);
  Long64_t tEntries = readTree->GetEntries();
  for(Long64_t entry = 0; entry < tEntries; entry++)
  {
    if(entry%1000 == 0)
    {
      double pct = 100.0*static_cast<double>(entry)/static_cast<double>(tEntries);
      std::cout << "Progress: " << std::setprecision(4) <<  pct << "%     \r" << std::flush;
    }
    readTree->GetEntry(entry);
    SetVariables(analyzerChoice,v);
    outTree->Fill();
  }
  writeFile->cd();
  outTree->Write();
  delete readFile;
  delete outTree;
  delete writeFile;

  return 0;
}

// Set branch addresses for branches in common with analyzer trees.
int SetupTree(int analyzer, TTree* anaTree)
{
  switch(analyzer)
  {
    case ABE_INDEX:
      br_Run_Number    = anaTree->GetBranch("runnumber");
      br_Evt_Number    = anaTree->GetBranch("eventnumber");
      br_Evt_bbcZ      = anaTree->GetBranch("zvtx");
      br_clockcross    = anaTree->GetBranch("lvl1_clock_cross");
      br_triggerbit    = anaTree->GetBranch("trigbit");
      br_Wness         = anaTree->GetBranch("Wness");
      br_charge        = anaTree->GetBranch("chargetest");
      br_pT            = anaTree->GetBranch("pt");
      br_pz            = anaTree->GetBranch("pz");
      br_phi           = anaTree->GetBranch("phi");
      br_eta           = anaTree->GetBranch("eta");
      br_DG0           = anaTree->GetBranch("dg0");
      br_DDG0          = anaTree->GetBranch("ddg0");
      br_chi2          = anaTree->GetBranch("trchi2");
      br_DCA_z         = anaTree->GetBranch("dcaz");
      br_DCA_r         = anaTree->GetBranch("dcar");
      br_dphi13        = anaTree->GetBranch("dphi12");
      br_dphi23        = anaTree->GetBranch("dphi23");
      br_dw23          = anaTree->GetBranch("dw23");
      br_Rpc1dca       = anaTree->GetBranch("rpc1dca");
      br_Rpc3dca       = anaTree->GetBranch("rpc3dca");
      br_fvtx_dtheta   = anaTree->GetBranch("dtheta_fvtx");
      br_fvtx_dphi     = anaTree->GetBranch("dphi_fvtx");
      br_fvtx_dr       = anaTree->GetBranch("dr_fvtx");
      br_fvtx_cone     = anaTree->GetBranch("fvtx_cone");
      //br_fvtx_tracklet = anaTree->GetBranch("fvtx_tracklets_cone");
      break;
    case DANIEL_INDEX:
      br_Run_Number    = anaTree->GetBranch("Run_Number");
      br_Evt_Number    = anaTree->GetBranch("Evt_Number");
      br_Evt_bbcZ      = anaTree->GetBranch("Evt_bbcZ");
      br_clockcross    = anaTree->GetBranch("clockcross");
      br_triggerbit    = anaTree->GetBranch("triggerbit");
      br_Wness         = anaTree->GetBranch("Wness");
      br_charge        = anaTree->GetBranch("charge");
      br_pT            = anaTree->GetBranch("pT");
      br_pz            = anaTree->GetBranch("pz");
      br_phi           = anaTree->GetBranch("phi");
      br_eta           = anaTree->GetBranch("eta");
      br_DG0           = anaTree->GetBranch("DG0");
      br_DDG0          = anaTree->GetBranch("DDG0");
      br_DG4           = anaTree->GetBranch("DG4");
      br_chi2          = anaTree->GetBranch("chi2");
      br_DCA_z         = anaTree->GetBranch("DCA_z");
      br_DCA_r         = anaTree->GetBranch("DCA_r");
      br_dw23          = anaTree->GetBranch("dw23");
      br_Rpc1dca       = anaTree->GetBranch("Rpc1dca");
      br_Rpc3dca       = anaTree->GetBranch("Rpc3dca");
      br_fvtx_dtheta   = anaTree->GetBranch("fvtx_dtheta");
      br_fvtx_dphi     = anaTree->GetBranch("fvtx_dphi");
      br_fvtx_dr       = anaTree->GetBranch("fvtx_dr");
      br_fvtx_cone     = anaTree->GetBranch("fvtx_cone");
      br_fvtx_tracklet = anaTree->GetBranch("fvtx_tracklcone");
      break;
    case RALF_INDEX:
      br_Run_Number    = anaTree->GetBranch("runnum");
      br_Evt_Number    = anaTree->GetBranch("evenum");
      br_Evt_bbcZ      = anaTree->GetBranch("bbcz");
      br_clockcross    = anaTree->GetBranch("xid");
      br_triggerbit    = anaTree->GetBranch("triggerbit");
      br_Wness         = anaTree->GetBranch("wness");
      br_charge        = anaTree->GetBranch("charge");
      br_pT            = anaTree->GetBranch("pT");
      br_pz            = anaTree->GetBranch("pz");
      br_phi           = anaTree->GetBranch("phi");
      br_eta           = anaTree->GetBranch("eta");
      br_DG0           = anaTree->GetBranch("dg0");
      br_DDG0          = anaTree->GetBranch("ddg0");
      br_chi2          = anaTree->GetBranch("chi2");
      br_DCA_z         = anaTree->GetBranch("dcaz");
      br_DCA_r         = anaTree->GetBranch("dcar");
      br_dw23          = anaTree->GetBranch("dw23");
      br_Rpc1dca       = anaTree->GetBranch("rpc1dca");
      br_Rpc3dca       = anaTree->GetBranch("rpc3dca");
      br_fvtx_dtheta   = anaTree->GetBranch("fvtx_dtheta");
      br_fvtx_dphi     = anaTree->GetBranch("fvtx_dphi");
      br_fvtx_dr       = anaTree->GetBranch("fvtx_dr");
      br_fvtx_cone     = anaTree->GetBranch("fvtxconecl");
      br_fvtx_tracklet = anaTree->GetBranch("fvtxconetr");
      break;
    default:
      std::cerr << anaTree->GetName() << " does not fit with any known analyzer" << std::endl;
      break;
  }
  return 0;
}

int SetVariables(int analyzer, BasicCutTreeVariables& vars)
{
  switch(analyzer)
  {
    case ABE_INDEX:
      SafeSet(vars.Run_Number , br_Run_Number , "runnumber"       );
      SafeSet(vars.Evt_Number , br_Evt_Number , "eventnumber"     );
      SafeSet(vars.Evt_bbcZ   , br_Evt_bbcZ   , "zvtx"            );
      SafeSet(vars.clockcross , br_clockcross , "lvl1_clock_cross");
      SafeSet(vars.triggerbit , br_triggerbit , "trigbit"         );
      SafeSet(vars.Wness      , br_Wness      , "Wness"           );
      SafeSet(vars.charge     , br_charge     , "chargetest"      );
      if( vars.charge < 1.0 ) {
        vars.charge = -1.0;
      } else {
        vars.charge =  1.0;
      }
      SafeSet(vars.pT            , br_pT           , "pt"                 );
      SafeSet(vars.pz            , br_pz           , "pz"                 );
      SafeSet(vars.phi           , br_phi          , "phi"                );
      SafeSet(vars.eta           , br_eta          , "eta"                );
      SafeSet(vars.DG0           , br_DG0          , "dg0"                );
      SafeSet(vars.DDG0          , br_DDG0         , "ddg0"               );
      SafeSet(vars.chi2          , br_chi2         , "trchi2"             );
      SafeSet(vars.DCA_z         , br_DCA_z        , "dcaz"               );
      SafeSet(vars.DCA_r         , br_DCA_r        , "dcar"               );
      SafeSet(vars.dphi13        , br_dphi13       , "dphi12"             );
      SafeSet(vars.dphi23        , br_dphi23       , "dphi23"             );
      SafeSet(vars.dw23          , br_dw23         , "dw23"               );
      SafeSet(vars.Rpc1dca       , br_Rpc1dca      , "rpc1dca"            );
      SafeSet(vars.Rpc3dca       , br_Rpc3dca      , "rpc3dca"            );
      SafeSet(vars.fvtx_dtheta   , br_fvtx_dtheta  , "dtheta_fvtx"        );
      SafeSet(vars.fvtx_dphi     , br_fvtx_dphi    , "dphi_fvtx"          );
      SafeSet(vars.fvtx_dr       , br_fvtx_dr      , "dr_fvtx"            );
      SafeSet(vars.fvtx_cone     , br_fvtx_cone    , "fvtx_cone"          );
      //SafeSet(vars.fvtx_tracklet , br_fvtx_tracklet, "fvtx_tracklets_cone");
      break;
    case DANIEL_INDEX:
      SafeSet(vars.Run_Number    , br_Run_Number   , "Run_Number"     );
      SafeSet(vars.Evt_Number    , br_Evt_Number   , "Evt_Number"     );
      SafeSet(vars.Evt_bbcZ      , br_Evt_bbcZ     , "Evt_bbcZ"       );
      SafeSet(vars.clockcross    , br_clockcross   , "clockcross"     );
      SafeSet(vars.triggerbit    , br_triggerbit   , "triggerbit"     );
      SafeSet(vars.Wness         , br_Wness        , "Wness"          );
      SafeSet(vars.charge        , br_charge       , "charge"         );
      SafeSet(vars.pT            , br_pT           , "pT"             );
      SafeSet(vars.pz            , br_pz           , "pz"             );
      SafeSet(vars.phi           , br_phi          , "phi"            );
      SafeSet(vars.eta           , br_eta          , "eta"            );
      SafeSet(vars.DG0           , br_DG0          , "DG0"            );
      SafeSet(vars.DDG0          , br_DDG0         , "DDG0"           );
      SafeSet(vars.DG4           , br_DG4          , "DG4"            );
      SafeSet(vars.chi2          , br_chi2         , "chi2"           );
      SafeSet(vars.DCA_z         , br_DCA_z        , "DCA_z"          );
      SafeSet(vars.DCA_r         , br_DCA_r        , "DCA_r"          );
      SafeSet(vars.dw23          , br_dw23         , "dw23"           );
      SafeSet(vars.Rpc1dca       , br_Rpc1dca      , "Rpc1dca"        );
      SafeSet(vars.Rpc3dca       , br_Rpc3dca      , "Rpc3dca"        );
      SafeSet(vars.fvtx_dtheta   , br_fvtx_dtheta  , "fvtx_dtheta"    );
      SafeSet(vars.fvtx_dphi     , br_fvtx_dphi    , "fvtx_dphi"      );
      SafeSet(vars.fvtx_dr       , br_fvtx_dr      , "fvtx_dr"        );
      SafeSet(vars.fvtx_cone     , br_fvtx_cone    , "fvtx_cone"      );
      SafeSet(vars.fvtx_tracklet , br_fvtx_tracklet, "fvtx_tracklcone");
      break;
    case RALF_INDEX:
      SafeSet(vars.Run_Number    , br_Run_Number    , "runnum");
      SafeSet(vars.Evt_Number    , br_Evt_Number    , "evenum");
      SafeSet(vars.Evt_bbcZ      , br_Evt_bbcZ      , "bbcz");
      SafeSet(vars.clockcross    , br_clockcross    , "xid");
      SafeSet(vars.triggerbit    , br_triggerbit    , "triggerbit");
      SafeSet(vars.Wness         , br_Wness         , "wness");
      SafeSet(vars.charge        , br_charge        , "charge");
      if( vars.charge < 1.0 ) {
        vars.charge = -1.0;
      } else {
        vars.charge =  1.0;
      }
      SafeSet(vars.pT            , br_pT            , "pT");
      SafeSet(vars.pz            , br_pz            , "pz");
      SafeSet(vars.phi           , br_phi           , "phi");
      SafeSet(vars.eta           , br_eta           , "eta");
      SafeSet(vars.DG0           , br_DG0           , "dg0");
      SafeSet(vars.DDG0          , br_DDG0          , "ddg0");
      SafeSet(vars.chi2          , br_chi2          , "chi2");
      SafeSet(vars.DCA_z         , br_DCA_z         , "dcaz");
      SafeSet(vars.DCA_r         , br_DCA_r         , "dcar");
      SafeSet(vars.dw23          , br_dw23          , "dw23");
      SafeSet(vars.Rpc1dca       , br_Rpc1dca       , "rpc1dca");
      SafeSet(vars.Rpc3dca       , br_Rpc3dca       , "rpc3dca");
      SafeSet(vars.fvtx_dtheta   , br_fvtx_dtheta   , "fvtx_dtheta");
      SafeSet(vars.fvtx_dphi     , br_fvtx_dphi     , "fvtx_dphi");
      SafeSet(vars.fvtx_dr       , br_fvtx_dr       , "fvtx_dr");
      SafeSet(vars.fvtx_cone     , br_fvtx_cone     , "fvtxconecl");
      SafeSet(vars.fvtx_tracklet , br_fvtx_tracklet , "fvtxconetr");
      break;
    default:
      std::cerr << " Could not identify analyzer associated with index " << analyzer << std::endl;
      break;
  }
  return 0;
}
