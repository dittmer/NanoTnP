# Implementing histogramming

import argparse, os
import ROOT
import numpy as np
from collections import OrderedDict
from ROOT.TMath import Max, Abs
ROOT.gROOT.SetBatch(True)

ROOT.ROOT.EnableImplicitMT(10)
ROOT.gStyle.SetOptStat(0);

nano_eta="Probe_sc_eta"
nanotag_eta="Tag_sc_eta"

#nano_eta="Probe_eta"
#nanotag_eta="Tag_eta"

######## 2016 benchmark #########
#mini
Mini_cutMissingInnerHits = 'el_mHits==0 '
Mini_cutdz = '(( abs(el_sc_abseta) > 1.497 && abs(el_dz) < 0.2 )||( abs(el_sc_abseta) < 1.497 && abs(el_dz) < 0.1 ))'
Mini_cutd0 = '(( abs(el_sc_abseta) > 1.497 && abs(el_dxy) < 0.1 )||( abs(el_sc_abseta) < 1.497 && abs(el_dxy) < 0.05 ))'
Mini_looseDef  = 'passingHLTsafe ==1 && '+ Mini_cutMissingInnerHits +' && '+ Mini_cutdz +' && '+ Mini_cutd0
Mini_effArea = '(abs(el_sc_abseta)>2.4)*0.2393 || (abs(el_sc_abseta)>2.3)*0.1937 ||  (abs(el_sc_abseta)>2.2)*0.1635 ||  (abs(el_sc_abseta)>2.0)*0.1230 ||  (abs(el_sc_abseta)>1.479)*0.1213 ||  (abs(el_sc_abseta)>1.0)*1.715 || (abs(el_sc_abseta)>0)*0.1703'
Mini_cutIso16Barrel = '( (abs(el_sc_abseta) < 1.479) && (pfRelIso03_all < 0.0588) )'
Mini_cutIso16Endcap = '( (abs(el_sc_abseta) > 1.479) && (pfRelIso03_all < 0.0571) )'

#nano
Nano_cutMissingInnerHits = 'Probe_lostHits<1'
Nano_cutdz = '(( abs('+nano_eta+') > 1.497 && abs(Probe_dz) < 0.2 )||( abs('+nano_eta+') < 1.497 && abs(Probe_dz) < 0.1 ))'
Nano_cutd0 = '(( abs('+nano_eta+') > 1.497 && abs(Probe_dxy) < 0.1 )||( abs('+nano_eta+') < 1.497 && abs(Probe_dxy) < 0.05 ))'
Nano_looseDef  = 'Probe_cutBased_HLTPreSel ==1 && '+ Nano_cutMissingInnerHits +' && '+ Nano_cutdz +' && '+ Nano_cutd0
Nano_cutIso16Barrel = '( (abs('+nano_eta+') < 1.479) && (Probe_pfRelIso03_all < 0.0588) )'
Nano_cutIso16Endcap = '( (abs('+nano_eta+') > 1.479) && (Probe_pfRelIso03_all < 0.0571) )'
#################################

##### 2017/2018 benchmark #######
#mini
Mini_cutLoose = '(( abs(el_sc_abseta) > 1.497 && (el_sieie < 0.03 ) && (el_1overEminus1overP < 0.014)) ||( abs(el_sc_abseta) < 1.497))'
Mini_cutDz = '(( abs(el_sc_abseta) > 1.497 && abs(el_dz) < 0.2 )||( abs(el_sc_abseta) < 1.497 && abs(el_dz) < 0.1 ))'
Mini_cutDxy = '(( abs(el_sc_abseta) > 1.497 && abs(el_dxy) < 0.1 )||( abs(el_sc_abseta) < 1.497 && abs(el_dxy) < 0.05 ))'
Mini_cutIso = '(el_reliso03 < 0.06)'

#nano
Nano_cutLoose = '(( abs('+nano_eta+') > 1.497 && (Probe_sieie < 0.03) && (Probe_eInvMinusPInv < 0.014)) ||( abs('+nano_eta+') < 1.497))'
Nano_cutDz = '(( abs('+nano_eta+') > 1.497 && abs(Probe_dz) < 0.2 )||( abs('+nano_eta+') < 1.497 && abs(Probe_dz) < 0.1 ))'
Nano_cutDxy = '(( abs('+nano_eta+') > 1.497 && abs(Probe_dxy) < 0.1 )||( abs('+nano_eta+') < 1.497 && abs(Probe_dxy) < 0.05 ))'
Nano_cutIso = '(Probe_pfRelIso03_all < 0.06)'
#################################
#################################

