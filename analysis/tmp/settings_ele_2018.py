#############################################################
########## General settings
#############################################################
# flag to be Tested
###
cutMissingInnerHits = 'el_mHits==0 '
cutLoose = '(( abs(el_sc_abseta) > 1.497 && (el_sieie < 0.03 ) && (el_1overEminus1overP < 0.014)) ||( abs(el_sc_abseta) < 1.497))'
cutDz = '(( abs(el_sc_abseta) > 1.497 && abs(el_dz) < 0.2 )||( abs(el_sc_abseta) < 1.497 && abs(el_dz) < 0.1 ))'
cutDxy = '(( abs(el_sc_abseta) > 1.497 && abs(el_dxy) < 0.1 )||( abs(el_sc_abseta) < 1.497 && abs(el_dxy) < 0.05 ))'
cutIso = '(el_reliso03 < 0.06)'

# flag to be Tested
flags = {
    'passingVeto94X'                    : '(passingVeto94X   == 1)',
    'passingLoose94X'                   : '(passingLoose94X  == 1)',
    'passingMedium94X'                  : '(passingMedium94X == 1)',
    'passingTight94X'                   : '(passingTight94X  == 1)',
    'passingMVA94Xwp80iso'              : '(passingMVA94Xwp80iso == 1)',
    'passingMVA94Xwp90iso'              : '(passingMVA94Xwp90iso == 1)',
    'passingMVA94Xwp80noiso'            : '(passingMVA94Xwp80noiso == 1)',
    'passingMVA94Xwp90noiso'            : '(passingMVA94Xwp90noiso == 1)',
    'passingMVA94Xwp90isoHWW'           : '({0}) && ({1}) && ({2}) && (passingVeto94X == 1) && (passingMVA94Xwp90iso == 1  )'.format(cutDxy,cutDz,cutLoose),
    'passingMVA94Xwp90isoSSHWW'         : '({0}) && ({1}) && ({2}) && (passingVeto94X == 1) && (el_3charge == 1) && (passingMVA94Xwp90iso == 1  )'.format(cutDxy,cutDz,cutLoose),

    'passingMVA94Xwp90isoHWWiso0p06'    : '({0}) && ({1}) && ({2}) && ({3}) && (passingMedium94X == 1) && (passingMVA94Xwp90iso == 1  )'.format(cutDxy,cutDz,cutLoose,cutIso),
    }

# baseOutDir = 'results/2017Data/tnpEleID/runB_HWW/'
# baseOutDir = 'results/2017Data/tnpEleID/runC_HWW/'
# baseOutDir = 'results/2017Data/tnpEleID/runD_HWW/'
# baseOutDir = 'results/2017Data/tnpEleID/runE_HWW/'
# baseOutDir = 'results/2017Data/tnpEleID/runF_HWW/'

#############################################################
########## samples definition  - preparing the samples
#############################################################
### samples are defined in etc/inputs/tnpSampleDef.py
### not: you can setup another sampleDef File in inputs
import etc.inputs.tnpSampleDef as tnpSamples
tnpTreeDir = 'tnpEleIDs'

samplesDef = {
    # 'data'   : tnpSamples.Data2018_94X['data_Run2018A'].clone(),
    # 'data'   : tnpSamples.Data2018_94X['data_Run2018B'].clone(),
    # 'data'   : tnpSamples.Data2018_94X['data_Run2018C'].clone(),
    'data'   : tnpSamples.Data2018_94X['data_Run2018D'].clone(),
    'mcNom'  : tnpSamples.Data2018_94X['DY_madgraph_Data2018'].clone(),
    'mcAlt'  : tnpSamples.Data2018_94X['DY_amcatnlo_Data2018'].clone(),
    'tagSel' : tnpSamples.Data2018_94X['DY_madgraph_Data2018'].clone(),
}

## can add data sample easily
# samplesDef['data'].add_sample( tnpSamples.Data2018_94X['data_Run2018B'] )
# samplesDef['data'].add_sample( tnpSamples.Data2018_94X['data_Run2018C'] )
# samplesDef['data'].add_sample( tnpSamples.Data2018_94X['data_Run2018D'] )

