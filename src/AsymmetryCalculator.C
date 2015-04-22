#include "AsymmetryCalculator.h"
#include <cstdio>
#include "TTreeIndex.h"

AsymmetryCalculator::AsymmetryCalculator() {
  std::cout << "AsymmetryCalculator Starting" << std::endl;
  AsymEtaBins.push_back( std::make_pair(1.1,2.6)); // Full range
  AsymEtaBins.push_back( std::make_pair(1.1,1.4));
  AsymEtaBins.push_back( std::make_pair(1.4,1.8));
  AsymEtaBins.push_back( std::make_pair(1.8,2.6));

  for(auto i = AsymEtaBins.begin(); i != AsymEtaBins.end(); ++i) {
    std::cout << "ETA Bin: " << i->first << ", " << i->second << std::endl;
  }

  MakeArmChargeSpinEtaContainer(patternCount,AsymEtaBins.size());
  MakeArmChargeEtaContainer( rawSingleAsymmetryPosEta     , AsymEtaBins.size() );
  MakeArmChargeEtaContainer( rawSingleAsymmetryNegEta     , AsymEtaBins.size() );
  MakeArmChargeEtaContainer( rawSingleAsymmetryPosEtaErr  , AsymEtaBins.size() );
  MakeArmChargeEtaContainer( rawSingleAsymmetryNegEtaErr  , AsymEtaBins.size() );
  MakeArmChargeEtaContainer( rawDoubleAsymmetry           , AsymEtaBins.size() );
  MakeArmChargeEtaContainer( rawDoubleAsymmetryErr        , AsymEtaBins.size() );
}

int AsymmetryCalculator::Run()
{
  Loop();
  CalculateAsymmetries();
  return 0;
}

