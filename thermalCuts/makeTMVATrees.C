
#include "FFTtools.h"
#define CUTS_LOADED

TCut isRealImpulsive = "mostImpulsivePeak(2).value > 0 && flags.isRF == 1 && mostImpulsivePeak(2).theta < 60 && mostImpulsivePeak(2).theta > -50";
TCut isWais = "flags.pulser == 1 || flags.pulser==5";

TCut isMCImpulsive = "abs(FFTtools::wrap(mostImpulsivePeak(2).phi - mc.phi,360,0)) < 5 && abs(FFTtools::wrap(mostImpulsivePeak(2).theta - mc.theta,360,0)) < 2.5";

TCut blastCutH = "!flags.isPayloadBlast && flags.maxBottomToTopRatio[0] < 2.8 && flags.maxBottomToTopRatio[0] > 0.98";
TCut blastCutV = "flags.maxBottomToTopRatio[1] < 2.8 && flags.maxBottomToTopRatio[1] > 0.98";
TCut nSectorsCut = "flags.nSectorsWhereBottomExceedsTop < 28";
TCut blastSum = "flags.maxBottomToTopRatio[0] + flags.maxBottomToTopRatio[1] < 4.3";
TCut totalBlastCut = blastSum && blastCutH && blastCutV && nSectorsCut;

TCut coherenceCut = "mostImpulsivePeak(2).antennaPeakAverage - mostImpulsiveCoherentFiltered(2).peakHilbert < 60"; 
TCut coherenceCut2 = "(mostImpulsivePeak(2).antennaPeakAverage/mostImpulsiveCoherentFiltered(2).peakHilbert*(mostImpulsivePeak(2).antennaPeakAverage - mostImpulsiveCoherentFiltered(2).peakHilbert)) < 250"; //changed from 200 for wais
TCut coherenceCut3 = "(mostImpulsivePeak(2).antennaPeakAverage/mostImpulsiveCoherentFiltered(2).peakHilbert) < 4"; //changed from 5
TCut totalCoherenceCut = coherenceCut && coherenceCut2 && coherenceCut3;

TCut moreImpulsiveDeco = "pow(mostImpulsiveDeconvolvedFiltered(2).impulsivityMeasure, 2) - pow(mostImpulsiveCoherentFiltered(2).impulsivityMeasure, 2) > -.1";
TCut impulsiveFrac = "mostImpulsiveDeconvolvedFiltered(2).fracPowerWindowGradient()/mostImpulsiveCoherentFiltered(2).fracPowerWindowGradient() < 1.6"; // don't go lower than 1.43

TCut upPointingImpulsive = "mostImpulsivePeak(2).theta < 0";

TCut stepFn = "flags.isStepFunction == 0 || flags.isStepFunction == 16";
TCut glitch = "flags.hasGlitch";
TCut fullGlitch = stepFn && !glitch;

TCut tunedCut = "mostImpulsiveDeconvolvedFiltered(2).peakHilbert > 375./800. * mostImpulsivePeak(2).antennaPeakAverage * flags.maxBottomToTopRatio[mostImpulsivePolAsInt(2)] - (375./800. * 200.)";
TCut tunedCut2 = "mostImpulsiveDeconvolvedFiltered(2).peakHilbert >  mostImpulsivePeak(2).antennaPeakAverage - (200)";

TCut powerDiffCut = "(flags.topPower[0]/flags.topPower[2] + flags.middleOrBottomPower[0]/flags.middleOrBottomPower[2]) > .34 && (mostImpulsiveDeconvolvedFiltered(2).peakHilbert/sqrt(mostImpulsiveDeconvolvedFiltered(2).totalPower)) - (mostImpulsiveCoherentFiltered(2).peakHilbert/sqrt(mostImpulsiveCoherentFiltered(2).totalPower)) < .014";
TCut powerDiffCut2 = "(flags.topPower[0]/flags.topPower[2] + flags.middleOrBottomPower[0]/flags.middleOrBottomPower[2]) > .93 && (mostImpulsiveDeconvolvedFiltered(2).peakHilbert/sqrt(mostImpulsiveDeconvolvedFiltered(2).totalPower)) - (mostImpulsiveCoherentFiltered(2).peakHilbert/sqrt(mostImpulsiveCoherentFiltered(2).totalPower)) < .032";
TCut powerDiffCut3 = "(flags.topPower[0]/flags.topPower[2] + flags.middleOrBottomPower[0]/flags.middleOrBottomPower[2]) > 1.12 && (mostImpulsiveDeconvolvedFiltered(2).peakHilbert/sqrt(mostImpulsiveDeconvolvedFiltered(2).totalPower)) - (mostImpulsiveCoherentFiltered(2).peakHilbert/sqrt(mostImpulsiveCoherentFiltered(2).totalPower)) < .068";

