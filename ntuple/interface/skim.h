#ifndef SKIM_H
#define SKIM_H
#include "nanotnp.h"

/*
 * EDFilter
 * goodElectron
 */
template <typename T , typename U>
auto Filterbaseline(T &df, config_t &cfg , std::map< U, std::vector< std::pair<U, U> > > &m_json) {

  auto isPassJSON = [&m_json](unsigned& run , unsigned& luminosityBlock)
    {
      int RUN = static_cast<int>(run);
      int LUM = static_cast<int>(luminosityBlock);
      return Helper::isRunLumiInJSON( m_json , RUN, LUM );
    };
  
  std::cout<<" >>> HLT : "<<cfg.name<<" <<< "<<std::endl;
  std::cout<<" >>> Json : "<<cfg.jsonFile<<" <<< "<<std::endl;
  
  if (cfg.name.find("NULL") != std::string::npos){
    return df
      .Filter("nElectron>=2"," --> At least two electrons")
      ;
  }
  else{
    std::string hlt(cfg.name +=" == true");
    return df
      .Define("passJSON",isPassJSON, { "run" , "luminosityBlock" } )
      .Filter("passJSON == 1"," --> Filtered by Golden Json")
      .Filter(hlt," --> passing trigger "+hlt)
      .Filter("nElectron>=2"," --> At least two electrons")
      ;
  }
}

/*************************************************** tag_sequence ***************************************************/

/*
 * EDFilter
 * Kinematically good electrons
 * https://github.com/cms-analysis/EgammaAnalysis-TnPTreeProducer/blob/master/python/egmGoodParticlesDef_cff.py#L75-L79
 */
template <typename T>
auto goodElectrons(T &df, config_t &cfg) {
  return df
    .Define("goodElectrons", cfg.goodElectron )
    .Filter("Sum(goodElectrons==0)==0"," --> All good electrons")
    ;
}

/*
 * EDProducer
 * Kinematically good jets
 */
template <typename T>
auto goodJets(T &df, config_t cfg) {
  return df
    .Define("goodJets", cfg.goodJet )
    ;
}

/*
 * EDProducer
 * clean electron from jets
 * Bool Vector
 */
template <typename T>
auto cleanFromJet(T &df , config_t &cfg) {
  using namespace ROOT::VecOps;
  auto cleanFromJet = [&cfg](RVec<int>& goodElectron, RVec<float>& eta_1, RVec<float>& phi_1,
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
        if (deltar > cfg.jetclean_dR) CleanElectron[iele] = 1; // no match, clean electron
      }
      return CleanElectron;
    };

  return df
    .Define("cleanFromJet", cleanFromJet, {"goodElectrons", "Electron_eta", "Electron_phi", "goodJets", "Jet_eta" , "Jet_phi"})
    .Filter("Sum(cleanFromJet==0)==0"," --> All electrons are cleaned")
    ;
}

#endif
