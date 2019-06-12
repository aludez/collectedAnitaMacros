This repository holds some of the important macros I made/used in my ANITA-IV analysis

It is divided into two directories, thermalCuts, which has things related to thermal and quality cuts, and clusteringCuts, which has things related to clustering cuts and background estimates

If you wanted to re-run my analysis, the bare essential skeleton would go like this:

-run thermalCuts/processAll and thermalCuts/doSim to create AnitaEventSummary files

-run thermalCuts/makeTMVATrees over all of the AnitaEventSummary files you've created to create TMVA trees

-thermalCuts/doTMVAMC on the TMVA trees created by the previous step to train a classifier

-fill those trees with the TMVA classifier scores using thermalCuts/fillTMVA

-run thermalCuts/fitToEff to pick a threshold TMVA score value and desired thermal background

-feed the passing events into clusteringCuts/doClusteringAcclaim

-run clusteringCuts/makeVPOLTree and clusteringCuts/makeCRTree on the outputs of doClusteringAcclaim to create trees to use for a background estimate

-use the numbers from these trees in AnitaAnalysisFramework/macros/poissonPosterior for your ABCD background estimate

-unblind!

