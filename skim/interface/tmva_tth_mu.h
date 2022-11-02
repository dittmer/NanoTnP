#ifndef TMVA_TTH_MU_H
#define TMVA_TTH_MU_H
#include "helper.h"

// https://github.com/root-project/root/blob/master/tutorials/tmva/tmva003_RReader.C

namespace TTHMU {

  std::vector<float> muon_pt_;
  std::vector<float> muon_eta_;
  std::vector<float> muon_pfRelIso03_all_;
  std::vector<float> muon_miniPFRelIso_chg_;
  std::vector<float> muon_miniRelIsoNeutral_;
  std::vector<float> muon_jetNDauCharged_;
  std::vector<float> muon_jetPtRelv2_;
  std::vector<float> muon_jetPtRatio_;
  std::vector<float> muon_jetBTagDeepFlavB_;
  std::vector<float> muon_sip3d_;
  std::vector<float> muon_logdxy_;
  std::vector<float> muon_logdz_;
  std::vector<float> muon_segmentComp_;

  std::vector<float> event_;
  std::vector<float> muon_mvaTTH_;
  std::vector<float> muon_miniPFRelIso_all_;
  std::vector<float> muon_genPartFlav_;
  std::vector<float> muon_isGlobal_;
  std::vector<float> muon_isTracker_;
  std::vector<float> muon_isPFcand_;
  std::vector<float> muon_mediumId_;
  std::vector<float> muon_looseId_;
  std::vector<float> muon_dxy_;
  std::vector<float> muon_dz_;

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
    muon_pt_.resize(poolSize);
    muon_eta_.resize(poolSize);
    muon_pfRelIso03_all_.resize(poolSize);
    muon_miniPFRelIso_chg_.resize(poolSize);
    muon_miniRelIsoNeutral_.resize(poolSize);
    muon_jetNDauCharged_.resize(poolSize);
    muon_jetPtRelv2_.resize(poolSize);
    muon_jetPtRatio_.resize(poolSize);
    muon_jetBTagDeepFlavB_.resize(poolSize);
    muon_sip3d_.resize(poolSize);
    muon_logdxy_.resize(poolSize);
    muon_logdz_.resize(poolSize);
    muon_segmentComp_.resize(poolSize);

    event_.resize(poolSize);
    muon_mvaTTH_.resize(poolSize);
    muon_miniPFRelIso_all_.resize(poolSize);
    muon_genPartFlav_.resize(poolSize);
    muon_isGlobal_.resize(poolSize);
    muon_isTracker_.resize(poolSize);
    muon_isPFcand_.resize(poolSize);
    muon_mediumId_.resize(poolSize);
    muon_looseId_.resize(poolSize);
    muon_dxy_.resize(poolSize);
    muon_dz_.resize(poolSize);
    
    for (size_t i=0 ; i<vreaders.size() ; i++ ){

      vreaders[i] = new TMVA::Reader(); // new return a pointer                                                                                                                               

      vreaders[i]->AddVariable( "Muon_pt"                                         , &muon_pt_[i]               );
      vreaders[i]->AddVariable( "Muon_eta"                                        , &muon_eta_[i]              );
      vreaders[i]->AddVariable( "Muon_pfRelIso03_all"                             , &muon_pfRelIso03_all_[i]   );
      vreaders[i]->AddVariable( "Muon_miniPFRelIso_chg"                           , &muon_miniPFRelIso_chg_[i] );
      vreaders[i]->AddVariable( "Muon_miniPFRelIso_all-Muon_miniPFRelIso_chg"     , &muon_miniRelIsoNeutral_[i]);
      vreaders[i]->AddVariable( "Muon_jetNDauCharged"                             , &muon_jetNDauCharged_[i]   );
      vreaders[i]->AddVariable( "Muon_jetPtRelv2"                                 , &muon_jetPtRelv2_[i]       );
      vreaders[i]->AddVariable( "min(1/(1+Muon_jetRelIso),1.5)"                   , &muon_jetPtRatio_[i]       );
      vreaders[i]->AddVariable( "Muon_jetIdx>-1?Jet_btagDeepFlavB[Muon_jetIdx]:0" , &muon_jetBTagDeepFlavB_[i] );
      vreaders[i]->AddVariable( "Muon_sip3d"                                      , &muon_sip3d_[i]            );
      vreaders[i]->AddVariable( "log(abs(Muon_dxy))"                              , &muon_logdxy_[i]           );
      vreaders[i]->AddVariable( "log(abs(Muon_dz))"                               , &muon_logdz_[i]            );
      vreaders[i]->AddVariable( "Muon_segmentComp"                                , &muon_segmentComp_[i]      );
     
      vreaders[i]->AddSpectator( "event"                 , &event_[i]                );
      vreaders[i]->AddSpectator( "Muon_mvaTTH"           , &muon_mvaTTH_[i]          );
      vreaders[i]->AddSpectator( "Muon_miniPFRelIso_all" , &muon_miniPFRelIso_all_[i]);
      vreaders[i]->AddSpectator( "Muon_looseId"          , &muon_looseId_[i]         );
      vreaders[i]->AddSpectator( "Muon_genPartFlav"      , &muon_genPartFlav_[i]     );
      vreaders[i]->AddSpectator( "Muon_isGlobal"         , &muon_isGlobal_[i]        );
      vreaders[i]->AddSpectator( "Muon_isTracker"        , &muon_isTracker_[i]       );
      vreaders[i]->AddSpectator( "Muon_isPFcand"         , &muon_isPFcand_[i]        );
      vreaders[i]->AddSpectator( "Muon_mediumId"         , &muon_mediumId_[i]        );
      vreaders[i]->AddSpectator( "Muon_looseId"          , &muon_looseId_[i]         );
      vreaders[i]->AddSpectator( "Muon_dxy"              , &muon_dxy_[i]             );
      vreaders[i]->AddSpectator( "Muon_dz"               , &muon_dz_[i]              );

      vreaders[i]->BookMVA( modelname , modelfile );
    }