int AsymmetryCalculator::CalculateAsymmetries() {
  float bottom;
  float top;
  float err_bottom;
  float err_top;

  for( int charge_i = 0; charge_i < NUMBER_OF_CHARGE_CONFIGS; charge_i++) {
    for( unsigned int eta_bin = 0; eta_bin < AsymEtaBins.size(); eta_bin++) {
      // ERROR NORTH
      // asymmetry_count [arm][charge][eta_bin][spin-pattern]
      // raw_single_asymmetry [arm][charge][eta_bin][0 -> minus_eta, 1 -> plus_eta ]
      
      // SOUTH
      bottom = patternCount[ARM_SOUTH][charge_i][eta_bin][spinPattern::PP] + 
          patternCount[ARM_SOUTH][charge_i][eta_bin][spinPattern::MP] + 
          patternCount[ARM_SOUTH][charge_i][eta_bin][spinPattern::PM] + 
          patternCount[ARM_SOUTH][charge_i][eta_bin][spinPattern::MM];

      top = patternCount[ARM_SOUTH][charge_i][eta_bin][spinPattern::PP] - 
          patternCount[ARM_SOUTH][charge_i][eta_bin][spinPattern::MP] + 
          patternCount[ARM_SOUTH][charge_i][eta_bin][spinPattern::PM] - 
          patternCount[ARM_SOUTH][charge_i][eta_bin][spinPattern::MM];
      rawSingleAsymmetryNegEta[ARM_SOUTH][charge_i][eta_bin] = top/bottom;

      top = patternCount[ARM_SOUTH][charge_i][eta_bin][spinPattern::PP] + 
          patternCount[ARM_SOUTH][charge_i][eta_bin][spinPattern::MP] - 
          patternCount[ARM_SOUTH][charge_i][eta_bin][spinPattern::PM] - 
          patternCount[ARM_SOUTH][charge_i][eta_bin][spinPattern::MM];
      rawSingleAsymmetryPosEta[ARM_SOUTH][charge_i][eta_bin] = top/bottom;

      top = patternCount[ARM_SOUTH][charge_i][eta_bin][spinPattern::PP] - 
          patternCount[ARM_SOUTH][charge_i][eta_bin][spinPattern::MP] - 
          patternCount[ARM_SOUTH][charge_i][eta_bin][spinPattern::PM] + 
          patternCount[ARM_SOUTH][charge_i][eta_bin][spinPattern::MM];
      rawDoubleAsymmetry[ARM_SOUTH][charge_i][eta_bin] = top/bottom;

      // NORTH
      bottom = patternCount[ARM_NORTH][charge_i][eta_bin][spinPattern::PP] + 
          patternCount[ARM_NORTH][charge_i][eta_bin][spinPattern::MP] + 
          patternCount[ARM_NORTH][charge_i][eta_bin][spinPattern::PM] + 
          patternCount[ARM_NORTH][charge_i][eta_bin][spinPattern::MM];

      top = patternCount[ARM_NORTH][charge_i][eta_bin][spinPattern::PP] + 
          patternCount[ARM_NORTH][charge_i][eta_bin][spinPattern::MP] - 
          patternCount[ARM_NORTH][charge_i][eta_bin][spinPattern::PM] - 
          patternCount[ARM_NORTH][charge_i][eta_bin][spinPattern::MM];
      rawSingleAsymmetryNegEta[ARM_NORTH][charge_i][eta_bin] = top/bottom;

      top = patternCount[ARM_NORTH][charge_i][eta_bin][spinPattern::PP] - 
          patternCount[ARM_NORTH][charge_i][eta_bin][spinPattern::MP] + 
          patternCount[ARM_NORTH][charge_i][eta_bin][spinPattern::PM] - 
          patternCount[ARM_NORTH][charge_i][eta_bin][spinPattern::MM];
      rawSingleAsymmetryPosEta[ARM_NORTH][charge_i][eta_bin] = top/bottom;

      top = patternCount[ARM_NORTH][charge_i][eta_bin][spinPattern::PP] - 
          patternCount[ARM_NORTH][charge_i][eta_bin][spinPattern::MP] - 
          patternCount[ARM_NORTH][charge_i][eta_bin][spinPattern::PM] + 
          patternCount[ARM_NORTH][charge_i][eta_bin][spinPattern::MM];
      rawDoubleAsymmetry[ARM_NORTH][charge_i][eta_bin] = top/bottom;

      //Error calculations
      //south
      err_bottom = pow(
          patternCount[ARM_SOUTH][charge_i][eta_bin][spinPattern::PP] + 
          patternCount[ARM_SOUTH][charge_i][eta_bin][spinPattern::MP] + 
          patternCount[ARM_SOUTH][charge_i][eta_bin][spinPattern::PM] + 
          patternCount[ARM_SOUTH][charge_i][eta_bin][spinPattern::MM]
          ,4);

      err_top = 4*((patternCount[ARM_SOUTH][charge_i][eta_bin][spinPattern::PP] + patternCount[ARM_SOUTH][charge_i][eta_bin][spinPattern::PM]) * (pow(patternCount[ARM_SOUTH][charge_i][eta_bin][spinPattern::MP] + patternCount[ARM_SOUTH][charge_i][eta_bin][spinPattern::MM],2))) - 4*((pow(patternCount[ARM_SOUTH][charge_i][eta_bin][spinPattern::PP] + patternCount[ARM_SOUTH][charge_i][eta_bin][spinPattern::PM],2)) * (patternCount[ARM_SOUTH][charge_i][eta_bin][spinPattern::MP] + patternCount[ARM_SOUTH][charge_i][eta_bin][spinPattern::MM]));
      rawSingleAsymmetryNegEtaErr[ARM_SOUTH][charge_i][eta_bin] = sqrt(fabs(err_top/err_bottom));

      err_top = 4*((patternCount[ARM_SOUTH][charge_i][eta_bin][spinPattern::PP] + patternCount[ARM_SOUTH][charge_i][eta_bin][spinPattern::MP]) * (pow(patternCount[ARM_SOUTH][charge_i][eta_bin][spinPattern::PM] + patternCount[ARM_SOUTH][charge_i][eta_bin][spinPattern::MM],2))) - 4*((pow(patternCount[ARM_SOUTH][charge_i][eta_bin][spinPattern::PP] + patternCount[ARM_SOUTH][charge_i][eta_bin][spinPattern::MP],2)) * (patternCount[ARM_SOUTH][charge_i][eta_bin][spinPattern::PM] + patternCount[ARM_SOUTH][charge_i][eta_bin][spinPattern::MM]));

      rawSingleAsymmetryPosEtaErr[ARM_SOUTH][charge_i][eta_bin] = sqrt(fabs(err_top/err_bottom));

      err_top = 4*((patternCount[ARM_SOUTH][charge_i][eta_bin][spinPattern::PP] + patternCount[ARM_SOUTH][charge_i][eta_bin][spinPattern::MM]) * (pow(patternCount[ARM_SOUTH][charge_i][eta_bin][spinPattern::MP] + patternCount[ARM_SOUTH][charge_i][eta_bin][spinPattern::PM],2))) - 4*((pow(patternCount[ARM_SOUTH][charge_i][eta_bin][spinPattern::PP] + patternCount[ARM_SOUTH][charge_i][eta_bin][spinPattern::MM],2)) * (patternCount[ARM_SOUTH][charge_i][eta_bin][spinPattern::MP] + patternCount[ARM_SOUTH][charge_i][eta_bin][spinPattern::PM]));

      rawDoubleAsymmetryErr[ARM_SOUTH][charge_i][eta_bin] = sqrt(fabs(err_top/err_bottom));

      //north
      err_bottom = pow(
          patternCount[ARM_NORTH][charge_i][eta_bin][spinPattern::PP] + 
          patternCount[ARM_NORTH][charge_i][eta_bin][spinPattern::MP] + 
          patternCount[ARM_NORTH][charge_i][eta_bin][spinPattern::PM] + 
          patternCount[ARM_NORTH][charge_i][eta_bin][spinPattern::MM]
          ,4);

      err_top = 4*((patternCount[ARM_NORTH][charge_i][eta_bin][spinPattern::PP] + patternCount[ARM_NORTH][charge_i][eta_bin][spinPattern::MP]) * (pow(patternCount[ARM_NORTH][charge_i][eta_bin][spinPattern::PM] + patternCount[ARM_NORTH][charge_i][eta_bin][spinPattern::MM],2))) - 4*((pow(patternCount[ARM_NORTH][charge_i][eta_bin][spinPattern::PP] + patternCount[ARM_NORTH][charge_i][eta_bin][spinPattern::MP],2)) * (patternCount[ARM_NORTH][charge_i][eta_bin][spinPattern::PM] + patternCount[ARM_NORTH][charge_i][eta_bin][spinPattern::MM]));
      
      rawSingleAsymmetryNegEtaErr[ARM_NORTH][charge_i][eta_bin] = sqrt(fabs(err_top/err_bottom));

      err_top = 4*((patternCount[ARM_NORTH][charge_i][eta_bin][spinPattern::PP] + patternCount[ARM_NORTH][charge_i][eta_bin][spinPattern::PM]) * (pow(patternCount[ARM_NORTH][charge_i][eta_bin][spinPattern::MP] + patternCount[ARM_NORTH][charge_i][eta_bin][spinPattern::MM],2))) - 4*((pow(patternCount[ARM_NORTH][charge_i][eta_bin][spinPattern::PP] + patternCount[ARM_NORTH][charge_i][eta_bin][spinPattern::PM],2)) * (patternCount[ARM_NORTH][charge_i][eta_bin][spinPattern::MP] + patternCount[ARM_NORTH][charge_i][eta_bin][spinPattern::MM]));

      rawSingleAsymmetryPosEtaErr[ARM_NORTH][charge_i][eta_bin] = sqrt(fabs(err_top/err_bottom));
      double ratio = err_top/err_bottom;
      std::cout << "top: " << err_top << ", bottom: " << err_bottom 
          <<  " calc: " << sqrt(fabs(err_top/err_bottom)) 
          <<  " ratio: " << ratio << std::endl;

      err_top = 4*((patternCount[ARM_NORTH][charge_i][eta_bin][spinPattern::PP] + patternCount[ARM_NORTH][charge_i][eta_bin][spinPattern::MM]) * (pow(patternCount[ARM_NORTH][charge_i][eta_bin][spinPattern::MP] + patternCount[ARM_NORTH][charge_i][eta_bin][spinPattern::PM],2))) - 4*((pow(patternCount[ARM_NORTH][charge_i][eta_bin][spinPattern::PP] + patternCount[ARM_NORTH][charge_i][eta_bin][spinPattern::MM],2)) * (patternCount[ARM_NORTH][charge_i][eta_bin][spinPattern::MP] + patternCount[ARM_NORTH][charge_i][eta_bin][spinPattern::PM]));

      rawDoubleAsymmetryErr[ARM_NORTH][charge_i][eta_bin] = sqrt(fabs(err_top/err_bottom));
    }
  }
  return 0;
}

