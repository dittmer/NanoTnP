#! /usr/bin/env python

from optparse import OptionParser
import os, sys, time
cwd = os.getcwd()
usage = "usage: %prog [options]"
parser = OptionParser(usage)
parser.add_option("-d","--dataset", action="store", type="string", dest="dataset", default="latinov7_16_102X")
parser.add_option("-a","--aodFormat", action="store", type="string", dest="aodFormat", default="nanov7")
parser.add_option("-o","--output", action="store", type="string", dest="output", default="%s/results/" %(cwd))
parser.add_option("-t","--test", action="store_true", dest="test", default=False)

(options, args) = parser.parse_args()

datasets = options.dataset
aodFormat = options.aodFormat
test = options.test
dirs = "%s/data/filelists/%s/%s/" % ( cwd , aodFormat , datasets )
output = "%s/%s/%s" %( options.output , aodFormat , datasets )
samplelists=[]
lumi=""

# DY LO, DY NLO and SingleElectron datasets
txtfiles={
    'latinov7_16_102X' : {
        "lumi" : "35.867",
        "txt"  : [ "DYJetsToLL_M-50-LO_ext1.txt" , "DYJetsToLL_M-50_ext2.txt" , "SingleElectron.txt"]
    },
    # absolutely necessary to make dataset into period dependent for next step processing
    'latinov7_17_102X' : {
	"lumi" : "41.35",
        "txt"  : [
            "DYJetsToLL_M-50-LO_ext1.txt" ,
            "DYJetsToLL_M-50_ext1.txt",
            "SingleElectron_RunB.txt",
            "SingleElectron_RunC.txt",
            "SingleElectron_RunD.txt",
            "SingleElectron_RunE.txt",
            "SingleElectron_RunF.txt"
        ]
    },
    'latinov7_18_102X' : {
	"lumi" : "59.74",
        "txt"  : [
            "DYJetsToLL_M-50-LO.txt",
            "DYJetsToLL_M-50_ext2.txt",
            "EGamma.txt"
        ]
    }
}

tcompile = time.time()
os.system("make clean")
os.system("make")
print("--- compilation took : %.3f seconds (%.3f minutes) ---" % ( (time.time() - tcompile) , (time.time() - tcompile)/60. ) )

# predefined samples
if datasets not in txtfiles:
    print(' >>> ERROR: Pick a datasets <<<')
    os.system('ls data/filelists/nanov7/')
    print(' exp: python tnp.py -d Full2018v7_102X')
    sys.exit(0)
else:
    samplelists = [ "%s/%s" %( dirs if not test else dirs.replace('filelists','local') ,itxt) for itxt in  txtfiles[datasets]['txt'] ]

if not os.path.exists(output):
    os.system("mkdir -p %s" %output)

trun = time.time();
for iproc in samplelists:
    sample = iproc.split('/')[-1].split('.txt')[0]
    # process-in ; process-out ; sample ; lumi
    cmd="./skim"; cmd+=" %s %s/%s.root %s" %( iproc , output , sample , txtfiles[datasets]['lumi'] )
    tproc = time.time()
    print(cmd)
    os.system(cmd)
    print("--- running on %s took : %.3f seconds (%.3f minutes) ---" % ( sample , (time.time() - tproc) , (time.time() - tproc)/60. ) )
print("--- Total run time : %.3f seconds (%.3f minutes) ---" % ( (time.time() - trun) , (time.time() - trun)/60. ) )
