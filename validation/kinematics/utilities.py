from array import array
import math
from ROOT import gPad, TCanvas, TH2D, TH1D, TFile, THStack, TLegend, gSystem, TLatex, TH1, gStyle, TGaxis
import os, sys

#TGaxis.SetMaxDigits(2)
TH1.SetDefaultSumw2()
gStyle.SetPaintTextFormat(".5f")

def setBotStyle(h, r=4, fixRange=True):
    h.GetXaxis().SetLabelSize(h.GetXaxis().GetLabelSize()*(r-1));
    h.GetXaxis().SetLabelOffset(h.GetXaxis().GetLabelOffset()*(r-1));
    h.GetXaxis().SetTitleSize(h.GetXaxis().GetTitleSize()*(r-1));
    h.GetYaxis().SetLabelSize(h.GetYaxis().GetLabelSize()*(r-1));
    h.GetYaxis().SetNdivisions(505);
    h.GetYaxis().SetTitleSize(h.GetYaxis().GetTitleSize()*(r-1));
    h.GetYaxis().SetTitleOffset(h.GetYaxis().GetTitleOffset()/(r-1));
    if fixRange:
        h.GetYaxis().SetRangeUser(0., 2.)
        for i in range(1, h.GetNbinsX()+1):
            if h.GetBinContent(i)<1.e-6:
                h.SetBinContent(i, -1.e-6)
pass

def drawCMS(lumi, text, onTop=False):
    latex = TLatex()
    latex.SetNDC()
    latex.SetTextSize(0.04)
    latex.SetTextColor(1)
    latex.SetTextFont(42)
    latex.SetTextAlign(33)
    if (type(lumi) is float or type(lumi) is int) and float(lumi) > 0: latex.DrawLatex(0.95, 0.985, "%.1f fb^{-1}  (13 TeV)" % (float(lumi)/1000.))
    elif type(lumi) is str: latex.DrawLatex(0.95, 0.985, "%s fb^{-1}  (13 TeV)" % lumi)
    if not onTop: latex.SetTextAlign(11)
    latex.SetTextFont(62)
    latex.SetTextSize(0.05 if len(text)>0 else 0.06)
    if not onTop: latex.DrawLatex(0.15, 0.87 if len(text)>0 else 0.84, "CMS")
    else: latex.DrawLatex(0.20, 0.99, "CMS")
    latex.SetTextSize(0.04)
    latex.SetTextFont(52)
    if not onTop: latex.DrawLatex(0.15, 0.83, text)
    else: latex.DrawLatex(0.40, 0.98, text)
pass

def setHistStyle(hist, r=1.1):
    hist.GetXaxis().SetTitleSize(hist.GetXaxis().GetTitleSize()*r*r)
    hist.GetYaxis().SetTitleSize(hist.GetYaxis().GetTitleSize()*r*r)
    hist.GetXaxis().SetLabelSize(hist.GetXaxis().GetLabelSize()*r)
    hist.GetYaxis().SetLabelSize(hist.GetYaxis().GetLabelSize()*r)
    hist.GetXaxis().SetLabelOffset(hist.GetXaxis().GetLabelOffset()*r*r*r*r)
    hist.GetXaxis().SetTitleOffset(hist.GetXaxis().GetTitleOffset()*r)
    hist.GetYaxis().SetTitleOffset(hist.GetYaxis().GetTitleOffset())
    if hist.GetXaxis().GetTitle().find("GeV") != -1: # and not hist.GetXaxis().IsVariableBinSize()
        div = (hist.GetXaxis().GetXmax() - hist.GetXaxis().GetXmin()) / hist.GetXaxis().GetNbins()
        hist.GetYaxis().SetTitle("Events / %.1f GeV" % div)
pass

