#ifndef PRODUCER_H
#define PRODUCER_H
#include "tnpper.h"

/*
 * EDProducer
 * tagEle
 * https://github.com/cms-analysis/EgammaAnalysis-TnPTreeProducer/blob/master/python/egmTreesSetup_cff.py#L29-L37
 * 1.) fulfill tightcutbased
 * 2.) match to last filter
 */
 template<typename T>
 auto tagCandProducer(T &df) {
  return df.Define("tagCand","Electron_cutBased>3 && Electron_pt>30 && abs(Electron_eta)< 2.2 && !(abs(Electron_eta)>= 1.4442 && abs(Electron_eta)<=1.566)");
}

/*
 * EDFilter + EDProducer
 * genEle + genTagEle + genProbeEle
 * https://github.com/cms-analysis/EgammaAnalysis-TnPTreeProducer/blob/master/python/egmTreesSetup_cff.py#L83-L86
 */

template<typename T>
  auto tagMatchProducer(T &df, std::string &flag) {
  using namespace ROOT::VecOps;

  // trigger matching producer
  auto trg_matchCandi = [](RVec<int>& id, RVec<int>& filterbits , RVec<float>& eta_el , RVec<float>& phi_el , RVec<float>& eta_trg , RVec<float>& phi_trg) {

    // get combination
    auto comb = Combinations(eta_el,eta_trg); // electron-trig object pair
    const auto numComb = comb[0].size();
    RVec<int> trg_matchCandidate(numComb,1); // in case matching is not apply, all deemed pass
    //if (ismc) return trg_matchCandidate; // no trg match for mc

    for (size_t j=0 ; j < numComb ; j++){
      const auto iele = comb[0][j]; //electron
      const auto itrg = comb[1][j]; //trigobj

      //trigger object selection
      if (abs(id[itrg]) != 11) continue;
      if ( (filterbits[itrg] & 1) != 1 ) continue;
      const auto deltar = sqrt( pow(eta_el[iele] - eta_trg[itrg], 2) + pow(Helper::DeltaPhi(phi_el[iele], phi_trg[itrg]), 2));
      if (deltar > 0.3) trg_matchCandidate[j] = 0;
      }
    return trg_matchCandidate;
  };

  // gen-matching producer
  auto gen_matchCandi = [](RVec<float>& eta_el , RVec<float>& phi_el , RVec<int>& gen_pdgId , RVec<float>& pt_gen ,
                          RVec<float>& eta_gen , RVec<float>& phi_gen , RVec<int>& statusflag) {
      // get combination
      auto comb = Combinations(eta_el,eta_gen); // electron-gen object pair
      const auto numComb = comb[0].size();
      RVec<int> gen_matchCandidate(numComb,0); // in case matching is not apply, all deemed pass
      for (size_t i = 0 ; i < numComb ; i++){
        const auto iele = comb[0][i]; // ele
        const auto igen = comb[1][i]; // genpart

        //genpart selection
        if (abs(gen_pdgId[iele]) != 11) continue;
        if (gen_pt[igen] < 3) continue;
        if (abs(gen_eta[igen]) > 2.7) continue;
        //isPromptFinalState = isPrompt + isLastCopy
        if ( ( statusflag[igen] & 0 ) != 0) continue;
        //genpart selection
        const auto deltar = sqrt( pow(eta_el[iele] - eta_gen[igen], 2) + pow(Helper::DeltaPhi(phi_el[iele], phi_gen[igen]), 2));
        if (deltar > 0.2) continue;
        // resolve ambiguity, pick lowest deltaR pair first , we dont need the gen information, so no need for now.
        gen_matchCandidate[i] = 1;
      }
      return gen_matchCandidate;
  };

  if (flag.compare("trigger")){
    return df.Define("trg_match",trg_matchCandi,{"TrigObj_id","TrigObj_filterBits","Electron_eta","Electron_phi","TrigObj_eta","TrigObj_phi"});
  }
  else if (flag.compare("gen")){
    return df.Define("gen_match",gen_matchCandi,{"Electron_eta","Electron_phi","GenPart_pdgId","GenPart_pt","GenPart_eta","GenPart_phi","GenPart_statusFlags"});
  }
}

template <typename T>
auto tagProducer(T &df){
  return df.Define("isTag","tagCand==1 && trg_match==1 && gen_match==1");
}

template <typename T>
auto probeProducer(T &df){
  return df.Define("isProbe","goodElectrons==1");
}

/*************************************************** tnpPairingEleIDs ***************************************************/
/*
 * EDProducer + EDAnalyzer
 * tnpPairingEleIDs
 * https://github.com/cms-analysis/EgammaAnalysis-TnPTreeProducer/blob/master/python/egmTreesSetup_cff.py#L105-L111
 * pair collection
 */

