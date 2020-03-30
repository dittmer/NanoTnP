#ifndef CONFIG_H
#define CONFIG_H

/*
 * configuration for ntuple
 */
struct config_t {
  // basic                                                                                                                                                                                                
  bool isMC=false;
  std::string mcweight="XSWeight*puWeight*METFilter_MC"; //nominal
  std::string lumi;
  std::string jsonFile;

  // trigger
  std::string name = "NULL";
  int bit;

  // goodElectron definition
  std::string goodElectron = "abs(Electron_eta) < 2.5 && Electron_pt > 10 && Electron_cutBased>=2";

  // goodJet definition
  std::string goodJet = "Jet_pt > 30 && abs(Jet_eta) < 2.5 && Jet_jetId > 0 && Jet_puId > 4";  

  // jet cleaning mathcing                                                                                                                                                                            
  float jetclean_dR=0.3;

  // tag candidate definition
  std::string TagCandidate = "Electron_pt>32 && Electron_cutBased==4 && abs(Electron_eta)<= 2.1 && !(abs(Electron_eta)>= 1.4442 && abs(Electron_eta)<=1.566)";

  // trigger object matching
  float trig_dR = 0.3;

  // gen-matching
  float gen_dR = 0.2;
  float gen_relPt = 50;
  bool resolveAmbiguities    = false; //Forbid two RECO objects to match to the same GEN objec
  bool resolveByMatchQuality = true;  //False = just match input in order; True = pick lowest deltaR pair first
};



/*
 * Declare all variables which will end up in the final reduced dataset
 */

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
  "probe_Ele_pt" ,
  "probe_Ele_eta" ,
  "probe_Ele_phi" ,
  "probe_Ele_q" ,
  "weight",
  "pair_pt" ,
  "pair_eta" ,
  "pair_phi" ,
  "pair_mass" ,
  
  // WP
  "passingMvaFall17V1Iso_WP90",
  "passingHWW_WP" , 
  "passing_New_HWW_WP" ,
  "passingMvaTTH",

  // met
  "event_met_pfmet" ,
  "event_met_pfphi" ,

};

#endif
