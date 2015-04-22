#include "Plotter.h"

Plotter::Plotter()
{
	std::cout << "Plotter Constructor" << std::endl;
}

Plotter::Plotter(std::string sigFile, std::string bakFile, std::string tree_name, std::string outArea)
{
	signalFile = sigFile;
	backgroundFile = bakFile;
	treeName = tree_name;
	plotsDirectory = outArea;
	std::cout << "Using " << treeName << " from: " << std::endl
		<< "\tSignal File    : " << signalFile << std::endl
		<< "\tBackground File: " << backgroundFile << std::endl
		<< "\tPlots Sent to  : " << outArea << std::endl;
  PLOT_VARS = NUMBER_OF_VARS - 1; // Get rid of fvtx_cone for now
}

Plotter::~Plotter()
{
	std::cout << "Plotter::~Plotter" << std::endl;
}

int Plotter::SetFigureOutputArea( std::string outArea )
{
	plotsDirectory = outArea;
	return 0;
}

int Plotter::Init()
{
	GrowVectors();
	InitTrees();
	InitHistograms();
	return 0;
}

int Plotter::Loop()
{
	for(int data_i = 0; data_i < NUMBER_OF_DATA_STREAMS; data_i++)
	{
		std::cout << "Running over data_" << data_i << std::endl;
		Long64_t sig_entries = plotTree[data_i]->GetEntries();
		for(Long64_t i = 0; i < sig_entries; i++)
		{
			plotTree[data_i]->GetEntry(i);
			int arm_i    = GetArmIndex   (pz[data_i]);
			int charge_i = GetChargeIndex(charge[data_i]);
			for(int var_i = 0; var_i < PLOT_VARS; var_i++)
			{
				vsDG0          [var_i][arm_i][charge_i][data_i]->Fill( DG0[data_i], GetVar(var_i,data_i));
				vsDDG0         [var_i][arm_i][charge_i][data_i]->Fill( DDG0[data_i],  GetVar(var_i,data_i));
				vsRpc3Dca      [var_i][arm_i][charge_i][data_i]->Fill(Rpc3dca[data_i], GetVar(var_i,data_i));
				vsRpc1Dca      [var_i][arm_i][charge_i][data_i]->Fill(Rpc1dca[data_i], GetVar(var_i,data_i));
				vsFvtxDrDtheta [var_i][arm_i][charge_i][data_i]->Fill(GetVar(FVTX_DRxDTHETA_INDEX,data_i), GetVar(var_i,data_i));
				vsFvtxDphi     [var_i][arm_i][charge_i][data_i]->Fill(fvtx_dphi[data_i], GetVar(var_i,data_i));
				vsChi2         [var_i][arm_i][charge_i][data_i]->Fill(chi2[data_i], GetVar(var_i,data_i));
				vsDCA_r        [var_i][arm_i][charge_i][data_i]->Fill(DCA_r[data_i], GetVar(var_i,data_i));
			}
		}
	}
	std::cout << "^_^" << std::endl;
	return 0;
}

float Plotter::GetVar(int var_i, int data_i)
{
	float val = -9999.9;
	if(data_i < 0 || data_i > PLOT_VARS) return val;
	
	switch(var_i)
	{
		 
		case DG0_INDEX:
			val = DG0[data_i]; 
			break;
		case DDG0_INDEX:
			val = DDG0[data_i];
			break;
		case CHI2_INDEX:
			val = chi2[data_i];
			break;
		case DCA_R_INDEX:
			val = DCA_r[data_i];
			break;
		case RPC1DCA_INDEX:
			val = Rpc1dca[data_i];
			break;
		case RPC3DCA_INDEX:
			val = Rpc3dca[data_i];
			break;
		case FVTX_DRxDTHETA_INDEX:
			val = fabs(fvtx_dtheta[data_i]*fvtx_dphi[data_i]);
			break;
		case FVTX_DPHI_INDEX:
			val = fabs(fvtx_dphi[data_i]);
			break;
		default:
			std::cout << "VAR INDEX: " << var_i << " IS UNKNOWN!!!" << std::endl;
	}
	return val;
}

