# NanoTnP
minimalistic TnP package on nanoAod.

## Tag-and-Probe Methodology 
The package produces the necessary ntuple format for identification efficiency of electrons. 
The tag is a well identified and isolated electron which pass an electron trigger to increase the purity. Once the tag electron is selected then we look for another object which is reconstructed as electron. After passing some kinematical selection, we get the total probe selection. The invariant mass of the pair is then reconstructed from Tag and Probe electron and a Z boson mass window is imposed. We ask the Probe to pass the ID working point and compute the efficiency. Once we obtain the efficiency for both DATA and MC then we can compute the scale factor, which is the ratio of efficiencies of DATA and MC.

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
 ROOT > 6.18 ; python3.6.5 ; on lxplus7
 
 ```
 source /cvmfs/sft.cern.ch/lcg/views/LCG_95apython3/x86_64-centos7-gcc7-opt/setup.sh
 ```
