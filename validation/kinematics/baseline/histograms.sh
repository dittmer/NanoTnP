#!/bin/bash

set -e

nano16="
DYJetsToLL_M-50-LO_ext2
DYJetsToLL_M-50
SingleElectron_Run2016
"

nano17="
DYJetsToLL_M-50-LO_ext1
DYJetsToLL_M-50_ext1
SingleElectron_Run2017
"

nano18="
DYJetsToLL_M-50-LO
DYJetsToLL_M-50_ext2
EGamma_Run2018
"

# 2016
for iproc in ${nano16}
do
    python histograms.py ../ntuple/results/latinov6_16/${iproc}.root ${iproc}
done
hadd -f ./results/latinov6_16/histogram.root ./results/latinov6_16/*.root

##nominal plotting
python plot.py ./results/latinov6_16/histogram.root ./results/latinov6_16/plots/DYJetsToLL_M-50-LO_ext2/ DYJetsToLL_M-50-LO_ext2
##altMC plotting
python plot.py ./results/latinov6_16/histogram.root ./results/latinov6_16/plots/DYJetsToLL_M-50/ DYJetsToLL_M-50


# 2017
for iproc in ${nano17}
do
    python histograms.py ../ntuple/results/latinov6_17/${iproc}.root ${iproc}
done
hadd -f ./results/latinov6_17/histogram.root ./results/latinov6_17/*.root

##nominal plotting
python plot.py ./results/latinov6_17/histogram.root ./results/latinov6_17/plots/DYJetsToLL_M-50-LO_ext1/ DYJetsToLL_M-50-LO_ext1
##altMC plotting
python plot.py ./results/latinov6_17/histogram.root ./results/latinov6_17/plots/DYJetsToLL_M-50_ext1/ DYJetsToLL_M-50_ext1

# 2018
for jproc in ${nano18}
do
    python histograms.py ../ntuple/results/latinov6_18/${jproc}.root ${jproc}
done
hadd -f ./results/latinov6_18/histogram.root ./results/latinov6_18/*.root

##nominal plotting
python plot.py ./results/latinov6_18/histogram.root ./results/latinov6_18/DYJetsToLL_M-50-LO/ DYJetsToLL_M-50-LO
##altMC plotting
python plot.py ./results/latinov6_18/histogram.root ./results/latinov6_18/DYJetsToLL_M-50_ext2/ DYJetsToLL_M-50_ext2
