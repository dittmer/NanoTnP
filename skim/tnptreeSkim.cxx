#include "interface/helper.h"
#include "interface/config.h"
#include "interface/cand_sequence.h"
#include "interface/tnpPairs_sequence.h"
#include "interface/tree_sequence.h"

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
    config_t mycfg; 
    mycfg.lumi = lumi;
    mycfg.isMC = (input.find("Run") != std::string::npos) ? false : true;

    std::cout << ">>> Process is mc: " << mycfg.isMC << std::endl;
    std::cout << ">>> Process input: " << input << std::endl;
    std::cout << ">>> Process output: " << output << std::endl;
    std::cout << ">>> Integrated luminosity: " << mycfg.lumi << std::endl;

    // customising configuration
    if (input.find("_16") != std::string::npos){
      if (!mycfg.isMC) mycfg.name     = "HLT_Ele27_eta2p1_WPTight_Gsf";
      if (!mycfg.isMC) mycfg.bit      = 1; //hltEle27WPTightTrackIsoFilter
      if (!mycfg.isMC) mycfg.jsonFile = "./data/Certs/Cert_271036-284044_13TeV_ReReco_07Aug2017_Collisions16_JSON.txt";
      mycfg.year                      = "2016";
      mycfg.denom                     = "Lepton_isTightElectron_mva_90p_Iso2016"; 
      mycfg.num                       = "Electron_mvaTTH"; //>0.7
    }
    else if (input.find("_17") != std::string::npos){
      if (!mycfg.isMC) mycfg.name     = "HLT_Ele35_WPTight_Gsf";
      if (!mycfg.isMC) mycfg.bit      = 1; //hltEle35noerWPTightGsfTrackIsoFilter
      if (!mycfg.isMC) mycfg.jsonFile = "./data/Certs/Cert_294927-306462_13TeV_EOY2017ReReco_Collisions17_JSON_v1.txt";
      mycfg.year                      = "2017";
      mycfg.denom                     = "Lepton_isTightElectron_mvaFall17V1Iso_WP90";
      mycfg.num                       = "Electron_mvaTTH"; //>0.7
    }
    else if (input.find("_18") != std::string::npos){
      if (!mycfg.isMC) mycfg.name     = "HLT_Ele32_WPTight_Gsf";
      if (!mycfg.isMC) mycfg.bit      = 1; //hltEle32WPTightGsfTrackIsoFilter
      if (!mycfg.isMC) mycfg.jsonFile = "./data/Certs/Cert_314472-325175_13TeV_17SeptEarlyReReco2018ABC_PromptEraD_Collisions18_JSON.txt";
      mycfg.year                      = "2018";
      mycfg.denom                     = "Lepton_isTightElectron_mvaFall17V1Iso_WP90";
      mycfg.num                       = "Electron_mvaTTH"; //>0.7
    }
    if (!mycfg.isMC) m_json = Helper::parseJSONAsMap(mycfg.jsonFile);
    
    // Initialize time
    TStopwatch time;
    time.Start();

    // filelist
    std::vector<std::string> infiles;
    std::ifstream file(input);
    std::string str;
    while (std::getline(file, str)) { infiles.push_back(str); }
    
    ROOT::RDataFrame df("Events", infiles); // maybe make an empty dataframe?

    auto df1 = hltfilter( df , mycfg , m_json ); // apply HLT and json
    
    // Cand_sequence
    auto df2 = goodElectrons( df1 , mycfg );
    auto df3 = tagEleCutBasedTight( df2 , mycfg );
    auto df4 = tagEle( df3 , mycfg );
    
    auto df5 = probeEle( df4 , mycfg ); // data no match
    auto df6 = (mycfg.isMC) ? genEle( df5 , mycfg ) : df5;
    auto df7 = (mycfg.isMC) ? genTagProbeEle( df6 , mycfg , "tagEle" ) : df6;
    auto df8 = (mycfg.isMC) ? genTagProbeEle( df7 , mycfg , "probeEle" ) : df7;
    auto df9 = tnpPairingEleIDs( df8 );
    
    auto df10 = tnpEleIDs( df9 , mycfg );
    auto df11 = AddEventWeight( df10 , mycfg );                                                                               // add event weights

    //ROOT::RDF::SaveGraph(df,"graph_"+sample+".dot");

    auto dfFinal = df11;
    auto report = dfFinal.Report();
    dfFinal.Snapshot("fitter_tree", output, finalVariables);
    time.Stop();

    report->Print();
    time.Print();
}
