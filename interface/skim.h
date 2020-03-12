#ifndef SKIM_H
#define SKIM_H
#include "nanotnp.h"

/*
 * EDFilter
 * goodElectron
 */
template <typename T>
auto Filterbaseline(T &df, config_t &HLT) {
  std::cout<<" >>> HLT : "<<HLT.name<<" <<< "<<std::endl;
  std::string hlt(HLT.name +=" == true");
  return df
    .Filter(hlt, " --> Passes trigger "+hlt)
    .Define("bits",HLT.bit)
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
auto goodElectrons(T &df, const char* good) {
  std::string in(good);
  return df
    .Define("goodElectrons", in )
    .Filter("Sum(goodElectrons==0)==0"," --> All good electrons")
    ;
}

/*
 * EDProducer
 * Kinematically good jets
 */
template <typename T>
auto goodJets(T &df, const char* good) {
  std::string in(good);
  return df
    .Define("goodJets", in )
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
        const auto iele = comb[0][i]; // ele
        const auto ijet = comb[1][i]; // jets
        if ( goodElectron[iele] != 1 ) continue;
        if ( goodJet[ijet] !=1 ) continue;
        const auto deltar = sqrt(pow(eta_1[iele] - eta_2[ijet], 2) + pow(Helper::DeltaPhi(phi_1[iele], phi_2[ijet]), 2));
        if (deltar > 0.3) CleanElectron[iele] = 1; // no match, clean electron
      }
      return CleanElectron;
    };

  return df
    .Define("cleanFromJet", cleanFromJet, {"goodElectrons", "Electron_eta", "Electron_phi", "goodJets", "Jet_eta" , "Jet_phi"})
    .Filter("Sum(cleanFromJet==0)==0"," --> All electrons are cleaned")
    ;
}

#endif
