#include "DuplicateSearch.h"

int DuplicateSearch(std::string searchFileName, std::string uniqFileName)
{
  std::cout << "Filtering out duplicate events from " << searchFileName << " and sending to " << uniqFileName << std::endl;
  TFile* searchFile = new TFile(searchFileName.c_str(),"READ");
  TTree* searchTree = (TTree*)searchFile->Get(wness_tree.c_str());
  BasicCutTreeVariables var;
  var.LinkTree(searchTree,READ);

  TFile* uniqFile = new TFile(uniqFileName.c_str(),"RECREATE");
  TTree* uniqTree = new TTree(wness_tree.c_str(), "Wness Tree - Data");
  BasicCutTreeVariables uniqVars;
  uniqVars.LinkTree(uniqTree,WRITE);

  std::map< int, std::set< int >  > evtMap;
  std::pair<std::set<int>::iterator,bool> ret; // insert returns pair - first being the iterator to the entry, second being whether or not entry was contained.
  std::map< int, std::map< int, std::vector<Long64_t> > > dupe_map;

  std::cout << "--Searching " << searchTree->GetName() << " for Duplicated Events--" << std::endl;
  long int entry_counter = 0;
  for(int i = 0; i < searchTree->GetEntries(); i++)
  {
    searchTree->GetEntry(i);
    ret = evtMap[var.Run_Number].insert(var.Evt_Number);
    dupe_map[var.Run_Number][var.Evt_Number].push_back(i);

    if( ret.second == false )
    {
      entry_counter++;
    }
  }

  std::map< int, std::map< int, std::vector<Long64_t> > >::iterator dupe_itr; 
  /*
  std::cout << "checking that events which are flagged as duplicates are REALLY duplicates" << std::endl;
  for(dupe_itr = dupe_map.begin(); dupe_itr != dupe_map.end(); ++dupe_itr)
  {
    std::map< int, std::vector<Long64_t> > evt_map = dupe_itr->second;
    std::map< int, std::vector<Long64_t> >::iterator evt_itr;
    for(evt_itr = evt_map.begin(); evt_itr != evt_map.end(); ++evt_itr)
    {
      if( (evt_itr->second).size() > 1 )
      {
        std::vector< Long64_t > evt_i = evt_itr->second;
        std::vector< Long64_t >::iterator evt_i_itr;
        searchTree->GetEntry(*(evt_i.begin()));
        double DG0_first = var.DG0;
        double DCA_first = var.DCA_r;
        for(evt_i_itr = evt_i.begin(); evt_i_itr != evt_i.end(); ++evt_i_itr)
        {
          searchTree->GetEntry(*evt_i_itr);
          double DG0_second = var.DG0;
          double DCA_second = var.DCA_r;
          if( fabs( abs(DG0_first - DG0_second) ) > 0.001 ) 
          {
            std::cout << "DGO IS NOT DUPLICATED FOR RUN[EVENT]: " << var.Run_Number << "[" << var.Evt_Number << "]" << std::endl;
          }
          if( fabs( abs(DCA_first - DCA_second) ) > 0.001 ) 
          {
            std::cout << "DCA_r IS NOT DUPLICATED FOR RUN[EVENT]: " << var.Run_Number << "[" << var.Evt_Number << "]" << std::endl;
          }
        }
      }
    }
  }
  */

  int runs_processed = 0;
  std::cout << "Sorting Tree For Faster Performance" << std::endl;
  // Sort the entries from the tree so it runs faster.
  std::set<Long64_t> entries; // fill with entry index we want to copy
  for(dupe_itr = dupe_map.begin(); dupe_itr != dupe_map.end(); ++dupe_itr)
  {
    runs_processed++;
    std::cout << "Progress: " << std::setprecision(4) << runs_processed << " runs               \r" << std::flush;
    std::map< int, std::vector<Long64_t> > evt_map = dupe_itr->second;
    std::map< int, std::vector<Long64_t> >::iterator evt_itr;
    for(evt_itr = evt_map.begin(); evt_itr != evt_map.end(); ++evt_itr)
    {
      std::vector< Long64_t > evt_i = evt_itr->second;
      entries.insert(evt_i[0]);
    }
  }
  std::cout << "We will now copy over " << entries.size() << " entries." << std::endl;
  
  std::cout << "Dumping uniqe tree from " << searchFileName << " to " << uniqFileName << std::endl;
  runs_processed = 0;
  for(std::set<Long64_t>::iterator entry_i = entries.begin(); entry_i != entries.end(); ++entry_i)
  {
    runs_processed++;
    std::cout << "Progress: " << *entry_i << " entries               \r" << std::flush;
    searchTree->GetEntry(*entry_i);
    uniqVars = var;
    uniqTree->Fill();
  }
  std::cout << entry_counter << " duplicates removed" << std::endl;
  searchFile->Close();
  delete searchFile;

  uniqFile->cd();
  uniqTree->Write();
  delete uniqTree;
  delete uniqFile;
	return 0;
}