int Plotter::GrowVectors()
{
	vsDG0          .resize(PLOT_VARS);
	vsDDG0         .resize(PLOT_VARS);
	vsRpc3Dca      .resize(PLOT_VARS);
	vsRpc1Dca      .resize(PLOT_VARS);
	vsDCA_r        .resize(PLOT_VARS);
	vsFvtxDrDtheta .resize(PLOT_VARS);
	vsFvtxDphi     .resize(PLOT_VARS);
	vsChi2         .resize(PLOT_VARS);
	for(int var_i = 0; var_i < PLOT_VARS; var_i++)
	{
		vsDG0          [var_i].resize(NUMBER_OF_ARM_CONFIGS);
		vsDDG0         [var_i].resize(NUMBER_OF_ARM_CONFIGS);
		vsRpc3Dca      [var_i].resize(NUMBER_OF_ARM_CONFIGS);
		vsRpc1Dca      [var_i].resize(NUMBER_OF_ARM_CONFIGS);
		vsDCA_r        [var_i].resize(NUMBER_OF_ARM_CONFIGS);
		vsFvtxDrDtheta [var_i].resize(NUMBER_OF_ARM_CONFIGS);
		vsFvtxDphi     [var_i].resize(NUMBER_OF_ARM_CONFIGS);
		vsChi2         [var_i].resize(NUMBER_OF_ARM_CONFIGS);
		for(int arm_i = 0; arm_i < NUMBER_OF_ARM_CONFIGS; arm_i++)
		{
			vsDG0          [var_i][arm_i].resize(NUMBER_OF_CHARGE_CONFIGS);
			vsDDG0         [var_i][arm_i].resize(NUMBER_OF_CHARGE_CONFIGS);
			vsRpc3Dca      [var_i][arm_i].resize(NUMBER_OF_CHARGE_CONFIGS);
			vsRpc1Dca      [var_i][arm_i].resize(NUMBER_OF_CHARGE_CONFIGS);
			vsDCA_r        [var_i][arm_i].resize(NUMBER_OF_CHARGE_CONFIGS);
			vsFvtxDrDtheta [var_i][arm_i].resize(NUMBER_OF_CHARGE_CONFIGS);
			vsFvtxDphi     [var_i][arm_i].resize(NUMBER_OF_CHARGE_CONFIGS);
			vsChi2         [var_i][arm_i].resize(NUMBER_OF_CHARGE_CONFIGS);
			for(int charge_i = 0; charge_i < NUMBER_OF_DATA_STREAMS; charge_i++)
			{
				vsDG0          [var_i][arm_i][charge_i].resize(NUMBER_OF_DATA_STREAMS);
				vsDDG0         [var_i][arm_i][charge_i].resize(NUMBER_OF_DATA_STREAMS);
				vsRpc3Dca      [var_i][arm_i][charge_i].resize(NUMBER_OF_DATA_STREAMS);
				vsRpc1Dca      [var_i][arm_i][charge_i].resize(NUMBER_OF_DATA_STREAMS);
				vsDCA_r        [var_i][arm_i][charge_i].resize(NUMBER_OF_DATA_STREAMS);
				vsFvtxDrDtheta [var_i][arm_i][charge_i].resize(NUMBER_OF_DATA_STREAMS);
				vsFvtxDphi     [var_i][arm_i][charge_i].resize(NUMBER_OF_DATA_STREAMS);
				vsChi2         [var_i][arm_i][charge_i].resize(NUMBER_OF_DATA_STREAMS);
			}
		}
	}
	treeFile.resize(NUMBER_OF_DATA_STREAMS);
	plotTree.resize(NUMBER_OF_DATA_STREAMS);
	return 0;
}

int Plotter::RegisterObject( TObject* obj )
{
	ObjectRegistry.push_back(obj);
	return 0;
}

