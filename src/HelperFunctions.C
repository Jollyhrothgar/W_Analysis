#include "HelperFunctions.h"
#include "SingleMuonsConsts.h"
// Usage: 
// first argument whitespace separated string to split
// second argument: an empty string vector
// 
// will return a filled string vector + the number of whitespace separated strings.

unsigned int StringSplit(const std::string &txt, std::vector<std::string> &strs)
{
  strs.clear();
  std::stringstream ss(txt);
  std::string temp;
  while(ss >> temp) {strs.push_back(temp);}
  return strs.size();
}

bool IsSubstring(std::string findString, std::string checkString)
{
  if(findString.find(findString) != std::string::npos)
  {
    return true;
  }
  else
  {
    return false;
  }
}

unsigned int LoadFileListToVector(const std::string fileList, std::vector<std::string>& vectorFileList)
{
  std::ifstream inFile(fileList.c_str());
  std::string line = "";
  if( inFile.is_open() )
  {
    while(getline(inFile,line))
    {
      vectorFileList.push_back(line);
    }
  }
  else
  {
    std::cerr << fileList << " cannot be opened." << std::endl;
    return -1;
  }
  inFile.close();
  return vectorFileList.size();
}

std::string GetVarName(int var_i)
{
  std::string varName = "NAME_ERROR";
  switch(var_i)
  {
    case DG0_INDEX:
      varName = "DG0";
      break;
    case DDG0_INDEX:
      varName = "DDG0";
      break;
    case CHI2_INDEX:
      varName = "chi2";
      break;
    case DCA_R_INDEX:
      varName = "DCA_r";
      break;
    case RPC1DCA_INDEX:
      varName = "Rpc1Dca";
      break;
    case RPC3DCA_INDEX:
      varName = "Rpc3Dca";
      break;
    case FVTX_DRxDTHETA_INDEX:
      varName = "fvtx_drxdtheta";
      break;
    case FVTX_DPHI_INDEX:
      varName = "fvtx_dphi";
      break;
    case FVTX_CONE_INDEX:
      varName = "fvtx_cone";
      break;
    default:
      varName = "NAME_ERROR";
  }
  return varName;
}

void MakeNameTitleStubPair(int arm_i, int charge_i, int type_i, std::string& name_str, std::string& title_str)
{
  std::stringstream name_ss;
  std::string charge_str;
  std::string arm_str;
  std::string _str;
  std::string var_string;
  std::string _tag; 
  switch(charge_i)
  {
    case CHARGE_POS:
      charge_str = "#mu+";
      break;
    case CHARGE_NEG:
      charge_str = "#mu-";
      break;
    default:
      charge_str = "ERROR";
  }
  switch(arm_i)
  {
    case ARM_NORTH:
      arm_str = "North";
      break;
    case ARM_SOUTH:
      arm_str = "South";
      break;
    default:
      arm_str = "ERROR";

  }
  switch(type_i)
  {
    case BACKGROUND_INDEX:
      _tag = "_bak";
      _str = "Background";
      break;
    case SIGNAL_INDEX:
      _tag = "_sig";
      _str = "Signal";
      break;
      // case MUON_BACKGROUND:
      //  _tag = "_muonbkgd";
      //  break;
    default:
      _tag = "ERROR_BACKGROUND_TAG";
      _str = "ERROR";

  }
  name_ss << "Arm" << arm_i << "_" << "Charge" << charge_i << _tag; 

  // return
  title_str = arm_str+" "+charge_str+" "+_str;
  name_str = name_ss.str();

}

// need to tack on _sig or _ to end.
std::string Make1DVarName(int var_i, int arm_i, int charge_i, int type_i)
{
  std::stringstream name;
  std::string var_string;
  std::string _tag; 
  if( charge_i >= NUMBER_OF_CHARGE_CONFIGS )
  {
    charge_i = -1;
  }
  if( arm_i >= NUMBER_OF_ARM_CONFIGS )
  {
    arm_i = -1;
  }
  switch(type_i)
  {
    case BACKGROUND_INDEX:
      _tag = "_bak";
      break;
    case SIGNAL_INDEX:
      _tag = "_sig";
      break;
      // case MUON_BACKGROUND:
      //  _tag = "_muonbkgd";
      //  break;
    default:
      _tag = "ERROR_BACKGROUND_TAG";

  }

  switch (var_i)
  {
    case DG0_INDEX:
      var_string = "DG0";
      break;
    case DDG0_INDEX:
      var_string = "DDG0";
      break;
    case CHI2_INDEX:
      var_string = "chi2";
      break;
    case DCA_R_INDEX:
      var_string = "DCA_r";
      break;
    case RPC1DCA_INDEX:
      var_string = "Rpc1Dca";
      break;
    case RPC3DCA_INDEX:
      var_string = "Rpc3Dca";
      break;
    case FVTX_DRxDTHETA_INDEX:
      var_string = "fvtx_drxdtheta";
      break;
    case FVTX_DPHI_INDEX:
      var_string = "fvtx_dphi";
      break;
    case FVTX_CONE_INDEX:
      var_string = "fvtx_cone";
      break;
    default:
      var_string = "VAR_ERROR";
      std::cerr << "There is no index, " << var_i << ", for the variable you want." << std::endl;
  }

  name << var_string << "_" << "Arm" << arm_i << "_" << "Charge" << charge_i << _tag; 

  return name.str();
}