## some sample-based cuts... general cuts defined here after
## require mcTruth on MC DY samples and additional cuts
## all the samples MUST have different names (i.e. sample.name must be different for all)
## if you need to use 2 times the same sample, then rename the second one
#samplesDef['data'  ].set_cut('run >= 273726')
samplesDef['data' ].set_tnpTree(tnpTreeDir)
if not samplesDef['mcNom' ] is None: samplesDef['mcNom' ].set_tnpTree(tnpTreeDir)
if not samplesDef['mcAlt' ] is None: samplesDef['mcAlt' ].set_tnpTree(tnpTreeDir)
if not samplesDef['tagSel'] is None: samplesDef['tagSel'].set_tnpTree(tnpTreeDir)

if not samplesDef['mcNom' ] is None: samplesDef['mcNom' ].set_mcTruth()
if not samplesDef['mcAlt' ] is None: samplesDef['mcAlt' ].set_mcTruth()
if not samplesDef['tagSel'] is None: samplesDef['tagSel'].set_mcTruth()
if not samplesDef['tagSel'] is None:
    samplesDef['tagSel'].rename('mcAltSel_DY_madgraph')
    samplesDef['tagSel'].set_cut('tag_Ele_pt > 37') #canceled non trig MVA cut

## set MC weight, simple way (use tree weight) 
#weightName = 'totWeight'
#if not samplesDef['mcNom' ] is None: samplesDef['mcNom' ].set_weight(weightName)
#if not samplesDef['mcAlt' ] is None: samplesDef['mcAlt' ].set_weight(weightName)
#if not samplesDef['tagSel'] is None: samplesDef['tagSel'].set_weight(weightName)

## set MC weight, can use several pileup rw for different data taking periods
# weightName = 'weights_2018_runABCD.totWeight'
weightName = 'weights_2018_runA.totWeight'
# weightName = 'weights_2018_runB.totWeight'
# weightName = 'weights_2018_runC.totWeight'
# weightName = 'weights_2018_runD.totWeight'

if not samplesDef['mcNom' ] is None: samplesDef['mcNom' ].set_weight(weightName)
if not samplesDef['mcAlt' ] is None: samplesDef['mcAlt' ].set_weight(weightName)
if not samplesDef['tagSel'] is None: samplesDef['tagSel'].set_weight(weightName)
if not samplesDef['mcNom' ] is None: samplesDef['mcNom' ].set_puTree('/eos/cms/store/group/phys_egamma/swmukher/NtupleForRecoSF/Rereco2018Data_Autumn18MC_AOD/DY_MG_ele.pu.puTree.root')
if not samplesDef['mcAlt' ] is None: samplesDef['mcAlt' ].set_puTree('/eos/cms/store/group/phys_egamma/swmukher/NtupleForRecoSF/Rereco2018Data_Autumn18MC_AOD/DY_powheg_ele.pu.puTree.root')
if not samplesDef['tagSel'] is None: samplesDef['tagSel'].set_puTree('/eos/cms/store/group/phys_egamma/swmukher/NtupleForRecoSF/Rereco2018Data_Autumn18MC_AOD/DY_MG_ele.pu.puTree.root')


#############################################################
########## bining definition  [can be nD bining]
#############################################################
   #{ #'var' : 'el_sc_eta' , 'type': 'float', 'bins': [-2.5,-2.0,-1.566,-1.4442, -0.8, 0.0, 0.8, 1.4442, 1.566, 2.0, 2.5] },
biningDef = [
   { 'var' : 'el_pt' , 'type': 'float', 'bins': [10,500] },
   { 'var' : 'el_sc_abseta' , 'type': 'float', 'bins': [0.,1.5,2.5] },
   { 'var' : 'event_nPV' , 'type': 'float', 'bins': [0,5,10,15,20,30,40,60,100] },
]

#############################################################
########## Cuts definition for all samples
#############################################################
### cut
cutBase   = 'tag_Ele_pt > 30 && abs(tag_sc_eta) < 2.17 && el_q*tag_Ele_q < 0'

