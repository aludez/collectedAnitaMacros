#include "FFTtools.h" 
#include "AnitaConventions.h" 
#include "AnitaDataset.h" 
#include "Hical2.h"
#include "AcclaimClustering.h"
#include "FilteredAnitaEvent.h"
#include "FilterStrategy.h"
#include "TGraph.h"
#include "WaveformCombiner.h"

using namespace Acclaim;


int makeVPOLTree(int start_run = 1, int end_run = 1, const char * prefix = "mc_trees/")
{
  TChain c("sumTree"); 
  UCorrelator::WaveformCombiner wfcomb(15, 3, true, false, 0);
  c.Add("thermalCutTrees/sim/simulation_1-250.root");
  c.Add("thermalCutTrees/sim/simulation_251-500.root");

  int n = c.Draw("run:eventNumber+int(lastFewDigits):weight:phi:theta",  TCut(TString::Format("(F2 >= -1) && run >= %d && run <= %d", start_run,end_run)) , "goff" ); 
  printf("%d events pass selection\n", n); 

  TFile * f  = new TFile(TString::Format("%s%d_%d.root",prefix,start_run, end_run), "RECREATE"); 

  TTree * tr = new TTree("events","events"); 
  int run, ev; 
  double CRcorr, CRcorrdeco, PAngleUp, PAngle, cohpolAngle, decopolAngle, CRPoldeco, CRPolcoh, polFrac, corr, totalPolFrac, instcohtotalPolFrac, instdecototalPolFrac, instcohpolFrac, instdecopolFrac, weight;

  tr->Branch("event",&ev); 
  tr->Branch("run",&run); 
  tr->Branch("cohpolAngle",&cohpolAngle); 
  tr->Branch("polFrac",&polFrac); 
  tr->Branch("instcohpolFrac",&instcohpolFrac); 
  tr->Branch("instdecopolFrac",&instdecopolFrac); 
  tr->Branch("totalPolFrac", &totalPolFrac);
  tr->Branch("instcohtotalPolFrac", &instcohtotalPolFrac);
  tr->Branch("instdecototalPolFrac", &instdecototalPolFrac);
  //tr->Branch("corr", &corr);
  tr->Branch("weight", &weight);
  

  int loaded_run = 0; 
  TFile * sumfile = 0; 
  TTree * sumtree = 0; 
  AnitaEventSummary* sum = 0;

  AnitaDataset d(start_run, false, WaveCalType::kDefault, AnitaDataset::ANITA_MC_DATA);

  TFile fwais("../anitaBuildTool/components/AnitaAnalysisFramework/data/templates/waisTemplateA4.root");
  TGraph* gwais = (TGraph*) fwais.Get("wais");
  AnalysisWaveform awf(gwais->GetN(), gwais->GetX(), gwais->GetY(), 0.096154);
  TGraph * wais = new TGraph(1561);
  for(int i = 0; i < wais->GetN(); i++)
  {
    wais->SetPoint(i, awf.even()->GetX()[i], awf.even()->GetY()[i]);
  }

  for (int i = 0; i < n; i++) 
  {
    weight = c.GetV3()[i];

    run = c.GetV1()[i]; 
    if (run!= loaded_run)
    {
      if (sumfile) delete sumfile; 
      sumfile = new TFile(TString::Format("/project2/avieregg/a4sim/%d.root",run));; 
      gROOT->cd(); 
      sumtree = (TTree*) sumfile->Get("simulation"); 
      sumtree->SetBranchAddress("summary",&sum); 
      sumtree->BuildIndex("eventNumber"); 
      //d.loadRun(run);
      loaded_run = run; 
      d.getEntry(0);
    }
      
    ev = int(c.GetV2()[i]);
    sumtree->GetEntryWithIndex(ev);
    for(int j = d.current(); j < d.N(); j++)
    {
      d.getEntry(j);
      if(int(d.header()->eventNumber) == ev) break;
    }

    cohpolAngle = sum->mostImpulsiveCoherentFiltered(2).linearPolAngle();
    decopolAngle = sum->mostImpulsiveCoherentFiltered(2).linearPolAngle();
    polFrac = (sum->mostImpulsiveCoherentFiltered(2).linearPolFrac()+sum->mostImpulsiveDeconvolvedFiltered(2).linearPolFrac()+sum->mostImpulsiveCoherentFiltered(2).instantaneousLinearPolFrac()+sum->mostImpulsiveDeconvolvedFiltered(2).instantaneousLinearPolFrac())/4.;
    instcohpolFrac = sum->mostImpulsiveCoherentFiltered(2).instantaneousLinearPolFrac();
    instdecopolFrac = sum->mostImpulsiveDeconvolvedFiltered(2).instantaneousLinearPolFrac();

    totalPolFrac = (sum->mostImpulsiveCoherentFiltered(2).totalPolFrac()+sum->mostImpulsiveDeconvolvedFiltered(2).totalPolFrac()+sum->mostImpulsiveCoherentFiltered(2).instantaneousTotalPolFrac()+sum->mostImpulsiveDeconvolvedFiltered(2).instantaneousTotalPolFrac())/4.;
    instcohtotalPolFrac = sum->mostImpulsiveCoherentFiltered(2).instantaneousTotalPolFrac();
    instdecototalPolFrac = sum->mostImpulsiveDeconvolvedFiltered(2).instantaneousTotalPolFrac();

    /*
    FilterStrategy strat;
    FilteredAnitaEvent fae(d.useful(), &strat, d.gps(), d.header());
    wfcomb.combine(sum->mostImpulsivePeak(2).phi, sum->mostImpulsivePeak(2).theta, &fae, AnitaPol::kVertical, 0, -25, 125);
    TGraph * g = new TGraph(wfcomb.getCoherent()->even()->GetN(), wfcomb.getCoherent()->even()->GetX(), wfcomb.getCoherent()->even()->GetY());
    //printf("%d, %d\n", g->GetN(), wais->GetN());

    TGraph * gCorr = FFTtools::getCorrelationGraph(wais, g);
    double normalization = 1./(g->GetRMS(2) * wais->GetRMS(2) * g->GetN()/TMath::Power(2, int(TMath::Log2(g->GetN()))));
    double maxCorr = normalization * TMath::MaxElement(gCorr->GetN(), gCorr->GetY());
    double minCorr = normalization * TMath::MinElement(gCorr->GetN(), gCorr->GetY());
    corr = TMath::Max(maxCorr, abs(minCorr));
    delete g;
    delete gCorr;
    
    if(int(c.GetV3()[i]) == 0)
    {
      cohpolAngle = 90 - abs(cohpolAngle);
      decopolAngle = 90 - abs(decopolAngle);
    }
    */

    tr->Fill(); 

  }

  f->cd(); 
  tr->Write(); 
  return 0; 
}




