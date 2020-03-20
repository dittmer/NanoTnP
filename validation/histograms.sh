#!/bin/bash
# 2017
python histograms.py ../results/latinov5_17/DYJetsToLL_M-50-LO.root DYJetsToLL_M-50-LO
python histograms.py ../results/latinov5_17/DYJetsToLL_M-50_ext1.root DYJetsToLL_M-50_ext1
python histograms.py ../results/latinov5_17/SingleElectron_Run2017.root Data_Run2017

# 2018
python histograms.py ../results/latinov5_18/DYJetsToLL_M-50-LO.root DYJetsToLL_M-50-LO
python histograms.py ../results/latinov5_18/DYJetsToLL_M-50_ext.root DYJetsToLL_M-50_ext
python histograms.py ../results/latinov5_18/EGamma_Run2018.root Data_Run2018
