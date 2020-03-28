#!/bin/bash

nano17="
DYJetsToLL_M-50-LO_ext1
DYJetsToLL_M-50_ext1
SingleElectron_Run2017
"

nano18="
DYJetsToLL_M-50-LO
DYJetsToLL_M-50_ext
EGamma_Run2018
"

# 2017
for iproc in ${nano17}
do
    python histograms.py ../ntuple/results/latinov5_17/${iproc}.root ${iproc}
done
hadd -f ./results/latinov5_17/histogram.root ./results/latinov5_17/*.root

##nominal plotting
python plot.py ./results/latinov5_17/histogram.root ./results/latinov5_17/plots/DYJetsToLL_M-50-LO_ext1/ DYJetsToLL_M-50-LO_ext1
##altMC plotting
python plot.py ./results/latinov5_17/histogram.root ./results/latinov5_17/plots/DYJetsToLL_M-50_ext1/ DYJetsToLL_M-50_ext1

# 2018
for jproc in ${nano18}
do
    python histograms.py ../ntuple/results/latinov5_18/${jproc}.root ${jproc}
done
hadd -f ./results/latinov5_18/histogram.root ./results/latinov5_18/*.root

##nominal plotting
python plot.py ./results/latinov5_18/histogram.root ./results/latinov5_18/DYJetsToLL_M-50-LO/ DYJetsToLL_M-50-LO
##altMC plotting
python plot.py ./results/latinov5_18/histogram.root ./results/latinov5_18/DYJetsToLL_M-50_ext/ DYJetsToLL_M-50_ext
