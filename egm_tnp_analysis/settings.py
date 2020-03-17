#############################################################
########## General settings
#############################################################
import os

# flag to be Tested
flag = {
    #'passingMvaFall17V1Iso_WP90_SS' : '(passingMvaFall17V1Iso_WP90_SS == 1)',
    'passingMvaFall17V1Iso_WP90' : '(passingMvaFall17V1Iso_WP90 == 1)',
    #'passingVeto' : '(passingVeto == 1)' ,
    #'passingLoose': '(passingLoose == 1)',
    #'passingMedium': '(passingMedium == 1)',
    #'passingTight': '(passingTight == 1)',
    #'passingMvaTTH': '(passingMvaTTH == 1)',
}

#baseOutDir = 'results/runB/%s/' % (flag.keys()[0])
baseOutDir = '%s/results/Legacy2016/tnpEleID/runAll' %os.getcwd()

#############################################################
########## samples definition  [can be nD bining]
#############################################################
#tnpTreeDir = 'GsfElectronToEleID'
tnpTreeDir = 'fitter_tree'
weightName = 'weight'

### MANDATORY nEvts in data = -1 // mcTruth will require mc Matching
## some sample based cuts... general cuts defined here after
cutAltSel = 'tag_Ele_pt > 35'
#&& tag_Ele_nonTrigMVA > 0.90'
cutData   = 'run >= 273726'
#cutData    = None

#* samplesDef: these are the main info
#  - data: data ntuple
#  - mcNom: nominal MC sample
#  - mcAlt: MC for generator syst
#  - tagSel: usually same as nominal MC + different base cuts: check the tag selection syst

samplesDef = {
    'data'     : { 'name' : 'SingleElectron' , 'mcTruth' : False, 'nEvts': -1, 'cut' : cutData,
                   'path' : '%s/results/latinov5_17/SingleElectron_Run2017.root' %os.getcwd() },
    'mcNom'    : { 'name' : 'DYJetsToLL_M-50-LONom' , 'mcTruth' : True , 'nEvts': -1, 'cut' : None,
                   'path' : '%s/results/latinov5_17/DYJetsToLL_M-50-LO.root' %os.getcwd() },
    'mcAlt'   : { 'name' : 'DYJetsToLL_M-50-LOAltSel' , 'mcTruth' : True , 'nEvts' : -1, 'cut' : None,
                  'path' : '%s/results/latinov5_17/DYJetsToLL_M-50_ext1.root' %os.getcwd() },
    'tagSel' : { 'name' : 'DYJetsToLL_M-50-LOtagSel' , 'mcTruth' : True , 'nEvts': -1, 'cut' : cutAltSel,
                 'path' : '%s/results/latinov5_17/DYJetsToLL_M-50-LO.root' %os.getcwd() },
}

### lumi in /fb
lumi = 35.867

#############################################################
########## bining definition  [can be nD bining]
#############################################################
biningDef = [
    { 'var' : 'probe_Ele_eta' , 'type': 'float', 'bins': [ -2.5 , -2.0 , -1.566 , -1.442 , -0.8 , 0.0 , 0.8 , 1.442 , 1.566 , 2.0 , 2.5 ] },
    { 'var' : 'probe_Ele_pt' , 'type': 'float', 'bins': [ 10 , 20 , 35 , 50 , 90 , 150 , 500 ] },
]

#############################################################
########## Cuts definition for all samples
#############################################################
### cut
cutBase   = 'tag_Ele_pt > 30 && abs(tag_Ele_eta) < 2.17 && tag_Ele_q*probe_Ele_q < 0'

# can add addtionnal cuts for some bins (first check bin number using tnpEGM --checkBins)
additionalCuts = { 
#    0 : 'tag_Ele_trigMVA > 0.92 && sqrt( 2*event_met_pfmet*tag_Ele_pt*(1-cos(event_met_pfphi-tag_Ele_phi))) < 45',
#    1 : 'tag_Ele_trigMVA > 0.92 && sqrt( 2*event_met_pfmet*tag_Ele_pt*(1-cos(event_met_pfphi-tag_Ele_phi))) < 45',
#    2 : 'tag_Ele_trigMVA > 0.92 && sqrt( 2*event_met_pfmet*tag_Ele_pt*(1-cos(event_met_pfphi-tag_Ele_phi))) < 45',
#    3 : 'tag_Ele_trigMVA > 0.92 && sqrt( 2*event_met_pfmet*tag_Ele_pt*(1-cos(event_met_pfphi-tag_Ele_phi))) < 45',
#    4 : 'tag_Ele_trigMVA > 0.92 && sqrt( 2*event_met_pfmet*tag_Ele_pt*(1-cos(event_met_pfphi-tag_Ele_phi))) < 45',
#    5 : 'tag_Ele_trigMVA > 0.92 && sqrt( 2*event_met_pfmet*tag_Ele_pt*(1-cos(event_met_pfphi-tag_Ele_phi))) < 45',
#    6 : 'tag_Ele_trigMVA > 0.92 && sqrt( 2*event_met_pfmet*tag_Ele_pt*(1-cos(event_met_pfphi-tag_Ele_phi))) < 45',
#    7 : 'tag_Ele_trigMVA > 0.92 && sqrt( 2*event_met_pfmet*tag_Ele_pt*(1-cos(event_met_pfphi-tag_Ele_phi))) < 45',
#    8 : 'tag_Ele_trigMVA > 0.92 && sqrt( 2*event_met_pfmet*tag_Ele_pt*(1-cos(event_met_pfphi-tag_Ele_phi))) < 45',
#    9 : 'tag_Ele_trigMVA > 0.92 && sqrt( 2*event_met_pfmet*tag_Ele_pt*(1-cos(event_met_pfphi-tag_Ele_phi))) < 45'
}

#### or remove any additional cut (default)
#additionalCuts = None

#############################################################
########## fitting params to tune fit by hand if necessary
#############################################################
tnpParNomFit = [
    "meanP[-0.0,-5.0,5.0]","sigmaP[0.5,0.1,5.0]",
    "meanF[-0.0,-5.0,5.0]","sigmaF[0.5,0.1,5.0]",
    "acmsP[60.,50.,80.]","betaP[0.05,0.01,0.08]","gammaP[0.1, 0, 1]","peakP[90.0]",
    "acmsF[60.,50.,80.]","betaF[0.05,0.01,0.08]","gammaF[0.1, 0, 1]","peakF[90.0]",
    ]

tnpParAltSigFit = [
    "meanP[-0.0,-5.0,5.0]","sigmaP[1,0.7,6.0]","alphaP[2.0,1.2,3.5]" ,'nP[3,0.05,5]',"sigmaP_2[1.5,0.5,6.0]",
    "meanF[-0.0,-5.0,5.0]","sigmaF[2,0.7,15.0]","alphaF[2.0,1.2,3.5]",'nF[3,0.05,5]',"sigmaF_2[2.0,0.5,6.0]",
    "acmsP[60.,50.,75.]","betaP[0.04,0.01,0.06]","gammaP[0.1, 0.005, 1]","peakP[90.0]",
    "acmsF[60.,50.,75.]","betaF[0.04,0.01,0.06]","gammaF[0.1, 0.005, 1]","peakF[90.0]",
    ]
     
tnpParAltBkgFit = [
    "meanP[-0.0,-5.0,5.0]","sigmaP[0.5,0.1,5.0]",
    "meanF[-0.0,-5.0,5.0]","sigmaF[0.5,0.1,5.0]",
    "alphaP[0.,-5.,5.]",
    "alphaF[0.,-5.,5.]",
    ]
        
