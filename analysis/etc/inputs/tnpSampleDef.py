from libPython.tnpClassUtils import tnpSample
import os

#latinov6_16 = '%s/ntuple/results/latinov6_16/' % os.environ.get('BASETNP')
#latinov6_17 = '%s/ntuple/results/latinov6_17/' % os.environ.get('BASETNP')
#latinov6_18 = '%s/ntuple/results/latinov6_18/' % os.environ.get('BASETNP')

#latinov7_16 = '%s/skim/results/latinov7_16/'   % os.environ.get('BASETNP') 
#latinov7_17 = '%s/skim/results/latinov7_17/'   % os.environ.get('BASETNP')
#latinov7_18 = '%s/skim/results/latinov7_18/'   % os.environ.get('BASETNP')

#nanoV7_16 = '%s/skim/results/Full2016v7_102X/' % os.environ.get('BASETNP')
#nanoV7_17 = '%s/skim/results/Full2017v7_102X/' % os.environ.get('BASETNP')
#nanoV7_18 = '%s/skim/results/Full2018v7_102X/' % os.environ.get('BASETNP')

latinoUL_16_HIPM   = '%s/skim/results/latino16_HIPM/merged/' % os.environ.get('BASETNP')
latinoUL_16_noHIPM = '%s/skim/results/latino16_noHIPM/merged/' % os.environ.get('BASETNP')
latinoUL_17        = '%s/skim/results/latino17/merged/' % os.environ.get('BASETNP')
latinoUL_18        = '%s/skim/results/latino18/merged/' % os.environ.get('BASETNP')

##########################
#UL
#nanoUL_16_HIPM = {
#    'DYJetsToLL_M-50-LO' : tnpSample('DYJetsToLL_M-50-LO', latinoUL_16_HIPM + 'DYJetsToLL_M-50-LO.root', isMC = True, nEvts =  -1 ),
#    'DYJetsToLL_M-50'    : tnpSample('DYJetsToLL_M-50',    latinoUL_16_HIPM + 'DYJetsToLL_M-50.root', isMC = True, nEvts =  -1 ),
#    'data_Run2016_HIPM'  : tnpSample('SingleElectron',     latinoUL_16_HIPM + 'SingleElectron.root' , lumi = 35.867 ),
#}

#nanoUL_16_noHIPM = {
#    'DYJetsToLL_M-50-LO'  : tnpSample('DYJetsToLL_M-50-LO', latinoUL_16_noHIPM + 'DYJetsToLL_M-50-LO.root', isMC = True, nEvts =  -1 ),
#    'DYJetsToLL_M-50'     : tnpSample('DYJetsToLL_M-50',    latinoUL_16_noHIPM + 'DYJetsToLL_M-50.root', isMC = True, nEvts =  -1 ),
#    'data_Run2016_noHIPM' : tnpSample('SingleElectron',     latinoUL_16_noHIPM + 'SingleElectron.root' , lumi = 35.867 ),
#}

nanoUL_17 = {
    'DYJetsToLL_M-50-LO' : tnpSample('DYJetsToLL_M-50-LO', latinoUL_17 + 'DYJetsToLL_M-50_LO.root', isMC = True, nEvts =  -1 ),

    'DYJetsToLL_M-50' : tnpSample('DYJetsToLL_M-50', latinoUL_17 + 'DYJetsToLL_M-50_ext1.root', isMC = True, nEvts =  -1 ),

    'data_Run2017' : tnpSample('SingleElectron_Run2017' , latinoUL_17 + 'SingleElectron.root' , lumi = 41.48 ),

    'data_Run2017B' : tnpSample('SingleElectron_Run2017B' , latinoUL_17 + 'SingleElectron_Run2017B.root' , lumi = 4.803 ),
    'data_Run2017C' : tnpSample('SingleElectron_Run2017C' , latinoUL_17 + 'SingleElectron_Run2017C.root' , lumi = 9.574),
    'data_Run2017D' : tnpSample('SingleElectron_Run2017D' , latinoUL_17 + 'SingleElectron_Run2017D.root' , lumi = 4.248 ),
    'data_Run2017E' : tnpSample('SingleElectron_Run2017E' , latinoUL_17 + 'SingleElectron_Run2017E.root' , lumi = 9.315),
    'data_Run2017F' : tnpSample('SingleElectron_Run2017F' , latinoUL_17 + 'SingleElectron_Run2017F.root' , lumi = 13.540),
}

