# Implementation of the plotting step of the analysis
# The plotting combines the histograms to plots which allow us to study the
# initial dataset based on observables motivated through physics.

import argparse
import ROOT
from pathlib import Path
import os
from helper import *

ROOT.gROOT.SetBatch(True)
ROOT.TH1.SetDefaultSumw2()
ROOT.gStyle.SetOptStat(0)

# Declare human readable label for each variable
labels = {
    "tag_Ele_pt"     : "Tagged Electron p_{T} [GeV/c^2]",
    "probe_Ele_pt"   : "Probed Electron p_{T} [GeV/c^2]",
    "tag_Ele_eta"    : "Tagged Electron #eta",
    "probe_Ele_eta"  : "Probed Electron #eta",
    "pair_pt"        : "pair p_{T} [GeV/c^2]",
    "pair_eta"       : "pair #eta",
    "pair_mass"      : "Mass (ll) [GeV/c]",
    }

# Retrieve a histogram from the input file based on the process and the variable
# name
def getHistogram(tfile, name, variable):
    name = "{}_{}".format(name, variable)
    h = tfile.Get(name)
    if not h:
        raise Exception("Failed to load histogram {}.".format(name))
    return h

# Write a histogram with a given name to the output ROOT file
def writeHistogram(h, name):
    h.SetName(name)
    h.Write()
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
    hist['DATA'] = getHistogram(tfile, "EGamma_Run2018" if '_18' in path else "SingleElectron_Run2017" , variable)

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
    parser.add_argument("path", type=str, help="Full path to ROOT file with all histograms")
    parser.add_argument("output", type=str, help="Output directory for plots")
    parser.add_argument("sample", type=str, help="Monte Carlo sample for comparison with data")
    #parser.add_argument("scale", type=float, help="Scaling of the integrated luminosity")
    args = parser.parse_args()
    
    for variable in labels.keys(): histo1D( args.path , args.output , args.sample , variable , labels[variable] , "41.53" if '_17' in args.path else "59.74" , 4 , False if 'eta' in variable else True )
