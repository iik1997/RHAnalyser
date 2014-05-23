process = cms.Process('HLT')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
###process.load('Configuration.EventContent.EventContentHeavyIons_cff')
#process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Digi_cff')
process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load('Configuration.StandardSequences.DigiToRaw_cff')
###process.load('HLTrigger.Configuration.HLT_HIon_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
)

# Input source
process.source = cms.Source("PoolSource",
    secondaryFileNames = cms.untracked.vstring(),
    fileNames = cms.untracked.vstring('/store/mc/Fall13dr/QCD_Pt-15to30_fwdJet_Tune4C_13TeV_pythia8/AODSIM/castor_tsg_PU1bx50_POSTLS162_V1-v1/00000/06150834-A56E-E311-AFF1-00304867C0EA.root'), 
    #inputCommands = cms.untracked.vstring('keep *', 
    #    'drop *_simEcalPreshowerDigis_*_*'),
    #dropDescendantsOfDroppedBranches = cms.untracked.bool(False)
)

process.options = cms.untracked.PSet()

# Production Info
#process.configurationMetadata = cms.untracked.PSet(
#    version = cms.untracked.string('$Revision: 1.381.2.28 $'),
#    annotation = cms.untracked.string('step2 nevts:1'),
#    name = cms.untracked.string('PyReleaseValidation')
#)

process.output = cms.OutputModule("PoolOutputModule",
    #splitLevel = cms.untracked.int32(0),
    #eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = cms.untracked.vstring('drop *', 'keep *_castorreco_*_*','keep ZDC*_*Digis_*_*'),
    fileName = cms.untracked.string('filtered.root'),
    #dataset = cms.untracked.PSet(
    #    filterName = cms.untracked.string(''),
    #    dataTier = cms.untracked.string('GEN-SIM-DIGI-RAW-HLTDEBUG')
    #)
)
# 'keep ZDCDataFramesSorted_*Digis_*_*',
# 'keep *_castorreco_*_*'

# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:startup', '')

# https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookDataSamples #Copy Data Locally 
process.end = cms.EndPath(process.output)

