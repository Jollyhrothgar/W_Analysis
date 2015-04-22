#include "SBRextract.h"

SBRextract::SBRextract( std::string pdfFile, std::string treeFile)
{
  pdfSeedFileName      = pdfFile;
  unbinnedDataFileName = treeFile;
  SbrOutFileName = baseDirectory + logDirectory + "SBR_Yields_Whole_Eta_Range.txt";
  sbrCSVName = baseDirectory + logDirectory + "SBR.csv";

}

void SBRextract::Run()
{
  LoadPdfs();
  LoadUnbinnedData();
  SetupSubTrees();
  MakeRooVariables(); // done
  MakeRooWSignalPdfs(); // done
  MakeRooHadronicBackgroundPdfs(); // done 
  MakeRooMuonBackgroundPdfs(); // done
  MakeRooDataSet(); // done ish wp
  MakeRooModel(); // done ish wp
  FitModelToDataSet(); // done ish wp
}
void SBRextract::SetSBROutFileNameStd(std::string fName)
{
  SbrOutFileName = fName;
  std::cout << "Sending SBR results for whole eta range to: " << SbrOutFileName << std::endl;
}

void SBRextract::SetSBROutFileNameCsv(std::string fName)
{
  sbrCSVName = fName;
  std::cout << "Sending SBR results for whole eta range to csv table: " << sbrCSVName << std::endl;
}

SBRextract::~SBRextract()
{
  for(int arm_i = 0; arm_i < NUMBER_OF_ARM_CONFIGS; arm_i++)
  {
    for(int charge_i = 0; charge_i < NUMBER_OF_CHARGE_CONFIGS; charge_i++)
    {
      delete subTree[arm_i][charge_i];
      for(int muon_i = 0; muon_i < NUMBER_OF_MUON_STREAMS; muon_i++)
      {
        delete rooHist_dw23 [muon_i][arm_i][charge_i];
        delete rooHist_eta  [muon_i][arm_i][charge_i];
        delete pdf_dw23     [muon_i][arm_i][charge_i];
        delete pdf_eta      [muon_i][arm_i][charge_i];
        delete prodPdf      [muon_i][arm_i][charge_i];
      }
      delete DataSet[arm_i][charge_i];
      delete Model[arm_i][charge_i];
    }
  }
  delete v_dw23 ; 
  delete v_eta  ;
  delete v_Wness;
  delete v_Nmu_HadBak;
  delete v_Nmu_WSig  ;
  delete v_Nmu_MuBak ;
  delete rootFilePdfSeeds;
  delete unbinnedDataFile;
}

int SBRextract::LoadPdfs()
{
  rootFilePdfSeeds = new TFile(pdfSeedFileName.c_str(), "READ");
  if(rootFilePdfSeeds)
  {
    for(int arm_i = 0; arm_i < NUMBER_OF_ARM_CONFIGS; arm_i++)
    {
      for(int charge_i = 0; charge_i < NUMBER_OF_CHARGE_CONFIGS; charge_i++)
      {
        h_dw23_pdfSeed [BAK_INDEX][arm_i][charge_i] = (TH1F*) rootFilePdfSeeds->Get( (MakePdfName("dw23" ,"muBak" , arm_i, charge_i ) ).c_str() );
        h_eta_pdfSeed  [BAK_INDEX][arm_i][charge_i] = (TH1F*) rootFilePdfSeeds->Get( (MakePdfName("eta","muBak" , arm_i, charge_i ) ).c_str() );
        h_dw23_pdfSeed [HAD_INDEX][arm_i][charge_i] = (TH1F*) rootFilePdfSeeds->Get( (MakePdfName("dw23" ,"HadBak", arm_i, charge_i ) ).c_str() );
        h_eta_pdfSeed  [HAD_INDEX][arm_i][charge_i] = (TH1F*) rootFilePdfSeeds->Get( (MakePdfName("eta","HadBak", arm_i, charge_i ) ).c_str() );
        h_dw23_pdfSeed [W_INDEX]  [arm_i][charge_i] = (TH1F*) rootFilePdfSeeds->Get( (MakePdfName("dw23" ,"WSig"  , arm_i, charge_i ) ).c_str() );
        h_eta_pdfSeed  [W_INDEX]  [arm_i][charge_i] = (TH1F*) rootFilePdfSeeds->Get( (MakePdfName("eta","WSig"  , arm_i, charge_i ) ).c_str() );
      }
    }

    // Extract Muon Yields
    int muBakArm0Charge0, muBakArm1Charge1, muBakArm0Charge1, muBakArm1Charge0;
    TTree* t_nMu = (TTree*)rootFilePdfSeeds->Get("NmuBak");
    t_nMu->SetBranchAddress("nMuBakMuons_Arm0_Charge0", &muBakArm0Charge0);
    t_nMu->SetBranchAddress("nMuBakMuons_Arm1_Charge0", &muBakArm1Charge0);
    t_nMu->SetBranchAddress("nMuBakMuons_Arm1_Charge1", &muBakArm1Charge1);
    t_nMu->SetBranchAddress("nMuBakMuons_Arm0_Charge1", &muBakArm0Charge1);
    t_nMu->GetEvent(0);

    Num_MuBak_Muons [0][0] = muBakArm0Charge0;
    Num_MuBak_Muons [1][0] = muBakArm1Charge0;
    Num_MuBak_Muons [1][1] = muBakArm1Charge1;
    Num_MuBak_Muons [0][1] = muBakArm0Charge1;
   
    std::cout << "Loaded: " << pdfSeedFileName << std::endl;
  }
  else
  {
    return -1;
  }
  return 0;
}

