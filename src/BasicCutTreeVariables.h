#ifndef _BASIC_CUT_TREE_VARIABLES_H_
#define _BASIC_CUT_TREE_VARIABLES_H_

#include "SingleMuonsConsts.h"
#include "TTree.h"

struct BasicCutTreeVariables
{
	BasicCutTreeVariables();
	int   Run_Number  ; 
	int   Evt_Number  ; 
	float Evt_bbcZ    ; 
	int   clockcross  ; 
	int   triggerbit  ;
	float Wness       ; 
	float charge      ; 
	float pT          ; 
	float pz          ; 
	float phi         ; 
	float eta         ; 
	float DG0         ; 
	float DDG0        ; 
	float DG4         ;
	float chi2        ; 
	float DCA_z       ; 
	float DCA_r       ; 
	float phiSt1      ; 
	float phiSt2      ; 
	float phiSt3      ; 
	float theta       ; 
	float dphi13      ; 
	float dphi23      ; 
	float dw13        ; 
	float dw23        ; 
	float Rpc1dca     ; 
	float Rpc3dca     ; 
	float fvtx_dtheta ;
	float fvtx_dphi   ;
	float fvtx_dr     ;
    UInt_t fvtx_cone   ;
    UInt_t fvtx_tracklet;
	
	int Reset();

	int LinkTree(TTree*, int);
  private:
	std::map<std::string, std::vector<std::string> > varMap;
};
#endif
