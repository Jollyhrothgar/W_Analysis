#include "dw23VsWness.h"
dw23VsWness::dw23VsWness()
{
  if(showDiagnostics) std::cout << "dw23VsWness" << std::endl;

  plotHadronicExtrapolation = false;
  showDiagnostics = false;
  dataTreeFile = "";
  histogramsFile = "";

  // Setup Default Wness Binning
  overrideDefaultWnessBinning = false;

  // assume abs(eta)
  eta_min = 1.1;
  eta_max = 2.6;
  eta_bins = SBR_PDF_BINS;
  dw23_min = -0.3;
  dw23_max = 0.3;
  pdf_dw23_min = -0.1;
  pdf_dw23_max = 0.1;
  dw23_bins = SBR_PDF_BINS;

  treeName = wness_tree.c_str();

  setDestruct    = true;
  readHistos     = false;
  histoFile      = NULL;

  NumCoaxial2DGaussPars = 13;

  NUMBER_OF_WNESS_BINS = 5;
  TOTAL_WNESS_BINS = NUMBER_OF_WNESS_BINS+1;
  WNESS_UPPER = 0.9;
  WNESS_LOWER = 0.1;

  WNESS_RANGE = WNESS_UPPER - WNESS_LOWER;
  WNESS_STEP = WNESS_RANGE/NUMBER_OF_WNESS_BINS;
  WnessHistoBins = 100;

  treeAlreadyInitialized = false;
  tracks = NULL;

  smoothing = SBR_SMOOTHING;

  nMuBakMuons[0][0] = 0.0;
  nMuBakMuons[1][0] = 0.0;
  nMuBakMuons[1][1] = 0.0;
  nMuBakMuons[0][1] = 0.0;
  
  // UUUUGGGGLLLYYYY.....
  MuIndex["dy"        ] = DIRECT_PHOTON_INDEX ;
  MuIndex["onium"     ] = ONIUM_INDEX         ;
  MuIndex["onlyz"     ] = ONLY_Z_INDEX        ;
  MuIndex["openbottom"] = OPEN_BOTTOM_INDEX   ;
  MuIndex["opencharm" ] = OPEN_CHARM_INDEX    ;
  MuIndex["whad"      ] = W_HAD_INDEX         ;
  MuIndex["wtau"      ] = W_TAU_INDEX         ;
  MuIndex["z"         ] = Z_INDEX             ;

  NewPdfFileName = baseDirectory+macrosDirectory+rootTreesDirectory+"Run13SbrPdfs.root"; 

  pdfSelect = -1;

  MuonBackgroundLog.open("/direct/phenix+spin2/beaumim/WAnalysisRun13/AnalysisCode/logs/MuonBackgroundLog.txt");
  MuonBackgroundLog << "Summary of Fitting Information" << std::endl;
}

int dw23VsWness::SetPDFSmoothing(int smooth)
{
  smoothing = smooth;
  return smoothing;
}

int dw23VsWness::SetPDFOutputFile(std::string file)
{
  NewPdfFileName = file;
  return 0;
}
int dw23VsWness::ShowHadronicExtrapolationPlots()
{
  plotHadronicExtrapolation = true;
  return 0;
}

int dw23VsWness::SetHighVerbosity()
{
  showDiagnostics = true;
  return 0;
}

int dw23VsWness::SelectPdfGroup(int pdfSelection)
{
  pdfSelect = pdfSelection;
  return pdfSelection;
}
int dw23VsWness::dw23Extrapolate_LoopAndFit(std::string treeFile, std::string histoFile)
{
  // Initial Setup
  SetupWnessBins();
  SetDataTreeFile(treeFile);
  SetAnalysisObjectOutputFile(histoFile);
  DataType = BACKGROUND_INDEX; // used in MakeAnalysisObjects

  // Prepare containers, and define each container with new objects
  PrepareAnalysisObjects();
  MakeAnalysisObjects();

  // Loop over the tree file and fill histograms
  Loop();

  // Fit dw23VsWness
  RunFitter();
  MakePlots();

  return 0;
}


// Wness Bins should be able to be defined from the histogram stored in the root file.
int dw23VsWness::dw23Extrapolate_ReadAndFit(std::string treeFile, std::string histoFile)
{
  SetupWnessBins();
  SetDataTreeFile(treeFile);
  SetAnalysisObjectOutputFile(histoFile);
  DataType = BACKGROUND_INDEX; // used in MakeAnalysisObjects

  PrepareAnalysisObjects();
  ReadHistograms();

  if(showDiagnostics)
  {
    for(unsigned int i = 0; i < Registry.size(); i++)
    {
      std::cout << "Found: " << Registry[i]->GetName() << std::endl;
    }
  }
  // Fit dw23VsWness
  RunFitter();
  MakePlots();
  return 0;
}

int dw23VsWness::SetNoDestruct()
{
  setDestruct = false;
  return 0;
}

dw23VsWness::~dw23VsWness()
{
  //Pause();
  MuonBackgroundLog.close();
  if(showDiagnostics) std::cout << "dw23VsWness::~dw23VsWness" << std::endl;
  if(setDestruct)
  {
    for(unsigned int i = 0; i < Registry.size(); i++)
    {
      if(Registry[i]) delete Registry[i];
    }
    for(unsigned int i = 0; i < CanvasRegistry.size(); i++)
    {
      delete CanvasRegistry[i];
    }
    delete histoFile;
    delete PdfOutFile;
    delete reducedTreeFile;
  }
}

int dw23VsWness::SetDataTreeFile(std::string  dataTreeFileName )
{
  dataTreeFile  = dataTreeFileName;
  std::cout << "Data Tree File is:  " << dataTreeFile << std::endl;
  return 0;
}

int dw23VsWness::SaveCanvases(std::string directory)
{
  for(unsigned int i = 0; i < CanvasRegistry.size(); i++)
  {
    if(!CanvasRegistry[i])
    {
      std::cout << "Undefined canvas in the CanvasRegistry at entry: " << i << std::endl;
      return -1;
    }
    std::string name = CanvasRegistry[i]->GetName();
    name = directory + name + ".png";
    CanvasRegistry[i]->SaveAs(name.c_str());
  }
  return 0;
}

int dw23VsWness::SetAnalysisObjectOutputFile(std::string outputFile)
{
  histogramsFile = baseDirectory + macrosDirectory + rootTreesDirectory + outputFile;
  std::cout << "output file set to: " << histogramsFile << std::endl;
  return 0;
}

int dw23VsWness::SetWnessBinning(float min_range, float max_range, int bins)
{
  std::cout << "Using User-Defined Wness Binning" << std::endl;
  overrideDefaultWnessBinning = true;
  NUMBER_OF_WNESS_BINS = bins;
  TOTAL_WNESS_BINS = NUMBER_OF_WNESS_BINS+1;
  WNESS_UPPER = max_range;
  WNESS_LOWER = min_range;
  WNESS_RANGE = WNESS_UPPER-WNESS_LOWER;
  WNESS_STEP = WNESS_RANGE/NUMBER_OF_WNESS_BINS;

  return 0;
}

int dw23VsWness::SetupWnessBins()
{
  float wness_low;
  float wness_high;
  if(overrideDefaultWnessBinning)
  {
    std::cout << "  Overriding Default Wness Binning" << std::endl;
    WnessBins.resize(0);
    WnessBins.push_back( std::make_pair( 0.0, 1.0 ));
    wness_low = WNESS_LOWER;
    for(int wness_bin = 1; wness_bin < TOTAL_WNESS_BINS; wness_bin++)
    {
      wness_high = wness_low+WNESS_STEP;
      WnessBins.push_back( std::make_pair(wness_low, wness_high));
      std::cout << "  WnessBin# " << std::setw(3) << wness_bin
          << ": [" << std::setw(6) << std::setprecision(3) << WnessBins.back().first << ", "
          << std::setw(6) << std::setprecision(3) << WnessBins.back().second << "]" << std::endl;
      wness_low = wness_high;
    }
    return 1;
  }
  else
  {
    WnessBins.push_back( std::make_pair( 0.0, 1.0 ));
    WnessBins.push_back( std::make_pair( 0.1, 0.3 ) );
    WnessBins.push_back( std::make_pair( 0.3, 0.5 ) );
    WnessBins.push_back( std::make_pair( 0.5, 0.7 ) );
    WnessBins.push_back( std::make_pair( 0.7, 0.9 ) );
    WnessBins.push_back( std::make_pair( 0.92, 1.0 ) ); // extrapolation region
    for(unsigned int wness_bin = 1; wness_bin < WnessBins.size(); wness_bin++)
    {
      std::cout << "  WnessBin# " << std::setw(3) << wness_bin
          << ": [" << std::setw(5) << std::setprecision(3) << WnessBins[wness_bin].first << ", "
          << std::setw(5) << std::setprecision(3) << WnessBins[wness_bin].second << "]" << std::endl;
    }
    return 2;
  }
  return -1;
}


int dw23VsWness::RunFitter()
{
  // Double gaussian fitting steps:
  // 1. Make coax-gaussian
  // 2. Set parameters based on distribution to fit
  // 3. Fix parameters of double gauss using pol4 from Wness
  // 4. Set Par limits for double-gauss fit
  // 5. Apply fit
  // 6. Extract parameters.

  // Double Gaussian function
  // width portion
  // polynomial wness matching portion
  ApplyNormalization();
  c_WnessVsdw23 = new TCanvas("c_WnessVsdw23","c_WnessVsdw23",1200, 900);
  c_WnessVsdw23->Divide(2,2);
  int canvas_pick = 1;
  for(int arm_i = 0; arm_i < NUMBER_OF_ARM_CONFIGS; arm_i++)
  {
    for(int charge_i = 0; charge_i < NUMBER_OF_CHARGE_CONFIGS; charge_i++)
    {
      c_WnessVsdw23->cd(canvas_pick);
      h_dw23VsWness[arm_i][charge_i]->SetStats(false);
      h_dw23VsWness[arm_i][charge_i]->Draw();

      std::cout << "Fitting 1D: " << h_Wness_Fit_Seed[arm_i][charge_i]->GetTitle() << std::endl;
      h_Wness_Fit_Seed[arm_i][charge_i]->Fit(f_Wness_Seed[arm_i][charge_i], "");

      // Parameter Meaning:
      // [0] x-offset
      // [1] x-offset slope (linear Wness dependance)
      // [2] sigma_1
      // [3] sigma_1 slope (linear Wness dependance)
      // [4] sigma_2
      // [5] sigma_2 slope (linear Wness dependance)
      // [6] gauss_factor - scaling of the second gaussian
      // [7] gauss_factor slope (linear Wness dependance)
      // Daniel Option
      f2_double_gaus_2d[arm_i][charge_i]->SetParameter(0, h_dw23[arm_i][charge_i]->GetMean());// mean(offset) constant
      f2_double_gaus_2d[arm_i][charge_i]->SetParameter(1, (charge_i?-1:1)*0.0005 );  // mean(offset) slope -- daniel
      f2_double_gaus_2d[arm_i][charge_i]->SetParameter(2, 0.1  );  // sigma1 constant
      f2_double_gaus_2d[arm_i][charge_i]->SetParameter(3,-0.006  );  // sigma1 slope
      f2_double_gaus_2d[arm_i][charge_i]->SetParameter(4, 0.04 );  // sigma2 constant
      f2_double_gaus_2d[arm_i][charge_i]->SetParameter(5,-0.01 );  // sigma2 slope
      f2_double_gaus_2d[arm_i][charge_i]->SetParameter(6, 1.0  );  // constant(factor) constant
      f2_double_gaus_2d[arm_i][charge_i]->SetParameter(7, 0.0  );  // constant(factor) slope -- daniel
      f2_double_gaus_2d[arm_i][charge_i]->FixParameter(8, f_Wness_Seed[arm_i][charge_i]->GetParameter(0));
      f2_double_gaus_2d[arm_i][charge_i]->FixParameter(9, f_Wness_Seed[arm_i][charge_i]->GetParameter(1));
      f2_double_gaus_2d[arm_i][charge_i]->FixParameter(10,f_Wness_Seed[arm_i][charge_i]->GetParameter(2));
      f2_double_gaus_2d[arm_i][charge_i]->FixParameter(11,f_Wness_Seed[arm_i][charge_i]->GetParameter(3));
      f2_double_gaus_2d[arm_i][charge_i]->FixParameter(12,f_Wness_Seed[arm_i][charge_i]->GetParameter(4));
      f2_double_gaus_2d[arm_i][charge_i]->SetParLimits(2, 0.09,1   );
      f2_double_gaus_2d[arm_i][charge_i]->SetParLimits(4, 0   ,0.09);

      std::cout << std::endl;
      std::cout << "Fitting 2D: " << h_dw23VsWness[arm_i][charge_i]->GetTitle() << std::endl;
      h_dw23VsWness[arm_i][charge_i]->Fit(f2_double_gaus_2d[arm_i][charge_i]->GetName(),"L");
      canvas_pick++;

    }
  }
  RegisterCanvas(c_WnessVsdw23);
  return 0;
}

