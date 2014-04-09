import FWCore.ParameterSet.Config as cms

process = cms.Process("GenParticles")

vtxTag="hiSelectedVertex"
#pp: "offlinePrimaryVertices"//HI: "hiSelectedVertex"
trkTag="hiGeneralTracks"
#pp: "generalTracks" //OldHI44X or old tags: "hiSelectedTracks" //NewHI53X: "hiGeneralTracks"
genPartTag="hiGenParticles"
#pp && pPb: "genParticles" //HI: "hiGenParticles"
pfTag="particleFlowTmp"
#pp && pPb: "particleFlow" //HI: "particleFlowTmp"
glbTag='FT_R_53_LV6::All'
#'STARTHI53_V27::All'
#'GR_R_53_LV6A::All'
#'GR_R_53_LV6::All'
#'STARTHI53_LV1::All'
#'STARTHI53_V28::All'
#'GR_P_V43D::All'
#'FT_R_42_V18A::All' (2.76 pp of 2011 in 42X @DESY) # GR_R_44_V15 -> fits in general to 44X # GR_P_V43D -> fits in general to 53X
# STARTHI53_LV1 STARTHI53_V28
evtNo=-1
# set evtNo to -1 if want to process all of them
#
# In addition check : input files (or empty list); execution path (distiguish between ZeroBias and PAZeroBias triggers!!!)
# process.pileupVertexFilterCutGplusUpsPP:
# # # # #process.load('Appeltel.RpPbAnalysis.PAPileUpVertexFilter_cff')
# process.hfEitherOfTwoFilter,hfCoincFilter:
# # # # #process.load('edwenger.Skims.hfCoincFilter_cff')
#
# REMEMBER that in the code there are:
#/***************************/
#/**********!!!**************/
#// 2011 pp-data
#// #define OLD_428_DATA 
#// 2011 PbPb-data
#// #define OLD_44X_DATA
#// Any kind of calibrated DATA
##define CALIBRATED_DATA 
#// for now just a switch to enable checks of saturation flag in the recent SW (can be data or MC)
##define NEW_53X
#//#define THIS_IS_MC
#/**********!!!**************/
#/**************************/

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
###process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
###process.load('Configuration.StandardSequences.ReconstructionHeavyIons_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
#####process.load('Configuration.EventContent.EventContentHeavyIons_cff')

process.load("FWCore.MessageService.MessageLogger_cfi")
#process.MessageLogger.cerr.threshold = 'INFO'
#process.MessageLogger.categories.append('Tracking')
#process.MessageLogger.cerr.INFO = cms.untracked.PSet(limit = cms.untracked.int32(-1))
##process.MessageLogger = cms.Service("MessageLogger",
##                                    destinations   = cms.untracked.vstring('detailedInfo', 'critical', 'cerr'),
##                                    critical       = cms.untracked.PSet(threshold = cms.untracked.string('ERROR'),filename = cms.untracked.string('critical.log')),
##                                    detailedInfo   = cms.untracked.PSet(threshold = cms.untracked.string('INFO'),filename = cms.untracked.string('cinfo.log')),
##                                    cerr           = cms.untracked.PSet(threshold = cms.untracked.string('WARNING'),filename = cms.untracked.string('cerr.log'))
##                                    )

process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = glbTag 
# 'FT_R_42_V18A::All' -> /AllPhysics2760/Run2011A-16Jul2011-v1/RECO (2.76 pp of 2011 in 42X)
# GR_R_44_V15 ---> fits in general to 44X
# GR_P_V43D   ---> fits in general to 53X (eg 2013 data), GR_P_V43F is updated pix geometry?
# GR_P_V41::All -> /PAPhysics/PARun2012-PromptReco-v2/RECO (pilot run in 53X)
# START53_V11::All -> Colin's pPb EPOS
# STARTHI53_V25, STARTHI53_V26 -> 2013 pPb, pp MC
# Misc: 'GR_R_44_V10::All' -> low-pt tracking
# Misc: STARTHI44_V11::All
# Misc: 'GR_E_V33A::All'


process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(int(evtNo)) )

#process.source = cms.Source("PoolSource",
#    fileNames = cms.untracked.vstring(
#        #'file:/usr/users/ikatkov/hiforest/CMSSW_4_4_2_patch7/test/skim_conformal_test.root'
#        'file:/storage/5/cbaus/RECO/PbPb_1380_eposLHC_HI44/job_N5_10.root'
#    )
#)

