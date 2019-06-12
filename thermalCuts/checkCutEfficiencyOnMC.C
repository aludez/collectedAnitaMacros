
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
TCut coherenceCut2 = "(mostImpulsivePeak(2).antennaPeakAverage/mostImpulsiveCoherentFiltered(2).peakHilbert*(mostImpulsivePeak(2).antennaPeakAverage - mostImpulsiveCoherentFiltered(2).peakHilbert)) < 200";
TCut coherenceCut3 = "(mostImpulsivePeak(2).antennaPeakAverage/mostImpulsiveCoherentFiltered(2).peakHilbert) < 5";
TCut totalCoherenceCut = coherenceCut && coherenceCut2 && coherenceCut3;

TCut moreImpulsiveDeco = "pow(mostImpulsiveDeconvolvedFiltered(2).impulsivityMeasure, 2) - pow(mostImpulsiveCoherentFiltered(2).impulsivityMeasure, 2) > -.1";
TCut impulsiveFrac = "mostImpulsiveDeconvolvedFiltered(2).fracPowerWindowGradient()/mostImpulsiveCoherentFiltered(2).fracPowerWindowGradient() < 1.6"; // don't go lower than 1.43

TCut upPointingImpulsive = "mostImpulsivePeak(2).theta < 0";

TCut stepFn = "flags.isStepFunction == 0 || flags.isStepFunction == 16";
TCut glitch = "flags.hasGlitch";
TCut fullGlitch = stepFn && !glitch;

TCut tunedCut = "mostImpulsiveDeconvolvedFiltered(2).peakHilbert > .25974 * mostImpulsivePeak(2).antennaPeakAverage * flags.maxBottomToTopRatio[mostImpulsivePolAsInt(2)] - 90.91";

TCut powerDiffCut = "(flags.topPower[0]/flags.topPower[2] + flags.middleOrBottomPower[0]/flags.middleOrBottomPower[2]) > .34 && (mostImpulsiveDeconvolvedFiltered(2).peakHilbert/sqrt(mostImpulsiveDeconvolvedFiltered(2).totalPower)) - (mostImpulsiveCoherentFiltered(2).peakHilbert/sqrt(mostImpulsiveCoherentFiltered(2).totalPower)) < .014";
TCut powerDiffCut2 = "(flags.topPower[0]/flags.topPower[2] + flags.middleOrBottomPower[0]/flags.middleOrBottomPower[2]) > .93 && (mostImpulsiveDeconvolvedFiltered(2).peakHilbert/sqrt(mostImpulsiveDeconvolvedFiltered(2).totalPower)) - (mostImpulsiveCoherentFiltered(2).peakHilbert/sqrt(mostImpulsiveCoherentFiltered(2).totalPower)) < .032";
TCut powerDiffCut3 = "(flags.topPower[0]/flags.topPower[2] + flags.middleOrBottomPower[0]/flags.middleOrBottomPower[2]) > 1.12 && (mostImpulsiveDeconvolvedFiltered(2).peakHilbert/sqrt(mostImpulsiveDeconvolvedFiltered(2).totalPower)) - (mostImpulsiveCoherentFiltered(2).peakHilbert/sqrt(mostImpulsiveCoherentFiltered(2).totalPower)) < .068";
TCut powerDiffCuts = !powerDiffCut && !powerDiffCut2 && !powerDiffCut3;

TCut ciCut = "(flags.topPower[0]/flags.topPower[2] + flags.middleOrBottomPower[0]/flags.middleOrBottomPower[2]) > .666 && (mostImpulsiveCoherentFiltered(2).fracPowerWindowGradient()) > 48.2";
TCut ciCut2 = "(flags.topPower[0]/flags.topPower[2] + flags.middleOrBottomPower[0]/flags.middleOrBottomPower[2]) > .92 && (mostImpulsiveCoherentFiltered(2).fracPowerWindowGradient()) > 24.8";
TCut ciCut3 = "(flags.topPower[0]/flags.topPower[2] + flags.middleOrBottomPower[0]/flags.middleOrBottomPower[2]) > 1.1 && (mostImpulsiveCoherentFiltered(2).fracPowerWindowGradient()) > 23.2";
TCut ciCuts = !ciCut && !ciCut2 && !ciCut3;


