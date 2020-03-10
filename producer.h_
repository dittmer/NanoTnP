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
auto tagEle(T &df) {
  using namespace ROOT::VecOps;
  // lambda function
  auto tagEle = [](RVec<int>& Ele_cutBased, RVec<float>& pt_1 , RVec<float>& eta_1 , RVec<float>& phi_1 , RVec<int>& id, RVec<int>& filterBits , RVec<float>& eta_2 , RVec<float>& phi_2)
    {
      // get combination
      auto comb = Combinations(eta_1,eta_2); // electron-trig object pair
      const auto numComb = comb[0].size();
      RVec<int> MatchElectron(numComb,0);

      for (size_t i=0 ; i < numComb ; i++){
        const auto i1 = comb[0][i]; //electron
        const auto i2 = comb[1][i]; //trigobj

        // Tag Ele selection
        if ( abs(eta_2[i1]) >= 2.2 ) continue;
        if ( abs(eta_2[i1]) >= 1.4442 && abs(eta_2[i1]) <= 1.566 ) continue;
        if ( pt_1[i1] <= 29 ) continue;
        if ( Ele_cutBased[i1] < 4) continue;
        // Tag Ele selection
        if (abs(id[i2]) != 11) continue;

        // hltEle*WPTight*TrackIsoFilter* --> HLT Ele27 eta2p1 WPTight Gsf v*
        // https://github.com/cms-sw/cmssw/blob/master/PhysicsTools/NanoAOD/python/triggerObjects_cff.py#L35-L46
        if ( (filterBits[i2] & 1) == 1 ) {
          //if (pt_2[i1] < 27) continue;
          //if (abs(eta_2[i1]) > 2.1) continue;
          const auto deltar = sqrt( pow(eta_1[i1] - eta_2[i2], 2) + pow(Helper::DeltaPhi(phi_1[i1], phi_2[i2]), 2));
          if (deltar < 0.3) {
            MatchElectron[i1] = 1;
          }
        }
      } // end numComb loops

      return MatchElectron;
    };
  return df
    .Define("tagEle",tagEle,{"Electron_cutBased","Electron_pt","Electron_eta","Electron_phi","TrigObj_id","TrigObj_filterBits","TrigObj_eta","TrigObj_phi"})
    .Filter("Sum(tagEle==1)>=1"," --> At least one Tag Electron (tightID + match Trigger)")
    ;
}

/*
 * EDFilter + EDProducer
 * genEle + genTagEle + genProbeEle
 * https://github.com/cms-analysis/EgammaAnalysis-TnPTreeProducer/blob/master/python/egmTreesSetup_cff.py#L83-L86
 */

template<typename T>
auto genTagEle(T &df, bool isMC) {
  using namespace ROOT::VecOps;
  auto genTagEle = [](RVec<int>& tagEle_flag , RVec<float>& ele_eta , RVec<float>& ele_phi ,
		      RVec<int>& gen_pdgId , RVec<float>& gen_pt , RVec<float>& gen_eta , RVec<float>& gen_phi , RVec<int>& statusflag) {

    auto comb = Combinations(ele_eta,gen_eta);
    const auto numComb = comb[0].size();

    // genmatch
    RVec<int> genMatch(numComb,0);

    for (size_t i = 0 ; i < numComb ; i++) {
      const auto i1 = comb[0][i]; // tagEle
      const auto i2 = comb[1][i]; // genTagEle

      if (tagEle_flag[i1]!=1) continue;

      // GenPart configuration
      if (abs(gen_pdgId[i2]) != 11) continue; // do not check charge
      if (gen_pt[i2] < 3) continue;
      if (abs(gen_eta[i2]) > 2.7) continue;
      if (( statusflag[i2] & 0 ) != 0) continue;
      // GenPart configuration

      // isPromptFinalState = isPrompt + isLastCopy
      // resolve ambiguity, pick lowest deltaR pair first , we dont need the gen information, so no need for now.
      const auto deltar = sqrt( pow(ele_eta[i1] - gen_eta[i2], 2) + pow(Helper::DeltaPhi(ele_phi[i1], gen_phi[i2]), 2));
      if (deltar < 0.2){
        genMatch[i1] = 1;
      }
    } // end numComb loop
    return genMatch;
  }; // end lambda function

  if (!isMC){
    return df
      .Define("TagEle","tagEle") // if data, always true
      .Filter("Sum(TagEle==1)>=1"," --> At least one tag electron gen-match (for data always gen-matched).")
      ;
  }
  else{
    return df
      .Define("TagEle",genTagEle,{"tagEle","Electron_eta","Electron_phi","GenPart_pdgId","GenPart_pt","GenPart_eta","GenPart_phi","GenPart_statusFlags"})
      .Filter("Sum(TagEle==1)>=1"," --> At least one tag electron gen-matched (for data always gen-matched).")
      ;
    }
}


