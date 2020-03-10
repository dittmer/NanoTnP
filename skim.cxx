#include "skim.h"
#include "prep.h"
#include "run.h"
#include "finito.h"

/*
 * Main function, skimming step of analysis
 * The function loops over the input samples,
 * and write them to new files
 */
int main(int argc, char **argv) {

  ROOT::EnableImplicitMT(10);
  bool isMC=false;

  if(argc != 6) {
        std::cout << "Use executable with following arguments: ./skim input output integrated_luminosity kfactor" << std::endl;
        return -1;
    }
    std::string input = argv[1];
    std::cout << ">>> Process input: " << input << std::endl;

    const std::string path = argv[1];
    const std::string sample = argv[3];

    if (path.find("DATA") != std::string::npos) {
      std::cout << "The dataset is DATA"<< std::endl;
    }
    else{
      std::cout << "The dataset is MC"<< std::endl;
      isMC=true;
    }

    std::cout << "Looking at : " << path+"*"+sample+"*.root" << std::endl;

    const auto lumi = argv[4];
    std::cout << "Integrated luminosity: " << lumi << std::endl;

    // HLT
    std::string HLT;
    std::string lasthtlfilter;
    if (path.find("2016") != std::string::npos) {
      HLT = "HLT_Ele27_eta2p1_WPTight_Gsf";
      lasthtlfilter="hltEle27WPTightTrackIsoFilter";
    }
    else if (path.find("2017") != std::string::npos) {
      HLT = "HLT_Ele35_WPTight_Gsf";
      lasthtlfilter = "hltEle35noerWPTightGsfTrackIsoFilter";
    }
    else if (path.find("2017") != std::string::npos) {
      HLT = "HLT_Ele32_WPTight_Gsf";
      lasthtlfilter = "hltEle32WPTightGsfTrackIsoFilter";
    }

    const std::string kfactor = argv[5];
    std::cout << "kfactor : " << kfactor << std::endl;

    TStopwatch time;
    time.Start();

    ROOT::RDataFrame df("Events", path+"*"+sample+"*.root" );


    auto df1 = Filterbaseline(df,HLT);
    auto df2 = goodElectrons(df1);
    auto df3 = goodJets(df2);
    auto df4 = cleanFromJet(df3);
    auto df5 = tagEle(df4);
    auto df6 = genTagEle(df5,isMC);
    auto df7 = tnpPairingEleIDs(df6);

    // should be applied last step
    auto df8 = DeclareVariables(df7);
    auto df9 = AddEventWeight(df8 , path, lumi , kfactor );

    auto dfFinal = df9;
    auto report = dfFinal.Report();
    const std::string output = argv[2];
    std::cout << "Output name: " << output << std::endl;
    dfFinal.Snapshot("Events", output, finalVariables);
    ROOT::RDF::SaveGraph(df,"graph_"+sample+".dot");
    time.Stop();

    report->Print();
    time.Print();
}