int dw23VsWness::MakePlots()
{
  c_dw23Slices = new TCanvas("c_dw23WnessSlices","dw23 sliced in each Wness Bin evalulated at mean Wness Value",1200,900);
  c_dw23SlicesCheck = new TCanvas("c_dw23WnessSlices_Check","dw23 slices fitted individually",1200,900);
  c_WnessLinearCheck = new TCanvas("c_WnessLinearCheck","Check if coax gaus terms are really linear in Wness",900,1200);

  RegisterCanvas(c_dw23Slices);
  RegisterCanvas(c_dw23SlicesCheck);
  RegisterCanvas(c_WnessLinearCheck);


  int wness_bins_size = WnessBins.size();
  c_dw23Slices->Divide(wness_bins_size,4);
  c_dw23SlicesCheck->Divide(wness_bins_size,4);
  int canvas_bin = 1;
  int fit_check_bin = 1;

  c_WnessFit1D = new TCanvas("c_WnessFit1D","pol4 fit to Wness 1D spectrum", 900,900);
  c_WnessFit1D->Divide(2,2);
  c_WnessFit1D->cd(1);
  h_Wness_Fit_Seed[0][0]->Draw();
  c_WnessFit1D->cd(2);
  h_Wness_Fit_Seed[0][1]->Draw();
  c_WnessFit1D->cd(3);
  h_Wness_Fit_Seed[1][0]->Draw();
  c_WnessFit1D->cd(4);
  h_Wness_Fit_Seed[1][1]->Draw();
  RegisterCanvas(c_WnessFit1D);

  // Parameter Summary
  std::cout << "***PARAMETER SUMMARY***" << std::endl;
  std::string csvName = "";
  const int num_bins = NUMBER_OF_WNESS_BINS;
  const int num_check_bins = num_bins-1;
  c_WnessLinearCheck->Divide(4,4);

  TF1* coaxialGaussian     [NUMBER_OF_ARM_CONFIGS][NUMBER_OF_CHARGE_CONFIGS][num_bins];
  TF1* coaxialGaussianCheck[NUMBER_OF_ARM_CONFIGS][NUMBER_OF_CHARGE_CONFIGS][num_check_bins];
  TF1* linearFit        [4][NUMBER_OF_ARM_CONFIGS][NUMBER_OF_CHARGE_CONFIGS];

  TGraphErrors* sig1VsWness[NUMBER_OF_ARM_CONFIGS][NUMBER_OF_CHARGE_CONFIGS];
  TGraphErrors* sig2VsWness[NUMBER_OF_ARM_CONFIGS][NUMBER_OF_CHARGE_CONFIGS];
  TGraphErrors* gFacVsWness[NUMBER_OF_ARM_CONFIGS][NUMBER_OF_CHARGE_CONFIGS];
  TGraphErrors* gOffVsWness[NUMBER_OF_ARM_CONFIGS][NUMBER_OF_CHARGE_CONFIGS];

  for(int arm_i = 0; arm_i < NUMBER_OF_ARM_CONFIGS; arm_i++)
  {
    for(int charge_i = 0; charge_i < NUMBER_OF_CHARGE_CONFIGS; charge_i++)
    {
      std::string name_stub;
      std::string title_stub;

      std::string name = "";
      std::string title = "";

      MakeNameTitleStubPair(arm_i, charge_i, BACKGROUND_INDEX, name_stub, title_stub);

      sig1VsWness[arm_i][charge_i]= new TGraphErrors();  sig1VsWness[arm_i][charge_i]->SetMarkerStyle(kFullCircle);
      sig2VsWness[arm_i][charge_i]= new TGraphErrors();  sig2VsWness[arm_i][charge_i]->SetMarkerStyle(kFullCircle);
      gFacVsWness[arm_i][charge_i]= new TGraphErrors();  gFacVsWness[arm_i][charge_i]->SetMarkerStyle(kFullCircle);
      gOffVsWness[arm_i][charge_i]= new TGraphErrors();  gOffVsWness[arm_i][charge_i]->SetMarkerStyle(kFullCircle);

      name  = "sig1VsWness_"+name_stub;
      title = "#sigma_{1} vs W_{ness} " + title_stub + ";W_{ness};#sigma_{1}";
      sig1VsWness[arm_i][charge_i]->SetName(name.c_str());  sig1VsWness[arm_i][charge_i]->SetTitle(title.c_str());
      name = name + "_fit"; linearFit[0][arm_i][charge_i] = new TF1(name.c_str(), "pol1", 0, 1);

      name  = "sig2VsWness_"+name_stub;
      title = "#sigma_{2} vs W_{ness} " + title_stub + ";W_{ness};#sigma_{2}";
      sig2VsWness[arm_i][charge_i]->SetName(name.c_str());  sig2VsWness[arm_i][charge_i]->SetTitle(title.c_str());
      name = name + "_fit"; linearFit[1][arm_i][charge_i] = new TF1(name.c_str(), "pol1", 0, 1);

      name  = "gFacVsWness_"+name_stub;
      title = "Gaus Factor vs W_{ness} " + title_stub + ";W_{ness};Gaus Factor";
      gFacVsWness[arm_i][charge_i]->SetName(name.c_str());  gFacVsWness[arm_i][charge_i]->SetTitle(title.c_str());
      name = name + "_fit"; linearFit[2][arm_i][charge_i] = new TF1(name.c_str(), "pol1", 0, 1);

      name = "gOffVsWness_"+name_stub;
      title = "Gaus Offset Vs W_{ness} " + title_stub + ";W_{ness};Gaus Offset";
      gOffVsWness[arm_i][charge_i]->SetName(name.c_str()); gOffVsWness[arm_i][charge_i]->SetTitle(title.c_str());
      name = name + "_fit"; linearFit[3][arm_i][charge_i] = new TF1(name.c_str(), "pol1", 0, 1);
    }
  }

  for(int arm_i = 0; arm_i < NUMBER_OF_ARM_CONFIGS; arm_i++)
  {
    for(int charge_i = 0; charge_i < NUMBER_OF_CHARGE_CONFIGS; charge_i++)
    {

      for(int wness_bin = 0; wness_bin < TOTAL_WNESS_BINS; wness_bin++)
      {
        std::stringstream name;
        name << "_Arm" << arm_i << "_Charge" << charge_i << "_WnessBin" << wness_bin;
        const std::string name_stub = name.str();
        name.str("");

        h_dw23Wness[arm_i][charge_i][wness_bin]->SetStats(false);
        if( wness_bin != static_cast<int>(WnessBins.size() -1 ) )
        {
          c_dw23Slices->cd(canvas_bin);
          h_dw23Wness[arm_i][charge_i][wness_bin]->Draw();
          c_dw23SlicesCheck->cd(canvas_bin);
          h_dw23Wness[arm_i][charge_i][wness_bin]->Draw();
        }
        else
        {
          h_dw23Wness[arm_i][charge_i][wness_bin]->SetMarkerColor(kWhite);
          h_dw23Wness[arm_i][charge_i][wness_bin]->SetLineColor(kWhite);
          c_dw23Slices->cd(canvas_bin);
          h_dw23Wness[arm_i][charge_i][wness_bin]->Draw();
          c_dw23SlicesCheck->cd(canvas_bin);
          h_dw23Wness[arm_i][charge_i][wness_bin]->Draw();
        }

        double WnessAverage = h_WnessBin[arm_i][charge_i][wness_bin]->GetMean();
        if(wness_bin > 0)
        {
          // Parameters to set to this generalized coaxial gaussian.
          // [0] -> (1/(gaus_sigma1*sqrt(2*pi)+gaus_factor*gaus_sigma2*sqrt(2*pi)))
          // [1] -> gauss_offset
          // [2] -> sigma_1
          // [3] -> gaus_factor
          // [4] -> sigma_2
          // use a function to get these parameters from the TF2, at a specific Wness.
          name << "coaxialGaussian" << name_stub;
          coaxialGaussian[arm_i][charge_i][wness_bin] = new TF1(
              name.str().c_str(),
              "[0]*(exp(-0.5*pow((x-[1])/[2],2))+[3]*exp(-0.5*pow((x-[1])/[4],2)))",
              dw23_min, dw23_max);
          name.str("");

          // Parameters of the 1D coaxial gaussian are set here, by extracting from the 2D fit.
          coaxialGaussian[arm_i][charge_i][wness_bin]->FixParameter(0,GetParameter(0,WnessAverage,arm_i,charge_i));
          coaxialGaussian[arm_i][charge_i][wness_bin]->FixParameter(1,GetParameter(1,WnessAverage,arm_i,charge_i));
          coaxialGaussian[arm_i][charge_i][wness_bin]->FixParameter(2,GetParameter(2,WnessAverage,arm_i,charge_i));
          coaxialGaussian[arm_i][charge_i][wness_bin]->FixParameter(3,GetParameter(3,WnessAverage,arm_i,charge_i));
          coaxialGaussian[arm_i][charge_i][wness_bin]->FixParameter(4,GetParameter(4,WnessAverage,arm_i,charge_i));

          if(wness_bin <= num_check_bins)
          {
            name << "coaxialGaussianCheck" << name_stub; 
            coaxialGaussianCheck[arm_i][charge_i][wness_bin] = new TF1(
                name.str().c_str(),
                "[0]*(exp(-0.5*pow((x-[1])/[2],2))+[3]*exp(-0.5*pow((x-[1])/[4],2)))",
                dw23_min, dw23_max);
            coaxialGaussianCheck[arm_i][charge_i][wness_bin]->SetParameter(0,GetParameter(0,WnessAverage,arm_i,charge_i));
            coaxialGaussianCheck[arm_i][charge_i][wness_bin]->SetParameter(1,GetParameter(1,WnessAverage,arm_i,charge_i));
            coaxialGaussianCheck[arm_i][charge_i][wness_bin]->SetParameter(2,GetParameter(2,WnessAverage,arm_i,charge_i));
            coaxialGaussianCheck[arm_i][charge_i][wness_bin]->SetParameter(3,GetParameter(3,WnessAverage,arm_i,charge_i));
            coaxialGaussianCheck[arm_i][charge_i][wness_bin]->SetParameter(4,GetParameter(4,WnessAverage,arm_i,charge_i));
            coaxialGaussianCheck[arm_i][charge_i][wness_bin]->SetLineColor(kGreen);
            h_dw23Wness[arm_i][charge_i][wness_bin]->Fit(coaxialGaussianCheck[arm_i][charge_i][wness_bin], "");

            sig1VsWness[arm_i][charge_i]->SetPoint(sig1VsWness[arm_i][charge_i]->GetN(), WnessAverage, coaxialGaussianCheck[arm_i][charge_i][wness_bin]->GetParameter(2));
            sig1VsWness[arm_i][charge_i]->SetPointError(sig1VsWness[arm_i][charge_i]->GetN()-1, 0.0, coaxialGaussianCheck[arm_i][charge_i][wness_bin]->GetParError(2));
            sig2VsWness[arm_i][charge_i]->SetPoint(sig2VsWness[arm_i][charge_i]->GetN(), WnessAverage, coaxialGaussianCheck[arm_i][charge_i][wness_bin]->GetParameter(4));
            sig2VsWness[arm_i][charge_i]->SetPointError(sig2VsWness[arm_i][charge_i]->GetN()-1, 0.0, coaxialGaussianCheck[arm_i][charge_i][wness_bin]->GetParError(4));
            gFacVsWness[arm_i][charge_i]->SetPoint(gFacVsWness[arm_i][charge_i]->GetN(), WnessAverage, coaxialGaussianCheck[arm_i][charge_i][wness_bin]->GetParameter(3));
            gFacVsWness[arm_i][charge_i]->SetPointError(gFacVsWness[arm_i][charge_i]->GetN()-1, 0.0, coaxialGaussianCheck[arm_i][charge_i][wness_bin]->GetParError(3));
            gOffVsWness[arm_i][charge_i]->SetPoint(gOffVsWness[arm_i][charge_i]->GetN(), WnessAverage, coaxialGaussianCheck[arm_i][charge_i][wness_bin]->GetParameter(1));
            gOffVsWness[arm_i][charge_i]->SetPointError(gOffVsWness[arm_i][charge_i]->GetN()-1, 0.0, coaxialGaussianCheck[arm_i][charge_i][wness_bin]->GetParError(1));
          }

          // Need to force the integral of the function to be the same as the integral of the fit.
          double function_integral = coaxialGaussian[arm_i][charge_i][wness_bin]->Integral(dw23_min, dw23_max);
          double histogram_integral = h_dw23Wness[arm_i][charge_i][wness_bin-1]->Integral("width");
          if(showDiagnostics)
          {
            if( wness_bin == static_cast<int>(WnessBins.size() - 1))
            {
              std::cout << "EXTRAPOLATED VALUES" << std::endl;
              std::cout << "arm_" << arm_i << ", charge" << charge_i << ", wnessbin [" << WnessBins[wness_bin].first << ", " << WnessBins[wness_bin].second << "]" << std::endl
                  << "\tfunction_integral: " << function_integral << std::endl
                  << "\thistogram_integral: " << histogram_integral << std::endl
                  << "\tbin central value: " << WnessAverage << std::endl << std::endl
                  << "\tscale       p0: " << GetParameter(0,WnessAverage,arm_i,charge_i) << std::endl
                  << "\toffset      p1: " << GetParameter(1,WnessAverage,arm_i,charge_i) << std::endl
                  << "\tsigma_1     p2: " << GetParameter(2,WnessAverage,arm_i,charge_i) << std::endl
                  << "\tsigma_2     p4: " << GetParameter(4,WnessAverage,arm_i,charge_i) << std::endl
                  << "\tgaus_factor p3: " << GetParameter(3,WnessAverage,arm_i,charge_i) << std::endl
                  << std::endl;
            }
          }
          c_dw23Slices->cd(canvas_bin);
          coaxialGaussian[arm_i][charge_i][wness_bin]->Draw("same");
          c_dw23SlicesCheck->cd(canvas_bin);
          if(wness_bin <= num_check_bins)
          {
            coaxialGaussianCheck[arm_i][charge_i][wness_bin]->Draw("same");
          }
        }
        canvas_bin++;
      }
    }
  }

  // Store extrapolated shapes - "NUMBER_OF_WNESS_BINS" corresponds to extrapolation bin
  // NUMBER_OF_WNESS_BINS is the extrapolation bin
  f_dw23_HadBak_pdfSeed[0][0] = coaxialGaussian[0][0][NUMBER_OF_WNESS_BINS];
  f_dw23_HadBak_pdfSeed[1][0] = coaxialGaussian[1][0][NUMBER_OF_WNESS_BINS];
  f_dw23_HadBak_pdfSeed[0][1] = coaxialGaussian[0][1][NUMBER_OF_WNESS_BINS];
  f_dw23_HadBak_pdfSeed[1][1] = coaxialGaussian[1][1][NUMBER_OF_WNESS_BINS];
  // for Francesca's parameter comparison
  ofstream ParamXCheck("/direct/phenix+u/beaumim/W_Analysis/logs/FrancescaDw23ExtrapXCheck.txt");
  std::cout << "CrossCheck Parameters: " << std::endl;
  for(int arm_i = 0; arm_i < NUMBER_OF_ARM_CONFIGS; arm_i++)
  {
    for(int charge_i = 0; charge_i < NUMBER_OF_CHARGE_CONFIGS; charge_i++)
    {
      c_WnessLinearCheck->cd(fit_check_bin);
      sig1VsWness[arm_i][charge_i]->Draw("AP");
      sig1VsWness[arm_i][charge_i]->Fit(linearFit[0][arm_i][charge_i]->GetName(),"Q");
      linearFit[0][arm_i][charge_i]->Draw("same");
      fit_check_bin++;

      c_WnessLinearCheck->cd(fit_check_bin);
      sig2VsWness[arm_i][charge_i]->Draw("AP");
      sig2VsWness[arm_i][charge_i]->Fit(linearFit[1][arm_i][charge_i]->GetName(),"Q");
      linearFit[1][arm_i][charge_i]->Draw("same");
      fit_check_bin++;

      c_WnessLinearCheck->cd(fit_check_bin);
      gFacVsWness[arm_i][charge_i]->Draw("AP");
      gFacVsWness[arm_i][charge_i]->Fit(linearFit[2][arm_i][charge_i]->GetName(),"Q");
      linearFit[2][arm_i][charge_i]->Draw("same");
      fit_check_bin++;

      c_WnessLinearCheck->cd(fit_check_bin);
      gOffVsWness[arm_i][charge_i]->Draw("AP");
      gOffVsWness[arm_i][charge_i]->Fit(linearFit[3][arm_i][charge_i]->GetName(),"Q");
      linearFit[3][arm_i][charge_i]->Draw("same");
      fit_check_bin++;

      float f_arm, f_charge, offset, offsetsl, sig1, sig1sl, sig2, sig2sl, fact, facts;

      f_arm = static_cast<float>(arm_i);
      f_charge = static_cast<float>(charge_i);
      offset   = f2_double_gaus_2d[arm_i][charge_i]->GetParameter(0);
      offsetsl = f2_double_gaus_2d[arm_i][charge_i]->GetParameter(1);
      sig1     = f2_double_gaus_2d[arm_i][charge_i]->GetParameter(2);
      sig1sl   = f2_double_gaus_2d[arm_i][charge_i]->GetParameter(3);
      sig2     = f2_double_gaus_2d[arm_i][charge_i]->GetParameter(4);
      sig2sl   = f2_double_gaus_2d[arm_i][charge_i]->GetParameter(5);
      fact     = f2_double_gaus_2d[arm_i][charge_i]->GetParameter(6);
      facts    = f2_double_gaus_2d[arm_i][charge_i]->GetParameter(7);

      ParamXCheck <<  f_arm << " " << f_charge << " " << offset << " "
          << offsetsl << " " << sig1 << " " << sig1sl << " "
          << sig2 << " " << sig2sl << " " << fact << " " << facts
          << std::endl;
    }
  }
  return 0;
}