int Plotter::InitTrees()
{
	treeFile[SIGNAL_INDEX] = new TFile(signalFile    .c_str(), "READ");
	treeFile[BACKGROUND_INDEX] = new TFile(backgroundFile.c_str(), "READ");
	for(int i = 0; i < NUMBER_OF_DATA_STREAMS; i++)
	{
		plotTree[i] = (TTree*)treeFile[i]->Get(treeName.c_str());
		plotTree[i] = (TTree*)treeFile[i]->Get(treeName.c_str());
		plotTree[i]->SetBranchAddress("Run_Number", &Run_Number  [i]);
		plotTree[i]->SetBranchAddress("Evt_Number", &Evt_Number  [i]);
		plotTree[i]->SetBranchAddress("Evt_bbcZ", &Evt_bbcZ    [i]);
		plotTree[i]->SetBranchAddress("clockcross", &clockcross  [i]);
		plotTree[i]->SetBranchAddress("Wness", &Wness       [i]);
		plotTree[i]->SetBranchAddress("charge", &charge      [i]);
		plotTree[i]->SetBranchAddress("pT", &pT          [i]);
		plotTree[i]->SetBranchAddress("pz", &pz          [i]);
		plotTree[i]->SetBranchAddress("phi", &phi         [i]);
		plotTree[i]->SetBranchAddress("eta", &eta         [i]);
		plotTree[i]->SetBranchAddress("DG0", &DG0         [i]);
		plotTree[i]->SetBranchAddress("DDG0", &DDG0        [i]);
		plotTree[i]->SetBranchAddress("chi2", &chi2        [i]);
		plotTree[i]->SetBranchAddress("DCA_z", &DCA_z       [i]);
		plotTree[i]->SetBranchAddress("DCA_r", &DCA_r       [i]);
		plotTree[i]->SetBranchAddress("phiSt1", &phiSt1      [i]);
		plotTree[i]->SetBranchAddress("phiSt2", &phiSt2      [i]);
		plotTree[i]->SetBranchAddress("phiSt3", &phiSt3      [i]);
		plotTree[i]->SetBranchAddress("theta", &theta       [i]);
		//plotTree[i]->SetBranchAddress("dphi23", &dphi23      [i]);
		//plotTree[i]->SetBranchAddress("dphi13", &dphi13      [i]);
		plotTree[i]->SetBranchAddress("dw13", &dw13        [i]);
		plotTree[i]->SetBranchAddress("dw23", &dw23        [i]);
		plotTree[i]->SetBranchAddress("Rpc1dca", &Rpc1dca     [i]);
		plotTree[i]->SetBranchAddress("Rpc3dca", &Rpc3dca     [i]);
		plotTree[i]->SetBranchAddress("fvtx_dtheta", &fvtx_dtheta [i]);
		plotTree[i]->SetBranchAddress("fvtx_dphi", &fvtx_dphi   [i]);
		plotTree[i]->SetBranchAddress("fvtx_dr", &fvtx_dr     [i]);
		//plotTree[i]->SetBranchAddress("fvtx_cone", &fvtx_cone   [i]);
	}

	std::cout << "Loaded trees." << std::endl
		<< "\tSignal Entries    : " << plotTree[SIGNAL_INDEX]->GetEntries() << std::endl
		<< "\tBackground Entries: " << plotTree[BACKGROUND_INDEX]->GetEntries() << std::endl;

	return 0;
}