int SBRextract::LoadUnbinnedData()
{
  unbinnedDataFile = new TFile(unbinnedDataFileName.c_str(), "READ");  
  if(unbinnedDataFile)
  {
    unbinnedData = (TTree*)unbinnedDataFile->Get(wness_tree.c_str());
    if(unbinnedData)
    {
      std::cout << "Loaded: "  << unbinnedData->GetName() << ": " << unbinnedData->GetEntries() <<  " entries. " << std::endl;
      return 0;
    }
    else
    {
      std::cout << " Coult Not Extract Tree From " << unbinnedDataFileName << std::endl;
      return -1;
    }
  }
  else
  {
    std::cout << "Cannot read or open " << unbinnedDataFileName << std::endl;
    return -1;
  }
  return 0;
}

std::string SBRextract::MakePdfName(const std::string& var, const std::string& type, int arm_i, int charge_i)
{
  std::stringstream name;
  name << "h_" << var << "_" << type << "_pdfSeed_Arm" << arm_i << "_Charge" << charge_i;
  return name.str();
}

int SBRextract::SetupSubTrees()
{
  // Make Copies of the tree
  BasicCutTreeVariables vSub[NUMBER_OF_ARM_CONFIGS][NUMBER_OF_CHARGE_CONFIGS];

  subTree[ARM_NORTH][CHARGE_POS] = unbinnedData->CopyTree("pz>0 && charge > 0 && Wness > 0.92");
  subTree[ARM_NORTH][CHARGE_NEG] = unbinnedData->CopyTree("pz>0 && charge < 0 && Wness > 0.92");
  subTree[ARM_SOUTH][CHARGE_POS] = unbinnedData->CopyTree("pz<0 && charge > 0 && Wness > 0.92");
  subTree[ARM_SOUTH][CHARGE_NEG] = unbinnedData->CopyTree("pz<0 && charge < 0 && Wness > 0.92");
  for(int arm_i = 0; arm_i < NUMBER_OF_ARM_CONFIGS; arm_i++)
  {
    for(int charge_i = 0; charge_i < NUMBER_OF_CHARGE_CONFIGS; charge_i++)
    {
      vSub[arm_i][charge_i].LinkTree(subTree[arm_i][charge_i],WRITE);
      for(Long64_t i = 0; i < subTree[arm_i][charge_i]->GetEntries(); i++)
      {
        vSub[arm_i][charge_i].eta = fabs(vSub[arm_i][charge_i].eta);
      }
    }
  }

  for(int arm_i = 0; arm_i < NUMBER_OF_ARM_CONFIGS; arm_i++)
  {
    for(int charge_i = 0; charge_i < NUMBER_OF_CHARGE_CONFIGS; charge_i++)
    {
      totalMuons[arm_i][charge_i] = subTree[arm_i][charge_i]->GetEntries();
      std::cout << " Sub Tree: arm" << arm_i << ", charge" << charge_i << ": " << totalMuons[arm_i][charge_i] << std::endl;
    }
  }
  return 0;
}

