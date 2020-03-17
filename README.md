# NanoTnP
minimalistic TnP package on nanoAod.

## Tag-and-Probe Methodology 
The package produces the necessary ntuple format for identification efficiency of electrons. 
The tag is a well identified and isolated electron which pass an electron trigger to increase the purity. Once the tag electron is selected then we look for another object which is reconstructed as electron. After passing some kinematical selection, we get the total probe selection. The invariant mass of the pair is then reconstructed from Tag and Probe electron and a Z boson mass window is imposed. We ask the Probe to pass the ID working point and compute the efficiency. Once we obtain the efficiency for both DATA and MC then we can compute the scale factor, which is the ratio of efficiencies of DATA and MC.

## Brief technical aspect
The package constructed in plug-in-play modular nature, in EDM's language, a series of EDMProducer/EDMFilter parameterized with PSet before EDAnalyzer, string in cms sequence. Thanks to RDataframe, module is implemented as collective transformation fucntions (lambda function implemented to compute particle-based quantity) which lazily executed upon action. Benefits are, speed and better cpu utilization.

 - Samples used for TnP defined in `NanoTnP\data\latinov5_17`, `NanoTnP\data\latinov5_18`
 - Execution script:  `NanoTnP/nanotnp.py`
 - Output definition: `NanoTnP/interface/config.h`
 - Skim definition:   `NanoTnP/interface/skim.h`
 - Producer definition: `NanoTnP/interface/producer.h`
 - Sequence definition: `NanoTnP/nanotnp.cxx`
 
Default definition
 
| Object Definition | Description |
| --- | --- |
| Tag (data) | Match to last trigger filter, with stricter selection. |
| Tag (mc) | Match to Gen-level lepton which originating from Z boson, with stricter selection. |
| Probe (data,mc) | no matching |
| tag-probe pair (data,mc) | probe pair with tag within Z mass window, no charge check, randomize pair definition |
| Weight (data) | METFilter_DATA |
| Weight (mc) | LUMI * XSWeight * puWeight * GenLepMatch2l * METFilter_MC |
 
 
 ## Setup
 ROOT > 6.18 ; python3.7 ; on lxplus7
 
 ```
 source /cvmfs/sft-nightlies.cern.ch/lcg/views/dev3python3/latest/x86_64-centos7-gcc7-opt/setup.sh
 git clone git@github.com:SiewYan/NanoTnP.git
 cd NanoTnp
 # for latinov5 2017
 python nanotnp.py -y latinov5_17 
 # for latinov5 2018
 python nanotnp.py -y latinov5_18 
 ```
  - default at 10 cores for parallelism
  - 2017's approximately takes 10 mins
  - 2018's approximately takes 12 mins
  
  A `results` folder will appear containing the output.
 
 ## To-do
 - [ ] mcTruth for tag and probe.
 - [ ] missing trigger filter for 2017, `hltEle35noerWPTightGsfTrackIsoFilter`