double dw23VsWness::GetParameter(int par, double Wness, int arm_i, int charge_i)
{
  double par_value = 0.0;
  // extract all parameters
  std::vector<double> p;
  for(int i = 0; i < 13; i++)
  {
    p.push_back(f2_double_gaus_2d[arm_i][charge_i]->GetParameter(i));
  }

  double gaus_offset = p[0] + p[1]*Wness;
  double gaus_sigma1 = p[2] + p[3]*Wness;
  double gaus_sigma2 = p[4] + p[5]*Wness;
  double gaus_factor = p[6] + p[7]*Wness;
  double pi; pi = 3.14159265358979323846;
  double par0 =  ( 1/(gaus_sigma1*sqrt(2*pi)+ gaus_factor*gaus_sigma2*sqrt(2*pi)) );

  switch(par)
  {
    case 0:
      par_value = par0;
      break;
    case 1:
      par_value = gaus_offset;
      break;
    case 2:
      par_value = gaus_sigma1;
      break;
    case 3:
      par_value = gaus_factor;
      break;
    case 4:
      par_value = gaus_sigma2;
      break;
    default:
      std::cout << "Parameter " << par << " unknown" << std::endl;
      par_value = 0.0;
  }
  return par_value;
}

double double_gaus_2d(double* x, double* par)
{
  // x[0] -> Wness
  // x[1] -> dw23
  double gaus_offset = par[0] + par[1]*x[0];
  double gaus_sigma1 = par[2] + par[3]*x[0]; //should be wider
  double gaus_sigma2 = par[4] + par[5]*x[0]; //should be narrower
  double gaus_factor = par[6] + par[7]*x[0];
  double pi; pi = 3.14159265358979323846;

  // polynomial describes the normalized 1D wness distribution.
  // used as a weight factor for the otherwise normalized dw23 distribution and different regions of wness
  double polynomial_factor = par[8] + par[9]*x[0] + par[10]*pow(x[0],2) + par[11]*pow(x[0],3) + par[12]*pow(x[0],4);

  double val = polynomial_factor *
      ( 1/(gaus_sigma1*sqrt(2*pi)+ gaus_factor*gaus_sigma2*sqrt(2*pi)) ) *
      ( exp(-0.5*pow((x[1]-gaus_offset)/gaus_sigma1,2)) + gaus_factor*exp(-0.5*pow((x[1]-gaus_offset)/gaus_sigma2,2)) );
  return val;
}

