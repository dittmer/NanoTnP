#include "tnpper.h"
#include "config.h"
#include "skim.h"
#include "producer.h"

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
    const bool isMC = (input.find("Electron") != std::string::npos) ? false : true;
    std::cout<<"isMC : "<<isMC<<std::endl;

    std::cout << ">>> Process input: " << input << std::endl;
    std::cout << ">>> Process output: " << output << std::endl;
    std::cout << ">>> Integrated luminosity: " << lumi << std::endl;

    // configuration
    if (input.find("2016") != std::string::npos) {
      cfg.name = "HLT_Ele27_eta2p1_WPTight_Gsf";
      cfg.bit="hltEle27WPTightTrackIsoFilter";
    }
    else if (input.find("2017") != std::string::npos) {
      cfg.name = "HLT_Ele35_WPTight_Gsf";
      cfg.bit = "hltEle35noerWPTightGsfTrackIsoFilter";
    }
    else if (input.find("2017") != std::string::npos) {
      cfg.name = "HLT_Ele32_WPTight_Gsf";
      cfg.bit = "hltEle32WPTightGsfTrackIsoFilter";
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
    std::string dummy="HLT_Ele35_WPTight_Gsf";

    // skim plus object cleaning
    auto df1 = Filterbaseline(df, dummy );
    auto df2 = goodElectrons(df1);
    auto df3 = goodJets(df2);
    auto df4 = cleanFromJet(df3);

    // tag and probe producer
    auto df5 = tagCandProducer(df4);
    auto df6 = ( !isMC ) ? tagMatchProducer(df5,"trigger") : df5; // data: tag matched with trigger
    auto df7 = ( isMC ) ? tagMatchProducer(df6,"gen") : df6; // mc: tag match with gen
    auto df8 = tagProducer(df7,isMC);
    auto df9 = probeProducer(df8,"Electron_mvaTTH > 0.7"); // <-- insert testflag // assume HWW working, test passing in pairProducer... zzzz

    // tag-probe pair producer (return pair Idx)
    auto df10 = pairProducer(df9);

    // should be applied last step
    auto df11 = DeclareVariables(df10);
    auto df12 = AddEventWeight(df11 , lumi , isMC );

    auto dfFinal = df12;
    auto report = dfFinal.Report();
    dfFinal.Snapshot("Events", output, finalVariables);
    //ROOT::RDF::SaveGraph(df,"graph_"+sample+".dot");
    time.Stop();

    report->Print();
    time.Print();
}
