#! /usr/bin/env python

import csv,sys,os,getopt
import subprocess as spr

def main(argv):

    iBase = '00' #'00'
    globalEvtNumber = '1000'
    outDestinationDir = '/store/group/phys_heavyions/katkov/ttbar_13_710p8_self'
    customCommand = '--customise_commands="process.MessageLogger.destinations = [\'./detailedInfo.txt\'];process.source.duplicateCheckMode = cms.untracked.string(\'noDuplicateCheck\');process.options.wantSummary = cms.untracked.bool(True);RandomNumberGeneratorService = cms.Service(\'RandomNumberGeneratorService\',generator = cms.PSet(initialSeed = cms.untracked.uint32(123456789),engineName = cms.untracked.string(\'HepJamesRandom\')));from IOMC.RandomEngine.RandomServiceHelper import RandomNumberServiceHelper;randSvc = RandomNumberServiceHelper(process.RandomNumberGeneratorService);randSvc.populate()"'

    idTags = (
      ('TTbar_Tauola_13TeV_cfi',globalEvtNumber,'--conditions auto:run2_mc --eventcontent FEVTDEBUG --relval 9000,100 -s GEN,SIM --datatier GEN-SIM --customise SLHCUpgradeSimulations/Configuration/postLS1Customs.customisePostLS1 --magField 38T_PostLS1','--customise Configuration/StandardSequences/SimWithCastor_cff.customise --fileout ./TTbar_Tauola_13TeV_cfi_GEN_SIM{refNumber:s}.root --no_exec {cusCom:s}'.format(refNumber=iBase,cusCom=customCommand),'TTbar_Tauola_13TeV_cfi_GEN_SIM.py'),
      ('step2',globalEvtNumber,'--conditions auto:run2_mc -s DIGI,L1,DIGI2RAW,HLT:@relval,RAW2DIGI,L1Reco --datatier GEN-SIM-RAW --customise SLHCUpgradeSimulations/Configuration/postLS1Customs.customisePostLS1 --eventcontent RAWSIM --magField 38T_PostLS1','--no_exec --filein {outDir:s}/TTbar_Tauola_13TeV_cfi_GEN_SIM{refNumber1:s}.root --fileout ./step2_DIGI_L1_DIGI2RAW_HLT_RAW2DIGI_L1Reco{refNumber2:s}.root {cusCom:s}'.format(outDir=outDestinationDir,refNumber1=iBase,refNumber2=iBase,cusCom=customCommand),'step2_DIGI_L1_DIGI2RAW_HLT_RAW2DIGI_L1Reco.py'),
      ('step3',globalEvtNumber,'--conditions auto:run2_mc -s RAW2DIGI,L1Reco,RECO,EI --datatier GEN-SIM-RECO,AODSIM --customise SLHCUpgradeSimulations/Configuration/postLS1Customs.customisePostLS1 --eventcontent RECOSIM,AODSIM --magField 38T_PostLS1','--no_exec --filein {outDir:s}/step2_DIGI_L1_DIGI2RAW_HLT_RAW2DIGI_L1Reco{refNumber:s}.root {cusCom:s}'.format(outDir=outDestinationDir,refNumber=iBase,cusCom=customCommand),'step3_RAW2DIGI_L1Reco_RECO_EI.py')
    )

    for i1,idTag in enumerate(idTags):
        cmdString = "cmsDriver.py {stepName:s} -n {evtNumber1:s} {firstPart:s} -n {evtNumber2:s} {secondPart:s}".format(stepName=idTag[0],evtNumber1=idTag[1],firstPart=idTag[2],evtNumber2=idTag[1],secondPart=idTag[3])
        #cmdString = "%%%./fast.py --id='{id0:s}' --fo='{preFixName1:s}{fileName1:s}.root' --to='{titleName1:s}' --fi='{preFixName2:s}{fileName2:s}.root' --ti='{titleName2:s}' --re='1.5' --co='{combVal:s}' --no='{normFlag:s}'###".format(id0=idTag[0],preFixName1=preFix,fileName1=pathInVariable1,titleName1=pathInVariable1,preFixName2=preFix,fileName2=pathInVariable2,titleName2=pathInVariable2,combVal=idTag[1],normFlag=idTag[2])
        #print(cmdString)
        pipeCmd = spr.Popen(cmdString,stdout=spr.PIPE,shell=True)
        pipeCmd.wait()
        pipeCmd.poll()
        pipeCmdRes = pipeCmd.communicate()
        print(pipeCmdRes)
        dic = {'arg_stepx_config_file':idTag[4],'arg_out_dir':outDestinationDir}
        f1 = open('./fast_template.sh', 'r')
        filestr = f1.read()
        for ifrom, ito in dic.iteritems():
            filestr = filestr.replace(ifrom,ito)
        f1.close()
        fname2 = './fast{}.sh'.format(i1+1)
        f2 = open(fname2, 'w')
        f2.write(filestr)
        f2.close()
        pipeCmdTmp = spr.Popen("chmod u+x {}".format(fname2),stdout=spr.PIPE,shell=True)
        pipeCmdTmp.wait()
        pipeCmdTmp.poll()
        pipeCmdTmpRes = pipeCmdTmp.communicate()
        print(pipeCmdTmpRes)

if __name__ == "__main__":
    main(sys.argv[1:])
