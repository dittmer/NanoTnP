#include "skim.h"
/*
 * Main function, skimming step of analysis
 * The function loops over the input samples,
 * and write them to new files
 */
int main(int argc, char **argv) {

  ROOT::EnableImplicitMT(10);

  if(argc != 7) {
        std::cout << "Use executable with following arguments: ./skim input output integrated_luminosity weight1 weight2" << std::endl;
        return -1;
    }
    std::string input = argv[1];
    std::cout << ">>> Process input: " << input << std::endl;

    const std::string path = argv[1];
    const std::string sample = argv[3];
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

    // skimming
    auto df1 = Filterbaseline(df);

    // post-processing
    auto df2 = FindGoodElectron(df1);
    auto df3 = FindGoodJet(df2);
    auto df4 = FindCleanFromJet(df3);
    auto df5 = FindPassTightCutBased(df4);
    auto df6 = FindTriggerMatchedElectron(df5);
    //auto df7 = FindTnP(df6);
    // analysis, select ONLY Two

    // should be applied last step
    auto df7 = DeclareVariables(df6);
    auto df8 = AddEventWeight(df7 , path, sample , lumi , weight1 , weight2 );

    auto dfFinal = df8;
    auto report = dfFinal.Report();
    const std::string output = argv[2];
    std::cout << "Output name: " << output << std::endl;
    dfFinal.Snapshot("Events", output, finalVariables);
    time.Stop();

    report->Print();
    time.Print();
}
