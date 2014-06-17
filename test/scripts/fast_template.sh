#!/bin/bash

CONFIGFILE=arg_stepx_config_file
OUTPUTEOSDIR=arg_out_dir

CPATH=/afs/cern.ch/work/k/katkov/hiforest/HcalDQM/CMSSW_7_1_0_pre8/src
CWORK=/afs/cern.ch/work/k/katkov/hiforest/HcalDQM/CMSSW_7_1_0_pre8/test

cd ${CPATH}
export SCRAM_ARCH=slc6_amd64_gcc481
eval `scramv1 runtime -sh`
uname -a
#go back to pool area:
cd -

#cd ${CWORK}
cmsRun ${CWORK}/${CONFIGFILE} #>& ./cout.txt
echo 'cmsRun DONE'
ENDPATH=job`date --rfc-3339=seconds | sed 's|\ ||g'`
mkdir ${CWORK}/${ENDPATH}
pwd
#mv -f ./cout.txt ./detailedInfo.txt ${CWORK}/${ENDPATH} 
mv -f ./detailedInfo.txt ${CWORK}/${ENDPATH}
cp -f ${CWORK}/${CONFIGFILE} ${CWORK}/${ENDPATH}
echo 'Log Files Saved / Pool Cleaned'
for f1 in `find . -type f -name "*.root" -exec echo {} \; | sed 's|./||'`; do /afs/cern.ch/project/eos/installation/0.3.15/bin/eos.select cp ${f1} /eos/cms${OUTPUTEOSDIR}/${f1}; done
echo 'ROOT Files Saved'
sleep 30
find . -type f -name "*.root" -exec rm -f {} \; 
echo 'ROOT Files Removed From Pool'
