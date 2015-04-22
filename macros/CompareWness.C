#include "../src/SingleMuonsConsts.h"
#include "TLatex.h"

TFile* had_bak  = new TFile("./rootTrees/Run12WnessTreeWithFvtxAndRpc_Data.root");
TFile* w_sig   = new TFile("./rootTrees/danielWnessCalcualtion.root");
//TFile* w_sig = new TFile("./rootTrees/Run12WnessTreeWithFvtxAndRpc_Simu367593.root");

void CompareWness()
{

	gStyle->SetPadLeftMargin(0.18);
	gStyle->SetPadBottomMargin(0.18);
	gStyle->SetNdivisions(505,"X");
	gStyle->SetNdivisions(505,"Y");
	gStyle->SetTitleOffset(1.8,"X");
	gStyle->SetLabelSize(0.05,"X");
	gStyle->SetTitleSize(0.05,"Y");
	gStyle->SetTitleOffset(1.8,"Y");
	gStyle->SetLabelSize(0.05,"Y");
	gStyle->SetOptStat(0);
	gStyle->SetOptDate(0);

	gSystem->Load("libSingleMuonsWAnalysis.so");
	
	TCanvas* PDF_save = new TCanvas("PDF_save","", 800,800);
	PDF_save->Print("figs/Wness.pdf[");
	
	TCanvas* Wness = new TCanvas("Wness","Wness",800,800);

	Wness->SetLogy();
	
	//TH1F* hFrame = drawFrame(0,0,1,1,"normalized wness","wness");
	//hFrame->Draw();

	TH1F* effFrame = drawFrame(0.1,0.1,1.0,1.1,"f_{cut}","Efficiency");

	TH1F* Wness_had = new TH1F("Wness_had","Wnes_had",100,0,1);
	TH1F* Wness_wsim = new TH1F("Wness_wsim","Wnes_wsim",100,0,1);
	TH1F* effic_sig = new TH1F("sig_effic","effic",100,0,1);
	TH1F* effic_bak = new TH1F("bak_effic","effic",100,0,1);
	effic_sig->SetLineColor(kRed);
	effic_sig->SetLineWidth(2.5);
	effic_bak->SetLineColor(kBlue);
	effic_bak->SetLineWidth(2.5);


	TTree* wSimTree = (TTree*)w_sig->Get("newsngmuons_basic_cut");
	TTree* wBakTree = (TTree*)had_bak->Get("newsngmuons_basic_cut");

	Wness_had  ->UseCurrentStyle();
	Wness_wsim ->UseCurrentStyle();

	Wness_had ->SetLineColor(kRed);
	Wness_wsim->SetLineColor(kBlue);
	Wness_had ->SetLineWidth(2.5);
	Wness_wsim->SetLineWidth(2.5);
	
	wSimTree->Draw("Wness>>Wness_wsim");
	wBakTree->Draw("Wness>>Wness_had" );
	
	Wness->cd();
	Wness_had->DrawNormalized();
	Wness_wsim->DrawNormalized("same");

	float total_sig  = Wness_wsim->Integral();
	float total_bak  = Wness_had->Integral();
	for(int i = 0; i < 100; i++)
	{
		float signal_sig = Wness_wsim->Integral(0,i);
		float signal_bak = Wness_had->Integral(0,i);

		if( total_sig > 0.0) effic_sig->SetBinContent(i,1-signal_sig/total_sig);
		if( total_bak > 0.0) effic_bak->SetBinContent(i,1-signal_bak/total_bak);
	}
	TCanvas* Efficiency = new TCanvas("Efficiency","Efficiency",800,800);
	Efficiency->cd();
	//effFrame->Draw();
	TGraph* gFrame = new TGraph();
	gFrame->SetName("gFrame");
	gFrame->SetTitle("Efficiency;Efficiency;a.u.");
	gFrame->SetPoint(0,0,0);
	gFrame->SetPoint(1,1.0,1.0);
	gFrame->Draw("AP");
	effic_bak->Draw("same");
	effic_sig->Draw("same");
	//effFrame->Draw("same");
	
	Efficiency->Print("figs/Wness.pdf");
	Wness->Print("figs/Wness.pdf");
	PDF_save->Print("figs/Wness.pdf]");

}


TH1F *drawFrame(Float_t xlow, Float_t ylow, Float_t xhigh, Float_t yhigh,
		TString xTitle="", TString yTitle="")
{
	TH1F *hFrame;

	if(gPad->GetLogy()==0) {
		hFrame = gPad->DrawFrame(xlow,ylow,xhigh,yhigh);
		hFrame->GetXaxis()->SetTitle(xTitle);
		hFrame->GetYaxis()->SetTitle(yTitle); }
	else {
		hFrame = gPad->DrawFrame(xlow,pow(10,ylow),xhigh,pow(10,yhigh));
		hFrame->GetXaxis()->SetTitle(xTitle);
		hFrame->GetYaxis()->SetTitle(yTitle); }

	if(ylow*yhigh<0.) {
		TLine *l = new TLine(xlow,0,xhigh,0);
		l->SetLineStyle(2);
		l->Draw(); }

	return hFrame;
}
