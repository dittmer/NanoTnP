#ifndef TMVA_EXPERIMENTAL_H
#define TMVA_EXPERIMENTAL_H
#include "helper.h"

// https://github.com/root-project/root/blob/master/tutorials/tmva/tmva003_RReader.C

float jetPtRatio( const int &probe_jetIdx , const float &probe_pt , const ROOT::VecOps::RVec<float> &jet_pt , const float &probe_miniPFRelIso_all ){
  float probe_jetPtRatio_ = probe_jetIdx >= 0 ? probe_pt / jet_pt[probe_jetIdx] : 1. / (1. + probe_miniPFRelIso_all);
  return probe_jetPtRatio_;
}

template<typename T>
auto experimental_eval( T &df , TMVA::Experimental::RReader &model , std::string &column ) {

  // use only probe values
  auto variables = model.GetVariableNames();

  auto df2 = df
    .Define( "Probe_jetPtRatio" , jetPtRatio , { "Probe_jetIdx" , "Probe_pt" , "Jet_pt" , "Probe_miniPFRelIso_all"})
    .Define( "Dxy_Probe" , "log(abs(Probe_dxy))" )
    .Define( "Dz_Probe" , "log(abs(Probe_dz))" )
    .Define( "ConvVeto_Probe" , "static_cast<float>(Probe_convVeto)" )
    .Define( "LostHits_Probe" , "static_cast<float>(Probe_lostHits)" )
    .Define( column ,
	     TMVA::Experimental::Compute< 12 , float>(model),
             {
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
                 });
  //std::cout << "variable output mva : " << column << std::endl;
  //std::cout << "Mean response on the signal sample: " << *df2.Mean(column) << std::endl;
  
  return df2;
}
#endif
