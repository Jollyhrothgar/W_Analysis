#include "SingleMuons.h"

SingleMuons::SingleMuons()
{
  Reset();
}

SingleMuons::SingleMuons(TTree* tree)
{
  Reset();
  RunTree = tree;
  InitBranches(RunTree);
  InitLeaves();
}

SingleMuons::~SingleMuons()
{
  Reset();
  //coneBitsCheck.close();
}

int SingleMuons::InitBranches(TTree* tree)
{
  if(RunTree == NULL)
  {
    std::cout << "TTree pointer is set to NULL." << std::endl;
    return -1;
  }

  // Get Branches From Tree
  b__RecoTracks  = tree->GetBranch ( "_RecoTracks"   );
  b_RecoTracks   = tree->GetBranch ( "RecoTracks"    );
  b_Eventdata    = tree->GetBranch ( "Eventdata"     );
  b_RpcMatchSt1  = tree->GetBranch ( "RpcMatchSt1"   );
  b_RpcMatchSt3  = tree->GetBranch ( "RpcMatchSt3"   );
  b__RpcHits     = tree->GetBranch ( "_RpcHits"      );
  b_FvtxMatch    = tree->GetBranch ( "FvtxMatch"     );

  // Delete old object when done
  b__RecoTracks  ->SetAutoDelete( kTRUE );
  b_RecoTracks   ->SetAutoDelete( kTRUE );
  b_Eventdata    ->SetAutoDelete( kTRUE );
  b_RpcMatchSt1  ->SetAutoDelete( kTRUE );
  b_RpcMatchSt3  ->SetAutoDelete( kTRUE );
  b__RpcHits     ->SetAutoDelete( kTRUE );
  b_FvtxMatch    ->SetAutoDelete( kTRUE );

  return 0;
}

// Map all values to tree
// Call before Loop
int SingleMuons::InitLeaves()
{
  // b_EventData Leaves	
  l_Run_Number  = b_Eventdata->GetLeaf("Run_Number" );
  l_Evt_Number  = b_Eventdata->GetLeaf("Evt_Number" );
  l_Evt_bbcZ    = b_Eventdata->GetLeaf("Evt_bbcZ"   );
  l_Evt_Z       = b_Eventdata->GetLeaf("Evt_Z");
  l_Evt_zdcEnerN= b_Eventdata->GetLeaf("Evt_zdcEnerN");
  l_triggerbit  = b_Eventdata->GetLeaf("triggerbit" );
  l_triggerlive = b_Eventdata->GetLeaf("triggerlive");
  l_clockcross  = b_Eventdata->GetLeaf("clockcross" );

  // Map Dynamic Array Sizes
  l__RecoTracks  = b__RecoTracks->GetLeaf("_RecoTracks");	
  l__RpcHits     = b__RpcHits->GetLeaf("_RpcHits");

  // b_RecoTracks Leaves
  l_Evt_Nmu  = b_RecoTracks->GetLeaf("Evt_Nmu");
  l_charge   = b_RecoTracks->GetLeaf("charge");
  l_px       = b_RecoTracks->GetLeaf("px");
  l_py       = b_RecoTracks->GetLeaf("py");
  l_pz       = b_RecoTracks->GetLeaf("pz");

  l_pxSt1    = b_RecoTracks->GetLeaf("pxSt1");
  l_pySt1    = b_RecoTracks->GetLeaf("pySt1");
  l_pzSt1    = b_RecoTracks->GetLeaf("pzSt1");

  l_pxSt2    = b_RecoTracks->GetLeaf("pxSt2");
  l_pySt2    = b_RecoTracks->GetLeaf("pySt2");
  l_pzSt2    = b_RecoTracks->GetLeaf("pzSt2");

  l_pxSt3    = b_RecoTracks->GetLeaf("pxSt3");
  l_pySt3    = b_RecoTracks->GetLeaf("pySt3");
  l_pzSt3    = b_RecoTracks->GetLeaf("pzSt3");

  l_pT       = b_RecoTracks->GetLeaf("pT");
  l_p        = b_RecoTracks->GetLeaf("p");
  l_chi2     = b_RecoTracks->GetLeaf("chi2");
  l_lastGap  = b_RecoTracks->GetLeaf("lastGap");
  l_eta      = b_RecoTracks->GetLeaf("eta");
  l_phi      = b_RecoTracks->GetLeaf("phi");
  l_DG0      = b_RecoTracks->GetLeaf("DG0");
  l_DDG0     = b_RecoTracks->GetLeaf("DDG0");
  l_xSt1     = b_RecoTracks->GetLeaf("xSt1");
  l_ySt1     = b_RecoTracks->GetLeaf("ySt1");
  l_xSt2     = b_RecoTracks->GetLeaf("xSt2");
  l_ySt2     = b_RecoTracks->GetLeaf("ySt2");
  l_xSt3     = b_RecoTracks->GetLeaf("xSt3");
  l_ySt3     = b_RecoTracks->GetLeaf("ySt3");
  l_DG4      = b_RecoTracks->GetLeaf("DG4");
  l_DCA_r    = b_RecoTracks->GetLeaf("DCA_r");
  l_DCA_z    = b_RecoTracks->GetLeaf("DCA_z");
  l_ELoss        = b_RecoTracks->GetLeaf("ELoss");
  l_mutr_nhits   = b_RecoTracks->GetLeaf("mutr_nhits");
  l_muid_nhits   = b_RecoTracks->GetLeaf("muid_nhits");
  l_ref_vtx_rdca = b_RecoTracks->GetLeaf("ref_vtx_rdca");
  l_ref_vtx_r    = b_RecoTracks->GetLeaf("ref_vtx_r");
  l_ref_vtx_z    = b_RecoTracks->GetLeaf("ref_vtx_z");
  l_refit_zvtx   = b_RecoTracks->GetLeaf("refit_zvtx");
  l_idhits       = b_RecoTracks->GetLeaf("idhits");
  l_idquad       = b_RecoTracks->GetLeaf("idquad");
  l_trhits        = b_RecoTracks->GetLeaf("trhits");

  // b_FvtxMatch branch
  l_fvtx_dphi      = b_FvtxMatch->GetLeaf("fvtx_dphi");
  l_fvtx_dtheta    = b_FvtxMatch->GetLeaf("fvtx_dtheta");
  l_fvtx_dr        = b_FvtxMatch->GetLeaf("fvtx_dr");
  l_fvtx_conebits  = b_FvtxMatch->GetLeaf("fvtx_conebits");
  l_fvtx_tracklconebits = b_FvtxMatch->GetLeaf("fvtx_tracklconebits");

  // b_RpcMatchSt1 Leaves
  l_RpcMatchSt1_Rpc3dca   = b_RpcMatchSt1->GetLeaf("Rpc3dca");
  l_RpcMatchSt1_Rpc3time  = b_RpcMatchSt1->GetLeaf("Rpc3time");
  l_RpcMatchSt1_Rpc3x     = b_RpcMatchSt1->GetLeaf("Rpc3x");
  l_RpcMatchSt1_Rpc3y     = b_RpcMatchSt1->GetLeaf("Rpc3y");
  l_RpcMatchSt1_Rpc1dca   = b_RpcMatchSt1->GetLeaf("Rpc1dca");
  l_RpcMatchSt1_Rpc1time  = b_RpcMatchSt1->GetLeaf("Rpc1time");
  l_RpcMatchSt1_Rpc1x     = b_RpcMatchSt1->GetLeaf("Rpc1x");
  l_RpcMatchSt1_Rpc1y     = b_RpcMatchSt1->GetLeaf("Rpc1y");

  // b_RpcMatchSt3 Leaves
  l_RpcMatchSt3_Rpc3dca   = b_RpcMatchSt3->GetLeaf("Rpc3dca");
  l_RpcMatchSt3_Rpc3time  = b_RpcMatchSt3->GetLeaf("Rpc3time");
  l_RpcMatchSt3_Rpc3x     = b_RpcMatchSt3->GetLeaf("Rpc3x");
  l_RpcMatchSt3_Rpc3y     = b_RpcMatchSt3->GetLeaf("Rpc3y");
  l_RpcMatchSt3_Rpc1dca   = b_RpcMatchSt3->GetLeaf("Rpc1dca");
  l_RpcMatchSt3_Rpc1time  = b_RpcMatchSt3->GetLeaf("Rpc1time");
  l_RpcMatchSt3_Rpc1x     = b_RpcMatchSt3->GetLeaf("Rpc1x");
  l_RpcMatchSt3_Rpc1y     = b_RpcMatchSt3->GetLeaf("Rpc1y");

  return 0;
}

