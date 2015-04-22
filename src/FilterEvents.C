#include "FilterEvents.h"
#include <fstream>

// the filterTreeName is the standard basic cut tree, with all desired data merged into one tree.
// One expects that some events will be duplicated when mergeing many disparate files, and so the std::map
// and std::set STL containers' properties are exploited here when looping over the data set to filter out
// only unique Evt_Numbers for a given Run_Number by mapping a set of Evt_Numbers to map using Run_Number
// as a key.
// 
// These unique entries are written out to a file called writeTreeName.
//
int FilterEvents( std::string filterTreeName, std::string writeTreeName)
{
  typedef std::map<std::string, int> trigMap; 
  typedef std::map<std::string, int>::iterator triggerItr; 
  trigMap TRIG;
  TRIG["RPC1C"]                    = 0x00200000;
  TRIG["OT1"]                      = 0x00040000;
  TRIG["OT1"]                      = 0x00080000;

  BasicCutTreeVariables writeTreeVars;
  BasicCutTreeVariables readTreeVars;
  writeTreeVars.Reset();
  readTreeVars.Reset();

  TFile* readFile = new TFile(filterTreeName.c_str(), "READ");
  TTree* readTree = (TTree*)readFile->Get(wness_tree.c_str());

  TFile* outFile = new TFile(writeTreeName.c_str(), "RECREATE");
  TTree* writeTree = new TTree(wness_tree.c_str(),"Filtered Analysis Tree - merged with reproduced data");

  if(!readTree) 
  {
    std::cerr << "Invalid tree  " << readTree->GetName() << " from " << filterTreeName << std::endl;
    return -1;
  }

  writeTreeVars.LinkTree(writeTree,WRITE);
  readTreeVars.LinkTree(readTree,READ);

  for(Long64_t tree_index = 0; tree_index < readTree->GetEntries(); tree_index++)
  {
    readTree->GetEntry(tree_index);
    if( 
		(((readTreeVars.triggerbit) & (TRIG["RPC1C"])) > 0)
		|| (((readTreeVars.triggerbit) & (TRIG["OT1"])) > 0)
		|| (((readTreeVars.triggerbit) & (TRIG["OT2"])) > 0)
		)
    {  // we have a RPC1C event
      writeTreeVars = readTreeVars;  
      writeTree->Fill();
    }
  }

  int missing_events = writeTree->GetEntries();
  outFile->Write();
  delete writeTree;
  delete outFile;
  delete readFile;

  std::cout << "  There were " << missing_events << " RPC1C events" << std::endl;
  return missing_events;
}
