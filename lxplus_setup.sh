#!/bin/bash

export BASETNP=$PWD
export PYTHONPATH=$PYTHONPATH:$PWD
# set your environment
# ROOT version
# LCG (LHC Computing Grid); python; gcc version

#source /cvmfs/sft.cern.ch/lcg/views/LCG_95apython3/x86_64-centos7-gcc7-opt/setup.sh

source /cvmfs/sft.cern.ch/lcg/views/setupViews.sh LCG_98python3 x86_64-centos7-gcc8-opt
#source /cvmfs/sft.cern.ch/lcg/views/setupViews.sh LCG_95apython3 x86_64-centos7-gcc7-opt

# https://root.cern/releases/release-62400/#installations-in-cvmfs
#source /cvmfs/sft.cern.ch/lcg/app/releases/ROOT/6.24.00/x86_64-centos7-gcc48-opt/bin/thisroot.sh