int AsymmetryCalculator::SaveRawAsymmetries(const std::string& rawAsymFile) {
  std::string txtFileName = rawAsymFile + ".txt";
  std::string csvFileName = rawAsymFile + ".csv";
  ofstream outTxtFile(txtFileName.c_str());
  ofstream outCsvFile(csvFileName.c_str());
  outTxtFile << "# Raw Asymmetries - Single and Double" << std::endl
      << "# Line format: arm charge eta_min eta_max AL(eta>0) AL_err(eta>0) AL(eta<0) AL_err(eta<0) ALL ALL_err" << std::endl;
  outCsvFile << "\"Raw Asymmetries - Single and Double\"" << std::endl
      << "\"arm\",\"charge\",\"eta_min\",\"eta_max\",\"AL(eta>0)\",\"AL_err(eta>0)\",\"AL(eta<0)\",\"AL_err(eta<0)\",\"ALL ALL_err\"" << std::endl;
  for(int arm_i = 0; arm_i < NUMBER_OF_ARM_CONFIGS; arm_i++) {
    for(int charge_i = 0; charge_i < NUMBER_OF_CHARGE_CONFIGS; charge_i++) {
      for( unsigned int eta_i = 0; eta_i < AsymEtaBins.size(); eta_i++) {
        outTxtFile << arm_i << " " << charge_i << " " <<  AsymEtaBins[eta_i].first << " " << AsymEtaBins[eta_i].second
          << " " << rawSingleAsymmetryPosEta[arm_i][charge_i][eta_i]
          << " " << rawSingleAsymmetryPosEtaErr[arm_i][charge_i][eta_i]
          << " " << rawSingleAsymmetryNegEta[arm_i][charge_i][eta_i]
          << " " << rawSingleAsymmetryNegEtaErr[arm_i][charge_i][eta_i]
          << " " << rawDoubleAsymmetry[arm_i][charge_i][eta_i]
          << " " << rawDoubleAsymmetryErr[arm_i][charge_i][eta_i]
          << std::endl;
        outCsvFile << "\"" <<  arm_i << "\",\"" << charge_i << "\",\"" <<  AsymEtaBins[eta_i].first << "\",\"" << AsymEtaBins[eta_i].second << "\","  
          << "\"" << rawSingleAsymmetryPosEta[arm_i][charge_i][eta_i]    << "\","
          << "\"" << rawSingleAsymmetryPosEtaErr[arm_i][charge_i][eta_i] << "\","
          << "\"" << rawSingleAsymmetryNegEta[arm_i][charge_i][eta_i]    << "\","
          << "\"" << rawSingleAsymmetryNegEtaErr[arm_i][charge_i][eta_i] << "\","
          << "\"" << rawDoubleAsymmetry[arm_i][charge_i][eta_i]          << "\","
          << "\"" << rawDoubleAsymmetryErr[arm_i][charge_i][eta_i]       << "\""
          << std::endl;
      }
    }
  }
  outTxtFile.close();
  return 0;
}

