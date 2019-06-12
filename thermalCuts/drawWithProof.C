{
	gROOT->ProcessLine(".x macros/setupProof.C");
	
	TChain* c = new TChain("sumTree");
	for(int i = 41; i < 368; i++){
		if(i == 46 || i == 60 || i == 61) continue;
		//c->Add(Form("/project/avieregg/a4out/decimated/run%d.root", i));
		c->Add(Form("/project2/avieregg/a4out/run%d.root", i));
	}
	AnitaEventSummary* csum = new AnitaEventSummary;
	c->SetBranchAddress("summary", &csum);
	c->SetProof();

	TChain* wais = new TChain("sumTree");
	for(int i = 122; i < 153; i++) wais->Add(Form("/project/avieregg/a4out/wais/run%d.root", i));
	AnitaEventSummary* waissum = new AnitaEventSummary;
	wais->SetBranchAddress("summary", &waissum);
	wais->SetProof();

	TCanvas* can = new TCanvas("cut", "cut", 1000, 800);
	TH2D* h = new TH2D("map", "map", 300, 0, 500, 100,0,.5);
	TH2D* h2 = new TH2D("map2", "map2", 300, 0, 500,100,0,.5);
	//TH1D* h = new TH1D("map", "map", 200, 0, 10);
	//TH1D* h2 = new TH1D("map2", "map2", 200, 0, 10);
	TEntryList* tempList;
	//this actually draws histos
	can->cd();
	c->Draw("mostImpulsivePeak(2).value:mostImpulsiveDeconvolvedFiltered(2).peakHilbert>>map", totalBlastCut && coherenceCut && moreImpulsiveDeco && !stepFn && isRealImpulsive && !isWais, "colz");
	//c->Draw("mostImpulsiveDeconvolved(2).bandwidthMeasure>>map", totalBlastCut && coherenceCut && !stepFn && !isWais && moreImpulsiveDeco && isRealImpulsive, "");
	//wais->Draw("flags.blastFraction>>map2", "abs(FFTtools::wrap(mostImpulsivePeak().phi-wais.phi,360,0))<5", "");
	//wais->Draw("mostImpulsiveCoherentFiltered().bandwidthMeasure>>map2", isWaisImpulsive, "");
	wais->Draw("mostImpulsivePeak(2).value:mostImpulsiveDeconvolvedFiltered(2).peakHilbert>>map2", isWaisImpulsive, "");
	//wais->Draw("mostImpulsiveDeconvolved(2).bandwidthMeasure>>map2", isWaisImpulsive, "");

	h->DrawCopy("colz");
	h2->SetMarkerColor(6);
	h2->SetLineColor(6);
	h2->DrawCopy("same");
/*	
	//this stuff prints out playslists based on cuts 
	AnitaEventSummary::setThresholdForMostImpulsive(.8);
	c->Draw(">>thermEvents", impulsiveThermals && lookAt, "entrylist");
	//c->Draw(">>thermEvents", impulsiveThermals, "entrylist");
	TEntryList *thermEvents = (TEntryList*)gProof->GetOutputList()->FindObject("thermEvents");
	std::ofstream tev("../anitaBuildTool/components/anitaMagicDisplay/thermEv.txt");
	std::vector<long> thermVec;
	long long offSet = 0;
	for(int i = 40; i < 368; i++){
		if(i == 46 || i == 60 || i == 61) continue;
		tempList = thermEvents->GetEntryList("sumTree", Form("/project/avieregg/a4out/decimated/run%d.root", i));
		for(long j = 0; j < tempList->GetN(); j++)
		{
			Long64_t chainEntry = tempList->GetEntry(j) + offSet;
			c->GetEntry(chainEntry);
			thermVec.push_back(csum->eventNumber);
		}
		TChain tempChain("sumTree");
		tempChain.Add(Form("/project/avieregg/a4out/decimated/run%d.root", i));
		offSet+=tempChain.GetEntries();
	}
	std::sort(thermVec.begin(), thermVec.end());
	for(long i = 0; i < thermVec.size(); i++) tev << thermVec[i] << "\n";
	thermVec.clear();
	tev.close();


	wais->Draw(">>waisEvents", "mostImpulsiveDeconvolvedFiltered().impulsivityMeasure < .6 && mostImpulsiveDeconvolved().impulsivityMeasure < .6 && abs(FFTtools::wrap(mostImpulsivePeak().phi-wais.phi,360,0))<5", "entrylist");
	TEntryList *waisEvents = (TEntryList*)gProof->GetOutputList()->FindObject("waisEvents");
	std::ofstream wev("../anitaBuildTool/components/anitaMagicDisplay/waisEv.txt");
	std::vector<long> wV;
	offSet = 0;
	for(int i = 122; i < 153; i++)
	{
		tempList = waisEvents->GetEntryList("sumTree", Form("/project/avieregg/a4out/wais/run%d.root", i));
		for(long j = 0; j < tempList->GetN(); j++)
		{
			Long64_t chainEntry = tempList->GetEntry(j) + offSet;
			wais->GetEntry(chainEntry);
			wV.push_back(waissum->eventNumber);
		}
		TChain tempChain("sumTree");
		tempChain.Add(Form("/project/avieregg/a4out/wais/run%d.root", i));
		offSet+=tempChain.GetEntries();
	}
	std::sort(wV.begin(), wV.end());
	for(int i=0; i < wV.size(); i++) wev << wV[i] << "\n";
	wev.close();
*/
}
