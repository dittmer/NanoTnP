#ifndef TREE_SEQUENCE_H
#define TREE_SEQUENCE_H
#include "config.h"
#include "helper.h"

/* tnpEleIDs
 *
 */
template <typename T>
auto tnpEleIDs(T &df , config_t &cfg) {
  using namespace ROOT::VecOps;
  auto add_p4 = [](float pt, float eta, float phi, float mass)
    {
      return ROOT::Math::PtEtaPhiMVector(pt, eta, phi, mass);
    };

  auto pair = [](ROOT::Math::PtEtaPhiMVector& p4_1, ROOT::Math::PtEtaPhiMVector& p4_2)
    {
      return std::vector<float>( { float((p4_1+p4_2).Pt()) , float((p4_1+p4_2).Eta()) , float((p4_1+p4_2).Phi()) , float((p4_1+p4_2).M()) } );
    };

  auto Lepton_float_convertor = [](
				  int& tagprobeidx,
				  RVec<int>& Lepton_electronIdx ,
				  RVec<float>& Electron_flag
				  ) {
    return Electron_flag[Lepton_electronIdx[tagprobeidx]];
  };
  
  auto Lepton_int_convertor = [](
				  int& tagprobeidx,
				  RVec<int>& Lepton_electronIdx ,
				  RVec<int>& Electron_flag
				  ) {
    return Electron_flag[Lepton_electronIdx[tagprobeidx]];
  };

  // mc specific variable
  std::string tagReco   = (cfg.isMC) ? "Lepton_RecoSF[tag_Idx]" : "-1";
  std::string probeReco = (cfg.isMC) ? "Lepton_RecoSF[probe_Idx]" : "-1";
  std::string mctruth   = (cfg.isMC) ? "genTagEle[tag_Idx]*genProbe[probe_Idx]" : "1";

  // year specific variable
  std::string tagTightHWWSF   = ( cfg.isMC && cfg.year == "2016") ? "Lepton_tightElectron_mva_90p_Iso2016_IdIsoSF[tag_Idx]" : "Lepton_tightElectron_mvaFall17V2Iso_WP90_IdIsoSF[tag_Idx]";
  std::string probeTightHWWSF   = ( cfg.isMC && cfg.year == "2016") ? "Lepton_tightElectron_mva_90p_Iso2016_IdIsoSF[probe_Idx]" : "Lepton_tightElectron_mvaFall17V2Iso_WP90_IdIsoSF[probe_Idx]";
  
  return df
    .Filter("tnpPairingEleID == 1"," --> filter none-valid tnp pair")

    .Define( "tag_RecoSF"              , tagReco                                                           )
    .Define( "probe_RecoSF"            , probeReco                                                         )
    
    // tag kinematcis
    .Define( "tag_Ele_pt"              , "Lepton_pt[tag_Idx]"                                            )
    .Define( "tag_Ele_eta"             , "Lepton_eta[tag_Idx]"                                           )
    .Define( "tag_Ele_phi"             , "Lepton_phi[tag_Idx]"                                           )
    .Define( "tag_Ele_mass"            , Lepton_float_convertor , { "tag_Idx" , "Lepton_electronIdx" , "Electron_mass" } )
    .Define( "tag_Ele_q"               , Lepton_int_convertor , { "tag_Idx" , "Lepton_electronIdx" , "Electron_charge" } )

    // probe kinematics
    .Define( "probe_Ele_pt"            , "Lepton_pt[probe_Idx]"                                          )
    .Define( "probe_Ele_eta"           , "Lepton_eta[probe_Idx]"                                         )
    .Define( "probe_Ele_phi"           , "Lepton_phi[probe_Idx]"                                         )
    .Define( "probe_Ele_mass"          , Lepton_float_convertor , { "probe_Idx" , "Lepton_electronIdx" , "Electron_mass" } )
    .Define( "probe_Ele_q"             , Lepton_int_convertor , { "probe_Idx" , "Lepton_electronIdx" , "Electron_charge" } )
    
    // mc probe
    .Define( "mcTrue"                  , mctruth                                                           )
    
    .Define( "tag_Ele"   , add_p4      , {"tag_Ele_pt","tag_Ele_eta","tag_Ele_phi","tag_Ele_mass"}         )
    .Define( "probe_Ele" , add_p4      , {"probe_Ele_pt","probe_Ele_eta","probe_Ele_phi","probe_Ele_mass"} )
    .Define( "pair"      , pair        , {"tag_Ele","probe_Ele"}                                           )

    .Define( "pair_pt"                 , "pair[0]"                                                         )
    .Define( "pair_eta"                , "pair[1]"                                                         )
    .Define( "pair_phi"                , "pair[2]"                                                         )
    .Define( "pair_mass"               , "pair[3]"                                                         )

    .Define( "tag_Ele_trigMVA"         , Lepton_float_convertor , { "tag_Idx" , "Lepton_electronIdx" , "Electron_mvaFall17V1Iso" } )
    .Define( "event_met_pfmet"         , "PuppiMET_pt"                                                     )
    .Define( "event_met_pfphi"         , "PuppiMET_phi"                                                    )

    .Define( "passingprobeEleTightHWW"                 , "probeEleTightHWW[probe_Idx]"                    )
    .Define( "passingprobeElettHMVA"                   , "probeElettHMVA[probe_Idx]"                      )
    .Define( "passingprobeTightHWW_ttHMVA_0p7"         , "probeTightHWW_ttHMVA_0p7[probe_Idx]"             )

    .Define( "tag_TightHWW_SF"      , (!cfg.isMC) ? "-1" : tagTightHWWSF )
    .Define( "probe_TightHWW_SF"    , (!cfg.isMC) ? "-1" : probeTightHWWSF )

    ;
}

/*
 * Add event weights
 */

template <typename T>
auto AddEventWeight(T &df, config_t &cfg) {
  // GenLepMatch2l ? PromptGenLepMatch2l
  std::string weights1 = (!cfg.isMC) ? "METFilter_DATA" : cfg.lumi + "*XSWeight*puWeight*TriggerEffWeight_2l*EMTFbug_veto*METFilter_MC*tag_RecoSF*probe_RecoSF" ;
  std::string weights2 = (!cfg.isMC) ? "METFilter_DATA" : cfg.mcweight;
  std::cout<<" >>> plotweights interpreted : "<<weights1<<" <<< "<<std::endl;
  std::cout<<" >>> weights interpreted : "<<weights2<<" <<< "<<std::endl;
  return df
    //.Filter( "passingHWW==1", " --> Define Denominator" ) // define denominator
    .Define( "plotweight", weights1 )
    .Define( "weight", weights2 )
    ;
}

#endif
