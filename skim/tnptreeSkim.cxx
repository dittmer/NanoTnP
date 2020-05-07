#include "interface/helper.h"


/* variable to keep */
std::vector<std::string> finalVariables = {
  "mcTrue",
  "weight",
  "tag_Ele_trigMVA",
  "event_met_pfmet",
  "event_met_pfphi",
  "pair_mass",
  "Tag_pt",
  "Tag_eta",
  "Tag_phi",
  "Tag_charge",
  "Probe_pt",
  "Probe_eta",
  "Probe_phi",
  "Probe_charge",
  "Probe_sieie",
  "Probe_eInvMinusPInv",
  "Probe_dz",
  "Probe_dxy",
  "Probe_cutBased_Fall17_V1",
  "Probe_mvaFall17V1Iso_WP90",
  "Probe_pfRelIso03_all",
  "Tag_cutBased_Fall17_V1",
  "Probe_mvaTTH"
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
    bool isMC = (input.find("Run") != std::string::npos) ? false : true;
    while (std::getline(file, str)) { infiles.push_back(str); }
    
    ROOT::RDataFrame df("Events", infiles);
    
    // Skim
    auto df1 = df
      .Filter("abs(Tag_pdgId)!=13 && abs(Probe_pdgId)!=13"," --> Tag and Probe are electron")
      .Filter("Tag_pt>30 && abs(Tag_eta)<2.1 && !(abs(Tag_eta)>= 1.4442 && abs(Tag_eta)<=1.566)"," --> Tag candidate skim")
      ;

    // dataset specific
    auto df2 = df1
      .Define("mcTrue", (isMC) ? "Tag_isGenMatched*Probe_isGenMatched" : "1")
      .Define("weight" , (isMC) ? "genWeight*baseW*puWeight" : "1")
      ;
    auto df3 = (isMC) ? df2 : df2.Filter("TnP_trigger==1"," --> data is matched to HLT filter");

    // variable for low pt cut
    auto df4 = df3
      .Define("tag_Ele_trigMVA","Tag_mvaFall17V1Iso") //ElectronMVAEstimatorRun2Fall17IsoV1Values
      .Define("event_met_pfmet","PuppiMET_pt")
      .Define("event_met_pfphi","PuppiMET_phi")
      .Define("pair_mass","TnP_mass")
      ;

    //ROOT::RDF::SaveGraph(df,"graph_"+sample+".dot");

    auto dfFinal = df4;
    auto report = dfFinal.Report();
    dfFinal.Snapshot("fitter_tree", output, finalVariables);
    time.Stop();

    report->Print();
    time.Print();
}
