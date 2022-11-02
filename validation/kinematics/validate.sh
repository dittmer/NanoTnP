#!/bin/bash

# HIPM BCDE
python plot.py -n UL2016BCDE_HIPM -l 16.80 -r \
    ../../skim/results/latino16_HIPM/merged/DYJetsToLL_M-50-LO.root \
    ../../skim/results/latino16_HIPM/merged/DYJetsToLL_M-50.root \
    ../../skim/results/latino16_HIPM/merged/SingleElectron_Run2016BCDE.root \

# HIPM F
python plot.py -n UL2016F_HIPM -l 2.72 -r \
    ../../skim/results/latino16_HIPM/merged/DYJetsToLL_M-50-LO.root \
    ../../skim/results/latino16_HIPM/merged/DYJetsToLL_M-50.root \
    ../../skim/results/latino16_HIPM/merged/SingleElectron_Run2016F-HIPM_UL2016-v1.root \
 
# HIPM
python plot.py -n UL2016_HIPM -l 19.52 -r \
    ../../skim/results/latino16_HIPM/merged/DYJetsToLL_M-50-LO.root \
    ../../skim/results/latino16_HIPM/merged/DYJetsToLL_M-50.root \
    ../../skim/results/latino16_HIPM/merged/SingleElectron_Run2016BCDE.root \
    ../../skim/results/latino16_HIPM/merged/SingleElectron_Run2016F-HIPM_UL2016-v1.root \
 
# noHIPM
python plot.py -n UL2016_noHIPM -l 16.81 -r \
    ../../skim/results/latino16_noHIPM/merged/DYJetsToLL_M-50-LO.root \
    ../../skim/results/latino16_noHIPM/merged/DYJetsToLL_M-50.root \
    ../../skim/results/latino16_noHIPM/merged/SingleElectron.root \

# 2017
python plot.py -n UL2017_BCDEF -l 41.48 -r \
    ../../skim/results/latino17/merged/DYJetsToLL_M-50_LO.root \
    ../../skim/results/latino17/merged/DYJetsToLL_M-50_ext1.root \
    ../../skim/results/latino17/merged/SingleElectron_Run2017B.root \
    ../../skim/results/latino17/merged/SingleElectron_Run2017C.root \
    ../../skim/results/latino17/merged/SingleElectron_Run2017D.root \
    ../../skim/results/latino17/merged/SingleElectron_Run2017E.root \
    ../../skim/results/latino17/merged/SingleElectron_Run2017F.root

# 2018
python plot.py -n UL2018_ABCD -l 59.83 -r \
    ../../skim/results/latino18/merged/DYJetsToLL_M-50_LO.root \
    ../../skim/results/latino18/merged/DYJetsToLL_M-50_ext1.root \
    ../../skim/results/latino18/merged/EGamma_Run2018A.root \
    ../../skim/results/latino18/merged/EGamma_Run2018B.root \
    ../../skim/results/latino18/merged/EGamma_Run2018C.root \
    ../../skim/results/latino18/merged/EGamma_Run2018D.root
