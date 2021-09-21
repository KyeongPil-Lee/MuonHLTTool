import FWCore.ParameterSet.Config as cms

ntuplerBase = cms.EDAnalyzer("MuonHLTNtupler",
	# -- information stored in edm file
	triggerResults    = cms.untracked.InputTag("TriggerResults::HLT"),
	triggerEvent      = cms.untracked.InputTag("hltTriggerSummaryAOD::HLT"),
	offlineLumiScaler = cms.untracked.InputTag("scalersRawToDigi"),
	offlineVertex     = cms.untracked.InputTag("offlinePrimaryVertices"),
	offlineMuon       = cms.untracked.InputTag("muons"),

	# -- newly created objects by HLT rerun
	# -- new process name = "MYHLT"
	myTriggerResults = cms.untracked.InputTag("TriggerResults",       "", "MYHLT"),
	myTriggerEvent   = cms.untracked.InputTag("hltTriggerSummaryAOD", "", "MYHLT"),
	lumiScaler       = cms.untracked.InputTag("hltScalersRawToDigi",  "", "MYHLT"),

	L1Muon = cms.untracked.InputTag("hltGmtStage2Digis",       "Muon", "MYHLT"), # -- if L1 emulator is used
	# L1Muon = cms.untracked.InputTag("gmtStage2Digis",          "Muon", "RECO"), # -- if L1 is not emulated
	L2Muon = cms.untracked.InputTag("hltL2MuonCandidates",     "",     "MYHLT"),
	L3Muon = cms.untracked.InputTag("hltIterL3MuonCandidates", "",     "MYHLT"),
	TkMuon = cms.untracked.InputTag("hltHighPtTkMuonCands",    "",     "MYHLT"),

	ECALIsoMap = cms.untracked.InputTag("HLTL3muonEcalPFisorecoSequenceNoBoolsForMuons", "",               "MYHLT"),
	HCALIsoMap = cms.untracked.InputTag("HLTL3muonHcalPFisorecoSequenceNoBoolsForMuons", "" ,              "MYHLT"),
	trkIsoMap  = cms.untracked.InputTag("hltMuonTkRelIsolationCut0p07Map",               "trkIsoDeposits", "MYHLT"),

	iterL3OI        = cms.untracked.InputTag("hltL3MuonsIterL3OI",                   "", "MYHLT"),
	iterL3IOFromL2  = cms.untracked.InputTag("hltL3MuonsIterL3IO",                   "", "MYHLT"),
	iterL3FromL2    = cms.untracked.InputTag("hltIterL3MuonsFromL2LinksCombination", "", "MYHLT"),
	iterL3IOFromL1  = cms.untracked.InputTag("hltIter3IterL3FromL1MuonMerged",       "", "MYHLT"),
	iterL3MuonNoID  = cms.untracked.InputTag("hltIterL3MuonsNoID",                   "", "MYHLT"),

	# -- generator information
	PUSummaryInfo = cms.untracked.InputTag("addPileupInfo"),
	genEventInfo = cms.untracked.InputTag("generator"),
	genParticle = cms.untracked.InputTag("genParticles"),

	# -- miniAOD information
	isMiniAOD             = cms.bool(False),
	triggerObject_miniAOD = cms.untracked.InputTag("notUsed"), # -- only used when isMiniAOD == True

	# -- save rerun object?
	doSaveRerunObject = cms.bool(True),

	# -- for HLTPrescaleProvider
	l1tAlgBlkInputTag     = cms.InputTag("gtStage2Digis"), # -- for L1TGlobalUtil
	l1tExtBlkInputTag     = cms.InputTag("gtStage2Digis"), # -- for L1TGlobalUtil
	ReadPrescalesFromFile = cms.bool( False ),             # -- for L1TGlobalUtil

	# isMiniAOD             = cms.untracked.bool(True),
	# triggerObject_miniAOD = cms.untracked.InputTag("slimmedPatTrigger")

	# -- for the extrapolation of offlie muon to 2nd muon station
	preselection = cms.string("gmtMuonCand.quality > 1"), # FIXME: maybe exclude CSC-only region?
	useTrack  = cms.string("tracker"),
	useState  = cms.string("atVertex"),
	maxDeltaR   = cms.double(1.5),             ## FIXME: to be tuned
	maxDeltaEta = cms.double(0.3),             ## FIXME: to be tuned
	l1PhiOffset = cms.double(1.25 * 3.14159265359/180.),
	useSimpleGeometry = cms.bool(True),
	fallbackToME1     = cms.bool(True),
)
