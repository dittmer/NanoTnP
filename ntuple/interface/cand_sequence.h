#ifndef CAND_SEQUENCE_H
#define CAND_SEQUENCE_H
#include "config.h"
#include "helper.h"

/* init */
template <typename T , typename U>
auto hltfilter(T &df, config_t &cfg , std::map< U, std::vector< std::pair<U, U> > > &m_json) {

  auto isPassJSON = [&m_json](unsigned& run , unsigned& luminosityBlock)
    {
      int RUN = static_cast<int>(run);
      int LUM = static_cast<int>(luminosityBlock);
      return Helper::isRunLumiInJSON( m_json , RUN, LUM );
    };
  
  if (cfg.name.find("NULL") != std::string::npos){
    std::cout<<" >>> Running MC, no HLT and JSON filter <<< "<<std::endl;
    return df
      .Filter("run==1"," --> MC dummy")
      ;
  }
  else{
    std::cout<<" >>> HLT : "<<cfg.name<<" <<< "<<std::endl;
    std::cout<<" >>> Json : "<<cfg.jsonFile<<" <<< "<<std::endl;

    std::string hlt(cfg.name +=" == true");
    return df
      .Define("passJSON",isPassJSON, { "run" , "luminosityBlock" } )
      .Filter("passJSON == 1"," --> Filtered by Golden Json")
      .Filter(hlt," --> passing trigger "+hlt)
      ;
  }
}

/* tag sequence
   process.goodElectrons             +
   process.tagEleCutBasedTight       +
   process.tagEle
   )
*/

/* goodElectrons */
template <typename T>
auto goodElectrons(T &df, config_t &cfg ) {
  std::string msg(" --> ");
  return df
    .Filter( cfg.LeptonCollection , msg+=cfg.LeptonCollection )
    .Define( "isGoodElectron" , cfg.goodLeptonCut+=" && abs(Lepton_pdgId) == 11" )
    ;
}

/* tagEleCutBasedTight */
template <typename T>
auto tagEleCutBasedTight(T &df, config_t &cfg ) {
  using namespace ROOT::VecOps;
  auto Lepton_isTight = [](
			   RVec<int>& Lepton_electronIdx ,
			   RVec<int>& Electron_flag
			   ) {
    RVec<int> Lepton_isTight(Lepton_electronIdx.size(),0);
    for (size_t i=0 ; i< Lepton_electronIdx.size() ; i++){
      if ( Electron_flag[Lepton_electronIdx[i]] == 4 ){
        Lepton_isTight[i] = 1;
      }
    }
    return Lepton_isTight;
  };
  
  return df
    .Define( "tagEleCut"           ,  cfg.TagCandidate )
    .Define( "tagEleCutBasedTight" , Lepton_isTight , { "Lepton_electronIdx" , "Electron_cutBased_Fall17_V1" } )
    .Define( "tagCandidate" , "isGoodElectron==1 && tagEleCut==1 && tagEleCutBasedTight==1" )
    ;
}

/* tagEle */
template <typename T>
auto tagEle(T &df, config_t &cfg) {
  using namespace ROOT::VecOps;
  //####### Lambda function
  auto eleHLTProducer = [&cfg](
			       RVec<int>&   id,
			       RVec<int>&   filterbits ,
			       RVec<int>&   tagCandidate ,
			       RVec<float>& eta_el ,
			       RVec<float>& phi_el ,
			       RVec<float>& pt_trg ,
			       RVec<float>& eta_trg ,
			       RVec<float>& phi_trg
			       ) {
    // get combination
    auto comb = Combinations(eta_el,eta_trg); // electron-trig object pair
    const auto numComb = comb[0].size();
    RVec<int> trigMatchTag(eta_el.size(),0);  // trigger matcher
    //RVec<int> mctruth(eta_el.size(),-1); // dummy
    for (size_t j=0 ; j < numComb ; j++){
      const auto iele = comb[0][j]; //electron
      const auto itrg = comb[1][j]; //trigobj
      if ( !(tagCandidate[iele])     ) continue;
      //trigger object selection
      if ( abs(id[itrg]) != cfg.LeptonID                                               ) continue;
      //https://github.com/cms-sw/cmssw/blob/master/PhysicsTools/NanoAOD/python/triggerObjects_cff.py#L35-L46
      if ( !Helper::bitdecoder(filterbits[itrg], cfg.bit)                    ) continue;
      // hardcoded
      if ( cfg.year=="2016" && pt_trg[itrg] < 27 && abs(eta_trg[itrg]) > 2.1 ) continue;
      if ( cfg.year=="2017" && pt_trg[itrg] < 35                             ) continue;
      if ( cfg.year=="2018" && pt_trg[itrg] < 32                             ) continue;
      const auto deltar = sqrt( pow(eta_el[iele] - eta_trg[itrg], 2) + pow(Helper::DeltaPhi(phi_el[iele], phi_trg[itrg]), 2));
      if ( deltar > cfg.trig_dR                                              ) continue; // Minimum deltaR for matching
      trigMatchTag[iele] = 1;
    }
    //out = std::make_pair(matcher,mctruth);
    return trigMatchTag;
  };
  //####### Lambda function

  return df
    .Define( "tagEle" , eleHLTProducer , {
	"TrigObj_id",
	  "TrigObj_filterBits",
	  "tagCandidate",
	  "Lepton_eta",
	  "Lepton_phi",
	  "TrigObj_pt",
	  "TrigObj_eta",
	  "TrigObj_phi"
	  }
      )
    ;
}

