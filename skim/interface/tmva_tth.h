#ifndef TMVA_TTH_H
#define TMVA_TTH_H
#include "helper.h"

// https://github.com/root-project/root/blob/master/tutorials/tmva/tmva003_RReader.C

namespace TTH {

  std::vector<float> electron_pt_;
  std::vector<float> electron_eta_;
  std::vector<float> electron_pfRelIso03_all_;
  std::vector<float> electron_miniPFRelIso_chg_;
  std::vector<float> electron_miniRelIsoNeutral_;
  std::vector<float> electron_jetNDauCharged_;
  std::vector<float> electron_jetPtRelv2_;
  std::vector<float> electron_jetPtRatio_;
  std::vector<float> electron_jetBTagDeepFlavB_;
  std::vector<float> electron_sip3d_;
  std::vector<float> electron_logdxy_;
  std::vector<float> electron_logdz_;
  std::vector<float> electron_mvaFall17V2noIso_;

  std::vector<float> event_;
  std::vector<float> electron_mvaTTH_;
  std::vector<float> electron_miniPFRelIso_all_;
  std::vector<float> electron_mvaFall17V2noIso_WPL_;
  std::vector<float> electron_lostHits_;
  std::vector<float> electron_genPartFlav_;
  std::vector<float> electron_dxy_;
  std::vector<float> electron_dz_;

  float jetPtRatio( const float &probe_jetRelIso ){
    float probe_jetPtRatio_ = 1/(1+probe_jetRelIso) > 1.5 ? 1.5 : 1/(1+probe_jetRelIso);
    return probe_jetPtRatio_;
  }

    
  // Reader
  template<typename T>
    std::vector<TMVA::Reader*> BDT_Readers( T &modelname , const std::string &modelfile ) {
    
    const auto poolSize = ROOT::GetThreadPoolSize();
    
    // initialize reader for each thread
    std::vector<TMVA::Reader*> vreaders(poolSize);
    
    // resize
    electron_pt_.resize(poolSize);
    electron_eta_.resize(poolSize);
    electron_pfRelIso03_all_.resize(poolSize);
    electron_miniPFRelIso_chg_.resize(poolSize);
    electron_miniRelIsoNeutral_.resize(poolSize);
    electron_jetNDauCharged_.resize(poolSize);
    electron_jetPtRelv2_.resize(poolSize);
    electron_jetPtRatio_.resize(poolSize);
    electron_jetBTagDeepFlavB_.resize(poolSize);
    electron_sip3d_.resize(poolSize);
    electron_logdxy_.resize(poolSize);
    electron_logdz_.resize(poolSize);
    electron_mvaFall17V2noIso_.resize(poolSize);

    event_.resize(poolSize);
    electron_mvaTTH_.resize(poolSize);
    electron_miniPFRelIso_all_.resize(poolSize);
    electron_mvaFall17V2noIso_WPL_.resize(poolSize);
    electron_lostHits_.resize(poolSize);
    electron_genPartFlav_.resize(poolSize);
    electron_dxy_.resize(poolSize);
    electron_dz_.resize(poolSize);
    
    for (size_t i=0 ; i<vreaders.size() ; i++ ){

      vreaders[i] = new TMVA::Reader(); // new return a pointer                                                                                                                               

      vreaders[i]->AddVariable( "Electron_pt"                                             , &electron_pt_[i]               );
      vreaders[i]->AddVariable( "Electron_eta"                                            , &electron_eta_[i]              );
      vreaders[i]->AddVariable( "Electron_pfRelIso03_all"                                 , &electron_pfRelIso03_all_[i]   );
      vreaders[i]->AddVariable( "Electron_miniPFRelIso_chg"                               , &electron_miniPFRelIso_chg_[i] );
      vreaders[i]->AddVariable( "Electron_miniPFRelIso_all-Electron_miniPFRelIso_chg"     , &electron_miniRelIsoNeutral_[i]);
      vreaders[i]->AddVariable( "Electron_jetNDauCharged"                                 , &electron_jetNDauCharged_[i]   );
      vreaders[i]->AddVariable( "Electron_jetPtRelv2"                                     , &electron_jetPtRelv2_[i]       );
      vreaders[i]->AddVariable( "min(1/(1+Electron_jetRelIso),1.5)"                       , &electron_jetPtRatio_[i]       );
      vreaders[i]->AddVariable( "Electron_jetIdx>-1?Jet_btagDeepFlavB[Electron_jetIdx]:0" , &electron_jetBTagDeepFlavB_[i] );
      vreaders[i]->AddVariable( "Electron_sip3d"                                          , &electron_sip3d_[i]            );
      vreaders[i]->AddVariable( "log(abs(Electron_dxy))"                                  , &electron_logdxy_[i]           );
      vreaders[i]->AddVariable( "log(abs(Electron_dz))"                                   , &electron_logdz_[i]            );
      vreaders[i]->AddVariable( "Electron_mvaFall17V2noIso"                               , &electron_mvaFall17V2noIso_[i] );

      vreaders[i]->AddSpectator( "event"                         , &event_[i]                         );
      vreaders[i]->AddSpectator( "Electron_mvaTTH"               , &electron_mvaTTH_[i]               );
      vreaders[i]->AddSpectator( "Electron_miniPFRelIso_all"     , &electron_miniPFRelIso_all_[i]     );
      vreaders[i]->AddSpectator( "Electron_mvaFall17V2noIso_WPL" , &electron_mvaFall17V2noIso_WPL_[i] );
      vreaders[i]->AddSpectator( "Electron_lostHits"             , &electron_lostHits_[i]             );
      vreaders[i]->AddSpectator( "Electron_genPartFlav"          , &electron_genPartFlav_[i]          );
      vreaders[i]->AddSpectator( "Electron_dxy"                  , &electron_dxy_[i]                  );
      vreaders[i]->AddSpectator( "Electron_dz"                   , &electron_dz_[i]                   );

      vreaders[i]->BookMVA( modelname , modelfile );
    }

    return vreaders;
  }
} // TTH