int SingleMuons::GetEvent(Long64_t i)
{
  ResetContainers();
  RunTree->GetEvent(i);
  if(l_Run_Number  ) { Run_Number   = l_Run_Number->GetValue(0);   } // already initialized to default value 
  if(l_Evt_Number  ) { Evt_Number   = l_Evt_Number->GetValue(0);   } //  "" 
  if(l_Evt_bbcZ    ) { Evt_bbcZ     = l_Evt_bbcZ->GetValue(0);     } //  "" 
  if(l_Evt_Z       ) { Evt_Z        = l_Evt_Z->GetValue(0);        } //  "" 
  if(l_Evt_zdcEnerN) { Evt_zdcEnerN = l_Evt_zdcEnerN->GetValue(0); } //  "" 
  if(l_triggerbit  ) { triggerbit   = l_triggerbit->GetValue(0);   } //  "" 
  if(l_triggerlive ) { triggerlive  = l_triggerlive->GetValue(0);  } //  "" 
  if(l__RecoTracks ) { _RecoTracks  = l__RecoTracks->GetValue(0);  } //  "" 
  if(l__RpcHits    ) { _RpcHits     = l__RpcHits->GetValue(0);     } //  "" 
  if(l_clockcross  ) { clockcross   = l_clockcross->GetValue(0);   } //  "" 

  // Set up arrays for _RecoTracks indexing
  if(_RecoTracks > 0 )
  {
    // b_RecoTracks
    Evt_Nmu.resize(_RecoTracks);		
    charge .resize(_RecoTracks);
    px     .resize(_RecoTracks);
    py     .resize(_RecoTracks);
    pz     .resize(_RecoTracks);
    pxSt1  .resize(_RecoTracks); 
    pySt1  .resize(_RecoTracks); 
    pzSt1  .resize(_RecoTracks); 
    pxSt2  .resize(_RecoTracks); 
    pySt2  .resize(_RecoTracks); 
    pzSt2  .resize(_RecoTracks); 
    pxSt3  .resize(_RecoTracks); 
    pySt3  .resize(_RecoTracks); 
    pzSt3  .resize(_RecoTracks); 
    pT     .resize(_RecoTracks);
    p      .resize(_RecoTracks);
    chi2   .resize(_RecoTracks);
    lastGap.resize(_RecoTracks);
    eta    .resize(_RecoTracks);
    phi    .resize(_RecoTracks);
    DG0    .resize(_RecoTracks);
    DDG0   .resize(_RecoTracks);
    xSt1   .resize(_RecoTracks);
    ySt1   .resize(_RecoTracks);
    xSt2   .resize(_RecoTracks);
    ySt2   .resize(_RecoTracks);
    xSt3   .resize(_RecoTracks);
    ySt3   .resize(_RecoTracks);
    DG4    .resize(_RecoTracks);
    DCA_r  .resize(_RecoTracks);
    DCA_z  .resize(_RecoTracks);
    ELoss        .resize(_RecoTracks);
    mutr_nhits   .resize(_RecoTracks);
    muid_nhits   .resize(_RecoTracks);
    ref_vtx_rdca .resize(_RecoTracks);
    ref_vtx_r    .resize(_RecoTracks);
    ref_vtx_z    .resize(_RecoTracks);
    refit_zvtx   .resize(_RecoTracks);
    idhits       .resize(_RecoTracks);
    idquad       .resize(_RecoTracks);
    trhits        .resize(_RecoTracks);

    // b_FvtxMatch
    fvtx_dphi   .resize(_RecoTracks);
    fvtx_dtheta .resize(_RecoTracks);
    fvtx_dr     .resize(_RecoTracks);
    fvtx_conebits.resize(_RecoTracks);
    fvtx_tracklconebits.resize(_RecoTracks);
    fvtx_cone.resize(_RecoTracks);
    fvtx_tracklet.resize(_RecoTracks);

    // b_RpcMatchSt1
    RpcMatchSt1_Rpc3dca .resize(_RecoTracks);
    RpcMatchSt1_Rpc3time.resize(_RecoTracks);
    RpcMatchSt1_Rpc3x   .resize(_RecoTracks);
    RpcMatchSt1_Rpc3y   .resize(_RecoTracks);
    RpcMatchSt1_Rpc1dca .resize(_RecoTracks);
    RpcMatchSt1_Rpc1time.resize(_RecoTracks);
    RpcMatchSt1_Rpc1x   .resize(_RecoTracks);
    RpcMatchSt1_Rpc1y   .resize(_RecoTracks);

    // b_RpcMatchSt3
    RpcMatchSt3_Rpc3dca .resize(_RecoTracks);
    RpcMatchSt3_Rpc3time.resize(_RecoTracks);
    RpcMatchSt3_Rpc3x   .resize(_RecoTracks);
    RpcMatchSt3_Rpc3y   .resize(_RecoTracks);
    RpcMatchSt3_Rpc1dca .resize(_RecoTracks);
    RpcMatchSt3_Rpc1time.resize(_RecoTracks);
    RpcMatchSt3_Rpc1x   .resize(_RecoTracks);
    RpcMatchSt3_Rpc1y   .resize(_RecoTracks);

    for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++)
    {
      // b_RecoTracks
      if(l_Evt_Nmu) { Evt_Nmu[Track_i] =  l_Evt_Nmu->GetValue(Track_i); } else { Evt_Nmu[Track_i] = -9999; }		
      if(l_charge ) { charge [Track_i] =  l_charge ->GetValue(Track_i); } else { charge [Track_i] = -9999.; }
      if(l_px     ) { px     [Track_i] =  l_px     ->GetValue(Track_i); } else { px     [Track_i] = -9999.; }
      if(l_py     ) { py     [Track_i] =  l_py     ->GetValue(Track_i); } else { py     [Track_i] = -9999.; }
      if(l_pz     ) { pz     [Track_i] =  l_pz     ->GetValue(Track_i); } else { pz     [Track_i] = -9999.; }
      if(l_pxSt1  ) { pxSt1  [Track_i] = l_pxSt1   ->GetValue(Track_i); } else { pxSt1  [Track_i] = -9999.; }
      if(l_pySt1  ) { pySt1  [Track_i] = l_pySt1   ->GetValue(Track_i); } else { pySt1  [Track_i] = -9999.; }
      if(l_pzSt1  ) { pzSt1  [Track_i] = l_pzSt1   ->GetValue(Track_i); } else { pzSt1  [Track_i] = -9999.; }
      if(l_pxSt2  ) { pxSt2  [Track_i] = l_pxSt2   ->GetValue(Track_i); } else { pxSt2  [Track_i] = -9999.; }
      if(l_pySt2  ) { pySt2  [Track_i] = l_pySt2   ->GetValue(Track_i); } else { pySt2  [Track_i] = -9999.; }
      if(l_pzSt2  ) { pzSt2  [Track_i] = l_pzSt2   ->GetValue(Track_i); } else { pzSt2  [Track_i] = -9999.; }
      if(l_pxSt3  ) { pxSt3  [Track_i] = l_pxSt3   ->GetValue(Track_i); } else { pxSt3  [Track_i] = -9999.; }
      if(l_pySt3  ) { pySt3  [Track_i] = l_pySt3   ->GetValue(Track_i); } else { pySt3  [Track_i] = -9999.; }
      if(l_pzSt3  ) { pzSt3  [Track_i] = l_pzSt3   ->GetValue(Track_i); } else { pzSt3  [Track_i] = -9999.; }
      if(l_pT     ) { pT     [Track_i] = l_pT      ->GetValue(Track_i); } else { pT     [Track_i] = -9999.; }
      if(l_p      ) { p      [Track_i] = l_p       ->GetValue(Track_i); } else { p      [Track_i] = -9999.; }
      if(l_chi2   ) { chi2   [Track_i] = l_chi2    ->GetValue(Track_i); } else { chi2   [Track_i] = -9999.; }
      if(l_lastGap) { lastGap[Track_i] = l_lastGap ->GetValue(Track_i); } else { lastGap[Track_i] = -9999; }
      if(l_eta    ) { eta    [Track_i] = l_eta     ->GetValue(Track_i); } else { eta    [Track_i] = -9999.; }
      if(l_phi    ) { phi    [Track_i] = l_phi     ->GetValue(Track_i); } else { phi    [Track_i] = -9999.; }
      if(l_DG0    ) { DG0    [Track_i] = l_DG0     ->GetValue(Track_i); } else { DG0    [Track_i] = -9999.; }
      if(l_DDG0   ) { DDG0   [Track_i] = l_DDG0    ->GetValue(Track_i); } else { DDG0   [Track_i] = -9999.; }
      if(l_xSt1   ) { xSt1   [Track_i] = l_xSt1    ->GetValue(Track_i); } else { xSt1   [Track_i] = -9999.; }
      if(l_ySt1   ) { ySt1   [Track_i] = l_ySt1    ->GetValue(Track_i); } else { ySt1   [Track_i] = -9999.; }
      if(l_xSt2   ) { xSt2   [Track_i] = l_xSt2    ->GetValue(Track_i); } else { xSt2   [Track_i] = -9999.; }
      if(l_ySt2   ) { ySt2   [Track_i] = l_ySt2    ->GetValue(Track_i); } else { ySt2   [Track_i] = -9999.; }
      if(l_xSt3   ) { xSt3   [Track_i] = l_xSt3    ->GetValue(Track_i); } else { xSt3   [Track_i] = -9999.; }
      if(l_ySt3   ) { ySt3   [Track_i] = l_ySt3    ->GetValue(Track_i); } else { ySt3   [Track_i] = -9999.; }
      if(l_DG4    ) { DG4    [Track_i] = l_DG4     ->GetValue(Track_i); } else { DG4    [Track_i] = -9999.; }
      if(l_DCA_r  ) { DCA_r  [Track_i] = l_DCA_r   ->GetValue(Track_i); } else { DCA_r  [Track_i] = -9999.; }
      if(l_DCA_z  ) { DCA_z  [Track_i] = l_DCA_z   ->GetValue(Track_i); } else { DCA_z  [Track_i] = -9999.; }
      if(l_ELoss       ) { ELoss       [Track_i] = l_ELoss       ->GetValue(Track_i); } else { ELoss       [Track_i] = -9999.; }
      if(l_mutr_nhits  ) { mutr_nhits  [Track_i] = l_mutr_nhits  ->GetValue(Track_i); } else { mutr_nhits  [Track_i] = -9999.; }
      if(l_muid_nhits  ) { muid_nhits  [Track_i] = l_muid_nhits  ->GetValue(Track_i); } else { muid_nhits  [Track_i] = -9999.; }
      if(l_ref_vtx_rdca) { ref_vtx_rdca[Track_i] = l_ref_vtx_rdca->GetValue(Track_i); } else { ref_vtx_rdca[Track_i] = -9999.; }
      if(l_ref_vtx_r   ) { ref_vtx_r   [Track_i] = l_ref_vtx_r   ->GetValue(Track_i); } else { ref_vtx_r   [Track_i] = -9999.; }
      if(l_ref_vtx_z   ) { ref_vtx_z   [Track_i] = l_ref_vtx_z   ->GetValue(Track_i); } else { ref_vtx_z   [Track_i] = -9999.; }
      if(l_refit_zvtx  ) { refit_zvtx  [Track_i] = l_refit_zvtx  ->GetValue(Track_i); } else { refit_zvtx  [Track_i] = -9999.; }
      if(l_idhits      ) { idhits      [Track_i] = l_idhits      ->GetValue(Track_i); } else { idhits      [Track_i] = -9999.; }
      if(l_idquad      ) { idquad      [Track_i] = l_idquad      ->GetValue(Track_i); } else { idquad      [Track_i] = -9999.; }
      if(l_trhits      ) { trhits      [Track_i] = l_trhits      ->GetValue(Track_i); } else { trhits      [Track_i] = -9999.; }

      // b_FvtxMatch	
      if(l_fvtx_dphi   ) { fvtx_dphi   [Track_i] = l_fvtx_dphi   ->GetValue(Track_i); } else { fvtx_dphi   [Track_i] = -9999.; }
      if(l_fvtx_dtheta ) { fvtx_dtheta [Track_i] = l_fvtx_dtheta ->GetValue(Track_i); } else { fvtx_dtheta [Track_i] = -9999.; }
      if(l_fvtx_dr     ) { fvtx_dr     [Track_i] = l_fvtx_dr     ->GetValue(Track_i); } else { fvtx_dr     [Track_i] = -9999.; }
      if(l_fvtx_conebits     ) { fvtx_conebits     [Track_i] = l_fvtx_conebits->GetValue(Track_i); } else { fvtx_conebits     [Track_i] = 9999; }
      if(l_fvtx_tracklconebits     ) { fvtx_tracklconebits     [Track_i] = l_fvtx_tracklconebits->GetValue(Track_i); } else { fvtx_tracklconebits     [Track_i] = 9999; }

      // Calculate fvtx_cone
      // wp: determine whether or not to fill for what kind of fvtx condition
      fvtx_cone[Track_i] = 0;
      fvtx_tracklet[Track_i] = 0;
      for(UInt_t test_bit = 2; test_bit < 7; test_bit++)
      {
        fvtx_cone[Track_i] += static_cast<UInt_t>( (fvtx_conebits[Track_i] >> test_bit*4)&0xf );
        fvtx_tracklet[Track_i] += static_cast<UInt_t>( (fvtx_tracklconebits[Track_i] >> test_bit*4)&0xf );
        //coneBitsCheck << "Run:" << Run_Number << "\t" << "Event:" << Evt_Number << "\t"
        //	<< "Track:" << Track_i << "\t" << "momentum:" << p[Track_i] << "\t"
        //	<< "fvtx_dr:" << fvtx_dr[Track_i] << "\t" << "fvtx_dphi:" << fvtx_dphi[Track_i] << "\t"
        //	<< "fvtx_dtheta:" << fvtx_dtheta[Track_i] << "\t" << "test_bit:" << test_bit << "\t"
        //	<< "fvtx_cone:" << fvtx_cone[Track_i] << "\t" << "fvtx_conebits:" << fvtx_conebits[Track_i] << std::endl;
      }

      // b_RpcMatchSt1
      if( l_RpcMatchSt1_Rpc3dca ) { RpcMatchSt1_Rpc3dca [Track_i] = l_RpcMatchSt1_Rpc3dca  ->GetValue(Track_i); } else { RpcMatchSt1_Rpc3dca [Track_i] = -9999; }
      if( l_RpcMatchSt1_Rpc3time) { RpcMatchSt1_Rpc3time[Track_i] = l_RpcMatchSt1_Rpc3time ->GetValue(Track_i); } else { RpcMatchSt1_Rpc3time[Track_i] = -9999; }
      if( l_RpcMatchSt1_Rpc3x   ) { RpcMatchSt1_Rpc3x   [Track_i] = l_RpcMatchSt1_Rpc3x    ->GetValue(Track_i); } else { RpcMatchSt1_Rpc3x   [Track_i] = -9999; }
      if( l_RpcMatchSt1_Rpc3y   ) { RpcMatchSt1_Rpc3y   [Track_i] = l_RpcMatchSt1_Rpc3y    ->GetValue(Track_i); } else { RpcMatchSt1_Rpc3y   [Track_i] = -9999; }
      if( l_RpcMatchSt1_Rpc1dca ) { RpcMatchSt1_Rpc1dca [Track_i] = l_RpcMatchSt1_Rpc1dca  ->GetValue(Track_i); } else { RpcMatchSt1_Rpc1dca [Track_i] = -9999; }
      if( l_RpcMatchSt1_Rpc1time) { RpcMatchSt1_Rpc1time[Track_i] = l_RpcMatchSt1_Rpc1time ->GetValue(Track_i); } else { RpcMatchSt1_Rpc1time[Track_i] = -9999; }
      if( l_RpcMatchSt1_Rpc1x   ) { RpcMatchSt1_Rpc1x   [Track_i] = l_RpcMatchSt1_Rpc1x    ->GetValue(Track_i); } else { RpcMatchSt1_Rpc1x   [Track_i] = -9999; }
      if( l_RpcMatchSt1_Rpc1y   ) { RpcMatchSt1_Rpc1y   [Track_i] = l_RpcMatchSt1_Rpc1y    ->GetValue(Track_i); } else { RpcMatchSt1_Rpc1y   [Track_i] = -9999; }

      // b_RpcMatchSt3
      if( l_RpcMatchSt3_Rpc3dca ) { RpcMatchSt3_Rpc3dca [Track_i] = l_RpcMatchSt3_Rpc3dca  ->GetValue(Track_i); } else { RpcMatchSt3_Rpc3dca [Track_i] = -9999; }
      if( l_RpcMatchSt3_Rpc3time) { RpcMatchSt3_Rpc3time[Track_i] = l_RpcMatchSt3_Rpc3time ->GetValue(Track_i); } else { RpcMatchSt3_Rpc3time[Track_i] = -9999; }
      if( l_RpcMatchSt3_Rpc3x   ) { RpcMatchSt3_Rpc3x   [Track_i] = l_RpcMatchSt3_Rpc3x    ->GetValue(Track_i); } else { RpcMatchSt3_Rpc3x   [Track_i] = -9999; }
      if( l_RpcMatchSt3_Rpc3y   ) { RpcMatchSt3_Rpc3y   [Track_i] = l_RpcMatchSt3_Rpc3y    ->GetValue(Track_i); } else { RpcMatchSt3_Rpc3y   [Track_i] = -9999; }
      if( l_RpcMatchSt3_Rpc1dca ) { RpcMatchSt3_Rpc1dca [Track_i] = l_RpcMatchSt3_Rpc1dca  ->GetValue(Track_i); } else { RpcMatchSt3_Rpc1dca [Track_i] = -9999; }
      if( l_RpcMatchSt3_Rpc1time) { RpcMatchSt3_Rpc1time[Track_i] = l_RpcMatchSt3_Rpc1time ->GetValue(Track_i); } else { RpcMatchSt3_Rpc1time[Track_i] = -9999; }
      if( l_RpcMatchSt3_Rpc1x   ) { RpcMatchSt3_Rpc1x   [Track_i] = l_RpcMatchSt3_Rpc1x    ->GetValue(Track_i); } else { RpcMatchSt3_Rpc1x   [Track_i] = -9999; }
      if( l_RpcMatchSt3_Rpc1y   ) { RpcMatchSt3_Rpc1y   [Track_i] = l_RpcMatchSt3_Rpc1y    ->GetValue(Track_i); } else { RpcMatchSt3_Rpc1y   [Track_i] = -9999; }

    }
  }
  else if (_RecoTracks == 0)
  {
    ResetContainers();
  }
  else
  {
    std::cout << "_RecoTracks for event " << i << " is not positive or zero!" << " It is: " << _RecoTracks << std::endl;
    std::cout << "THIS IS A MAJOR PROBLEM" << std::endl;
  }
  return 0;
}

