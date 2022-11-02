# NanoTnP
minimalistic TnP package on nanoAod.

## quick run
### source environment
 
```
cd ..
source lxplus_setup.sh
cd skim
```

### run locally
what does it do? :
 - "refresh" textfile (some UL might still be in production)
 - compile skim code 
 - execute skim


```
# default dataset --> latino17
python skim.py
# use -d for other dataset
python skim.py -d latino18
```
the dataset information is defined here
```
skim/data/dataset_def.py
```

### dry run locally
```
python skim.py -t
```

### submit job 
```
python skim.py -b

## after jobs are done, run merge
./scripts/merge.sh

## merged output in 
./results/latino17/merged
```

### dry run submit job
```
python skim.py -b -t
```

### Output ntuple in 
```
skim/results
```

## Customizing the code

The code skim.cxx is written in c++, using RDataframe (DF) as a central object. Actions on DF includes __filtering__, __defining column__ and __dropping column__
- filtering perform on event-level; particle-level filter can be achieve with Lambda function.

### TnP workflow
define in [skim/skim.cxx](https://github.com/dittmer/NanoTnP/blob/master/skim/skim.cxx#L53), take ```df``` and ```cfg```, cfg defined in [skim/interface/helper.h](https://github.com/dittmer/NanoTnP/blob/master/skim/interface/helper.h#L164)

the TnP workflow defined in [skim/interface/tnp.h](https://github.com/dittmer/NanoTnP/blob/master/skim/interface/workflow.h#L50-L93), with output variables defined in [skim/interface/tnp.h](https://github.com/dittmer/NanoTnP/blob/master/skim/interface/tnp.h#L6-L47)

### Adding MVAs

In addition to the TnP variables taken from the input rootfiles, new variables can be defined on the fly. This is useful when adding MVA discriminants for IDs not yet included in nanoAOD. The HWW MVA and UL ttH MVA scores are added to the TnP tree at [skim/skim.cxx](https://github.com/dittmer/NanoTnP/blob/master/skim/skim.cxx#L56-L78). The paths to the weights files are currently hardcoded, in order to support job submission to condor -- these will need to be fixed. The weights are read using TMVA::Reader, and the interface between TMVA::Reader and RDataFrame is handled by dedicated classes [skim/interface/tmva_classical.h](https://github.com/dittmer/NanoTnP/blob/master/skim/interface/tmva_classical.h) and [skim/interface/tmva_tth.h](https://github.com/dittmer/NanoTnP/blob/master/skim/interface/tmva_tth.h) for the HWW MVA and ttH MVA, respectively.

## Ultra Legacy datasets

Central processing used to produce UL TnP: ```MCTandP__addTnPEle```

 - [T&P Skims: MCTandP](https://github.com/latinos/LatinoAnalysis/blob/76e7c4b93aa5f056c92440d4e8d24e7de749c8fe/NanoGardener/python/framework/Steps_cfg.py#L1254-L1264)
 - [T&P Module: addTnPEle](https://github.com/latinos/LatinoAnalysis/blob/master/NanoGardener/python/modules/addTnpTree.py)
