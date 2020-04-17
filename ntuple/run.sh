#!/bin/bash

set -e

if [ -e /cvmfs/sft.cern.ch/lcg/views/LCG_95apython3/x86_64-centos7-gcc7-opt/setup.sh ];then
    echo "source /cvmfs/sft.cern.ch/lcg/views/LCG_95apython3/x86_64-centos7-gcc7-opt/setup.sh"
    source /cvmfs/sft.cern.ch/lcg/views/LCG_95apython3/x86_64-centos7-gcc7-opt/setup.sh
fi

for year in latinov6_17 latinov6_18 #latinov6_16 latinov6_17 latinov6_18
do
    echo "python nanotnp.py -d ${year}"
    python nanotnp.py -d ${year}
done
