# Implementing histogramming

import argparse, os
import ROOT
import numpy as np
from ROOT import array
ROOT.gROOT.SetBatch(True)

from skim.skim import dataset_config

ROOT.ROOT.EnableImplicitMT(10)

# Declare range of the histogram for each variables
# Each entry in the dictionary contains of the variable name as key and a tuple
# specifying the histogram layout as value. The tuple sets the number of bins,
# the lower edge and the upper edge of the histogram.

#labels = {
#    "tag_Ele_pt"     : "Tagged Electron p_{T} [GeV/c^2]",
#    "probe_Ele_pt"   : "Probed Electron p_{T} [GeV/c^2]",
#    "tag_Ele_eta"    : "Tagged Electron #eta",
#    "probe_Ele_eta"  : "Probed Electron #eta",
#    "pair_pt"        : "pair p_{T} [GeV/c^2]",
#    "pair_eta"       : "pair #eta",
#    "pair_mass"      : "Mass (ll) [GeV/c]",
#    "passingtagEleTightHWW" : "Tagged Electron TightHWW",
#    "passingprobeEleTightHWW" : "Probe Electron TightHWW",
#    "passingprobeElettHMVA" : "Probe Electron ttHMVA_0p7",
#    "passingprobeTightHWW_ttHMVA_0p7" : "Probe Electron TightHWW_ttHMVA_0p7",
#    }



ranges = {
    "Tag_pt"     : [ ( 50 , 0.   , 500 ) , "Tagged Electron p_{T} [GeV/c^2]" ],
    "Probe_pt"   : [ ( 50 , 0.   , 500 ) , "Probed Electron #eta" ] ,
    "Tag_eta"    : [ ( 120 , -3.0 , 3.0 ) , "Tagged Electron #eta" ] ,
    "Probe_eta"  : [ ( 120 , -3.0 , 3.0 ) , "Probed Electron #eta" ] ,
    #"pair_pt"        : [ ( 50 , 0.   , 500 ) , "pair p_{T} [GeV/c^2]" ] ,
    #"pair_eta"       : [ ( 40 , -10.0 , 10.0 ) , "pair #eta" ] ,
    "pair_mass"      : [ ( 80 , 50   , 130 ) , "Mass (ll) [GeV/c]" ] ,
    #"passingtagEleTightHWW" : ( 2 , -0.5 , 0.5 ),
    #"passingprobeEleTightHWW" : ( 2 , -0.5 , 0.5 ),
    #"passingprobeElettHMVA" : ( 2 , -0.5 , 1.5 ),
    #"passingprobeTightHWW_ttHMVA_0p7" : ( 2 , -0.5 , 1.5 ),
    }

# Book a histogram for a specific variable
def bookHistogram(df, variable, range_, ismc):
    ##.Filter("probe_Ele_pt > 35 && abs(probe_Ele_eta) < 2.17","high pt low eta probe ele")\
    #match="tag_PromptGenLepMatch*probe_PromptGenLepMatch"
    #passingtagEleTightHWW==1
    match="mcTrue" #*tag_TightHWW_SF*probe_TightHWW_SF"
    #probe="probe_Ele_eta > 0 && probe_Ele_eta < 0.8 && probe_Ele_pt > 50 && probe_Ele_pt < 100"
    probe="1==1"
    #flag="passingprobeEleTightHWW==1"
    flag="1==1"
    # what is plotweight
    WEIGHT = match if ismc else "1==1"
    print( "WEIGHT : ", WEIGHT )
    return df.Define( "plotweights" , WEIGHT )\
             .Filter( "Tag_pt > 32 && abs(Tag_eta) < 2.17 && Tag_charge*Probe_charge < 0" , "Nominal cut" )\
             .Filter( flag , "passing flag" )\
             .Filter( probe , "probe low eta high pt cut" )\
             .Histo1D(ROOT.ROOT.RDF.TH1DModel(variable, variable, range_[0], range_[1], range_[2]), variable, "plotweights")
pass

# Write a histogram with a given name to the output ROOT file
def writeHistogram(h, name, range_):
    h.GetYaxis().SetTitle("Events / %s GeV" % ( float( (range_[2]-range_[1]) / range_[0] ) ) )
    h.SetName(name)
    h.Write()
pass

# main function will loop over the outputs from the skimming step and produces
# required histograms for the final plotting.
# perform selection on same sign and opposite sign only
def main(sample, process):
    output="./results/%s" % sample.split('skim/results/')[-1].split('/')[0]

    # Create output file
    if not os.path.isdir(output): os.system('mkdir -p %s' % output)

    tfile = ROOT.TFile( output + "/" + process + "_hist.root" , "RECREATE" )
    variables = ranges.keys()
    hists={}

    # Process skimmed datasets and produce histograms of variables
    print(">>> Process skimmed sample {} for process {}".format(sample, process))

    # Load skimmed dataset and apply baseline selection (if any)
    df = ROOT.ROOT.RDataFrame("fitter_tree", sample)
    
    # Book histogram
    for variable in variables: hists[variable] = bookHistogram(df, variable, ranges[variable][0], False if 'Run' in process else True )

    # Write histograms to output file
    for variable in variables: writeHistogram(hists[variable], "{}_{}".format(process,variable), ranges[variable][0])

    tfile.Close()
    
    
pass

if __name__ == "__main__":
    #parser = argparse.ArgumentParser()
    #parser.add_argument("sample", type=str, help="Full path to skimmed sample")
    #parser.add_argument("process", type=str, help="Process name")
    #args = parser.parse_args()
    
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
