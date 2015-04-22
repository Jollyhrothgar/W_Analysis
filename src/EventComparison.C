#include "EventComparison.h"

EventComparison::EventComparison()
{
  std::cout << "Running Event Comparison" << std::endl;
}

EventComparison::~EventComparison()
{
  std::cout << "Finished with Event Comparison" << std::endl;
  std::map<std::string, std::pair<TFile*, TTree* > >::iterator del_itr;
  for(del_itr = data_set.begin(); del_itr != data_set.end(); ++del_itr)
  {
    delete (del_itr->second).first;
  }
}

int EventComparison::AddComparisonTree(std::string owner, std::string treeName)
{
  TFile* f = NULL;
  TTree* t = NULL;
  f = new TFile(treeName.c_str(), "READ");
  if(f)
  {
    t = (TTree*)f->Get("newsngmuons_basic_cut");
    if(t)
    {
      std::cout << "Loading " << std::setw(12) << owner 
          << " from file " << treeName << " (" << t->GetEntriesFast() << " entries)" << std::endl;
    }
    else
    {
      std::cerr << "Couldn't get " << "newsngmuons_basic_cut" << " from " << treeName << std::endl;
      return -2;
    }
  }
  else
  {
    std::cerr << "Couldn't load " << treeName << std::endl;
    return -1;
  }
  data_set[owner] = std::make_pair(f,t);
  return data_set.size();
}