int SingleMuons::Show(Long64_t entry)
{
  if(!RunTree)
  {
    std::cerr << "The tree is not defined" << std::endl;
    return -1;
  }
  GetEvent(entry);
  std::cout << "Entry: " << entry << std::endl;
  std::cout << "Run_Number   : " << Run_Number   << std::endl;
  std::cout << "Evt_Number   : " << Evt_Number   << std::endl;
  std::cout << "Evt_bbcZ     : " << Evt_bbcZ     << std::endl;
  std::cout << "Evt_Z     : " << Evt_Z     << std::endl;
  std::cout << "Evt_zdcEnerN     : " << Evt_zdcEnerN     << std::endl;
  std::cout << "triggerbit   : " << triggerbit   << std::endl;
  std::cout << "triggerlive  : " << triggerlive  << std::endl;
  std::cout << "clockcross   : " << clockcross   << std::endl;
  std::cout << "_RpcHits     : " << _RpcHits     << std::endl;
  std::cout << "_RecoTracks  : " << _RecoTracks  << std::endl;

  if(_RecoTracks > 0)
  {
    std::cout << "    Evt_Nmu { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  Evt_Nmu[Track_i] << " }" << std::endl; } else { std::cout << Evt_Nmu[Track_i] << ", ";} }
    std::cout << "    charge  { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  charge [Track_i] << " }" << std::endl; } else { std::cout << charge [Track_i] << ", ";} }
    std::cout << "    px      { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  px     [Track_i] << " }" << std::endl; } else { std::cout << px     [Track_i] << ", ";} }
    std::cout << "    py      { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  py     [Track_i] << " }" << std::endl; } else { std::cout << py     [Track_i] << ", ";} }
    std::cout << "    pz      { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  pz     [Track_i] << " }" << std::endl; } else { std::cout << pz     [Track_i] << ", ";} }
    std::cout << "    pxSt1      { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  pxSt1     [Track_i] << " }" << std::endl; } else { std::cout << pxSt1     [Track_i] << ", ";} }
    std::cout << "    pySt1      { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  pySt1     [Track_i] << " }" << std::endl; } else { std::cout << pySt1     [Track_i] << ", ";} }
    std::cout << "    pzSt1      { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  pzSt1     [Track_i] << " }" << std::endl; } else { std::cout << pzSt1     [Track_i] << ", ";} }
    std::cout << "    pxSt2      { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  pxSt2     [Track_i] << " }" << std::endl; } else { std::cout << pxSt2     [Track_i] << ", ";} }
    std::cout << "    pySt2      { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  pySt2     [Track_i] << " }" << std::endl; } else { std::cout << pySt2     [Track_i] << ", ";} }
    std::cout << "    pzSt2      { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  pzSt2     [Track_i] << " }" << std::endl; } else { std::cout << pzSt2     [Track_i] << ", ";} }
    std::cout << "    pxSt3      { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  pxSt3     [Track_i] << " }" << std::endl; } else { std::cout << pxSt3     [Track_i] << ", ";} }
    std::cout << "    pySt3      { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  pySt3     [Track_i] << " }" << std::endl; } else { std::cout << pySt3     [Track_i] << ", ";} }
    std::cout << "    pzSt3      { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  pzSt3     [Track_i] << " }" << std::endl; } else { std::cout << pzSt3     [Track_i] << ", ";} }
    std::cout << "    pT      { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  pT     [Track_i] << " }" << std::endl; } else { std::cout << pT     [Track_i] << ", ";} }
    std::cout << "    p       { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  p      [Track_i] << " }" << std::endl; } else { std::cout << p      [Track_i] << ", ";} }
    std::cout << "    chi2    { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  chi2   [Track_i] << " }" << std::endl; } else { std::cout << chi2   [Track_i] << ", ";} }
    std::cout << "    lastGap { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  lastGap[Track_i] << " }" << std::endl; } else { std::cout << lastGap[Track_i] << ", ";} }
    std::cout << "    eta     { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  eta    [Track_i] << " }" << std::endl; } else { std::cout << eta    [Track_i] << ", ";} }
    std::cout << "    phi     { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  phi    [Track_i] << " }" << std::endl; } else { std::cout << phi    [Track_i] << ", ";} }
    std::cout << "    DG0     { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  DG0    [Track_i] << " }" << std::endl; } else { std::cout << DG0    [Track_i] << ", ";} }
    std::cout << "    DDG0    { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  DDG0   [Track_i] << " }" << std::endl; } else { std::cout << DDG0   [Track_i] << ", ";} }
    std::cout << "    xSt1    { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  xSt1   [Track_i] << " }" << std::endl; } else { std::cout << xSt1   [Track_i] << ", ";} }
    std::cout << "    ySt1    { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  ySt1   [Track_i] << " }" << std::endl; } else { std::cout << ySt1   [Track_i] << ", ";} }
    std::cout << "    xSt2    { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  xSt2   [Track_i] << " }" << std::endl; } else { std::cout << xSt2   [Track_i] << ", ";} }
    std::cout << "    ySt2    { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  ySt2   [Track_i] << " }" << std::endl; } else { std::cout << ySt2   [Track_i] << ", ";} }
    std::cout << "    xSt3    { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  xSt3   [Track_i] << " }" << std::endl; } else { std::cout << xSt3   [Track_i] << ", ";} }
    std::cout << "    ySt3    { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  ySt3   [Track_i] << " }" << std::endl; } else { std::cout << ySt3   [Track_i] << ", ";} }
    std::cout << "    DG4     { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  DG4    [Track_i] << " }" << std::endl; } else { std::cout << DG4    [Track_i] << ", ";} }
    std::cout << "    DCA_r   { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  DCA_r  [Track_i] << " }" << std::endl; } else { std::cout << DCA_r  [Track_i] << ", ";} }
    std::cout << "    DCA_z   { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  DCA_z  [Track_i] << " }" << std::endl; } else { std::cout << DCA_z  [Track_i] << ", ";} }
    std::cout << "    idhits   { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  idhits  [Track_i] << " }" << std::endl; } else { std::cout << idhits  [Track_i] << ", ";} }
    std::cout << "    idquad   { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  idquad  [Track_i] << " }" << std::endl; } else { std::cout << idquad  [Track_i] << ", ";} }
    std::cout << "    trhits   { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  trhits  [Track_i] << " }" << std::endl; } else { std::cout << trhits  [Track_i] << ", ";} }
    std::cout << "    ELoss   { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  ELoss  [Track_i] << " }" << std::endl; } else { std::cout << ELoss  [Track_i] << ", ";} }
    std::cout << "    mutr_nhits   { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  mutr_nhits  [Track_i] << " }" << std::endl; } else { std::cout << mutr_nhits  [Track_i] << ", ";} }
    std::cout << "    muid_nhits   { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  muid_nhits  [Track_i] << " }" << std::endl; } else { std::cout << muid_nhits  [Track_i] << ", ";} }
    std::cout << "    ref_vtx_rdca   { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  ref_vtx_rdca  [Track_i] << " }" << std::endl; } else { std::cout << ref_vtx_rdca  [Track_i] << ", ";} }
    std::cout << "    ref_vtx_r   { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  ref_vtx_r  [Track_i] << " }" << std::endl; } else { std::cout << ref_vtx_r  [Track_i] << ", ";} }
    std::cout << "    ref_vtx_z   { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  ref_vtx_z  [Track_i] << " }" << std::endl; } else { std::cout << ref_vtx_z  [Track_i] << ", ";} }
    std::cout << "    refit_zvtx   { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  refit_zvtx  [Track_i] << " }" << std::endl; } else { std::cout << refit_zvtx  [Track_i] << ", ";} }

    // b_FvtxMatch
    std::cout << "    fvtx_dtheta     { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  fvtx_dtheta    [Track_i] << " }" << std::endl; } else { std::cout << fvtx_dtheta    [Track_i] << ", ";} }
    std::cout << "    fvtx_dphi     { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  fvtx_dphi    [Track_i] << " }" << std::endl; } else { std::cout << fvtx_dphi    [Track_i] << ", ";} }
    std::cout << "    fvtx_dr     { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  fvtx_dr    [Track_i] << " }" << std::endl; } else { std::cout << fvtx_dr    [Track_i] << ", ";} }
    std::cout << "    fvtx_conebits     { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  fvtx_conebits    [Track_i] << " }" << std::endl; } else { std::cout << fvtx_conebits    [Track_i] << ", ";} }
    std::cout << "    fvtx_tracklconebits     { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  fvtx_tracklconebits    [Track_i] << " }" << std::endl; } else { std::cout << fvtx_tracklconebits    [Track_i] << ", ";} }
    std::cout << "    fvtx_cone     { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  fvtx_cone    [Track_i] << " }" << std::endl; } else { std::cout << fvtx_cone    [Track_i] << ", ";} }
    std::cout << "    fvtx_tracklet     { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  fvtx_tracklet    [Track_i] << " }" << std::endl; } else { std::cout << fvtx_tracklet    [Track_i] << ", ";} }

    // b_RpcMatchSt1
    std::cout << "RpcMatchSt1  : " << std::endl; 
    std::cout << "    RpcMatchSt1_Rpc3dca  { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  RpcMatchSt1_Rpc3dca [Track_i] << " }" << std::endl; } else { std::cout << RpcMatchSt1_Rpc3dca [Track_i] << ", ";} }
    std::cout << "    RpcMatchSt1_Rpc3time { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  RpcMatchSt1_Rpc3time[Track_i] << " }" << std::endl; } else { std::cout << RpcMatchSt1_Rpc3time[Track_i] << ", ";} }
    std::cout << "    RpcMatchSt1_Rpc3x    { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  RpcMatchSt1_Rpc3x   [Track_i] << " }" << std::endl; } else { std::cout << RpcMatchSt1_Rpc3x   [Track_i] << ", ";} }
    std::cout << "    RpcMatchSt1_Rpc3y    { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  RpcMatchSt1_Rpc3y   [Track_i] << " }" << std::endl; } else { std::cout << RpcMatchSt1_Rpc3y   [Track_i] << ", ";} }
    std::cout << "    RpcMatchSt1_Rpc1dca  { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  RpcMatchSt1_Rpc1dca [Track_i] << " }" << std::endl; } else { std::cout << RpcMatchSt1_Rpc1dca [Track_i] << ", ";} }
    std::cout << "    RpcMatchSt1_Rpc1time { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  RpcMatchSt1_Rpc1time[Track_i] << " }" << std::endl; } else { std::cout << RpcMatchSt1_Rpc1time[Track_i] << ", ";} }
    std::cout << "    RpcMatchSt1_Rpc1x    { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  RpcMatchSt1_Rpc1x   [Track_i] << " }" << std::endl; } else { std::cout << RpcMatchSt1_Rpc1x   [Track_i] << ", ";} }
    std::cout << "    RpcMatchSt1_Rpc1y    { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  RpcMatchSt1_Rpc1y   [Track_i] << " }" << std::endl; } else { std::cout << RpcMatchSt1_Rpc1y   [Track_i] << ", ";} }

    // b_RpcMatchSt3
    std::cout << "RpcMatchSt3  : " << std::endl; 
    std::cout << "    RpcMatchSt3_Rpc3dca  { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  RpcMatchSt3_Rpc3dca [Track_i] << " }" << std::endl; } else { std::cout << RpcMatchSt3_Rpc3dca [Track_i] << ", ";} }
    std::cout << "    RpcMatchSt3_Rpc3time { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  RpcMatchSt3_Rpc3time[Track_i] << " }" << std::endl; } else { std::cout << RpcMatchSt3_Rpc3time[Track_i] << ", ";} }
    std::cout << "    RpcMatchSt3_Rpc3x    { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  RpcMatchSt3_Rpc3x   [Track_i] << " }" << std::endl; } else { std::cout << RpcMatchSt3_Rpc3x   [Track_i] << ", ";} }
    std::cout << "    RpcMatchSt3_Rpc3y    { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  RpcMatchSt3_Rpc3y   [Track_i] << " }" << std::endl; } else { std::cout << RpcMatchSt3_Rpc3y   [Track_i] << ", ";} }
    std::cout << "    RpcMatchSt3_Rpc1dca  { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  RpcMatchSt3_Rpc1dca [Track_i] << " }" << std::endl; } else { std::cout << RpcMatchSt3_Rpc1dca [Track_i] << ", ";} }
    std::cout << "    RpcMatchSt3_Rpc1time { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  RpcMatchSt3_Rpc1time[Track_i] << " }" << std::endl; } else { std::cout << RpcMatchSt3_Rpc1time[Track_i] << ", ";} }
    std::cout << "    RpcMatchSt3_Rpc1x    { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  RpcMatchSt3_Rpc1x   [Track_i] << " }" << std::endl; } else { std::cout << RpcMatchSt3_Rpc1x   [Track_i] << ", ";} }
    std::cout << "    RpcMatchSt3_Rpc1y    { "; for(unsigned int Track_i = 0; Track_i < static_cast<unsigned int>(_RecoTracks); Track_i++) { if(Track_i == static_cast<unsigned int>(_RecoTracks)-1) { std::cout <<  RpcMatchSt3_Rpc1y   [Track_i] << " }" << std::endl; } else { std::cout << RpcMatchSt3_Rpc1y   [Track_i] << ", ";} }

  }

  return 0;
}

