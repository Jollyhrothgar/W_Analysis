void MergeCondorOutput()
{
  gSystem->Load("libSingleMuonsWAnalysis.so");
  std::string lists_dir = "/direct/phenix+spin2/beaumim/WAnalysisRun13/AnalysisCode/runlists/";
  std::string root_trees_dir = "/direct/phenix+spin2/beaumim/WAnalysisRun13/AnalysisCode/macros/rootTrees/";
  MergeTrees( lists_dir +"bottom_tree.txt"   , root_trees_dir + "Run13_bottom_nocalc.root"           );
  MergeTrees( lists_dir +"charm_tree.txt"    , root_trees_dir + "Run13_charm_nocalc.root"            );
  MergeTrees( lists_dir +"dy_tree.txt"       , root_trees_dir + "Run13_dy_nocalc.root"               );
  MergeTrees( lists_dir +"onium_tree.txt"    , root_trees_dir + "Run13_onium_nocalc.root"            );
  MergeTrees( lists_dir +"only_z_tree.txt"   , root_trees_dir + "Run13_onlyz_nocalc.root"            );
  MergeTrees( lists_dir +"whad_tree.txt"     , root_trees_dir + "Run13_whad_nocalc.root"             );
  MergeTrees( lists_dir +"wsim_tree.txt"     , root_trees_dir + "Run13_WSim_nocalc.root"             );
  MergeTrees( lists_dir +"wtau_tree.txt"     , root_trees_dir + "Run13_wtau_nocalc.root"             );
  MergeTrees( lists_dir +"zsum_tree.txt"     , root_trees_dir + "Run13_zsum_nocalc.root"             );
  MergeTrees( lists_dir +"data_prod_tree.txt", root_trees_dir + "Run13_Data_prod_nocalc_needsfilter.root" );
  MergeTrees( lists_dir +"data_OT_tree.txt"  , root_trees_dir + "Run13_Data_OT_nocalc_needsfilter.root" );
  MergeTrees( lists_dir +"merge_OT_prod.txt" , root_trees_dir + "Run13_Data_Merged_Needs_Filter.root");
  DuplicateSearch("/direct/phenix+spin2/beaumim/WAnalysisRun13/AnalysisCode/macros/rootTrees/Run13_Data_Merged_Needs_Filter.root"
		 ,"/direct/phenix+spin2/beaumim/WAnalysisRun13/AnalysisCode/macros/rootTrees/Run13_data_uniq_needscalc.root");
}
