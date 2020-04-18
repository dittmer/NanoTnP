#ifndef HELPER_H
#define HELPER_H
#include "ROOT/RDataFrame.hxx"
#include "ROOT/RDFHelpers.hxx"
#include "ROOT/RVec.hxx"

#include "Math/Vector4D.h"
#include "TStopwatch.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <cmath>
#include <map>

#include "utility" // std::pair
#include <algorithm> // for std::find
#include <iterator> // for std::begin, std::end

#include "TRandom3.h"
#include "TLorentzVector.h"

namespace Helper {

  /*
   * sort Gen matched candidates in ascending order
   */
  struct dRSorter {
    bool operator() (std::pair<std::pair<int,int>,float> i , std::pair<std::pair<int,int>,float> j) { return ( (i.second) < (j.second) ); }
  };

  template <typename T>
    std::vector<int> IndexBydeltaR(T v){
    dRSorter comparator;
    std::vector<int> indecies;
    std::sort (v.begin() , v.end() , comparator);
    for (auto it = v.begin() ; it != v.end() ; ++it){
      indecies.push_back((*it).first.second);
    }
    return indecies;
  }
  
  /*
   * bit decoder
   */
  template <typename T>
    int bitdecoder( T decimal , T kbit){
    // shift bit from the left to right and inspect with AND operator, it will return decimal 
    //int on=0;
    //if ( decimal & ( 1 << kbit ) ) on=1;
    //return on;
    // shift to specific bit to the left, and query on/off state
    return (decimal >> kbit & 1);
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

  template<typename T>
    TLorentzVector VectorMaker(T pt, T eta, T phi, T m){
    TLorentzVector out;
    out.SetPtEtaPhiM(pt,eta,phi,m);
    return out;
  }

  /*
   * Json reader (author: the overlord Afiq @DESY)
   */
  template <typename T>
    std::map<int, std::vector<std::pair<int, int> > > parseJSONAsMap(const T jsonFile) {
    std::map<int, std::vector<std::pair<int, int> > > m_json;
    
    std::ifstream json(jsonFile);
    std::string line;

    while (getline(json, line)) {
      
      const unsigned qCount = std::count(line.begin(), line.end(), '"');
      if (qCount < 2 or qCount % 2 != 0) continue; // skip this line, weird that we don't have even quote count
      
      unsigned q1 = line.find('"');
      while (q1 < line.length()) {

	unsigned q2 = line.find('"', q1 + 1);
	
	unsigned bo1 = line.find('[', q1 + 1);
	if (line.find('[', bo1 + 1) == bo1 + 1)
	  ++bo1;
  
	unsigned bc1 = line.find(']', bo1 + 1);
	if (bo1 > q1 and bo1 > q2) {
	  const int nRun = std::stoi( line.substr(q1 + 1, q2 - q1 - 1) );
	  std::vector<std::pair<int, int> > vp_lumi;
	  while (true) {
	    if (bc1 > bo1) {
	      const std::string sLumi = line.substr(bo1 + 1, bc1 - bo1 - 1);
	      const unsigned comma = sLumi.find(',');
	      
	      vp_lumi.push_back( std::make_pair(std::stoi(sLumi.substr(0, comma)),
						std::stoi(sLumi.substr(comma + 1, sLumi.length() - comma))) );
	    }
	    if (line.find(']', bc1 + 1) == bc1 + 1) break; 
	    bc1 = line.find(']', bc1 + 1);
	    bo1 = line.find('[', bo1 + 1);
	  } 
	  m_json.insert( std::make_pair(nRun, vp_lumi) );
	}	
	q1 = line.find('"', q2 + 1);
      }
    }    
    return m_json;
  }
 
  /*
   * json selector (author: the overlord Afiq @DESY)
   */
  template <typename T>
    bool isRunLumiInJSON(const std::map<T, std::vector<std::pair<T, T> > > m_json, T nRun, T nLumi) {
    
    if (m_json.empty())
      return true;
    
    for (auto &p_json : m_json) {
      if (nRun == p_json.first) {
	for (auto &p_lumi : p_json.second) {
	  if (nLumi >= p_lumi.first and nLumi <= p_lumi.second)
	    return true;
	}
      }
    }
    return false;
  }

  /**  
  struct config_t {
    // basic
    bool isMC=false;
    bool passHLT=false;
    // jet cleaning
    float jetclean_dR=0.3;
    //trigger
    std::string name = "NULL";
    std::string bit = "NULL";
    // tag and pair flag
    bool resolveAmbiguity=false;
    // cert
    std::string jsonFile;
    //std::map<int, std::vector<std::pair<int, int> > > jsonFile;
  };
  **/
} //helper

#endif
