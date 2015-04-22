#ifndef SingleMuons_h
#define SingleMuons_h

// STL
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

// ROOT
#include <TTree.h>
#include <TROOT.h>
#include <TFile.h>
#include <TChain.h>
#include <TFile.h>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TBranch.h>
#include <TLeaf.h>

class SingleMuons
{
 public:
  // use to dump values
  //ofstream coneBitsCheck;

  // Member Functions
  // Initialize Right Away
  SingleMuons(TTree*);

  // Initialize Later
  SingleMuons();
  ~SingleMuons();

  // Prints out all members of a current entry. Lists are of form { entry1, entry2, ..., entryN }  
  int Show(Long64_t);

  // Sets all internal members to an entry in the tree.
  int GetEvent(Long64_t);	

  // Loops over all events in the tree, settings all branches and leaves for each event.
  int Loop();

  // Calls BranchInit and LeafInit on a new TTree. 
  int Initialize(TTree*);

  // Member Data

  // The Tree to be parsed
 private:
  TTree* RunTree;
  TBranch *b__RecoTracks;  // [ _RecoTracks  ]  Stores number of tracks in MuTr (mapped to _RecoTracks) per event 
  TLeaf* l__RecoTracks  ;
 public: 
  int _RecoTracks       ;  // [ _RecoTracks  ] Mapped to b__RecoTracks. There are _RecoTracks MuTr tracks for each event
 
 private:
  TBranch *b__RpcHits;  // [ _RpcHits     ]  Stores number of rpc hits per event (mapped to _RpcHits)
  TLeaf* l__RpcHits  ;  
 public: 
  int _RpcHits       ;  // [ _RpcHits     ] Mapped to b__RpcHits. There are _RpcHits RPC hits per event.

  // Tree Data (with mappings in comment)

  // b_EventData branch, static variables per event
 private:
  TBranch *b_Eventdata;    // [ Eventdata    ]  Stores global, trigger, and min bias data as single leaves per event
  TLeaf* l_Run_Number  ; // [ Run_Number  ]
  TLeaf* l_Evt_Number  ; // [ Evt_Number  ]
  TLeaf* l_Evt_Z       ; // [ Evt_Z       ]
  TLeaf* l_Evt_bbcZ    ; // [ Evt_bbcZ    ]
  TLeaf* l_Evt_zdcEnerN; // [ Evt_zdcEnerN]
  TLeaf* l_triggerbit  ; // [ triggerbit  ]
  TLeaf* l_triggerlive ; // [ triggerlive ]
  TLeaf* l_clockcross  ; // [ clockcross  ]
 public:
  int   Run_Number  ; // [ Run_Number  ]
  int   Evt_Number  ; // [ Evt_Number  ]
  float Evt_Z       ; // [ Evt_Z       ] 
  float Evt_bbcZ    ; // [ Evt_bbcZ    ]
  float Evt_zdcEnerN; // [ Evt_zdcEnerN] 
  int   triggerbit  ; // [ triggerbit  ]
  int   triggerlive ; // [ triggerlive ]
  int   clockcross  ; // [ clockcross ]