# can add addtionnal cuts for some bins (first check bin number using tnpEGM --checkBins)
additionalCuts = { 
    0 : 'tag_Ele_trigMVA > 0.92 && sqrt( 2*event_met_pfmet*tag_Ele_pt*(1-cos(event_met_pfphi-tag_Ele_phi))) < 45',
    1 : 'tag_Ele_trigMVA > 0.92 && sqrt( 2*event_met_pfmet*tag_Ele_pt*(1-cos(event_met_pfphi-tag_Ele_phi))) < 45',
    2 : 'tag_Ele_trigMVA > 0.92 && sqrt( 2*event_met_pfmet*tag_Ele_pt*(1-cos(event_met_pfphi-tag_Ele_phi))) < 45',
    3 : 'tag_Ele_trigMVA > 0.92 && sqrt( 2*event_met_pfmet*tag_Ele_pt*(1-cos(event_met_pfphi-tag_Ele_phi))) < 45',
    4 : 'tag_Ele_trigMVA > 0.92 && sqrt( 2*event_met_pfmet*tag_Ele_pt*(1-cos(event_met_pfphi-tag_Ele_phi))) < 45',
    5 : 'tag_Ele_trigMVA > 0.92 && sqrt( 2*event_met_pfmet*tag_Ele_pt*(1-cos(event_met_pfphi-tag_Ele_phi))) < 45',
    6 : 'tag_Ele_trigMVA > 0.92 && sqrt( 2*event_met_pfmet*tag_Ele_pt*(1-cos(event_met_pfphi-tag_Ele_phi))) < 45',
    7 : 'tag_Ele_trigMVA > 0.92 && sqrt( 2*event_met_pfmet*tag_Ele_pt*(1-cos(event_met_pfphi-tag_Ele_phi))) < 45',
    8 : 'tag_Ele_trigMVA > 0.92 && sqrt( 2*event_met_pfmet*tag_Ele_pt*(1-cos(event_met_pfphi-tag_Ele_phi))) < 45',
    9 : 'tag_Ele_trigMVA > 0.92 && sqrt( 2*event_met_pfmet*tag_Ele_pt*(1-cos(event_met_pfphi-tag_Ele_phi))) < 45'
}

#### or remove any additional cut (default)
#additionalCuts = None

#############################################################
########## fitting params to tune fit by hand if necessary
#############################################################
tnpParNomFit = [
    "meanP[-0.0,-5.0,5.0]","sigmaP[0.9,0.5,5.0]",
    "meanF[-0.0,-5.0,5.0]","sigmaF[0.9,0.5,5.0]",
    "acmsP[60.,50.,80.]","betaP[0.05,0.01,0.08]","gammaP[0.1, -2, 2]","peakP[90.0]",
    "acmsF[60.,50.,80.]","betaF[0.05,0.01,0.08]","gammaF[0.1, -2, 2]","peakF[90.0]",
    ]

tnpParAltSigFit = [
    "meanP[-0.0,-5.0,5.0]","sigmaP[1,0.7,6.0]","alphaP[2.0,1.2,3.5]" ,'nP[3,-5,5]',"sigmaP_2[1.5,0.5,6.0]","sosP[1,0.5,5.0]",
    "meanF[-0.0,-5.0,5.0]","sigmaF[2,0.7,15.0]","alphaF[2.0,1.2,3.5]",'nF[3,-5,5]',"sigmaF_2[2.0,0.5,6.0]","sosF[1,0.5,5.0]",
    "acmsP[60.,50.,75.]","betaP[0.04,0.01,0.06]","gammaP[0.1, 0.005, 1]","peakP[90.0]",
    "acmsF[60.,50.,75.]","betaF[0.04,0.01,0.06]","gammaF[0.1, 0.005, 1]","peakF[90.0]",
    ]
     
tnpParAltBkgFit = [

    "meanP[-0.0,-5.0,5.0]","sigmaP[0.9,0.5,5.0]",
    "meanF[-0.0,-5.0,5.0]","sigmaF[0.9,0.5,5.0]",
    "alphaP[0.,-5.,5.]",
    "alphaF[0.,-5.,5.]",
    ]
        
