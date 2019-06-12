#include "FFTtools.h"
#include "Analyzer.h"
#include "FilteredAnitaEvent.h"
#include "TruthAnitaEvent.h" 
#include "BasicFilters.h" 
#include "BH13Filter.h" 
#include "SystemResponse.h" 
#include "FilterStrategy.h"
#include "UCUtil.h"
#include "TTree.h"
#include "TFile.h"
#include "UCFilters.h"
#include "AnalysisConfig.h"
#include "AnitaDataset.h"
#include "RawAnitaHeader.h"


void doSim(int run = 1)
{

  FFTtools::loadWisdom("wisdom.dat"); 

//  /*AnalysisWaveform::InterpolationType*/ AnalysisWaveform::defaultInterpolationType = AnalysisWaveform::REGULARIZED_SPARSE_YEN; 

  AnitaDataset d(run,false,WaveCalType::kDefault,AnitaDataset::ANITA_MC_DATA); // Monte Carlo! 
  UCorrelator::AnalysisConfig cfg; 
  cfg.nmaxima = 3;
  cfg.enable_group_delay = false; 
  cfg.saturation_threshold = 1000;
  cfg.fill_channel_info = false;
  cfg.fill_blast_fraction = true; 
  cfg.only_use_usable = true;
  cfg.combine_nantennas = 15; 
  cfg.zoomed_nant = 15; 
  cfg.zoomed_ntheta = 30; 
  //cfg.simulation_time_shift_correction = true;
  //cfg.use_best_antenna_snr = true;

  cfg.response_option = UCorrelator::AnalysisConfig::ResponseA4; 
  cfg.deconvolution_method = new AnitaResponse::AllPassDeconvolution; 

  UCorrelator::Analyzer analyzer(&cfg); 
  double dtheta = 1.; double dphi = 2.; bool blockout = true;
  //analyzer.setTrackSun(dtheta, dphi, blockout);

  FilterStrategy forDeco;
  forDeco.addOperation(new UCorrelator::AntiBH13Filter());
  analyzer.setExtraFiltersDeconvolved(&forDeco);

  TString outname; 
  const char * out_dir = "/project2/avieregg/a4sim/";
  outname.Form("%s/%d.root",out_dir,run); 

  TFile ofile(outname, "RECREATE"); 
  TTree * tree = new TTree("simulation"," Simulated events"); 
  AnitaEventSummary * sum = new AnitaEventSummary; 
  
  FilterStrategy strategy; 
  UCorrelator::fillStrategyWithKey(&strategy, "sinsub_10_3_ad_2"); 
  strategy.addOperation(new UCorrelator::BH13Filter());

  RawAnitaHeader *hdr = 0 ; 
  Adu5Pat *patptr = 0; 
  TruthAnitaEvent * truth = 0; 
  tree->Branch("summary",           &sum       ); 
  tree->Branch("header",            &hdr       ); 
  tree->Branch("pat",               &patptr    );
  tree->Branch("truth",               &truth    );
  int ndone = 0; 
 
  printf("n events = %d\n", d.N());
  for (int i =0 ; i < d.N(); i++) {

    d.getEntry(i); 
    if(d.header()->realTime >= 1480725529. && d.header()->realTime <= 1480730679.) continue;
    //if(d.header()->realTime > 1480796518. && d.header()->realTime < 1480804183.) continue;
    
    UsefulAdu5Pat pat(d.gps()); 
    
    printf("Processing event %d (%d)\n",d.header()->eventNumber,ndone); 
    FilteredAnitaEvent ev(d.useful(), &strategy, d.gps(), d.header()); 

    analyzer.analyze(&ev, sum,d.truth()); 

    ofile.cd(); 
    hdr = d.header(); 
    patptr = d.gps(); 
    truth = d.truth(); 

    tree->Fill(); 
  }

  ofile.cd(); 
  tree->Write(); 

  FFTtools::saveWisdom("wisdom.dat"); 
}
