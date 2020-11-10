#! /usr/bin/env python

from optparse import OptionParser
import os, sys, time
cwd = os.getcwd()
usage = "usage: %prog [options]"
parser = OptionParser(usage)
parser.add_option("-d","--dataset", action="store", type="string", dest="dataset", default="Full2016v7_102X")
parser.add_option("-l","--location", action="store", type="string", dest="location", default="eos")
parser.add_option("-o","--output", action="store", type="string", dest="output", default="%s/results/" %(cwd))

(options, args) = parser.parse_args()

datasets = options.dataset
locations = options.location
dirs = "%s/data/filelists/%s/%s/" % (cwd,locations,datasets)
output = options.output + "%s" %(datasets)
samplelists=[]
lumi=""

tcompile = time.time()
os.system("make")
print("--- compilation took : %.3f seconds (%.3f minutes) ---" % ( (time.time() - tcompile) , (time.time() - tcompile)/60. ) )

# predefined samples
if datasets == 'Full2016v7_102X':
    lumi = "35.867"
    for itxt in [
        "DYJetsToLL_M-50-LO_ext1.txt" , # DY LO
        "DYJetsToLL_M-50_ext2.txt" ,    # alt DY NLO
        "SingleElectron.txt"            # single electron
        ]:
        samplelists.append( "%s/%s" %(dirs,itxt) )

elif datasets == 'Full2017v7_102X':
    lumi="41.53"
    for itxt in [
        "DYJetsToLL_M-50-LO_ext1.txt" ,
        "DYJetsToLL_M-50_ext1.txt",
        "SingleElectron_RunB.txt",
        "SingleElectron_RunC.txt",
        "SingleElectron_RunD.txt",
        "SingleElectron_RunE.txt",
        "SingleElectron_RunF.txt"
        ]:
        samplelists.append( "%s/%s" %(dirs,itxt) )

elif datasets == 'Full2018v7_102X':
    lumi = "59.74"
    for itxt in [
        "DYJetsToLL_M-50-LO.txt",
        "DYJetsToLL_M-50_ext2.txt",
        "EGamma.txt"
        ]:
        samplelists.append( "%s/%s" %(dirs,itxt) )

else:
    print(' >>> ERROR: Dude... really? Pick one datasets here <<<')
    os.system('ls data')
    print(' exp: python nanotnp.py -y Full2018v7_102X')
    sys.exit(0)

if not os.path.exists(output):
    os.system("mkdir -p %s" %output)

print("year : %s" %(datasets))
print("lumi : %s 1/fb" %(lumi))

#if len(samplelists)!=3: print("ERROR: len(samplelists)!=3"); sys.exit(0);

trun = time.time();
for iproc in samplelists:
    sample = iproc.split('/')[-1].split('.txt')[0]
    # process-in ; process-out ; sample ; lumi
    cmd="./tnptreeSkim"; cmd+=" %s %s/%s.root %s" %(iproc,output,sample,lumi)
    tproc = time.time()
    print(cmd)
    os.system(cmd)
    print("--- running on %s took : %.3f seconds (%.3f minutes) ---" % ( sample , (time.time() - tproc) , (time.time() - tproc)/60. ) )
print("--- Total run time : %.3f seconds (%.3f minutes) ---" % ( (time.time() - trun) , (time.time() - trun)/60. ) )
