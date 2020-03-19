#ifndef CONFIG_H
#define CONFIG_H

/*
 * Declare all variables which will end up in the final reduced dataset
 */

const std::vector<std::string> finalVariables = {
  // event quantity
  "run" ,
  "luminosityBlock" ,
  "event" ,

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
  "ipair",
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