#!#!#readFiles = cms.untracked.vstring()
process.source = cms.Source ("PoolSource",duplicateCheckMode = cms.untracked.string("checkEachRealDataFile"),
fileNames = cms.untracked.vstring(
[
##'/store/user/katkov/particleGunNuE10Eta0_CMSSW_5_3_8_patch3_STARTHI53_V27/SingleNuE10_cfi_GEN_SIM_DIGI_L1_DIGI2RAW_RAW2DIGI_RECO_4k.root',
##'/store/user/katkov/particleGunNuE10Eta0_CMSSW_5_3_8_patch3_STARTHI53_V27/SingleNuE10_cfi_GEN_SIM_DIGI_L1_DIGI2RAW_RAW2DIGI_RECO_4k_01.root'
#'/store/user/katkov/CMSSW_5_3_16_14Mar2014ReReco/7A008022-08AF-E311-A520-842B2B6AE9EA.root',
#'/store/user/katkov/CMSSW_5_3_16_14Mar2014ReReco/7AEE9F8E-37B0-E311-A8FC-003048F319D4.root',
#'/store/user/katkov/CMSSW_5_3_16_14Mar2014ReReco/7AD3EB92-7FB0-E311-9E15-003048F5D964.root',
#'/store/user/katkov/CMSSW_5_3_16_14Mar2014ReReco/7AEDFAE3-38AF-E311-9C53-003048F2A916.root',
#'/store/user/katkov/CMSSW_5_3_16_14Mar2014ReReco/7AD5E1B2-82AE-E311-8A76-003048CDCC18.root',
#'/store/user/katkov/CMSSW_5_3_16_14Mar2014ReReco/7AB59508-85AE-E311-AF0A-003048F310B4.root',
#'/store/user/katkov/CMSSW_5_3_16_14Mar2014ReReco/7A10E6A1-9EAF-E311-B2B0-003048F317FE.root',
#'/store/user/katkov/CMSSW_5_3_16_14Mar2014ReReco/7A22D427-3AB1-E311-80C1-00A0D1E95318.root',
#'/store/user/katkov/CMSSW_5_3_16_14Mar2014ReReco/7A2488E6-7AB0-E311-B762-003048F29A12.root',
#'/store/user/katkov/CMSSW_5_3_16_14Mar2014ReReco/7A2F9098-48AF-E311-953E-003048F2A91A.root',
#'/store/user/katkov/CMSSW_5_3_16_14Mar2014ReReco/7A0B149D-38B0-E311-92E0-00A0D1E943F8.root',
#'/store/user/katkov/CMSSW_5_3_16_14Mar2014ReReco/7A6EE7A2-89AF-E311-AC35-842B2B6AECDD.root',
#'/store/user/katkov/CMSSW_5_3_16_14Mar2014ReReco/7A27A382-7AB0-E311-8B0D-003048F7D74C.root',
#'/store/user/katkov/CMSSW_5_3_16_14Mar2014ReReco/7A86CA0D-3BB1-E311-B5B6-00A0D1E94408.root',
#'/store/user/katkov/CMSSW_5_3_16_14Mar2014ReReco/7A4E779C-8AAE-E311-BB07-003048F63A0A.root',
#'/store/user/katkov/CMSSW_5_3_16_14Mar2014ReReco/7A59DB26-A4AF-E311-A2D2-842B2B6F5D95.root',
#'/store/user/katkov/CMSSW_5_3_16_14Mar2014ReReco/7AA466D9-38B0-E311-8F9E-003048F31688.root'
]
##['/store/relval/CMSSW_5_3_16/RelValHydjetQ_MinBias_2760GeV/GEN-SIM-RECO/STARTHI53_LV1_mar03-v2/00000/068E7404-74A3-E311-80E8-0025904CDDEE.root',
##'/store/relval/CMSSW_5_3_16/RelValHydjetQ_MinBias_2760GeV/GEN-SIM-RECO/STARTHI53_LV1_mar03-v2/00000/1EF519FA-77A3-E311-AA9B-0025904C51F2.root',
##'/store/relval/CMSSW_5_3_16/RelValHydjetQ_MinBias_2760GeV/GEN-SIM-RECO/STARTHI53_LV1_mar03-v2/00000/1EF60918-7DA3-E311-9E4C-0025904C5DDA.root',
##'/store/relval/CMSSW_5_3_16/RelValHydjetQ_MinBias_2760GeV/GEN-SIM-RECO/STARTHI53_LV1_mar03-v2/00000/9E951B9A-65A3-E311-8464-0025904C4E2A.root',
##'/store/relval/CMSSW_5_3_16/RelValHydjetQ_MinBias_2760GeV/GEN-SIM-RECO/STARTHI53_LV1_mar03-v2/00000/BE07AAD0-86A3-E311-96D1-0025904C66A2.root',
##'/store/relval/CMSSW_5_3_16/RelValHydjetQ_MinBias_2760GeV/GEN-SIM-RECO/STARTHI53_LV1_mar03-v2/00000/DC5F40A8-84A3-E311-933D-0025904C6224.root']
#['/store/relval/CMSSW_5_3_16/HIMinBiasUPC/RECO/GR_R_53_LV6_mar03_RelVal_hi2011-v2/00000/0E7BA19C-FBA2-E311-A90A-0025904CF710.root',
#'/store/relval/CMSSW_5_3_16/HIMinBiasUPC/RECO/GR_R_53_LV6_mar03_RelVal_hi2011-v2/00000/142E1971-28A3-E311-A6F7-0025904C66E8.root',
#'/store/relval/CMSSW_5_3_16/HIMinBiasUPC/RECO/GR_R_53_LV6_mar03_RelVal_hi2011-v2/00000/20CB90FD-F8A2-E311-87DF-0025904C51FC.root',
#'/store/relval/CMSSW_5_3_16/HIMinBiasUPC/RECO/GR_R_53_LV6_mar03_RelVal_hi2011-v2/00000/28293DD5-13A3-E311-B817-0025904C4F50.root',
#'/store/relval/CMSSW_5_3_16/HIMinBiasUPC/RECO/GR_R_53_LV6_mar03_RelVal_hi2011-v2/00000/361FF1A2-0BA3-E311-9AB5-0025904CF758.root']
###[
###'/store/user/katkov/hydjetPythia80to120_RECO_44X/HidjetQuenchedMinBias_ef35771acd3d097eb58bb506e874348e_hiReco_RAW2DIGI_RECO_100_1_IDJ.root',
###'/store/user/katkov/hydjetPythia80to120_RECO_44X/HidjetQuenchedMinBias_ef35771acd3d097eb58bb506e874348e_hiReco_RAW2DIGI_RECO_101_1_MoD.root',
###'/store/user/katkov/hydjetPythia80to120_RECO_44X/HidjetQuenchedMinBias_ef35771acd3d097eb58bb506e874348e_hiReco_RAW2DIGI_RECO_102_1_GvY.root'
###
###'/store/user/katkov/hydjetPythia80to120_RECO_53X/Hydjet_Drum_53X_test01_dijet80to120_pythiaPlusHydjet_ad3b645a1a1ef23fcdc562fb4f1ce354_hiReco_RAW2DIGI_L1Reco_RECO_102_2_bgc.root',
###'/store/user/katkov/hydjetPythia80to120_RECO_53X/Hydjet_Drum_53X_test01_dijet80to120_pythiaPlusHydjet_ad3b645a1a1ef23fcdc562fb4f1ce354_hiReco_RAW2DIGI_L1Reco_RECO_100_2_FiT.root',
###'/store/user/katkov/hydjetPythia80to120_RECO_53X/Hydjet_Drum_53X_test01_dijet80to120_pythiaPlusHydjet_ad3b645a1a1ef23fcdc562fb4f1ce354_hiReco_RAW2DIGI_L1Reco_RECO_101_2_vPh.root'
###
###'/store/user/katkov/pp2760GeV2013Data211752RECO/Run2013A_PPMinBias_RECO_PromptReco-v1_000_211752_00000_00C017DB-A176-E211-926D-002481E0D7D8.root'
###
#'/store/group/phys_heavyions/katkov/pp2760GeV2011Data/Run2011A_AllPhysics2760_RECO_161396_16Jul2011-v1_0000_1C14DE8B-63B1-E011-9D94-003048D45F42.root',
#'/store/group/phys_heavyions/katkov/pp2760GeV2011Data/Run2011A_AllPhysics2760_RECO_161396_16Jul2011-v1_0000_8C4F0720-64B1-E011-BCB2-001A64789474.root',
#'/store/group/phys_heavyions/katkov/pp2760GeV2011Data/Run2011A_AllPhysics2760_RECO_161396_16Jul2011-v1_0000_9AFE19FB-72B1-E011-B8FB-003048D47742.root'
#'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/209/948/00000/20032E91-D25C-E211-AC52-5404A63886CF.root' #LS13
#'root://eoscms//eos/cms/store/caf/user/vzhukova/EPOS_RECO/epos_reco_fix_1817_1_53Q.root'
###                                 ]
),
#store/hidata/HIRun2013A/PAMinBiasUPC/RAW/v1/000/209/948/2C18FB73-FC5C-E211-95E9-001D09F2305C.root :: LS1-10
);

