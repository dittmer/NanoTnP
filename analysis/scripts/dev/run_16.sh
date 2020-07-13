#!/bin/bash

set -e

#WP="passingMVA94Xwp90isoHWWiso0p06"
#WP="passingMVA80Xwp90Iso16" # 2016
#WP="passingttHMVA0p7"
#WP="passingMVA80Xwp90Iso16"
#WP="passingMVA80Xwp90Iso16SS"
WP="passingttHMVA0p7SS"

for config in settings_nanov7_16.py #settings_nanov7_17.py settings_nanov7_18.py
do
    name=`echo $config | awk -F '.' '{print $1}'`
    echo "Running on $name"
    mkdir -p jobs/$name
    cat<<EOF > jobs/$name/$name.sh
#!/bin/bash

python ${PWD}/tnpEGM_fitter.py settings/${config} --flag ${WP} --checkBins
python ${PWD}/tnpEGM_fitter.py settings/${config} --flag ${WP} --createBins
python ${PWD}/tnpEGM_fitter.py settings/${config} --flag ${WP} --createHist
python ${PWD}/tnpEGM_fitter.py settings/${config} --flag ${WP} --doFit
python ${PWD}/tnpEGM_fitter.py settings/${config} --flag ${WP} --doFit --mcSig --altSig
python ${PWD}/tnpEGM_fitter.py settings/${config} --flag ${WP} --doFit --altSig
python ${PWD}/tnpEGM_fitter.py settings/${config} --flag ${WP} --doFit --altBkg
python ${PWD}/tnpEGM_fitter.py settings/${config} --flag ${WP} --sumUp

EOF

chmod +x jobs/$name/$name.sh

if [ -z $@ ];then
    echo "dry-runing"
    ./jobs/$name/$name.sh
else
    echo "submitting job"
    cat<<EOF > jobs/$name/$name.sub
executable = ${PWD}/jobs/${name}/${name}.sh
universe = vanilla
input = settings/${config}
output = ${PWD}/jobs/${name}/${name}.out
error = ${PWD}/jobs/${name}/${name}.err
log = ${PWD}/jobs/${name}/${name}.log
request_cpus = 1
+JobFlavour = "longlunch"
queue
EOF
    voms-proxy-init -voms cms -valid 168:00
    condor_submit jobs/$name/$name.sub
fi
    
done
