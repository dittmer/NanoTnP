#ifndef TNPPAIRS_SEQUENCE_H
#define TNPPAIRS_SEQUENCE_H
#include "config.h"
#include "helper.h"

/* tnpEleIDs 
 * note: its reco object pairing, no gen-selection
*/
template <typename T>
auto tnpEleIDs(T &df){
  using namespace ROOT::VecOps;
  //####### Lambda function
  auto tnpPairing = [](
		       RVec<int>& tagele ,
		       RVec<int>& probeele,
		       RVec<float>& pt ,
		       RVec<float>& eta ,
		       RVec<float>& phi ,
		       RVec<float>& m ,
		       RVec<int>& eleIdx
		       )
    {
      auto comb = Combinations(tagele,probeele);
      const auto paircomb = comb[0].size();
      std::vector<std::pair<std::pair<int,int>,float>> pair_candidate; // store idx and mass
      // step 1 , make pair 
      for(size_t i = 0 ; i < paircomb ; i++){
	const auto tagIdx = comb[0][i];
	const auto probeIdx = comb[1][i];
	if (tagIdx==probeIdx) continue;
	//passing tag and probe flag gaurantee were looking at electron in Lepton collection
	if (!tagele[tagIdx]) continue;
	if (!probeele[probeIdx]) continue;
	//recover Idx for Electron collection
	auto tag_eleIdx= eleIdx[tagIdx]; auto probe_eleIdx= eleIdx[probeIdx];
	TLorentzVector tot = Helper::VectorMaker( pt[tagIdx] , eta[tagIdx] , phi[tagIdx] , m[tag_eleIdx] );
	TLorentzVector probe = Helper::VectorMaker( pt[probeIdx] , eta[probeIdx] , phi[probeIdx] , m[probe_eleIdx] );
	tot+=probe;
	if ( tot.M() > 50 && tot.M() < 130 ) pair_candidate.push_back(std::make_pair(std::make_pair(tagIdx,probeIdx),tot.M()));
      }
      // step 2 , select pair closest to the Z mass pole, 91.18 GeV
      auto nTnP = pair_candidate.size();
      float zmassMin=999; int pairIdx = -1, tIdx = -1, pIdx = -1; int randomness = -1.; std::pair<int,int> tagProbePair;
      if (nTnP==0){
	return std::vector<int>({tIdx , pIdx , static_cast<int>(nTnP) , pairIdx , randomness });
      }
      else if (nTnP==1){
	tagProbePair=pair_candidate[0].first;
      }
      else {
	for (size_t ipair = 0 ; ipair < nTnP ; ipair++){
	  auto zmass = pair_candidate[ipair].second;
	  if ( abs(zmass-91.18) < zmassMin ){
	    zmassMin = abs(zmass-91.18);
	    pairIdx = ipair;
	  }//
	} // end of for loop
	tagProbePair=pair_candidate[pairIdx].first;
      } // end of if
      
      // step 3 , randomize tag selection if two electron statisfies tag selection
      if ( tagele[tagProbePair.first] && !tagele[tagProbePair.second] ){
	tIdx = tagProbePair.first; pIdx = tagProbePair.second;
      }
      else if ( tagele[tagProbePair.first] && tagele[tagProbePair.second] ){
	// throw random number
	TRandom3 generator = TRandom3(0);
	randomness = static_cast<int>(generator.Uniform(1.,5.)) % 2 ;
	if ( randomness == 0 ){ //even
	  tIdx = tagProbePair.first; pIdx = tagProbePair.second;
	}
	else{
	  tIdx = tagProbePair.second; pIdx = tagProbePair.first;
	}
      } // end of if
      
      return std::vector<int>({ tIdx , pIdx , static_cast<int>(nTnP) , pairIdx , randomness });
      
    };
  //####### Lambda function
  return df
    .Define("tnpEleIDs", tnpPairing, { "tagEle" , "probeEle" , "Lepton_pt" , "Lepton_eta" , "Lepton_phi" , "Electron_mass" , "Lepton_electronIdx" } )
    .Define("tag_Idx", "tnpEleIDs[0]")
    .Define("probe_Idx","tnpEleIDs[1]")
    .Define("nTnP","tnpEleIDs[2]")
    .Define("ipair","tnpEleIDs[3]")
    .Define("randomness","tnpEleIDs[4]")
    .Define("validPair","tag_Idx!=-1 && probe_Idx!=-1")
    ;
  
}

#endif
