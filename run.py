#! /usr/bin/env python

from optparse import OptionParser
import os, sys, time
cwd = os.getcwd()
usage = "usage: %prog [options]"
parser = OptionParser(usage)
parser.add_option("-y","--year", action="store", type="string", dest="year", default="2016")
parser.add_option("-o","--output", action="store", type="string", dest="output", default="%s/../skim/" %(cwd))

(options, args) = parser.parse_args()

datasets = options.year
output = options.output + "%s" %(datasets)

if not os.path.exists(output):
    os.system("mkdir -p %s" %output)

tcompile = time.time()
os.system("make")
print("--- compilation took : %.3f seconds (%.3f minutes) ---" % ( (time.time() - tcompile) , (time.time() - tcompile)/60. ) )

samplelists=[]
lumi=""

# predefined samples
if datasets == '2017':
    lumi="41.53"
    # DY LO
    samplelists.append("%s/samplelists/latinov5_17/DYJetsToLL_M-50-LO.txt" %cwd)
    # alt DY NLO
    samplelists.append("%s/samplelists/latinov5_17/DYJetsToLL_M-50_ext1.txt" %cwd)
    # single electrons
    samplelists.append("%s/samplelists/latinov5_17/SingleElectron_Run2017.txt" %cwd)

elif datasets == '2018':
    lumi = "59.74"
    # DY LO
    samplelists.append("%s/samplelists/latinov5_17/DYJetsToLL_M-50-LO.txt" %cwd)
    # alt DY NLO
    samplelists.append("%s/samplelists/latinov5_17/DYJetsToLL_M-50_ext1.txt" %cwd)
    # single electrons
    samplelists.append("%s/samplelists/latinov5_17/SingleElectron_Run2017.txt" %cwd)

else:
    lumi = "35.867"
    # DY LO
    samplelists.append("%s/samplelists/latinov5_17/DYJetsToLL_M-50-LO.txt" %cwd)
    # alt DY NLO
    samplelists.append("%s/samplelists/latinov5_17/DYJetsToLL_M-50_ext1.txt" %cwd)
    # single electrons
    samplelists.append("%s/samplelists/latinov5_17/SingleElectron_Run2017.txt" %cwd)


print("year : %s" %(datasets))
print("lumi : %s 1/fb" %(lumi))

if len(samplelists)!=3: print("ERROR: len(samplelists)!=3"); sys.exit(0);

trun = time.time();
for iproc in samplelists:
    sample = iproc.split('/')[-1].split('.txt')[0]
    cmd="./tnpper"; cmd+=" %s %s/%s.root %s" %(iproc,output,sample,lumi)
    tproc = time.time()
    print(cmd)
    os.system(cmd)
    sys.exit()
    print("--- running on %s took : %.3f seconds (%.3f minutes) ---" % ( sample , (time.time() - tproc) , (time.time() - tproc)/60. ) )
print("--- Total run time : %.3f seconds (%.3f minutes) ---" % ( (time.time() - trun) , (time.time() - trun)/60. ) )
