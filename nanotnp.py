#! /usr/bin/env python

from optparse import OptionParser
import os, sys, time
cwd = os.getcwd()
usage = "usage: %prog [options]"
parser = OptionParser(usage)
parser.add_option("-y","--year", action="store", type="string", dest="year", default="latinov5_17")
parser.add_option("-o","--output", action="store", type="string", dest="output", default="%s/results/" %(cwd))

(options, args) = parser.parse_args()

datasets = options.year
output = options.output + "%s" %(datasets)

tcompile = time.time()
os.system("make")
print("--- compilation took : %.3f seconds (%.3f minutes) ---" % ( (time.time() - tcompile) , (time.time() - tcompile)/60. ) )

samplelists=[]
lumi=""

# predefined samples
if datasets == 'latinov5_16':
    lumi = "35.867"
    # DY LO
    samplelists.append("%s/data/%s/DYJetsToLL_M-50-LO.txt" %(cwd,datasets) )
    # alt DY NLO
    samplelists.append("%s/data/%s/DYJetsToLL_M-50_ext1.txt" %(cwd,datasets) )
    # single electrons
    samplelists.append("%s/data/%s/SingleElectron_Run2017.txt" %(cwd,datasets) )

elif datasets == 'latinov5_17':
    lumi="41.53"
    # DY LO
    samplelists.append("%s/data/%s/DYJetsToLL_M-50-LO.txt" %(cwd,datasets) )
    # alt DY NLO
    #samplelists.append("%s/data/%s/DYJetsToLL_M-50_ext1.txt" %(cwd,datasets) )
    # single electrons
    #samplelists.append("%s/data/%s/SingleElectron_Run2017.txt" %(cwd,datasets) )

elif datasets == 'latinov5_18':
    lumi = "59.74"
    # DY LO
    samplelists.append("%s/data/%s/DYJetsToLL_M-50-LO.txt" %(cwd,datasets) )
    # alt DY NLO
    samplelists.append("%s/data/%s/DYJetsToLL_M-50_ext1.txt" %(cwd,datasets) )
    # single electrons
    samplelists.append("%s/data/%s/SingleElectron_Run2017.txt" %(cwd,datasets) )

else:
    print(' >>> ERROR: Dude... really? Pick one datasets here <<<')
    os.system('ls data')
    print(' exp: python nanotnp.py -y latinov5_17')
    sys.exit(0)

if not os.path.exists(output):
    os.system("mkdir -p %s" %output)

print("year : %s" %(datasets))
print("lumi : %s 1/fb" %(lumi))

#if len(samplelists)!=3: print("ERROR: len(samplelists)!=3"); sys.exit(0);

trun = time.time();
for iproc in samplelists:
    sample = iproc.split('/')[-1].split('.txt')[0]
    cmd="./nanotnp"; cmd+=" %s %s/%s.root %s" %(iproc,output,sample,lumi)
    tproc = time.time()
    print(cmd)
    os.system(cmd)
    print("--- running on %s took : %.3f seconds (%.3f minutes) ---" % ( sample , (time.time() - tproc) , (time.time() - tproc)/60. ) )
print("--- Total run time : %.3f seconds (%.3f minutes) ---" % ( (time.time() - trun) , (time.time() - trun)/60. ) )
