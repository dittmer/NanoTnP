#!/bin/bash

set -e

nano18="
DYJetsToLL_M-50-LO
DYJetsToLL_M-50_ext2
EGamma_Run2018
"

# 2018
for jproc in ${nano18}
do
    python histograms.py ../../skim/results/latinov7_18/${jproc}.root ${jproc}
done
hadd -f ./results/latinov7_18/histogram.root ./results/latinov7_18/*.root

##nominal plotting
python plot.py ./results/latinov7_18/histogram.root ./results/latinov7_18/DYJetsToLL_M-50-LO/ DYJetsToLL_M-50-LO
##altMC plotting
python plot.py ./results/latinov7_18/histogram.root ./results/latinov7_18/DYJetsToLL_M-50_ext2/ DYJetsToLL_M-50_ext2
