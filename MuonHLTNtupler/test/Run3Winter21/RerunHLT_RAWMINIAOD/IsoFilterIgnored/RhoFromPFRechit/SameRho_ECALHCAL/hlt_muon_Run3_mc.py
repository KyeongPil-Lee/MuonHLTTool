# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: hlt_muon --python_filename=hlt_muon_Run3_mc.py --step HLT:MuonHLT --process MYHLT --era=Run3 --mc --conditions=auto:phase1_2021_realistic --customise=HLTrigger/Configuration/MuonHLTForRun3/customizeMuonHLTForRun3.customizeDoubleMuIsoFix --customise=HLTrigger/Configuration/MuonHLTForRun3/customizeMuonHLTForRun3.customizeMuonHLTForDoubletRemoval --customise=HLTrigger/Configuration/MuonHLTForRun3/customizeMuonHLTForRun3.customizeMuonHLTForCscSegment --customise=HLTrigger/Configuration/MuonHLTForRun3/customizeMuonHLTForRun3.customizeMuonHLTForGEM --filein=/store/mc/Run3Winter21DRMiniAOD/DYToLL_M-50_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW/FlatPU30to80FEVT_112X_mcRun3_2021_realistic_v16-v2/120003/e786c41e-21ba-489f-880c-42d0a248e59e.root -n 100 --no_output --no_exec
import FWCore.ParameterSet.Config as cms

from Configuration.Eras.Era_Run3_cff import Run3

process = cms.Process('MYHLT',Run3)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('HLTrigger.Configuration.HLT_MuonHLT_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100),
    output = cms.optional.untracked.allowed(cms.int32,cms.PSet)
)

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('/store/mc/Run3Winter21DRMiniAOD/DYToLL_M-50_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW/FlatPU30to80FEVT_112X_mcRun3_2021_realistic_v16-v2/120003/e786c41e-21ba-489f-880c-42d0a248e59e.root'),
    secondaryFileNames = cms.untracked.vstring()
)

process.options = cms.untracked.PSet(
    FailPath = cms.untracked.vstring(),
    IgnoreCompletely = cms.untracked.vstring(),
    Rethrow = cms.untracked.vstring(),
    SkipEvent = cms.untracked.vstring(),
    allowUnscheduled = cms.obsolete.untracked.bool,
    canDeleteEarly = cms.untracked.vstring(),
    deleteNonConsumedUnscheduledModules = cms.untracked.bool(True),
    dumpOptions = cms.untracked.bool(False),
    emptyRunLumiMode = cms.obsolete.untracked.string,
    eventSetup = cms.untracked.PSet(
        forceNumberOfConcurrentIOVs = cms.untracked.PSet(
            allowAnyLabel_=cms.required.untracked.uint32
        ),
        numberOfConcurrentIOVs = cms.untracked.uint32(0)
    ),
    fileMode = cms.untracked.string('FULLMERGE'),
    forceEventSetupCacheClearOnNewRun = cms.untracked.bool(False),
    makeTriggerResults = cms.obsolete.untracked.bool,
    numberOfConcurrentLuminosityBlocks = cms.untracked.uint32(0),
    numberOfConcurrentRuns = cms.untracked.uint32(1),
    numberOfStreams = cms.untracked.uint32(0),
    numberOfThreads = cms.untracked.uint32(1),
    printDependencies = cms.untracked.bool(False),
    sizeOfStackForThreadsInKB = cms.optional.untracked.uint32,
    throwIfIllegalParameter = cms.untracked.bool(True),
    wantSummary = cms.untracked.bool(False)
)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('hlt_muon nevts:100'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

# Additional output definition

# Other statements
from HLTrigger.Configuration.CustomConfigs import ProcessName
process = ProcessName(process)

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2021_realistic', '')

# Path and EndPath definitions
process.endjob_step = cms.EndPath(process.endOfProcess)

# Schedule definition
process.schedule = cms.Schedule()
process.schedule.extend(process.HLTSchedule)
process.schedule.extend([process.endjob_step])
from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)

