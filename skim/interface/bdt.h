#ifndef BDT_H
#define BDT_H
#include "helper.h"
#include "TMVA/RReader.hxx"
#include "TMVA/RInferenceUtils.hxx"

// https://github.com/root-project/root/blob/master/tutorials/tmva/tmva003_RReader.C
using namespace TMVA::Experimental;

template <typename T>
auto BDT_lambda(T &df) {
  using namespace ROOT::VecOps;
  // https://github.com/srudra3/LeptonID/blob/master/ClassificationBDT_ele.py
  auto makeBDTvar = [](
		       const RVec<float> &electron_miniPFRelIso_all,
		       const RVec<int> &electron_jetIdx,
		       const RVec<float> &electron_pt,
		       const RVec<float> &jet_pt
		       ){
    // Electron_jetPtRatio
    // proxy for jet pt when no associated jets found
    size_t nsize = electron_pt.size();
    RVec<float> electron_jetPtRatio( nsize , 0. );
    
    float A; float B;
    for ( size_t i = 0 ; i < nsize ; i++ ) {
      float mini_Iso_all = electron_miniPFRelIso_all[i];
      int jIdx= electron_jetIdx[i];
      float pt = electron_pt[i];

      A=0.; B=0.;
      if (jIdx == -1) A = 1. / (1. + mini_Iso_all);
      if (jIdx >= 0 ) B = pt / jet_pt[jIdx];
      
      electron_jetPtRatio.push_back(A+B);
    }
    
    // return the first index 
    return electron_jetPtRatio[0];
  };

  return df
    .Define( "Electron_jetPtRatio" , makeBDTvar , { "Electron_miniPFRelIso_all" , "Electron_jetIdx" , "Electron_pt" , "Jet_pt" } )
    ;
}


#endif
