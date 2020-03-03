#include "skim.h"
/*
 * Main function, skimming step of analysis
 * The function loops over the input samples,
 * and write them to new files
 */
int main(int argc, char **argv) {

  ROOT::EnableImplicitMT(10);
  bool isMC=false;

  if(argc != 7) {
        std::cout << "Use executable with following arguments: ./skim input output integrated_luminosity weight1 weight2" << std::endl;
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

    const std::string weight1 = argv[5];
    const std::string weight2 = argv[6];
    std::cout << "weight1 : " << weight1 << std::endl;
    std::cout << "weight2 : " << weight2 << std::endl;

    TStopwatch time;
    time.Start();

    ROOT::RDataFrame df("Events", path+"*"+sample+"*.root" );


    auto df2 = Filterbaseline(df);
    auto df3 = goodElectrons(df2);
    auto df4 = goodJets(df3);
    auto df5 = cleanFromJet(df4);
    auto df6 = tagEleCutBasedTight(df5);
    auto df7 = tagEle(df6);
    auto df8 = genTagEle(df7,isMC);
    auto df9 = WPsequence(df8);
    auto df10 = tnpPairingEleIDs(df9);
    auto df11 = tnpEleIDs(df10);

    // should be applied last step
    auto df12 = DeclareVariables(df11);
    auto df13 = AddEventWeight(df12 , path, sample , lumi , weight1 , weight2 );

    auto dfFinal = df13;
    auto report = dfFinal.Report();
    const std::string output = argv[2];
    std::cout << "Output name: " << output << std::endl;
    //dfFinal.Snapshot("Events", output, finalVariables);
    dfFinal.Snapshot("Events", output, df13.GetDefinedColumnNames());
    ROOT::RDF::SaveGraph(df12,"graph_"+sample+".dot");
    time.Stop();

    report->Print();
    time.Print();
}