TCut diCut = "mostImpulsiveDeconvolvedFiltered(2).fracPowerWindowGradient() <=20 || (flags.topPower[0]/flags.topPower[2] + flags.middleOrBottomPower[0]/flags.middleOrBottomPower[2]) < 1 / (mostImpulsiveDeconvolvedFiltered(2).fracPowerWindowGradient() - 20) + .575";
TCut usableCiCut = "(flags.topPower[0]/flags.topPower[2] + flags.middleOrBottomPower[0]/flags.middleOrBottomPower[2]) > .33 && (mostImpulsiveCoherentFiltered(2).fracPowerWindowGradient()) > 219.";
TCut usableCiCut2 = "(flags.topPower[0]/flags.topPower[2] + flags.middleOrBottomPower[0]/flags.middleOrBottomPower[2]) > .83 && (mostImpulsiveCoherentFiltered(2).fracPowerWindowGradient()) > 65.";
/*
TCut ciCut = "(flags.topPower[0]/flags.topPower[2] + flags.middleOrBottomPower[0]/flags.middleOrBottomPower[2]) > .945 && (mostImpulsiveCoherentFiltered(2).fracPowerWindowGradient()) > 33.45"; //change to .76?
TCut ciCut2 = "(flags.topPower[0]/flags.topPower[2] + flags.middleOrBottomPower[0]/flags.middleOrBottomPower[2]) > .76 && (mostImpulsiveCoherentFiltered(2).fracPowerWindowGradient()) > 36.5";
TCut ciCut3 = "(flags.topPower[0]/flags.topPower[2] + flags.middleOrBottomPower[0]/flags.middleOrBottomPower[2]) > .68 && (mostImpulsiveCoherentFiltered(2).fracPowerWindowGradient()) > 45.9";
TCut ciCut4 = "(flags.topPower[0]/flags.topPower[2] + flags.middleOrBottomPower[0]/flags.middleOrBottomPower[2]) > 1.09 && (mostImpulsiveCoherentFiltered(2).fracPowerWindowGradient()) > 27.75"; 
*/
TCut hpCut = "(flags.topPower[0]/flags.topPower[2] + flags.middleOrBottomPower[0]/flags.middleOrBottomPower[2]) > .755 && (mostImpulsiveCoherentFiltered(2).peakHilbert + mostImpulsiveDeconvolvedFiltered(2).peakHilbert) < 46.";
TCut hppdCut = "(flags.topPower[1]/flags.topPower[2] + flags.middleOrBottomPower[1]/flags.middleOrBottomPower[2]) > .31 && (mostImpulsivePeak(2).antennaPeakAverage - mostImpulsiveCoherentFiltered(2).peakHilbert) > 70.";
TCut idCut = "(flags.topPower[0]/flags.topPower[2] + flags.middleOrBottomPower[0]/flags.middleOrBottomPower[2]) > .305 && pow(mostImpulsiveDeconvolvedFiltered(2).impulsivityMeasure, 2) - pow(mostImpulsiveCoherentFiltered(2).impulsivityMeasure, 2) < -.035";

TCut lowPowerRatio = "flags.middleOrBottomPower[0] / flags.topPower[0] < 7.";
TCut lowPowerRatio2 = "flags.topPower[0] / flags.middleOrBottomPower[0] < 4.";
TCut allPowerRatio = "flags.middleOrBottomPower[2] / flags.topPower[2] < 3.5";
TCut allPowerRatio2 = "flags.topPower[2] / flags.middleOrBottomPower[2] < 6.";
TCut highPowerCut = "flags.middleOrBottomPower[1]/flags.middleOrBottomPower[2] < .6 && flags.topPower[1]/flags.topPower[2] < .7";

TCut amountOfLowPowerLine = "flags.topPower[0]/flags.topPower[2] < flags.middleOrBottomPower[0]/flags.middleOrBottomPower[2] * (.85/.7) + .15";
TCut amountOfLowPowerLine2 = "flags.topPower[0]/flags.topPower[2] > flags.middleOrBottomPower[0]/flags.middleOrBottomPower[2] * (.4/.6) - (.4/.6 * .4)";
TCut amountOfLowPowerLine3 = "flags.topPower[0] < flags.middleOrBottomPower[0] * (.9/.25) + .1";
TCut amountOfLowPowerLine4 = "flags.topPower[0] > flags.middleOrBottomPower[0] * (.15/.7) - (.15/.7 * .3)";

