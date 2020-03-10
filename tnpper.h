#ifndef TNPPER_H
#define TNPPER_H
#include "ROOT/RDataFrame.hxx"
#include "ROOT/RDFHelpers.hxx"
#include "ROOT/RVec.hxx"

#include "Math/Vector4D.h"
#include "TStopwatch.h"

#include <iostream>
#include <fstream>
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
    bool operator() (std::pair<int,float> i , std::pair<int,float> j) { return ( (i.second) > (j.second) ); }
  };

  template <typename T>
    std::vector<int> IndexBypT(T v){
    pTSorter comparator;
    std::vector<int> indecies;
    std::sort (v.begin() , v.end() , comparator);
    for (auto it = v.begin() ; it != v.end() ; ++it){
      indecies.push_back((*it).first);
    }
    return indecies;
  }

  /*
   * bit decoder
   */
  template <typename T>
    int bitdecoder( T decimal , T kbit){
      // shift bit from the left to right and inspect with AND operator
      int on=0;
      if ( decimal & ( 1 << kbit ) ) on=1;
      return on;
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


struct config_t {

  // basic
  bool isMC=false;
  bool passHLT=false;
  // jet cleaning
  float jetclean_dR=0.3;
  //trigger
  std::string name;
  std::string bit;
  // tag passing flag
  bool passfilter=false;
  float passfilter_dR=0.2;
  bool genMatch=false;
  float genMatch_dR=0.3;
  // tag passing flag
  std::string tag_WP;
  std::string tag_sel;
  // probe passig flag
  std::string probe_WP;
  std::string probe_sel;
  // tag and pair flag
  bool resolveAmbiguity=false;

} cfg;

#endif