nanoUL_18 = {
    'DYJetsToLL_M-50-LO' : tnpSample('DYJetsToLL_M-50-LO', latinoUL_18 + 'DYJetsToLL_M-50_LO.root', isMC = True, nEvts =  -1 ),

    'DYJetsToLL_M-50' : tnpSample('DYJetsToLL_M-50', latinoUL_18 + 'DYJetsToLL_M-50_ext1.root', isMC = True, nEvts =  -1 ),

    'data_Run2018' : tnpSample('EGamma_Run2018' , latinoUL_18 + 'EGamma.root' , lumi = 59.83 ),

    'data_Run2018A' : tnpSample('EGamma_Run2018A' , latinoUL_18 + 'EGamma_Run2018A.root' , lumi = 14.03 ),
    'data_Run2018B' : tnpSample('EGamma_Run2018B' , latinoUL_18 + 'EGamma_Run2018B.root' , lumi = 7.067 ),
    'data_Run2018C' : tnpSample('EGamma_Run2018C' , latinoUL_18 + 'EGamma_Run2018C.root' , lumi = 6.899 ),
    'data_Run2018D' : tnpSample('EGamma_Run2018D' , latinoUL_18 + 'EGamma_Run2018D.root' , lumi = 31.84 ),
}

###################

#nanov5_16 = {
#    'DYJetsToLL_M-50-LO_ext2' : tnpSample('DYJetsToLL_M-50-LO_ext2',
#                                     latinov6_16 + 'DYJetsToLL_M-50-LO_ext2.root',
#                                     isMC = True, nEvts =  -1 ),

#    'DYJetsToLL_M-50' : tnpSample('DYJetsToLL_M-50',
#                                  latinov6_16 + 'DYJetsToLL_M-50.root',
#                                  isMC = True, nEvts =  -1 ),

#    'data_Run2016' : tnpSample('SingleElectron' , latinov6_16 + 'SingleElectron_Run2016.root' , lumi = 35.867 ),
#}

#nanov5_17 = {
#    'DYJetsToLL_M-50-LO_ext1' : tnpSample('DYJetsToLL_M-50-LO_ext1',
#                                          latinov6_17 + 'DYJetsToLL_M-50-LO_ext1.root',
#                                     isMC = True, nEvts =  -1 ),
    
#    'DYJetsToLL_M-50_ext1' : tnpSample('DYJetsToLL_M-50_ext1',
#                                       latinov6_17 + 'DYJetsToLL_M-50_ext1.root',
#                                       isMC = True, nEvts =  -1 ),

#    'data_Run2017' : tnpSample('SingleElectron' , latinov6_17 + 'SingleElectron_Run2017.root' , lumi = 41.53 ),
#}

#nanov6_18 = {
#    'DYJetsToLL_M-50-LO' : tnpSample('DYJetsToLL_M-50-LO',
#                                     latinov6_18 + 'DYJetsToLL_M-50-LO.root',
#                                     isMC = True, nEvts =  -1 ),

#    'DYJetsToLL_M-50_ext2' : tnpSample('DYJetsToLL_M-50_ext2',
#                                      latinov6_18 + 'DYJetsToLL_M-50_ext2.root',
#                                      isMC = True, nEvts =  -1 ),

#    'data_Run2018' : tnpSample('EGamma' , latinov6_18 + 'EGamma_Run2018.root' , lumi = 59.74 ),

#}

#####################
#nanov6_16 = {
#    'DYJetsToLL_M-50-LO' : tnpSample('DYJetsToLL_M-50-LO_ext1',
#                                     latinov7_16 + 'DYJetsToLL_M-50-LO_ext1.root',
#                                     isMC = True, nEvts =  -1 ),

#    'DYJetsToLL_M-50' : tnpSample('DYJetsToLL_M-50_ext2',
#                                  latinov7_16 + 'DYJetsToLL_M-50_ext2.root',
#                                  isMC = True, nEvts =  -1 ),

#    'data_Run2016' : tnpSample('SingleElectron' , latinov7_16 + 'SingleElectron_Run2016.root' , lumi = 35.867 ),
#}

#nanov6_17 = {
#    'DYJetsToLL_M-50-LO' : tnpSample('DYJetsToLL_M-50-LO_ext1',
#                                          latinov7_17 + 'DYJetsToLL_M-50-LO_ext1.root',
#                                     isMC = True, nEvts =  -1 ),

#    'DYJetsToLL_M-50' : tnpSample('DYJetsToLL_M-50_ext1',
#                                       latinov7_17 + 'DYJetsToLL_M-50_ext1.root',
#                                       isMC = True, nEvts =  -1 ),

#    'data_Run2017' : tnpSample('SingleElectron' , latinov7_17 + 'SingleElectron_Run2017.root' , lumi = 41.53 ),
    