TCut allPowerLine = "flags.topPower[2] > flags.middleOrBottomPower[2] * (2./2.5) - (2./2.5 * 1.5)";
TCut allPowerLine2 = "flags.topPower[2] < flags.middleOrBottomPower[2] * (3./.5) + 1.";
//TCut amountOfLowPowerLine2 = "flags.topPower[0]/flags.topPower[2] > flags.middleOrBottomPower[0]/flags.middleOrBottomPower[2] * (.58/.8) - (.58/.8 * .2)"; // this cuts one bad one at a ~1% hit on mc eff

//TCut lineCuts = amountOfLowPowerLine && amountOfLowPowerLine2 && amountOfLowPowerLine3 && amountOfLowPowerLine4 && allPowerLine && allPowerLine2 && tunedCut && tunedCut2;
TCut lineCuts = amountOfLowPowerLine && amountOfLowPowerLine2 && amountOfLowPowerLine4 && allPowerLine && allPowerLine2 && tunedCut && tunedCut2; // removed amt low power line 3
//TCut squareCuts = !ciCut && !ciCut2 && !ciCut3 && !ciCut4 && !hpCut && !hppdCut && !idCut;
TCut squareCuts = !usableCiCut && diCut;// && !hpCut && !hppdCut && !idCut; //removed all ciCuts, added usableCiCut && diCut, took out usable2 again, idCut takes out the remaining worst therm ev

//TCut powerRatioCuts = lowPowerRatio && lowPowerRatio2 && allPowerRatio && allPowerRatio2 && highPowerCut;
TCut powerRatioCuts = lowPowerRatio && allPowerRatio && allPowerRatio2 && highPowerCut; //removed lowPowerRatio2
TCut powerDiffCuts = !powerDiffCut && !powerDiffCut2 && !powerDiffCut3;
//TCut ciCuts = !ciCut && !ciCut2 && !ciCut3 && !ciCut4 && !fracCut;

//TCut tunedCuts = !powerDiffCut && tunedCut;

TCut impulsiveThermals = isRealImpulsive && fullGlitch && !isWais && totalBlastCut && upPointingImpulsive && coherenceCut;

//old cuts
//TCut qCuts = totalCoherenceCut  && isRealImpulsive && fullGlitch && totalBlastCut && powerRatioCuts && impulsiveFrac  && ciCuts; //removed lowpowerline for now
//TCut signal_cut= isMCImpulsive && isRealImpulsive && fullGlitch && totalBlastCut && totalCoherenceCut && powerRatioCuts && impulsiveFrac && ciCuts; //removed lowpowerline for now

//new cuts
TCut qCuts = totalCoherenceCut  && isRealImpulsive && fullGlitch && totalBlastCut && powerRatioCuts && squareCuts; //these were my final sample qcuts
//TCut qCuts = isRealImpulsive && fullGlitch && totalBlastCut; //minimalist cuts
TCut signal_cut= isMCImpulsive && isRealImpulsive && fullGlitch && totalBlastCut && totalCoherenceCut && powerRatioCuts && squareCuts;

#include "AnitaTMVA.h" 


void makeTMVATrees(int sig_start = 1, int sig_stop=200, const char * sig_dir = "/project2/avieregg/a4sim", const char * outname= "simulation", int nworkers = 1, int simulation = 0) 
{
  // Step 1: load data
  TChain signal;
  if(simulation) signal.SetName("simulation"); 
  else signal.SetName("sumTree");
  std::vector<TChain*> bg(nworkers); 
  if (nworkers > 1) ROOT::EnableThreadSafety(); 
  TString tmp; 

  if (sig_dir) 
  {
    for (int i = sig_start; i<= sig_stop; i++)
    {
      if(!simulation)
      {
        if(i == 46 || i == 60 || i == 61) continue;
      }
      simulation ? tmp.Form(Form("%s/%d.root", sig_dir, i),i) : tmp.Form(Form("%s/run%d.root", sig_dir, i),i); 
      signal.Add(tmp.Data()); 
    }
  }
  
  AnitaTMVA::MVAVarSet varset("vars.tmva"); 

  TString treefilename;
  TString sim = simulation ? "sim" : "data";
  treefilename.Form("thermalCutTrees/%s/%s_%d-%d.root", sim.Data(), outname, sig_start, sig_stop); 

  TFile newOut(treefilename.Data(),"RECREATE");
  
  TCut tree_cut = simulation ? signal_cut : qCuts;

  TString treeName = "sumTree";
  TTree * sigtree= AnitaTMVA::makeTMVATree(&signal, &newOut,  treeName.Data(), varset, tree_cut); 
  sigtree->Write(); 

  newOut.Write(); 
}