TCut lowPowerRatio = "flags.middleOrBottomPower[0] / flags.topPower[0] < 7.";
TCut lowPowerRatio2 = "flags.topPower[0] / flags.middleOrBottomPower[0] < 4.";
TCut allPowerRatio = "flags.middleOrBottomPower[2] / flags.topPower[2] < 3.5";
TCut allPowerRatio2 = "flags.topPower[2] / flags.middleOrBottomPower[2] < 6.";
TCut highPowerCut = "flags.middleOrBottomPower[1]/flags.middleOrBottomPower[2] < .6 && flags.topPower[1]/flags.topPower[2] < .7";

TCut amountOfLowPowerLine = "flags.topPower[0]/flags.topPower[2] < flags.middleOrBottomPower[0]/flags.middleOrBottomPower[2] * (.85/.9) + .15";
TCut amountOfLowPowerLine2 = "flags.topPower[0]/flags.topPower[2] > flags.middleOrBottomPower[0]/flags.middleOrBottomPower[2] * (.58/.8) - (.58/.8 * .2)"; // this cuts one bad one at a ~1% hit on mc eff

TCut powerRatioCuts = lowPowerRatio && lowPowerRatio2 && allPowerRatio && allPowerRatio2 && amountOfLowPowerLine && amountOfLowPowerLine2 && highPowerCut;

TCut impulsiveThermals = isRealImpulsive && fullGlitch && !isWais && totalBlastCut && upPointingImpulsive && coherenceCut;

TCut qCuts = totalCoherenceCut  && isRealImpulsive && fullGlitch && totalBlastCut && powerRatioCuts && impulsiveFrac && powerDiffCuts && ciCuts;

TCut signal_cut= isMCImpulsive && isRealImpulsive && fullGlitch && totalBlastCut && totalCoherenceCut && powerRatioCuts && impulsiveFrac && powerDiffCuts && ciCuts;

#include "AnitaTMVA.h" 


void checkCutEfficiencyOnMC(int sig_start = 1, int sig_stop=500, const char * sig_dir = "/project2/avieregg/a4sim", int nworkers = 1) 
{
  // Step 1: load data

  TChain signal("simulation"); 
  std::vector<TChain*> bg(nworkers); 
  if (nworkers > 1) ROOT::EnableThreadSafety(); 
  TString tmp; 

  if (sig_dir) 
  {
    for (int i = sig_start; i<= sig_stop; i++)
    {
      tmp.Form(Form("%s/%d.root", sig_dir, i),i); 
      signal.Add(tmp.Data()); 
    }
  }
  AnitaEventSummary* aes = 0;
  signal.SetBranchAddress("summary", &aes);

  //Step 2: set cuts
  double totalWeight = 0;
  std::vector<TCut> cuts;
  cuts.push_back(powerRatioCuts);
  cuts.push_back(powerDiffCuts);
  cuts.push_back(ciCuts);
  cuts.push_back(impulsiveFrac);
  cuts.push_back(totalBlastCut);
  cuts.push_back(totalCoherenceCut);
  
  std::vector<double> weights;
  for(int i = 0; cuts.size(); i++)
    weights.push_back(0);

  for(int i = 0; i < signal.GetEntries(); i++)
  {
    signal.GetEntry(i);
    totalWeight += aes->mc.weight;
    for(int j = 0; j < cuts.size(); j++)
    {
      if(signal.Draw("mc.weight", cuts[j], "goff", 1, i)) weights[j] += aes->mc.weight;
    }
  }
  fprintf(stdout, "total weight = %g\n", totalWeight);
  for(int j = 0; j < cuts.size(); j++)
  {
    fprintf(stdout, "%s = %g\n", cuts[j].GetTitle(), weights[j]/totalWeight);
  }
}