int dw23VsWness::ApplyNormalization()
{
  for(int arm_i = 0; arm_i < NUMBER_OF_ARM_CONFIGS; arm_i++)
  {
    for(int charge_i = 0; charge_i < NUMBER_OF_CHARGE_CONFIGS; charge_i++)
    {
      double integral_value = h_dw23VsWness[arm_i][charge_i]->Integral("width");
      if(fabs(integral_value) < 0.001 )
      {
        std::cout << "BAD NORMALIZATION: " << integral_value << " for "
            << h_dw23VsWness[arm_i][charge_i]->GetName() << ", " << h_dw23VsWness[arm_i][charge_i]->GetTitle()
            << std::endl;
        return -1;
      }
      h_dw23VsWness[arm_i][charge_i]->Scale(1.0/integral_value); // Normalized

      integral_value = h_Wness_Fit_Seed[arm_i][charge_i]->Integral("width");
      if(fabs(integral_value) < 0.001 )
      {
        std::cout << "BAD NORMALIZATION: " << integral_value << " for "
            << h_Wness_Fit_Seed[arm_i][charge_i]->GetName()
            << std::endl;
        return -1;
      }
      h_Wness_Fit_Seed[arm_i][charge_i]->Scale(1.0/integral_value);

      for(int wness_bin = 0; wness_bin < TOTAL_WNESS_BINS; wness_bin++)
      {
        integral_value = h_dw23Wness[arm_i][charge_i][wness_bin]->Integral("width");
        if(fabs(integral_value) < 0.001)
        {
          std::cout << "BAD NORMALIZATION: " << integral_value << " for "
              << h_Wness_Fit_Seed[arm_i][charge_i]->GetName()
              << std::endl;
          return -1;

        }
        h_dw23Wness[arm_i][charge_i][wness_bin]->Scale(1.0/integral_value);
      }
    }
  }
  return 0;
}

int dw23VsWness::Write()
{
  TFile* fout = new TFile(histogramsFile.c_str(), "RECREATE");
  fout->cd();
  for(unsigned int i = 0; i < Registry.size(); i++)
  {
    Registry[i]->Write();
  }
  fout->Write();
  fout->Close();
  return 0;
}

int dw23VsWness::Loop()
{
  if(!treeAlreadyInitialized) InitTree();

  Long64_t entries = tracks->GetEntries();
  std::cout << treeName << " has " << entries << " entries." << std::endl;

  // Loop over tree.
  for( Long64_t i = 0; i < entries; i++)
  {
    tracks->GetEntry(i);
    if(showDiagnostics)
    {
      double pct = 100.0*static_cast<double>(i)/static_cast<double>(tracks->GetEntries());
      std::cout << "Progress: " << std::setprecision(4) <<  pct << "%     \r" << std::flush;
    }
    int arm_i    = GetArmIndex(vars.pz);        // HelperFunctions.C
    int charge_i = GetChargeIndex(vars.charge); // HelperFunctions.C

    vars.eta = fabs(vars.eta);

    h_Wness->Fill(vars.Wness);
    h_Wness_Fit_Seed[arm_i][charge_i]->Fill(vars.Wness);
    h_eta           [arm_i][charge_i]->Fill(vars.eta);
    h_dw13          [arm_i][charge_i]->Fill(vars.dw13);
    h_dw23          [arm_i][charge_i]->Fill(vars.dw23);
    h_etaVsWness    [arm_i][charge_i]->Fill(vars.Wness,vars.eta);
    h_dw23VsEta     [arm_i][charge_i]->Fill(vars.eta,vars.dw23);
    h_dw23VsWness   [arm_i][charge_i]->Fill(vars.Wness,vars.dw23);

    if( vars.Wness < 0.7 && vars.Wness > 0.1) h_eta_hadBak_pdfSeed[arm_i][charge_i]->Fill(vars.eta);

    // Loop over Wness bins
    for( unsigned int bin = 0; bin < WnessBins.size(); bin++)
    {
      float Wness_lo_edge = WnessBins[bin].first;
      float Wness_hi_edge = WnessBins[bin].second;
      if( (vars.Wness > Wness_lo_edge)&&(vars.Wness < Wness_hi_edge) )
      {
        h_etaWness [arm_i][charge_i][bin]->Fill(vars.eta);
        h_dw23Wness[arm_i][charge_i][bin]->Fill(vars.dw23);
        h_WnessBin [arm_i][charge_i][bin]->Fill(vars.Wness);
      }
    }
  }
  std::cout << std::endl;
  return 0;
}

unsigned int dw23VsWness::RegisterObject(TObject* obj)
{
  Registry.push_back(obj);
  return Registry.size();
}

unsigned int dw23VsWness::RegisterCanvas(TCanvas* can)
{
  CanvasRegistry.push_back(can);
  return CanvasRegistry.size();
}

int dw23VsWness::Print()
{
  for(unsigned int i = 0; i < Registry.size(); i++)
  {
    std::cout << "    ITEM# " << std::setw(4) << i << "    " << Registry[i]->GetName() << std::endl;
  }
  std::cout << std::endl;
  std::cout << "    WNESS BINNING" << std::endl;
  for(unsigned int wness_bin = 0; wness_bin < WnessBins.size(); wness_bin++)
  {
    std::cout << "    WnessBin# " << std::setw(3) << wness_bin
        << ": [" << std::setw(5) << std::setprecision(3) << WnessBins[wness_bin].first << ", "
        << std::setw(5) << std::setprecision(3) << WnessBins[wness_bin].second << "]" << std::endl;
  }
  return 0;
}

int dw23VsWness::PrepareAnalysisObjects()
{
  // resize histogram containers
  // 1. Resize to 'outer vector'
  // 2. Loop to resize 'inner vector'
  // 3. Loop to resize 'inner inner vector'
  f2_double_gaus_2d.resize(NUMBER_OF_ARM_CONFIGS); // [#arms][nosize]
  f_Wness_Seed    .resize(NUMBER_OF_ARM_CONFIGS);  // ''
  h_Wness_Fit_Seed.resize(NUMBER_OF_ARM_CONFIGS);  // ''
  h_eta           .resize(NUMBER_OF_ARM_CONFIGS);  // ''
  h_dw13          .resize(NUMBER_OF_ARM_CONFIGS);  // ''
  h_dw23          .resize(NUMBER_OF_ARM_CONFIGS);  // ''
  h_etaVsWness    .resize(NUMBER_OF_ARM_CONFIGS);  // ''
  h_dw23VsWness   .resize(NUMBER_OF_ARM_CONFIGS);  // ''
  h_eta_hadBak_pdfSeed  .resize(NUMBER_OF_ARM_CONFIGS);  // ''
  h_dw23VsEta     .resize(NUMBER_OF_ARM_CONFIGS);  // ''
  h_etaWness      .resize(NUMBER_OF_ARM_CONFIGS);  // [#arms][nosize][nosize]
  h_dw23Wness     .resize(NUMBER_OF_ARM_CONFIGS);  // ''
  h_WnessBin      .resize(NUMBER_OF_ARM_CONFIGS);  // ''

  for(int arm_i = 0; arm_i < NUMBER_OF_ARM_CONFIGS; arm_i++)
  {
    f2_double_gaus_2d[arm_i].resize(NUMBER_OF_CHARGE_CONFIGS); // [#arms][nosize]
    f_Wness_Seed    [arm_i].resize(NUMBER_OF_CHARGE_CONFIGS);  // ''
    h_Wness_Fit_Seed[arm_i].resize(NUMBER_OF_CHARGE_CONFIGS);  // ''
    h_eta           [arm_i].resize(NUMBER_OF_CHARGE_CONFIGS);  // ''
    h_dw13          [arm_i].resize(NUMBER_OF_CHARGE_CONFIGS);  // ''
    h_dw23          [arm_i].resize(NUMBER_OF_CHARGE_CONFIGS);  // ''
    h_etaVsWness    [arm_i].resize(NUMBER_OF_CHARGE_CONFIGS);  // ''
    h_dw23VsWness   [arm_i].resize(NUMBER_OF_CHARGE_CONFIGS);  // ''
    h_eta_hadBak_pdfSeed  [arm_i].resize(NUMBER_OF_CHARGE_CONFIGS);  // ''
    h_dw23VsEta     [arm_i].resize(NUMBER_OF_CHARGE_CONFIGS);  // ''
    h_etaWness      [arm_i].resize(NUMBER_OF_CHARGE_CONFIGS);  // [#arms][#charges][nosize]
    h_dw23Wness     [arm_i].resize(NUMBER_OF_CHARGE_CONFIGS);  // ''
    h_WnessBin      [arm_i].resize(NUMBER_OF_CHARGE_CONFIGS);  // ''
    for(int charge_i = 0; charge_i < NUMBER_OF_CHARGE_CONFIGS; charge_i++)
    {
      h_etaWness   [arm_i][charge_i].resize(TOTAL_WNESS_BINS);  // [#arms][#charges][#wbins]
      h_dw23Wness  [arm_i][charge_i].resize(TOTAL_WNESS_BINS);  // ''
      h_WnessBin   [arm_i][charge_i].resize(TOTAL_WNESS_BINS);  // ''
    }
  }
  return 0;
}


