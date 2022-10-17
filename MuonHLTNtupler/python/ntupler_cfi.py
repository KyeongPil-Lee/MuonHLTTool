# -- two types of ntupler configuration
# -- 1) ntuplerBase:         ntupler when HLT rerunning is not done
# -- 2) ntuplerBaseRerunHLT: ntupler when HLT rerunning is done

import FWCore.ParameterSet.Config as cms

ntuplerBase = cms.EDAnalyzer("MuonHLTNtupler",
	# -- rerun HLT?
	rerunHLT = cms.bool(False),
	# -- save rerun object?
	doSaveRerunObject = cms.bool(False),

	# -- miniAOD information
	isMiniAOD             = cms.bool(False),
	triggerObject_miniAOD = cms.untracked.InputTag("notUsed"), # -- only used when isMiniAOD == True

	# -- Offline information
	L1Muon            = cms.untracked.InputTag("gmtStage2Digis", "Muon", "RECO"), # -- if L1 is not emulated
	triggerResults    = cms.untracked.InputTag("TriggerResults::HLT"),
	triggerEvent      = cms.untracked.InputTag("hltTriggerSummaryAOD::HLT"),
	offlineLumiScaler = cms.untracked.InputTag("scalersRawToDigi"),
	offlineVertex     = cms.untracked.InputTag("offlinePrimaryVertices"),
	offlineMuon       = cms.untracked.InputTag("muons"),
	# -- MTD study
	generalTrack        = cms.untracked.InputTag("generalTracks"),
	trackTime           = cms.untracked.InputTag("tofPID", "t0safe"),
	trackTimeError      = cms.untracked.InputTag("tofPID", "sigmat0safe"),
	trackTimeQualityMVA = cms.untracked.InputTag("mtdTrackQualityMVA", "mtdQualMVA"),

	# -- generator information
	PUSummaryInfo = cms.untracked.InputTag("addPileupInfo"),
	genEventInfo = cms.untracked.InputTag("generator"),
	genParticle = cms.untracked.InputTag("genParticles"),

	# -- Rerun object: ignored
	myTriggerResults = cms.untracked.InputTag(""),
	myTriggerEvent   = cms.untracked.InputTag(""),
	lumiScaler       = cms.untracked.InputTag(""),

	L2Muon = cms.untracked.InputTag(""),
	L3Muon = cms.untracked.InputTag(""),
	TkMuon = cms.untracked.InputTag(""),

	ECALIsoMap = cms.untracked.InputTag(""),
	HCALIsoMap = cms.untracked.InputTag(""),
	trkIsoMap  = cms.untracked.InputTag(""),

	rho_ECAL = cms.untracked.InputTag(""),
	rho_HCAL = cms.untracked.InputTag(""),

	iterL3OI        = cms.untracked.InputTag(""),
	iterL3IOFromL2  = cms.untracked.InputTag(""),
	iterL3FromL2    = cms.untracked.InputTag(""),
	iterL3IOFromL1  = cms.untracked.InputTag(""),
	iterL3MuonNoID  = cms.untracked.InputTag(""),

	# -- for HLTPrescaleProvider
	l1tAlgBlkInputTag     = cms.InputTag("gtStage2Digis"), # -- for L1TGlobalUtil
	l1tExtBlkInputTag     = cms.InputTag("gtStage2Digis"), # -- for L1TGlobalUtil
	ReadPrescalesFromFile = cms.bool( False ),             # -- for L1TGlobalUtil
	stageL1Trigger        = cms.uint32( 2 ),               # -- newly added in higher CMSSW version

	# -- for the extrapolation of offlie muon to 2nd muon station
	# preselection = cms.string("gmtMuonCand.quality > 1"), # FIXME: maybe exclude CSC-only region?
	# useTrack  = cms.string("tracker"),
	# useState  = cms.string("atVertex"),
	# maxDeltaR   = cms.double(1.5),             ## FIXME: to be tuned
	# maxDeltaEta = cms.double(0.3),             ## FIXME: to be tuned
	# l1PhiOffset = cms.double(1.25 * 3.14159265359/180.),
	# useSimpleGeometry = cms.bool(True),
	# fallbackToME1     = cms.bool(True),

	# -- for the extrapolation of offlie muon to 2nd muon station: new version (works in higher CMSSW version)
	# -- from https://github.com/cms-sw/cmssw/blob/master/L1Trigger/L1TNtuples/python/l1MuonRecoTree_cfi.py
	useTrack = cms.string("tracker"),  # 'none' to use Candidate P4; or 'tracker', 'muon', 'global'
	useState = cms.string("atVertex"), # 'innermost' and 'outermost' require the TrackExtra
	useSimpleGeometry = cms.bool(True),
	useStation2 = cms.bool(True),
	fallbackToME1 = cms.bool(False),
	cosmicPropagationHypothesis = cms.bool(False),
	useMB2InOverlap = cms.bool(False),
	propagatorAlong = cms.ESInputTag("", "SteppingHelixPropagatorAlong"),
	propagatorAny = cms.ESInputTag("", "SteppingHelixPropagatorAny"),
	propagatorOpposite = cms.ESInputTag("", "SteppingHelixPropagatorOpposite")
)

