#!/bin/bash

# 41.48
python plot.py -n UL2017_BCDEF -l 41.48 -r \
    ../../skim/results/latino17/DYJetsToLL_M-50_LO.root \
    ../../skim/results/latino17/DYJetsToLL_M-50.root \
    ../../skim/results/latino17/SingleElectron_Run2017B.root \
    ../../skim/results/latino17/SingleElectron_Run2017C.root \
    ../../skim/results/latino17/SingleElectron_Run2017D.root \
    ../../skim/results/latino17/SingleElectron_Run2017E.root \
    ../../skim/results/latino17/SingleElectron_Run2017F.root

# 59.83
python plot.py -n UL2018_ABCD -l 59.83 -r \
    ../../skim/results/latino18/DYJetsToLL_M-50_LO.root \
    ../../skim/results/latino18/DYJetsToLL_M-50.root \
    ../../skim/results/latino18/EGamma_Run2018A.root \
    ../../skim/results/latino18/EGamma_Run2018B.root \
    ../../skim/results/latino18/EGamma_Run2018C.root \
    ../../skim/results/latino18/EGamma_Run2018D.root
