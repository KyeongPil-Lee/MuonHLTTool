import FWCore.ParameterSet.Config as cms

process = cms.Process("ntupler")

flag_HLTRerun = False
flag_miniAOD = True

exampleFile = ""
if flag_miniAOD:
  exampleFile = "file:/pnfs/knu.ac.kr/data/cms/store/data/Run2018D/HLTPhysics/MINIAOD/PromptReco-v2/000/320/479/00000/26CBAEBD-E194-E811-94FB-FA163E50185F.root" # -- @ KNU
else:
  exampleFile = "file:/eos/cms/store/data/Run2018A/SingleMuon/AOD/PromptReco-v1/000/316/187/00000/1CCE3B04-E457-E811-A80C-FA163E0178DF.root" # -- @ lxplus

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(exampleFile),
    secondaryFileNames = cms.untracked.vstring(),
    # lumisToProcess = cms.untracked.VLuminosityBlockRange('258158:1-258158:1786'),
)

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
process.GlobalTag.globaltag = '102X_dataRun2_Prompt_v11'

process.load("Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff")
process.load('Configuration.Geometry.GeometryRecoDB_cff')

# -- for the extrapolation of offlie muon to 2nd muon station
process.load("TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorAny_cfi")
process.load("TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorAlong_cfi")
process.load("TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorOpposite_cfi")

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
    # process.ntupler.triggerObject_miniAOD = cms.untracked.InputTag("selectedPatTrigger") # -- 2016 ntuple
    process.ntupler.PUSummaryInfo         = cms.untracked.InputTag("slimmedAddPileupInfo")
    process.ntupler.genParticle           = cms.untracked.InputTag("prunedGenParticles")

    process.ntupler.doSaveRerunObject     = cms.bool(False)

  process.TFileService = cms.Service("TFileService",
    fileName = cms.string("ntuple.root"),
    closeFileFast = cms.untracked.bool(False),
    )

  process.mypath = cms.EndPath(process.ntupler)


process.MessageLogger = cms.Service( "MessageLogger",
	destinations = cms.untracked.vstring("cerr"),
	cerr = cms.untracked.PSet( threshold = cms.untracked.string('ERROR'), ),
	)