int Plotter::DrawPlots()
{
	varVsDG0 = new TCanvas("varVsDG0","varVsDG0",1400,4000);
	varVsDG0->Divide(4,14);
	int canvas_index = 1;
	for(int var_i = 0; var_i < PLOT_VARS; var_i++)
	{
		for(int data_i = 0; data_i < NUMBER_OF_DATA_STREAMS; data_i++)
		{
			for(int arm_i = 0; arm_i < NUMBER_OF_ARM_CONFIGS; arm_i++)
			{
				for(int charge_i = 0; charge_i < NUMBER_OF_CHARGE_CONFIGS; charge_i++)
				{
					if(var_i == DG0_INDEX) continue;
					varVsDG0->cd(canvas_index);
					vsDG0[var_i][arm_i][charge_i][data_i]->Draw("cont1");
					canvas_index++;
				}
			}
		}
	}
	std::string outFile = varVsDG0->GetName();
	outFile = plotsDirectory+outFile +".png";
	varVsDG0->SaveAs(outFile.c_str());
  delete varVsDG0;

	varVsDDG0 = new TCanvas("varVsDDG0","varVsDDG0",1400,3600);
	varVsDDG0->Divide(4,12);
	canvas_index = 1;
	for(int var_i = 0; var_i < PLOT_VARS; var_i++)
	{
		for(int data_i = 0; data_i < NUMBER_OF_DATA_STREAMS; data_i++)
		{
			for(int arm_i = 0; arm_i < NUMBER_OF_ARM_CONFIGS; arm_i++)
			{
				for(int charge_i = 0; charge_i < NUMBER_OF_CHARGE_CONFIGS; charge_i++)
				{
					if( var_i == DG0_INDEX || var_i == DDG0_INDEX ) continue;
					varVsDDG0->cd(canvas_index);
					vsDDG0[var_i][arm_i][charge_i][data_i]->Draw("cont1");
					canvas_index++;
				}
			}
		}
	}
	outFile = varVsDDG0->GetName();
	outFile = plotsDirectory+outFile +".png";
	varVsDDG0->SaveAs(outFile.c_str());
  delete varVsDDG0;

	varVsRpc3Dca = new TCanvas("varVsRpc3Dca","varVsRpc3Dca",1400,3400);
	varVsRpc3Dca->Divide(4,10);
	canvas_index = 1;
	for(int var_i = 0; var_i < PLOT_VARS; var_i++)
	{
		for(int data_i = 0; data_i < NUMBER_OF_DATA_STREAMS; data_i++)
		{
			for(int arm_i = 0; arm_i < NUMBER_OF_ARM_CONFIGS; arm_i++)
			{
				for(int charge_i = 0; charge_i < NUMBER_OF_CHARGE_CONFIGS; charge_i++)
				{
					if( var_i == DG0_INDEX || var_i == DDG0_INDEX || var_i == RPC3DCA_INDEX ) continue;
					varVsRpc3Dca->cd(canvas_index);
					vsRpc3Dca[var_i][arm_i][charge_i][data_i]->Draw("cont1");
					canvas_index++;
				}
			}
		}
	}
	outFile = varVsRpc3Dca->GetName();
	outFile = plotsDirectory+outFile +".png";
	varVsRpc3Dca->SaveAs(outFile.c_str());
  delete varVsRpc3Dca;
	
	varVsRpc1Dca = new TCanvas("varVsRpc1Dca","varVsRpc1Dca",1400,3200);
	varVsRpc1Dca->Divide(4,8);
	canvas_index = 1;
	for(int var_i = 0; var_i < PLOT_VARS; var_i++)
	{
		for(int data_i = 0; data_i < NUMBER_OF_DATA_STREAMS; data_i++)
		{
			for(int arm_i = 0; arm_i < NUMBER_OF_ARM_CONFIGS; arm_i++)
			{
				for(int charge_i = 0; charge_i < NUMBER_OF_CHARGE_CONFIGS; charge_i++)
				{
					if( var_i == DG0_INDEX || var_i == DDG0_INDEX || var_i == RPC3DCA_INDEX || var_i == RPC1DCA_INDEX ) continue;
					varVsRpc1Dca->cd(canvas_index);
					vsRpc1Dca[var_i][arm_i][charge_i][data_i]->Draw("cont1");
					canvas_index++;
				}
			}
		}
	}
	outFile = varVsRpc1Dca->GetName();
	outFile = plotsDirectory+outFile +".png";
	varVsRpc1Dca->SaveAs(outFile.c_str());
  delete varVsRpc1Dca;
	
	varVsFvtxDphi = new TCanvas("varVsFvtxDphi","varVsFvtxDphi",1200,1800);
	varVsFvtxDphi->Divide(4,6);
	canvas_index = 1;
	for(int var_i = 0; var_i < PLOT_VARS; var_i++)
	{
		for(int data_i = 0; data_i < NUMBER_OF_DATA_STREAMS; data_i++)
		{
			for(int arm_i = 0; arm_i < NUMBER_OF_ARM_CONFIGS; arm_i++)
			{
				for(int charge_i = 0; charge_i < NUMBER_OF_CHARGE_CONFIGS; charge_i++)
				{
					if( var_i == DG0_INDEX || var_i == DDG0_INDEX || var_i == RPC3DCA_INDEX || var_i == RPC1DCA_INDEX || var_i == FVTX_DPHI_INDEX) continue;
					varVsFvtxDphi->cd(canvas_index);
					vsFvtxDphi[var_i][arm_i][charge_i][data_i]->Draw("cont1");
					canvas_index++;
				}
			}
		}
	}
	outFile = varVsFvtxDphi->GetName();
	outFile = plotsDirectory+outFile +".png";
	varVsFvtxDphi->SaveAs(outFile.c_str());
  delete varVsFvtxDphi;
	
	varVsChi2 = new TCanvas("varVsChi2","varVsChi2",1200,1200);
	varVsChi2->Divide(4,4);
	canvas_index = 1;
	for(int var_i = 0; var_i < PLOT_VARS; var_i++)
	{
		for(int data_i = 0; data_i < NUMBER_OF_DATA_STREAMS; data_i++)
		{
			for(int arm_i = 0; arm_i < NUMBER_OF_ARM_CONFIGS; arm_i++)
			{
				for(int charge_i = 0; charge_i < NUMBER_OF_CHARGE_CONFIGS; charge_i++)
				{
					if( var_i == DG0_INDEX || var_i == DDG0_INDEX || var_i == RPC3DCA_INDEX || var_i == RPC1DCA_INDEX || var_i == FVTX_DPHI_INDEX || var_i == CHI2_INDEX) continue;
					varVsChi2->cd(canvas_index);
					vsChi2[var_i][arm_i][charge_i][data_i]->Draw("cont1");
					canvas_index++;
				}
			}
		}
	}
	outFile = varVsChi2->GetName();
	outFile = plotsDirectory+outFile +".png";
	varVsChi2->SaveAs(outFile.c_str());
  delete varVsChi2;
	return 0;
}

