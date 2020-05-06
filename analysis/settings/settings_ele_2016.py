#############################################################
########## General settings
#############################################################
# flag to be Tested
cutMissingInnerHits = 'probe_Ele_mHits==0 '
cutdz = '(( abs(probe_sc_eta) > 1.497 && abs(probe_Ele_dz) < 0.2 )||( abs(probe_sc_eta) < 1.497 && abs(probe_Ele_dz) < 0.1 ))'
cutd0 = '(( abs(probe_sc_eta) > 1.497 && abs(probe_Ele_dxy) < 0.1 )||( abs(probe_sc_eta) < 1.497 && abs(probe_Ele_dxy) < 0.05 ))'
looseDef  = 'passingHLTsafe ==1 && '+ cutMissingInnerHits +' && '+ cutdz +' && '+ cutd0
cutpass80 = '(( abs(probe_sc_eta) < 0.8 && probe_Ele_nonTrigMVA > %f ) ||  ( abs(probe_sc_eta) > 0.8 && abs(probe_sc_eta) < 1.479&& probe_Ele_nonTrigMVA > %f ) || ( abs(probe_sc_eta) > 1.479 && probe_Ele_nonTrigMVA > %f ) )' % (0.967083,0.929117,0.726311)
cutpass90 = '(( abs(probe_sc_eta) < 0.8 && probe_Ele_nonTrigMVA > %f ) ||  ( abs(probe_sc_eta) > 0.8 && abs(probe_sc_eta) < 1.479&& probe_Ele_nonTrigMVA > %f ) || ( abs(probe_sc_eta) > 1.479 && probe_Ele_nonTrigMVA > %f ) )' % (0.913286,0.805013,0.358969)

effArea = '(abs(probe_sc_eta)>2.4)*0.2393 || (abs(probe_sc_eta)>2.3)*0.1937 ||  (abs(probe_sc_eta)>2.2)*0.1635 ||  (abs(probe_sc_eta)>2.0)*0.1230 ||  (abs(probe_sc_eta)>1.479)*0.1213 ||  (abs(probe_sc_eta)>1.0)*1.715 || (abs(probe_sc_eta)>0)*0.1703'

cutIso15Barrel = '((abs(probe_sc_eta) < 1.479) && (probe_Ele_chIso + TMath::Max(0.0,probe_Ele_neuIso + probe_Ele_phoIso -(event_rho)*({0})))/probe_Ele_pt < {1})'.format(effArea, 0.0354)
cutIso15Endcap = '((abs(probe_sc_eta) > 1.479) && (probe_Ele_chIso + TMath::Max(0.0,probe_Ele_neuIso + probe_Ele_phoIso -(event_rho)*({0})))/probe_Ele_pt < {1})'.format(effArea, 0.0646)
cutIso16Barrel = '((abs(probe_sc_eta) < 1.479) && (probe_Ele_chIso + TMath::Max(0.0,probe_Ele_neuIso + probe_Ele_phoIso -(event_rho)*({0})))/probe_Ele_pt < {1})'.format(effArea, 0.0588)
cutIso16Endcap = '((abs(probe_sc_eta) > 1.479) && (probe_Ele_chIso + TMath::Max(0.0,probe_Ele_neuIso + probe_Ele_phoIso -(event_rho)*({0})))/probe_Ele_pt < {1})'.format(effArea, 0.0571)
passHWW = 'passingTight80X  == 1 && passingHLTsafe == 1 && probe_Ele_3charge == 1 && probe_Ele_mHits < 1 && ((abs(probe_sc_eta) < 1.479 && abs(probe_Ele_dxy) < 0.05 && abs(probe_Ele_dz) < 0.1) ||      (abs(probe_sc_eta) >= 1.479 && abs(probe_Ele_dxy) < 0.1 && abs(probe_Ele_dz) < 0.2))'