AsymmetryCalculator::~AsymmetryCalculator() {
  for(auto i = HelicityCrossCheck.begin(); i != HelicityCrossCheck.end(); ++i) {
    std::ofstream* str = i->second;
    delete str;
  }
  std::cout << "AsymmetryCalculator Finished" << std::endl;
}

int AsymmetryCalculator::DumpHelicityInformation(const std::string& outFileNameStub, int arm, int charge, int pat) {
  if( (charge < CHARGE_NEG) || (charge > CHARGE_POS) ) return -1;
  if( (arm    < ARM_SOUTH ) || (arm    > ARM_NORTH ) ) return -2;
  if( (pat  < spinPattern::PP ) || ( pat > spinPattern::EMPTY ) ) return -3;

  // Make Key
  std::stringstream key;
  key << arm << charge << pat;
  std::string outFileName = outFileNameStub + key.str() + ".txt";
  std::cout << "Dumping arm:" << arm << " charge:" << charge << " pat:" << pat << " to file: " << outFileName << std::endl;
  HelicityCrossCheck[key.str()] = new ofstream(outFileName);
  return 0;
}

int AsymmetryCalculator::XCheckPatternFile(const std::string& inFileName, int spin_pattern_check ) {
  BasicCutTreeVariables v;
  dataTree->BuildIndex("Run_Number","Evt_Number"); 
  v.LinkTree(dataTree, READ);

  std::cout << "Checking for events in " << inFileName << std::endl;
  // Load the run number + event number for lookup.
  std::vector< std::pair < int, Long64_t >> lookup; // Run_Number -> Evt_Number
  std::ifstream inFile(inFileName.c_str());
  if(inFile) {
    std::string line = "";
    while(getline(inFile,line)) {
      std::stringstream ss;
      int Run_Number = -999;
      int Evt_Number = -999;
      ss.str(line);
      ss >> Run_Number >> Evt_Number;
      if( (Run_Number == -999) || (Evt_Number == -999) ) {
        std::cout << "Couldn't process " << line << " check source code." << std::endl;
        continue;
      }
      lookup.push_back(std::make_pair(Run_Number,Evt_Number));
    }
  } else {
    std::cout << inFileName << " could not be opened " << std::endl;
  }
  for( auto v_itr = lookup.begin(); v_itr != lookup.end(); ++v_itr) {
    int Run_Number = v_itr->first;
    int Evt_Number = v_itr->second;
    Long64_t entry = dataTree->GetEntryNumberWithIndex(Run_Number, Evt_Number);
    pol.InitPat(Run_Number);
    if( entry == -1 ) {
      std::cout << "Missing entry. No data for: " << Run_Number << std::endl;
      continue;
    } 
    dataTree->GetEntry(entry);
    if( (Run_Number != v.Run_Number) || (Evt_Number != v.Evt_Number ) ) {
      std::cout << "  Index building is screwed up, run numbers and event numbers don't correspond to the correct tree index." << std::endl;
    }
    int spin_pattern = pol.GetPattern(v.clockcross);
    if( spin_pattern_check != spin_pattern ) {
      std::cout << "  Spin pattern for Run: " << Run_Number << " Event: " << Evt_Number << " was supposed to be " << spin_pattern_check
          << " but was instead " << spin_pattern << std::endl;
      std::cout << "  " << v.Run_Number << ", " << v.Evt_Number << ", " << v.clockcross << std::endl;
    }
    if( v.Wness < 0.99 ) {
      std::cout << "  Run: " << Run_Number 
          << " Event: " << Evt_Number 
          << " was skipped because its Wness was: " << v.Wness << std::endl; 
    }
    if(std::isnan(v.Wness) ) {
      std::cout << "  Run: " << Run_Number 
          << " Event: " << Evt_Number 
          << " was skipped because its Wness had a \"nan\" value." << std::endl;
    }
    if( (v.dw23 > 0.01) || (v.dw23 < -0.05) ) {
      std::cout << "  Run: " << Run_Number 
          << " Event: " << Evt_Number 
          << " was skipped because of its dw23 value: " << v.dw23 << std::endl;
    }
    if( v.pT < 16.0 ) {
      std::cout << "Run: " << Run_Number 
          << " Event: " << Evt_Number 
          << " was skipped because of its pT value: " << v.pT << std::endl;
    }
  }
  return 0;
}

