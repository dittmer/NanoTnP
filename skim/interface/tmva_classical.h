#ifndef TMVA_CLASSICAL_H
#define TMVA_CLASSICAL_H
#include "helper.h"

// https://github.com/root-project/root/blob/master/tutorials/tmva/tmva003_RReader.C

namespace Tmva {
  
  std::vector<float> electron_eta_;
  std::vector<float> electron_miniPFRelIso_all_;
  std::vector<float> electron_r9_;
  std::vector<float> electron_dxy_;
  std::vector<float> electron_dz_;
  std::vector<float> electron_sip3d_;
  std::vector<float> electron_convVeto_;
  std::vector<float> electron_lostHits_;
  std::vector<float> electron_mvaFall17V2noIso_;
  std::vector<float> electron_jetBTagDeepFlavB_;
  std::vector<float> electron_jetPtRelv2_;
  std::vector<float> electron_jetPtRatio_;

  float jetPtRatio( const int &probe_jetIdx , const float &probe_pt , const ROOT::VecOps::RVec<float> &jet_pt , const float &probe_miniPFRelIso_all ){
    float probe_jetPtRatio_ = probe_jetIdx >= 0 ? probe_pt / jet_pt[probe_jetIdx] : 1. / (1. + probe_miniPFRelIso_all);
    return probe_jetPtRatio_;
  }

  // Reader
  template<typename T>
    std::vector<TMVA::Reader*> BDT_Readers( T &modelname , const std::string &modelfile ) {
    
    const auto poolSize = ROOT::GetThreadPoolSize();

    // initialize reader for each thread
    std::vector<TMVA::Reader*> vreaders(poolSize);

    // resize
    electron_eta_.resize(poolSize);
    electron_miniPFRelIso_all_.resize(poolSize);
    electron_r9_.resize(poolSize);
    electron_dxy_.resize(poolSize);
    electron_dz_.resize(poolSize);
    electron_sip3d_.resize(poolSize);
    electron_convVeto_.resize(poolSize);
    electron_lostHits_.resize(poolSize);
    electron_mvaFall17V2noIso_.resize(poolSize);
    electron_jetBTagDeepFlavB_.resize(poolSize);
    electron_jetPtRelv2_.resize(poolSize);
    electron_jetPtRatio_.resize(poolSize);

    for (size_t i=0 ; i<vreaders.size() ; i++ ){
      vreaders[i] = new TMVA::Reader(); // new return a pointer                                                                                                                               
      vreaders[i]->AddVariable( "Electron_eta"                                                     , &electron_eta_[i]              );
      vreaders[i]->AddVariable( "Electron_miniPFRelIso_all"                                        , &electron_miniPFRelIso_all_[i] );
      vreaders[i]->AddVariable( "Electron_r9"                                                      , &electron_r9_[i]               );
      vreaders[i]->AddVariable( "log(abs(Electron_dxy))"                                           , &electron_dxy_[i]              );
      vreaders[i]->AddVariable( "log(abs(Electron_dz))"                                            , &electron_dz_[i]               );
      vreaders[i]->AddVariable( "Electron_sip3d"                                                   , &electron_sip3d_[i]            );
      vreaders[i]->AddVariable( "Electron_convVeto"                                                , &electron_convVeto_[i]         );
      vreaders[i]->AddVariable( "Electron_lostHits"                                                , &electron_lostHits_[i]         );
      vreaders[i]->AddVariable( "Electron_mvaFall17V2noIso"                                        , &electron_mvaFall17V2noIso_[i] );
      vreaders[i]->AddVariable( "(Electron_jetIdx>=0)*(Jet_btagDeepFlavB[max(Electron_jetIdx,0)])" , &electron_jetBTagDeepFlavB_[i] );
      vreaders[i]->AddVariable( "(Electron_jetIdx>=0)*(Electron_jetPtRelv2)"                       , &electron_jetPtRelv2_[i]       );
      vreaders[i]->AddVariable( "(Electron_jetIdx==-1)*((1)/(1+(Electron_miniPFRelIso_all)))+(Electron_jetIdx>=0)*((Electron_pt)/(Jet_pt[max(Electron_jetIdx,0)]))", &electron_jetPtRatio_[i]);
      vreaders[i]->BookMVA( modelname , modelfile );
    }
    
    return vreaders;
  }
} // Tmva

// Application
template<typename T>
auto classic_eval( T &df , const std::vector<TMVA::Reader*> &readers , std::string &column ) {
  
  auto eval = [&](
		  unsigned int nslot,
		  float probe_eta,
		  float probe_miniPFRelIso_all,
		  float probe_r9,
		  float dxy_probe,
		  float dz_probe,
		  float probe_sip3d,
		  float convVeto_probe,
		  float lostHits_probe,
		  float probe_mvaFall17V2noIso,
		  float probe_jetBTagDeepFlavB,
		  float probe_jetPtRelv2,
		  float probe_jetPtRatio
		  ){
    Tmva::electron_eta_[nslot]              = probe_eta;
    Tmva::electron_miniPFRelIso_all_[nslot] = probe_miniPFRelIso_all;
    Tmva::electron_r9_[nslot]               = probe_r9;
    Tmva::electron_dxy_[nslot]              = dxy_probe;
    Tmva::electron_dz_[nslot]               = dz_probe;
    Tmva::electron_sip3d_[nslot]            = probe_sip3d;
    Tmva::electron_convVeto_[nslot]         = convVeto_probe;
    Tmva::electron_lostHits_[nslot]         = lostHits_probe;
    Tmva::electron_mvaFall17V2noIso_[nslot] = probe_mvaFall17V2noIso;
    Tmva::electron_jetBTagDeepFlavB_[nslot] = probe_jetBTagDeepFlavB;
    Tmva::electron_jetPtRelv2_[nslot]       = probe_jetPtRelv2;
    Tmva::electron_jetPtRatio_[nslot]       = probe_jetPtRatio;
    
    auto out = readers[nslot]->EvaluateMVA("BDT::BDTG");
    return out;
  };
  
  auto df2 = df
    .Define( "Probe_jetPtRatio" , Tmva::jetPtRatio , { "Probe_jetIdx" , "Probe_pt" , "Jet_pt" , "Probe_miniPFRelIso_all"})
    .Define( "Dxy_Probe" , "log(abs(Probe_dxy))" )
    .Define( "Dz_Probe" , "log(abs(Probe_dz))" )
    .Define( "ConvVeto_Probe" , "static_cast<float>(Probe_convVeto)" )
    .Define( "LostHits_Probe" , "static_cast<float>(Probe_lostHits)" )
    .DefineSlot( column , eval, {
	"Probe_eta",
	  "Probe_miniPFRelIso_all",
	  "Probe_r9",
	  "Dxy_Probe",
	  "Dz_Probe",
	  "Probe_sip3d",
	  "ConvVeto_Probe",
	  "LostHits_Probe",
	  "Probe_mvaFall17V2noIso",
	  "Probe_jetBTagDeepFlavB",
	  "Probe_jetPtRelv2",
	  "Probe_jetPtRatio"
	  }
      );
  return df2;
}

#endif