# flag to be Tested
flags = {
    'passingVeto'       : '({0}) && (passingVeto   == 1)'.format(looseDef),
    'passingLoose'      : '({0}) && (passingLoose  == 1)'.format(looseDef),
    'passingMedium'     : '({0}) && (passingMedium == 1)'.format(looseDef),
    'passingTight'      : '({0}) && (passingTight  == 1)'.format(looseDef),
    'passingVeto80X'    : '({0}) && (passingVeto80X   == 1)'.format(looseDef),
    'passingLoose80X'   : '({0}) && (passingLoose80X  == 1)'.format(looseDef),
    'passingMedium80X'  : '({0}) && (passingMedium80X == 1)'.format(looseDef),
    'passingTight80X'   : '({0}) && (passingTight80X  == 1)'.format(looseDef),
    'passingTight80XTriChg'  : '({0}) && (passingTight80X  == 1) && (probe_Ele_3charge == 1)'.format(looseDef),
    'passingMVAwp80'    : '({0} && {1})'.format(looseDef,cutpass80),
    'passingMVAwp90'    : '({0} && {1})'.format(looseDef,cutpass90),
    'passingMVA80Xwp80'             : '({0}) && (passingMVA80Xwp80 == 1)'.format(looseDef),
    'passingMVA80Xwp90'             : '({0}) && (passingMVA80Xwp90 == 1)'.format(looseDef),
    'passingMVA80Xwp80Iso15'        : '({0}) && (passingMVA80Xwp80 == 1) && ( {1} || {2} )'.format(looseDef,cutIso15Barrel,cutIso15Endcap),
    'passingMVA80Xwp90Iso15'        : '({0}) && (passingMVA80Xwp90 == 1) && ( {1} || {2} )'.format(looseDef,cutIso15Barrel,cutIso15Endcap),
    'passingMVA80Xwp80Iso16'        : '({0}) && (passingMVA80Xwp80 == 1) && ( {1} || {2} )'.format(looseDef,cutIso16Barrel,cutIso16Endcap),
    'passingMVA80Xwp90Iso16'        : '({0}) && (passingMVA80Xwp90 == 1) && ( {1} || {2} )'.format(looseDef,cutIso16Barrel,cutIso16Endcap),
    'passingHWW'                    : passHWW
    }

# baseOutDir = 'results/Moriond17/tnpEleID/run2016/'
# baseOutDir = 'results/Moriond17/tnpEleID/run2016_POGd0dz_tightToLoose/'
# baseOutDir = 'results/Moriond17/tnpEleID/run2016_POGd0dz/'
# baseOutDir = 'results/Moriond17/tnpEleID/run2016_POGd0dzFixmcAlt/'
# baseOutDir = 'results/Moriond17/tnpEleID/run2016_fineEta/'
# baseOutDir = 'results/Moriond17/tnpEleID/run2016BCD/'
# baseOutDir = 'results/Moriond17/tnpEleID/run2016EF/'
# baseOutDir = 'results/Moriond17/tnpEleID/run2016GH/'
# baseOutDir = 'results/Moriond17/tnpEleID/run2016_noSCEtafinerBin/'
baseOutDir = 'results/Moriond17/tnpEleID/run2016_EvenFineEta/'
# baseOutDir = 'results/Moriond17/tnpEleID/run2016_veryFineEta/'
# baseOutDir = 'results/Moriond17/tnpEleID/run2016_noSCEtaBin/'

#############################################################
########## samples definition  - preparing the samples
#############################################################
### samples are defined in etc/inputs/tnpSampleDef.py
### not: you can setup another sampleDef File in inputs
import etc.inputs.tnpSampleDef as tnpSamples
tnpTreeDir = 'GsfElectronToEleID'

samplesDef = {
    # 'data'   : tnpSamples.Moriond17_80X['data_Run2016G'].clone(),
    # 'data'   : tnpSamples.Moriond17_80X['data_Run2016E'].clone(),
    'data'   : tnpSamples.Moriond17_80X['data_Run2016B'].clone(),
    'mcNom'  : tnpSamples.Moriond17_80X['DY_madgraph_Winter17'].clone(),
   'mcAlt'  : tnpSamples.Moriond17_80X['DY_amcatnlo_Winter17'].clone(),
    # 'mcAlt'  : tnpSamples.Moriond17_80X['DYee_powheg_Winter17'].clone(),
    'tagSel' : tnpSamples.Moriond17_80X['DY_madgraph_Winter17'].clone(),
}
## can add data sample easily
samplesDef['data'].add_sample( tnpSamples.Moriond17_80X['data_Run2016H'] )
samplesDef['data'].add_sample( tnpSamples.Moriond17_80X['data_Run2016G'] )
samplesDef['data'].add_sample( tnpSamples.Moriond17_80X['data_Run2016F'] )
samplesDef['data'].add_sample( tnpSamples.Moriond17_80X['data_Run2016E'] )
samplesDef['data'].add_sample( tnpSamples.Moriond17_80X['data_Run2016D'] )
samplesDef['data'].add_sample( tnpSamples.Moriond17_80X['data_Run2016C'] )

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
    samplesDef['tagSel'].set_cut('tag_Ele_pt > 27  && tag_Ele_nonTrigMVA > 0.95')
    # samplesDef['tagSel'].set_cut('tag_Ele_pt > 35  && tag_Ele_nonTrigMVA > 0.95')
    # samplesDef['tagSel'].set_cut('tag_Ele_pt > 35  && tag_Ele_nonTrigMVA80X > 0.95')

