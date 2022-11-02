#include "interface/helper.h"
#include "interface/tnp.h"
#include "interface/tmva_classical.h"
#include "interface/tmva_tth.h"
//#include "interface/tmva_experimental.h"

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

  if(argc != 5) {
        std::cout << "Use executable with following arguments: ./skim input output integrated_luminosity year" << std::endl;
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
  mycfg.year = argv[4];
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

  // HWW MVA //
  std::string column_hww = "Probe_mvaHWW";
  const std::string hww_weights = "/afs/cern.ch/work/d/dittmer/private/NanoTnP/skim/data/xml/HWWMVA_el_BDTG.weights.xml";
  mycfg.outputVar.push_back( column_hww );

  // 1.) Experimental
  //TMVA::Experimental::RReader model( BDT_file );
  //auto df2 = experimental_eval( df1 , model , column_name );

  // 2.) Classical
  std::vector<TMVA::Reader*> readers_hww = Tmva::BDT_Readers( "BDT::BDTG" , hww_weights );  
  auto df2 = classic_eval( df1 , readers_hww , column_hww );
  

  // ttH MVA //
  std::string column_tth = "Probe_mvaTTHUL";
  //const std::string ttH_weights = "/afs/cern.ch/work/d/dittmer/private/NanoTnP/skim/data/xml/UL20_el_TTH-like_2016_preVFP_BDTG.weights.xml";
  const std::string ttH_weights = "/afs/cern.ch/work/d/dittmer/private/NanoTnP/skim/data/xml/UL20_el_TTH-like_2016_BDTG.weights.xml";
  //const std::string ttH_weights = "/afs/cern.ch/work/d/dittmer/private/NanoTnP/skim/data/xml/UL20_el_TTH-like_2017_BDTG.weights.xml";
  //const std::string ttH_weights = "/afs/cern.ch/work/d/dittmer/private/NanoTnP/skim/data/xml/UL20_el_TTH-like_2018_BDTG.weights.xml";
  mycfg.outputVar.push_back( column_tth );

  std::vector<TMVA::Reader*> readers_tth = TTH::BDT_Readers( "BDT::BDTG" , ttH_weights );  
  auto df3 = tth_eval( df2 , readers_tth , column_tth , mycfg );
  
  // ---------- //
  auto dfout = df3;
  
  dfout.Snapshot( "fitter_tree", mycfg.output , mycfg.outputVar );

  auto report = dfout.Report();
  report->Print();

  ROOT::RDF::SaveGraph( dfout , "graph.dot" );

  time.Stop();
  time.Print();
}
