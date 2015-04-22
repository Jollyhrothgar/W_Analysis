#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclasses;

// Functions
#pragma link C++ function ReduceProductionSingleMuons;  // ReduceProductionSingleMuons.C
#pragma link C++ function GeneratePhysicsDistributions; // GeneratePhysicsDistributions.C
#pragma link C++ function CalculateLikelihoodFunctions; // CalculateLikelihoodFunctions.C
#pragma link C++ function StringSplit;                  // HelperFunctions.C
#pragma link C++ function IsSubstring;                  // HelperFunctions.C
#pragma link C++ function LoadFileListToVector;         // HelperFunctions.C
#pragma link C++ function Make1DVarName;                // HelperFunctions.C
#pragma link C++ function Make2DVarName;                // HelperFunctions.C
#pragma link C++ function GetVarMax;                    // HelperFunctions.C
#pragma link C++ function GetCorrVarMax;                // HelperFunctions.C
#pragma link C++ function GetCorrIndicies;              // HelperFunctions.C
#pragma link C++ function MakeNameTitleStubPair;        // HelperFunctions.C 
#pragma link C++ function GetArmIndex;                  // HelperFunctions.C
#pragma link C++ function GetChargeIndex;               // HelperFunctions.C
#pragma link C++ function GetVarName;                   // HelperFunctions.C
#pragma link C++ function GetLikelihoodIndex;           // HelperFunctions.C
#pragma link C++ function Count_Generated_Events;       // HelperFunctions.C
#pragma link C++ function CheckNan;                     // HelperFunctions.C
#pragma link C++ function ReduceProduction;             // ReduceProduction.C
#pragma link C++ function ReduceProductionChain;        // ReduceProductionChain.C
#pragma link C++ function MergeTrees;                   // MergeTrees.C
#pragma link C++ function FilterEvents;                 // FilterEvents.C
#pragma link C++ function DuplicateSearch;              // DuplicateEvents.C
#pragma link C++ function TranslateTree;                // TranslateTree.C

// Classes
#pragma link C++ class SingleMuons-!;
#pragma link C++ class SingleMuonsLikelihood-!;
#pragma link C++ class dw23VsWness-!;
#pragma link C++ class Plotter-!;
#pragma link C++ struct BasicCutTreeVariables-!;
#pragma link C++ class Run13Triggers-!;
#pragma link C++ class W2eGetPol-!;
#pragma link C++ class SBRextract-!;
#pragma link C++ class AsymmetryCalculator-!;
#pragma link C++ class EventComparison-!;
#pragma link C++ class HadronicBackgroundSbrPdf-!;
#pragma link C++ class MuonBackgroundSbrPdf-!;
#pragma link C++ class WSignalSbrPdf-!;
#pragma link C++ class std::vector<BasicCutTreeVariables>-!;
#pragma link C++ class std::vector<BasicCutTreeVariables*>-!;
#pragma link C++ class std::vector<TH1F*>-!;
#pragma link C++ class std::vector<TH2F*>-!;
#pragma link C++ class std::vector<TTree*>-!;
#pragma link C++ class std::vector<TFile*>-!;
#pragma link C++ class std::vector<TF1*>-!;
#pragma link C++ class std::vector<TF2*>-!;
#pragma link C++ class std::vector<TGraph*>-!;
#pragma link C++ class std::vector<TGraphErrors*>-!;
#pragma link C++ class std::vector<TChain*>-!;
#pragma link C++ class std::vector<TProfile*>-!;
#pragma link C++ class std::vector<TCanvas*>-!;
#pragma link C++ class std::vector<TGraph2D*>-!;
#pragma link C++ class std::vector<TGraph2DErrors*>-!;
#pragma link C++ class std::vector<TObject*>-!;
#pragma link C++ class std::vector<TH1*>+;
#pragma link C++ class std::vector<std::pair<std::string, double> >-!;
#endif
