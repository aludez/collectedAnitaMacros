
#include "FFTtools.h"
#define CUTS_LOADED
#include "AnitaTMVA.h" 


void doTMVAMC(int nworkers = 1) 
{
  TChain* cbg = new TChain("sumTree");
  cbg->Add("thermalCutTrees/data/*");
//  TTree* wais = cbg->CopyTree("isWais");
  
  TChain* csig = new TChain("sumTree");
  csig->Add("thermalCutTrees/sim/*");

  TString tmvaOutName; 
  tmvaOutName.Form("thermalCuts/thermalCuts_2var.root"); 

  TFile tmvaOut(tmvaOutName.Data(),"RECREATE"); 

  TMVA::Factory *factory = new TMVA::Factory("thermal_cuts2var", &tmvaOut,"V"); 

  TMVA::DataLoader *dl = new TMVA::DataLoader("thermal2var"); 
  dl->SetSignalWeightExpression("weight"); 

  /* These are the variables to be used. They must have been generated already */ 
  //dl->AddVariable("mapPeak"); 
  //dl->AddVariable("bandwidth"); 
  //dl->AddVariable("avgPeak"); 
  //dl->AddVariable("decoHilbertPeak"); 
  //dl->AddVariable("coherentHilbertPeak"); 
  //dl->AddVariable("deltaHilbert := decoHilbertPeak - coherentHilbertPeak");
  //dl->AddVariable("peakDiff"); 
  //dl->AddVariable("newVar := avgPeak/coherentHilbertPeak*(avgPeak - coherentHilbertPeak)"); 
  //dl->AddVariable("coherence := avgPeak / coherentHilbertPeak"); 
  //dl->AddVariable("decoImpulsivity"); 
  //dl->AddVariable("coherentImpulsivity"); 
  //dl->AddVariable("impulsivityDiff"); 
  //dl->AddVariable("impfrac := coherentImpulsivity / decoImpulsivity"); 
  //dl->AddVariable("cohLinearPolFraction"); 
  //dl->AddVariable("decoLinearPolFraction"); 
  //dl->AddVariable("cohInstLinearPolFraction"); 
  //dl->AddVariable("decoInstLinearPolFraction"); 
	dl->AddVariable("powerDiff");
  dl->AddVariable("altDecoImpulsivity"); 
  //dl->AddVariable("altCoherentImpulsivity"); 
  //dl->AddVariable("altImpDiff := pow(altDecoImpulsivity,2) - pow(altCoherentImpulsivity,2)"); 
  //dl->AddVariable("altImpDiff := altDecoImpulsivity - altCoherentImpulsivity"); 
  //dl->AddVariable("altImpulsivityFrac"); 
  //dl->AddVariable("pickVal := decoImpulsivity * mapPeak"); 
  //dl->AddVariable("dPhiSun");

  dl->AddSpectator("run"); 
  dl->AddSpectator("eventNumber"); 
  dl->AddSpectator("lastFewDigits"); 

  dl->AddSignalTree(csig); 
//  dl->AddSignalTree(wais);
  dl->AddBackgroundTree(cbg); 

  //dl->AddCut(TCut("mapPeak > .02 "));
  dl->PrepareTrainingAndTestTree(0, TCut("theta < 0"), "");

  //setup methods 
  factory->BookMethod(dl, TMVA::Types::kFisher, "Fisher","CreateMVAPdfs=false"); 
//  factory->BookMethod(dl, TMVA::Types::kKNN, "kNN"); 
  //factory->BookMethod(dl, TMVA::Types::kBDT, "BDT","CreateMVAPdfs=false:BoostType=AdaBoost:NTrees=800"); 
  //factory->BookMethod(dl, TMVA::Types::kBDT, "BDTgrad","CreateMVAPdfs=true:BoostType=Grad:UseBaggedGrad:NTrees=800"); 
  //factory->BookMethod(dl, TMVA::Types::kBDT, "BDT","CreateMVAPdfs=true:BoostType=Grad:NTrees=800"); 
//  factory->BookMethod(dl, TMVA::Types::kMLP, "MLP"); 
  //factory->BookMethod(dl, TMVA::Types::kSVM, "SVM"); 


  factory->TrainAllMethods(); 
  factory->TestAllMethods(); 
  factory->EvaluateAllMethods(); 

  tmvaOut.Write(); 
}


