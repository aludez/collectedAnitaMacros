#include "FFTtools.h"
#define CUTS_LOADED

TCut isRealImpulsive = "mostImpulsivePeak(2).value > 0 && flags.isRF == 1 && mostImpulsivePeak(2).theta < 60 && mostImpulsivePeak(2).theta > -50";
TCut isWais = "flags.pulser == 1 || flags.pulser==5";

TCut isWaisImpulsive = "abs(FFTtools::wrap(mostImpulsivePeak(2).phi - wais.phi,360,0)) < 5 && abs(FFTtools::wrap(mostImpulsivePeak(2).theta - wais.theta,360,0)) < 2.5";

TCut isMCImpulsive = "abs(FFTtools::wrap(mostImpulsivePeak(2).phi - mc.phi,360,0)) < 5 && abs(FFTtools::wrap(mostImpulsivePeak(2).theta - mc.theta,360,0)) < 2.5";

TCut blastCutH = "!flags.isPayloadBlast && flags.maxBottomToTopRatio[0] < 2.7 && flags.maxBottomToTopRatio[0] > 0.98";
TCut blastCutV = "flags.maxBottomToTopRatio[1] < 2.7 && flags.maxBottomToTopRatio[1] > 0.98";
TCut blastSum = "(flags.maxBottomToTopRatio[0] + flags.maxBottomToTopRatio[1]) < 4.3";
TCut nSectorsCut = "flags.nSectorsWhereBottomExceedsTop < 28";
TCut totalBlastCut = blastSum && blastCutH && blastCutV && nSectorsCut;

TCut coherenceCut = "mostImpulsivePeak(2).antennaPeakAverage - mostImpulsiveCoherentFiltered(2).peakHilbert < 60";
TCut coherenceCut2 = "(mostImpulsivePeak(2).antennaPeakAverage/mostImpulsiveCoherentFiltered(2).peakHilbert*(mostImpulsivePeak(2).antennaPeakAverage - mostImpulsiveCoherentFiltered(2).peakHilbert)) < 250";

TCut moreImpulsiveDeco = "pow(mostImpulsiveDeconvolvedFiltered(2).impulsivityMeasure, 2) - pow(mostImpulsiveCoherentFiltered(2).impulsivityMeasure, 2) > -.1";

TCut upPointingImpulsive = "mostImpulsivePeak(2).theta < 0";

TCut stepFn = "flags.isStepFunction";

TCut impulsiveThermals = isRealImpulsive && !stepFn && !isWais && totalBlastCut && upPointingImpulsive && moreImpulsiveDeco && coherenceCut;
TCut waisQuality = isRealImpulsive && isWaisImpulsive && isWais && totalBlastCut && !stepFn;