##vvvvv


process.TFileService = cms.Service("TFileService",
                                    fileName = cms.string("gen_out.root"), #/afs/cern.ch/work/k/katkov/data/
                                    closeFileFast = cms.untracked.bool(True)
                                  )
#Trigger Selection
#import HLTrigger.HLTfilters.hltHighLevel_cfi
#process.hltMinBias = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
#process.hltMinBias.HLTPaths = ["HLT_HIJet65_Jet55_v1"] #HLT_HIMinBiasHfOrBSC_v1 #HLT_PAL1Tech53_MB_v1 #HLT_PAL1Tech53_MB_SingleTrack_v1 #HLT_ZeroBias_v1 #HLT_PAZeroBias_v1 #HLT_ZeroBiasPixel_SingleTrack_v*
process.load("HLTrigger.HLTfilters.hltHighLevel_cfi")
process.hltJet55or65Trigger = process.hltHighLevel.clone()
process.hltJet55or65Trigger.HLTPaths = ["HLT_HIJet55_v1","HLT_HIJet65_v1"]
process.hltJet55or65Trigger.andOr = cms.bool(True)

#Single Vertex Selection
process.primaryVertexFilter = cms.EDFilter("VertexSelector",
   src = cms.InputTag(vtxTag), #"offlinePrimaryVertices"
   cut = cms.string("!isFake && tracksSize() > 1 && abs(z) <= 25 && position.Rho <= 2"), #("!isFake && ndof > 4 && abs(z) <= 24 && position.Rho <= 2"), # tracksSize() > 3 for the older cut ##abs(z) <= 15 # ndof > 4
   filter = cms.bool(True),   # if False it won't filter the events, just produce an empty vertex collection. ##True
)
process.justOneVertexFilter = cms.EDFilter( ### in primaryVertexFilter set 'filter' to False to filter here on number of vertices
  "VertexCountFilter",
  src = cms.InputTag("primaryVertexFilter"),
  minNumber = cms.uint32(1),
  maxNumber = cms.uint32(1)
)
#Removal of Scraping 
process.noscraping = cms.EDFilter("FilterOutScraping",
applyfilter = cms.untracked.bool(True),
debugOn = cms.untracked.bool(False),
numtrack = cms.untracked.uint32(10),
thresh = cms.untracked.double(0.25),
src = cms.untracked.InputTag(trkTag)
)

