#include "ROOT/RDataFrame.hxx"
#include "ROOT/RVec.hxx"

#include "Math/Vector4D.h"
#include "TStopwatch.h"

#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <cmath>

#include "utility" // std::pair
#include <algorithm> // for std::find
#include <iterator> // for std::begin, std::end

#include "TRandom3.h"
#include "TLorentzVector.h"

namespace Helper {
  
  /*
   * sort TnP electron candidates in pT descending order
   */
  struct pTSorter {
    bool operator() (std::pair<int,double> i , std::pair<int,double> j) { return ( (i.second) > (j.second) ); }
  };
  
  template <typename T>
    std::vector<std::pair<int,double>> IndexBypT(T v){
    pTSorter comparator;
    std::sort (v.begin() , v.end() , comparator);
    return v;
  }
  
  /*
   * Compute the difference in the azimuth coordinate taking the boundary conditions at 2*pi into account.
   */
  template <typename T>
    float DeltaPhi(T v1, T v2, const T c = M_PI)
    {
      auto r = std::fmod(v2 - v1, 2.0 * c);
      if (r < -c) {
	r += 2.0 * c;
      }
      else if (r > c) {
	r -= 2.0 * c;
      }
      return r;
    }
}

/*
 * template
 */

/*
 * soft skim
 */
template <typename T>
auto Filterbaseline(T &df) {
  return df.Filter("nElectron>0","None zero electrons")
    .Filter("nMuon==0","no Muon")
    .Filter("nJet>0","at least 1 jet");
}


/*
 * Kinematically good electrons
 * Bool Vec
 */
template <typename T>
auto FindGoodElectron(T &df) {
  return df.Define("goodElectron","Electron_pt > 5 && abs(Electron_eta) < 2.1 && Electron_cutBased > 0"); // ID ?
}

/*
 * Kinematically good jets
 * Bool Vec
 */
template <typename T>
auto FindGoodJet(T &df) {
  return df.Define("goodJet","Jet_pt > 30 && abs(Jet_eta) < 2.5 && Jet_jetId > 0 && Jet_puId > 4");
}

/*
 * Reduce the dataset to the interesting events.
 */
template <typename T>
auto FilterGoodEvents(T &df) {
  return df.Filter("Sum(goodElectron)>0","Event has good electrons")
           .Filter("Sum(goodJet)>0","Event has good jets");
}

/*
 * clean electron from jets
 */
template <typename T>
auto FindCleanFromJet(T &df) {
  using namespace ROOT::VecOps;
  auto pair_Builder = [](RVec<int>& goodElectron, RVec<float>& eta_1, RVec<float>& phi_1,
                       RVec<int>& goodJet, RVec<float>& eta_2, RVec<float>& phi_2)
    {
      
      // Get indices of all possible combinations
      auto comb = Combinations(eta_1,eta_2);
      const auto numComb = comb[0].size();
      // Find match pair and eliminate the electron
      RVec<int> CleanElectron(eta_1.size(),0);
      for (size_t i = 0 ; i < numComb ; i++) {
        const auto i1 = comb[0][i];
        const auto i2 = comb[1][i];
        if( goodElectron[i1] == 1 && goodJet[i2] == 1 ){
          const auto deltar = sqrt(
				   pow(eta_1[i1] - eta_2[i2], 2) +
				   pow(Helper::DeltaPhi(phi_1[i1], phi_2[i2]), 2));
          if (deltar > 0.3) {
	    CleanElectron[i1] = 1; // no match, good electron
	  }
	}
      }
      
      return CleanElectron;
    };
  
  return df.Define("cleanFromJet", pair_Builder,
		   {"goodElectron", "Electron_eta", "Electron_phi",
                       "goodJet", "Jet_eta" , "Jet_phi"})
    .Filter("Sum(cleanFromJet==1)>=2","At least two Clean Electrons");
}

/*
 * pass standard tight selection
 */
template <typename T>
auto FindPassTightCutBased(T &df) { // tight cut base , detector-level cuts??

  return df.Define("passTightCutBased","1");
}

