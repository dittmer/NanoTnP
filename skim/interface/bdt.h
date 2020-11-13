#ifndef BDT_H
#define BDT_H
#include "helper.h"
#include "TMVA/RReader.hxx"
#include "TMVA/RInferenceUtils.hxx"

// https://github.com/root-project/root/blob/master/tutorials/tmva/tmva003_RReader.C
using namespace TMVA::Experimental;

template <typename T>
auto BDT_composite_var(T &df) {
  using namespace ROOT::VecOps;
  // https://github.com/srudra3/LeptonID/blob/master/ClassificationBDT_ele.py
  auto makeBDTvar = [](
		       const RVec<float> &electron_miniPFRelIso_all,
		       const RVec<float> &electron_miniPFRelIso_chg,
		       const RVec<int> &electron_jetIdx,
		       const RVec<float> &electron_pt,
		       const RVec<float> &jet_pt
		       ){
    // Electron_jetPtRatio
    // proxy for jet pt when no associated jets found
    size_t nsize = electron_pt.size();
    
    RVec<float> electron_jetPtRatio( nsize , 0. );
    RVec<float> electron_miniPFRelIso_neu( nsize , 0. );
    
    float A; float B;
    for ( size_t i = 0 ; i < nsize ; i++ ) {
      float mini_Iso_all = electron_miniPFRelIso_all[i];
      float mini_Iso_chg = electron_miniPFRelIso_chg[i];
      int jIdx= electron_jetIdx[i];
      float pt = electron_pt[i];
      
      A=0.; B=0.;
      if (jIdx == -1) A = 1. / (1. + mini_Iso_all);
      if (jIdx >= 0 ) B = pt / jet_pt[jIdx];
      electron_jetPtRatio.push_back(A+B);

      electron_miniPFRelIso_neu.push_back( (mini_Iso_all - mini_Iso_chg) ) ;
    }
    
    return std::vector<ROOT::VecOps::RVec<float>>( {electron_miniPFRelIso_neu , electron_jetPtRatio } );
  };

  return df
    .Define( "makeBDTvar" , makeBDTvar , { "Electron_miniPFRelIso_all" , "Electron_miniPFRelIso_chg" , "Electron_jetIdx" , "Electron_pt" , "Jet_pt" } )
    .Define( "Electron_miniPFRelIso_neu" , "makeBDTvar[0]" )
    .Define( "Electron_jetPtRatio" , "makeBDTvar[1]" )
    ;
}

#endif