#MinBias data RECO (and thus hiForest) produced includes these cuts (Marguerite)
#process.load("HLTrigger.HLTfilters.hltHighLevel_cfi")
#process.hltMinBiasHFOrBSC = process.hltHighLevel.clone()
#process.hltMinBiasHFOrBSC.HLTPaths = ["HLT_HIMinBiasHfOrBSC_v1"]
#process.load("HeavyIonsAnalysis.Configuration.collisionEventSelection_cff")
#process.filterSequence = cms.Sequence(process.hltMinBiasHFOrBSC*process.collisionEventSelection)
## Path and EndPath definitions
#process.raw2digi_step = cms.Path(process.hltMinBiasHFOrBSC*process.RawToDigi)
#process.reconstruction_step = cms.Path(process.hltMinBiasHFOrBSC*process.reconstructionHeavyIons)
#process.filter_step = cms.Path(process.filterSequence)

process.demo = cms.EDAnalyzer('RHAnalyser',
                              showDebug = cms.untracked.bool(False), #True
                              vtxSrc = cms.untracked.InputTag(vtxTag),
                              trkSrc = cms.untracked.InputTag(trkTag),
                              genPartSrc = cms.untracked.InputTag(genPartTag),
                              pfSrc = cms.untracked.InputTag(pfTag)
)

#process.p = cms.Path(process.demo)
process.p = cms.Path(process.hltJet55or65Trigger*process.noscraping*process.primaryVertexFilter*process.demo)
# # # # #process.p = cms.Path(process.hltMinBias*process.noscraping*process.primaryVertexFilter*process.hfEitherOfTwoFilter*process.demo)
#process.p = cms.Path(process.hltMinBias*process.noscraping*process.primaryVertexFilter*process.justOneVertexFilter*process.pileupVertexFilterCutGplusUpsPP*process.demo)
