# Implementation of the plotting step of the analysis
# The plotting combines the histograms to plots which allow us to study the
# initial dataset based on observables motivated through physics.

import argparse, os
import ROOT
from pathlib import Path
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
def bookHistogram( df , variable , range_ , lumi="1." ):
    ##.Filter("probe_Ele_pt > 35 && abs(probe_Ele_eta) < 2.17","high pt low eta probe ele")\
    #match="tag_PromptGenLepMatch*probe_PromptGenLepMatch"
    #passingtagEleTightHWW==1
    match="mcTrue" #*tag_TightHWW_SF*probe_TightHWW_SF"
    #probe="probe_Ele_eta > 0 && probe_Ele_eta < 0.8 && probe_Ele_pt > 50 && probe_Ele_pt < 100"
    probe="1==1"
    #flag="passingprobeEleTightHWW==1"
    flag="1==1"
    # what is plotweight
    WEIGHT = match + "*" + lumi
    print( "WEIGHT : ", WEIGHT )
    return df.Define( "plotweights" , WEIGHT )\
             .Filter( "Tag_pt > 32 && abs(Tag_eta) < 2.17 && Tag_charge*Probe_charge < 0" , "Nominal cut" )\
             .Filter( flag , "passing flag" )\
             .Filter( probe , "probe low eta high pt cut" )\
             .Histo1D(ROOT.ROOT.RDF.TH1DModel(variable, variable, range_[0], range_[1], range_[2]), variable, "plotweights")
pass


# main function of the plotting step
#
# The major part of the code below is dedicated to define a nice-looking layout.
# The interesting part is the combination of the histograms to the QCD estimation.
# There, we take the data histogram from the control region and subtract all known
# processes defined in simulation and define the remaining part as QCD. Then,
# this shape is extrapolated into the signal region with a scale factor.

