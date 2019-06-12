#include "FFTtools.h" 
#include "AnitaConventions.h" 
#include "AnitaDataset.h" 
#include "Hical2.h"
#include "AcclaimClustering.h"

using namespace Acclaim;


void addRuns(TChain & c, int start_run, int end_run)
{
  for (int i = 41; i <= 367; i+=10) 
  {
    if (start_run < i + 10 && end_run >= i)
    {
      TString adding = TString::Format("thermalCutTrees/data/a4all_%d-%d.root",i,i+9);
      printf("Adding %s\n", adding.Data() ); 
      c.Add(adding.Data() ); 
    }
  }
}


int makeCRTree(int start_run = 41, int end_run = 41, const char * prefix = "cr_trees/")
{
  //TFile* fCluster = new TFile("/project/kicp/avieregg/anitaIV/sourceMaps/prod3_F2.12_40km.root");
  TFile* fCluster = new TFile("/project/kicp/avieregg/anitaIV/sourceMaps/prod3_recreated.root");
  //TFile* fCluster = new TFile("/project/kicp/avieregg/anitaIV/sourceMaps/allNoBase_40km_F-.2.root");
  TTree* events = (TTree*) fCluster->Get("eventTree");
  TTree* clusters3 = (TTree*) fCluster->Get("clusterTree3"); //6
  TTree* clusters4 = (TTree*) fCluster->Get("clusterTree4"); //8
  TTree* clusters5 = (TTree*) fCluster->Get("clusterTree5"); //10
  TTree* clusters6 = (TTree*) fCluster->Get("clusterTree6"); //12
  TTree* clusters7 = (TTree*) fCluster->Get("clusterTree7"); //15
  TTree* clusters8 = (TTree*) fCluster->Get("clusterTree8"); //20
  Acclaim::Clustering::Cluster* cluster_3 = 0;
  Acclaim::Clustering::Cluster* cluster_4 = 0;
  Acclaim::Clustering::Cluster* cluster_5 = 0;
  Acclaim::Clustering::Cluster* cluster_6 = 0;
  Acclaim::Clustering::Cluster* cluster_7 = 0;
  Acclaim::Clustering::Cluster* cluster_8 = 0;
  Acclaim::Clustering::Event* event = 0;
  clusters3->SetBranchAddress("cluster", &cluster_3);
  clusters4->SetBranchAddress("cluster", &cluster_4);
  clusters5->SetBranchAddress("cluster", &cluster_5);
  clusters6->SetBranchAddress("cluster", &cluster_6);
  clusters7->SetBranchAddress("cluster", &cluster_7);
  clusters8->SetBranchAddress("cluster", &cluster_8);
  events->SetBranchAddress("event", &event);
  events->BuildIndex("eventNumber");

  TChain c("sumTree"); 
  UCorrelator::WaveformCombiner wfcomb(15, 3, true, false, 0);

  addRuns(c,start_run,end_run); 

  int n = c.Draw("run:eventNumber+int(lastFewDigits):pol:phi:theta",  TCut(TString::Format("(F2 >= -1) && (run >= %d && run <= %d && !isWais)", start_run,end_run)) , "goff" ); 
  printf("%d events pass selection\n", n); 

  TFile * f  = new TFile(TString::Format("%s%d_%d.root",prefix,start_run, end_run), "RECREATE"); 

  TTree * tr = new TTree("events","events"); 
  int run, ev; 
  double CRcorr, CRcorrdeco, PAngleUp, PAngle, cohpolAngle, decopolAngle, CRPoldeco, CRPolcoh, polFrac, corr, totalPolFrac, instcohtotalPolFrac, instdecototalPolFrac, instcohpolFrac, instdecopolFrac;
  int cluster3, cluster4, cluster5, cluster6, cluster7, cluster8;

  tr->Branch("event",&ev); 
  tr->Branch("run",&run); 
  tr->Branch("cohpolAngle",&cohpolAngle); 
  tr->Branch("polFrac",&polFrac); 
  tr->Branch("instcohpolFrac",&instcohpolFrac); 
  tr->Branch("instdecopolFrac",&instdecopolFrac); 
  tr->Branch("totalPolFrac", &totalPolFrac);
  tr->Branch("instcohtotalPolFrac", &instcohtotalPolFrac);
  tr->Branch("instdecototalPolFrac", &instdecototalPolFrac);
  tr->Branch("pa_diff", &PAngle);
  //tr->Branch("corr", &corr);
  tr->Branch("cluster3num", &cluster3);
  tr->Branch("cluster4num", &cluster4);
  tr->Branch("cluster5num", &cluster5);
  tr->Branch("cluster6num", &cluster6);
  tr->Branch("cluster7num", &cluster7);
  tr->Branch("cluster8num", &cluster8);
  

  int loaded_run = 0; 
  TFile * sumfile = 0; 
  TTree * sumtree = 0; 
  AnitaEventSummary* sum = 0;

  AnitaDataset d(start_run);

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
    if(int(c.GetV3()[i]) == 1) continue; //don't look at V

    run = c.GetV1()[i]; 
    if (run!= loaded_run)
    {
      if (sumfile) delete sumfile; 
      sumfile = new TFile(TString::Format("/project/kicp/avieregg/anitaIV/a4out/run%d.root",run));; 
      gROOT->cd(); 
      sumtree = (TTree*) sumfile->Get("sumTree"); 
      sumtree->SetBranchAddress("summary",&sum); 
      sumtree->BuildIndex("eventNumber"); 
      d.loadRun(run);
      loaded_run = run; 
    }
      
    ev = int(c.GetV2()[i]);
    if(events->GetEntryWithIndex(ev) == -1) continue;
    sumtree->GetEntryWithIndex(ev);
    if(event->selfLogLikelihood > 25) continue;
    clusters3->GetEntry(event->cluster[3]);
    if(cluster_3->numDataEvents > 100) continue;
    UsefulAdu5Pat upat(d.gps());

    cohpolAngle = sum->mostImpulsiveCoherent(2).instantaneousLinearPolAngle();
    decopolAngle = sum->mostImpulsiveCoherentFiltered(2).linearPolAngle();
    polFrac = (sum->mostImpulsiveCoherentFiltered(2).linearPolFrac()+sum->mostImpulsiveDeconvolvedFiltered(2).linearPolFrac()+sum->mostImpulsiveCoherentFiltered(2).instantaneousLinearPolFrac()+sum->mostImpulsiveDeconvolvedFiltered(2).instantaneousLinearPolFrac())/4.;
    instcohpolFrac = sum->mostImpulsiveCoherentFiltered(2).instantaneousLinearPolFrac();
    instdecopolFrac = sum->mostImpulsiveDeconvolvedFiltered(2).instantaneousLinearPolFrac();

    totalPolFrac = (sum->mostImpulsiveCoherentFiltered(2).totalPolFrac()+sum->mostImpulsiveDeconvolvedFiltered(2).totalPolFrac()+sum->mostImpulsiveCoherentFiltered(2).instantaneousTotalPolFrac()+sum->mostImpulsiveDeconvolvedFiltered(2).instantaneousTotalPolFrac())/4.;
    instcohtotalPolFrac = sum->mostImpulsiveCoherentFiltered(2).instantaneousTotalPolFrac();
    instdecototalPolFrac = sum->mostImpulsiveDeconvolvedFiltered(2).instantaneousTotalPolFrac();

    PAngle = cohpolAngle - TMath::RadToDeg() * GeoMagnetic::getExpectedPolarisation(upat, TMath::DegToRad() * sum->mostImpulsivePeak(2).phi, TMath::DegToRad() * sum->mostImpulsivePeak(2).theta);

    /*
    FilterStrategy strat;
    FilteredAnitaEvent fae(d.useful(), &strat, d.gps(), d.header());
    wfcomb.combine(sum->mostImpulsivePeak(2).phi, sum->mostImpulsivePeak(2).theta, &fae, AnitaPol::kVertical, 0, -25, 125);
    TGraph * g = new TGraph(wfcomb.getCoherent()->even()->GetN(), wfcomb.getCoherent()->even()->GetX(), wfcomb.getCoherent()->even()->GetY());
    printf("%d, %d\n", g->GetN(), wais->GetN());

    TGraph * gCorr = FFTtools::getCorrelationGraph(wais, g);
    double normalization = 1./(g->GetRMS(2) * wais->GetRMS(2) * g->GetN()/TMath::Power(2, int(TMath::Log2(g->GetN()))));
    double maxCorr = normalization * TMath::MaxElement(gCorr->GetN(), gCorr->GetY());
    double minCorr = normalization * TMath::MinElement(gCorr->GetN(), gCorr->GetY());
    corr = TMath::Max(maxCorr, abs(minCorr));
    delete g;
    delete gCorr;
    */
    
    if(int(c.GetV3()[i]) == 0)
    {
      cohpolAngle = 90 - abs(cohpolAngle);
      decopolAngle = 90 - abs(decopolAngle);
    }

    clusters3->GetEntry(event->cluster[3]);
    clusters4->GetEntry(event->cluster[4]);
    clusters5->GetEntry(event->cluster[5]);
    clusters6->GetEntry(event->cluster[6]);
    clusters7->GetEntry(event->cluster[7]);
    clusters8->GetEntry(event->cluster[8]);
    cluster3 = cluster_3->numDataEvents;
    cluster4 = cluster_4->numDataEvents;
    cluster5 = cluster_5->numDataEvents;
    cluster6 = cluster_6->numDataEvents;
    cluster7 = cluster_7->numDataEvents;
    cluster8 = cluster_8->numDataEvents;

    tr->Fill(); 

  }

  f->cd(); 
  tr->Write(); 
  return 0; 
}




