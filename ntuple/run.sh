#!/bin/bash

for year in latinov5_16 latinov5_17 latinov5_18
do
    echo "python nanotnp.py -d ${year} -l local"
    python nanotnp.py -d ${year} -l local
done