// need to tack on _sig or _ to end
std::string Make2DVarName(int var_i, int arm_i, int charge_i, int type_i)
{
  std::stringstream name;
  std::string var_string    = "";
  std::string _tag      = "";
  if( charge_i >= NUMBER_OF_CHARGE_CONFIGS )
  {
    charge_i = -1;
  }
  if( arm_i >= NUMBER_OF_ARM_CONFIGS )
  {
    arm_i = -1;
  }
  switch(type_i)
  {
    case BACKGROUND_INDEX:
      _tag = "_bak";
      break;
    case SIGNAL_INDEX:
      _tag = "_sig";
      break;
      //case MUON_BACKGROUND:
      // _tag = "_muonbkgd";
      // break;
    default:
      _tag = "ERROR_BACKGROUND_TAG";

  }
  switch (var_i)
  {
    case DG0_DDG0_INDEX:
      var_string = "DDG0_vs_DG0";
      break;
    case DCA_R_CHI2_INDEX:
      var_string = "Chi2_vs_DCA_r";
      break;
    default:
      var_string = "VAR_ERROR";
      std::cerr << "There is no index, " 
          << var_i << ", for the variable you want." << std::endl;
  }

  name << var_string << "_Arm" << arm_i << "_Charge" << charge_i << _tag; 
  return name.str();
}

float GetVarMax(int var_i)
{
  float max = 0.0;
  switch (var_i)
  {
    case DG0_INDEX:
      max = BASIC_CUT_DG0;
      break;
    case DDG0_INDEX:
      max = BASIC_CUT_DDG0;
      break;
    case CHI2_INDEX:
      max = BASIC_CUT_chi2;
      break;
    case DCA_R_INDEX:
      max = BASIC_CUT_DCA_r;
      break;
    case RPC1DCA_INDEX:
      max = RPC1_DIST_HIGH;
      break;
    case RPC3DCA_INDEX:
      max = RPC3_DIST_HIGH;
      break;
    case FVTX_DRxDTHETA_INDEX:
      max = FVTX_DRxDTHETA_MAX_VALUE; 
      break;
    case FVTX_DPHI_INDEX:
      max = FVTX_DPHI_MAX_VALUE;
      break;
    case FVTX_CONE_INDEX:
      max = FVTX_CONE_MAX_VALUE;
      break;
    default:
      std::cerr << "There is no max value for the variable, " << var_i << ", that you want." << std::endl;
  }
  return max;
}


void GetCorrVarMax(int var_i, float& max1, float& max2)
{
  switch (var_i)
  {
    case DG0_DDG0_INDEX:
      max1 = BASIC_CUT_DG0;
      max2 = BASIC_CUT_DDG0;
      break;
    case DCA_R_CHI2_INDEX:
      max1 = BASIC_CUT_DCA_r;
      max2 = BASIC_CUT_chi2;
      break;
    default:
      max1 = 0.0;
      max2 = 0.0;
  }
}

void GetCorrIndicies( int var_i, int& index_x, int& index_y )
{
  switch (var_i)
  {
    case DG0_DDG0_INDEX:
      index_x = DG0_INDEX;
      index_y = DDG0_INDEX;
      break;
    case DCA_R_CHI2_INDEX:
      index_x = DCA_R_INDEX;
      index_y = CHI2_INDEX;
      break;
    default:
      index_x = 0.0;
      index_y = 0.0;
  }
}

int GetArmIndex   (float z_momentum  )
{
  if(z_momentum > 0) return ARM_NORTH;
  else if(z_momentum < 0) return ARM_SOUTH;
  std::cerr << "USING NONSENSE INDEX FOR ARM: -1 SOMETHING IS VERY WRONG!!!!" << "Z MOMENTUM IS: " << z_momentum << std::endl;
  return -1;
}

int GetChargeIndex(float charge_value)
{
  if(charge_value > 0) return CHARGE_POS;
  if(charge_value < 0) return CHARGE_NEG;
  std::cerr << "USING NONSENSE INDEX FOR CHARGE: -1 SOMETHING IS VERY WRONG!!!! CHAREG GIVEN IS: " << charge_value << std::endl;
  return -1;
}