template <typename T>
auto pairProducer(T &df) {
  using namespace ROOT::VecOps;
  auto tnpProducer = [](RVec<int>& isTag , RVec<int>& isProbe , RVec<float>& pt , RVec<float>& eta , RVec<float>& phi , RVec<float>& m ){

    auto comb = Combinations(isTag,isProbe);
    const auto paircomb = comb[0].size();
    std::vector<std::pair<int,int>> pair_Idx;

    TLorentzVector tot, probe;
    for(size_t i = 0 ; i < paircomb ; i++){
      const auto itag = comb[0][i];
      const auto iprobe = comb[1][i];

      tot.SetPtEtaPhiM(0.,0.,0.,0.);

      if (itag==iprobe) continue;
      if (isTag[itag]!=1) continue;
      if (isProbe[iprobe]==1){
        tot.SetPtEtaPhiM( pt[itag] , eta[itag] , phi[itag] , m[itag] );
        probe.SetPtEtaPhiM( pt[iprobe] , eta[iprobe] , phi[iprobe] , m[iprobe] );
        tot+=probe;
        if ( tot.M() < 50 && tot.M() > 130 ) continue;
        pair_Idx.push_back(std::make_pair(itag,iprobe));
      }
    }
    //
    auto nTnP = pair_Idx.size();
    TRandom3 rand = TRandom3();
    int irand, tidx = -1, pidx = -1;

    if (nTnP==1) return std::vector<int>({pair_Idx[0].first , pair_Idx[0].second , static_cast<int>(nTnP)});

    // throw random number on selecting which pair
    irand = static_cast<int>(rand.Uniform(0,nTnP));
    //std::cout<<"size num : "<<nTnP<<std::endl;
    //std::cout<<"throwing random number : "<<irand<<std::endl;
    tidx = pair_Idx[irand].first;
    pidx = pair_Idx[irand].second;
    //std::cout<<"tidx : "<<tidx<<std::endl;
    //std::cout<<"pidx : "<<pidx<<std::endl;
    return std::vector<int>({tidx , pidx , static_cast<int>(nTnP)});

};

  return df
          .Define("tnpProducer", tnpProducer, {"isTag","isProbe","Electron_pt","Electron_eta","Electron_phi","Electron_mass"})
          .Define("tag_Idx", "tnpProducer[0]")
          .Define("probe_Idx","tnpProducer[1]")
          .Define("nTnP","tnpProducer[2]")
          .Filter("(tag_Idx!=-1 && probe_Idx!=-1) && (tag_Idx!=probe_Idx)"," --> Filter invalid tnp pair")
          // WP
          .Define("passingVeto","Electron_cutBased[probe_Idx]==1")
          .Define("passingLoose","Electron_cutBased[probe_Idx]==2")
          .Define("passingMedium","Electron_cutBased[probe_Idx]==3")
          .Define("passingTight","Electron_cutBased[probe_Idx]==4")
          .Define("passingMVAtth","Electron_mvaTTH[probe_Idx] > 0.7");
          ;
}

/*
 * Declare variables for analysis
 */

template <typename T>
auto DeclareVariables(T &df) {

  auto add_p4 = [](float pt, float eta, float phi, float mass)
    {
      return ROOT::Math::PtEtaPhiMVector(pt, eta, phi, mass);
    };

  auto pair_kin = [](ROOT::Math::PtEtaPhiMVector& p4_1, ROOT::Math::PtEtaPhiMVector& p4_2)
    {
      return std::vector<float>( { float((p4_1+p4_2).Pt()) , float((p4_1+p4_2).Eta()) , float((p4_1+p4_2).Phi()) , float((p4_1+p4_2).M()) } );
    };

  return df
    .Define("tag_Ele_pt" ,"Electron_pt[tag_Idx]")
    .Define("tag_Ele_eta","Electron_eta[tag_Idx]")
    .Define("tag_Ele_phi","Electron_phi[tag_Idx]")
    .Define("tag_Ele_mass","Electron_mass[tag_Idx]")
    .Define("tag_Ele_q","Electron_charge[tag_Idx]")

    .Define("probe_Ele_pt" ,"Electron_pt[probe_Idx]")
    .Define("probe_Ele_eta","Electron_eta[probe_Idx]")
    .Define("probe_Ele_phi","Electron_phi[probe_Idx]")
    .Define("probe_Ele_mass","Electron_mass[probe_Idx]")
    .Define("probe_Ele_q","Electron_charge[probe_Idx]")

    .Define("tag_Ele",add_p4,{"tag_Ele_pt","tag_Ele_eta","tag_Ele_phi","tag_Ele_mass"})
    .Define("probe_Ele",add_p4,{"probe_Ele_pt","probe_Ele_eta","probe_Ele_phi","probe_Ele_mass"})

    .Define("pair_kin" ,pair_kin,{"tag_Ele","probe_Ele"})
    .Define("pair_pt" ,"pair_kin[0]")
    .Define("pair_eta" ,"pair_kin[1]")
    .Define("pair_phi" ,"pair_kin[2]")
    .Define("pair_mass" ,"pair_kin[3]")
    ;
}

/*
 * Add event weights
 */

template <typename T>
auto AddEventWeight(T &df, const std::string& lumi, const bool isMC) {

  std::string weights = (isMC==false) ? "METFilter_DATA" : lumi+"*XSWeight*puWeight*GenLepMatch2l*METFilter_MC"; //PromptGenLepMatch2l
  std::cout<<">>> weights interpreted : "<<weights<<std::endl;
  return df.Define( "weight", weights );
}

#endif
