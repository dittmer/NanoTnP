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
    const bool isMC = (input.find("Run") != std::string::npos) ? false : true;
    std::cout<<"isMC : "<<isMC<<std::endl;

    std::cout << ">>> Process input: " << input << std::endl;
    std::cout << ">>> Process output: " << output << std::endl;
    std::cout << ">>> Integrated luminosity: " << lumi << std::endl;

    // configuration
    config_t myhlt;
    if (input.find("_16") != std::string::npos) {
      myhlt.name = "HLT_Ele27_eta2p1_WPTight_Gsf";
      // hltEle27WPTightTrackIsoFilter
      myhlt.bit=1; // bit's position
    }
    else if (input.find("_17") != std::string::npos) {
      myhlt.name = "HLT_Ele35_WPTight_Gsf";
      // hltEle35noerWPTightGsfTrackIsoFilter ??
      myhlt.bit=1; // bit's position
    }
    else if (input.find("_18") != std::string::npos) {
      myhlt.name = "HLT_Ele32_WPTight_Gsf";
      // hltEle32WPTightGsfTrackIsoFilter
      myhlt.bit =1; // bit's position
    }

    TStopwatch time;
    time.Start();

    std::vector<std::string> infiles;
    std::ifstream file(input);
    std::string str;
    while (std::getline(file, str)) {
      infiles.push_back(str);
    }

    ROOT::RDataFrame df("Events", infiles);

    // skim plus object cleaning
    auto df1 = Filterbaseline( df , myhlt.name );                                                       // mild skim
    auto df2 = goodElectrons( df1 , "abs(Electron_eta) < 2.5 && Electron_pt > 5" );                     // definition of good electron
    auto df3 = goodJets( df2 , "Jet_pt > 30 && abs(Jet_eta) < 2.5 && Jet_jetId > 0 && Jet_puId > 4");   // definition of good jets
    auto df4 = cleanFromJet(df3);                                                                       // cleaning good electron with good jets

    // tag and probe producer
    auto df5 = tagCandProducer(df4);                                                                    // standard tag cuts definition
    auto df6 = ( !isMC ) ? tagMatchProducer(df5,"trigger", myhlt.bit) : tagMatchProducer(df5,"gen");    // data: tag matched with trigger object ; mc: tag match with gen level
    auto df8 = tagProducer(df6,isMC);                                                                   // tag candidates

    // tag-probe pair producer (return pair Idx)
    auto df9 = pairProducer(df8);                                                                       // tnp pair candidate
    auto df10 = probeWPProducer(df9);                                                                   // definition of WP in interest on probe

    // should be applied last step
    auto df11 = DeclareVariables(df10);                                                                 // declare variables
    auto df12 = AddEventWeight(df11 , lumi , isMC );                                                    // add event weights

    auto dfFinal = df12;
    auto report = dfFinal.Report();
    dfFinal.Snapshot("fitter_tree", output, finalVariables);
    //ROOT::RDF::SaveGraph(df,"graph_"+sample+".dot");
    time.Stop();

    report->Print();
    time.Print();
}
