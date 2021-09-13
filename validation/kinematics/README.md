# validating skimmed samples

## Luminosity

```
brilcalc lumi -b "STABLE BEAMS" \
              --normtag /cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags/normtag_PHYSICS.json \
              -u /fb \
              -i ../../skim/data/certs/Cert_294927-306462_13TeV_UL2017_Collisions17_GoldenJSON.txt
```

| Dataset | Recorded Lumi | Golden JSON |
| --- | --- | --- |
| UL2016 | | |
| UL2016 | | |
| UL2017 | 41.48 | Cert_294927-306462_13TeV_UL2017_Collisions17_GoldenJSON.txt |
| UL2018 | 59.83 | Cert_314472-325175_13TeV_Legacy2018_Collisions18_JSON.txt |

## Analysis weight

```
MC Weight = genWeight*baseW*puWeight*mcTrue*LUMI
```
