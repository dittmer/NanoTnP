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

  // output variable
  std::string fileName="data/cfg/TnP_SF.txt";
  std::ifstream in(fileName.c_str());
  if(!in) {
    std::cerr << "Cannot open the File : "<<fileName<<std::endl;
    return false;
  }
  std::string str;
  // Read the next line from File untill it reaches the end.
  while (std::getline(in, str)) {
    // Line contains string of length > 0 then save it in vector
    if(str.size() > 0)
      mycfg.outputVar.push_back(str);
  }
  //Close The File
  in.close();
  
  // filelist
  std::ifstream file(mycfg.input);
  str.clear();
  while (std::getline(file, str)) { mycfg.infiles.push_back(str); }
  
  std::cout << ">>> Process input: " << mycfg.input << std::endl;
  std::cout << ">>> Process output: " << mycfg.output << std::endl;
  
  auto df_out = TnP_SF(mycfg);
  
  //RReader model("data/xml/TMVAClassification_BDTG.weights.xml");
  //auto df5 = BDT_composite_var( df4 );
  //auto df6 = BDT_evaluate( df5 , model );
  
  //ROOT::RDF::SaveGraph(df,"graph_"+sample+".dot");
  
  df_out.Snapshot( "fitter_tree", mycfg.output , mycfg.outputVar );
  
  auto report = df_out.Report();
  report->Print();
  
  time.Stop();
  time.Print();
}
