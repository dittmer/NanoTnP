from libPython.tnpClassUtils import tnpSample
import os

latinov5_17 = '/afs/cern.ch/work/s/shoh/analysis/TnP/NanoTnP/results/latinov5_17/'
latinov5_18 = '/afs/cern.ch/work/s/shoh/analysis/TnP/NanoTnP/results/latinov5_18/'

nanov4_17 = {
    'DYJetsToLL_M-50-LO' : tnpSample('DYJetsToLL_M-50-LO',
                                     latinov5_17 + 'DYJetsToLL_M-50-LO.root',
                                     isMC = True, nEvts =  -1 ),
    
    'DYJetsToLL_M-50_ext1' : tnpSample('DYJetsToLL_M-50_ext1',
                                       latinov5_17 + 'DYJetsToLL_M-50_ext1.root',
                                       isMC = True, nEvts =  -1 ),

    'data_Run2017' : tnpSample('SingleElectron' , latinov5_17 + 'SingleElectron_Run2017.root' , lumi = 41.53 ),
    
    #'data_Run2017B' : tnpSample('data_Run2017B' , eosMoriond18 + 'RunB.root' , lumi = 4.793 ),
    #'data_Run2017C' : tnpSample('data_Run2017C' , eosMoriond18 + 'RunC.root' , lumi = 9.753),
    #'data_Run2017D' : tnpSample('data_Run2017D' , eosMoriond18 + 'RunD.root' , lumi = 4.320 ),
    #'data_Run2017E' : tnpSample('data_Run2017E' , eosMoriond18 + 'RunE.root' , lumi = 8.802),
    #'data_Run2017F' : tnpSample('data_Run2017F' , eosMoriond18 + 'RunF.root' , lumi = 13.567),
    
    }

#nanov5_18 = {
#    'DYJetsToLL_M-50-LO' : tnpSample('DYJetsToLL_M-50-LO',
#                                     latinov5_17 + 'DYJetsToLL_M-50-LO.root',
#                                     isMC = True, nEvts =  -1 ),

#    'DYJetsToLL_M-50_ext1' : tnpSample('DYJetsToLL_M-50_ext1',
#                                       latinov5_17 + 'DYJetsToLL_M-50_ext1.root',
#                                       isMC = True, nEvts =  -1 ),

#    'data_Run2017' : tnpSample('SingleElectron' , eosMoriond18 + 'SingleElectron_Run2017.root' , lumi = 41.53 ),#


#}