/*************************************************** ele_sequence ***************************************************/

/*************************************************** tnpPairingEleIDs ***************************************************/
/*
 * EDProducer + EDAnalyzer
 * tnpPairingEleIDs
 * https://github.com/cms-analysis/EgammaAnalysis-TnPTreeProducer/blob/master/python/egmTreesSetup_cff.py#L105-L111
 * pair collection
 */
template <typename T>
auto tnpPairingEleIDs(T &df) {

  using namespace ROOT::VecOps;
  auto tnpPairingEleIDs = [](RVec<int>& tagEle_flag, RVec<int>& probeEle_flag , RVec<float>& pt , RVec<float>& eta , RVec<float>& phi , RVec<float>& m){

    auto comb = Combinations(tagEle_flag,probeEle_flag);
    const auto tagcomb = comb[0].size();
    std::vector<std::pair<int,int>> TnPCandidate;

    for(size_t i = 0; i < tagcomb; i++) {
      const auto tag = comb[0][i];
      const auto probe = comb[1][i];

      if (tagEle_flag[tag] != 1) continue;
      if (probeEle_flag[probe] != 1) continue;

      // cannot be the same
      if (tag == probe) continue;

      TLorentzVector tot, LVprobe;
      tot.SetPtEtaPhiM( pt[tag] , eta[tag] , phi[tag] , m[tag] );
      LVprobe.SetPtEtaPhiM( pt[probe] , eta[probe] , phi[probe] , m[probe] );
      tot+=LVprobe;

      if ( tot.M() < 50 && tot.M() > 130 ) continue;
      TnPCandidate.push_back(std::make_pair(tag,probe));
    }
    //
    auto nTnP = TnPCandidate.size();
    TRandom3 rand = TRandom3();
    int irand, tidx = -1, pidx = -1;

    // throw random number on the tag-probe pair
    // if more then one pair, randomly select pair
    if (nTnP >1){
      irand = static_cast<int>(rand.Uniform(0,nTnP));
      //std::cout<<"size num : "<<nTnP<<std::endl;
      //std::cout<<"throwing random number : "<<irand<<std::endl;
      tidx = TnPCandidate[irand].first;
      pidx = TnPCandidate[irand].second;
      //std::cout<<"tidx : "<<tidx<<std::endl;
      //std::cout<<"pidx : "<<pidx<<std::endl;
    }
    // if only one pair, its either index 0 or 1, obviously
    else if (nTnP==1){
      irand = static_cast<int>(rand.Uniform(1,30)) % 2;
      //even
      if (irand==0){
          tidx=0; pidx=1;
      }
      else{ // odd
        //check if probe is tag as well
        if (tagEle_flag[1]==1 &&probeEle_flag[1]==1){
          tidx=1; pidx=0;
        }
        else {
          tidx=0; pidx=1;
        }
      }
    }
    // data by default is true
    //int mcTrue = genTagEle[tidx];
    return std::vector<int>({tidx , pidx , static_cast<int>(nTnP)});

  }; // end of lambda function

  return df
          .Define("tnpEleIDs", tnpPairingEleIDs, {"TagEle","probeEle","Electron_pt","Electron_eta","Electron_phi","Electron_mass"})
          .Define("tag_Idx", "tnpEleIDs[0]")
          .Define("probe_Idx","tnpEleIDs[1]")
          .Define("nTnP","tnpEleIDs[2]")
          .Filter("(tag_Idx!=-1 && probe_Idx!=-1) && (tag_Idx!=probe_Idx)"," --> Valid tnp pair")
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
