from libPython.tnpClassUtils import tnpSample
import os
cwd = os.getcwd()
latinov6_16 = '%s/../ntuple/results/latinov6_16/' % cwd
latinov6_17 = '%s/../ntuple/results/latinov6_17/' % cwd
latinov6_18 = '%s/../ntuple/results/latinov6_18/' % cwd
latinov7_16 = '%s/../skim/results/latinov7_16/'   % cwd 
latinov7_17 = '%s/../skim/results/latinov7_17/'   % cwd
latinov7_18 = '%s/../skim/results/latinov7_18/'   % cwd

nanov5_16 = {
    'DYJetsToLL_M-50-LO_ext2' : tnpSample('DYJetsToLL_M-50-LO_ext2',
                                     latinov6_16 + 'DYJetsToLL_M-50-LO_ext2.root',
                                     isMC = True, nEvts =  -1 ),

    'DYJetsToLL_M-50' : tnpSample('DYJetsToLL_M-50',
                                  latinov6_16 + 'DYJetsToLL_M-50.root',
                                  isMC = True, nEvts =  -1 ),

    'data_Run2016' : tnpSample('SingleElectron' , latinov6_16 + 'SingleElectron_Run2016.root' , lumi = 35.867 ),
}

nanov5_17 = {
    'DYJetsToLL_M-50-LO_ext1' : tnpSample('DYJetsToLL_M-50-LO_ext1',
                                          latinov6_17 + 'DYJetsToLL_M-50-LO_ext1.root',
                                     isMC = True, nEvts =  -1 ),
    
    'DYJetsToLL_M-50_ext1' : tnpSample('DYJetsToLL_M-50_ext1',
                                       latinov6_17 + 'DYJetsToLL_M-50_ext1.root',
                                       isMC = True, nEvts =  -1 ),

    'data_Run2017' : tnpSample('SingleElectron' , latinov6_17 + 'SingleElectron_Run2017.root' , lumi = 41.53 ),
}

nanov6_18 = {
    'DYJetsToLL_M-50-LO' : tnpSample('DYJetsToLL_M-50-LO',
                                     latinov6_18 + 'DYJetsToLL_M-50-LO.root',
                                     isMC = True, nEvts =  -1 ),

    'DYJetsToLL_M-50_ext2' : tnpSample('DYJetsToLL_M-50_ext2',
                                      latinov6_18 + 'DYJetsToLL_M-50_ext2.root',
                                      isMC = True, nEvts =  -1 ),

    'data_Run2018' : tnpSample('EGamma' , latinov6_18 + 'EGamma_Run2018.root' , lumi = 59.74 ),

}

nanov7_16 = {
    'DYJetsToLL_M-50-LO' : tnpSample('DYJetsToLL_M-50-LO_ext1',
                                     latinov7_16 + 'DYJetsToLL_M-50-LO_ext1.root',
                                     isMC = True, nEvts =  -1 ),

    'DYJetsToLL_M-50' : tnpSample('DYJetsToLL_M-50_ext2',
                                  latinov7_16 + 'DYJetsToLL_M-50_ext2.root',
                                  isMC = True, nEvts =  -1 ),

    'data_Run2016' : tnpSample('SingleElectron' , latinov7_16 + 'SingleElectron_Run2016.root' , lumi = 35.867 ),
}

nanov7_17 = {
    'DYJetsToLL_M-50-LO' : tnpSample('DYJetsToLL_M-50-LO_ext1.root',
                                          latinov7_17 + 'DYJetsToLL_M-50-LO_ext1.root',
                                     isMC = True, nEvts =  -1 ),

    'DYJetsToLL_M-50' : tnpSample('DYJetsToLL_M-50_ext1',
                                       latinov7_17 + 'DYJetsToLL_M-50_ext1.root',
                                       isMC = True, nEvts =  -1 ),

    'data_Run2017' : tnpSample('SingleElectron' , latinov7_17 + 'SingleElectron_Run2017.root' , lumi = 41.53 ),
}

nanov7_18 = {
    'DYJetsToLL_M-50-LO' : tnpSample('DYJetsToLL_M-50-LO',
                                     latinov7_18 + 'DYJetsToLL_M-50-LO.root',
                                     isMC = True, nEvts =  -1 ),

    'DYJetsToLL_M-50' : tnpSample('DYJetsToLL_M-50_ext2',
                                      latinov7_18 + 'DYJetsToLL_M-50_ext2.root',
                                      isMC = True, nEvts =  -1 ),

    'data_Run2018' : tnpSample('EGamma' , latinov7_18 + 'EGamma_Run2018.root' , lumi = 59.74 ),

}