int dw23VsWness::MakeAnalysisObjects()
{
  h_Wness = new TH1F("Wness", "Wness Distribution;Wness;#Tracks",100,0,1);
  h_Wness->Sumw2();
  RegisterObject(h_Wness);

  for(int arm_i = 0; arm_i < NUMBER_OF_ARM_CONFIGS; arm_i++)
  {
    for(int charge_i = 0; charge_i < NUMBER_OF_CHARGE_CONFIGS; charge_i++)
    {

      std::string name_stub  = "";
      std::string title_stub = "";
      MakeNameTitleStubPair(arm_i,charge_i,DataType,name_stub,title_stub); // HelperFunctions

      std::string name;
      std::string title;

      // 1D Wness Function
      name = "f_Wness_Seed_" + name_stub;
      f_Wness_Seed[arm_i][charge_i] = new TF1(
          name.c_str(),
          "pol4",
          WNESS_LOWER,
          WNESS_UPPER);
      RegisterObject(f_Wness_Seed[arm_i][charge_i]);

      // 2D Fit For Extrapolation
      name = "f2_double_gaus_2d_"+name_stub;
      f2_double_gaus_2d[arm_i][charge_i] = new TF2(
          name.c_str(),
          double_gaus_2d,
          0.1,0.9,-.3,.3,NumCoaxial2DGaussPars);
      RegisterObject(f2_double_gaus_2d[arm_i][charge_i]);

      // h_Wness_Fit_Seed
      name = "Wness_Fit_Seed_"+name_stub;
      title = "Wness Fit Seed " + title_stub +";Wness;#Tracks";
      h_Wness_Fit_Seed[arm_i][charge_i] = new TH1F(
          name.c_str(),
          title.c_str(),
          16, WNESS_LOWER, WNESS_UPPER);
      h_Wness_Fit_Seed[arm_i][charge_i]->Sumw2();
      RegisterObject(h_Wness_Fit_Seed[arm_i][charge_i]);

      // h_eta
      name = "eta_"+name_stub;
      title = "|#eta| "+title_stub+";|#eta|;#Tracks";
      h_eta [arm_i][charge_i] = new TH1F(
          name.c_str(), title.c_str(),
          eta_bins, eta_min, eta_max);
      h_eta [arm_i][charge_i]->Sumw2();
      RegisterObject(h_eta[arm_i][charge_i]);

      // h_eta_hadBak_pdfSeed
      name = "h_eta_hadBak_pdfSeed_"+name_stub;
      title = "|#eta| Seed Distribution for PDF (0.1 < Wness < 0.7 )"
          +title_stub+";|#eta|;#Tracks";
      h_eta_hadBak_pdfSeed [arm_i][charge_i] = new TH1F(
          name.c_str(), title.c_str(),
          30, eta_min, eta_max);
      h_eta_hadBak_pdfSeed [arm_i][charge_i]->Sumw2();
      RegisterObject(h_eta_hadBak_pdfSeed[arm_i][charge_i]);

      // h_dw13
      name = "dw13_"+name_stub;
      title = "dw_{13} "+title_stub+";dw_{13};#Tracks";
      h_dw13 [arm_i][charge_i] = new TH1F(
          name.c_str(),
          title.c_str(),
          100, -5.0, 5.0);
      h_dw13 [arm_i][charge_i]->Sumw2();
      RegisterObject(h_dw13[arm_i][charge_i]);

      // h_dw23
      name = "dw23_"+name_stub;
      title = "dw_{23} "+title_stub+";dw_{23};#Tracks";
      h_dw23 [arm_i][charge_i] = new TH1F(
          name.c_str(),
          title.c_str(),
          dw23_bins, dw23_min, dw23_max);
      h_dw23 [arm_i][charge_i]->Sumw2();
      RegisterObject(h_dw23[arm_i][charge_i]);

      // h_etaVsWness
      name = "etaVsWness_"+name_stub;
      title = "|#eta| vs Wness "+title_stub+";Wness;|#eta|;#Tracks";
      h_etaVsWness[arm_i][charge_i] = new TH2F(
          name.c_str(),
          title.c_str(),
          WnessHistoBins, WNESS_LOWER, WNESS_UPPER,
          eta_bins, eta_min, eta_max);
      h_etaVsWness[arm_i][charge_i]->Sumw2();
      RegisterObject(h_etaVsWness[arm_i][charge_i]);

      // h_dw23VsWness
      name = "dw23VsWness_"+name_stub;
      title = "dw_{23} vs Wness "+title_stub+";Wness;dw_{23};#Tracks";
      h_dw23VsWness[arm_i][charge_i] = new TH2F(
          name.c_str(),
          title.c_str(),
          WnessHistoBins, WNESS_LOWER, WNESS_UPPER,
          dw23_bins, dw23_min, dw23_max);
      h_dw23VsWness[arm_i][charge_i]->Sumw2();
      RegisterObject(h_dw23VsWness[arm_i][charge_i]);

      // h_dw23VsEta
      name = "dw23VsEta_"+name_stub;
      title = "dw_{23} vs |#eta| "+title_stub+";|#eta|;dw_{23};#Tracks";
      h_dw23VsEta[arm_i][charge_i] = new TH2F(
          name.c_str(),
          title.c_str(),
          eta_bins, eta_min, eta_max,
          dw23_bins, dw23_min, dw23_max);
      h_dw23VsEta[arm_i][charge_i]->Sumw2();
      RegisterObject(h_dw23VsEta[arm_i][charge_i]);

      for(int wness_bin = 0; wness_bin < TOTAL_WNESS_BINS; wness_bin++)
      {
        std::stringstream name_ss;
        std::stringstream title_ss;

        // h_etaWness
        name_ss << "etaWness_WnessBin" << wness_bin << "_" << name_stub;
        title_ss << "|#eta| Wness fcut: "
            << WnessBins[wness_bin].first << " < fcut < " << WnessBins[wness_bin].second
            << ", " << title_stub << ";|#eta|;#Tracks";
        h_etaWness[arm_i][charge_i][wness_bin] = new TH1F(
            name_ss.str().c_str(),
            title_ss.str().c_str(),
            eta_bins, eta_min, eta_max);
        h_etaWness [arm_i][charge_i][wness_bin]->Sumw2();
        RegisterObject(h_etaWness[arm_i][charge_i][wness_bin]);

        name_ss.str("");
        title_ss.str("");

        // h_dw23Wness
        name_ss << "dw23Wness_WnessBin" << wness_bin << "_" << name_stub;
        title_ss << "dw_{23} fcut: "
            << WnessBins[wness_bin].first << " < fcut < " << WnessBins[wness_bin].second << ", "
            << title_stub << ";dw_{23};#Tracks";
        h_dw23Wness [arm_i][charge_i][wness_bin] = new TH1F(
            name_ss.str().c_str(),
            title_ss.str().c_str(),
            dw23_bins, dw23_min, dw23_max);
        h_dw23Wness [arm_i][charge_i][wness_bin]->Sumw2();
        RegisterObject(h_dw23Wness[arm_i][charge_i][wness_bin]);

        name_ss.str("");
        title_ss.str("");

        // h_WnessBin
        name_ss << "h_WnessBin" << wness_bin << "_" << name_stub;
        title_ss << "W_{ness} Bin " << wness_bin
            << " " << WnessBins[wness_bin].first << " < fcut < " << WnessBins[wness_bin].second
            << ";Wness;Counts";
        h_WnessBin[arm_i][charge_i][wness_bin] = new TH1F(
            name_ss.str().c_str(),
            title_ss.str().c_str(),
            1, WnessBins[wness_bin].first, WnessBins[wness_bin].second);
        h_WnessBin[arm_i][charge_i][wness_bin]->Sumw2();
        RegisterObject(h_WnessBin[arm_i][charge_i][wness_bin]);

        name_ss.str("");
        title_ss.str("");
      }
    }
  }
  return 0;
}

int dw23VsWness::ReadHistograms()
{
  std::cout << "Reading in histograms from " << histogramsFile << std::endl;
  histoFile = new TFile(histogramsFile.c_str(), "READ");

  h_Wness = (TH1F*)histoFile->Get("Wness");
  RegisterObject(h_Wness);

  for(int arm_i = 0; arm_i < NUMBER_OF_ARM_CONFIGS; arm_i++)
  {
    for(int charge_i = 0; charge_i < NUMBER_OF_CHARGE_CONFIGS; charge_i++)
    {
      std::string name_stub  = "";
      std::string title_stub = "";
      MakeNameTitleStubPair(arm_i,charge_i,DataType,name_stub,title_stub); // HelperFunctions

      std::string name;
      if(showDiagnostics) std::cout << name << std::endl;

      // 1D Wness Function
      name = "f_Wness_Seed_" + name_stub;
      f_Wness_Seed[arm_i][charge_i] = new TF1(
          name.c_str(),
          "pol4",
          WNESS_LOWER,
          WNESS_UPPER);
      RegisterObject(f_Wness_Seed[arm_i][charge_i]);

      // f2_double_gaus_2d (re-do fit, regardless, do not read)
      name = "f2_double_gaus_2d_"+name_stub;
      f2_double_gaus_2d[arm_i][charge_i] = new TF2(
          name.c_str(),
          double_gaus_2d,
          0.1,0.9,-.3,.3,NumCoaxial2DGaussPars);
      RegisterObject(f2_double_gaus_2d[arm_i][charge_i]);
      if(showDiagnostics) std::cout << name << std::endl;

      // h_Wness_Fit_Seed
      name = "Wness_Fit_Seed_"+name_stub;
      h_Wness_Fit_Seed[arm_i][charge_i] = (TH1F*)histoFile->Get(name.c_str());
      if(showDiagnostics) std::cout << name << std::endl;
      RegisterObject(h_Wness_Fit_Seed[arm_i][charge_i]);

      // h_eta
      name = "eta_"+name_stub;
      h_eta[arm_i][charge_i] = (TH1F*)histoFile->Get(name.c_str());
      if(showDiagnostics) std::cout << name << std::endl;
      RegisterObject(h_eta[arm_i][charge_i]);

      // h_eta_hadBak_pdfSeed
      name = "h_eta_hadBak_pdfSeed_"+name_stub;
      if(showDiagnostics) std::cout << name << std::endl;
      h_eta_hadBak_pdfSeed [arm_i][charge_i] = (TH1F*)histoFile->Get(name.c_str());
      RegisterObject(h_eta_hadBak_pdfSeed[arm_i][charge_i]);

      // h_dw13
      name = "dw13_"+name_stub;
      h_dw13[arm_i][charge_i] = (TH1F*)histoFile->Get(name.c_str());
      if(showDiagnostics) std::cout << name << std::endl;
      RegisterObject(h_dw13[arm_i][charge_i]);

      // h_dw23
      name = "dw23_"+name_stub;
      h_dw23[arm_i][charge_i] = (TH1F*)histoFile->Get(name.c_str());
      if(showDiagnostics) std::cout << name << std::endl;
      RegisterObject(h_dw23[arm_i][charge_i]);

      // h_etaVsWness
      name = "etaVsWness_"+name_stub;
      h_etaVsWness[arm_i][charge_i] = (TH2F*)histoFile->Get(name.c_str());
      if(showDiagnostics) std::cout << name << std::endl;
      RegisterObject(h_etaVsWness[arm_i][charge_i]);

      // h_dw23VsWness
      name = "dw23VsWness_"+name_stub;
      h_dw23VsWness[arm_i][charge_i] = (TH2F*)histoFile->Get(name.c_str());
      if(showDiagnostics) std::cout << name << std::endl;
      RegisterObject(h_dw23VsWness[arm_i][charge_i]);

      // h_dw23VsEta
      name = "dw23VsEta_"+name_stub;
      h_dw23VsEta[arm_i][charge_i] = (TH2F*)histoFile->Get(name.c_str());
      if(showDiagnostics) std::cout << name << std::endl;
      RegisterObject(h_dw23VsEta[arm_i][charge_i]);

      for(int wness_bin = 0; wness_bin < TOTAL_WNESS_BINS; wness_bin++)
      {
        std::stringstream name_ss;

        // h_etaWness
        name_ss << "etaWness_WnessBin" << wness_bin << "_" << name_stub;
        h_etaWness [arm_i][charge_i][wness_bin] = (TH1F*)histoFile->Get(name_ss.str().c_str());
        if(showDiagnostics) std::cout << name_ss.str() << std::endl;
        RegisterObject(h_etaWness[arm_i][charge_i][wness_bin]);

        name_ss.str("");

        // h_dw23Wness
        name_ss << "dw23Wness_WnessBin" << wness_bin << "_" << name_stub;
        h_dw23Wness [arm_i][charge_i][wness_bin] = (TH1F*)histoFile->Get(name_ss.str().c_str());
        if(showDiagnostics) std::cout << name_ss.str() << std::endl;
        RegisterObject(h_dw23Wness[arm_i][charge_i][wness_bin]);

        name_ss.str("");

        // h_WnessBin
        name_ss << "h_WnessBin" << wness_bin << "_" << name_stub;
        h_WnessBin[arm_i][charge_i][wness_bin] = (TH1F*)histoFile->Get(name_ss.str().c_str());
        if(showDiagnostics) std::cout << name_ss.str() << std::endl;
        RegisterObject(h_WnessBin[arm_i][charge_i][wness_bin]);

        name_ss.str("");
      }
    }
  }
  return 0;
}

int dw23VsWness::InitTree()
{
  if(showDiagnostics) std::cout << "Extracting " << treeName << "from : " << dataTreeFile << std::endl;
  // setting up the histograms
  reducedTreeFile = new TFile(dataTreeFile.c_str(),"READ");
  tracks = (TTree*)reducedTreeFile->Get(treeName.c_str());
  vars.LinkTree(tracks, READ);

  if(showDiagnostics) std::cout << treeName << " branch addresses set, entries: " << tracks->GetEntries() << std::endl;
  treeAlreadyInitialized = true;
  return tracks->GetEntries();
}

