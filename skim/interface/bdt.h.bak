#ifndef BDT_H
#define BDT_H
#include "helper.h"

// https://github.com/root-project/root/blob/master/tutorials/tmva/tmva003_RReader.C
using namespace TMVA::Experimental;

template <typename T>
auto BDT_preprocess(T &df) {
  using namespace ROOT::VecOps;

  // lambda function 1 : jet_btagDeepFlavB
  auto jetbtagdeep = [](
			const RVec<float> &jet_btagDeepFlavB,
			const RVec<int> &electron_jetIdx
			){
    size_t nsize = electron_jetIdx.size();
    RVec<float> out( nsize );
    int iter;
    for ( size_t i = 0 ; i < nsize ; i++ ) {
      iter = i;
      if ( electron_jetIdx[i] < 0 ) iter = 0;
      out.push_back(jet_btagDeepFlavB[iter]);
    }
    return out;
  };
  
  // lambda function 2 : Electron_jetPtRatio
  auto jetPtRatio = [](
		       const RVec<float> &electron_miniPFRelIso_all,
		       const RVec<int> &electron_jetIdx,
		       const RVec<float> &electron_pt,
		       const RVec<float> &jet_pt
		       ){
    // Electron_jetPtRatio
    // proxy for jet pt when no associated jets found
    size_t nsize = electron_pt.size();
    RVec<float> electron_jetPtRatio( nsize );
    
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
    return electron_jetPtRatio;
  };

  // https://github.com/srudra3/LeptonID/blob/master/ClassificationBDT_ele.py
  return df
    .Define( "Electron_miniPFRelIso_chg_bdt" , "Electron_miniPFRelIso_chg" )
    .Define( "Electron_miniPFRelIso_neu_bdt" , "Electron_miniPFRelIso_all-Electron_miniPFRelIso_chg" )
    .Define( "Electron_dxy_bdt" , "log(Electron_dxy[Electron_dxy > 0])" )
    .Define( "Jet_btagDeepFlavB_bdt" , jetbtagdeep , { "Jet_btagDeepFlavB" , "Electron_jetIdx" } )
    .Define( "Electron_jetPtRelv2_bdt" , "Electron_jetPtRelv2[Electron_jetIdx >0]" )
    .Define( "Electron_jetPtRatio_bdt" , jetPtRatio , { "Electron_miniPFRelIso_all" , "Electron_jetIdx" , "Electron_pt" , "Jet_pt" } )
    ;
}


template<typename T>
auto BDT_reader( T &df , const std::vector<TMVA::Reader*> &readers , const std::string &bdtname ) {
  using namespace ROOT::VecOps;
  
  auto predict = [&](
		    unsigned int nslot,
		    const RVec<float> &electron_miniPFRelIso_chg,
		    const RVec<float> &electron_miniPFRelIso_neu,
		    const RVec<float> &electron_dxy,
		    const RVec<float> &jet_btagDeepFlavB,
		    const RVec<float> &electron_jetPtRelv2,
		    const RVec<float> &electron_jetPtRatio
		    ){
    
    size_t nsize = electron_miniPFRelIso_chg.size();
    RVec<float> out(nsize);
    for ( size_t i = 0 ; i < nsize ; i++ ) {
      Helper::electron_miniPFRelIso_chg_[nslot] = electron_miniPFRelIso_chg[i];
      Helper::electron_miniPFRelIso_neu_[nslot] = electron_miniPFRelIso_neu[i];
      Helper::electron_dxy_[nslot]              = electron_dxy[i];
      Helper::jet_btagDeepFlavB_[nslot]         = jet_btagDeepFlavB[i];
      Helper::electron_jetPtRelv2_[nslot]       = electron_jetPtRelv2[i];
      Helper::electron_jetPtRatio_[nslot]       = electron_jetPtRatio[i];
      out.push_back(readers[nslot]->EvaluateMVA(bdtname));
    }
    return out;
  };
  
  return df.DefineSlot( "mvaBDT" , predict ,
			{
			  "Electron_miniPFRelIso_chg_bdt" ,
			    "Electron_miniPFRelIso_neu_bdt" ,
			    "Electron_dxy_bdt" ,
			    "Jet_btagDeepFlavB_bdt" ,
			    "Electron_jetPtRelv2_bdt" ,
			    "Electron_jetPtRatio_bdt"
			    }
			);
}

#endif
