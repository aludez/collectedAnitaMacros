Housed in this directory are various macros for the clustering cuts and post-clustering cuts stage of analysis (background estimate, etc)

doClusteringAcclaim.C -- handles all clustering, output trees are used for other things
makeVPOLTree.C -- takes Acclaim clustering outputs and makes smaller summary trees of all VPOL triggered events. used for making a vpol background estimate
makeCRTree.C -- takes Acclaim clustering outputs and makes smaller summary trees of all HPOL triggered events. used for making a hpol background estimate
polarityTest.C -- runs polarity tests on individual events
runPolaritySim.C -- runs the PolarityMachine simulation for specific EAS candidates and saves the output

After clustering and finding the number of events in the non-signal regions, I used the poissonPosterior.C in AnitaAnalysisFramework/macros to actually make my background estimate.