int AsymmetryCalculator::SplitKey(std::string key, int& arm, int& charge, int& pat) {
  if(key.length() != 3 ) {
    std::cout << "MALFORMED KEY FOR HelicityCrossCheck Container: " << key << std::endl;
    return -1;
  }
  std::stringstream arm_s   ;  arm_s    << key[0];
  std::stringstream charge_s;  charge_s << key[1];
  std::stringstream pat_s   ;  pat_s    << key[2];

  arm    = atoi(arm_s.str().c_str());
  charge = atoi(charge_s.str().c_str());
  pat    = atoi(pat_s.str().c_str());
  return 0;
}

int AsymmetryCalculator::SetupEtaBins(int number_of_eta_bins) {
  std::cout << "Overriding default eta-binning" << std::endl;
  if(number_of_eta_bins < 1) {
    return -1;
  }
  AsymEtaBins.resize(0);
  patternCount.resize(0);
  MakeArmChargeSpinEtaContainer(patternCount,number_of_eta_bins);
  float low = 1.1;
  float binw = (2.6 - 1.1)/(static_cast<float>(number_of_eta_bins));
  float high = 1.1 + binw;
  AsymEtaBins.push_back( std::make_pair(1.1,2.6) );
  for(int i = 0; i < number_of_eta_bins; i++) {
    AsymEtaBins.push_back( std::make_pair(low, high));
    low = high;
    high = high + binw;
    std::cout << "  ETA BIN: " << i << ": " << AsymEtaBins.back().first << ", " << AsymEtaBins.back().second << std::endl;  
  }
  return 0;
}

