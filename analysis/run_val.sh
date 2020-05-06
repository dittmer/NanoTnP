#!/bin/bash

set -e

WP="passingMVA94Xwp90isoHWWiso0p06"

for config in settings_18_val.py #settings_18_val.py
do
    python tnpEGM_fitter.py settings/${config} --flag ${WP} --checkBins
    python tnpEGM_fitter.py settings/${config} --flag ${WP} --createBins
    python tnpEGM_fitter.py settings/${config} --flag ${WP} --createHist
    python tnpEGM_fitter.py settings/${config} --flag ${WP} --doFit
    python tnpEGM_fitter.py settings/${config} --flag ${WP} --doFit --mcSig --altSig
    python tnpEGM_fitter.py settings/${config} --flag ${WP} --doFit --altSig
    python tnpEGM_fitter.py settings/${config} --flag ${WP} --doFit --altBkg
    python tnpEGM_fitter.py settings/${config} --flag ${WP} --sumUp
    
done