/* genEle */
template <typename T>
auto genEle(T &df , config_t &cfg) {
  using namespace ROOT::VecOps;
  //####### Lambda function
  auto GenParticleSelector = [&cfg](
				RVec<int>& gen_pdgId ,
				RVec<float>& pt_gen  ,
				RVec<float>& eta_gen ,
				//RVec<float>& phi_gen ,
				RVec<int>& statusflag ,
				RVec<int>& mother_gen ,
				RVec<int>& status_gen
				) {
    RVec<int> goodgen(gen_pdgId.size(),0);
    for (size_t j=0 ; j < gen_pdgId.size() ; j++){
      //https://github.com/cms-analysis/EgammaAnalysis-TnPTreeProducer/blob/master/python/egmTreesSetup_cff.py#L83-L86
      if ( abs(gen_pdgId[j]) != cfg.LeptonID    ) continue;
      if ( pt_gen[j] < 3                        ) continue;
      if ( abs(eta_gen[j]) > 2.7                ) continue;
      // https://github.com/cms-sw/cmssw/blob/master/PhysicsTools/NanoAOD/python/genparticles_cff.py#L48-L80
      if ( status_gen[j]!=1                     ) continue;
      // isPrompt or isDirectPromptTauDecay
      //if ( !Helper::bitdecoder(statusflag[j],0) || !Helper::bitdecoder(statusflag[j],5) ) continue;
      // checking on electron mom
      int momIdx= mother_gen[j];
      // checking on matched electron mom
      while(momIdx!=-1){
	//  Z boson origin, and its prompt
	if ( status_gen[momIdx]==62 && abs(gen_pdgId[momIdx])==23 && Helper::bitdecoder(statusflag[momIdx],0) ){
	  goodgen[j]=1; break;
	}
	momIdx= mother_gen[momIdx];
      } // end of while loop
    } // end of for loop
    return goodgen;
  };
  //####### Lambda function
  return df
    .Define( "genEle" , GenParticleSelector ,
	     {
	       "GenPart_pdgId",
		 "GenPart_pt",
		 "GenPart_eta",
		 //"GenPart_phi",
		 "GenPart_statusFlags",
		 "GenPart_genPartIdxMother",
		 "GenPart_status"
		 }
	     )
    ;
}

/* genTagEle  or genProbe*/
template <typename T>
auto genTagProbeEle(T &df , config_t &cfg , std::string ele_type) {
  using namespace ROOT::VecOps;
  // type_gen_ele = tagEle or probeEle
  std::string outbranch;
  if (ele_type=="tagEle"){
    outbranch="genTagEle";
  }
  else if (ele_type=="probeEle"){
    outbranch="genProbe";
  }

  //####### Lambda function
  auto MCMatcher = [ &cfg ] (
			     RVec<int>& genele,
			     RVec<int>& tagProbeObject, // tagEle or probeEle
			     RVec<float>& pt_el,
			     RVec<float>& eta_el,
			     RVec<float>& phi_el,
			     RVec<float>& gen_pt_el,
			     RVec<float>& gen_eta_el,
			     RVec<float>& gen_phi_el
			     ) {
    RVec<int> mcMatchEle(eta_el.size(),0);
    // get combination
    auto comb = Combinations(eta_el,gen_eta_el); // electron-gen object pair
    const auto numComb = comb[0].size();
    for (size_t j=0 ; j < numComb ; j++){
      const auto iele = comb[0][j]; //electron
      const auto igen = comb[1][j]; //gen
      
      if ( !tagProbeObject[iele]     ) continue;
      if ( !genele[igen] ) continue;

      const auto deltar = sqrt( pow(eta_el[iele] - gen_eta_el[igen], 2) + pow(Helper::DeltaPhi(phi_el[iele], gen_phi_el[igen]), 2));
      const auto dpt_rel = fabs(pt_el[iele] - gen_pt_el[igen]) / gen_pt_el[igen];
      if ( deltar > cfg.gen_dR                     ) continue; // Minimum deltaR for matching
      if ( dpt_rel > cfg.gen_relPt                 ) continue; // Minimum deltaPt/Pt for matching
      mcMatchEle[iele] = 1;
    } // end of for loop
    return mcMatchEle;
  };
  //####### Lambda function

  return df
    .Define( outbranch , MCMatcher ,
	     {
	       "genEle" , // genPart pruned
		 ele_type,
		 "Lepton_pt" ,
		 "Lepton_eta" ,
		 "Lepton_phi" ,
		 "GenPart_pt",
		 "GenPart_eta",
		 "GenPart_phi"
		}
	    )
    ;
}

/* ele_sequence = process.probeEleCutBasedVeto80X
   + process.probeEle
*/

/* process.probe */
template <typename T>
auto probeEle(T &df, config_t &cfg) {
  using namespace ROOT::VecOps;
  
  auto Lepton_isttHMVA = [](
			    RVec<int>& Lepton_electronIdx ,
			    RVec<float>& Electron_flag
			    ) {
    RVec<int> Lepton_isttHMVA(Lepton_electronIdx.size(),0);
    for (size_t i=0 ; i< Lepton_electronIdx.size() ; i++){
      if ( Electron_flag[Lepton_electronIdx[i]] > 0.7 ){
	Lepton_isttHMVA[i] = 1;
      }
    }
    return Lepton_isttHMVA;
  };
  
  return df
    .Define("probeEle", "isGoodElectron==1" )
    .Define("probeEleTightHWW", cfg.denom+"==1")
    .Define("probeElettHMVA" , Lepton_isttHMVA , { "Lepton_electronIdx" , cfg.num })
    .Define("probeTightHWW_ttHMVA_0p7" , "probeEle==1 && probeEleTightHWW==1 && probeElettHMVA==1" )
    ;
}

#endif