def drawRatio(data, bkg):
    errData = array('d', [1.0])
    errBkg = array('d', [1.0])
    intData = data.IntegralAndError(1, data.GetNbinsX(), errData)
    intBkg = bkg.IntegralAndError(1, bkg.GetNbinsX(), errBkg)
    ratio = intData / intBkg if intBkg!=0 else 0.
    error = math.hypot(errData[0]*ratio/intData,  errBkg[0]*ratio/intBkg) if intData>0 and intBkg>0 else 0
    latex = TLatex()
    latex.SetNDC()
    latex.SetTextColor(1)
    latex.SetTextFont(62)
    latex.SetTextSize(0.08)
    #latex.DrawLatex(0.25, 0.85, "Data/Bkg = %.3f #pm %.3f" % (ratio, error))
    latex.DrawLatex(0.15, 0.85, "Data/Bkg = %.3f #pm %.3f" % (ratio, error))
    print ("  Ratio:\t%.3f +- %.3f" % (ratio, error))
    return [ratio, error]
pass

def drawKolmogorov(data, bkg):
    latex = TLatex()
    latex.SetNDC()
    latex.SetTextColor(1)
    latex.SetTextFont(62)
    latex.SetTextSize(0.08)
    #latex.DrawLatex(0.45, 0.85, "#chi^{2}/ndf = %.2f,   K-S = %.3f" % (data.Chi2Test(bkg, "CHI2/NDF"), data.KolmogorovTest(bkg)))
    latex.DrawLatex(0.45, 0.85, "#chi^{2}/ndf = %.2f" % ( data.Chi2Test(bkg, "CHI2/NDF") ) )

pass

def drawRelativeYield(data,bkg):
    latex = TLatex()
    latex.SetNDC()
    latex.SetTextColor(1)
    latex.SetTextFont(62)
    latex.SetTextSize(0.08)
    latex.DrawLatex(0.75, 0.85, "rel. Yield= %.3f" % ((data.Integral()/bkg.Integral())*100) )
pass

def setTopPad(TopPad, r=4):
    TopPad.SetPad("TopPad", "", 0., 1./r, 1.0, 1.0, 0, -1, 0)
    TopPad.SetTopMargin(0.24/r)
    TopPad.SetBottomMargin(0.04/r)
    TopPad.SetRightMargin(0.05)
    TopPad.SetTicks(1, 1)
pass

def setBotPad(BotPad, r=4):
    BotPad.SetPad("BotPad", "", 0., 0., 1.0, 1./r, 0, -1, 0)
    BotPad.SetTopMargin(r/100.)
    BotPad.SetBottomMargin(r/10.)
    BotPad.SetRightMargin(0.05)
    BotPad.SetTicks(1, 1)
pass

def addOverflow(hist, addUnder=True):
    n = hist.GetNbinsX()
    hist.SetBinContent(n, hist.GetBinContent(n) + hist.GetBinContent(n+1))
    hist.SetBinError(n, math.sqrt( hist.GetBinError(n)**2 + hist.GetBinError(n+1)**2 ) )
    hist.SetBinContent(n+1, 0.)
    hist.SetBinError(n+1, 0.)
    if addUnder:
        hist.SetBinContent(1, hist.GetBinContent(0) + hist.GetBinContent(1))
        hist.SetBinError(1, math.sqrt( hist.GetBinError(0)**2 + hist.GetBinError(1)**2 ) )
        hist.SetBinContent(0, 0.)
        hist.SetBinError(0, 0.)
pass

# Write a histogram with a given name to the output ROOT file
def writeHistogram(h, name, range_):
    h.GetYaxis().SetTitle("Events / %s GeV" % ( float( (range_[2]-range_[1]) / range_[0] ) ) )
    h.SetName(name)
    h.Write()
pass

# Retrieve a histogram from the input file based on the process and the variable
# name
def getHistogram( tfile , name ):
    h = tfile.Get(name)
    if not h:
        raise Exception("Failed to load histogram {}.".format(name))
    return h
pass

def histo1D( hdata , hmc , output , variable , xlabel , scale , ratio=False , logy=False ):

    hist={}
    hist['DATA'] = hdata.Clone("DATA")
    hist['DY'] = hmc.Clone("DY")
    hist['BkgSum'] = hist['DY'].Clone("BkgSum")

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
    leg.AddEntry(hist['DY'], 'DY' , "f")
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
    drawCMS( "%s" %scale, "Object Study")

    c1.Update()

    print( "{}/{}.pdf".format( output , variable) )
    c1.SaveAs( "{}/{}.pdf".format(output, variable) )
    c1.SaveAs( "{}/{}.png".format(output, variable) )
pass
