#!/bin/bash

set -e

INPUT_DIR="/media/shoh/02A1ACF427292FC0/latinov5"
OUTPUT_DIR=$1

# Compile executable
echo ">>> Compile skimming executable ..."
COMPILER=$(root-config --cxx)
FLAGS=$(root-config --cflags --libs)
time $COMPILER -g -O3 -Wall -Wextra -Wpedantic -o skim skim.cxx $FLAGS

# Skim samples
while IFS=, read -r DIR SAMPLE LUMI WEIGHT1 WEIGHT2
do
    [[ $DIR =~ ^#.* ]] && continue

    #Make folder
    YEAR=`echo ${DIR} | awk -F "/" '{print $2}'`
    if [ ! -e ${OUTPUT_DIR}/${YEAR} ]; then
	mkdir -p ${OUTPUT_DIR}/${YEAR}
    fi

    echo ">>> Skim sample ${SAMPLE}"
    INPUT=${INPUT_DIR}${DIR}/
    if [[ $DIR == *"fake"* ]]; then
	OUTPUT=${OUTPUT_DIR}/${YEAR}/${SAMPLE}_fake_Skim.root
    else
	OUTPUT=${OUTPUT_DIR}/${YEAR}/${SAMPLE}_Skim.root
    fi
    echo "./skim $INPUT $OUTPUT $SAMPLE $LUMI $WEIGHT1"
    ./skim $INPUT $OUTPUT $SAMPLE $LUMI $WEIGHT1

done < skim.csv
