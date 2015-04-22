#include "TLatex.h"
#include "../src/SingleMuonsConsts.h"

#include <string>


void dw23Plots(int fileOption = 0)
{

	TFile* f;
	if(fileOption == 0 )
	{
		f = new TFile("./rootTrees/Run13WnessTreeWithFvtxAndRpc_Data.root","READ"); // data
	}
	else if (fileOption == 1)
	{
		f = new TFile("./rootTrees/Run13WnessTreeWithFvtxAndRpc_Simu367593.root","READ"); // simu
	}
	else if(fileOption == 2 )
	{
		f = new TFile("./rootTrees/Run13WnessTreeWithFvtxAndRpc_SimuAll.root","READ"); // simu
	}
	else
	{
		f = new TFile("./rootTrees/Run13WnessTreeWithFvtxAndRpc_Data.root","READ");
	}
	TTree* data = (TTree*)f->Get("newsngmuons_basic_cut");
	gStyle->SetPadLeftMargin(0.18);
	gStyle->SetPadBottomMargin(0.18);
	gStyle->SetNdivisions(505,"X");
	gStyle->SetNdivisions(505,"Y");
	gStyle->SetTitleOffset(0.9,"X");
	gStyle->SetTitleOffset(0.9,"Y");
	gStyle->SetTitleSize(0.08,"Y");
	gStyle->SetTitleSize(0.08,"X");
	gStyle->SetLabelSize(0.08,"X");
	gStyle->SetLabelSize(0.08,"Y");
	gStyle->SetOptStat(0);
	gStyle->SetOptDate(0);
	gSystem->Load("libSingleMuonsWAnalysis.so");
	
	//TCanvas* PDFsave    = new TCanvas("PDFsave","PDFsave",800,800);
	TCanvas* dw23Contour = new TCanvas("dw23Contour","dw23Contour",0,0,800,800);
	TCanvas* dw23vsEta   = new TCanvas("dw23vsEta","dw23vsEta",0,0,800,800);
	TCanvas* fcut        = new TCanvas("fcut","fcut",0,0,800,800);

	dw23Contour->Divide(3,4);
	dw23vsEta->Divide(3,4);
	fcut->Divide(4,4);


	TH2F* etaVsWness [2][2];
	TH2F* dw23VsWness[2][2];
	TH1F* Wness      [2][2];

	TH2F* dw23VsEta  [2][2]; // new 
	TH1F* dw23_h     [2][2]; // new 
	TH1F* Eta_h      [2][2]; // new
	TH1F* fcut_h     [4][2][2]; // new, [fcut binning],[arm bin],[charge bin]

	TCut cut[2][2][3];
	TCut North = "pz>0";
	TCut South = "pz<0";
	TCut Positive = "charge>0";
	TCut Negative = "charge<0";
	TCut WnessCutData = "(Wness>0.1)&&(Wness<0.9)";
	TCut WnessCutSimu = "Wness>0.9";
	cut[ARM_NORTH][CHARGE_POS][HADRONIC_BACKGROUND] = (North&&Positive)&&WnessCutData;  
	cut[ARM_NORTH][CHARGE_NEG][HADRONIC_BACKGROUND] = (North&&Negative)&&WnessCutData;
	cut[ARM_SOUTH][CHARGE_POS][HADRONIC_BACKGROUND] = (South&&Positive)&&WnessCutData;
	cut[ARM_SOUTH][CHARGE_NEG][HADRONIC_BACKGROUND] = (South&&Negative)&&WnessCutData;
	cut[ARM_NORTH][CHARGE_POS][W_SIMULATION] = (North&&Positive)&&WnessCutSimu;  
	cut[ARM_NORTH][CHARGE_NEG][W_SIMULATION] = (North&&Negative)&&WnessCutSimu;
	cut[ARM_SOUTH][CHARGE_POS][W_SIMULATION] = (South&&Positive)&&WnessCutSimu;
	cut[ARM_SOUTH][CHARGE_NEG][W_SIMULATION] = (South&&Negative)&&WnessCutSimu;
	cut[ARM_NORTH][CHARGE_POS][W_SIMULATION] = (North&&Positive)&&WnessCutSimu;  
	cut[ARM_NORTH][CHARGE_NEG][W_SIMULATION] = (North&&Negative)&&WnessCutSimu;
	cut[ARM_SOUTH][CHARGE_POS][W_SIMULATION] = (South&&Positive)&&WnessCutSimu;
	cut[ARM_SOUTH][CHARGE_NEG][W_SIMULATION] = (South&&Negative)&&WnessCutSimu;

	int canvas_i = 1;
	int canvas_j = 1;
	int canvas_k = 1;

	int bins = 50;
	std::cout << data->GetEntries() << std::endl;
	for( int arm_i = 0; arm_i < 2; arm_i++)
	{
		for( int charge_i = 0; charge_i < 2; charge_i++)
		{
			std::string name_stub;
			std::string title_stub;
			std::string name;
			std::string title;
			std::string drawCmd;
			
			MakeNameTitleStubPair(arm_i, charge_i, 0, name_stub, title_stub);

			std::cout << name_stub << ", " << title_stub << std::endl;

			// dw23, eta, Wness
			std::cout << canvas_i << std::endl;
			name = "etaVsWness_" + name_stub;
			title = "|#eta| VS Wness " + title_stub+";Wness;|#eta|;Counts";
			etaVsWness[arm_i][charge_i] = new TH2F(name.c_str(),title.c_str(),bins,0.1,0.95,bins,1.2,2.6);	
			etaVsWness[arm_i][charge_i]->Sumw2();
			drawCmd = "abs(eta):Wness>>"+name;
			dw23Contour->cd(canvas_i); canvas_i++;
			data->Draw(drawCmd.c_str(),cut[arm_i][charge_i][HADRONIC_BACKGROUND]);
			etaVsWness[arm_i][charge_i]->Draw("cont1");

			std::cout << canvas_i << std::endl;
			name = "dw23VsWness_" + name_stub;
			title = "dw23 VS Wness " + title_stub + ";Wness;dw23;Counts";
			dw23VsWness[arm_i][charge_i] = new TH2F(name.c_str(), title.c_str(),bins,0.1,0.95,bins,-0.3,0.3);
			dw23VsWness[arm_i][charge_i]->Sumw2();
			drawCmd = "dw23:Wness>>"+name;
			dw23Contour->cd(canvas_i); canvas_i++;
			data->Draw(drawCmd.c_str(),cut[arm_i][charge_i][HADRONIC_BACKGROUND]);
			dw23VsWness[arm_i][charge_i]->Draw("cont1");

			std::cout << canvas_i << std::endl;
			name = "Wness_" + name_stub;
			title = "Wness " + title_stub+";Wness;Counts";
			Wness[arm_i][charge_i] = new TH1F(name.c_str(),title.c_str(), bins, 0.1, 0.95);
			Wness[arm_i][charge_i]->Sumw2();
			drawCmd = "Wness>>"+name;
			// TPad* p = dw23Contour->cd(canvas_i); canvas_i++;
			// p->SetLogy();
			dw23Contour->cd(canvas_i); canvas_i++;
			data->Draw(drawCmd.c_str(),cut[arm_i][charge_i][HADRONIC_BACKGROUND]);
			Wness[arm_i][charge_i]->Draw();

			dw23Contour->Update();

			// dw23 hist, eta hist, dw23 vs eta
			name = "dw23VsEta_" + name_stub;
			title = "dw23 VS |#eta| " + title_stub + ";|#eta|;dw23";
			dw23VsEta[arm_i][charge_i] = new TH2F(name.c_str(), title.c_str(), bins,1,2.6,bins,-0.3,0.3);
			dw23VsEta[arm_i][charge_i]->Sumw2();
			drawCmd = "dw23:abs(eta)>>"+name;
			dw23vsEta->cd(canvas_j); canvas_j++;
			data->Draw(drawCmd.c_str(),cut[arm_i][charge_i][HADRONIC_BACKGROUND]);
			dw23VsEta[arm_i][charge_i]->Draw("cont1");

			name = "Eta_h_" + name_stub;
			title = "Eta_h_" + title_stub + ";|#eta|;a.u.";
			Eta_h[arm_i][charge_i] = new TH1F(name.c_str(), title.c_str(), bins,1,2.6);
			Eta_h[arm_i][charge_i]->Sumw2();
			drawCmd = "abs(eta)>>"+name;
			dw23vsEta->cd(canvas_j); canvas_j++;
			data->Draw(drawCmd.c_str(),cut[arm_i][charge_i][HADRONIC_BACKGROUND]);
			Eta_h[arm_i][charge_i]->Draw();

			name = "dw23_h_"+name_stub;
			title = "dw23_h_"+title_stub+";dw23;a.u.";
			dw23_h[arm_i][charge_i] = new TH1F(name.c_str(), title.c_str(), bins, -0.3, 0.3);
			dw23_h[arm_i][charge_i]->Sumw2();
			drawCmd = "dw23>>"+name;
			dw23vsEta->cd(canvas_j); canvas_j++;
			data->Draw(drawCmd.c_str(),cut[arm_i][charge_i][HADRONIC_BACKGROUND]);
			dw23_h[arm_i][charge_i]->Draw();

			dw23vsEta->Update();

			// Fcut 
			name = "fcut_0_"+name_stub;
			title = "fcut 0.1 < Wness < 0.3, " + title_stub + ";dw23;a.u";
			fcut_h[0][arm_i][charge_i] = new TH1F(name.c_str(), title.c_str(), bins,-0.3,0.3);
			fcut_h[0][arm_i][charge_i]->Sumw2();
			drawCmd = "dw23>>"+name;
			fcut->cd(canvas_k); canvas_k++;
			data->Draw(drawCmd.c_str(),"(0.1<Wness)&&(Wness<0.3)");
			fcut_h[0][arm_i][charge_i]->Draw();
			
			name = "fcut_1_"+name_stub;
			title = "fcut 0.3 < Wness < 0.5, " + title_stub + ";dw23;a.u";
			fcut_h[1][arm_i][charge_i] = new TH1F(name.c_str(), title.c_str(), bins,-0.3,0.3);
			fcut_h[1][arm_i][charge_i]->Sumw2();
			drawCmd = "dw23>>"+name;
			fcut->cd(canvas_k); canvas_k++;
			data->Draw(drawCmd.c_str(),"(0.3<Wness)&&(Wness<0.5)");
			fcut_h[1][arm_i][charge_i]->Draw();
			
			name = "fcut_2_"+name_stub;
			title = "fcut 0.5 < Wness < 0.7, " + title_stub + ";dw23;a.u";
			fcut_h[2][arm_i][charge_i] = new TH1F(name.c_str(), title.c_str(), bins,-0.3,0.3);
			fcut_h[2][arm_i][charge_i]->Sumw2();
			drawCmd = "dw23>>"+name;
			fcut->cd(canvas_k); canvas_k++;
			data->Draw(drawCmd.c_str(),"(0.5<Wness)&&(Wness<0.7)");
			fcut_h[2][arm_i][charge_i]->Draw();
			
			name = "fcut_3_"+name_stub;
			title = "fcut 0.7 < Wness < 0.9, " + title_stub + ";dw23;a.u";
			fcut_h[3][arm_i][charge_i] = new TH1F(name.c_str(), title.c_str(), bins,-0.3,0.3);
			fcut_h[3][arm_i][charge_i]->Sumw2();
			drawCmd = "dw23>>"+name;
			fcut->cd(canvas_k); canvas_k++;
			data->Draw(drawCmd.c_str(),"(0.7<Wness)&&(Wness<0.9)");
			fcut_h[3][arm_i][charge_i]->Draw();

			fcut->Update();
		}
	}

	// Save Canvases For Presentations, etc
	fcut->SaveAs("./figs/fcut_double_gauss.png");
	dw23Contour->SaveAs("./figs/dw23_and_eta_vs_Wness.png");
	dw23vsEta->SaveAs("./figs/dw23_vs_eta.png");
}
