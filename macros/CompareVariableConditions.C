#include "../src/SingleMuonsConsts.h"

//TFile * f   = new TFile("./rootTrees/Run12_BasicCut_wnessTree_Simu.root");
TFile * f = new TFile("./rootTrees/PDF_DistributionsRun13.root","READ");

void CompareVariableConditions()
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
	PDF_save->Print("figs/KinematicVariables.pdf[");
	
	// DG0
	TCanvas* DG0 = new TCanvas("DG0","DG0",800,800);
	
	// sim max: 0.06
	// data max: 
	hFrame = drawFrame(0,0.0,20.0,0.06,"DG0 (cm)","a.u");
	hFrame->Draw();
	
	DG0_Leg = new TLegend(0.6,0.7,0.89,0.89);

	int i = -5;
	for( int arm_i = 0; arm_i < 2; arm_i++ )
	{
		for( int charge_i = 0; charge_i < 2; charge_i++)
		{
			std::string getHisto = Make1DVarName(DG0_INDEX, arm_i, charge_i, BACKGROUND_INDEX); 
			std::cout << getHisto << std::endl;
			TH1F* h = (TH1F*)f->Get(getHisto.c_str());
			h->UseCurrentStyle();
			h->SetLineColor(kRed+i);
			h->DrawNormalized("same");
			DG0_Leg->AddEntry(h,getHisto.c_str(),"l");
			i++;
		}
	}
	DG0_Leg->Draw("same");

	// DDG0
	TCanvas* DDG0 = new TCanvas("DDG0","DDG0",800, 800);
	// sim max: 0.055
	// data max: 
	hFrame = drawFrame(0,0,9.0,0.055,"DDG0 (deg)","a.u");
	hFrame->Draw();
	
	DDG0_Leg = new TLegend(0.6,0.7,0.89,0.89);

	i = -5;
	for( int arm_i = 0; arm_i < 2; arm_i++ )
	{
		for( int charge_i = 0; charge_i < 2; charge_i++)
		{
			std::string getHisto = Make1DVarName(DDG0_INDEX, arm_i, charge_i, BACKGROUND_INDEX); 
			std::cout << getHisto << std::endl;
			TH1F* h = (TH1F*)f->Get(getHisto.c_str());
			h->UseCurrentStyle();
			h->SetLineColor(kBlue+i);
			h->DrawNormalized("same");
			DDG0_Leg->AddEntry(h,getHisto.c_str(),"l");
			i++;
		}
	}
	DDG0_Leg->Draw("same");
	
	// CHI2
	TCanvas* CHI2 = new TCanvas("CHI2","CHI2",800, 800);
	// sim max: 0.07
	// data max: 
	hFrame = drawFrame(0,0,20.0,0.07,"CHI2 (a.u.)","a.u");
	hFrame->Draw();
	
	CHI2_Leg = new TLegend(0.6,0.7,0.89,0.89);

	int i = -5;
	for( int arm_i = 0; arm_i < 2; arm_i++ )
	{
		for( int charge_i = 0; charge_i < 2; charge_i++)
		{
			std::string getHisto = Make1DVarName(CHI2_INDEX, arm_i, charge_i, BACKGROUND_INDEX); 
			std::cout << getHisto << std::endl;
			TH1F* h = (TH1F*)f->Get(getHisto.c_str());
			h->UseCurrentStyle();
			h->SetLineColor(kPink+i);
			h->DrawNormalized("same");
			CHI2_Leg->AddEntry(h,getHisto.c_str(),"l");
			i++;
		}
	}
	CHI2_Leg->Draw("same");
	
	// DCA_R
	TCanvas* DCA_R = new TCanvas("DCA_R","DCA_R",800, 800);
	// sim max: 0.25
	// data max: 
	hFrame = drawFrame(0,0,20.0,0.25,"DCA_R (cm)","a.u");
	hFrame->Draw();
	
	DCA_R_Leg = new TLegend(0.6,0.7,0.89,0.89);

	i = -5;
	for( int arm_i = 0; arm_i < 2; arm_i++ )
	{
		for( int charge_i = 0; charge_i < 2; charge_i++)
		{
			std::string getHisto = Make1DVarName(DCA_R_INDEX, arm_i, charge_i, BACKGROUND_INDEX); 
			std::cout << getHisto << std::endl;
			TH1F* h = (TH1F*)f->Get(getHisto.c_str());
			h->UseCurrentStyle();
			h->SetLineColor(kGreen+i);
			h->DrawNormalized("same");
			DCA_R_Leg->AddEntry(h,getHisto.c_str(),"l");
			i++;
		}
	}
	DCA_R_Leg->Draw("same");
	
	// RPC1DCA
	TCanvas* RPC1DCA = new TCanvas("RPC1DCA","RPC1DCA",800, 800);
	// sim max: 0.35
	// data max: 
	hFrame = drawFrame(0,0,20.0,0.35,"RPC1DCA (cm)","a.u");
	hFrame->Draw();
	
	RPC1DCA_Leg = new TLegend(0.6,0.7,0.89,0.89);

	i = -5;
	for( int arm_i = 0; arm_i < 2; arm_i++ )
	{
		for( int charge_i = 0; charge_i < 2; charge_i++)
		{
			std::string getHisto = Make1DVarName(RPC1DCA_INDEX, arm_i, charge_i, BACKGROUND_INDEX); 
			std::cout << getHisto << std::endl;
			TH1F* h = (TH1F*)f->Get(getHisto.c_str());
			h->UseCurrentStyle();
			h->SetLineColor(kOrange+i);
			h->DrawNormalized("same");
			RPC1DCA_Leg->AddEntry(h,getHisto.c_str(),"l");
			i++;
		}
	}
	RPC1DCA_Leg->Draw("same");
	
	// RPC3DCA
	TCanvas* RPC3DCA = new TCanvas("RPC3DCA","RPC3DCA",800, 800);
	// sim max: 0.09
	// data max: 
	hFrame = drawFrame(0,0,20.0,0.09,"RPC3DCA (cm)","a.u");
	hFrame->Draw();
	
	RPC3DCA_Leg = new TLegend(0.6,0.7,0.89,0.89);

	i = -5;
	for( int arm_i = 0; arm_i < 2; arm_i++ )
	{
		for( int charge_i = 0; charge_i < 2; charge_i++)
		{
			std::string getHisto = Make1DVarName(RPC3DCA_INDEX, arm_i, charge_i, BACKGROUND_INDEX); 
			std::cout << getHisto << std::endl;
			TH1F* h = (TH1F*)f->Get(getHisto.c_str());
			h->UseCurrentStyle();
			h->SetLineColor(kViolet+i);
			h->DrawNormalized("same");
			RPC3DCA_Leg->AddEntry(h,getHisto.c_str(),"l");
			i++;
		}
	}
	RPC3DCA_Leg->Draw("same");
	
	DG0->Print    ("figs/KinematicVariables.pdf");
	DDG0->Print   ("figs/KinematicVariables.pdf");
	CHI2->Print   ("figs/KinematicVariables.pdf");
	DCA_R->Print  ("figs/KinematicVariables.pdf");
	RPC1DCA->Print("figs/KinematicVariables.pdf");
	RPC3DCA->Print("figs/KinematicVariables.pdf");
	DG0    ->SaveAs("figs/DG0.png");
	DDG0   ->SaveAs("figs/DDG0.png");
	CHI2   ->SaveAs("figs/CHI2.png");
	DCA_R  ->SaveAs("figs/DCA_R.png");
	RPC1DCA->SaveAs("figs/RPC1DCA.png");
	RPC3DCA->SaveAs("figs/RPC3DCA.png");
	

	PDF_save->Print("figs/KinematicVariables.pdf]");

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