std::string SBRextract::MakeRooVarName(const std::string &stub, int arm_i, int charge_i)
{
  std::stringstream name;
  name << stub << "_Arm" << arm_i << "_Charge" << charge_i;
  return name.str();
}

int SBRextract::MakeRooVariables()
{
  for(int arm_i = 0; arm_i < NUMBER_OF_ARM_CONFIGS; arm_i++)
  {
    for(int charge_i = 0; charge_i < NUMBER_OF_CHARGE_CONFIGS; charge_i++)
    {
      // must have same name as tree variable.
      v_dw23        = new RooRealVar("dw23" , "RooVar for dw23 - reduced azimuthal bending"          , 0    , -0.1    , 0.1 );
      v_eta         = new RooRealVar("eta"  , "RooVar for eta - the pseudorapidity"                  , 1.5  , 1.1     , 2.6 );
      v_Wness       = new RooRealVar("Wness", "RooVar - the Wness"                                   , 9    , 0       , 1   );
      v_Nmu_HadBak = new RooRealVar("v_Nmu_HadBak", "RooVar counting the number of fake  muons"      , 1    , 0       , 4500);
      v_Nmu_WSig   = new RooRealVar("v_Nmu_WSig"  , "RooVar counting the number of w-signal muons"   , 1    , 0       , 4500);
      v_Nmu_MuBak  = new RooRealVar("v_Nmu_MuBak" , "RooVar counting the number of background muons" , 1    , 0       , 4500);
    }
  }
  return 0;
}

int SBRextract::MakeRooWSignalPdfs()
{
  // prod pdf
  for(int arm_i = 0; arm_i < NUMBER_OF_ARM_CONFIGS; arm_i++)
  {
    for(int charge_i = 0; charge_i < NUMBER_OF_CHARGE_CONFIGS; charge_i++)
    {
      std::stringstream name;
      name << "rooHist_dw23_WSig" << "_Arm" << arm_i << "_Charge" << charge_i;
      rooHist_dw23[W_INDEX][arm_i][charge_i] = new RooDataHist(
          name.str().c_str(),
          name.str().c_str(),
          *v_dw23,
          h_dw23_pdfSeed[W_INDEX][arm_i][charge_i]);
      name.str("");

      // RooHistPdf - dw23
      name << "pdf_dw23_WSig" << "_Arm" << arm_i << "_Charge" << charge_i;
      pdf_dw23[W_INDEX][arm_i][charge_i] = new RooHistPdf(
          name.str().c_str(),
          name.str().c_str(),
          *v_dw23,
          *(rooHist_dw23[W_INDEX][arm_i][charge_i]));
      name.str("");

      // RooDataHist - eta
      name << "rooHist_eta_WSig" << "_Arm" << arm_i << "_Charge" << charge_i;
      rooHist_eta[W_INDEX][arm_i][charge_i] = new RooDataHist(
          name.str().c_str(),
          name.str().c_str(),
          *v_eta,
          h_eta_pdfSeed[W_INDEX][arm_i][charge_i]);
      name.str("");

      // RooDataHist - eta
      name << "pdf_eta_WSig" << "_Arm" << arm_i << "_Charge" << charge_i;
      pdf_eta[W_INDEX][arm_i][charge_i] = new RooHistPdf(
          name.str().c_str(),
          name.str().c_str(),
          *v_eta,
          *(rooHist_eta[W_INDEX][arm_i][charge_i]));
      name.str("");

      // Form ProdPdfs for the PDF which is fit to data
      name.str("");
      name << "wSigBak_prodPdf" << "_Arm" << arm_i << "_Charge" << charge_i;
      prodPdf[W_INDEX][arm_i][charge_i] = new RooProdPdf(
          name.str().c_str(),
          name.str().c_str(),
          RooArgList(*pdf_eta[W_INDEX][arm_i][charge_i], *pdf_dw23[W_INDEX][arm_i][charge_i]),
          0);
      name.str("");
    }
  }
  return 0;
}

