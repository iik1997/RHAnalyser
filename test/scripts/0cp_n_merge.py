#! /usr/bin/env python

# lcg-ls -v 'srm://dcache-se-cms.desy.de:8443/srm/managerv2?SFN=/pnfs/desy.de/cms/tier2/store/user/katkov' | grep gen_out | cut -d'/' -f9
# lcg-cp -v --srm-timeout 3600 -b -D srmv2 --sendreceive-timeout 240000 --connect-timeout 30000000 srm://dcache-se-cms.desy.de:8443/srm/managerv2'?'SFN=/pnfs/desy.de/cms/tier2/store/user/katkov/gen_out_17_1_ahT.root srm://srm-eoscms.cern.ch:8443/srm/v2/server'?'SFN=/eos/cms/store/group/phys_heavyions/katkov/pp2760GeV2011Data161366Simplev1/gen_out_17_1_ahT.root

# CMSSW_6_2_5 --> python 2.7.3

import os
import sys
import subprocess as spr

def main():

    doMerge = True #DOES NOT WORK IF your output path is a Tier-2 location rather than a local directory
   
    pathInVariable = "RelValTTbar_13_CMSSW_7_1_0_pre6-PU25ns_PRE_LS171_V5-v1" #"RelValTTbar_13_CMSSW_7_1_0_pre6-PU25ns_PRE_LS171_V3-v1"
    pathOutVariable = "zout1" #zout

    pathInFixed = "srm://dcache-se-cms.desy.de:8443/srm/managerv2'?'SFN=/pnfs/desy.de/cms/tier2/store/user/katkov"
    pathOutFixed = "/tmp/katkov" #remove file:/// below + "srm://srm-eoscms.cern.ch:8443/srm/v2/server'?'SFN=/eos/cms/store/group/phys_heavyions/katkov" 

    dirOutName = "{}/{}".format(pathOutFixed,pathOutVariable)  
    if not os.path.isdir(dirOutName):
       os.makedirs(dirOutName) 

    cmdString = "lcg-ls -v {pNameF1:s}/{pNameV1:s} | grep gen_out | cut -d'/' -f10".format(pNameF1=pathInFixed,pNameV1=pathInVariable)
    #cmdString = "date"
    pipeCmd = spr.Popen(cmdString,stdout=spr.PIPE,shell=True)
    pipeCmd.wait()
    pipeCmd.poll()
    pipeCmdRes = pipeCmd.communicate()
    #print(pipeCmdRes[0].strip())
    fileNames = (pipeCmdRes[0].strip()).split() #just list of file-names without pre-pended paths

    ##fileNames = (
    #'gen_out_22_1_p7s.root',
    #'gen_out_8_1_SGc.root',
    #'gen_out_17_1_ahT.root',
    ##'gen_out_9_1_G5i.root',
    ##'gen_out_19_1_fTT.root',
    ##'gen_out_34_1_6Xq.root',
    ##'gen_out_23_1_NGy.root',
    ##'gen_out_25_1_ZKV.root',
    ##'gen_out_10_1_wsc.root',
    ##'gen_out_6_1_NeX.root',
    ##'gen_out_27_1_B3e.root',
    ##'gen_out_3_1_FsB.root',
    ##'gen_out_7_1_upn.root'
    ##)

    for fileName in fileNames:
        cmdString = "lcg-cp -v --srm-timeout 3600 -b -D srmv2 --sendreceive-timeout 240000 --connect-timeout 30000000 {pNameF1:s}/{pNameV1:s}/{fName1:s} file:///{pName2:s}/{fName2:s}"
        cmdString = cmdString.format(pNameF1=pathInFixed,pNameV1=pathInVariable,fName1=fileName,pName2=dirOutName,fName2=fileName)
        #print(cmdString)
        #os.system(cmd)
        pipeCmd = spr.Popen(cmdString,stdout=spr.PIPE,shell=True)
        pipeCmd.wait()
        pipeCmd.poll()
        pipeCmdRes = pipeCmd.communicate()
        print(pipeCmdRes)

    if doMerge:
        cmdList = ['hadd -T {}/out.root'.format(dirOutName)]
        for fileName in fileNames: 
            cmdList.append(' {}/{}'.format(dirOutName,fileName)) 
        cmdString = ''.join(cmdList) 
        #print(cmdString)
        pipeCmd = spr.Popen(cmdString,stdout=spr.PIPE,shell=True)
        pipeCmd.wait()
        pipeCmd.poll()
        pipeCmdRes = pipeCmd.communicate()
        print(pipeCmdRes[0].strip())
        cmdMvString = 'mv {}/out.root ./merged_{}.root'.format(dirOutName,pathInVariable)
        pipeMvCmd = spr.Popen(cmdMvString,stdout=spr.PIPE,shell=True)

if __name__ == "__main__":
    main()