/*
 * Find match to trigger electron
 */
//template <typename T>
//auto FindTriggerMatchedElectron(T &df) {
//  using namespace ROOT::VecOps;
//  auto trig_Matcher = [](RVec<>) {
    
//  };
//}
/*
 * TnP candidate
 */

template <typename T>
auto FindTnP(T &df) {
  using namespace ROOT::VecOps;
  auto tnp_pair = [](RVec<int>& isEle_cleaned, RVec<float>& pt , RVec<float>& eta , RVec<float>& phi , RVec<float>& mass)
    {

      // Build collection of clean electron with pT ordered
      std::vector<std::pair<int,double>> pT_unordered;
      const auto elec_size = isEle_cleaned.size();
      // consider only clean Electron
      for(size_t i = 0 ; i < elec_size ; i++) {
	if (isEle_cleaned[i]==1)
	  pT_unordered.push_back(std::make_pair(i,pt[i]));
      }

      // sorted in descending Pt
      std::vector<std::pair<int,double>> pT = Helper::IndexBypT(pT_unordered);
      
      // randomly selecting Tag and Probe
      TRandom3 rand(0);
      int TagIdx = -1;
      TagIdx = pT[ static_cast<int>(rand.Uniform()*10) % 2 ].first;
      
      // match tag to trigger filter
      
      // probe
      double mindis= 99999.;
      int ProbeIdx = -1;
      TLorentzVector tag, probe, tot;
      tag.SetPtEtaPhiM( pt[TagIdx] , eta[TagIdx] , phi[TagIdx] , mass[TagIdx] );
      for(size_t j = 0 ; j < pT.size() ; j++ ){
	tot.SetPtEtaPhiM(0. , 0. , 0. , 0.);
	tot = tag;
	if ( static_cast<int>(j) == TagIdx ) continue;
	probe.SetPtEtaPhiM( pt[j] , eta[j] , phi[j] , mass[j] );
	tot+=probe;
	// pair which closest to the Z mass pole
	if ( abs(tot.M() - 91.14) < mindis ){
	  mindis = abs(tot.M() - 91.14);
	  ProbeIdx = j;
	}
      }
      return std::vector<int>({TagIdx, ProbeIdx});
    };

  return df.Define("TnPIdx", tnp_pair,
		   {"isEle_cleaned","Electron_pt","Electron_eta","Electron_phi","Electron_mass"})
    .Define("tag_Idx" , "TnPIdx[0]")
    .Define("probe_Idx" , "TnPIdx[1]")
    .Filter("tag_Idx != -1" , "Valid tag")
    .Filter("probe_Idx != -1" , "Valid probe");
}

/*
 * is Tag pass the threshold? Standard Tight ID ; trigger match and kinematics threshold
 */
//template <>


/*
 * Declare variables for analysis
 */

template <typename T>
auto DeclareVariables(T &df) {

  return df.Define("ele_pt1","Electron_pt[0]")
           .Define("ele_pt2","Electron_pt[1]")
           .Define("ele_eta1","Electron_eta[0]")
           .Define("ele_eta2","Electron_eta[1]")
           .Define("isZmassW","60 < mll && mll < 120");
}

/*
 * Add event weights
 */

template <typename T>
auto AddEventWeight(T &df, const std::string& path, const std::string& sample, const std::string& lumi, const std::string& weight1, const std::string& weight2) {
  std::string weights;
  if (path.find("DATA") != std::string::npos) {
    weights = "METFilter_DATA*"+weight1+"*"+weight2;
  }
  else {
    weights = lumi+"*XSWeight*PrefireWeight*GenLepMatch2l*METFilter_MC*("+weight1+")*("+weight2+")";
  }
  std::cout<<" weights interpreted : "<<weights<<std::endl;
  return df.Define( "weight", weights );
}

/*
 * Declare all variables which will end up in the final reduced dataset
 */
const std::vector<std::string> finalVariables = {
  "ele_pt1" , "ele_pt2" , "ele_eta1" , "ele_eta2", "weight", "isEle_cleaned" , "tag_Idx" , "probe_Idx"
};