int SBRextract::MakeRooHadronicBackgroundPdfs()
{
  for(int arm_i = 0; arm_i < NUMBER_OF_ARM_CONFIGS; arm_i++)
  {
    for(int charge_i = 0; charge_i < NUMBER_OF_CHARGE_CONFIGS; charge_i++)
    {
      std::stringstream name;
      // RooDatahist - dw23
      name << "rooHist_dw23_HadBak" << "_Arm" << arm_i << "_Charge" << charge_i;
      rooHist_dw23[HAD_INDEX][arm_i][charge_i] = new RooDataHist(
          name.str().c_str(),
          name.str().c_str(),
          *v_dw23,
          h_dw23_pdfSeed[HAD_INDEX][arm_i][charge_i]);
      name.str("");

      // RooHistPdf - dw23
      name << "pdf_dw23_HadBak" << "_Arm" << arm_i << "_Charge" << charge_i;
      pdf_dw23[HAD_INDEX][arm_i][charge_i] = new RooHistPdf(
          name.str().c_str(),
          name.str().c_str(),
          *v_dw23,
          *(rooHist_dw23[HAD_INDEX][arm_i][charge_i]));
      name.str("");

      // RooDataHist - eta
      name << "rooHist_eta_HadBak" << "_Arm" << arm_i << "_Charge" << charge_i;
      rooHist_eta[HAD_INDEX][arm_i][charge_i] = new RooDataHist(
          name.str().c_str(),
          name.str().c_str(),
          *v_eta,
          h_eta_pdfSeed[HAD_INDEX][arm_i][charge_i]);
      name.str("");

      // RooDataHist - eta
      name << "pdf_eta_HadBak" << "_Arm" << arm_i << "_Charge" << charge_i;
      pdf_eta[HAD_INDEX][arm_i][charge_i] = new RooHistPdf(
          name.str().c_str(),
          name.str().c_str(),
          *v_eta,
          *(rooHist_eta[HAD_INDEX][arm_i][charge_i]));
      name.str("");

      // Form ProdPdfs for the PDF which is fit to data
      name << "hadBak_prodPdf" << "_Arm" << arm_i << "_Charge" << charge_i;
      prodPdf[HAD_INDEX][arm_i][charge_i] = new RooProdPdf(
          name.str().c_str(),
          name.str().c_str(),
          RooArgList(*pdf_eta[HAD_INDEX][arm_i][charge_i], *pdf_dw23[HAD_INDEX][arm_i][charge_i]),
          0);
    }
  }
  return 0;
}

int SBRextract::MakeRooMuonBackgroundPdfs()
{
  // Now that PdfSeeds are created, we can make the actual RooFit PDFs.
  for(int arm_i = 0; arm_i < NUMBER_OF_ARM_CONFIGS; arm_i++)
  {
    for(int charge_i = 0; charge_i < NUMBER_OF_CHARGE_CONFIGS; charge_i++)
    {
      // RooDatahist - dw23
      std::stringstream name;
      name << "rooHist_dw23_MuBak" << "_Arm" << arm_i << "_Charge" << charge_i;
      rooHist_dw23[BAK_INDEX][arm_i][charge_i] = new RooDataHist(
          name.str().c_str(),
          name.str().c_str(),
          *v_dw23,
          h_dw23_pdfSeed[BAK_INDEX][arm_i][charge_i]);
      name.str("");

      // RooHistPdf - dw23
      name << "pdf_dw23_MuBak" << "_Arm" << arm_i << "_Charge" << charge_i;
      pdf_dw23[BAK_INDEX][arm_i][charge_i] = new RooHistPdf(
          name.str().c_str(),
          name.str().c_str(),
          *v_dw23,
          *(rooHist_dw23[BAK_INDEX][arm_i][charge_i]));
      name.str("");

      // RooDatahist - eta
      name.str("");
      name << "rooHist_eta_MuBak" << "_Arm" << arm_i << "_Charge" << charge_i;
      rooHist_eta[BAK_INDEX][arm_i][charge_i] = new RooDataHist(
          name.str().c_str(),
          name.str().c_str(),
          *v_eta,
          h_eta_pdfSeed[BAK_INDEX][arm_i][charge_i]);
      name.str("");

      // RooHistPdf - eta
      name << "pdf_eta_MuBak" << "_Arm" << arm_i << "_Charge" << charge_i;
      pdf_eta[BAK_INDEX][arm_i][charge_i] = new RooHistPdf(
          name.str().c_str(),
          name.str().c_str(),
          *v_eta,
          *(rooHist_eta[BAK_INDEX][arm_i][charge_i]));
      name.str("");

      // Form ProdPdfs for the PDF which is fit to data
      name << "muBak_prodPdf" << "_Arm" << arm_i << "_Charge" << charge_i;
      prodPdf[BAK_INDEX][arm_i][charge_i] = new RooProdPdf(
          name.str().c_str(),
          name.str().c_str(),
          RooArgList(*pdf_eta[BAK_INDEX][arm_i][charge_i], *pdf_dw23[BAK_INDEX][arm_i][charge_i]),
          0);
    }
  }
  return 0;
}


