import os, sys, time

cwd = os.getcwd()
trun = time.time();

dirs = "%s/data/filelists" %cwd
output = "%s" %cwd

dataset_config = {
    'latinov8_16_106X' : {
        "lumi" : "35.867",
        "filelist"  : [],
    },
    "latinov8_17_106X" : {
        "lumi" : "41.35",
        "filelist"  : [ 'DYJetsToLL_M-50.txt', 'SingleElectron.txt' ],
    },
    "latinov8_18_106X" : {
        "lumi" : "59.74",
        "filelist"  : [],
    },
}

def skim(dataset):

    global output
    
    # load data config
    dataset_ = dataset_config[dataset]
    output += "/out/%s" %dataset

    if not os.path.exists(output): 
        os.system("rm -rf %s" %output)
        os.system("mkdir -p %s" %output)
    print('dirs :', dirs)
    samplelists = [ "%s/%s" %(dirs,itxt) for itxt in dataset_['filelist'] ]\

    ## compile
    os.system("make clean")
    os.system("make")
    ###
    for iproc in samplelists:
        sample = iproc.split('/')[-1].split('.txt')[0]
        # process-in ; process-out ; sample ; lumi
        cmd="./skim"; cmd+=" %s %s/%s.root %s" %( iproc , output , sample , dataset_['lumi'] )
        tproc = time.time()
        print(cmd)
        #os.system('gdb --args %s' %cmd)
        os.system(cmd)
        print("--- running on %s took : %.3f seconds (%.3f minutes) ---" % ( sample , (time.time() - tproc) , (time.time() - tproc)/60. ) )
    print("--- Total run time : %.3f seconds (%.3f minutes) ---" % ( (time.time() - trun) , (time.time() - trun)/60. ) )
    pass
    
if __name__ == "__main__":
    
    skim('latinov8_17_106X')
