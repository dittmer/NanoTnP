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
		       RVec<float>& electron_miniPFRelIso_all,
		       RVec<int>& electron_jetIdx,
		       RVec<float>& electron_pt,
		       RVec<float>& jet_pt
		       ){
    // Electron_jetPtRatio
    // proxy for jet pt when no associated jets found
    std::vector<float> electron_jetPtRatio;
    float A; float B;
    for ( size_t i = 0 ; i < electron_pt.size() ; i++ ) {
      int jIdx= electron_jetIdx[i]; float minIso= electron_miniPFRelIso_all[i]; float pt = electron_pt[i];
      A=0.; B=0.;
      if (jIdx == -1) A = 1. / (1. + minIso); 
      if (jIdx >= 0 ) B = pt / jet_pt[jIdx];
      electron_jetPtRatio.push_back(A+B);
    }
    return electron_jetPtRatio;
  };
  
  return df
    .Define( "Electron_miniPFRelIso_neu" , "Electron_miniPFRelIso_all - Electron_miniPFRelIso_chg" )
    .Define( "Electron_jetPtRatio" , makeBDTvar , { "Electron_miniPFRelIso_all" , "Electron_jetIdx" , "Electron_pt" , "Jet_pt" }  )
    ;
}

template <typename T, typename U>
auto BDT_evaluate(T &df, U &model) {
  std::vector< std::string > variables = model.GetVariableNames();
  
  return df.Define("mvaBDTG", Compute<4,float>(model) , variables );
}

#endif
