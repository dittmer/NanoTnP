#!/bin/bash

set -e

VER="v6"

if [ `echo ${PWD} | awk -F "/" '{print $NF}'` != "analysis" ]; then

    echo "please run the script on analysis folder"
    exit
fi

TnP () {

    echo "RUNNING"
    python tnpEGM_fitter.py settings/nano${VER}/${1} --flag ${2} --checkBins
    python tnpEGM_fitter.py settings/nano${VER}/${1} --flag ${2} --createBins
    python tnpEGM_fitter.py settings/nano${VER}/${1} --flag ${2} --createHist
    python tnpEGM_fitter.py settings/nano${VER}/${1} --flag ${2} --doFit
    python tnpEGM_fitter.py settings/nano${VER}/${1} --flag ${2} --doFit --mcSig --altSig
    python tnpEGM_fitter.py settings/nano${VER}/${1} --flag ${2} --doFit --altSig
    python tnpEGM_fitter.py settings/nano${VER}/${1} --flag ${2} --doFit --altBkg
    python tnpEGM_fitter.py settings/nano${VER}/${1} --flag ${2} --sumUp

}

# 2016
#TnP settings_nanov6_16.py passingMVA80Xwp90Iso16
#TnP settings_nanov6_16.py passingMVA80Xwp90Iso16SS
#TnP settings_nanov6_16.py passingttHMVA0p7
#TnP settings_nanov6_16.py passingttHMVA0p7SS

# 2017
#TnP settings_nanov6_17.py passingMVA80Xwp90Iso16
#TnP settings_nanov6_17.py passingMVA80Xwp90Iso16SS
#TnP settings_nanov6_17.py passingttHMVA0p7
#TnP settings_nanov6_17.py passingttHMVA0p7SS

# 2018
TnP settings_nanov6_1_18.py passingMVA94Xwp90isoHWWiso0p06
TnP settings_nanov6_1_18.py passingMVA94Xwp90isoHWWiso0p06SS
TnP settings_nanov6_1_18.py passingttHMVA0p7
TnP settings_nanov6_1_18.py passingttHMVA0p7SS
