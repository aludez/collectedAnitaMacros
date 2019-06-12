{
gROOT->ProcessLine(".x macros/setupProof.C "); 
gROOT->ProcessLine(" .L macros/plotCuts.C "); 

TChain decimated("sumTree"); 
for(int i = 41; i < 368; i++)
{
	if(i == 46 || i == 60 || i == 61) continue;	
	//decimated.Add(Form("/project/avieregg/a4out/decimated/run%d.root", i));
	decimated.Add(Form("/project2/avieregg/a4out/run%d.root", i));
}
decimated.SetProof();

TChain wais("sumTree"); wais.Add("/project/avieregg/a4out/wais/*.root"); wais.SetProof();

plotThermalCuts(&wais,&decimated,"cuts_deconvolved.pdf");

}

