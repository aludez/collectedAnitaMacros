#include "AcclaimClustering.h"
#include <iostream>

using namespace Acclaim;

//void doClusteringAcclaim(Double_t surfaceDist = 40)
void doClusteringAcclaim(Int_t fnum, Double_t surfaceDist = 40)
{

  //const char* outFileName = "/project/kicp/avieregg/anitaIV/sourceMaps/waisNoBase.root";
  //const char* dataGlob = "thermalCutTrees/wais/*";
  //const char* mcGlob = NULL;
  const char* outFileName = Form("/project/kicp/avieregg/anitaIV/sourceMaps/mc/prod4_F-1_MC_%d.root", fnum);
  //const char* outFileName = "/project/kicp/avieregg/anitaIV/sourceMaps/prod3_recreated.root";
  //const char* outFileName = "/project/kicp/avieregg/anitaIV/sourceMaps/wais_horizon.root";
  //const char* outFileName = Form("/project/kicp/avieregg/anitaIV/sourceMaps/base_F0_%gkm.root", surfaceDist);
  const char* dataGlob = "thermalCutTrees/data/*";
  const char* mcGlob = "thermalCutTrees/sim/*";

  Clustering::LogLikelihoodMethod clusterer;
  clusterer.setUseBaseList(false);
  //clusterer.setSelfLLMax(25);
  clusterer.setPercentOfMC(2);
  //clusterer.setDebug(true);
  //clusterer.setCut(TCut("F >= 2.12 && !isWais"));
  //clusterer.setCut(TCut("isWais && (run < 123 || run > 151) && F2 >= -1"));
  clusterer.setCut(TCut("F2 >= -1 && !isWais"));
  clusterer.setCutHical(true);
  clusterer.setSurfaceDistThresholdKm(surfaceDist);
  //clusterer.setDebug(1);
  
  clusterer.doClustering(dataGlob, mcGlob, outFileName, 0);

  return 0;
}