int AsymmetryCalculator::LoadData(const std::string& fileName, const std::string& badSpinDBRunsName ) {
  dataFile = std::make_shared<TFile>(fileName.c_str(),"READ"); 
  dataTree = (TTree*)dataFile->Get(wness_tree.c_str());
  total_tracks = dataTree->GetEntries();
  std::cout << "Opened " << fileName << " and found a tree with " << total_tracks << " entries" << std::endl;
  
  std::ifstream inFile(badSpinDBRunsName);
  if(inFile) {
    std::string line = "";
    while( getline(inFile,line)) {
      std::stringstream run_ss;
      run_ss.str(line);
      int Run_Number;
      run_ss >> Run_Number;
      badSpinDBRuns.insert(Run_Number);
    }
    std::cout << "  Loaded " << badSpinDBRuns.size() << " runs." << std::endl;
  } else {
    std::cout << badSpinDBRunsName << " was not found." << std::endl;
  }
  SortTreeByRun();
  return total_tracks; 
}

int AsymmetryCalculator::Loop(){
  std::cout << "Getting polarization from database for each run" << std::endl;
  SetupHistograms(); // needs to be called here, in case eta bins were modified from the default.

  //dataTree->BuildIndex("Run_Number","Evt_Number"); // Segfault
  BasicCutTreeVariables v;
  v.LinkTree(dataTree,READ);

  Long64_t tree_entries = dataTree->GetEntries();
  for(Long64_t entry = 0; entry < tree_entries; entry++) {
    dataTree->GetEntry(entry);
    if ( badSpinDBRuns.find(v.Run_Number) != badSpinDBRuns.end() ) continue;
    if(CutEvent(v)) continue;
    int eta_bin = GetAsymEtaBin(v.eta);
    if(eta_bin < 0) { 
      std::cout 
          << " Out of range eta value: Run: " << v.Run_Number 
          << ", Evt: " << v.Evt_Number 
          << ", Eta: " << v.eta << std::endl;
      continue;
    } 

    int arm_i    = GetArmIndex    ( v.pz     );
    int charge_i = GetChargeIndex ( v.charge );

    pol.InitPat(v.Run_Number);
    int spin_pattern = pol.GetPattern(v.clockcross);
    
    if( spin_pattern < 0 || spin_pattern > NUMBER_OF_SPIN_PATTERNS ){
      std::cout << "    Run: " << v.Run_Number << " PROBLEM: spin_pattern: " << spin_pattern << std::endl;
      continue;
    }
    
    patternCount[arm_i][charge_i][spin_pattern][eta_bin] += 1;
    patternCount[arm_i][charge_i][spin_pattern][0      ] += 1; // whole eta range
    // Need to figure out new way of accessing runs.
    polarizationInRunBlue  ->SetBinContent(static_cast<int>(std::distance(SortedRuns.begin(), SortedRuns.find(v.Run_Number))), pol.GetPol(spinPattern::BLUE)   );
    polarizationInRunYellow->SetBinContent(static_cast<int>(std::distance(SortedRuns.begin(), SortedRuns.find(v.Run_Number))), pol.GetPol(spinPattern::YELLOW) );
    
    std::stringstream log_key;
    log_key << arm_i << charge_i << spin_pattern;
    // Lookup only succeeds if the key exists. The key is supplied in DumpHelicityInformation
    if ( HelicityCrossCheck.find(log_key.str()) != HelicityCrossCheck.end() ) {
      // (format: run event xid pT eta dw23 wness)
      *HelicityCrossCheck[log_key.str()]
          << v.Run_Number << " "
          << v.Evt_Number << " "
          << v.clockcross << " "
          << v.pT         << " "
          << v.eta        << " "
          << v.dw23       << " "  
          << v.Wness      
          << std::endl;
    }

    if(entry > 0) {
      std::cout << "Progress: " << entry << "/" << tree_entries << "               \r" << std::flush;
    }
    else if (entry == tree_entries - 1) {
      std::cout << "Progress: " << entry << "/" << tree_entries << "               \r" << std::flush;
      std::cout << std::endl;
    }
  }
  return 0;
}