// Application
template<typename T>
auto tth_eval( T &df , const std::vector<TMVA::Reader*> &readers , std::string &column, Helper::config_t &cfg ) {
  
  auto eval = [&](
		  unsigned int nslot,
		  float probe_pt,
		  float probe_eta,
		  float probe_pfRelIso03_all,
		  float probe_miniPFRelIso_chg,
		  float probe_miniRelIsoNeutral,
		  float probe_jetNDauCharged,
		  float probe_jetPtRelv2,
		  float probe_jetPtRatio,
		  float probe_jetBTagDeepFlavB,
		  float probe_sip3d,
		  float probe_logdxy,
		  float probe_logdz,
		  float probe_mvaFall17V2noIso,
		  float event,
		  float probe_mvaTTH,
		  float probe_miniPFRelIso_all,
		  float probe_mvaFall17V2noIso_WPL,
		  float probe_lostHits,
		  float probe_genPartFlav,
		  float probe_dxy,
		  float probe_dz
		  ){
    TTH::electron_pt_[nslot]                   = probe_pt;
    TTH::electron_eta_[nslot]                  = probe_eta;
    TTH::electron_pfRelIso03_all_[nslot]       = probe_pfRelIso03_all;
    TTH::electron_miniPFRelIso_chg_[nslot]     = probe_miniPFRelIso_chg;
    TTH::electron_miniRelIsoNeutral_[nslot]    = probe_miniRelIsoNeutral;
    TTH::electron_jetNDauCharged_[nslot]       = probe_jetNDauCharged;
    TTH::electron_jetPtRelv2_[nslot]           = probe_jetPtRelv2;
    TTH::electron_jetPtRatio_[nslot]           = probe_jetPtRatio;
    TTH::electron_jetBTagDeepFlavB_[nslot]     = probe_jetBTagDeepFlavB;
    TTH::electron_sip3d_[nslot]                = probe_sip3d;
    TTH::electron_dxy_[nslot]                  = probe_logdxy;
    TTH::electron_dz_[nslot]                   = probe_logdz;
    TTH::electron_mvaFall17V2noIso_[nslot]     = probe_mvaFall17V2noIso;
    TTH::event_[nslot]                         = event;
    TTH::electron_mvaTTH_[nslot]               = probe_mvaTTH;
    TTH::electron_miniPFRelIso_all_[nslot]     = probe_miniPFRelIso_all;
    TTH::electron_mvaFall17V2noIso_WPL_[nslot] = probe_mvaFall17V2noIso_WPL;
    TTH::electron_lostHits_[nslot]             = probe_lostHits;
    TTH::electron_genPartFlav_[nslot]          = probe_genPartFlav;
    TTH::electron_dxy_[nslot]                  = probe_dxy;
    TTH::electron_dz_[nslot]                   = probe_dz;
    
    auto out = readers[nslot]->EvaluateMVA("BDT::BDTG");
    return out;
  };

  auto df2 = df
    .Define( "Probe_miniRelIsoNeutral"    , "Probe_miniPFRelIso_all-Probe_miniPFRelIso_chg" )
    .Define( "Probe_jetPtRatioTTH" , TTH::jetPtRatio , {"Probe_jetRelIso"})
    .Define( "Event"                      , "static_cast<float>(event)" )
    .Define( "Probe_JetNDauCharged"       , "static_cast<float>(Probe_jetNDauCharged)" )
    .Define( "Probe_MvaFall17V2noIso_WPL" , "static_cast<float>(Probe_mvaFall17V2noIso_WPL)")
    .Define( "Probe_GenPartFlav"          , (cfg.isMC) ? "static_cast<float>(Probe_genPartFlav)" : "-1.f")
    .DefineSlot( column , eval, {
  	"Probe_pt",
	  "Probe_eta",
	  "Probe_pfRelIso03_all",
	  "Probe_miniPFRelIso_chg",
	  "Probe_miniRelIsoNeutral",
	  "Probe_JetNDauCharged",
	  "Probe_jetPtRelv2",
	  "Probe_jetPtRatioTTH",
	  "Probe_jetBTagDeepFlavB",
	  "Probe_sip3d",
	  "Dxy_Probe",
	  "Dz_Probe",
	  "Probe_mvaFall17V2noIso",
	  "Event",
	  "Probe_mvaTTH",
	  "Probe_miniPFRelIso_all",
	  "Probe_MvaFall17V2noIso_WPL",
	  "LostHits_Probe",
	  "Probe_GenPartFlav",
	  "Probe_dxy",
	  "Probe_dz"
	  }
      );
  return df2;
}

#endif
