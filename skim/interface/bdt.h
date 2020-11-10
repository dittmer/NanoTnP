#ifndef BDT_H
#define BDT_H
#include "helper.h"
#include "TMVA/RReader.hxx"
#include "TMVA/RInferenceUtils.hxx"

// https://github.com/root-project/root/blob/master/tutorials/tmva/tmva003_RReader.C
using namespace TMVA::Experimental;

template <typename T, typename U>
auto BDT_evaluate(T &df, U &model) {
  std::vector< std::string > variables = model.GetVariableNames();
  return df.Define("mvaBDTG", Compute<4,float>(model) , variables );
}

#endif