int AsymmetryCalculator::SortTreeByRun() {
  std::cout << "Sorting data set by run" << std::endl;
  BasicCutTreeVariables v;
  v.LinkTree(dataTree,READ);
  // program runs slowly when we initialize pol for every event.
  std::vector< Long64_t > blank_vector;
  for(Long64_t entry_i = 0; entry_i < total_tracks; entry_i++) {
    dataTree->GetEntry(entry_i);
    SortedRuns.insert(v.Run_Number);
    if(entry_i % 10000 == 0) {
      std::cout << "Progress: " << entry_i << "/" << total_tracks << "               \r" << std::flush;
    }
  }
  return 0;
}

int AsymmetryCalculator::Register(TObject* obj) {
  registry.push_back(obj);
  return registry.size();
}

bool AsymmetryCalculator::CutEvent(const BasicCutTreeVariables& v){
  bool cut_event = false;
  if( v.Wness < 0.99  ) cut_event=true;
  if(std::isnan(v.Wness) ) cut_event=true;
  if( v.charge > 0 ) { // cut around peak of dw23 for positive charge
    if( (v.dw23 < -0.01) || (v.dw23 > 0.05) ) cut_event=true;
  } else if ( v.charge < 0 ) { // cut around peak of dw23 for negative charge
    if( (v.dw23 < -0.05) || (v.dw23 > 0.01) ) cut_event=true;
  }
  if( v.pT < 16.0 ) cut_event=true;
  return cut_event;
}

int AsymmetryCalculator::SetupHistograms() {
  // eventsInRun must be initialized after the tree is sorted.
  polarizationInRunBlue = std::make_shared<TH1F>("polarizationInRunBlue","Polarization Per Run Blue Beam",SortedRuns.size(),0,SortedRuns.size()-1);
  polarizationInRunYellow = std::make_shared<TH1F>("polarizationInRunYellow","Polarization Per Run Yellow Beam",SortedRuns.size(),0,SortedRuns.size()-1);
  Register(polarizationInRunBlue.get());
  Register(polarizationInRunYellow.get());
  return 0;
}

int AsymmetryCalculator::SaveHistograms(const std::string& saveTo ) {
  TFile* fout = new TFile(saveTo.c_str(), "RECREATE");
  for(unsigned int i = 0; i < registry.size(); i++) {
    fout->cd();
    registry[i]->Write();
  }
  fout->Write();
  delete fout;
  return 0;
}

std::string AsymmetryCalculator::GetPatternString(int pat_i) {
  std::string pattern = "EMPTY";
  switch(pat_i)
  {
    case (spinPattern::PP):
      pattern = "++"; 
      break;
    case (spinPattern::PM):
      pattern = "+-";
      break;
    case (spinPattern::MP):
      pattern = "-+";
      break;
    case (spinPattern::MM):
      pattern = "--";
      break;
  }
  return pattern;
}