// must take fabs() of fvtx_dphi before we get here.
// can use this to check fill condition 
int GetLikelihoodIndex(float rpc1dca, float rpc3dca, float fvtxdtheta, float fvtxdr, float fvtxdphi)
{
  // FVTX is good
  if( (fvtxdr  > 0 ) && (fvtxdr < 100) && (fvtxdphi < 1.5 ) 
     && (fvtxdphi > 0) && (fvtxdtheta < 1.5) && (fvtxdtheta > 0) ) // FVTX is good
  {
    if ( (rpc1dca < RPC_MAX_VALUE) && (rpc3dca > RPC_MAX_VALUE) ) // RPC1 is good, RPC3 is bad
    {                                                                                           
      return RPC1_AND_FVTX;                                                                     
    }                                                                                           
    if ( (rpc1dca > RPC_MAX_VALUE) && (rpc3dca < RPC_MAX_VALUE) ) // RPC1 is bad, RPC3 is good
    {                                                                                           
      return RPC3_AND_FVTX;                                                                     
    }                                                                                           
    if ( (rpc1dca < RPC_MAX_VALUE) && (rpc3dca < RPC_MAX_VALUE) ) // RPC 1 is good, RPC3 is good
    {
      return RPC13_AND_FVTX;
    }
  }
  else // FVTX is bad
  {
    if ( (rpc1dca < RPC_MAX_VALUE) && (rpc3dca > RPC_MAX_VALUE) ) // RPC1 is good, RPC3 is bad
    {
      return RPC1;
    }
    if ( (rpc1dca > RPC_MAX_VALUE) && (rpc3dca < RPC_MAX_VALUE) ) // RPC1 is bad, RPC3 is good
    {
      return RPC3;
    }
    if ( (rpc1dca < RPC_MAX_VALUE) && (rpc3dca < RPC_MAX_VALUE) ) // RPC 1 is good, RPC3 is good
    {
      return RPC13;
    }
  }
  return -1;
}

void Count_Generated_Events()
{
  std::map < std::string, std::vector<std::string> > fList;
  LoadFileListToVector("../runlists/run12_DrellYan_MuonBkgd_Run367593.txt"         ,fList["drell_yan"]);
  LoadFileListToVector("../runlists/run12_LightSum_MuonBkgd_Run367593.txt"         ,fList["light"]);     
  LoadFileListToVector("../runlists/run12_OniumSum_MuonBkgd_Run367593.txt"         ,fList["onium"]);
  LoadFileListToVector("../runlists/run12_OnlyZSum_MuonBkgd_Run367593.txt"         ,fList["only_z"]);
  LoadFileListToVector("../runlists/run12_OpenBottomSum_MuonBkgd_Run367593.txt"    ,fList["open_bottom"]);
  LoadFileListToVector("../runlists/run12_OpenCharmSum_MuonBkgd_Run367593.txt"     ,fList["open_charm"]);
  LoadFileListToVector("../runlists/run12_WHadSum_MuonBkgd_Run367593.txt"          ,fList["w_had"]);
  LoadFileListToVector("../runlists/run12_WJetSum_MuonBkgd_Run367593.txt"          ,fList["w_jet"]);
  LoadFileListToVector("../runlists/run12_WTauSum_MuonBkgd_Run367593.txt"          ,fList["w_tau"]);
  LoadFileListToVector("../runlists/run12_ZSum_MuonBkgd_Run367593.txt"             ,fList["z"]);
  LoadFileListToVector("../runlists/run12_ZJetSum_MuonBkgd_Run367593.txt"          ,fList["z_jet"]);

  std::map<std::string, std::vector<std::string> >::iterator iter;
  for( iter = fList.begin(); iter != fList.end(); ++iter )
  {
    std::string key = iter->first;
    std::vector<std::string> list = iter->second;

    std::cout << key << " has " << list.size() << " files" << std::endl;
  }
  std::cout << std::endl;
  // count events in each tree
  // fill cross-section values into consts file
}

int CheckNan( const std::string& inTree, const std::string& logFileName) {
  BasicCutTreeVariables v;
  TFile* f = new TFile(inTree.c_str(),"READ");
  if(!f) return -1;
  TTree* t = (TTree*) f->Get("newsngmuons_basic_cut");
  if(!t) return -2;
  v.LinkTree(t,0);
  std::ofstream logFile(logFileName.c_str());
      
  logFile
      <<  "\"v.Run_Number\""
      << ",\"v.Evt_Number\""
      << ",\"v.Wness\""
      << ",\"v.DG0\""
      << ",\"v.DDG0\""
      << ",\"v.chi2\""
      << ",\"v.Rpc1dca\""
      << ",\"v.Rpc3dca\""
      << ",\"v.fvtx_cone\""
      << ",\"v.fvtx_dphi\""
      << ",\"v.fvtx_dr\""
      << std::endl;

  for(Long64_t i = 0; i < t->GetEntries(); i++) {
    t->GetEntry(i);
    if( std::isnan(v.Wness) ) {
      logFile
          <<  "\"" << v.Run_Number << "\""
          << ",\"" << v.Evt_Number << "\"" 
          << ",\"" << v.Wness      << "\"" 
          << ",\"" << v.DG0        << "\"" 
          << ",\"" << v.DDG0       << "\"" 
          << ",\"" << v.chi2       << "\"" 
          << ",\"" << v.Rpc1dca    << "\"" 
          << ",\"" << v.Rpc3dca    << "\"" 
          << ",\"" << v.fvtx_cone  << "\"" 
          << ",\"" << v.fvtx_dphi  << "\"" 
          << ",\"" << v.fvtx_dr    << "\"" 
          << std::endl;
    }
  }
  logFile.close();
  return 0;
}
