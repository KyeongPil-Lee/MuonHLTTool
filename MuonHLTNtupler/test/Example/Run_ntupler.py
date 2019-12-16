import FWCore.ParameterSet.Config as cms

process = cms.Process("ntupler")

flag_HLTRerun = False
flag_miniAOD = True

exampleFile = ""
if flag_miniAOD:
  exampleFile = "file:/u/user/kplee/scratch/ROOTFiles_Test/102X/MINIAODSIM_Autumn18_DYJetsToLL_M50.root" # -- @ KNU
else:
  exampleFile = "file:/eos/cms/store/data/Run2018A/SingleMuon/AOD/PromptReco-v1/000/316/187/00000/1CCE3B04-E457-E811-A80C-FA163E0178DF.root" # -- @ lxplus

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(exampleFile),
    secondaryFileNames = cms.untracked.vstring(),
    # lumisToProcess = cms.untracked.VLuminosityBlockRange('258158:1-258158:1786'),
)

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
process.GlobalTag.globaltag = '101X_dataRun2_Prompt_v9'

process.load("Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff")
process.load('Configuration.Geometry.GeometryRecoDB_cff')

# -- ntupler -- #

if flag_HLTRerun:
  newProcessName = "MYHLT"
  
  from MuonHLTTool.MuonHLTNtupler.customizerForMuonHLTNtupler import customizerFuncForMuonHLTNtupler
  process = customizerFuncForMuonHLTNtupler(process, newProcessName)

else: # -- without HLT rerun
  from MuonHLTTool.MuonHLTNtupler.ntupler_cfi import ntuplerBase
  process.ntupler = ntuplerBase.clone()

  # -- change the input tag to store the default objects in the input edm file
  process.ntupler.L1Muon           = cms.untracked.InputTag("gmtStage2Digis",       "Muon", "RECO") # -- if L1 is not emulated
  process.ntupler.myTriggerResults = cms.untracked.InputTag("TriggerResults",       "",     "HLT")
  process.ntupler.myTriggerEvent   = cms.untracked.InputTag("hltTriggerSummaryAOD", "",     "HLT")
  process.ntupler.lumiScaler       = cms.untracked.InputTag("scalersRawToDigi")

  if flag_miniAOD:
    process.ntupler.isMiniAOD             = cms.bool(True)
    process.ntupler.offlineVertex         = cms.untracked.InputTag("offlineSlimmedPrimaryVertices")
    process.ntupler.offlineMuon           = cms.untracked.InputTag("slimmedMuons")
    process.ntupler.triggerObject_miniAOD = cms.untracked.InputTag("slimmedPatTrigger")
    process.ntupler.PUSummaryInfo         = cms.untracked.InputTag("slimmedAddPileupInfo")
    process.ntupler.genParticle           = cms.untracked.InputTag("prunedGenParticles")

  process.TFileService = cms.Service("TFileService",
    fileName = cms.string("ntuple.root"),
    closeFileFast = cms.untracked.bool(False),
    )

  process.mypath = cms.EndPath(process.ntupler)


process.MessageLogger = cms.Service( "MessageLogger",
	destinations = cms.untracked.vstring("cerr"),
	cerr = cms.untracked.PSet( threshold = cms.untracked.string('ERROR'), ),
	)