int AsymmetryCalculator::SaveHelicityCount(const std::string& outFileStub) {
  std::string outFileCSV = outFileStub + ".csv";
  std::string outFileTxt = outFileStub + ".txt";
  std::string outFileEtaCSV = outFileStub + "_eta_bins.csv";
  std::string outFileEtaTxt = outFileStub + "_eta_bins.txt";
  std::ofstream csvFile(outFileCSV.c_str());
  std::ofstream txtFile(outFileTxt.c_str());
  std::ofstream csvFileAsymEta(outFileEtaCSV.c_str());
  std::ofstream txtFileAsymEta(outFileEtaTxt.c_str());

  txtFile << "# HELICITY PATTERN SUMMARY"
      << std::endl
      << "# ARM CHARGE PATTERN_INDEX PATTERN_STRING MUON_COUNT " << std::endl;
  std::cout << "# HELICITY PATTERN SUMMARY"
      << std::endl
      << "# ARM CHARGE PATTERN_INDEX PATTERN_STRING MUON_COUNT " << std::endl;

  csvFile 
      << "\"ARM\",\"CHARGE\",\"PATTERN INDEX\",\"HELICITY COMBINATION\",\"MUON COUNT\"" << std::endl;
  
  csvFileAsymEta
      << "\"ARM\",\"CHARGE\",\"PATTERN INDEX\",\"HELICITY COMBINATION\",\"ETA BIN\",\"ETA MIN\",\"ETA MAX\",\"MUON COUNT\"" << std::endl;
  
  txtFileAsymEta
      << "# HELICITY PATTERN SUMMARY - ETA BINS" << std::endl
      << "# ARM CHARGE PATTERN_INDEX HELICITY_COMBINATION ETA_BIN ETA_MIN ETA_MAX MUON_COUNT" << std::endl;
  
  for(int arm_i = 0; arm_i < NUMBER_OF_ARM_CONFIGS; arm_i++) {
    for(int charge_i = 0; charge_i < NUMBER_OF_CHARGE_CONFIGS; charge_i++) {
      for(int pat_i = 0; pat_i < NUMBER_OF_SPIN_PATTERNS; pat_i++) {
        txtFile << arm_i 
            << "\t" << charge_i 
            << "\t" << pat_i 
            << "\t" << GetPatternString(pat_i) 
            << "\t" << patternCount[arm_i][charge_i][pat_i][0]
            << std::endl;
        std::cout << arm_i << "\t" << charge_i << "\t" << pat_i << "\t" << GetPatternString(pat_i) << "\t"  << patternCount[arm_i][charge_i][pat_i][0] << std::endl;
        csvFile 
            << "\"" << arm_i                                << "\","
            << "\"" << charge_i                             << "\","
            << "\"" << pat_i                                << "\","
            << "\"" << GetPatternString(pat_i)              << "\","
            << "\"" << patternCount[arm_i][charge_i][pat_i][0] << "\""
            << std::endl;
        for( unsigned int eta_i = 1; eta_i < AsymEtaBins.size(); eta_i++) {
          csvFileAsymEta 
              << "\"" << arm_i                                       << "\","
              << "\"" << charge_i                                    << "\","
              << "\"" << pat_i                                       << "\","
              << "\"" << GetPatternString(pat_i)                     << "\","
              << "\"" << eta_i                                       << "\","
              << "\"" << AsymEtaBins[eta_i].first                    << "\","
              << "\"" << AsymEtaBins[eta_i].second                   << "\","
              << "\"" << patternCount[arm_i][charge_i][pat_i][eta_i] << "\""
              << std::endl;
           txtFileAsymEta 
              << arm_i                               
              << "\t" << charge_i                    
              << "\t" << pat_i                       
              << "\t" << GetPatternString(pat_i)     
              << "\t" << eta_i                       
              << "\t" << AsymEtaBins[eta_i].first    
              << "\t" << AsymEtaBins[eta_i].second   
              << "\t" << patternCount[arm_i][charge_i][pat_i][eta_i] 
              << std::endl;
        }
      }
    }
  }

  csvFile.close();
  txtFile.close();

  return 0;
}

int AsymmetryCalculator::GetAsymEtaBin(float eta) {
  int bad_bin = -1;
  int bin = 0;
  for(auto i = std::next( AsymEtaBins.begin()); i != AsymEtaBins.end(); ++i) {
    if( (eta > i->first) && (eta < i->second) ) {
      return bin;
    }
    bin++;
  }
  return bad_bin;
}