int Plotter::InitHistograms()
{	
	for(int arm_i = 0; arm_i < NUMBER_OF_ARM_CONFIGS; arm_i++)
	{
		for(int charge_i = 0; charge_i < NUMBER_OF_CHARGE_CONFIGS; charge_i++)
		{
			for(int data_i = 0; data_i < NUMBER_OF_DATA_STREAMS; data_i++)
			{
				for(int var_i = 0; var_i < PLOT_VARS; var_i++)
				{
					std::string name_stub = "";
					std::string title_stub = "";
					MakeNameTitleStubPair(arm_i,charge_i,data_i, name_stub, title_stub);

					std::string name_vsDG0          = GetVarName(var_i) + "_vs_" + GetVarName(DG0_INDEX           )+"_"+name_stub;
					std::string name_vsDDG0         = GetVarName(var_i) + "_vs_" + GetVarName(DDG0_INDEX          )+"_"+name_stub;
					std::string name_vsRpc3Dca      = GetVarName(var_i) + "_vs_" + GetVarName(RPC3DCA_INDEX       )+"_"+name_stub;
					std::string name_vsRpc1Dca      = GetVarName(var_i) + "_vs_" + GetVarName(RPC1DCA_INDEX       )+"_"+name_stub;
					std::string name_vsFvtxDrDtheta = GetVarName(var_i) + "_vs_" + GetVarName(FVTX_DRxDTHETA_INDEX)+"_"+name_stub;
					std::string name_vsFvtxDphi     = GetVarName(var_i) + "_vs_" + GetVarName(FVTX_DPHI_INDEX     )+"_"+name_stub;
					std::string name_vsChi2         = GetVarName(var_i) + "_vs_" + GetVarName(CHI2_INDEX          )+"_"+name_stub;
					std::string name_vsDCA_r        = GetVarName(var_i) + "_vs_" + GetVarName(DCA_R_INDEX         )+"_"+name_stub;

					std::string var_name = GetVarName(var_i);
					std::string var_0    = GetVarName(DG0_INDEX            ); 
					std::string var_1    = GetVarName(DDG0_INDEX           ); 
					std::string var_2    = GetVarName(RPC3DCA_INDEX        ); 
					std::string var_3    = GetVarName(RPC1DCA_INDEX        ); 
					std::string var_4    = GetVarName(FVTX_DRxDTHETA_INDEX ); 
					std::string var_5    = GetVarName(FVTX_DPHI_INDEX      ); 
					std::string var_6    = GetVarName(CHI2_INDEX           ); 
					std::string var_7    = GetVarName(DCA_R_INDEX          ); 
					
					std::string title_vsDG0          = title_stub + " " + var_name + " vs " + var_0 + ";" + var_0 + ";" + var_name;
					std::string title_vsDDG0         = title_stub + " " + var_name + " vs " + var_1 + ";" + var_1 + ";" + var_name;
					std::string title_vsRpc3Dca      = title_stub + " " + var_name + " vs " + var_2 + ";" + var_2 + ";" + var_name;
					std::string title_vsRpc1Dca      = title_stub + " " + var_name + " vs " + var_3 + ";" + var_3 + ";" + var_name;
					std::string title_vsFvtxDrDtheta = title_stub + " " + var_name + " vs " + var_4 + ";" + var_4 + ";" + var_name;
					std::string title_vsFvtxDphi     = title_stub + " " + var_name + " vs " + var_5 + ";" + var_5 + ";" + var_name;
					std::string title_vsChi2         = title_stub + " " + var_name + " vs " + var_6 + ";" + var_6 + ";" + var_name;
					std::string title_vsDCA_r        = title_stub + " " + var_name + " vs " + var_7 + ";" + var_7 + ";" + var_name;
						
					vsDG0          [var_i][arm_i][charge_i][data_i] = new TH2F(name_vsDG0         .c_str(), title_vsDG0          .c_str(), 100, 0.0, GetVarMax(DG0_INDEX           ), 100, 0.0, GetVarMax(var_i));
					vsDDG0         [var_i][arm_i][charge_i][data_i] = new TH2F(name_vsDDG0        .c_str(), title_vsDDG0         .c_str(), 100, 0.0, GetVarMax(DDG0_INDEX          ), 100, 0.0, GetVarMax(var_i));
					vsRpc3Dca      [var_i][arm_i][charge_i][data_i] = new TH2F(name_vsRpc3Dca     .c_str(), title_vsRpc3Dca      .c_str(), 100, 0.0, GetVarMax(RPC3DCA_INDEX       ), 100, 0.0, GetVarMax(var_i));
					vsRpc1Dca      [var_i][arm_i][charge_i][data_i] = new TH2F(name_vsRpc1Dca     .c_str(), title_vsRpc1Dca      .c_str(), 100, 0.0, GetVarMax(RPC1DCA_INDEX       ), 100, 0.0, GetVarMax(var_i));
					vsFvtxDrDtheta [var_i][arm_i][charge_i][data_i] = new TH2F(name_vsFvtxDrDtheta.c_str(), title_vsFvtxDrDtheta .c_str(), 100, 0.0, GetVarMax(FVTX_DRxDTHETA_INDEX), 100, 0.0, GetVarMax(var_i));
					vsFvtxDphi     [var_i][arm_i][charge_i][data_i] = new TH2F(name_vsFvtxDphi    .c_str(), title_vsFvtxDphi     .c_str(), 100, 0.0, GetVarMax(FVTX_DPHI_INDEX     ), 100, 0.0, GetVarMax(var_i));
					vsChi2         [var_i][arm_i][charge_i][data_i] = new TH2F(name_vsChi2        .c_str(), title_vsChi2         .c_str(), 100, 0.0, GetVarMax(CHI2_INDEX          ), 100, 0.0, GetVarMax(var_i));
					vsDCA_r        [var_i][arm_i][charge_i][data_i] = new TH2F(name_vsDCA_r       .c_str(), title_vsDCA_r        .c_str(), 100, 0.0, GetVarMax(DCA_R_INDEX         ), 100, 0.0, GetVarMax(var_i));
				}
			}
		}
	}
	return 0;
}

int Plotter::SetSignalFile(std::string sigFile)
{
	signalFile = sigFile;
	return 0; 
}

int Plotter::SetBackgroundFile(std::string bakFile)
{
	backgroundFile = bakFile;
	return 0; 
}

int Plotter::SetTreeName(std::string tree_name)
{
	treeName = treeName;
	return 0; 
}
