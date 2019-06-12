#include "AnitaTMVA.h" 

void fillTMVA(const char * file, const char * tree="simulation", const char * weights="thermal/weights/thermal_cuts_Fisher.weights.xml", const char * name="F2", const char * vars_file="usedVars.tmva") 
{
  AnitaTMVA::MVAVarSet vars(vars_file); 

  TFile f(file,"update"); 
  TTree * t = (TTree*) f.Get(tree); 
  AnitaTMVA::evaluateTMVA(t, vars, name, weights); 
  t->Write(); 

}


void fillAllTMVA()
{
  TString str; 
  //str.Form("thermalCutTrees/sim/simulation_%d-%d.root", 1, 10); 
  str.Form("thermalCutTrees/sim/simulation_%d-%d.root", 1, 250); 
  fillTMVA(str.Data(),"sumTree");
  str.Form("thermalCutTrees/sim/simulation_%d-%d.root", 251, 500); 
  fillTMVA(str.Data(),"sumTree");
  /*
  for (int i = 41; i <=367; i+=10) 
  //for (int i = 271; i <=367; i+=10) 
  {
    TString str2; 
    if(i < 360) str2.Form("thermalCutTrees/data/a4all_%d-%d.root", i, i + 9); 
    if(i == 361) str2.Form("thermalCutTrees/data/a4all_%d-%d.root", i, 367); 
    fillTMVA(str2.Data(),"sumTree"); 
  }
  */
}