## set MC weight, simple way (use tree weight) 
#weightName = 'totWeight'
#if not samplesDef['mcNom' ] is None: samplesDef['mcNom' ].set_weight(weightName)
#if not samplesDef['mcAlt' ] is None: samplesDef['mcAlt' ].set_weight(weightName)
#if not samplesDef['tagSel'] is None: samplesDef['tagSel'].set_weight(weightName)

## set MC weight, can use several pileup rw for different data taking periods
#weightName = 'weights_2016_runGH.totWeight'
weightName = 'weights_2016_runAll.totWeight'
if not samplesDef['mcNom' ] is None: samplesDef['mcNom' ].set_weight(weightName)
if not samplesDef['mcAlt' ] is None: samplesDef['mcAlt' ].set_weight(weightName)
if not samplesDef['tagSel'] is None: samplesDef['tagSel'].set_weight(weightName)
if not samplesDef['mcNom' ] is None: samplesDef['mcNom' ].set_puTree('/wk_cms2/pchen/work/HWAnalysis/data/eos/cms/store/group/phys_egamma/tnp/80X/pu/Winter17/DY_madgraph_Winter17_ele.pu.puTree.root')
if not samplesDef['mcAlt' ] is None: samplesDef['mcAlt' ].set_puTree('/wk_cms2/pchen/work/HWAnalysis/data/eos/cms/store/group/phys_egamma/tnp/80X/pu/Winter17/DY_amcatnlo_Winter17_ele.pu.puTree.root')
# if not samplesDef['mcAlt' ] is None: samplesDef['mcAlt' ].set_puTree('/wk_cms2/pchen/work/HWAnalysis/data/eos/cms/store/group/phys_egamma/tnp/80X/pu/Winter17/DYee_powheg_Winter17_ele.pu.puTree.root')
if not samplesDef['tagSel'] is None: samplesDef['tagSel'].set_puTree('/wk_cms2/pchen/work/HWAnalysis/data/eos/cms/store/group/phys_egamma/tnp/80X/pu/Winter17/DY_madgraph_Winter17_ele.pu.puTree.root')


#############################################################
########## bining definition  [can be nD bining]
#############################################################
# With RECO SFbiningDef = [
biningDef = [
  # { 'var' : 'probe_sc_eta' , 'type': 'float', 'bins': [-2.5,-2.0,-1.566,-1.4442, -0.8, 0.0, 0.8, 1.4442, 1.566, 2.0, 2.5] },
  # { 'var' : 'probe_sc_eta' , 'type': 'float', 'bins': [-2.5,-2.48,-2.46,-2.44,-2.42,-2.4,-2.38,-2.36,-2.34,-2.32,-2.3,-2.25,-2.2,-2.15,-2.1,-2.05,-2.0,-1.566,-1.4442, -0.8, 0.0, 0.8, 1.4442, 1.566, 2.0,2.05, 2.1,2.15,2.2,2.25,2.3,2.32,2.34,2.36,2.38,2.4,2.42,2.44,2.46,2.48,2.5] },
  { 'var' : 'probe_sc_eta' , 'type': 'float', 'bins': [-2.5,-2.48,-2.46,-2.44,-2.42,-2.4,-2.38,-2.36,-2.34,-2.32,-2.3,-2.25,-2.2,-2.15,-2.1,-2.05,-2.0,-1.566,-1.4442, -0.8, 0.0, 0.8, 1.4442, 1.566, 2.0,2.05, 2.1,2.15,2.2,2.25,2.3,2.32,2.34,2.36,2.38,2.4,2.42,2.44,2.5] },
  # { 'var' : 'probe_Ele_eta' , 'type': 'float', 'bins': [-2.5,-2.0,-1.566,-1.4442, -0.8, 0.0, 0.8, 1.4442, 1.566, 2.0, 2.5] },
  # { 'var' : 'probe_sc_eta' , 'type': 'float', 'bins': [-2.5,-2.4,-2.3,-2.2,-2.1,-2.0,-1.566,-1.4442, -0.8, 0.0, 0.8, 1.4442, 1.566, 2.0, 2.1,2.2,2.3,2.4,2.5] },
  # { 'var' : 'probe_Ele_pt' , 'type': 'float', 'bins': [10,20,35,50,100,200,500] },
  { 'var' : 'probe_Ele_pt' , 'type': 'float', 'bins': [10,20,35,50,90,150,500] },
]

#############################################################
########## Cuts definition for all samples
#############################################################
### cut
cutBase  = 'tag_Ele_pt > 30 && abs(tag_Ele_eta) < 2.17 && probe_Ele_q*tag_Ele_q < 0'
# cutBase  = 'tag_Ele_pt > 30 && abs(tag_sc_eta) < 2.17 && probe_Ele_q*tag_Ele_q < 0'

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
        
