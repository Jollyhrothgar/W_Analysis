#include "MergeTrees.h"

int MergeTrees(std::string treeList, std::string outRoot)
{
	TFile* inFile;
	TTree* inTree;

	BasicCutTreeVariables outVars;
	TFile * outFile = new TFile(outRoot.c_str(), "RECREATE");	
	TTree * outTree = new TTree(wness_tree.c_str(), "Wness For Events with RPC_1||3 and FVTX. DCA_r is not correllated to chi2");
	outVars.LinkTree(outTree, WRITE);

	ifstream readList(treeList.c_str());
	std::string treeFileName;
  
  int zero_entries = 0;
  int few_entries = 0;
  int total_trees = 0;

	BasicCutTreeVariables inVars;
	if(readList)
	{
		while(getline(readList,treeFileName))
		{
			inVars.Reset();
			
			inFile = new TFile(treeFileName.c_str(),"READ");
			inTree = (TTree*)inFile->Get(wness_tree.c_str());

			if(!inTree) { std::cout << "bad tree pointer for " << treeFileName << std::endl; continue; }
			inVars.LinkTree(inTree, READ);
			
			Long64_t entries = inTree->GetEntries();

      total_trees++;
			if(entries == 0 )
			{
        zero_entries++;
			}
      else if( entries < 10)
      {
        few_entries++;
      }
			
			for (Long64_t i = 0; i < entries; i++)
			{
				inTree->GetEntry(i);
				outVars = inVars;
				outTree->Fill();
			}

			delete inFile;
		}
	}
	else
	{
		std::cout << treeList << " was not opened." << std::endl;
	}
	readList.close();
	outFile->Write();
	delete outTree;
	delete outFile;

  std::cout << "In list: " << treeList << std::endl;
  std::cout << "\tThere were " << zero_entries << "/" << total_trees << " trees with zero entries in " << std::endl;
  std::cout << "\tThere were " << few_entries << "/" << total_trees << " trees with fewer than 10 entries" << std::endl;
	
	return 0;
}