int SBRextract::MakeRooModel()
{
  for(int arm_i = 0; arm_i < NUMBER_OF_ARM_CONFIGS; arm_i++)
  {
    for(int charge_i = 0; charge_i < NUMBER_OF_CHARGE_CONFIGS; charge_i++)
    {
      std::stringstream name;
      name << "RooModel_Arm" << arm_i << "_Charge" << charge_i;
      Model[arm_i][charge_i] = new RooAddPdf(
          name.str().c_str(),
          "Model formed from PDFs generated from extrapolated values for |eta|, dw23 and Wness",
          RooArgList(*prodPdf[W_INDEX][arm_i][charge_i], *prodPdf[HAD_INDEX][arm_i][charge_i], *prodPdf[BAK_INDEX][arm_i][charge_i]),
          RooArgList(*v_Nmu_WSig, *v_Nmu_HadBak, *v_Nmu_MuBak));
      name.str("");
    }
  }
  return 0;
}

int SBRextract::MakeRooDataSet()
{

  for(int arm_i = 0; arm_i < NUMBER_OF_ARM_CONFIGS; arm_i++)
  {
    for(int charge_i = 0; charge_i < NUMBER_OF_CHARGE_CONFIGS; charge_i++)
    {
      std::stringstream name;
      name << "dataSet" << "_Arm" << arm_i << "_Charge" << charge_i;

      DataSet[arm_i][charge_i] = new RooDataSet(
          name.str().c_str(),
          "Data set filtered fow high Wness and arm and charge",
          subTree[arm_i][charge_i],
          RooArgSet(*v_dw23, *v_eta, *v_Wness)
          );
      name.str("");
    }
  }
  return 0;
}

