import os, sys, time
import subprocess as sp
from optparse import OptionParser

from data.dataset_def import dataset_config

cwd = os.getcwd()

usage = "usage: %prog [options]"
parser = OptionParser(usage)
parser.add_option("-d","--dataset", action="store", type="string", dest="dataset", default="latino17")
parser.add_option("-b","--batch", action="store_true", dest="batch", default=False)
parser.add_option("-t","--test", action="store_true", dest="test", default=False)
parser.add_option("-n","--nfile", action="store", type="int", dest="nfile", default=10)

(options, args) = parser.parse_args()

cwd = os.getcwd()
trun = time.time();

EOS="/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano"
dataset = options.dataset
location = "%s/data/filelists/%s" %(cwd,dataset)
batch = options.batch
test = options.test
nfile = options.nfile

output = "%s/results/%s" %(cwd,dataset)

def prepare( dataset_ ):
    
    out = []
    
    for camp in [ dataset_['data'] , dataset_['mc'] ]:
        addtnp = "DataTandP__addTnPEle" if "Run" in camp else "MCTandP__addTnPEle"
        path = "%s/%s/%s/" %( EOS , camp , addtnp )
        
        # infer samples
        samples = [ r.split('-UL')[0] if "Run" in r else r.split('__')[0].replace('M-','M-50') for r in sp.getoutput( "ls %s/* | xargs -n1 basename | tr -d '[:digit:]' | uniq" % path ).split("\n") ]
        
        for sample in samples:
            sample = sample.strip('nanoLatino_').replace( 'Run' , camp.split('_')[0] )
            print("Preparing textfile : %s" %sample )
            os.system( "ls %s/*%s* > %s/%s.txt" %( path , sample , location , sample ) )
            out.append( "%s/%s.txt" %( location , sample ) )
    return out
pass

def submit_script( sample_name__ , jobname_ , output_ , lumi_ ):
    outscript=jobname_.replace('.txt','.sh')
    outname=jobname_.split('/')[-1].split('.txt')[0]
    # condor
    with open( outscript , 'a') as script :
        script.write( '#!/bin/bash\n' )
        script.write( 'export X509_USER_PROXY=/afs/cern.ch/user/'+os.environ["USER"][:1]+'/'+os.environ["USER"]+'/.proxy\n' )
        script.write( 'voms-proxy-info\n' )
        #script.write( 'export SCRAM_ARCH=$SCRAM_ARCH\n' )
        script.write( 'export VO_CMS_SW_DIR=/cvmfs/cms.cern.ch\n' )
        script.write( 'source $VO_CMS_SW_DIR/cmsset_default.sh\n' )
        script.write( 'export HOME=%s\n' %os.environ['PWD'] )
        script.write( 'source /cvmfs/sft.cern.ch/lcg/views/setupViews.sh LCG_98python3 x86_64-centos7-gcc8-opt\n' ) # hardcoded
        #script.write( 'export CMSSW_BASE=%s\n' %os.environ['CMSSW_BASE'] )
        #script.write( 'echo $HOME\n' )
        #script.write( 'echo $CMSSW_BASE\n' )
        #script.write( 'cd $CMSSW_BASE/src\n' )
        #script.write( 'eval `scramv1 ru -sh`\n' )
        #script.write( 'ulimit -c 0\n' )
        script.write( 'cd $HOME\n' )
        #script.write( 'make\n')
        script.write( 'cd $TMPDIR\n' )
        script.write( 'pwd\n' )
        script.write( '$HOME/skim %s %s/%s.root %s\n' %( jobname_ , output_ , outname , lumi_ ) )
        script.close()
    os.system( 'chmod +x %s' %outscript )
        
    # submission script
    outscriptSub = outscript.replace( '.sh' , '.sub' )
    with open( outscriptSub , 'a' ) as script :
        script.write( 'executable              = %s\n' %outscript )
        # https://batchdocs.web.cern.ch/local/submit.html#job-flavours
        script.write( '+JobFlavour             = \"longlunch\"\n')
        # https://batchdocs.web.cern.ch/local/submit.html#resources-and-limits
        script.write( 'request_cpus            = 3\n' )
        script.write( 'output                  = %s.out\n' %outscript.replace('.sh' , '') )
        script.write( 'error                   = %s.err\n' %outscript.replace('.sh' , '') )
        script.write( 'log                     = %s.log\n' %outscript.replace('.sh','') )
        script.write( 'transfer_output_remaps  = \"%s.root=%s.root\"\n' %( sample_name__ , outscript.split('/')[-1].replace('.sh','') ) )
        script.write( 'queue\n' )
        script.close()
    if not test:
        os.system( 'condor_submit %s' %outscriptSub )
    else:
        print("--> prepare condor jobs")
    pass

def text_writer( split_line_ , output_ , sample_name_ , lumi_ ):
    for num , ichunck in enumerate(split_line_) :
        jobname = '%s/%s__job%s.txt' %( output_ , sample_name_ , num )
        f=open( jobname , 'w' )
        if batch : ichunck = map( lambda x : 'root://eoscms.cern.ch/'+x , ichunck )
        f.write( '\n'.join(ichunck) )
        submit_script( sample_name_ , jobname , output_ , lumi_ )
        f.close()
    pass

def prepare_batch( proctxt , sample_name , lumi , nfile ):
    # text file
    textfile = open( proctxt , 'r')
    Lines = [ itxt.replace('\n','') for itxt in textfile.readlines() ]
    split_line = [ Lines[ i:i+nfile ] for i in range(0, len(Lines), nfile) ]
    text_writer( split_line , output , sample_name , lumi )
    pass

def execute( sample_ , iproc_ , output_ , lumi_ ):
    cmd="./skim"
    if batch :
        prepare_batch( iproc_ , sample_ , lumi_ , nfile )
    else :
        trun = time.time();
        cmd+=" %s %s/%s.root %s" %( iproc_ , output_ , sample_ , lumi_  )
        tproc = time.time()
        if test:
            print(cmd)
        else:
            print(cmd)
            os.system(cmd)
            #os.system('gdb --args %s' %cmd)
            print("--- running on %s took : %.3f seconds (%.3f minutes) ---" % ( sample , (time.time() - tproc) , (time.time() - tproc)/60. ) )
            print("")
            print("--- Total run time : %.3f seconds (%.3f minutes) ---" % ( (time.time() - trun) , (time.time() - trun)/60. ) )
    pass
    
if __name__ == "__main__":

    # load data config 
    dataset_ = dataset_config[dataset]

    if not os.path.exists(output) : 
        os.system( "mkdir -p %s" %output   )
    else:
        os.system( "rm -rf %s" %output   )
        os.system( "mkdir -p %s" %output   )
    
    # make filelist, if exist, refreshing
    if not os.path.exists(location) : os.system( "mkdir -p %s" %location )
    samplelists = prepare( dataset_ )

    ##
    print("")
    print( "EO       : " , EOS      )
    print( "dataset  : " , dataset  )
    print( "location : " , location )
    print( "batch    : " , batch    ) 
    print( "nfile    : " , nfile    )
    print( "test     : " , test     )
    print("")

    os.system("make")
    if batch: os.system('voms-proxy-init -voms cms -valid 168:00')
    for iproc in samplelists:
        sample = iproc.split('/')[-1].split('.txt')[0]
        execute( sample , iproc , output , dataset_['lumi'] )