int dw23VsWness::GeneratePDFs()
{
  if(showDiagnostics) std::cout << "dw23VsWness::GeneratePDFs" << std::endl;
  // we only need to set branch addresses once.
  if(!treeAlreadyInitialized) InitTree();

  PdfOutFile = new TFile(NewPdfFileName.c_str(),"RECREATE");
  TTree* t_muBak = new TTree("NmuBak","Number of Muons in Muon Background");
  int muBakArm0Charge0, muBakArm1Charge0, muBakArm1Charge1, muBakArm0Charge1;
  t_muBak->Branch("nMuBakMuons_Arm0_Charge0", &muBakArm0Charge0, "nMuBakMuons_Arm0_Charge0/I");
  t_muBak->Branch("nMuBakMuons_Arm1_Charge0", &muBakArm1Charge0, "nMuBakMuons_Arm1_Charge0/I");
  t_muBak->Branch("nMuBakMuons_Arm1_Charge1", &muBakArm1Charge1, "nMuBakMuons_Arm1_Charge1/I");
  t_muBak->Branch("nMuBakMuons_Arm0_Charge1", &muBakArm0Charge1, "nMuBakMuons_Arm0_Charge1/I");
  
  LoadEtaEfficiencies();
  FormHadBackgroundPdfSeeds();
  FormWSignalPdfSeeds();
  FormMuonBackgroundPdfSeeds();
  for(int arm_i = 0; arm_i < NUMBER_OF_ARM_CONFIGS; arm_i++)
  {
    for(int charge_i = 0; charge_i < NUMBER_OF_CHARGE_CONFIGS; charge_i++)
    {
      std::stringstream name;
      name << "c_dw23_Eta_PDF" << "_Arm" << arm_i << "_Charge" << charge_i;
      c_dw23_Eta_PDF[arm_i][charge_i] = new TCanvas(name.str().c_str(),"Summary of PDF seeds",1800,800);
      RegisterCanvas(c_dw23_Eta_PDF[arm_i][charge_i]);
      name.str("");
    
      // Setup Plots
      TCanvas* c = c_dw23_Eta_PDF[arm_i][charge_i];
      c->Divide(3,2);
      int canvas_bin = 1;
      c->cd(canvas_bin); h_dw23_pdfSeed [HAD_INDEX][arm_i][charge_i]->Draw(); canvas_bin++;
      c->cd(canvas_bin); h_dw23_pdfSeed [BAK_INDEX][arm_i][charge_i]->Draw(); canvas_bin++;
      c->cd(canvas_bin); h_dw23_pdfSeed [W_INDEX  ][arm_i][charge_i]->Draw(); canvas_bin++;
      c->cd(canvas_bin); h_eta_pdfSeed  [HAD_INDEX][arm_i][charge_i]->Draw(); canvas_bin++;
      c->cd(canvas_bin); h_eta_pdfSeed  [BAK_INDEX][arm_i][charge_i]->Draw(); canvas_bin++;
      c->cd(canvas_bin); h_eta_pdfSeed  [W_INDEX  ][arm_i][charge_i]->Draw(); canvas_bin++;

      PdfOutFile->cd();
      h_dw23_pdfSeed [HAD_INDEX][arm_i][charge_i]->Write();
      h_dw23_pdfSeed [BAK_INDEX][arm_i][charge_i]->Write();
      h_dw23_pdfSeed [W_INDEX  ][arm_i][charge_i]->Write();
      h_eta_pdfSeed  [HAD_INDEX][arm_i][charge_i]->Write();
      h_eta_pdfSeed  [BAK_INDEX][arm_i][charge_i]->Write();
      h_eta_pdfSeed  [W_INDEX  ][arm_i][charge_i]->Write();

      canvas_bin = 0;
    }
  }
  muBakArm0Charge0 = nMuBakMuons[0][0];
  muBakArm1Charge0 = nMuBakMuons[1][0];
  muBakArm1Charge1 = nMuBakMuons[1][1];
  muBakArm0Charge1 = nMuBakMuons[0][1];
  t_muBak->Fill();
  PdfOutFile->cd();
  t_muBak->Write();
  delete t_muBak;
  if(showDiagnostics) std::cout << "All PDFs have been generated successfully" << std::endl;
  if(showDiagnostics) std::cout << "dw23VsWness::GeneratePDFs Done" << std::endl;
  return 0;
}

int dw23VsWness::AddMuonBackground(const std::string bkgd_key, const std::string bkgd_file_name , double weight)
{
  std::pair<std::string,double> mu_file_pair = std::make_pair(bkgd_file_name, weight);
  MuBkgMap[bkgd_key] = mu_file_pair;
  return static_cast<int>(MuBkgMap.size()); // return number of successfully added muon background histograms?
}

int dw23VsWness::AddWSignal(const std::string WSigFile)
{
  WSignalFile = WSigFile;
  if(showDiagnostics) std::cout << "Added W Signal File: " << WSignalFile << std::endl;
  return 0;
}

