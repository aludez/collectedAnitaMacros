{
  AnitaResponse::ResponseManager responses("A4ImpulseTUFFs", 50, new AnitaResponse::AllPassDeconvolution);
  UCorrelator::WaveformCombiner wfcomb(15, 3, true, true, &responses);
  //UCorrelator::WaveformCombiner wfcomb(15, 3, false, true, &responses);

  TFile f("crsummaries_coherent_snr.root");
  TTree* crs = (TTree*) f.Get("sumTree");
  AnitaEventSummary* aes = new AnitaEventSummary;
  crs->SetBranchAddress("summary", &aes);

  PolarityMachine apm;
  //apm.setWindowSizeNs(10.);
  apm.setWindowSizeNs(5.);
  //apm.setTestCoherent(false);

  //AnitaDataset d(41, false, WaveCalType::kDefault, AnitaDataset::ANITA_ROOT_DATA, AnitaDataset::kRandomizePolarity);
  AnitaDataset d(41);
  FilterStrategy strategy;
  //strategy.addOperation(new SimplePassBandFilter(0, 0.6));
  //UCorrelator::fillStrategyWithKey(&strategy, "sinsub_10_3_ad_2");


  //TFile* fout = new TFile("polarityTest_candidates_fourier.root", "RECREATE");

  int which_metric = 5;
  TH2D* h = new TH2D("h", "h", 100, 0, 3, 100, 0, 3);
  TH1D* hdeco = new TH1D("h2", "h2", 100, 0, 3);
  AnalysisWaveform* wf = 0;

  std::vector<int> evNums;
  std::vector<double> coh_corr;
  std::vector<double> deco_corr;

  //TFile* fout = new TFile("additional_corr4.root", "RECREATE");
  TGraph* gs[30];
  TGraph* gcs[30];
  
  //TCanvas* c1 = new TCanvas("c1", "c1", 1800, 1200);
  //c1->Divide(6,5);
  //TCanvas* c2 = new TCanvas("c2", "c2", 1800, 1200);
  //c2->Divide(6,5);

  int j = 0;

  for(int i = 0; i < crs->GetEntries(); i++)
  //for(int i = 0; i < 2; i++)
  {
    crs->GetEntry(i);
    //if(aes->eventNumber == 50549772 || aes->eventNumber == 17904564) continue;
    //if(aes->eventNumber != 50549772) continue;
    //if(aes->eventNumber != 17904564) continue;
    //if(aes->eventNumber != 36785931) continue;
    //if(aes->eventNumber != 4098827) continue;
    //if(aes->eventNumber != 74197411) continue;
    //printf("%d\n", i);
    
    d.getEvent(aes->eventNumber);
    apm.loadTemplates(d.header()->realTime);
    FilteredAnitaEvent fae(d.useful(), &strategy, d.gps(), d.header());

    wfcomb.combine(aes->mostImpulsivePeak(2).phi, aes->mostImpulsivePeak(2).theta, &fae, AnitaPol::kHorizontal, 0, -25, 125);
    wf = (AnalysisWaveform*) wfcomb.getCoherent();
    double polarity = apm.testPolarity(which_metric, wf, 0);
    //double polarity = 0;
    gs[j] = new TGraph(apm.getInputWaveform()->GetN(), apm.getInputWaveform()->GetX(), apm.getInputWaveform()->GetY());
    gs[j]->SetTitle(Form("gwf%d", aes->eventNumber));
    gs[j]->SetName(Form("gwf%d", aes->eventNumber));
    gcs[j] = new TGraph(apm.getCorrelationGraph()->GetN(), apm.getCorrelationGraph()->GetX(), apm.getCorrelationGraph()->GetY());
    gcs[j]->SetTitle(Form("gcorr%d", aes->eventNumber));
    gcs[j]->SetName(Form("gcorr%d", aes->eventNumber));
    wf = (AnalysisWaveform*) wfcomb.getDeconvolved();
    double polarity_deco = apm.testPolarity(which_metric, wf, 1);
    evNums.push_back(aes->eventNumber);
    coh_corr.push_back(polarity);
    deco_corr.push_back(polarity_deco);

    printf("ev = %d, polarity = %g, decopol = %g\n", aes->eventNumber, polarity, polarity_deco);
    //fout->cd();
    //gs[j]->Write();
    //gcs[j]->Write();

    //c1->cd(j+1);
    //gs[j]->Draw();
    //c2->cd(j+1);
    //gcs[j]->Draw();
    j++;
    //printf("starting the simulation\n");
    //TH2D* h = apm.runPolaritySimulation(1000, aes->eventNumber, aes->mostImpulsivePeak(2).phi, aes->mostImpulsivePeak(2).theta, aes->mostImpulsiveCoherent(2).snr, 0, which_metric, Form("%d", aes->eventNumber));
    //fout->cd();
    //h->Write();
    //delete h;
  }
  /*
  gcs[0]->Draw();
  TGraph* gcr = apm.getCRWaveform();
  gcr->SetName("crwf");
  fout->cd();
  gcr->Write();
  TCanvas* c1 = new TCanvas("c1", "c1", 1800, 1200);
  c1->Divide(6,5);
  for(int i = 0; i < evNums.size(); i++)
  {
    c1->cd(i+1);
    gs[i]->GetXaxis()->SetRangeUser(0.2, 0.6);
    gs[i]->Draw();
  }
  TGraph* cr = new TGraph(apm.getCRWaveform()->GetN(), apm.getCRWaveform()->GetX(), apm.getCRWaveform()->GetY());
  TGraph* cr_flipped = new TGraph(cr->GetN(), cr->GetX(), cr->GetY());
  for(int i = 0; i < cr_flipped->GetN(); i++) cr_flipped->GetY()[i] *= -1;
  cr->SetLineColor(kRed);
  cr_flipped->SetLineColor(kRed);
  */

  //fout->cd();
  //h->Write();
  //fout->Close();
}
    