# customisation of the process.

# Automatic addition of the customisation function from HLTrigger.Configuration.MuonHLTForRun3.customizeMuonHLTForRun3
from HLTrigger.Configuration.MuonHLTForRun3.customizeMuonHLTForRun3 import customizeDoubleMuIsoFix,customizeMuonHLTForDoubletRemoval,customizeMuonHLTForCscSegment,customizeMuonHLTForGEM 

#call to customisation function customizeDoubleMuIsoFix imported from HLTrigger.Configuration.MuonHLTForRun3.customizeMuonHLTForRun3
process = customizeDoubleMuIsoFix(process)

#call to customisation function customizeMuonHLTForDoubletRemoval imported from HLTrigger.Configuration.MuonHLTForRun3.customizeMuonHLTForRun3
process = customizeMuonHLTForDoubletRemoval(process)

#call to customisation function customizeMuonHLTForCscSegment imported from HLTrigger.Configuration.MuonHLTForRun3.customizeMuonHLTForRun3
process = customizeMuonHLTForCscSegment(process)

#call to customisation function customizeMuonHLTForGEM imported from HLTrigger.Configuration.MuonHLTForRun3.customizeMuonHLTForRun3
process = customizeMuonHLTForGEM(process)

# Automatic addition of the customisation function from HLTrigger.Configuration.customizeHLTforMC
from HLTrigger.Configuration.customizeHLTforMC import customizeHLTforMC 

#call to customisation function customizeHLTforMC imported from HLTrigger.Configuration.customizeHLTforMC
process = customizeHLTforMC(process)

# End of customisation functions


# Customisation from command line

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion

################################
# -- update for the ntupler -- #
################################
process.source.fileNames = cms.untracked.vstring("/store/mc/Run3Winter21DRMiniAOD/ZToMuMu_M-50To120_TuneCP5_14TeV-powheg-pythia8/MINIAODSIM/FlatPU30to80FEVT_112X_mcRun3_2021_realistic_v16-v2/130000/e891d16a-78a3-4c5e-aabe-1b6f8c1f3b58.root")
process.source.secondaryFileNames = cms.untracked.vstring("/store/mc/Run3Winter21DRMiniAOD/ZToMuMu_M-50To120_TuneCP5_14TeV-powheg-pythia8/GEN-SIM-DIGI-RAW/FlatPU30to80FEVT_112X_mcRun3_2021_realistic_v16-v2/130001/0cf6e16c-7a35-47f4-9935-0f5df2d9a23f.root")
process.maxEvents.input = cms.untracked.int32( 100 )

# -- update rho calculation: use PF rechit instead of calo tower & remove calo tower sequences
from MuonHLTTool.MuonHLTNtupler.customizerForMuonHLTIsolation_rhoFromPFRechit import *
process = customizeForMuonHLTIsolation_sameRhoForECALHCAL(process)

from MuonHLTTool.MuonHLTNtupler.customizerForMuonHLTNtupler import *
customizerFuncForMuonHLTNtupler(process, "MYHLT")
process.ntupler.isMiniAOD             = cms.bool(True)
process.ntupler.offlineVertex         = cms.untracked.InputTag("offlineSlimmedPrimaryVertices")
process.ntupler.offlineMuon           = cms.untracked.InputTag("slimmedMuons")
process.ntupler.triggerObject_miniAOD = cms.untracked.InputTag("slimmedPatTrigger")
process.ntupler.PUSummaryInfo         = cms.untracked.InputTag("slimmedAddPileupInfo")
process.ntupler.genParticle           = cms.untracked.InputTag("prunedGenParticles")

process.ntupler.rho_ECAL = cms.untracked.InputTag("hltFixedGridRhoFastjetForMuons", "", "MYHLT")
process.ntupler.rho_HCAL = cms.untracked.InputTag("hltFixedGridRhoFastjetForMuons", "", "MYHLT")

process.schedule.extend([process.mypath])