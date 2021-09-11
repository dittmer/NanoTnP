# Implementation of the plotting step of the analysis
# The plotting combines the histograms to plots which allow us to study the
# initial dataset based on observables motivated through physics.

import argparse, os, sys
import ROOT
import numpy as np
from ROOT import array
from utilities import *

ROOT.gROOT.SetBatch(True)
ROOT.ROOT.EnableImplicitMT(10)
ROOT.TH1.SetDefaultSumw2()
ROOT.gStyle.SetOptStat(0)

# Declare human readable label for each variable
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

# Declare range of the histogram for each variables
# Each entry in the dictionary contains of the variable name as key and a tuple
# specifying the histogram layout as value. The tuple sets the number of bins,
# the lower edge and the upper edge of the histogram.

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

### RDataframe
# Book a histogram for a specific variable
def bookHistogram( df , variable , range_ , lumi=None ):
    ##.Filter("probe_Ele_pt > 35 && abs(probe_Ele_eta) < 2.17","high pt low eta probe ele")\
    #match="tag_PromptGenLepMatch*probe_PromptGenLepMatch"
    #passingtagEleTightHWW==1
    match="mcTrue*weight" #*tag_TightHWW_SF*probe_TightHWW_SF"
    #probe="probe_Ele_eta > 0 && probe_Ele_eta < 0.8 && probe_Ele_pt > 50 && probe_Ele_pt < 100"
    probe="1==1"
    #flag="passingprobeEleTightHWW==1"
    flag="1==1"
    # what is plotweight
    WEIGHT = match + "*" + lumi if lumi is not None else "1."
    print( "WEIGHT : ", WEIGHT )
    return df.Define( "weights" , WEIGHT )\
             .Filter( "Tag_pt > 32 && abs(Tag_eta) < 2.17 && Tag_charge*Probe_charge < 0" , "Nominal cut" )\
             .Filter( flag , "passing flag" )\
             .Filter( probe , "probe low eta high pt cut" )\
             .Histo1D( ROOT.ROOT.RDF.TH1DModel(variable, variable, range_[0], range_[1], range_[2]), variable, "weights" )
pass

# Loop over all variable names and make a plot for each
if __name__ == "__main__":

    parser = argparse.ArgumentParser()
    parser.add_argument( '-n' , '--name' , type=str , help='DATA Name' )
    parser.add_argument( '-l' , '--lumi' , type=str , help='total luminosity' )
    parser.add_argument( '-r' , '--rootfiles' , type=str , nargs='+', help='path/root file', required=True )

    args = parser.parse_args()
    Name_ = args.name
    Lumi_ = args.lumi
    file_ = args.rootfiles
    Mc_ = list(filter( lambda x : 'DY' in x.split('/')[-1] , file_ ))
    Data_ = list( set(file_) - set(Mc_) )

    print( 'NAME : ', Name_ )
    print( 'Lumi : ', Lumi_ )
    print( 'DATA : ', Data_ )
    print( 'MC   : ', Mc_   )

    # apply selection, book histogram
    outpath = './results/%s' %Name_
    if not os.path.isdir(outpath): os.system( 'mkdir -p %s' %outpath )
    # root file consist of variables for each sample
    tfile = ROOT.TFile( "%s/histo_%s.root" %( outpath , Name_ ) , "RECREATE" )

    variables = ranges.keys()
    hists={}
    
    # Process skimmed datasets and produce histograms of variables
    # Process MC
    for imc in Mc_:
        mcName = imc.split('/')[-1].split('.root')[0]
        print(" --> MC samples : %s" %( mcName ) )

        # Load skimmed dataset and apply baseline selection (if any)
        df = ROOT.ROOT.RDataFrame( 'fitter_tree' , imc )
        # Book histogram
        for variable in variables: hists[variable] = bookHistogram( df , variable , ranges[variable][0] , Lumi_ )

        # Write histograms to output file
        for variable in variables: hists[variable].SetName( "{}_{}".format( mcName , variable ) ); hists[variable].Write()

    hists.clear()

    print(" --> DATA : %s" %Name_ )
    # Process Data
    ddf = ROOT.ROOT.RDataFrame( 'fitter_tree' , Data_ )
    # Book histogram
    for variable in variables: hists[variable] = bookHistogram( ddf , variable , ranges[variable][0] )
    # Write histograms to output file
    for variable in variables: hists[variable].SetName( "{}_{}".format( Name_ , variable ) ); hists[variable].Write()
    
    #tfile = ROOT.TFile( "./results/UL2018_ABCD/histo_UL2018_ABCD.root" , "READ" )
    
    # plot!
    for imc in Mc_:
        mcName = imc.split('/')[-1].split('.root')[0]
        out_ = '%s/%s' %( outpath , mcName )
        if not os.path.isdir(out_): os.system( 'mkdir -p %s' %out_ )
        print( "using mc sample : " , imc )
        for variable in variables:
            print( "variable : " , variable )
            hist_mc = getHistogram( tfile , "{}_{}".format( mcName , variable ) )
            hist_data = getHistogram( tfile , "{}_{}".format( Name_ , variable ) )
            histo1D( hist_data , hist_mc , out_ , variable , ranges[variable][1] , Lumi_ , 4 , False if 'eta' in variable else True )

    tfile.Close()
