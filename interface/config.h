#ifndef CONFIG_H
#define CONFIG_H

/*
 * Declare all variables which will end up in the final reduced dataset
 */

const std::vector<std::string> finalVariables = {
// sanity checks variables
"goodElectrons" ,
"goodJets" ,
"cleanFromJet" ,
"tagMatcher" ,
"isTag" ,
"nElectron" ,
"tag_Idx" ,
"probe_Idx" ,
"nTnP" ,
"ipair",
"randomness",

// ntuple variables
"tag_Ele_pt" ,
"tag_Ele_eta" ,
"tag_Ele_phi" ,
"probe_Ele_pt",
"probe_Ele_eta" ,
"probe_Ele_phi" ,
"weight",
"pair_pt" ,
"pair_eta" ,
"pair_phi" ,
"pair_mass" ,

// WP
"passingVeto" ,
"passingLoose" ,
"passingMedium" ,
"passingTight" ,
"passingMvaFall17V1Iso_WP90_SS",
"passingMvaFall17V1Iso_WP90",
"passingMvaTTH",

};

#endif
