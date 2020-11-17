#ifndef WORKFLOW_H
#define WORKFLOW_H
#include "helper.h"
#include "bdt.h"

std::vector<std::string> TnP_variables = {
  "mcTrue",
  "weight",
  "tag_Ele_trigMVA",
  "event_met_pfmet",
  "event_met_pfphi",
  "pair_mass",
  "Tag_pt",
  "Tag_eta",
  "Tag_phi",
  "Tag_charge",
  "Probe_pt",
  "Probe_eta",
  "Probe_phi",
  "Probe_charge",
  "Probe_sieie",
  "Probe_eInvMinusPInv",
  "Probe_dz",
  "Probe_dxy",
  "Probe_cutBased_Fall17_V1",
  "Probe_mvaFall17V1Iso_WP90",
  "Probe_pfRelIso03_all",
  "Tag_cutBased_Fall17_V1",
  "Probe_mvaTTH",
  "Probe_lostHits",
  "Probe_convVeto",
  "Tag_sc_eta",
  "Probe_sc_eta",
  "Probe_3charge",
  "nElectron",
  "Electron_pt"
};

// TnP WORKFLOW 
template <typename T>
auto TnP(T &df , Helper::config_t &cfg) {
  // https://github.com/latinos/LatinoAnalysis/blob/master/NanoGardener/python/modules/addTnpTree.py
  
  // dataset specific variables
  cfg.outputVar = TnP_variables;
  if ( cfg.input.find("2016") != std::string::npos ) cfg.outputVar.push_back("Probe_mvaSpring16GP_WP90");
  if ( cfg.input.find("2016") != std::string::npos ) cfg.outputVar.push_back("Tag_mvaSpring16GP");
  if ( cfg.input.find("2016") != std::string::npos ) cfg.outputVar.push_back("Probe_cutBased_HLTPreSel");

  // single electron trigger treshold
  std::string tagCut="1==1";
  if ( cfg.input.find("2016") != std::string::npos ) tagCut="Tag_pt>32 && abs(Tag_eta)<2.17";
  if ( cfg.input.find("2017") != std::string::npos ) tagCut="Tag_pt>40";
  if ( cfg.input.find("2018") != std::string::npos ) tagCut="Tag_pt>37";

  auto df1 = df
    .Filter("abs(Tag_pdgId)!=13 && abs(Probe_pdgId)!=13"," --> Tag and Probe are electron")
    .Filter("abs(Probe_eta)<2.5"," --> Probe candidate skim")
    .Filter("!(abs(Tag_eta)>= 1.4442 && abs(Tag_eta)<=1.566)"," --> Tag candidate eta skim")
    .Filter( tagCut ," --> Tag candidate pt skim : "+tagCut)
    ;

  // dataset specific
  auto df2 = df1
    .Define("mcTrue", (cfg.isMC) ? "Tag_isGenMatched*Probe_isGenMatched" : "1")
    .Define("weight" , (cfg.isMC) ? "genWeight*puWeight" : "1")
    ;
  auto df3 = (cfg.isMC) ? df2 : df2.Filter("TnP_trigger==1"," --> data is matched to HLT filter");

  // Define variables
  auto df_out = df3
    .Define("tag_Ele_trigMVA","Tag_mvaFall17V1Iso")
    .Define("event_met_pfmet","PuppiMET_pt")
    .Define("event_met_pfphi","PuppiMET_phi")
    .Define("pair_mass","TnP_mass")
    .Define("Tag_sc_eta","Tag_deltaEtaSC+Tag_eta")
    .Define("Probe_sc_eta","Probe_deltaEtaSC+Probe_eta")
    .Define("Probe_3charge","Probe_tightCharge==2")
    ;

  return df_out;
}

// BDT WORKFLOW
template <typename T>
auto BDT( T &df , Helper::config_t &cfg ){
  // https://root.cern/doc/master/tmva003__RReader_8C.html

  cfg.outputVar.push_back("mvaBDTG");

  auto df1 = df
    .Define( "Electron_pt_0" , "Electron_pt[0]" )
    .Define( "Electron_eta_0" , "Electron_eta[0]" )
    .Define( "Electron_miniPFRelIso_chg_0" , "Electron_miniPFRelIso_chg[0]" )
    .Define( "Electron_miniPFRelIso_neu_0" , "(Electron_miniPFRelIso_all-Electron_miniPFRelIso_chg)[0]" )
    .Define( "Electron_dxy_0" , "Electron_dxy[0]" )
    .Define( "Electron_dz_0" , "Electron_dz[0]" )
    .Define( "Electron_sip3d_0" , "Electron_sip3d[0]" )
    .Define( "Electron_mvaFall17V1Iso_WP90_0" , "float(Electron_mvaFall17V1Iso_WP90[0])" )
    .Define( "Jet_btagDeepFlavB_0" , "((Electron_jetIdx >= 0)*(Jet_btagDeepFlavB[Electron_jetIdx[0]]))[0]" )
    .Define( "Electron_jetPtRelv2_0" , "((Electron_jetIdx >= 0)*(Electron_jetPtRelv2[0]))[0]" )
    ;
  auto df2 = BDT_lambda( df1 ); // Electron_jetPtRatio
  
  RReader model("data/xml/TMVAClassification_BDTG.weights.xml");
  auto computeModel = Compute< 11 , float >(model);
  auto variables = model.GetVariableNames();

  //auto df3 = df2.Define("mvaBDTG", computeModel , variables );
  auto df3 = df2.Define("mvaBDTG", computeModel ,
			{
			  "Electron_pt_0", "Electron_eta_0", "Electron_miniPFRelIso_chg_0", "Electron_miniPFRelIso_neu_0", "Electron_dxy_0", "Electron_dz_0", "Electron_sip3d_0", "Electron_mvaFall17V1Iso_WP90_0", "Jet_btagDeepFlavB_0", "Electron_jetPtRelv2_0", "Electron_jetPtRatio"
			    } );
  return df3;
}

#endif
