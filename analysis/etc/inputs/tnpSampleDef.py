from libPython.tnpClassUtils import tnpSample
import os
cwd = os.getcwd()
latinov5_17 = '%s/../results/latinov5_17/' % cwd
latinov5_18 = '%s/../results/latinov5_18/' % cwd

nanov4_17 = {
    'DYJetsToLL_M-50-LO' : tnpSample('DYJetsToLL_M-50-LO',
                                     latinov5_17 + 'DYJetsToLL_M-50-LO.root',
                                     isMC = True, nEvts =  -1 ),
    
    'DYJetsToLL_M-50_ext1' : tnpSample('DYJetsToLL_M-50_ext1',
                                       latinov5_17 + 'DYJetsToLL_M-50_ext1.root',
                                       isMC = True, nEvts =  -1 ),

    'data_Run2017' : tnpSample('SingleElectron' , latinov5_17 + 'SingleElectron_Run2017.root' , lumi = 41.53 ),
}

nanov5_18 = {
    'DYJetsToLL_M-50-LO' : tnpSample('DYJetsToLL_M-50-LO',
                                     latinov5_18 + 'DYJetsToLL_M-50-LO.root',
                                     isMC = True, nEvts =  -1 ),

    'DYJetsToLL_M-50_ext' : tnpSample('DYJetsToLL_M-50_ext',
                                      latinov5_18 + 'DYJetsToLL_M-50_ext.root',
                                      isMC = True, nEvts =  -1 ),

    'data_Run2018' : tnpSample('EGamma' , latinov5_18 + 'EGamma_Run2018.root' , lumi = 59.74 ),

}
