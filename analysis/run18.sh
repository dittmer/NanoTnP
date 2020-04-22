#!/bin/bash

WP="passingprobeTightHWW_ttHMVA_0p7"

for config in settings_18.py #settings_17.py settings_18.py
do
    python tnpEGM_fitter.py ${config} --flag ${WP} --checkBins
    python tnpEGM_fitter.py ${config} --flag ${WP} --createBins
    python tnpEGM_fitter.py ${config} --flag ${WP} --createHist
    python tnpEGM_fitter.py ${config} --flag ${WP} --doFit
    python tnpEGM_fitter.py ${config} --flag ${WP} --doFit --mcSig --altSig
    python tnpEGM_fitter.py ${config} --flag ${WP} --doFit --altSig
    python tnpEGM_fitter.py ${config} --flag ${WP} --doFit --altBkg
    python tnpEGM_fitter.py ${config} --flag ${WP} --sumUp
    
done
