#include "interface/helper.h"
#include "interface/tnp.h"
#include "interface/tmva_classical.h"
#include "interface/tmva_experimental.h"

/**
   skimmer for
   https://github.com/latinos/LatinoAnalysis/blob/master/NanoGardener/python/modules/addTnpTree.py
**/

/*
 * Main function, skimming step of analysis
 * The function loops over the input samples,
 * and write them to new files
 */
int main(int argc, char **argv) {

  //may return 0 when not able to detect   
  const auto processor_count = std::thread::hardware_concurrency();

  std::cout << "ncpu detected : " << processor_count << ", using it all!" << std::endl;
  ROOT::EnableImplicitMT(processor_count);

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
  
  // TnP //
  auto df1 = TnP( df , mycfg );

  // BDT //
  std::string column_name = "Probe_mva";
  const std::string BDT_file = "data/xml/TMVAClassification_BDTG.weights.xml";
  mycfg.outputVar.push_back( column_name );

  // 1.) Experimental
  //TMVA::Experimental::RReader model( BDT_file );
  //auto df2 = experimental_eval( df1 , model , column_name );

  // 2.) Classical
  std::vector<TMVA::Reader*> readers = Tmva::BDT_Readers( "BDT::BDTG" , BDT_file );  
  auto df2 = classic_eval( df1 , readers , column_name );
  
  // ---------- //
  auto dfout = df2;
  //std::cout<<"average mva : "<< *dfout.Mean(column_name) <<std::endl;
  
  dfout.Snapshot( "fitter_tree", mycfg.output , mycfg.outputVar );

  auto report = dfout.Report();
  report->Print();

  ROOT::RDF::SaveGraph( dfout , "graph.dot" );

  time.Stop();
  time.Print();
}
