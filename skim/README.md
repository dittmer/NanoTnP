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
```

### dry run submit job
```
python skim.py -b -t
```

