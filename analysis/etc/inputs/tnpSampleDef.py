from libPython.tnpClassUtils import tnpSample
import os

#UL
latinoUL_16_HIPM   = '%s/skim/results/latino16_HIPM/merged/'   % os.environ.get('BASETNP')
latinoUL_16_noHIPM = '%s/skim/results/latino16_noHIPM/merged/' % os.environ.get('BASETNP')
latinoUL_17        = '%s/skim/results/latino17/merged/'        % os.environ.get('BASETNP')
latinoUL_18        = '%s/skim/results/latino18/merged/'        % os.environ.get('BASETNP')

##########################
#UL
nanoUL_16_HIPM = {
    'DYJetsToLL_M-50-LO'    : tnpSample('DYJetsToLL_M-50-LO',         latinoUL_16_HIPM + 'DYJetsToLL_M-50-LO.root', isMC = True, nEvts =  -1 ),
    'DYJetsToLL_M-50'       : tnpSample('DYJetsToLL_M-50',            latinoUL_16_HIPM + 'DYJetsToLL_M-50.root',    isMC = True, nEvts =  -1 ),
    'data_Run2016_HIPM'     : tnpSample('SingleElectron',             latinoUL_16_HIPM + 'SingleElectron.root',                         lumi = 19.521 ),
    'data_Run2016BCDE_HIPM' : tnpSample('SingleElectron_Run2016BCDE', latinoUL_16_HIPM + 'SingleElectron_Run2016BCDE.root',             lumi = 16.803 ),
    'data_Run2016F_HIPM'    : tnpSample('SingleElectron_Run2016F',    latinoUL_16_HIPM + 'SingleElectron_Run2016F-HIPM_UL2016-v1.root', lumi = 2.718 ),
}

nanoUL_16_noHIPM = {
    'DYJetsToLL_M-50-LO'   : tnpSample('DYJetsToLL_M-50-LO',      latinoUL_16_noHIPM + 'DYJetsToLL_M-50-LO.root', isMC = True, nEvts =  -1 ),
    'DYJetsToLL_M-50'      : tnpSample('DYJetsToLL_M-50',         latinoUL_16_noHIPM + 'DYJetsToLL_M-50.root',    isMC = True, nEvts =  -1 ),
    'data_Run2016_noHIPM'  : tnpSample('SingleElectron',          latinoUL_16_noHIPM + 'SingleElectron.root',                    lumi = 16.812 ),
    'data_Run2016F_noHIPM' : tnpSample('SingleElectron_Run2016F', latinoUL_16_noHIPM + 'SingleElectron_Run2016F.root',           lumi = 0.419 ),
    'data_Run2016G_noHIPM' : tnpSample('SingleElectron_Run2016G', latinoUL_16_noHIPM + 'SingleElectron_Run2016G_UL2016-v1.root', lumi = 7.653 ),
    'data_Run2016H_noHIPM' : tnpSample('SingleElectron_Run2016H', latinoUL_16_noHIPM + 'SingleElectron_Run2016H_UL2016-v1.root', lumi = 8.740 ),
}

nanoUL_17 = {
    'DYJetsToLL_M-50-LO' : tnpSample('DYJetsToLL_M-50-LO',         latinoUL_17 + 'DYJetsToLL_M-50_LO.root',   isMC = True, nEvts =  -1 ),
    'DYJetsToLL_M-50'    : tnpSample('DYJetsToLL_M-50',            latinoUL_17 + 'DYJetsToLL_M-50_ext1.root', isMC = True, nEvts =  -1 ),
    'data_Run2017'       : tnpSample('SingleElectron_Run2017',     latinoUL_17 + 'SingleElectron.root',             lumi = 41.48 ),
    'data_Run2017BCDE'   : tnpSample('SingleElectron_Run2017BCDE', latinoUL_17 + 'SingleElectron_Run2017BCDE.root', lumi = 27.94 ),
    'data_Run2017B'      : tnpSample('SingleElectron_Run2017B',    latinoUL_17 + 'SingleElectron_Run2017B.root',    lumi = 4.803 ),
    'data_Run2017C'      : tnpSample('SingleElectron_Run2017C',    latinoUL_17 + 'SingleElectron_Run2017C.root',    lumi = 9.574 ),
    'data_Run2017D'      : tnpSample('SingleElectron_Run2017D',    latinoUL_17 + 'SingleElectron_Run2017D.root',    lumi = 4.248 ),
    'data_Run2017E'      : tnpSample('SingleElectron_Run2017E',    latinoUL_17 + 'SingleElectron_Run2017E.root',    lumi = 9.315 ),
    'data_Run2017F'      : tnpSample('SingleElectron_Run2017F',    latinoUL_17 + 'SingleElectron_Run2017F.root',    lumi = 13.540 ),
}

nanoUL_18 = {
    'DYJetsToLL_M-50-LO' : tnpSample('DYJetsToLL_M-50-LO', latinoUL_18 + 'DYJetsToLL_M-50_LO.root',   isMC = True, nEvts =  -1 ),
    'DYJetsToLL_M-50'    : tnpSample('DYJetsToLL_M-50',    latinoUL_18 + 'DYJetsToLL_M-50_ext1.root', isMC = True, nEvts =  -1 ),
    'data_Run2018'       : tnpSample('EGamma_Run2018',     latinoUL_18 + 'EGamma.root',          lumi = 59.83 ),
    'data_Run2018A'      : tnpSample('EGamma_Run2018A',    latinoUL_18 + 'EGamma_Run2018A.root', lumi = 14.03 ),
    'data_Run2018B'      : tnpSample('EGamma_Run2018B',    latinoUL_18 + 'EGamma_Run2018B.root', lumi = 7.067 ),
    'data_Run2018C'      : tnpSample('EGamma_Run2018C',    latinoUL_18 + 'EGamma_Run2018C.root', lumi = 6.899 ),
    'data_Run2018D'      : tnpSample('EGamma_Run2018D',    latinoUL_18 + 'EGamma_Run2018D.root', lumi = 31.84 ),
}