int dw23VsWness::FormMuonBackgroundPdfSeeds()
{
  if(showDiagnostics) std::cout << "dw23VsWness::FormMuonBackgroundPdfSeeds" << std::endl;
  // here, we have to extract histograms from each tree, and
  // then add them together according to their specific weights.
  TH1F* dw23_muBak[NUMBER_OF_MUON_BACKGROUNDS][NUMBER_OF_ARM_CONFIGS][NUMBER_OF_CHARGE_CONFIGS];
  TH1F*  eta_muBak[NUMBER_OF_MUON_BACKGROUNDS][NUMBER_OF_ARM_CONFIGS][NUMBER_OF_CHARGE_CONFIGS];
  TH1F* dw23_tmp  [NUMBER_OF_MUON_BACKGROUNDS][NUMBER_OF_ARM_CONFIGS][NUMBER_OF_CHARGE_CONFIGS]; // for nmu check
  TH1F*  eta_tmp  [NUMBER_OF_MUON_BACKGROUNDS][NUMBER_OF_ARM_CONFIGS][NUMBER_OF_CHARGE_CONFIGS]; // for nmu check

  std::vector<std::string> summaryOutput;
  std::stringstream summaryLine;
  std::vector<std::string> muBackCheck;
  std::stringstream muBackCheckLine;

  std::string chkName = baseDirectory + logDirectory + "MuonBackgroundPopulationCheck.txt";
  std::ofstream muBakPopulationCheck(chkName.c_str());
  
  std::vector<TObject*> memory_tracker;
  
  if(MuBkgMap.size() == static_cast<unsigned int>(NUMBER_OF_MUON_BACKGROUNDS))
  {
    // Make histograms
    TCanvas* mu_pdf_canvas = new TCanvas("mu_pdf_canvas","muon background pdfs", 800,400);
    mu_pdf_canvas->Divide(2,1);

    MuBakMap::iterator itr;
    for(itr = MuBkgMap.begin(); itr != MuBkgMap.end(); ++itr)
    {
      for( int arm_i = 0; arm_i < NUMBER_OF_ARM_CONFIGS; arm_i++)
      {
        for( int charge_i = 0; charge_i < NUMBER_OF_CHARGE_CONFIGS; charge_i++)
        {
          std::string mubak_key = itr->first;
          std::string file_name  = (itr->second).first;
          double mubak_weight = (itr->second).second;

          std::cout << mubak_key << ", " << file_name << ", " << mubak_weight << std::endl;
          std::stringstream dw23_name; dw23_name << "dw23_muBak_" << mubak_key << "_Arm" << arm_i << "_Charge" << charge_i;
          dw23_muBak[MuIndex[mubak_key]][arm_i][charge_i] = new TH1F(
              dw23_name.str().c_str(),
              dw23_name.str().c_str(),
              dw23_bins, pdf_dw23_min, pdf_dw23_max
              );
          std::string tmpName = "tmp_" + dw23_name.str();
          dw23_tmp  [MuIndex[mubak_key]][arm_i][charge_i] = new TH1F(
              tmpName.c_str(),
              tmpName.c_str(),
              dw23_bins, pdf_dw23_min, pdf_dw23_max
              ); // for nmu check
          memory_tracker.push_back(dw23_muBak[MuIndex[mubak_key]][arm_i][charge_i]);
          memory_tracker.push_back(dw23_tmp[MuIndex[mubak_key]][arm_i][charge_i]);

          if(showDiagnostics) std::cout << "\t" << dw23_muBak[MuIndex[mubak_key]][arm_i][charge_i]->GetName() << std::endl;

          std::stringstream eta_name; eta_name << "eta_muBak_" << mubak_key << "_Arm" << arm_i << "_Charge" << charge_i;
          eta_muBak[MuIndex[mubak_key]][arm_i][charge_i] = new TH1F(
              eta_name.str().c_str(),
              eta_name.str().c_str(),
              eta_bins, eta_min, eta_max
              );
          tmpName = "tmp_" + eta_name.str();
          eta_tmp  [MuIndex[mubak_key]][arm_i][charge_i] = new TH1F(
              tmpName.c_str(),
              tmpName.c_str(),
              eta_bins, eta_min, eta_max
              ); // for nmu check
          memory_tracker.push_back(eta_muBak[MuIndex[mubak_key]][arm_i][charge_i]);
          memory_tracker.push_back(eta_tmp[MuIndex[mubak_key]][arm_i][charge_i]);

          eta_name.str("");
          dw23_name.str("");

          eta_muBak[MuIndex[mubak_key]][arm_i][charge_i]->Sumw2();
          dw23_muBak[MuIndex[mubak_key]][arm_i][charge_i]->Sumw2();
          eta_tmp[MuIndex[mubak_key]][arm_i][charge_i]->Sumw2();
          dw23_tmp[MuIndex[mubak_key]][arm_i][charge_i]->Sumw2();
        }
      }
    }

    // Fill each Tree's PDF histogram with appropriate weight
    std::string events_log = baseDirectory + logDirectory + "MuBak_OutOfEtaRange.txt";
    ofstream out_of_range_events_file(events_log.c_str());
    for(itr = MuBkgMap.begin(); itr != MuBkgMap.end(); ++itr)
    {
      // Extract tree
      // Loop over Tree to extract histograms
      std::string mubak_key = itr->first;
      std::string file_name  = (itr->second).first;
      //double mubak_weight = (itr->second).second;

      TFile* tempFile = new TFile(file_name.c_str(),"READ");
      TTree* tempTree = (TTree*)tempFile->Get(wness_tree.c_str());
      BasicCutTreeVariables v;
      v.LinkTree(tempTree,READ);

      // Need arm and charge discrimination - the trees might be short enough
      // to just loop through four times...though with newer simulations this
      // may change. Better do it "the right way".
      std::cout << file_name << std::endl;
      int out_of_range_events = 0;
      int in_range_events = 0;
      for(Long64_t entry = 0; entry < tempTree->GetEntries(); entry++)
      {
        tempTree->GetEntry(entry);
        double temp_eta = fabs(v.eta);
        if(v.Wness > 0.92)
        {
          eta_tmp [MuIndex[mubak_key]][GetArmIndex(v.pz)][GetChargeIndex(v.charge)]->Fill(v.eta  );
          dw23_tmp[MuIndex[mubak_key]][GetArmIndex(v.pz)][GetChargeIndex(v.charge)]->Fill(v.dw23 );
          // we will weight the files when we finally add them together.
          int eta_bin = -1;
          for(int temp_eta_bin = 0; temp_eta_bin < NUMBER_OF_ETA_BINS; temp_eta_bin++)
          {
            if((eta_eff[0][0][temp_eta_bin].eta_min < temp_eta )&&(temp_eta < eta_eff[0][0][temp_eta_bin].eta_max))
            {
              eta_bin = temp_eta_bin;
              break;
            }
          }
          if( eta_bin >= 0)
          {
            // use weight now
            int i = GetArmIndex(v.pz);
            int j = GetChargeIndex(v.charge);
            int k = eta_bin;
            eta_muBak [MuIndex[mubak_key]][i][j]->Fill(temp_eta,eta_eff[i][j][k].effi);
            dw23_muBak[MuIndex[mubak_key]][i][j]->Fill(v.dw23, eta_eff[i][j][k].effi  );
            in_range_events++;
          }
          else
          {
            out_of_range_events++;
            out_of_range_events_file << mubak_key << ", " << "Event:" << v.Evt_Number << ", Run:" << v.Run_Number << ", Eta: "  << eta_eff[0][0][0].eta_min << " < " << temp_eta << " < " << eta_eff[0][0][14].eta_max << std::endl;
          }
        }
      }
      delete tempFile;  // tree is also owned by tempFile, so it gets deleted.
      std::cout << mubak_key << ", " << out_of_range_events << "/" << in_range_events << " events out of eta range" << std::endl;
      //Pause();
    }
    out_of_range_events_file.close();

    // Draw PDF seeds (unmerged)
    for(int i = 0; i < NUMBER_OF_MUON_BACKGROUNDS; i++)
    {
      for (int arm_i = 0; arm_i < NUMBER_OF_ARM_CONFIGS; arm_i++)
      {
        for(int charge_i = 0; charge_i < NUMBER_OF_CHARGE_CONFIGS; charge_i++)
        {
          mu_pdf_canvas->cd(1);
          dw23_muBak[i][arm_i][charge_i]->Draw();
          mu_pdf_canvas->cd(2);
          eta_muBak[i][arm_i][charge_i]->Draw();
          mu_pdf_canvas->Update();
        }
      }
    }

    // Create PDFs via merging
    for(int arm_i = 0; arm_i < NUMBER_OF_ARM_CONFIGS; arm_i++)
    {
      for(int charge_i = 0; charge_i < NUMBER_OF_CHARGE_CONFIGS; charge_i++)
      {
        std::stringstream name;
        name << "h_dw23_muBak_pdfSeed" << "_Arm" << arm_i << "_Charge" << charge_i;
        h_dw23_pdfSeed [BAK_INDEX][arm_i][charge_i] = new TH1F(name.str().c_str(), name.str().c_str(), dw23_bins, pdf_dw23_min, pdf_dw23_max);
        RegisterObject(h_dw23_pdfSeed[BAK_INDEX][arm_i][charge_i]);
        name.str(""); name << "h_eta_muBak_pdfSeed" << "_Arm" << arm_i << "_Charge" << charge_i;
        h_eta_pdfSeed  [BAK_INDEX][arm_i][charge_i] = new TH1F(name.str().c_str(), name.str().c_str(), eta_bins , eta_min , eta_max );
        RegisterObject(h_eta_pdfSeed[BAK_INDEX][arm_i][charge_i]);
        h_eta_pdfSeed  [BAK_INDEX][arm_i][charge_i]->Sumw2();
        h_dw23_pdfSeed [BAK_INDEX][arm_i][charge_i]->Sumw2();

        // Merge PDFs to make Muon Background Merged PDF
        for(itr = MuBkgMap.begin(); itr != MuBkgMap.end(); ++itr)
        {
          std::string mubak_key = itr->first;
          std::string file_name  = (itr->second).first;
          double mubak_weight = (itr->second).second;

          h_dw23_pdfSeed[BAK_INDEX][arm_i][charge_i]->Add(dw23_muBak[MuIndex[mubak_key]][arm_i][charge_i], mubak_weight);
          h_eta_pdfSeed [BAK_INDEX][arm_i][charge_i]->Add(eta_muBak [MuIndex[mubak_key]][arm_i][charge_i], mubak_weight);
          
          // Scaled Entries Total
          nMuBakMuons[arm_i][charge_i] += dw23_muBak[MuIndex[mubak_key]][arm_i][charge_i]->Integral()*mubak_weight; 

          summaryLine.str("");
          muBackCheckLine.str("");
          if(arm_i == 0 && charge_i == 1)
          {
            muBackCheckLine << mubak_key << ", " << dw23_muBak[MuIndex[mubak_key]][arm_i][charge_i]->Integral() << ", " << mubak_weight;
            muBackCheck.push_back(muBackCheckLine.str());
          }

          summaryLine << "Arm_" << arm_i << ", Charge_" << charge_i << ", " << mubak_key 
              << ", With Weight: " << mubak_weight
              << ", Scaled Events: "  << dw23_muBak[MuIndex[mubak_key]][arm_i][charge_i]->Integral()*mubak_weight;
          summaryOutput.push_back(summaryLine.str());
        }
        // Smooth PDFs to reduce statistical noise
        h_dw23_pdfSeed[BAK_INDEX][arm_i][charge_i]->Smooth(0);
        h_eta_pdfSeed [BAK_INDEX][arm_i][charge_i]->Smooth(smoothing);

        mu_pdf_canvas->cd(1);
        h_dw23_pdfSeed[BAK_INDEX][arm_i][charge_i]->Draw();
        mu_pdf_canvas->cd(2);
        h_eta_pdfSeed[BAK_INDEX][arm_i][charge_i]->Draw();
        mu_pdf_canvas->Update();
      }
    }
    delete mu_pdf_canvas;
  }
  else
  {
    std::cerr << "Exptected to find " << NUMBER_OF_MUON_BACKGROUNDS << " background files, but did not\n";
    return -1;
  }

  // Create Log File For Fitting.
  MuonBackgroundLog << std::endl << "Muon Bkg Scaling Factors (including eff):" << std::endl;
  

  for(MuBakMap::iterator itr = MuBkgMap.begin(); itr != MuBkgMap.end(); ++itr)
  {
     std::string mubak_key = itr->first;
     double mubak_weight = (itr->second).second;

     MuonBackgroundLog << mubak_key << ": " << mubak_weight << std::endl;
  }

  for(int arm_i = 0; arm_i < NUMBER_OF_ARM_CONFIGS; arm_i++)
  {
    for(int charge_i = 0; charge_i < NUMBER_OF_CHARGE_CONFIGS; charge_i++ )
    {
      MuonBackgroundLog << std::endl << "Mu Bg Yields Arm" << arm_i << " Charge" << charge_i << std::endl;
      for(MuBakMap::iterator itr = MuBkgMap.begin(); itr != MuBkgMap.end(); ++itr)
      {
        std::string mubak_key = itr->first;
        double mubak_weight = (itr->second).second;
        double yield = dw23_muBak[MuIndex[mubak_key]][arm_i][charge_i]->Integral()*mubak_weight; 
        MuonBackgroundLog << mubak_key << " " << yield << std::endl;
      }
    }
  }

  // Make Comparison Log File for Scaled Backgrounds
  muBakPopulationCheck << "# raw: no trigger efficiencies applied - raw integral of mubak for Wness > .92" << std::endl;
  muBakPopulationCheck << "# sc: trigger efficiency applied and distribution is integrated for Wness > .92" << std::endl;
  muBakPopulationCheck << "# tot: total population used after trigger efficiency + scale factor is applied" << std::endl;
  
  muBakPopulationCheck << std::setw(12) 
      << "# mubk" 
      << std::setw(12) 
      << "arm" 
      << std::setw(12) 
      << "charge" 
      << std::setw(12) 
      << "raw eta" 
      << std::setw(12) 
      << "raw dw23" 
      << std::setw(12) 
      << "sc eta" 
      << std::setw(12) 
      << "sc dw23" 
      << std::setw(12)
      << "tot eta"
      << std::setw(12)
      << "tot dw23"
      << std::endl;
  for( MuBakMap::iterator itr = MuBkgMap.begin(); itr != MuBkgMap.end(); ++itr)
  {
    for( int arm_i = 0; arm_i < NUMBER_OF_ARM_CONFIGS; arm_i++)
    {
      for(int charge_i = 0; charge_i < NUMBER_OF_CHARGE_CONFIGS; charge_i++)
      {
        std::string mubak_key = itr->first;
        double mubak_weight = (itr->second).second;
        int mu_i = MuIndex[mubak_key];
        muBakPopulationCheck << std::setw(12) <<  mubak_key
            << std::setw(12) <<  arm_i 
            << std::setw(12) <<  charge_i 
            << std::setw(12) <<  eta_tmp   [mu_i][arm_i][charge_i]->Integral() 
            << std::setw(12) <<  dw23_tmp  [mu_i][arm_i][charge_i]->Integral() 
            << std::setw(12) <<  eta_muBak [mu_i][arm_i][charge_i]->Integral() 
            << std::setw(12) <<  dw23_muBak[mu_i][arm_i][charge_i]->Integral() 
            << std::setw(12) <<  eta_muBak [mu_i][arm_i][charge_i]->Integral()*mubak_weight
            << std::setw(12) <<  dw23_muBak[mu_i][arm_i][charge_i]->Integral()*mubak_weight
            << std::endl;
      }
    }
  }
  muBakPopulationCheck.close();
  
  if(showDiagnostics) std::cout << "dw23VsWness::FormMuonBackgroundPdfSeeds done" << std::endl;

  // Clean up
  if(showDiagnostics) std::cout << "Deleting locally allocated memory!" << std::endl;
  for(unsigned int i = 0; i < memory_tracker.size(); i++)
  {
    if(showDiagnostics) std::cout << "Deleting memory_tracker[" << i << "]: " << memory_tracker[i]->GetName() << std::endl;
    delete memory_tracker[i];
  }
  return 0;
}

int dw23VsWness::FormHadBackgroundPdfSeeds()
{
  if(showDiagnostics) std::cout << "dw23VsWness::FormHadBackgroundPdfSeeds" << std::endl;
  for(int arm_i = 0; arm_i < NUMBER_OF_ARM_CONFIGS; arm_i++)
  {
    for(int charge_i = 0; charge_i < NUMBER_OF_CHARGE_CONFIGS; charge_i++)
    {
      // Store extrapolated shapes
      std::stringstream name;
      name << "h_dw23_HadBak_pdfSeed" << "_Arm" << arm_i << "_Charge" << charge_i;

      // Generate dw23 pdf seed from extrapolation
      int dw23_extrap_bins = 30;
      h_dw23_pdfSeed[HAD_INDEX][arm_i][charge_i] = new TH1F(name.str().c_str(), name.str().c_str(), dw23_extrap_bins, pdf_dw23_min, pdf_dw23_max);
      h_dw23_pdfSeed[HAD_INDEX][arm_i][charge_i]->Sumw2();
      RegisterObject(h_dw23_pdfSeed[HAD_INDEX][arm_i][charge_i]);
      /* Method 1 - Randomly Fill Histogram */
      std::cout << f_dw23_HadBak_pdfSeed[arm_i][charge_i]->GetName() << std::endl;
      //Pause();
      h_dw23_pdfSeed[HAD_INDEX][arm_i][charge_i]->FillRandom(f_dw23_HadBak_pdfSeed[arm_i][charge_i]->GetName(), 1000000);
      
      /* Method 2 - Set Each Bin Center to the Value of the Function */
      /*
      for(int bin_i = 1; bin_i <= dw23_extrap_bins; bin_i++)
      {
        float x = h_dw23_pdfSeed[HAD_INDEX][arm_i][charge_i]->GetBinCenter(bin_i);     // get the bin to fill
        float y = f_dw23_HadBak_pdfSeed    [arm_i][charge_i]->Eval(x);                 // get the number to fill the bin with
        h_dw23_pdfSeed[HAD_INDEX]          [arm_i][charge_i]->SetBinContent(bin_i, y); // set the bin to the right value.
      }
      */

      /* Method 3 Directly use Function to generate the PDF */

      name.str("");
      
      name << "h_eta_HadBak_pdfSeed_Arm" << arm_i << "_Charge" << charge_i;
      h_eta_pdfSeed[HAD_INDEX][arm_i][charge_i] = h_eta_hadBak_pdfSeed[arm_i][charge_i];
      h_eta_pdfSeed[HAD_INDEX][arm_i][charge_i]->SetName(name.str().c_str());
      h_eta_pdfSeed[HAD_INDEX][arm_i][charge_i]->Smooth(0);
      h_dw23_pdfSeed[HAD_INDEX][arm_i][charge_i]->Smooth(0);
      name.str("");
    }
  }
  return 0;
  if(showDiagnostics) std::cout << "dw23VsWness::FormHadBackgroundPdfSeeds done" << std::endl;
}

