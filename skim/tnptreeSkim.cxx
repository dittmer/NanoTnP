#include "interface/helper.h"
//#include "interface/config.h"
//#include "interface/cand_sequence.h"
//#include "interface/tnpPairs_sequence.h"
//#include "interface/tree_sequence.h"


/* variable to keep */
std::vector<std::string> finalVariables = {
  "baseW",
  "Xsec"
};

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

    const std::string input = argv[1];
    const std::string output = argv[2];
    const std::string lumi = argv[3];

    // JSON file initialization
    std::map<int, std::vector<std::pair<int, int> > > m_json;
    //config_t mycfg; 
    //mycfg.lumi = lumi;
    bool isMC = (input.find("Run") != std::string::npos) ? false : true;

    //std::cout << ">>> Process is mc: " << mycfg.isMC << std::endl;
    std::cout << ">>> Process input: " << input << std::endl;
    std::cout << ">>> Process output: " << output << std::endl;
    //std::cout << ">>> Integrated luminosity: " << mycfg.lumi << std::endl;
    
    // Initialize time
    TStopwatch time;
    time.Start();

    // filelist
    std::vector<std::string> infiles;
    std::ifstream file(input);
    std::string str;
    while (std::getline(file, str)) { infiles.push_back(str); }
    
    ROOT::RDataFrame df("Events", infiles); // maybe make an empty dataframe?

    auto columns = df.GetColumnNames();
    for (auto &&columns : columns){
      if ( ( columns.find("Tag_") != std::string::npos ) || ( columns.find("Probe_") != std::string::npos ) || ( columns.find("TnP_") != std::string::npos ) ){
	if (columns.find("FatJet_") != std::string::npos ) continue;
	finalVariables.push_back(columns);
      }
    }

    //auto df1 = hltfilter( df , mycfg , m_json ); // apply HLT and json
    
    // Skim
    auto df1 = df
      .Filter("nMuon==0"," --> number of muon ==0")
      .Filter("Probe_pt>10 && abs(Probe_eta)<2.5"," --> Probe skim")
      .Filter("Tag_pt>30 && abs(Tag_eta)<2.1 && !(abs(Tag_eta)>= 1.4442 && abs(Tag_eta)<=1.566)"," --> Tag skim")
      ;

    // dataset specific
    auto df2 = df1
      .Define("mctrue", (isMC) ? "Tag_isGenMatched*Probe_isGenMatched" : "1")
      .Define("GenWeight", (isMC) ? "genWeight" : "1")
      .Define("weight" , (isMC) ? "GenWeight*baseW*Xsec*puWeight" : "1")
      ;
    auto df3 = (isMC) ? df2 : df2.Filter("TnP_trigger==1"," --> data is matched to HLT filter");

    //ROOT::RDF::SaveGraph(df,"graph_"+sample+".dot");

    auto dfFinal = df3;
    auto report = dfFinal.Report();
    dfFinal.Snapshot("fitter_tree", output, finalVariables);
    time.Stop();

    report->Print();
    time.Print();
}
