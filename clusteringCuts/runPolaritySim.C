void runPolaritySim(int metric=6, int crNum=0, int offset=0)
{
  //want to run metric 5 and 6
  int i = crNum;
  AnitaResponse::ResponseManager responses("A4ImpulseTUFFs", 50, new AnitaResponse::AllPassDeconvolution);
  UCorrelator::WaveformCombiner wfcomb(15, 3, true, true, &responses);
//  UCorrelator::WaveformCombiner wfcomb(15, 3, false, true, &responses);

  TFile f("crsummaries_coherent_snr.root");
  TTree* crs = (TTree*) f.Get("sumTree");
  AnitaEventSummary* aes = new AnitaEventSummary;
  crs->SetBranchAddress("summary", &aes);

  PolarityMachine apm;
  apm.setWindowSizeNs(10.);
  apm.setOffset(offset);
//  apm.setTestCoherent(false);

  //AnitaDataset d(41, false, WaveCalType::kDefault, AnitaDataset::ANITA_ROOT_DATA, AnitaDataset::kRandomizePolarity);
  AnitaDataset d(41);
  FilterStrategy strategy;
  //strategy.addOperation(new SimplePassBandFilter(0, 0.6));
  //UCorrelator::fillStrategyWithKey(&strategy, "sinsub_10_3_ad_2");

  crs->GetEntry(i);
  TFile* fout = new TFile(Form("polarity_test_files/%d_%d_%d.root", aes->eventNumber, metric, offset), "RECREATE");

  int which_metric = metric;

  std::vector<int> evNums;
  std::vector<double> coh_corr;
  std::vector<double> deco_corr;

  d.getEvent(aes->eventNumber);
  apm.loadTemplates(d.header()->realTime);
  FilteredAnitaEvent fae(d.useful(), &strategy, d.gps(), d.header());

  TH2D* h = apm.runPolaritySimulation(1000, aes->eventNumber, aes->mostImpulsivePeak(2).phi, aes->mostImpulsivePeak(2).theta, aes->mostImpulsiveCoherent(2).snr, 0, which_metric, Form("%d", aes->eventNumber));
  printf("finished up\n");
  
  fout->cd();
  h->Write();
  fout->Close();
}
    