int SingleMuons::Loop()
{
  Long64_t nentries = RunTree->GetEntries();

  for( Long64_t i = 0; i < nentries; i++)
  {
    GetEvent(i);
    // Do analysis
  }
  return 0;
}
int SingleMuons::ResetContainers()
{
  _RecoTracks                 = -1;
  _RpcHits                    = -1;

  /***EVENT DATA***/
  Run_Number                  = -1;
  Evt_Number                  = -1;
  Evt_bbcZ                    = -1; 
  Evt_Z                       = -1;
  Evt_zdcEnerN                = -1;
  triggerbit                  = -1;
  triggerlive                 = -1;
  clockcross                  = -1;
  /***EVENT DATA***/

  /***RECO TRACKS***/
  Evt_Nmu               .resize(0);
  charge                .resize(0);
  px                    .resize(0);
  py                    .resize(0);
  pz                    .resize(0);
  pxSt1                 .resize(0);
  pySt1                 .resize(0);
  pzSt1                 .resize(0);
  pxSt2                 .resize(0);
  pySt2                 .resize(0);
  pzSt2                 .resize(0);
  pxSt3                 .resize(0);
  pySt3                 .resize(0);
  pzSt3                 .resize(0);
  pT                    .resize(0);
  p                     .resize(0);
  chi2                  .resize(0);
  lastGap               .resize(0);
  eta                   .resize(0);
  phi                   .resize(0);
  DG0                   .resize(0);
  DDG0                  .resize(0);
  xSt1                  .resize(0);
  ySt1                  .resize(0);
  xSt2                  .resize(0);
  ySt2                  .resize(0);
  xSt3                  .resize(0);
  ySt3                  .resize(0);
  ELoss                 .resize(0);
  idhits                .resize(0);
  idquad                .resize(0);
  trhits                .resize(0);
  mutr_nhits            .resize(0);
  muid_nhits            .resize(0);
  ref_vtx_rdca          .resize(0);
  ref_vtx_r             .resize(0);
  ref_vtx_z             .resize(0);
  refit_zvtx            .resize(0);
  DG4                   .resize(0);
  DCA_r                 .resize(0);
  DCA_z                 .resize(0);
  /***RECO TRACKS***/

  /***FVTX MATCH***/
  fvtx_dtheta           .resize(0);
  fvtx_dphi             .resize(0);
  fvtx_dr               .resize(0);
  fvtx_conebits         .resize(0);
  fvtx_tracklconebits   .resize(0);
  fvtx_cone             .resize(0);
  fvtx_tracklet         .resize(0);
  /***FVTX MATCH***/

  /***RPC MATCH ST1***/
  RpcMatchSt1_Rpc3dca   .resize(0);
  RpcMatchSt1_Rpc3time  .resize(0);
  RpcMatchSt1_Rpc3x     .resize(0);
  RpcMatchSt1_Rpc3y     .resize(0);
  RpcMatchSt1_Rpc1dca   .resize(0);
  RpcMatchSt1_Rpc1time  .resize(0);
  RpcMatchSt1_Rpc1x     .resize(0);
  RpcMatchSt1_Rpc1y     .resize(0);
  /***RPC MATCH ST1***/

  /***RPC MATCH ST3***/
  RpcMatchSt3_Rpc3dca   .resize(0);
  RpcMatchSt3_Rpc3time  .resize(0);
  RpcMatchSt3_Rpc3x     .resize(0);
  RpcMatchSt3_Rpc3y     .resize(0);
  RpcMatchSt3_Rpc1dca   .resize(0);
  RpcMatchSt3_Rpc1time  .resize(0);
  RpcMatchSt3_Rpc1x     .resize(0);
  RpcMatchSt3_Rpc1y     .resize(0);
  /***RPC MATCH ST3***/

  return 0;
}
int SingleMuons::ResetPointers()
{
  RunTree        = NULL;

  /**RECO TRACKS SIZE***/
  b__RecoTracks  = NULL;
  l__RecoTracks  = NULL;
  /**RECO TRACKS SIZE***/


  /**RPC HITS SIZE***/
  b__RpcHits     = NULL;
  l__RpcHits     = NULL;
  /**RPC HITS SIZE***/

  /***EVENT DATA***/
  b_Eventdata    = NULL;
  l_Run_Number   = NULL;
  l_Evt_Number   = NULL;
  l_Evt_bbcZ     = NULL;
  l_Evt_Z		   = NULL;
  l_Evt_zdcEnerN = NULL;
  l_triggerbit   = NULL;
  l_triggerlive  = NULL;
  l_clockcross   = NULL;
  /***EVENT DATA***/

  /***RECO TRACKS***/
  b_RecoTracks   = NULL;
  l_Evt_Nmu      = NULL;
  l_charge       = NULL;
  l_px           = NULL;
  l_py           = NULL;
  l_pz           = NULL;
  l_pxSt1        = NULL;
  l_pySt1        = NULL;
  l_pzSt1        = NULL;
  l_pxSt2        = NULL;
  l_pySt2        = NULL;
  l_pzSt2        = NULL;
  l_pxSt3        = NULL;
  l_pySt3        = NULL;
  l_pzSt3        = NULL;
  l_pT           = NULL;
  l_p            = NULL;
  l_chi2         = NULL;
  l_lastGap      = NULL;
  l_eta          = NULL;
  l_phi          = NULL;
  l_DG0          = NULL;
  l_DDG0         = NULL;
  l_xSt1         = NULL;
  l_ySt1         = NULL;
  l_xSt2         = NULL;
  l_ySt2         = NULL;
  l_xSt3         = NULL;
  l_ySt3         = NULL;
  l_ELoss        = NULL;
  l_idhits       = NULL;
  l_idquad       = NULL;
  l_trhits       = NULL;
  l_mutr_nhits   = NULL;
  l_muid_nhits   = NULL;
  l_ref_vtx_rdca = NULL;
  l_ref_vtx_r    = NULL;
  l_ref_vtx_z    = NULL;
  l_refit_zvtx   = NULL;
  l_DG4          = NULL;
  l_DCA_r        = NULL;
  l_DCA_z        = NULL;
  /***RECO TRACKS***/

  /***FVTX MATCH***/
  b_FvtxMatch    = NULL;
  l_fvtx_dtheta  = NULL;
  l_fvtx_dphi    = NULL;
  l_fvtx_dr      = NULL;
  l_fvtx_conebits= NULL;
  l_fvtx_tracklconebits = NULL;
  /***FVTX MATCH***/

  /***RPC MATCH ST1***/
  b_RpcMatchSt1           = NULL;
  l_RpcMatchSt1_Rpc3dca   = NULL;
  l_RpcMatchSt1_Rpc3time  = NULL;
  l_RpcMatchSt1_Rpc3x     = NULL;
  l_RpcMatchSt1_Rpc3y     = NULL;
  l_RpcMatchSt1_Rpc1dca   = NULL;
  l_RpcMatchSt1_Rpc1time  = NULL;
  l_RpcMatchSt1_Rpc1x     = NULL;
  l_RpcMatchSt1_Rpc1y     = NULL;
  /***RPC MATCH ST1***/

  /***RPC MATCH ST3***/
  b_RpcMatchSt3           = NULL;
  l_RpcMatchSt3_Rpc3dca   = NULL;
  l_RpcMatchSt3_Rpc3time  = NULL;
  l_RpcMatchSt3_Rpc3x     = NULL;
  l_RpcMatchSt3_Rpc3y     = NULL;
  l_RpcMatchSt3_Rpc1dca   = NULL;
  l_RpcMatchSt3_Rpc1time  = NULL;
  l_RpcMatchSt3_Rpc1x     = NULL;
  l_RpcMatchSt3_Rpc1y     = NULL;
  /***RPC MATCH ST3***/
  return 0;
}
int SingleMuons::Reset()
{
  ResetPointers();
  ResetContainers();
  return 0;
}

int SingleMuons::Initialize(TTree* tree)
{
  //coneBitsCheck.open("/direct/phenix+spin2/beaumim/WAnalysisRun13/AnalysisCode/condor/coneBitCheck.txt");
  Reset();

  RunTree = tree;
  InitBranches(RunTree);
  InitLeaves();

  return 0;
}

// checks state of internal variables to determine if fvtx is "good" or "bad"
bool SingleMuons::FVTXGood(int Track_i)
{
  bool FVTX_GOOD = 0;
  if( _RecoTracks > 0 )
  {
    if( (fvtx_dr[Track_i] > 0 ) && (fvtx_dr[Track_i] < 100) && 
        (fvtx_dphi[Track_i] < 1.5 ) && (fvtx_dphi[Track_i] > 0) && 
        (fvtx_dtheta[Track_i] < 1.5) && (fvtx_dtheta[Track_i] > 0) 
      ) 
    {
      FVTX_GOOD = 1;
    }
  }
  return FVTX_GOOD;
}
