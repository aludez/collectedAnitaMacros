this directory includes macros for doing quality cuts and thermal cuts.

Quality Cut stuff:
processAll.C -- prepare the AnitaEventSummary files
doSim.C -- prepare the AnitaEventSummary files for the MC neutrinos
finalCutsALudwig.C -- all the final quality cuts I used for the neutrino search
setupProof.C, loader.C, cuts.C -- these three things go together to use proof for drawing things
drawWithProof.C -- arbitrarily draw whatever you want using proof to speed things up
doCuts.C, plotCuts.C -- plot the effect of quality cuts on wais and all events
checkCutEfficiencyOnMC.C -- check quality cut efficiency on MC neutrinos

Thermal Cut stuff:
makeTMVATrees.C -- prepare the trees that are used to do the thermal cuts TMVA (quality cuts are applied here)
vars.tmva -- the variables that go into the TMVA trees
doTMVAMC.C -- use the signal and background trees to run the TMVA 
fillTMVA.C -- add TMVA scores to the TMVA trees 
fitToEff.C -- use the TMVA output to estimate cut efficiency

Skeleton of necessary procedure for an analysis:
-run processAll and doSim over all events and MC events
-makeTMVATrees for all of the anitaEventSummary files you generated
-doTMVAMC using these TMVA trees
-fill those trees with the TMVA scores
-use passing events for clustering, etc.