  // b_RecoTracks - contains dynamic arrays of size _RecoTracks
 private:
  TBranch *b_RecoTracks;   // [ RecoTracks   ]  Stores arrays[_RpcTracks] (indexed by track) of tracking variables, per event
  // each leaf is used to fill the vector
  TLeaf* l_Evt_Nmu                 ; // [ Evt_Nmu ]
  TLeaf* l_charge                  ; // [ charge  ]
  TLeaf* l_px                      ; // [ px      ]
  TLeaf* l_py                      ; // [ py      ]
  TLeaf* l_pz                      ; // [ pz      ]
  TLeaf* l_pxSt1                   ; // [ pxSt1   ]
  TLeaf* l_pySt1                   ; // [ pySt1   ]
  TLeaf* l_pzSt1                   ; // [ pzSt1   ]
  TLeaf* l_pxSt2                   ; // [ pxSt2   ]
  TLeaf* l_pySt2                   ; // [ pySt2   ]
  TLeaf* l_pzSt2                   ; // [ pzSt2   ]
  TLeaf* l_pxSt3                   ; // [ pxSt3   ]
  TLeaf* l_pySt3                   ; // [ pySt3   ]
  TLeaf* l_pzSt3                   ; // [ pzSt3   ]
  TLeaf* l_pT                      ; // [ pT      ]
  TLeaf* l_p                       ; // [ p       ]
  TLeaf* l_chi2                    ; // [ chi2    ]
  TLeaf* l_lastGap                 ; // [ lastGap ]
  TLeaf* l_eta                     ; // [ eta     ]
  TLeaf* l_phi                     ; // [ phi     ]
  TLeaf* l_DG0                     ; // [ DG0     ]
  TLeaf* l_DDG0                    ; // [ DDG0    ]
  TLeaf* l_xSt1                    ; // [ xSt1    ]
  TLeaf* l_ySt1                    ; // [ ySt1    ]
  TLeaf* l_xSt2                    ; // [ xSt2    ]
  TLeaf* l_ySt2                    ; // [ ySt2    ]
  TLeaf* l_xSt3                    ; // [ xSt3    ]
  TLeaf* l_ySt3                    ; // [ ySt3    ]
  TLeaf* l_ELoss                   ; // [ ELoss   ]      
  TLeaf* l_idhits                  ; // [ idhits  ]      
  TLeaf* l_idquad                  ; // [ idquad  ]      
  TLeaf* l_trhits                  ; // [ trhits   ]    
  TLeaf* l_mutr_nhits              ; // [ mutr_nhits   ] 
  TLeaf* l_muid_nhits              ; // [ muid_nhits   ] 
  TLeaf* l_ref_vtx_rdca            ; // [ ref_vtx_rdca ] 
  TLeaf* l_ref_vtx_r               ; // [ ref_vtx_r    ] 
  TLeaf* l_ref_vtx_z               ; // [ ref_vtx_z    ] 
  TLeaf* l_refit_zvtx              ; // [ refit_zvtx   ] 
  TLeaf* l_DG4                     ; // [ DG4     ]
  TLeaf* l_DCA_r                   ; // [ DCA_r   ]
  TLeaf* l_DCA_z                   ; // [ DCA_z   ]
 public:
  std::vector<int>   Evt_Nmu       ; // [ Evt_Nmu ]
  std::vector<float> charge        ; // [ charge  ]
  std::vector<float> px            ; // [ px      ]
  std::vector<float> py            ; // [ py      ]
  std::vector<float> pz            ; // [ pz      ]
  std::vector<float> pxSt1         ; // [ pxSt1   ] 
  std::vector<float> pySt1         ; // [ pySt1   ] 
  std::vector<float> pzSt1         ; // [ pzSt1   ] 
  std::vector<float> pxSt2         ; // [ pxSt2   ] 
  std::vector<float> pySt2         ; // [ pySt2   ] 
  std::vector<float> pzSt2         ; // [ pzSt2   ] 
  std::vector<float> pxSt3         ; // [ pxSt3   ] 
  std::vector<float> pySt3         ; // [ pySt3   ] 
  std::vector<float> pzSt3         ; // [ pzSt3   ] 
  std::vector<float> pT            ; // [ pT      ]
  std::vector<float> p             ; // [ p       ]
  std::vector<float> chi2          ; // [ chi2    ]
  std::vector<int>   lastGap       ; // [ lastGap ]
  std::vector<float> eta           ; // [ eta     ]
  std::vector<float> phi           ; // [ phi     ]
  std::vector<float> DG0           ; // [ DG0     ]
  std::vector<float> DDG0          ; // [ DDG0    ]
  std::vector<float> xSt1          ; // [ xSt1    ] 
  std::vector<float> ySt1          ; // [ ySt1    ] 
  std::vector<float> xSt2          ; // [ xSt2    ] 
  std::vector<float> ySt2          ; // [ ySt2    ] 
  std::vector<float> xSt3          ; // [ xSt3    ] 
  std::vector<float> ySt3          ; // [ ySt3    ] 
  std::vector<float> ELoss         ; // [ ELoss   ] 
  std::vector<float> idhits        ; // [ idhits  ]
  std::vector<float> idquad        ; // [ idquad  ]
  std::vector<float> trhits         ; // [ trhits   ]
  std::vector<float> mutr_nhits    ; // [ mutr_nhits   ]
  std::vector<float> muid_nhits    ; // [ muid_nhits   ]
  std::vector<float> ref_vtx_rdca  ; // [ ref_vtx_rdca ]
  std::vector<float> ref_vtx_r     ; // [ ref_vtx_r    ]
  std::vector<float> ref_vtx_z     ; // [ ref_vtx_z    ]
  std::vector<float> refit_zvtx    ; // [ refit_zvtx   ]
  std::vector<float> DG4           ; // [ DG4     ]
  std::vector<float> DCA_r         ; // [ DCA_r   ]
  std::vector<float> DCA_z         ; // [ DCA_z   ]
  // End Reco Tracks Branch

  // b_FvtxMatch - contains dynamic arrays of size _RecoTracks
 private:
  TBranch *b_FvtxMatch             ; // [ FvtxMatch ]      	
  TLeaf* l_fvtx_dphi               ;
  TLeaf* l_fvtx_dtheta             ;
  TLeaf* l_fvtx_dr                 ; 
  TLeaf* l_fvtx_conebits           ;
  TLeaf* l_fvtx_tracklconebits     ;
 public:
  std::vector<float>  fvtx_dphi     ; // [ fvtx_dphi   ]
  std::vector<float>  fvtx_dtheta   ; // [ fvtx_dtheta ]
  std::vector<float>  fvtx_dr       ; // [ fvtx_dr ]
  std::vector<UInt_t> fvtx_conebits ; // [ fvtx_conebits ] calculate fvtx_cone
  std::vector<UInt_t> fvtx_tracklconebits; // [ fvtx_tracklconebits ] calculate fvtx_tracklet wp
  std::vector<UInt_t> fvtx_cone     ; // calculated
  std::vector<UInt_t> fvtx_tracklet ; // calculated - add along with fvtx_cone stuff (search with /l_fvtx_cone\(b\)\@!\|fvtx_cone\s*\(b\)\@ ) 