ntuplerBaseRerunHLT = cms.EDAnalyzer("MuonHLTNtupler",
	# -- rerun HLT?
	rerunHLT = cms.bool(True),
	# -- save rerun object?
	doSaveRerunObject = cms.bool(True),

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

	ECALIsoMap = cms.untracked.InputTag("hltMuonEcalMFPFClusterIsoForMuons",  "",              "MYHLT"),
	HCALIsoMap = cms.untracked.InputTag("hltMuonHcalRegPFClusterIsoForMuons", "",              "MYHLT"),
	trkIsoMap  = cms.untracked.InputTag("hltMuonTkRelIsolationCut0p07Map",   "trkIsoDeposits", "MYHLT"),

	rho_ECAL = cms.untracked.InputTag("hltFixedGridRhoFastjetECALMFForMuons", "", "MYHLT"),
	rho_HCAL = cms.untracked.InputTag("hltFixedGridRhoFastjetHCAL",           "", "MYHLT"),

	iterL3OI        = cms.untracked.InputTag("hltL3MuonsIterL3OI",                   "", "MYHLT"),
	iterL3IOFromL2  = cms.untracked.InputTag("hltL3MuonsIterL3IO",                   "", "MYHLT"),
	iterL3FromL2    = cms.untracked.InputTag("hltIterL3MuonsFromL2LinksCombination", "", "MYHLT"),
	iterL3IOFromL1  = cms.untracked.InputTag("hltIter3IterL3FromL1MuonMerged",       "", "MYHLT"),
	iterL3MuonNoID  = cms.untracked.InputTag("hltIterL3MuonsNoID",                   "", "MYHLT"),

	# -- miniAOD information
	isMiniAOD             = cms.bool(False),
	triggerObject_miniAOD = cms.untracked.InputTag("notUsed"), # -- only used when isMiniAOD == True

	# -- Offline information
	triggerResults    = cms.untracked.InputTag("TriggerResults::HLT"),
	triggerEvent      = cms.untracked.InputTag("hltTriggerSummaryAOD::HLT"),
	offlineLumiScaler = cms.untracked.InputTag("scalersRawToDigi"),
	offlineVertex     = cms.untracked.InputTag("offlinePrimaryVertices"),
	offlineMuon       = cms.untracked.InputTag("muons"),
	# -- MTD study
	generalTrack        = cms.untracked.InputTag("generalTracks"),
	trackTime           = cms.untracked.InputTag("tofPID", "t0safe"),
	trackTimeError      = cms.untracked.InputTag("tofPID", "sigmat0safe"),
	trackTimeQualityMVA = cms.untracked.InputTag("mtdTrackQualityMVA", "mtdQualMVA"),
	
	# -- generator information
	PUSummaryInfo = cms.untracked.InputTag("addPileupInfo"),
	genEventInfo = cms.untracked.InputTag("generator"),
	genParticle = cms.untracked.InputTag("genParticles"),

	# -- for HLTPrescaleProvider
	l1tAlgBlkInputTag     = cms.InputTag("gtStage2Digis"), # -- for L1TGlobalUtil
	l1tExtBlkInputTag     = cms.InputTag("gtStage2Digis"), # -- for L1TGlobalUtil
	ReadPrescalesFromFile = cms.bool( False ),             # -- for L1TGlobalUtil
	stageL1Trigger        = cms.uint32( 2 ),               # -- newly added in higher CMSSW version

	# -- for the extrapolation of offlie muon to 2nd muon station
	# preselection = cms.string("gmtMuonCand.quality > 1"), # FIXME: maybe exclude CSC-only region?
	# useTrack  = cms.string("tracker"),
	# useState  = cms.string("atVertex"),
	# maxDeltaR   = cms.double(1.5),             ## FIXME: to be tuned
	# maxDeltaEta = cms.double(0.3),             ## FIXME: to be tuned
	# l1PhiOffset = cms.double(1.25 * 3.14159265359/180.),
	# useSimpleGeometry = cms.bool(True),
	# fallbackToME1     = cms.bool(True),

	# -- for the extrapolation of offlie muon to 2nd muon station: new version (works in higher CMSSW version)
	# -- from https://github.com/cms-sw/cmssw/blob/master/L1Trigger/L1TNtuples/python/l1MuonRecoTree_cfi.py
	useTrack = cms.string("tracker"),  # 'none' to use Candidate P4; or 'tracker', 'muon', 'global'
	useState = cms.string("atVertex"), # 'innermost' and 'outermost' require the TrackExtra
	useSimpleGeometry = cms.bool(True),
	useStation2 = cms.bool(True),
	fallbackToME1 = cms.bool(False),
	cosmicPropagationHypothesis = cms.bool(False),
	useMB2InOverlap = cms.bool(False),
	propagatorAlong = cms.ESInputTag("", "SteppingHelixPropagatorAlong"),
	propagatorAny = cms.ESInputTag("", "SteppingHelixPropagatorAny"),
	propagatorOpposite = cms.ESInputTag("", "SteppingHelixPropagatorOpposite")
)
