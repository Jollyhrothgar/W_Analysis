#include "BasicCutTreeVariables.h"

BasicCutTreeVariables::BasicCutTreeVariables()
{
	Reset();
}

int BasicCutTreeVariables::Reset()
{
	Run_Number  = -9999; 
	Evt_Number  = -9999; 
	Evt_bbcZ    = -9999; 
	clockcross  = -9999; 
	triggerbit  = 0;
	Wness       = -9999.; 
	charge      = -9999.; 
	pT          = -9999.; 
	pz          = -9999.; 
	phi         = -9999.; 
	eta         = -9999.; 
	DG0         = -9999.; 
	DDG0        = -9999.; 
	DG4         = -9999.;
	chi2        = -9999.; 
	DCA_z       = -9999.; 
	DCA_r       = -9999.; 
	phiSt1      = -9999.; 
	phiSt2      = -9999.; 
	phiSt3      = -9999.; 
	theta       = -9999.; 
	dphi13      = -9999.; 
	dphi23      = -9999.; 
	dw13        = -9999.; 
	dw23        = -9999.; 
	Rpc1dca     = -9999.; 
	Rpc3dca     = -9999.; 
	fvtx_dtheta = -9999.;
	fvtx_dphi   = -9999.;
	fvtx_dr     = -9999.;
	fvtx_cone     = 0;
	fvtx_tracklet = 0;
	return 0;
}