  ////////////////////////////////
  //  RPC Track Matching Branches
  //  * Leaves all have same name in each branch
  //    inside root - this is okay, because they 
  //    live on separate leaves, but we have to
  //    give them unique names in this class

  // b_RpcMatchSt1 - contains dynamic arrays of size _RecoTracks per event
 private:
  TBranch *b_RpcMatchSt1;  // [ RpcMatchSt1  ]  Stores arrays[_RpcTracks] of Rpc Station 1 track matching variables per event
  TLeaf* l_RpcMatchSt1_Rpc3dca   ; // [ Rpc3dca  ]
  TLeaf* l_RpcMatchSt1_Rpc3time  ; // [ Rpc3time ]
  TLeaf* l_RpcMatchSt1_Rpc3x     ; // [ Rpc3x    ]
  TLeaf* l_RpcMatchSt1_Rpc3y     ; // [ Rpc3y    ]
  TLeaf* l_RpcMatchSt1_Rpc1dca   ; // [ Rpc1dca  ]
  TLeaf* l_RpcMatchSt1_Rpc1time  ; // [ Rpc1time ]
  TLeaf* l_RpcMatchSt1_Rpc1x     ; // [ Rpc1x    ]
  TLeaf* l_RpcMatchSt1_Rpc1y     ; // [ Rpc1y    ]
 public:
  std::vector<float> RpcMatchSt1_Rpc3dca   ; // [ Rpc3dca  ]
  std::vector<float> RpcMatchSt1_Rpc3time  ; // [ Rpc3time ]
  std::vector<float> RpcMatchSt1_Rpc3x     ; // [ Rpc3x    ] 
  std::vector<float> RpcMatchSt1_Rpc3y     ; // [ Rpc3y    ] 
  std::vector<float> RpcMatchSt1_Rpc1dca   ; // [ Rpc1dca  ]
  std::vector<float> RpcMatchSt1_Rpc1time  ; // [ Rpc1time ]
  std::vector<float> RpcMatchSt1_Rpc1x     ; // [ Rpc1x    ] 
  std::vector<float> RpcMatchSt1_Rpc1y     ; // [ Rpc1y    ] 

  // End b_RpcMatchSt1

  // b_RpcMatchSt3 - contains dynamic arrays of size _RecoTracks per event
 private:
  TBranch *b_RpcMatchSt3;  // [ RpcMatchSt3  ]  Stores arrays[_RpcTracks] of Rpc Station 3 track matching variables per event
  TLeaf* l_RpcMatchSt3_Rpc3dca  ; // [ Rpc3dca  ]   
  TLeaf* l_RpcMatchSt3_Rpc3time ; // [ Rpc3time ]
  TLeaf* l_RpcMatchSt3_Rpc3x    ; // [ Rpc3x    ]  
  TLeaf* l_RpcMatchSt3_Rpc3y    ; // [ Rpc3y    ]  
  TLeaf* l_RpcMatchSt3_Rpc1dca  ; // [ Rpc1dca  ]
  TLeaf* l_RpcMatchSt3_Rpc1time ; // [ Rpc1time ]
  TLeaf* l_RpcMatchSt3_Rpc1x    ; // [ Rpc1x    ]  
  TLeaf* l_RpcMatchSt3_Rpc1y    ; // [ Rpc1y    ]  
 public:
  std::vector<float> RpcMatchSt3_Rpc3dca  ; // [ Rpc3dca  ]   
  std::vector<float> RpcMatchSt3_Rpc3time ; // [ Rpc3time ]
  std::vector<float> RpcMatchSt3_Rpc3x    ; // [ Rpc3x    ]
  std::vector<float> RpcMatchSt3_Rpc3y    ; // [ Rpc3y    ]
  std::vector<float> RpcMatchSt3_Rpc1dca  ; // [ Rpc1dca  ]
  std::vector<float> RpcMatchSt3_Rpc1time ; // [ Rpc1time ]
  std::vector<float> RpcMatchSt3_Rpc1x    ; // [ Rpc1x    ]
  std::vector<float> RpcMatchSt3_Rpc1y    ; // [ Rpc1y    ]
  // end b_RpcMatchSt3

  //
  // End Rpc3 Matching Branches
  /////////////////////////////////

 private:
  // Sets pointers to TTree contents for branches. Called before InitLeaves.
  int InitBranches(TTree*);

  // Sets pointers to TTree contents for leaves. Called after InitBranches.
  int InitLeaves();

  // Resizes all dynamic members to zero, and sets all pointers to NULL value
  // Ownership of all pointer values is assumed to be handled outside the class.
  int Reset(); // Calls reset containers and reset pointers
  int ResetContainers(); // resets all containers, resizes all vectors
  int ResetPointers();   // sets all pointers to NULL 
  //(no memory is dynamically allocated with this class- 'new' is never called)

  // Checks if fvtx condition is "good" - uses code copy/pasted from HelperFunctions
  // But does not explicity include the header to keep this class free of local dependancies
  // in case we want to put it in a different library later.
  bool FVTXGood( int Track_i );
};

#endif
