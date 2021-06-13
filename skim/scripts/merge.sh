#!/bin/bash

set -e

if [[ -z "${BASETNP}" ]]; then
    echo "environment not set up, please run:"
    echo "source lxplus_setup.sh"
    exit
fi  

for ifold in `ls $BASETNP/skim/results/`
do
    if ls $BASETNP/skim/results/${ifold}/*.sub 1> /dev/null 2>&1 && ls $BASETNP/skim/results/${ifold}/*.root 1> /dev/null 2>&1 ; then 
	mkdir -p $BASETNP/skim/results/${ifold}/merged
	for iname in `ls $BASETNP/skim/results/${ifold}/*.root | xargs -n1 basename | awk -F "__" '{print $1}' | uniq`
	do
	    echo hadding $iname
	    #python $BASETNP/skim/scripts/haddnano.py $BASETNP/skim/results/${ifold}/merged/${iname}.root $BASETNP/skim/results/${ifold}/${iname}__*.root
	done
	# hadd all data
	# for validation
	python $BASETNP/skim/scripts/haddnano.py $BASETNP/skim/results/${ifold}/merged/SingleElectron.root $BASETNP/skim/results/${ifold}/merged/SingleElectron_Run*.root 
    fi
done