int SBRextract::FitModelToDataSet()
{
  // Ralf example:
  // Getting Results:
  // eta.setRange(etachar,etas[i],etas[i+1]);
  // dw23.setRange(etachar,-0.05+q*0.04,0.01+q*0.04);
  // RooAbsReal* fracHad = had_pdf->createIntegral(RooArgSet(eta,dw23));
  // RooAbsReal* fracHadRange = had_pdf->createIntegral(RooArgSet(eta,dw23),NormS
  // et(RooArgSet(eta,dw23)),Range(etachar));
  // nhad.getVal() * fracHadRange->getVal()
  // and similar for the other two variables.

  // Here, we must fix the number of muon background contributions.
  std::ofstream SbrWholeEtaRange(SbrOutFileName.c_str());
  for(int arm_i = 0; arm_i < NUMBER_OF_ARM_CONFIGS; arm_i++)
  {
    for(int charge_i = 0; charge_i < NUMBER_OF_CHARGE_CONFIGS; charge_i++)
    {
      Num_HadBak_Muons[arm_i][charge_i] = 0;
      Num_WSig_Muons   [arm_i][charge_i] = 0;
      Err_Hi_HadBak_Muons [arm_i][charge_i] = 0;
      Err_Lo_HadBak_Muons [arm_i][charge_i] = 0;
      Err_Hi_WSig_Muons [arm_i][charge_i] = 0;
      Err_Lo_WSig_Muons [arm_i][charge_i] = 0;
      SBR[arm_i][charge_i] = 0;
      Err_Hi_SBR[arm_i][charge_i] = 0;
      Err_Lo_SBR[arm_i][charge_i] = 0;

      *v_Nmu_MuBak = Num_MuBak_Muons[arm_i][charge_i];
      *v_Nmu_HadBak = 500;
      *v_Nmu_WSig = 500;
      v_Nmu_MuBak->setConstant(true);

      Model[arm_i][charge_i] -> fitTo(*DataSet[arm_i][charge_i],Extended(kTRUE), Minos(kTRUE),NumCPU(2),SumW2Error(kTRUE));

      Num_HadBak_Muons[arm_i][charge_i] = v_Nmu_HadBak->getVal();
      Num_WSig_Muons   [arm_i][charge_i] = v_Nmu_WSig  ->getVal();
      Err_Hi_HadBak_Muons [arm_i][charge_i] = v_Nmu_HadBak->getErrorHi();
      Err_Lo_HadBak_Muons [arm_i][charge_i] = v_Nmu_HadBak->getErrorLo();
      Err_Hi_WSig_Muons [arm_i][charge_i] = v_Nmu_WSig->getErrorHi();
      Err_Lo_WSig_Muons [arm_i][charge_i] = v_Nmu_WSig->getErrorLo();

      double signal = Num_WSig_Muons[arm_i][charge_i];
      double background = Num_MuBak_Muons[arm_i][charge_i] + Num_HadBak_Muons[arm_i][charge_i];
      SBR[arm_i][charge_i] = signal/background; 

      Err_Hi_SBR[arm_i][charge_i] = pow(pow(static_cast<double>((Err_Hi_HadBak_Muons[arm_i][charge_i])/Num_HadBak_Muons[arm_i][charge_i]),2)+ pow(static_cast<double>((Err_Hi_WSig_Muons[arm_i][charge_i]/Num_WSig_Muons[arm_i][charge_i])),2),0.5);
      Err_Lo_SBR[arm_i][charge_i] = pow(pow(static_cast<double>((Err_Lo_HadBak_Muons[arm_i][charge_i])/Num_HadBak_Muons[arm_i][charge_i]),2)+ pow(static_cast<double>((Err_Lo_WSig_Muons[arm_i][charge_i]/Num_WSig_Muons[arm_i][charge_i])),2),0.5);

      Err_Hi_SBR[arm_i][charge_i] = SBR[arm_i][charge_i]*Err_Hi_SBR[arm_i][charge_i];
      Err_Lo_SBR[arm_i][charge_i] = SBR[arm_i][charge_i]*Err_Lo_SBR[arm_i][charge_i];

      // Experimental wp
      //RooPlot *dw23Frame = v_dw23->frame();
      //dw23Frame->SetName("dw23 Fit");
      //DataSet[arm_i][charge_i]->plotOn(dw23Frame);
      //c_RooPlotTest = new TCanvas("c_RooPlotTest","RooPlot Test", 800, 800);
      //RegisterCanvas(c_RooPlotTest);
      //c_RooPlotTest->cd();
      //dw23Frame->Draw();
    }
  }
  SbrWholeEtaRange << "# arm charge total_events sig_evts err_sig_events_hi err_sig_evts_lo had_evts err_had_evts_hi err_had_evts_lo mubak_evts sbr err_sbr_hi err_sbr_lo" << std::endl;
  std::ofstream sbrOutFile(sbrCSVName.c_str());
  // Header
  sbrOutFile 
      << "\"arm\","
      << "\"charge\","
      << "\"total muons\","
      << "\"w sig muons\","
      << "\"err hi w sig muons\","
      << "\"err lo w sig muons\","
      << "\"had bak muons\","
      << "\"err hi had bak muons\","
      << "\"err lo had bak muons\","
      << "\"muon background muons\","
      << "\"SBR\","
      << "\"err hi SBR\","
      << "\"err lo SBR\""
      << std::endl;

  for(int arm_i = 0; arm_i < NUMBER_OF_ARM_CONFIGS; arm_i++)
  {
    for(int charge_i = 0; charge_i < NUMBER_OF_CHARGE_CONFIGS; charge_i++)
    {
      std::cout << "Arm: " << arm_i << " Charge: " << charge_i << std::endl;
      std::cout << "----------------------------------------------------" << std::endl;
      std::cout << "   Hadron Backgrd  Count: " << Num_HadBak_Muons[arm_i][charge_i]<< std::endl;
      std::cout << "   Muon Background Count: " << Num_MuBak_Muons [arm_i][charge_i]<< std::endl;
      std::cout << "   Signal Muons    Count: " << Num_WSig_Muons   [arm_i][charge_i]<< std::endl;
      std::cout << " Signal to Background Ratio: " << SBR[arm_i][charge_i] << std::endl;
      std::cout << std::endl;

      int total = (Num_HadBak_Muons[arm_i][charge_i] + Num_MuBak_Muons[arm_i][charge_i] + Num_WSig_Muons[arm_i][charge_i]);
      SbrWholeEtaRange << arm_i 
          << " " <<  charge_i 
          << " " <<  total
          << " " <<  Num_WSig_Muons[arm_i][charge_i]
          << " " <<  Err_Hi_WSig_Muons[arm_i][charge_i]
          << " " <<  Err_Lo_WSig_Muons[arm_i][charge_i]
          << " " <<  Num_HadBak_Muons[arm_i][charge_i]
          << " " <<  Err_Hi_HadBak_Muons[arm_i][charge_i]
          << " " <<  Err_Lo_HadBak_Muons[arm_i][charge_i]
          << " " <<  Num_MuBak_Muons[arm_i][charge_i]
          << " " <<  SBR[arm_i][charge_i]
          << " " <<  Err_Hi_SBR[arm_i][charge_i]
          << " " <<  Err_Lo_SBR[arm_i][charge_i]
          << std::endl;
      /*
      sbrOutFile <<  "\"" << arm_i << "\"," 
          << "\"" <<  charge_i                             << "\","
          << "\"" <<  total                                << "\","
          << "\"" <<  Num_WSig_Muons[arm_i][charge_i]      
          << " +" <<  Err_Hi_WSig_Muons[arm_i][charge_i]   
          << " "  <<  Err_Lo_WSig_Muons[arm_i][charge_i]   << "\","
          << "\"" <<  Num_HadBak_Muons[arm_i][charge_i]    
          << " +" <<  Err_Hi_HadBak_Muons[arm_i][charge_i] 
          << " "  <<  Err_Lo_HadBak_Muons[arm_i][charge_i] << "\","
          << "\"" <<  Num_MuBak_Muons[arm_i][charge_i]     << "\","
          << "\"" <<  SBR[arm_i][charge_i]                 
          << " +" <<  Err_Hi_SBR[arm_i][charge_i]          
          << " -"  <<  Err_Lo_SBR[arm_i][charge_i]          << "\""
          << std::endl;
      */
      sbrOutFile 
          << "\"" << arm_i                                << "\","
          << "\"" << charge_i                             << "\","
          << "\"" << total                                << "\","
          << "\"" << Num_WSig_Muons[arm_i][charge_i]      << "\","
          << "\"" << Err_Hi_WSig_Muons[arm_i][charge_i]   << "\","
          << "\"" << Err_Lo_WSig_Muons[arm_i][charge_i]   << "\","
          << "\"" << Num_HadBak_Muons[arm_i][charge_i]    << "\","
          << "\"" << Err_Hi_HadBak_Muons[arm_i][charge_i] << "\","
          << "\"" << Err_Lo_HadBak_Muons[arm_i][charge_i] << "\","
          << "\"" << Num_MuBak_Muons[arm_i][charge_i]     << "\","
          << "\"" << SBR[arm_i][charge_i]                 << "\","
          << "\"" << Err_Hi_SBR[arm_i][charge_i]          << "\","
          << "\"" << Err_Lo_SBR[arm_i][charge_i]          << "\""
          << std::endl;
    }
  }
  SbrWholeEtaRange.close();
  return 0;
}
