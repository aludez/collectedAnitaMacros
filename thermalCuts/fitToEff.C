TF1* ef; TF1* f2; TF1* f3; TF1* f4;
double finter(double* x, double* par)
{
	return TMath::Abs(ef->EvalPar(x,par) - f2->EvalPar(x, par));
}

double finter2(double* x, double* par)
{
	return TMath::Abs(ef->EvalPar(x,par) - f3->EvalPar(x, par));
}

double finter3(double* x, double* par)
{
	return TMath::Abs(ef->EvalPar(x,par) - f4->EvalPar(x, par));
}

void fitToEff(){

  //gStyle->SetOptStat(0);
	double nsamp = 5093338;
	double halfBG = .5/nsamp;
	double lowBG = .05/nsamp;

	double nBGnoCuts_100 = 96307012;
	double nBGCuts = 74003556;
	//TFile* f = new TFile("thermalCuts/thermalCuts_wais_100.root");
	//TFile* f = new TFile("thermalCuts/thermalCuts_wais.root");
	TFile* f = new TFile("thermalCuts/thermalCuts_sig.root");

	TH1D* effSFisher =  (TH1D*) f->Get("thermal/Method_Fisher/Fisher/MVA_Fisher_effS");
	TH1D* effBFisher =  (TH1D*) f->Get("thermal/Method_Fisher/Fisher/MVA_Fisher_effB");
	//TH1D* effSFisher =  (TH1D*) f->Get("thermal/Method_BDT/BDT/MVA_BDT_effS");
	//TH1D* effBFisher =  (TH1D*) f->Get("thermal/Method_BDT/BDT/MVA_BDT_effB");

	TCanvas* c1 = new TCanvas("c1", "c1", 1000,800);
	c1->Divide(1,2);
	
	effSFisher->SetLineColor(kBlue);
	effBFisher->SetLineColor(kRed);
	effSFisher->SetLineWidth(2);
	effBFisher->SetLineWidth(2);

  effSFisher->SetTitle("Signal Efficiency");
  effBFisher->SetTitle("Background Efficiency");

  effSFisher->GetYaxis()->SetTitle("Fraction of Events above Fisher Score");
  effSFisher->GetYaxis()->SetTitleSize(.045);
  effBFisher->GetYaxis()->SetTitle("Fraction of Events above Fisher Score");
  effBFisher->GetYaxis()->SetTitleSize(.045);

  effSFisher->GetXaxis()->SetTitle("Fisher Score");
  effSFisher->GetXaxis()->SetTitleSize(.045);
  effBFisher->GetXaxis()->SetTitle("Fisher Score");
  effBFisher->GetXaxis()->SetTitleSize(.045);

	c1->cd(1);
	gPad->SetLogy();
	effBFisher->GetYaxis()->SetRangeUser(1e-10, 1);
	//effBFisher->GetXaxis()->SetRangeUser(-7.5, 7.5);
	effBFisher->GetXaxis()->SetRangeUser(-25, 25);
	effBFisher->Draw();
	c1->cd(2);
	//gPad->SetLogy();
	effSFisher->GetYaxis()->SetRangeUser(0, 1);
	//effSFisher->GetXaxis()->SetRangeUser(-7.5, 7.5);
	effSFisher->GetXaxis()->SetRangeUser(-25, 25);
	effSFisher->Draw();

	double x[10000-2];
	double y[10000-2];
	double x2[10000-2];
	double y2[10000-2];

	for(int i = 1; i < 10000-1; i ++)
	{
		x[i-1]  = effBFisher->GetBinCenter(i);
		y[i-1]  = effBFisher->GetBinContent(i);
		x2[i-1] = effSFisher->GetBinCenter(i);
		y2[i-1] = effSFisher->GetBinContent(i);
	}

	TGraph* g  = new TGraph(10000-2, x, y);
	TGraph* g2 = new TGraph(10000-2, x2, y2);

	g->Fit("expo", "", "", -5, -3.5);
	//g->Fit("expo", "", "", -5,-2);
	//g->Draw("alp");
	ef = (TF1*) g->GetFunction("expo");
	ef->SetLineStyle(7);
	c1->cd(1);
	ef->DrawF1(-8,10, "same");
	f2 = new TF1("f2", "1.24e-8", -10,10);
	f3 = new TF1("f3", "1.24e-9", -10,10);
	f4 = new TF1("f4", "4.48e-10", -10,10);
	TF1* fint = new TF1("fint", finter, -10,10,0);
	TF1* fint2 = new TF1("fint2", finter2, -10,10,0);
	TF1* fint3 = new TF1("fint3", finter3, -10,10,0);
	double xint = fint->GetMinimumX();
	double xint2 = fint2->GetMinimumX();
	double xint3 = fint3->GetMinimumX();
	printf("xint = %g, eff at .5 bg = %g\n", xint, g2->Eval(xint));
	printf("xint = %g, eff at .05 bg = %g\n", xint2, g2->Eval(xint2));
	printf("xint = %g, eff at .01 bg = %g\n", xint3, g2->Eval(xint3));

	TLine* l = new TLine(xint, 0, xint, 1);
	TLine* l2 = new TLine(xint2, 0, xint2, 1);
	TLine* l3 = new TLine(xint3, 0, xint3, 1);
	l->Draw();
	l2->Draw();
	l3->Draw();
	c1->cd(2);
	l->Draw();
	l2->Draw();
	l3->Draw();
}