int dw23VsWness::FormWSignalPdfSeeds()
{
  if(showDiagnostics) std::cout << "dw23VsWness::FormWSignalPdfSeeds" << std::endl;
  // Need: eta, dw23, tree.
  // Need to wrap this into a call which does the right thing.
  // During development, members are called in the macro, with no
  // call order control.
  // Tree is not kept as a member, becase we just want the histograms, not every
  // tree entry.

  // Init Histograms
  for(int arm_i = 0; arm_i < NUMBER_OF_ARM_CONFIGS; arm_i++)
  {
    for(int charge_i = 0; charge_i < NUMBER_OF_CHARGE_CONFIGS; charge_i++)
    {
      std::stringstream name;
      std::string name_stub;
      std::string title_stub;
      MakeNameTitleStubPair(arm_i, charge_i, SIGNAL_INDEX, name_stub, title_stub);

      name << "h_dw23_WSig_pdfSeed_" << "Arm" << arm_i << "_Charge" << charge_i;
      h_dw23_pdfSeed [W_INDEX][arm_i][charge_i] = new TH1F(
          name.str().c_str(),
          name.str().c_str(),
          dw23_bins, pdf_dw23_min, pdf_dw23_max);
      name.str("");
      h_dw23_pdfSeed [W_INDEX][arm_i][charge_i]->Sumw2();
      RegisterObject(h_dw23_pdfSeed [W_INDEX][arm_i][charge_i]);

      name << "h_eta_WSig_pdfSeed_" << "Arm" << arm_i << "_Charge" << charge_i;
      h_eta_pdfSeed  [W_INDEX][arm_i][charge_i] = new TH1F(
          name.str().c_str(),
          name.str().c_str(),
          eta_bins, eta_min, eta_max);
      h_eta_pdfSeed  [W_INDEX][arm_i][charge_i]->Sumw2();
      RegisterObject(h_eta_pdfSeed  [W_INDEX][arm_i][charge_i]);
      name.str("");
    }
  }
  // Init Tree
  BasicCutTreeVariables v;
  TFile* WTreeFile = new TFile(WSignalFile.c_str(),"READ");
  TTree* WTree = (TTree*)WTreeFile->Get(treeName.c_str());
  v.LinkTree(WTree, READ);

  int out_of_range_events = 0;
  int in_range_events = 0;
  std::string events_log = baseDirectory + logDirectory + "WSig_OutOfEtaRange.txt";
  ofstream out_of_range_events_file(events_log.c_str());
  for(Long64_t entry = 0; entry < WTree->GetEntries(); entry++)
  {
    WTree->GetEntry(entry);
    if(v.Wness > 0.92)
    { 
      int eta_bin = -1;
      float temp_eta = fabs(v.eta);
      //double compare_eta = round(temp_eta * 10.0)/(10.0);

      // lookup eta bin
      for(int temp_eta_bin = 0; temp_eta_bin < NUMBER_OF_ETA_BINS; temp_eta_bin++)
      {
        if((eta_eff[0][0][temp_eta_bin].eta_min < temp_eta )&&( temp_eta < eta_eff[0][0][temp_eta_bin].eta_max))
        {
          eta_bin = temp_eta_bin;
          break;
        }
      }
      if( eta_bin >= 0)
      {
        int i = GetArmIndex(v.pz);
        int j = GetChargeIndex(v.charge);
        int k = eta_bin;
        h_eta_pdfSeed [W_INDEX][i][j]->Fill(temp_eta, eta_eff[i][j][k].effi);
        h_dw23_pdfSeed[W_INDEX][i][j]->Fill(v.dw23, eta_eff[i][j][k].effi);
      
        in_range_events++;
      }
      else
      {
        out_of_range_events_file << "WSig, Event:" << v.Evt_Number 
            << ", Run:" << v.Run_Number 
            << ", Eta: "  << eta_eff[0][0][0].eta_min 
            << " < " << temp_eta 
            << " < " << eta_eff[0][0][14].eta_max 
            << std::endl;
        out_of_range_events++;
        //Pause();
      }
    }
  }
  for(int arm_i = 0; arm_i < NUMBER_OF_ARM_CONFIGS; arm_i++)
  {
    for(int charge_i = 0; charge_i < NUMBER_OF_CHARGE_CONFIGS; charge_i++)
    {
        h_eta_pdfSeed [W_INDEX][arm_i][charge_i]->Smooth(smoothing);
        h_dw23_pdfSeed[W_INDEX][arm_i][charge_i]->Smooth(0);
    }
  }
  out_of_range_events_file.close();
  std::cout << out_of_range_events << "/" << in_range_events << " events out of eta range" << std::endl;

  //Pause();
  delete WTree;
  delete WTreeFile;
  
  if(showDiagnostics) std::cout << "dw23VsWness::FormWSignalPdfSeeds done" << std::endl;
  return 0;
}

void dw23VsWness::Pause()
{
  std::cout << "Press enter to continue..." << std::endl;
  getchar();
}

int dw23VsWness::LoadEtaEfficiencies()
{
  // make the eta efficinecy storage data structure
  eta_eff.resize(NUMBER_OF_ARM_CONFIGS);  // [#arms][nosize][nosize]
  for(int arm_i = 0; arm_i < NUMBER_OF_ARM_CONFIGS; arm_i++)
  {
    eta_eff[arm_i].resize(NUMBER_OF_CHARGE_CONFIGS);  // [#arms][#charges][nosize]
    for(int charge_i = 0; charge_i < NUMBER_OF_CHARGE_CONFIGS; charge_i++)
    {
      eta_eff[arm_i][charge_i].resize(NUMBER_OF_ETA_BINS);  // [#arms][#charges][#etabins]
    }
  }
  std::string trigEfficFileName = baseDirectory+macrosDirectory+rootTreesDirectory+"totaltrigefficiencies2_new13.dat";
  std::ifstream inFileEta(trigEfficFileName.c_str());
  std::string line;

  double eta_min, eta_max, effi_pos, delta_effi_pos, effi_syst_pos, effi_neg, delta_effi_neg, effi_syst_neg;
  bool switch_arm = true;
  int line_count = 0;
  if(inFileEta)
  {
    std::cout << "opening eta efficiencies: " << trigEfficFileName << std::endl;
    int eta_bin = 0;
    while(getline(inFileEta,line))
    {
      //std::cout << line << std::endl;
      std::stringstream ss;
      ss.str(line);
      std::cout.precision(10);
      ss >> eta_min >> eta_max >>  effi_neg >> delta_effi_neg >> effi_syst_neg >> effi_pos >> delta_effi_pos >> effi_syst_pos;
      ss.str("");
      
      if(eta_bin == 15)
      {
        switch_arm = false;
        eta_bin = 0;
      }

      // south
      if(switch_arm)
      {
        // positive
        eta_eff[ARM_SOUTH][CHARGE_POS][eta_bin].eta_min    = eta_min        ;   
        eta_eff[ARM_SOUTH][CHARGE_POS][eta_bin].eta_max    = eta_max        ;   
        eta_eff[ARM_SOUTH][CHARGE_POS][eta_bin].effi       = effi_pos       ;      
        eta_eff[ARM_SOUTH][CHARGE_POS][eta_bin].delta_effi = delta_effi_pos ;
        eta_eff[ARM_SOUTH][CHARGE_POS][eta_bin].syst_effi  = effi_syst_pos  ; 
       
        // negative
        eta_eff[ARM_SOUTH][CHARGE_NEG][eta_bin].eta_min    = eta_min        ;   
        eta_eff[ARM_SOUTH][CHARGE_NEG][eta_bin].eta_max    = eta_max        ;   
        eta_eff[ARM_SOUTH][CHARGE_NEG][eta_bin].effi       = effi_neg       ;      
        eta_eff[ARM_SOUTH][CHARGE_NEG][eta_bin].delta_effi = delta_effi_neg ;
        eta_eff[ARM_SOUTH][CHARGE_NEG][eta_bin].syst_effi  = effi_syst_neg  ; 
      }
      // north
      else
      {
        // positive
        eta_eff[ARM_NORTH][CHARGE_POS][eta_bin].eta_min    = eta_min        ;   
        eta_eff[ARM_NORTH][CHARGE_POS][eta_bin].eta_max    = eta_max        ;   
        eta_eff[ARM_NORTH][CHARGE_POS][eta_bin].effi       = effi_pos       ;      
        eta_eff[ARM_NORTH][CHARGE_POS][eta_bin].delta_effi = delta_effi_pos ;
        eta_eff[ARM_NORTH][CHARGE_POS][eta_bin].syst_effi  = effi_syst_pos  ; 
       
        // negative
        eta_eff[ARM_NORTH][CHARGE_NEG][eta_bin].eta_min    = eta_min        ;   
        eta_eff[ARM_NORTH][CHARGE_NEG][eta_bin].eta_max    = eta_max        ;   
        eta_eff[ARM_NORTH][CHARGE_NEG][eta_bin].effi       = effi_neg       ;      
        eta_eff[ARM_NORTH][CHARGE_NEG][eta_bin].delta_effi = delta_effi_neg ;
        eta_eff[ARM_NORTH][CHARGE_NEG][eta_bin].syst_effi  = effi_syst_neg  ; 
      }
      //std::cout << eta_bin << ", " << eta_eff[0][0][eta_bin].eta_min << ", " << eta_eff[0][0][eta_bin].eta_max << std::endl;
      eta_bin++;
      line_count++;
    }
    std::cout << line_count << std::endl;
    inFileEta.close();
  }
  else
  {
    std::cout << "Cannot open " << trigEfficFileName << std::endl;
  }
  // Print out Eta Bins
  for(int eta_i = 0; eta_i < NUMBER_OF_ETA_BINS; eta_i++)
  {
    for(int arm_i = 0; arm_i < NUMBER_OF_ARM_CONFIGS; arm_i++)
    {
      for(int charge_i = 0; charge_i < NUMBER_OF_CHARGE_CONFIGS; charge_i++)
      {
        std::cout << std::setw(8) << "eta_bin: " << eta_i 
           << std::setw(8) << "arm: " << std::setprecision(1) << arm_i 
           << std::setw(8) << "  charge: " << std::setprecision(1) << charge_i 
           << std::setw(16) << " efficiency: " << std::setprecision(6) << eta_eff[arm_i][charge_i][eta_i].effi << std::endl;
      }
    }
  }
  //Pause();
  return 0;
}
