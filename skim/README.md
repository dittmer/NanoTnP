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
define in [skim/skim.cxx](https://github.com/SiewYan/NanoTnP/blob/master/skim/skim.cxx#L49), take ```df``` and ```cfg```, cfg defined in [skim/interface/helper.h](https://github.com/SiewYan/NanoTnP/blob/master/skim/interface/helper.h#L164-L172)

the TnP workflow defined in [skim/interface/workflow.h](https://github.com/SiewYan/NanoTnP/blob/master/skim/interface/workflow.h#L40-L83), with output variables defined in [skim/interface/workflow.h](https://github.com/SiewYan/NanoTnP/blob/master/skim/interface/workflow.h#L6-L38)

## Ultra Legacy datasets

Central processing used to produce UL TnP: ```MCTandP__addTnPEle```

 - [T&P Skims: MCTandP](https://github.com/latinos/LatinoAnalysis/blob/76e7c4b93aa5f056c92440d4e8d24e7de749c8fe/NanoGardener/python/framework/Steps_cfg.py#L1079-L1088)
 - [T&P Module: addTnPEle](https://github.com/latinos/LatinoAnalysis/blob/master/NanoGardener/python/modules/addTnpTree.py)