flags={
    '16' :
    {
        'mini' : {
            'baseline' : '({0}) && ( passingMVA80Xwp90 == 1) && ( {1} || {2} )'.format(Mini_looseDef,Mini_cutIso16Barrel,Mini_cutIso16Endcap),
        },
        'nano' : {
            #passingMVA80Xwp90Iso16
            'baseline' : '({0}) && (Probe_mvaSpring16GP_WP90 == 1) && ( {1} || {2} )'.format(Nano_looseDef,Nano_cutIso16Barrel,Nano_cutIso16Endcap),
        },
    },
    '17' : 
    {
        'mini' : {
            #'passingMVA94Xwp90isoHWWiso0p06'
            'baseline'    : '({0}) && ({1}) && ({2}) && ({3}) && (passingMedium94X == 1) && (passingMVA94Xwp90iso == 1)'.format(Mini_cutDxy,Mini_cutDz,Mini_cutLoose,Mini_cutIso),
        },
        'nano' : {
            #'passingMVA94Xwp90isoHWWiso0p06'
            'baseline'    : '({0}) && ({1}) && ({2}) && ({3}) && (Probe_cutBased_Fall17_V1>=3) && (Probe_mvaFall17V1Iso_WP90==1)'.format(Nano_cutDxy,Nano_cutDz,Nano_cutLoose,Nano_cutIso),
        },
    },
    '18' :
    {
        'mini' : {
            #'passingMVA94Xwp90isoHWWiso0p06'
            'baseline'    : '({0}) && ({1}) && ({2}) && ({3}) && (passingMedium94X == 1) && (passingMVA94Xwp90iso == 1)'.format(Mini_cutDxy,Mini_cutDz,Mini_cutLoose,Mini_cutIso),
        },
        'nano' : {
            #'passingMVA94Xwp90isoHWWiso0p06'
            'baseline'    : '({0}) && ({1}) && ({2}) && ({3}) && (Probe_cutBased_Fall17_V1>=3) && (Probe_mvaFall17V1Iso_WP90==1)'.format(Nano_cutDxy,Nano_cutDz,Nano_cutLoose,Nano_cutIso),
        },
    },
}

cuts={
    'tagpt':{
        '16' : '32',
        '17' : '40',
        '18' : '37',
    }
}

dft={}

class cutter:
    def __init__(self, name_ , isMC_):
        self.name = name_
        self.isMC = isMC_
        self.nanoseq = []
        self.miniseq = []
        self.dd = OrderedDict()

    def registerCut(self, df , desp,  miniC="1==1" , nanoC="1==1" ):
        self.miniseq.append(miniC) ; miniCuts = " && ".join(self.miniseq)
        self.nanoseq.append(nanoC) ; nanoCuts = " && ".join(self.nanoseq)
        #print(miniCuts)
        minidf = df['mini'] if self.isMC else df['mini'].Define("totWeight","1")
        miniY = minidf.Filter( miniCuts ).Sum( "totWeight" ).GetValue()
        nanoY = df['nano'].Filter( nanoCuts ).Sum( "weight" ).GetValue()
        diffs = ROOT.TMath.Abs(float(nanoY)-float(miniY))/float(miniY)
        # store the difference
        self.dd[desp] = diffs
        
        print(" - %s --> mini : %.2f ; nano : %.2f ; diff : %.2f%%" %( desp , miniY , nanoY , float(diffs)*100 ) )
    pass

    def define(self, df , aod , branch , defs):
        df[aod] = df[aod].Define(branch,defs)
    pass

    def histo(self, proc): ## make slef.dd
        nbin = len(self.dd)
        xlabels = list(self.dd.keys())
        out = ROOT.TCanvas( "out" , "cutflow abs diffirence %s" %proc )
        hist = ROOT.TH1F("hist", "cutflow abs diffirence %s" %proc , nbin , 0 , nbin )
        for ibin in range(1,nbin+1): hist.GetXaxis().SetBinLabel( ibin , xlabels[ibin-1] )
        for ibin in range(0,nbin): hist.Fill( ibin , self.dd[xlabels[ibin]] )
        #for ibin in range(1,nbin+1): print("ibin : ", ibin , " ; xlabels : ", xlabels[ibin-1] , " ; self.dd[xlabels[ibin-1]] :" , self.dd[xlabels[ibin-1]] )
        #hist.SetFillColor(38)
        hist.Draw('HIST')
        out.SaveAs( "%s.png" %proc)
    pass