def histo1D(path, output, samplename, variable, xlabel, scale, ratio=0, logy=False):
    tfile = ROOT.TFile(path, "READ")

    hist={}
    
    ########################################################################
    # Simulation
    #  DY
    hist['DY'] = getHistogram(tfile, samplename, variable)
    
    # clone bkgsum template
    hist['BkgSum'] = hist['DY'].Clone("BkgSum")
    
    # Data
    dataIn= "SingleElectron" if "18" not in path else "EGamma"
    hist['DATA'] = getHistogram(tfile , dataIn , variable)

    ######################################################################3
    # Plotting style
    #hist['BkgSum'].Reset("MICES")
    hist['BkgSum'].SetFillStyle(3003)
    hist['BkgSum'].SetFillColor(1)
    hist['BkgSum'].SetMarkerStyle(0)

    # simulation
    hist['DY'].SetFillColor(418)
    hist['DY'].SetFillStyle(1001)
    hist['DY'].SetLineColor(418)
    hist['DY'].SetLineStyle(1)
    hist['DY'].SetLineWidth(2)

    # data
    hist['DATA'].SetMarkerStyle(20)
    hist['DATA'].SetMarkerSize(1.25)
    hist['DATA'].SetFillColor(418)
    hist['DATA'].SetFillStyle(1001)
    hist['DATA'].SetLineColor(1)
    hist['DATA'].SetLineStyle(1)
    hist['DATA'].SetLineWidth(2)

    #############################################################################

    for i, s in enumerate(hist):
        addOverflow(hist[s], False) # Add overflow

    # stack
    bkg = THStack('bkg', ";"+xlabel+";"+hist['BkgSum'].GetYaxis().GetTitle())
    # ADD ALL BKG
    for proc in [ 'DY' ]: bkg.Add(hist[proc])

    #Legend
    n=len(hist)
    leg = TLegend(0.7, 0.9-0.05*n, 0.95, 0.9)
    leg.SetBorderSize(0)
    leg.SetFillStyle(0) #1001
    leg.SetFillColor(0)
    leg.SetTextSize(0.03)
    leg.AddEntry(hist['DATA'], 'Data', "pl")
    leg.AddEntry(hist['DY'], samplename, "f")
    c1 = TCanvas("c1", list(hist.values())[-1].GetXaxis().GetTitle(), 800, 800 if ratio else 600 )

    #Ratio pad
    if ratio:
        c1.Divide(1, 2)
        setTopPad(c1.GetPad(1), ratio)
        setBotPad(c1.GetPad(2), ratio)

    c1.cd(1)
    c1.GetPad(bool(ratio)).SetTopMargin(0.06)
    c1.GetPad(bool(ratio)).SetRightMargin(0.05)
    c1.GetPad(bool(ratio)).SetTicks(1, 1)
    if logy:
        c1.GetPad(bool(ratio)).SetLogy()

    #Draw
    bkg.Draw("HIST") # stack
    hist['BkgSum'].Draw("SAME, E2") # sum of bkg
    hist['DATA'].Draw("SAME, PE") # data

    bkg.GetYaxis().SetTitleOffset(0.95)
    bkg.SetMaximum((6.0 if logy else 1.5)*max(bkg.GetMaximum(), hist['DATA'].GetBinContent(hist['DATA'].GetMaximumBin())+hist['DATA'].GetBinError(hist['DATA'].GetMaximumBin())))
    bkg.SetMinimum(max(min(hist['BkgSum'].GetBinContent(hist['BkgSum'].GetMinimumBin()), hist['DATA'].GetMinimum()), 5.e-1)  if logy else 0.)

    #bkg.SetMinimum(1.0)

    leg.Draw()

    setHistStyle(bkg, 1.2 if ratio else 1.1)
    setHistStyle(hist['BkgSum'], 1.2 if ratio else 1.1)

    #####################################################################
    if ratio:
        c1.cd(2)
        err = hist['BkgSum'].Clone("BkgErr;")
        err.SetTitle("")
        err.GetYaxis().SetTitle("Data / MC")
        err.GetXaxis().SetTitle(xlabel)

        for i in range(1, err.GetNbinsX()+1):
            err.SetBinContent(i, 1)
            if hist['BkgSum'].GetBinContent(i) > 0:
                err.SetBinError(i, hist['BkgSum'].GetBinError(i)/hist['BkgSum'].GetBinContent(i))
        setBotStyle(err)
        errLine = err.Clone("errLine")
        errLine.SetLineWidth(1)
        errLine.SetFillStyle(0)
        errLine.SetLineColor(1)
        err.Draw("E2")

        errLine.Draw("SAME, HIST")

        if 'DATA' in hist:
            res = hist['DATA'].Clone("Residues")
            res.SetTitle("")
            res.GetYaxis().SetTitle("Data / MC")
            res.GetXaxis().SetTitle(xlabel)
            for i in range(0, res.GetNbinsX()+1):
                if hist['BkgSum'].GetBinContent(i) > 0:
                    res.SetBinContent(i, res.GetBinContent(i)/hist['BkgSum'].GetBinContent(i))
                    res.SetBinError(i, res.GetBinError(i)/hist['BkgSum'].GetBinContent(i))
            setBotStyle(res)
            res.Draw("SAME, PE0")
            if len(err.GetXaxis().GetBinLabel(1))==0: # Bin labels: not a ordinary plot
                drawRatio(hist['DATA'], hist['BkgSum'])
                drawKolmogorov(hist['DATA'], hist['BkgSum'])
        else: res = None
            
    gPad.Modified()
    gPad.Update()
    c1.cd(1)
    drawCMS("%s" %scale, "Object Study")
    
    c1.Update()

    if not os.path.isdir(output):
        os.system('mkdir -p %s' % output )
    
    c1.SaveAs("{}/{}.pdf".format(output, variable))
    c1.SaveAs("{}/{}.png".format(output, variable))
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

    #tfile.Close()

    # plot!
    for imc in Mc_:
        for variable in variables:
        print("variable : ", variable)
        ###
        histo1D( "./results/histo_%s.root" %Name_ , pth ,
                 imc ,
                 variable ,
                 ranges[variable][1] ,
                 lm ,
                 4 ,
                 False if 'eta' in variable else True
        )                            

    '''
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


    # convert result folder in skim to histograms
    for ifold in os.listdir("%s/validation/kinematics/results/" %( os.environ['BASETNP'] ) ):
        lm = dataset_config[ifold]['lumi']
        pth = "%s/validation/kinematics/results/%s" %( os.environ['BASETNP'] , ifold )
        for imc in [ "DYJetsToLL_M-50_LO" , "DYJetsToLL_M-50" ] :
            for variable in labels.keys(): 
                print("variable : ", variable)
                histo1D( "%s/histogram.root" % pth , 
                         pth , 
                         imc , 
                         variable , 
                         labels[variable][1] , 
                         lm , 
                         4 , 
                         False if 'eta' in variable else True 
                     )
                
    '''
