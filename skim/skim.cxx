#include "interface/helper.h"
#include "interface/bdt.h"
#include "interface/workflow.h"

/* skimmer for
https://github.com/latinos/LatinoAnalysis/blob/master/NanoGardener/python/modules/addTnpTree.py
*/

/*
 * Main function, skimming step of analysis
 * The function loops over the input samples,
 * and write them to new files
 */
int main(int argc, char **argv) {

  ROOT::EnableImplicitMT(10);

  if(argc != 4) {
        std::cout << "Use executable with following arguments: ./tnptreeSkim input output integrated_luminosity" << std::endl;
        return -1;
  }

  // start time
  TStopwatch time;
  time.Start();

  // characterize configuration
  Helper::config_t mycfg;
  mycfg.input = argv[1];
  mycfg.output = argv[2];
  mycfg.lumi = argv[3];
  mycfg.isMC = ( mycfg.input.find("Run") != std::string::npos || mycfg.input.find("Single") != std::string::npos || mycfg.input.find("EGamma") != std::string::npos ) ? false : true;

  // filelist
  std::ifstream file(mycfg.input);
  std::string str;
  while (std::getline(file, str)) { mycfg.infiles.push_back(str); }

  std::cout << ">>> Process input: " << mycfg.input << std::endl;
  std::cout << ">>> Process output: " << mycfg.output << std::endl;

  ROOT::RDataFrame df("Events", mycfg.infiles);

  auto dfout1 = TnP( df , mycfg );
  auto dfout2 = Evaluate( dfout1 , mycfg ); // evaluate MVA variables

  dfout2.Snapshot( "fitter_tree", mycfg.output , mycfg.outputVar );

  auto report = dfout2.Report();
  report->Print();

  ROOT::RDF::SaveGraph(df,"graph.dot");

  time.Stop();
  time.Print();
}