#    'data_Run2017B' : tnpSample('data_Run2017B' , latinov7_17 + 'RunB.root' , lumi = 4.793 ),
#    'data_Run2017C' : tnpSample('data_Run2017C' , latinov7_17 + 'RunC.root' , lumi = 9.753),
#    'data_Run2017D' : tnpSample('data_Run2017D' , latinov7_17 + 'RunD.root' , lumi = 4.320 ),
#    'data_Run2017CD' : tnpSample('data_Run2017CD' , latinov7_17 + 'RunCD.root' , lumi = 14.073 ),
#    'data_Run2017E' : tnpSample('data_Run2017E' , latinov7_17 + 'RunE.root' , lumi = 8.802),
#    'data_Run2017F' : tnpSample('data_Run2017F' , latinov7_17 + 'RunF.root' , lumi = 13.567),

#}

#nanov6_1_18 = {
#    'DYJetsToLL_M-50-LO' : tnpSample('DYJetsToLL_M-50-LO',
#                                     latinov7_18 + 'DYJetsToLL_M-50-LO.root',
#                                     isMC = True, nEvts =  -1 ),

#    'DYJetsToLL_M-50' : tnpSample('DYJetsToLL_M-50_ext2',
#                                      latinov7_18 + 'DYJetsToLL_M-50_ext2.root',
#                                      isMC = True, nEvts =  -1 ),

#    'data_Run2018' : tnpSample('EGamma' , latinov7_18 + 'EGamma_Run2018.root' , lumi = 59.74 ),

#}

##########################
#nanov7_16 = {
#    'DYJetsToLL_M-50-LO' : tnpSample('DYJetsToLL_M-50-LO_ext1',
#                                     nanoV7_16 + 'DYJetsToLL_M-50-LO_ext1.root',
#                                     isMC = True, nEvts =  -1 ),

#    'DYJetsToLL_M-50' : tnpSample('DYJetsToLL_M-50_ext2',
#                                  nanoV7_16 + 'DYJetsToLL_M-50_ext2.root',
#                                  isMC = True, nEvts =  -1 ),

#    'data_Run2016' : tnpSample('SingleElectron' , nanoV7_16 + 'SingleElectron.root' , lumi = 35.867 ),
#}

#nanov7_17 = {
#    'DYJetsToLL_M-50-LO' : tnpSample('DYJetsToLL_M-50-LO_ext1',
#                                          nanoV7_17 + 'DYJetsToLL_M-50-LO_ext1.root',
#                                     isMC = True, nEvts =  -1 ),

#    'DYJetsToLL_M-50' : tnpSample('DYJetsToLL_M-50_ext1',
#                                       nanoV7_17 + 'DYJetsToLL_M-50_ext1.root',
#                                       isMC = True, nEvts =  -1 ),

#    'data_Run2017' : tnpSample('SingleElectron' , nanoV7_17 + 'SingleElectron.root' , lumi = 41.53 ),

#    'data_Run2017B' : tnpSample('data_Run2017B' , nanoV7_17 + 'SingleElectron_RunB.root' , lumi = 4.793 ),
#    'data_Run2017C' : tnpSample('data_Run2017C' , nanoV7_17 + 'SingleElectron_RunC.root' , lumi = 9.753),
#    'data_Run2017D' : tnpSample('data_Run2017D' , nanoV7_17 + 'SingleElectron_RunD.root' , lumi = 4.320 ),
#    'data_Run2017CD' : tnpSample('data_Run2017CD' , nanoV7_17 + 'SingleElectron_RunCD.root' , lumi = 14.073 ),
#    'data_Run2017E' : tnpSample('data_Run2017E' , nanoV7_17 + 'SingleElectron_RunE.root' , lumi = 8.802),
#    'data_Run2017F' : tnpSample('data_Run2017F' , nanoV7_17 + 'SingleElectron_RunF.root' , lumi = 13.567),

#}

#nanov7_18 = {
#    'DYJetsToLL_M-50-LO' : tnpSample('DYJetsToLL_M-50-LO',
#                                     nanoV7_18 + 'DYJetsToLL_M-50-LO.root',
#                                     isMC = True, nEvts =  -1 ),

#    'DYJetsToLL_M-50' : tnpSample('DYJetsToLL_M-50_ext2',
#                                      nanoV7_18 + 'DYJetsToLL_M-50_ext2.root',
#                                      isMC = True, nEvts =  -1 ),

#    'data_Run2018' : tnpSample('EGamma' , nanoV7_18 + 'EGamma.root' , lumi = 59.74 ),

#}



