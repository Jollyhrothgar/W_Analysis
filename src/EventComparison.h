#ifndef __EVENT_COMPARISON_H__
#define __EVENT_COMPARISON_H__

// STL
#include<iostream>
#include<map>
#include<algorithm>
#include<iomanip>
// ROOT
#include "TFile.h"
#include "TTree.h"
// CUSTOM

class EventComparison
{
  public: 
    EventComparison();
    ~EventComparison();
    int AddComparisonTree(std::string owner, std::string treeName);
    
  private:
    std::map<std::string, std::pair<TFile*, TTree*> > data_set;
    std::map<int, std::map<int, std::vector<Long64_t> > > event_map; // Run_Number -> Event_Number -> [EVT_INDEX_1, EVT_INDEX_2, ... ]
};
#endif