int BasicCutTreeVariables::LinkTree(TTree* thisTree, int mode)
{
	switch (mode)
	{
		case READ:
			thisTree->SetBranchAddress("Run_Number"   , &(this->Run_Number  )); 
			thisTree->SetBranchAddress("Evt_Number"   , &(this->Evt_Number  )); 
			thisTree->SetBranchAddress("Evt_bbcZ"     , &(this->Evt_bbcZ    )); 
			thisTree->SetBranchAddress("clockcross"   , &(this->clockcross  ));
			thisTree->SetBranchAddress("triggerbit"   , &(this->triggerbit  ));
			thisTree->SetBranchAddress("Wness"        , &(this->Wness       )); 
			thisTree->SetBranchAddress("charge"       , &(this->charge      )); 
			thisTree->SetBranchAddress("pT"           , &(this->pT          )); 
			thisTree->SetBranchAddress("pz"           , &(this->pz          )); 
			thisTree->SetBranchAddress("phi"          , &(this->phi         )); 
			thisTree->SetBranchAddress("eta"          , &(this->eta         )); 
			thisTree->SetBranchAddress("DG0"          , &(this->DG0         )); 
			thisTree->SetBranchAddress("DDG0"         , &(this->DDG0        )); 
			thisTree->SetBranchAddress("DG4"          , &(this->DG4         ));
			thisTree->SetBranchAddress("chi2"         , &(this->chi2        )); 
			thisTree->SetBranchAddress("DCA_z"        , &(this->DCA_z       )); 
			thisTree->SetBranchAddress("DCA_r"        , &(this->DCA_r       )); 
			thisTree->SetBranchAddress("phiSt1"       , &(this->phiSt1      ));
			thisTree->SetBranchAddress("phiSt2"       , &(this->phiSt2      ));
			thisTree->SetBranchAddress("phiSt3"       , &(this->phiSt3      ));
			thisTree->SetBranchAddress("theta"        , &(this->theta       ));
			thisTree->SetBranchAddress("dphi13"       , &(this->dphi13      ));
			thisTree->SetBranchAddress("dphi23"       , &(this->dphi23      ));
			thisTree->SetBranchAddress("dw13"         , &(this->dw13        )); 
			thisTree->SetBranchAddress("dw23"         , &(this->dw23        )); 
			thisTree->SetBranchAddress("Rpc1dca"      , &(this->Rpc1dca     )); 
			thisTree->SetBranchAddress("Rpc3dca"      , &(this->Rpc3dca     )); 
			thisTree->SetBranchAddress("fvtx_dtheta"  , &(this->fvtx_dtheta )); 
			thisTree->SetBranchAddress("fvtx_dphi"    , &(this->fvtx_dphi   )); 
			thisTree->SetBranchAddress("fvtx_dr"      , &(this->fvtx_dr     )); 
			thisTree->SetBranchAddress("fvtx_cone"    , &(this->fvtx_cone   ));
			thisTree->SetBranchAddress("fvtx_tracklet", &(this->fvtx_tracklet));
			break;
		case WRITE:
			thisTree->Branch("Run_Number"   , &(this->Run_Number      ), "Run_Number/I" ); 
			thisTree->Branch("Evt_Number"   , &(this->Evt_Number      ), "Evt_Number/I" ); 
			thisTree->Branch("Evt_bbcZ"     , &(this->Evt_bbcZ        ), "Evt_bbcZ/F" ); 
			thisTree->Branch("clockcross"   , &(this->clockcross      ), "clockcross/I" );
			thisTree->Branch("triggerbit"   , &(this->triggerbit      ), "triggerbit/I" );
			thisTree->Branch("Wness"        , &(this->Wness           ), "Wness/F" ); 
			thisTree->Branch("charge"       , &(this->charge          ), "charge/F" ); 
			thisTree->Branch("pT"           , &(this->pT              ), "pT/F" ); 
			thisTree->Branch("pz"           , &(this->pz              ), "pz/F" ); 
			thisTree->Branch("phi"          , &(this->phi             ), "phi/F" ); 
			thisTree->Branch("eta"          , &(this->eta             ), "eta/F" ); 
			thisTree->Branch("DG0"          , &(this->DG0             ), "DG0/F" ); 
			thisTree->Branch("DDG0"         , &(this->DDG0            ), "DDG0/F" ); 
			thisTree->Branch("DG4"          , &(this->DG4             ), "DG4/F" );
			thisTree->Branch("chi2"         , &(this->chi2            ), "chi2/F" ); 
			thisTree->Branch("DCA_z"        , &(this->DCA_z           ), "DCA_z/F" ); 
			thisTree->Branch("DCA_r"        , &(this->DCA_r           ), "DCA_r/F" ); 
			thisTree->Branch("phiSt1"       , &(this->phiSt1          ), "phiSt1/F" );
			thisTree->Branch("phiSt2"       , &(this->phiSt2          ), "phiSt2/F" );
			thisTree->Branch("phiSt3"       , &(this->phiSt3          ), "phiSt3/F" );
			thisTree->Branch("theta"        , &(this->theta           ), "theta/F" );
			thisTree->Branch("dphi13"       , &(this->dphi13          ), "dphi13/F" );
			thisTree->Branch("dphi23"       , &(this->dphi23          ), "dphi23/F" );
			thisTree->Branch("dw13"         , &(this->dw13            ), "dw13/F" ); 
			thisTree->Branch("dw23"         , &(this->dw23            ), "dw23/F" ); 
			thisTree->Branch("Rpc1dca"      , &(this->Rpc1dca         ), "Rpc1dca/F" ); 
			thisTree->Branch("Rpc3dca"      , &(this->Rpc3dca         ), "Rpc3dca/F" ); 
			thisTree->Branch("fvtx_dtheta"  , &(this->fvtx_dtheta     ), "fvtx_dtheta/F" ); 
			thisTree->Branch("fvtx_dphi"    , &(this->fvtx_dphi       ), "fvtx_dphi/F" ); 
			thisTree->Branch("fvtx_dr"      , &(this->fvtx_dr         ), "fvtx_dr/F" ); 
			thisTree->Branch("fvtx_cone"    , &(this->fvtx_cone       ), "fvtx_cone/i" );
			thisTree->Branch("fvtx_tracklet", &(this->fvtx_tracklet   ), "fvtx_tracklet/i" );
			break;
	}
	return 0;
}
