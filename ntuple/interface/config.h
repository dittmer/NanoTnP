#ifndef CONFIG_H
#define CONFIG_H

/*
 * configuration for ntuple
 */
struct config_t {
  // basic
  bool isMC=false;
  std::string mcweight="XSWeight*puWeight*METFilter_MC"; //nominal
  std::string year;
  std::string lumi;
  std::string jsonFile;

  // trigger
  std::string name = "NULL";

  // goodElectron definition
  int LeptonID=11;
  std::string LeptonCollection = "Sum(abs(Lepton_pdgId)==11)>=2";
  std::string goodLeptonCut    = "abs(Lepton_eta) < 2.5 && Lepton_pt > 10";

  // tag candidate definition
  std::string TagCandidate = "Lepton_pt>30 && abs(Lepton_eta)<= 2.1 && !(abs(Lepton_eta)>= 1.4442 && abs(Lepton_eta)<=1.566)";

  // trigger object matching
  float trig_dR = 0.3;
  int bit;

  // gen-matching
  float gen_dR = 0.2;
  float gen_relPt = 50.;
  bool resolveAmbiguities    = false; //Forbid two RECO objects to match to the same GEN objec
  bool resolveByMatchQuality = true;  //False = just match input in order; True = pick lowest deltaR pair first

  // probe denominator flag
  std::string denom;
  // probe nominator flag
  std::string num;
};



/*
 * Declare all variables which will end up in the final reduced dataset
 */

const std::vector<std::string> finalVariables = {
  /*
  //cross check variables
  "nLepton",
  "Lepton_pdgId",
  "Lepton_muonIdx",
  "Lepton_electronIdx",
  "isGoodElectron",
  "tagEleCutBasedTight",
  "tagCandidate",
  "tagEle",
  "probeEle",
  //"genEle",
  //"genTagEle",
  //"genProbe",
  "probeEleTightHWW",
  "probeElettHMVA" ,
  "probeTightHWW_ttHMVA_0p7" ,
  //"tnpPairing",
  "tag_Idx",
  "probe_Idx",
  "nTnP",
  "ipair",
  "randomness",
  "tnpPairingEleID",
  */
  //analysis variable
  "tag_Ele_pt"              ,
  "tag_Ele_eta"             ,
  "tag_Ele_phi"             ,
  "tag_Ele_mass"            ,
  "tag_Ele_q"               ,

  // probe kinematics
  "probe_Ele_pt"            ,
  "probe_Ele_eta"           ,
  "probe_Ele_phi"           ,
  "probe_Ele_mass"          ,
  "probe_Ele_q"             ,

  // mc probe
  "mcTrue"                  ,

  "pair_pt"                 ,
  "pair_eta"                ,
  "pair_phi"                ,
  "pair_mass"               ,

  "tag_Ele_trigMVA"         ,
  "event_met_pfmet"         ,
  "event_met_pfphi"         ,

  "passingprobeEleTightHWW"                 ,
  "passingprobeElettHMVA"                   ,
  "passingprobeTightHWW_ttHMVA_0p7"         ,

  "tag_TightHWW_SF" ,
  "probe_TightHWW_SF" ,

  "plotweight" , 
  "weight" ,
};

/*
const std::vector<std::string> finalVariables = {
  // event quantity
  "run" ,
  "luminosityBlock" ,
  "event" ,

  // test
  //"PromptGenLepMatch2l" ,
  "tag_PromptGenLepMatch" ,
  "probe_PromptGenLepMatch" ,
  "tag_RecoSF" ,
  "probe_RecoSF" ,

  // sanity checks variables
  //"goodElectrons" ,
  //"goodJets" ,
  //"cleanFromJet" ,
  //"tagMatcher" ,
  "mcTrue" ,
  //"isTag" ,
  //"nElectron" ,
  "tag_Idx" ,
  "probe_Idx" ,
  "nTnP" ,
  //"ipair",
  "randomness",

  // ntuple variables
  "tag_Ele_pt" ,
  "tag_Ele_eta" ,
  "tag_Ele_phi" ,
  "tag_Ele_q" ,
  "tag_Ele_trigMVA" ,
  "tag_sc_eta" ,
  "probe_Ele_pt" ,
  "probe_Ele_eta" ,
  "probe_Ele_phi" ,
  "probe_Ele_q" ,
  "weight",
  "plotweight",
  "pair_pt" ,
  "pair_eta" ,
  "pair_phi" ,
  "pair_mass" ,

  // WP component
  "passingMvattH" ,
  "passingHWW" ,
  //"passingMedium" ,
  //"passingMediumTight" ,
  //"passingMVA94Xwp90iso" ,
  //"passingMvaFall17V1Iso_WP90" ,
  //"passingMvaTTH" ,
  //"el_sc_abseta" ,
  //"el_sieie" ,
  //"el_1overEminus1overP" ,
  //"el_dz" ,
  //"el_dxy" ,
  //"el_reliso03" ,
  //"el_sc_eta" ,
  //"el_mHits" ,
  //"passingConvVeto" ,
  //"el_deltaEtaSC" ,

  // met
  "event_met_pfmet" ,
  "event_met_pfphi" ,

};
*/
#endif