def main(mini , nano , process):
    isMC = False if process.split('_')[0]=="Data" else True
    cutlister = cutter(process , isMC)

    ptcuts = cuts['tagpt'][process.split('_')[-1]]

    # Load skimmed dataset
    dft['mini'] = ROOT.ROOT.RDataFrame( 'tnpEleIDs/fitter_tree' , mini)
    dft['nano'] = ROOT.ROOT.RDataFrame( 'fitter_tree' , nano )
    print("")
    cutlister.registerCut( dft , "Default" )

    if (process.split('_')[-1]=='16'):
        #eta restriction on 2016
        cutlister.registerCut( dft , "Tag cut pt>%s GeV" %ptcuts , "tag_Ele_pt > %s && abs(tag_sc_eta) < 2.17" %ptcuts ,  "Tag_pt > %s && abs(%s) < 2.17" %(ptcuts,nanotag_eta) )
        #WP definition 2016
        cutlister.define( dft , 'mini' , 'pfRelIso03_all_2' , '(el_neuIso + el_phoIso - (event_rho.rho)*({0}))'.format(Mini_effArea) )
        cutlister.define( dft , 'mini' , 'pfRelIso03_all' , '( el_chIso + TMath::Max( 0.0 , double(pfRelIso03_all_2) ) )/el_pt' )
        #cutlister.define( dft , 'mini' , 'pfRelIso03_all' , '(el_chIso + TMath::Max( 0.0 , el_neuIso + el_phoIso - (event_rho.rho)*({0})))/el_pt'.format(Mini_effArea) )
    else:
        cutlister.registerCut( dft , "Tag cut pt>%s GeV" %ptcuts , "tag_Ele_pt > %s" %ptcuts , "Tag_pt > %s" %ptcuts )

    cutlister.registerCut( dft , "Probe cut" , " el_pt > 7 && abs(el_sc_eta) < 2.5" , "Probe_pt > 7 && abs("+nano_eta+") < 2.5" )
    #cutlister.registerCut( dft , "Data HLT match" , "1==1" if isMC else "1==1" , "1==1" if isMC else "TnP_trigger==1")
    cutlister.registerCut( dft , "MC Truth match" , "1==1" if not isMC else "mcTrue==1" , "1==1" if not isMC else "mcTrue==1")
    cutlister.registerCut( dft , "Opposite Sign" , "el_q*tag_Ele_q < 0" , "Tag_charge*Probe_charge < 0")
    cutlister.registerCut( dft , "Pair mass selection" , "pair_mass > 60 && pair_mass < 120" , "pair_mass > 60 && pair_mass < 120")
    cutlister.define( dft , 'mini' , "lowpt" , "sqrt( 2*event_met_pfmet.mpfMET*tag_Ele_pt*(1-cos(event_met_pfphi.mpfPhi-tag_Ele_phi)))" )
    cutlister.define( dft , 'nano' , "lowpt" , "sqrt( 2*event_met_pfmet*Tag_pt*(1-cos(event_met_pfphi-Tag_phi)))" )
    #cutlister.registerCut( dft , "pt[15,20] ; eta[-0.8,0.0]" , "(el_pt > 15 && el_pt < 20) && (el_sc_eta > -0.8 && el_sc_eta < 0)" , "(Probe_pt > 15 && Probe_pt < 20) && ("+nano_eta+" > -0.8 && "+nano_eta+"< 0)")
    cutlister.registerCut( dft , "pt[15,20] ; eta[-2.5,-2.0]" , "(el_pt > 15 && el_pt < 20) && (el_sc_eta > -2.5 && el_sc_eta < -2.0 )" , "(Probe_pt > 15 && Probe_pt < 20) && ("+nano_eta+" > -2.5 && "+nano_eta+"< -2.0)")
    cutlister.registerCut( dft , "lowpt cut" , "tag_Ele_trigMVA > 0.92 && lowpt < 45" , "tag_Ele_trigMVA > 0.92 && lowpt < 45")
    cutlister.define( dft , 'mini' , "flags" , flags[process.split('_')[-1]]['mini']['baseline'] )
    cutlister.define( dft , 'nano' , "flags" , flags[process.split('_')[-1]]['nano']['baseline'] )
    cutlister.registerCut( dft , "pass baseline flag" , "flags==1" , "flags==1" )
    
    print("")
    
    cutlister.histo(process)
    #ROOT.RDF.SaveGraph(dft['mini'],"graph_mini"+process+".dot");
    #ROOT.RDF.SaveGraph(dft['nano'],"graph_nano"+process+".dot");
    #report = fltr.Report()
    #report.Print()
    pass

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("mini", type=str, help="Full path to mini root file")
    parser.add_argument("nano", type=str, help="Full path to nano root file")
    parser.add_argument("process", type=str, help="Process name")
    
    args = parser.parse_args()
    print(" --> mini : %s" % args.mini)
    print(" --> nano : %s" % args.nano)
    print(" --> process : %s" % args.process)
    main( args.mini , args.nano , args.process )
