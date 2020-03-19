#include "interface/nanotnp.h"
#include "interface/config.h"
#include "interface/skim.h"
#include "interface/producer.h"

/*
 * Main function, skimming step of analysis
 * The function loops over the input samples,
 * and write them to new files
 */
int main(int argc, char **argv) {

  ROOT::EnableImplicitMT(10);

  if(argc != 4) {
        std::cout << "Use executable with following arguments: ./skim input output integrated_luminosity" << std::endl;
        return -1;
    }

    const std::string input = argv[1];
    const std::string output = argv[2];
    const std::string lumi = argv[3];

    // JSON file initialization
    const bool isMC = (input.find("Run") != std::string::npos) ? false : true;
    std::map<int, std::vector<std::pair<int, int> > > m_json;
    Helper::config_t mycfg;

    std::cout << ">>> Process is mc: " << isMC << std::endl;
    std::cout << ">>> Process input: " << input << std::endl;
    std::cout << ">>> Process output: " << output << std::endl;
    std::cout << ">>> Integrated luminosity: " << lumi << std::endl;


    // customising configuration
    if (!isMC) mycfg.name = (input.find("_17") != std::string::npos) ? "HLT_Ele35_WPTight_Gsf" : "HLT_Ele32_WPTight_Gsf";
    if (!isMC) mycfg.bit="1"; // 2017:  hltEle35noerWPTightGsfTrackIsoFilter ; 2018: hltEle32WPTightGsfTrackIsoFilter
    if (!isMC) mycfg.jsonFile= (input.find("_17") != std::string::npos) ? 
		 "./data/Certs/Cert_294927-306462_13TeV_EOY2017ReReco_Collisions17_JSON_v1.txt" :
		 "./data/Certs/Cert_314472-325175_13TeV_17SeptEarlyReReco2018ABC_PromptEraD_Collisions18_JSON.txt" ;
    m_json = Helper::parseJSONAsMap(mycfg.jsonFile);

    //if (input.find("_16") != std::string::npos) {
    //  if (!isMC) mycfg.name = "HLT_Ele27_eta2p1_WPTight_Gsf";
    //  mycfg.bit="1"; // hltEle27WPTightTrackIsoFilter
    //  mycfg.jsonFile="./data/Certs/Cert_271036-284044_13TeV_ReReco_07Aug2017_Collisions16_JSON.txt";
    // }

    // Initialize time
    TStopwatch time;
    time.Start();

    // filelist
    std::vector<std::string> infiles;
    std::ifstream file(input);
    std::string str;
    while (std::getline(file, str)) { infiles.push_back(str); }
    
    ROOT::RDataFrame df("Events", infiles);
    
    // skim plus object cleaning
    auto df1 = Filterbaseline( df , mycfg , m_json );                                                   // mild skim, HLT and JSON filter for DATA
    auto df2 = goodElectrons( df1 , "abs(Electron_eta) < 2.5 && Electron_pt > 10" );                    // definition of good electron
    auto df3 = goodJets( df2 , "Jet_pt > 30 && abs(Jet_eta) < 2.5 && Jet_jetId > 0 && Jet_puId > 4" );  // definition of good jets
    auto df4 = cleanFromJet( df3 );                                                                     // cleaning good electron with good jets

    // tag and probe producer
    auto df5 = tagCandProducer( df4 );                                                                  // standard tag cuts definition
    auto df6 = ( !isMC ) ? tagMatchProducer( df5 , "trigger" ) : tagMatchProducer( df5 , "gen" );       // data: tag matched with trigger object ; mc: tag match with gen level
    auto df8 = tagProducer( df6 );                                                                      // tag candidates

    // tag-probe pair producer (return pair Idx)
    auto df9 = pairProducer( df8 );                                                                     // tnp pair candidate
    auto df10 = probeWPProducer( df9 );                                                                 // definition of WP in interest on probe

    // should be applied last step
    auto df11 = DeclareVariables( df10 );                                                               // declare variables
    auto df12 = AddEventWeight( df11 , lumi , isMC );                                                   // add event weights

    auto dfFinal = df12;
    auto report = dfFinal.Report();
    dfFinal.Snapshot("fitter_tree", output, finalVariables);
    //ROOT::RDF::SaveGraph(df,"graph_"+sample+".dot");
    time.Stop();

    report->Print();
    time.Print();
}
