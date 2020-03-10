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
  //https://github.com/cms-analysis/EgammaAnalysis-TnPTreeProducer/blob/master/python/egmElectronIDModules_cff.py#L144-L150
  return df.Define("tagCand","Electron_cutBased>3 && Electron_pt>=30 && abs(Electron_eta)<= 2.1 && !(abs(Electron_eta)>= 1.4442 && abs(Electron_eta)<=1.566)"); // tag cuts
}

/*
 * EDFilter + EDProducer
 * genEle + genTagEle + genProbeEle
 * https://github.com/cms-analysis/EgammaAnalysis-TnPTreeProducer/blob/master/python/egmTreesSetup_cff.py#L83-L86
 */

template<typename T>
  auto tagMatchProducer(T &df, const char* s) {
  using namespace ROOT::VecOps;
  std::string flag(s);
  // trigger matching producer
  auto trg_matchCandi = [](RVec<int>& id, RVec<int>& filterbits , RVec<float>& eta_el , RVec<float>& phi_el , RVec<float>& eta_trg , RVec<float>& phi_trg) {

    // get combination
    auto comb = Combinations(eta_el,eta_trg); // electron-trig object pair
    const auto numComb = comb[0].size();
    RVec<int> trg_matchCandidate(eta_el.size(),1); // in case matching is not apply, all deemed pass

    for (size_t j=0 ; j < numComb ; j++){
      const auto iele = comb[0][j]; //electron
      const auto itrg = comb[1][j]; //trigobj

      //trigger object selection
      if (abs(id[itrg]) != 11) continue;
      if ( (filterbits[itrg] & 1) != 1 ) continue;
      const auto deltar = sqrt( pow(eta_el[iele] - eta_trg[itrg], 2) + pow(Helper::DeltaPhi(phi_el[iele], phi_trg[itrg]), 2));
      if (deltar > 0.3) trg_matchCandidate[iele] = 0;
      }
    return trg_matchCandidate;
  };

  // gen-matching producer
  auto gen_matchCandi = [](RVec<float>& pt_el, RVec<float>& eta_el , RVec<float>& phi_el , RVec<int>& gen_pdgId , RVec<float>& pt_gen ,
                          RVec<float>& eta_gen , RVec<float>& phi_gen , RVec<int>& statusflag) {

      // get combination
      auto comb = Combinations(eta_el,eta_gen); // electron-gen object pair
      const auto numComb = comb[0].size();
      RVec<int> gen_matchCandidate(eta_el.size(),0); // in case matching is not apply, all deemed pass

      for (size_t i = 0 ; i < numComb ; i++){
        const auto iele = comb[0][i]; // ele
        const auto igen = comb[1][i]; // genpart

        //genpart selection
        //https://github.com/cms-analysis/EgammaAnalysis-TnPTreeProducer/blob/master/python/egmTreesSetup_cff.py#L83-L86
        if (abs(gen_pdgId[igen]) != 11) continue;
        if (pt_gen[igen] < 3) continue;
        if (abs(eta_gen[igen]) > 2.7) continue;
        //isPromptFinalState = isPrompt + isLastCopy
        if ( ( statusflag[igen] & 0 ) != 0) continue;
        //genpart selection

        //https://github.com/cms-analysis/EgammaAnalysis-TnPTreeProducer/blob/master/python/egmTreesSetup_cff.py#L88-L98
        const auto deltar = sqrt( pow(eta_el[iele] - eta_gen[igen], 2) + pow(Helper::DeltaPhi(phi_el[iele], phi_gen[igen]), 2));
        const auto dpt_rel = fabs(pt_el[iele] - pt_gen[igen]) / pt_gen[igen];
        if (deltar > 0.2) continue; // Minimum deltaR for the match
        if (dpt_rel > 50.) continue; // Minimum deltaPt/Pt for the match
        // resolve ambiguity, pick lowest deltaR pair first , we dont need the gen information, so no need for now.
        gen_matchCandidate[iele] = 1;
      }
      return gen_matchCandidate;

  };

  auto out = df;
  std::cout<<" >> Matcher deployed : "<<flag<<" matching << "<<std::endl;
  if (flag=="trigger"){
    out = df.Define("trg_match",trg_matchCandi,{"TrigObj_id","TrigObj_filterBits","Electron_eta","Electron_phi","TrigObj_eta","TrigObj_phi"});
  }
  else if (flag=="gen"){
    out = df.Define("gen_match",gen_matchCandi,{"Electron_pt","Electron_eta","Electron_phi","GenPart_pdgId","GenPart_pt","GenPart_eta","GenPart_phi","GenPart_statusFlags"});
  }
  return out;
}

template <typename T>
auto tagProducer(T &df, const bool &isMC){
  //https://github.com/cms-analysis/EgammaAnalysis-TnPTreeProducer/blob/master/python/egmTreesSetup_cff.py#L30-L37
  return (isMC) ? df.Define("isTag","tagCand==1 && gen_match==1") : df.Define("isTag","tagCand==1 && trg_match==1");
}

template <typename T>
auto probeProducer(T &df, const char* testWP){
  //https://github.com/cms-analysis/EgammaAnalysis-TnPTreeProducer/blob/master/python/egmTreesSetup_cff.py#L39-L42
  std::string WP(testWP);
  return df.Define("isProbe","goodElectrons==1 &&"+WP);
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

      tot.SetPtEtaPhiM( 0. , 0. , 0. , 0. );
      probe.SetPtEtaPhiM( 0. , 0. , 0. , 0. );

      if (itag==iprobe) continue;
      if (isTag[itag]!=1) continue;
      if (isProbe[iprobe]!=1) continue;
      tot.SetPtEtaPhiM( pt[itag] , eta[itag] , phi[itag] , m[itag] );
      probe.SetPtEtaPhiM( pt[iprobe] , eta[iprobe] , phi[iprobe] , m[iprobe] );
      tot+=probe;
      if ( tot.M() > 50 && tot.M() < 130 ) pair_Idx.push_back(std::make_pair(itag,iprobe));
    }
    //
    auto nTnP = pair_Idx.size();
    int irand, tidx = -1, pidx = -1;
    //std::cout<<"pair_Idx size : "<<nTnP<<std::endl;
    if (nTnP==1) return std::vector<int>({pair_Idx[0].first , pair_Idx[0].second , static_cast<int>(nTnP)});
    if (nTnP==0) return std::vector<int>({tidx , pidx , static_cast<int>(nTnP)});

    TRandom3 rand = TRandom3();
    // throw random number on selecting which pair
    irand = static_cast<int>(rand.Uniform(0,nTnP));
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
