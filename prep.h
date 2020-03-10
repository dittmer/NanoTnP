#include "skim.h"
#ifndef PREP_H
#define PREP_H

/*
 * EDFilter
 * goodElectron
 */
template <typename T>
auto Filterbaseline(T &df, std::string HLT) {
  HLT+=" == true";
  return df
    .Define("isMC", "run==1")
    .Filter(HLT, " --> Passes trigger "+HLT)
    .Filter("nElectron>=2"," --> At least two electrons")
    ;
}

/*************************************************** tag_sequence ***************************************************/

/*
 * EDFilter
 * Kinematically good electrons
 * https://github.com/cms-analysis/EgammaAnalysis-TnPTreeProducer/blob/master/python/egmGoodParticlesDef_cff.py#L75-L79
 */
template <typename T>
auto goodElectrons(T &df) {
  return df
    .Define("goodElectrons","abs(Electron_eta) < 2.5 && Electron_pt > 5")
    .Define("probeEle","goodElectrons==1")
    .Filter("Sum(goodElectrons==0)==0"," --> All good electrons")
    ;
}

/*
 * EDProducer
 * Kinematically good jets
 */
template <typename T>
auto goodJets(T &df) {
  return df
    .Define("goodJets","Jet_pt > 30 && abs(Jet_eta) < 2.5 && Jet_jetId > 0 && Jet_puId > 4")
    .Filter("Sum(goodJets)>0"," --> At least 1 good jets")
    ;
}

/*
 * EDProducer
 * clean electron from jets
 * Bool Vector
 */
template <typename T>
auto cleanFromJet(T &df) {
  using namespace ROOT::VecOps;
  auto cleanFromJet = [](RVec<int>& goodElectron, RVec<float>& eta_1, RVec<float>& phi_1,
			 RVec<int>& goodJet, RVec<float>& eta_2, RVec<float>& phi_2)
    {

      // Get indices of all possible combinations
      auto comb = Combinations(eta_1,eta_2);
      const auto numComb = comb[0].size();
      // Find match pair and eliminate the electron
      RVec<int> CleanElectron(eta_1.size(),0);
      for (size_t i = 0 ; i < numComb ; i++) {
        const auto i1 = comb[0][i];
        const auto i2 = comb[1][i];
        if( goodElectron[i1] == 1 && goodJet[i2] == 1 ){
          const auto deltar = sqrt(
				   pow(eta_1[i1] - eta_2[i2], 2) +
				   pow(Helper::DeltaPhi(phi_1[i1], phi_2[i2]), 2));
          if (deltar > 0.3) {
	    CleanElectron[i1] = 1; // no match, good electron
	  }
	}
      }

      return CleanElectron;
    };

  return df
    .Define("cleanFromJet", cleanFromJet, {"goodElectrons", "Electron_eta", "Electron_phi", "goodJets", "Jet_eta" , "Jet_phi"})
    .Filter("Sum(cleanFromJet==0)==0"," --> All electrons are cleaned")
    ;
}

#endif