    return vreaders;
  }
} // TTH

// Application
template<typename T>
auto tth_mu_eval( T &df , const std::vector<TMVA::Reader*> &readers , std::string &column, Helper::config_t &cfg ) {
  
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
		  float probe_segmentComp,
		  float event,
		  float probe_mvaTTH,
		  float probe_miniPFRelIso_all,
		  float probe_genPartFlav,
		  float probe_isGlobal,
		  float probe_isTracker,
		  float probe_isPFcand,
		  float probe_mediumId,
		  float probe_looseId,
		  float probe_dxy,
		  float probe_dz
		  ){
    TTHMU::muon_pt_[nslot]                = probe_pt;
    TTHMU::muon_eta_[nslot]               = probe_eta;
    TTHMU::muon_pfRelIso03_all_[nslot]    = probe_pfRelIso03_all;
    TTHMU::muon_miniPFRelIso_chg_[nslot]  = probe_miniPFRelIso_chg;
    TTHMU::muon_miniRelIsoNeutral_[nslot] = probe_miniRelIsoNeutral;
    TTHMU::muon_jetNDauCharged_[nslot]    = probe_jetNDauCharged;
    TTHMU::muon_jetPtRelv2_[nslot]        = probe_jetPtRelv2;
    TTHMU::muon_jetPtRatio_[nslot]        = probe_jetPtRatio;
    TTHMU::muon_jetBTagDeepFlavB_[nslot]  = probe_jetBTagDeepFlavB;
    TTHMU::muon_sip3d_[nslot]             = probe_sip3d;
    TTHMU::muon_dxy_[nslot]               = probe_logdxy;
    TTHMU::muon_dz_[nslot]                = probe_logdz;
    TTHMU::muon_segmentComp_[nslot]       = probe_segmentComp;
    TTHMU::event_[nslot]                  = event;
    TTHMU::muon_mvaTTH_[nslot]            = probe_mvaTTH;
    TTHMU::muon_miniPFRelIso_all_[nslot]  = probe_miniPFRelIso_all;
    TTHMU::muon_genPartFlav_[nslot]       = probe_genPartFlav;
    TTHMU::muon_isGlobal_[nslot]          = probe_isGlobal;
    TTHMU::muon_isTracker_[nslot]         = probe_isTracker;
    TTHMU::muon_isPFcand_[nslot]          = probe_isPFcand;
    TTHMU::muon_mediumId_[nslot]          = probe_mediumId;
    TTHMU::muon_looseId_[nslot]           = probe_looseId;
    TTHMU::muon_dxy_[nslot]               = probe_dxy;
    TTHMU::muon_dz_[nslot]                = probe_dz;
    
    auto out = readers[nslot]->EvaluateMVA("BDT::BDTG");
    return out;
  };

  auto df2 = df
    .Define( "Probe_miniRelIsoNeutral"    , "Probe_miniPFRelIso_all-Probe_miniPFRelIso_chg" )
    .Define( "Probe_jetPtRatioTTH" , TTHMU::jetPtRatio , {"Probe_jetRelIso"})
    .Define( "Event"                      , "static_cast<float>(event)" )
    .Define( "Probe_JetNDauCharged"       , "static_cast<float>(Probe_jetNDauCharged)" )
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
	  "Probe_segmentComp",
	  "Event",
	  "Probe_mvaTTH",
	  "Probe_miniPFRelIso_all",
	  "Probe_GenPartFlav",
	  "Probe_isGlobal",
	  "Probe_isTracker",
	  "Probe_isPFcand",
	  "Probe_mediumId",
	  "Probe_looseId",
	  "Probe_dxy",
	  "Probe_dz"
	  }
      );
  return df2;
}

#endif
