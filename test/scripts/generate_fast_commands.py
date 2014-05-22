#! /usr/bin/env python

import csv,sys,os,getopt

def main(argv):

    pathInVariable1 = 'RelValTTbar_13_CMSSW_7_0_0_pre9-PU25ns_POSTLS162_V1-v3' #'RelValTTbar_13_CMSSW_7_1_0_pre6-PU25ns_PRE_LS171_V3-v1'
    pathInVariable2 = 'RelValTTbar_13_CMSSW_7_1_0_pre6-PU25ns_PRE_LS171_V5-v1update' #'RelValTTbar_13_CMSSW_7_1_0_pre6-PU25ns_PRE_LS171_V5-v1'

    preFix = '/afs/cern.ch/user/k/katkov/afswork/hiforest/CMSSW_4_2_8/src/Analysis/RHAnalyser/test/./RelValTTbar_13_CMSSW_7_1_0_pre6-PU25ns_PRE_LS171_V5_vs_7_0_0_pre9-PU25ns_POSTLS162_V1/merged_' #'/afs/cern.ch/user/k/katkov/afswork/hiforest/CMSSW_4_2_8/src/Analysis/RHAnalyser/test/./RelValTTbar_13_CMSSW_7_1_0_pre6-PU25ns_PRE_LS171_V3_vs_V5/merged_'

    idTags = (
      ('genet_vs_eta_reco','1','kFALSE'),('pfet_vs_eta_reco','1','kFALSE'),('track_pt','10','kTRUE'),('towet_vs_eta_reco','1','kFALSE'),('energy_vs_eta_reco','1','kFALSE'),('et_vs_eta_reco','1','kFALSE'),('energy_reco_0','80','kTRUE'),('energy_reco_1','80','kTRUE'),('cas_etot15','200','kTRUE'),('cas_zprofile','1','kFALSE'),('cas_phiprofile15','1','kFALSE'),('towet_reco_3','200','kTRUE'),('cas_chprofile','1','kFALSE')
    )

    for idTag in idTags:
        cmdString = "%%%./fast.py --id='{id0:s}' --fo='{preFixName1:s}{fileName1:s}.root' --to='{titleName1:s}' --fi='{preFixName2:s}{fileName2:s}.root' --ti='{titleName2:s}' --re='1.5' --co='{combVal:s}' --no='{normFlag:s}'###".format(id0=idTag[0],preFixName1=preFix,fileName1=pathInVariable1,titleName1=pathInVariable1,preFixName2=preFix,fileName2=pathInVariable2,titleName2=pathInVariable2,combVal=idTag[1],normFlag=idTag[2])
        print(cmdString)

if __name__ == "__main__":
    main(sys.argv[1:])
