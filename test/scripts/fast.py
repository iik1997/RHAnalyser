#! /usr/bin/env python

#In general: modify fast_template.C, run ./fast.py as in examples below, run root -l fastm.C

# ./fast.py --id='towet_vs_eta_reco' --fo='/afs/cern.ch/user/k/katkov/afswork/hiforest/CMSSW_4_2_8/src/Analysis/RHAnalyser/test/gen_out_53X_LV1_track8_jet5.root' --to='HydjetDrum1p8_MB_STARTHI53_LV1_track8_Jet5_5Feb2014_12pmEST' --fi='/afs/cern.ch/user/k/katkov/afswork/hiforest/CMSSW_4_2_8/src/Analysis/RHAnalyser/test/gen_out_53X_GR_R_53_LV6_track8_Jet15_data.root' --ti='HIMinBias2011_GR_R_53_LV6_20Feb2014_1000amEST_Track8_Jet15'

# ./fast.py --id='towet_vs_eta_reco' --fo='/afs/cern.ch/user/k/katkov/afswork/hiforest/CMSSW_4_2_8/src/Analysis/RHAnalyser/test/gen_out_relval5316_GR_R_53_LV6_mar03_data.root' --to='HIMinBiasUPC_CMSSW_5_3_16-GR_R_53_LV6_mar03_RelVal_hi2011-v2' --fi='/afs/cern.ch/user/k/katkov/afswork/hiforest/CMSSW_4_2_8/src/Analysis/RHAnalyser/test/gen_out_relval5316_STARTHI_LV1_mar03_mc.root' --ti='RelValHydjetQ_MinBias_2760GeV_CMSSW_5_3_16-STARTHI53_LV1_mar03-v2'

# ./fast.py --id='towet_reco_0' --fo='/afs/cern.ch/user/k/katkov/afswork/hiforest/CMSSW_4_2_8/src/Analysis/RHAnalyser/test/gen_out_relval5316_GR_R_53_LV6_mar03_data_hforbsc_noscr_vtx.root' --to='HIMinBiasUPC_CMSSW_5_3_16-GR_R_53_LV6_mar03_RelVal_hi2011-v2' --fi='/afs/cern.ch/user/k/katkov/afswork/hiforest/CMSSW_4_2_8/src/Analysis/RHAnalyser/test/gen_out_53X_GR_R_53_LV6_track8_Jet15_data.root' --ti='HIMinBias2011_GR_R_53_LV6_20Feb2014_1000amEST_Track8_Jet15'

# ./fast.py --id='cas_etot' --fo='/afs/cern.ch/user/k/katkov/afswork/hiforest/CMSSW_4_2_8/src/Analysis/RHAnalyser/test/gen_out_rereco5316_Jet55or65ScraVtx_FT_R_53_LV6_14mar2014_data.root' --to='HIHighPt_CMSSW_5_3_16-FT_R_53_LV6_mar14_ReReco' --fi='/afs/cern.ch/user/k/katkov/afswork/hiforest/CMSSW_4_2_8/src/Analysis/RHAnalyser/test/gen_out_privreco5316_Jet55or65ScraVtx_GR_R_53_LV6_03Mar2014_1600CET.root' --ti='HIHighPt_CMSSW_5_3_16-GR_R_53_LV6_03Mar2014_1600CET_PrivRecoStas'

# ./fast.py --id='cas_phiprofile' --fo='/afs/cern.ch/user/k/katkov/afswork/hiforest/CMSSW_4_2_8/src/Analysis/RHAnalyser/test/./RelValTTbar_13_CMSSW_7_1_0_pre6-PU25ns_PRE_LS171_V3_vs_V5/merged_RelValTTbar_13_CMSSW_7_1_0_pre6-PU25ns_PRE_LS171_V3-v1.root' --to='RelValTTbar_13_CMSSW_7_1_0_pre6-PU25ns_PRE_LS171_V3-v1' --fi='/afs/cern.ch/user/k/katkov/afswork/hiforest/CMSSW_4_2_8/src/Analysis/RHAnalyser/test/./RelValTTbar_13_CMSSW_7_1_0_pre6-PU25ns_PRE_LS171_V3_vs_V5/merged_RelValTTbar_13_CMSSW_7_1_0_pre6-PU25ns_PRE_LS171_V5-v1.root' --ti='RelValTTbar_13_CMSSW_7_1_0_pre6-PU25ns_PRE_LS171_V5-v1' --re='1.5' --co='1' --no='kFALSE'

import csv,sys,os,getopt

def main(argv):

    cmd0 = ""
    usage = 'Usage: %s --id=hist_id --fo=file0.root --to=title0 --fi=file1.root --ti=title1 --re=yaxisfact --co=ncombin --no=wantnorm' % (sys.argv[0])

    try:
       opts, args = getopt.getopt(argv,"hi:f:t:g:n:r:c:w:",["id=","fo=","to=","fi=","ti=","re=","co=","no="])
    except getopt.GetoptError:
       print usage; sys.exit(2)
    if len(opts) == 0: #or len(args) == 0:
       print usage; sys.exit(2)

    for opt, arg in opts:
       if opt == '-h':
          print usage; sys.exit()
       elif opt in ("-i", "--id"):
          arg_hid = arg 
          print ('Histogram ID: ',arg_hid)
       elif opt in ("-f", "--fo"):
          arg_f0 = arg 
          print ('First input file: ',arg_f0)
       elif opt in ("-t", "--to"):
          arg_tit0 = arg 
          print ('First input name: ',arg_tit0)
       elif opt in ("-g", "--fi"):
          arg_f1 = arg
          print ('Second input file: ',arg_f1)
       elif opt in ("-n", "--ti"):
          arg_tit1 = arg
          print ('Second input name: ',arg_tit1)
       elif opt in ("-r", "--re"):
          arg_fact = arg
          print ('Re-scale maximum of Y-axis: ',arg_fact)
       elif opt in ("-c", "--co"):
          arg_comb = arg
          print ('Number of bins to combine: ',arg_comb)
       elif opt in ("-w", "--no"):
          arg_norm = arg
          print ('Normalize to number of entries: ',arg_norm)

    dic = {'arghisto':arg_hid, 'arg_full_path_to_file0':arg_f0, 'arg_file0_title':arg_tit0, 'arg_full_path_to_file1':arg_f1, 'arg_file1_title':arg_tit1, 're_scale_factor':arg_fact, 'bins_to_combine':arg_comb, 'want_normalization':arg_norm}
    f1 = open('./fast_template.C', 'r')
    filestr = f1.read()
    for ifrom, ito in dic.iteritems():
        filestr = filestr.replace(ifrom,ito)
    f1.close()

    f2 = open('./fastm.C', 'w')
    f2.write(filestr)
    f2.close()
    print ('Now run created ROOT macro ./fastm.C')

if __name__ == "__main__":
    main(sys.argv[1:])

