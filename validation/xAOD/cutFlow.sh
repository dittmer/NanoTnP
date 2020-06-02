#!/bin/bash

# inspect homogeniety of event selection across miniaod and nanoaod

set -e

# 2016
#Data
#python cutFlow.py \
#    /eos/cms/store/group/phys_egamma/soffi/TnP/ntuples_04162018-Legacy2016/Legacy16_V1/data/TnPTree_07Aug17_Run2016BCDEFGH.root \
#    /afs/cern.ch/work/s/shoh/analysis/TnP/NanoTnP/skim/results/latinov7_16/SingleElectron_Run2016.root \
#    Data_16

#LO
python cutFlow.py \
    /eos/cms/store/group/phys_egamma/soffi/TnP/ntuples_04162018-Legacy2016/Legacy16_V1/mc/TnPTree_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_all.root \
    ../../skim/results/latinov7_16/DYJetsToLL_M-50-LO_ext1.root \
    DY_LO_16

# 2017
#python cutFlow.py \
#    /eos/cms/store/user/pchen/HWW/TnPTrees/Moriond18_V1/data/SingleElectron/TnPTree_17Nov2017_RunB.root \
#    /afs/cern.ch/work/s/shoh/analysis/TnP/NanoTnP/skim/results/latinov7_17/RunB.root \
#    Data_B_17

#python cutFlow.py \
#    /eos/cms/store/user/pchen/HWW/TnPTrees/Moriond18_V1/data/SingleElectron/TnPTree_17Nov2017_RunC.root \
#    /afs/cern.ch/work/s/shoh/analysis/TnP/NanoTnP/skim/results/latinov7_17/RunC.root \
#    Data_C_17

#python cutFlow.py \
#    /eos/cms/store/user/pchen/HWW/TnPTrees/Moriond18_V1/data/SingleElectron/TnPTree_17Nov2017_RunD.root \
#    /afs/cern.ch/work/s/shoh/analysis/TnP/NanoTnP/skim/results/latinov7_17/RunD.root \
#    Data_D_17

#python cutFlow.py \
#    /eos/cms/store/user/pchen/HWW/TnPTrees/Moriond18_V1/data/SingleElectron/TnPTree_17Nov2017_RunE.root \
#    /afs/cern.ch/work/s/shoh/analysis/TnP/NanoTnP/skim/results/latinov7_17/RunE.root \
#    Data_E_17

#python cutFlow.py \
#    /eos/cms/store/user/pchen/HWW/TnPTrees/Moriond18_V1/data/SingleElectron/TnPTree_17Nov2017_RunF.root \
#    /afs/cern.ch/work/s/shoh/analysis/TnP/NanoTnP/skim/results/latinov7_17/RunF.root \
#    Data_F_17

#LO
#python cutFlow.py \
#    /eos/cms/store/user/pchen/HWW/TnPTrees/Moriond18_V1/mc/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/TnPTree_DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8_all.root \
#    /afs/cern.ch/work/s/shoh/analysis/TnP/NanoTnP/skim/results/latinov7_17/DYJetsToLL_M-50-LO_ext1.root \
#    DY_LO_17

# 2018
#Data
#python cutFlow.py \
#    /eos/cms/store/group/phys_egamma/swmukher/rereco2018/ECAL_NOISE/RunABCD.root \
#    /afs/cern.ch/work/s/shoh/analysis/TnP/NanoTnP/skim/results/latinov7_18/EGamma_Run2018.root \
#    Data_18

#LO
#python cutFlow.py \
#    /eos/cms/store/group/phys_egamma/swmukher/rereco2018/ECAL_NOISE/DYJetsToLLmadgraphMLM.root \
#    /afs/cern.ch/work/s/shoh/analysis/TnP/NanoTnP/skim/results/latinov7_18/DYJetsToLL_M-50-LO.root \
#    DY_LO_18
