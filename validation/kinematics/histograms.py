# Implementing histogramming

import argparse, os
import ROOT
import numpy as np
from ROOT import array
ROOT.gROOT.SetBatch(True)

from skim import dataset_config

ROOT.ROOT.EnableImplicitMT(10)



if __name__ == "__main__":
    #parser = argparse.ArgumentParser()
    #parser.add_argument("sample", type=str, help="Full path to skimmed sample")
    #parser.add_argument("process", type=str, help="Process name")
    #args = parser.parse_args()

    # create histogram
    
    print("converting ntuple to histogram")
    for ifold in os.listdir("%s/skim/results/" %( os.environ['BASETNP'] ) ):
        pth = "%s/skim/results/%s/merged" %( os.environ['BASETNP'] , ifold ) if os.path.exists( "%s/skim/results/%s/merged" %( os.environ['BASETNP'] , ifold ) ) else "%s/skim/results/%s/" %( os.environ['BASETNP'] , ifold )
        for iroot in os.listdir(pth):
            if 'Run' in iroot : continue
            print(iroot)
            print(" --> samples : %s/%s" %( pth , iroot ) )
            print(" --> process : %s" % iroot.strip('.root') )
            main( "%s/%s" %( pth , iroot ) , iroot.strip('.root') )

    from validation.kinematics.plot import histo1D

    # convert result folder in skim to histograms
    for ifold in os.listdir("%s/validation/kinematics/results/" %( os.environ['BASETNP'] ) ):
        
        lm = dataset_config[ifold]['lumi']
        pth = "%s/validation/kinematics/results/%s" %( os.environ['BASETNP'] , ifold )
        if os.path.isfile( "%s/histogram.root" % pth ) : os.system("rm %s/histogram.root" %pth )
        os.system("hadd -f %s/histogram.root %s/*.root" % ( pth , pth ) )
        for imc in [ "DYJetsToLL_M-50_LO" , "DYJetsToLL_M-50" ] :
            for variable in ranges.keys():
                print("variable : ", variable)
                histo1D( "%s/histogram.root" % pth ,
                         pth ,
                         imc ,
                         variable ,
                         ranges[variable][1] ,
                         lm ,
                         4 ,
                         False if 'eta' in variable else True
                     )
