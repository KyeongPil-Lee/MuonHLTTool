import FWCore.ParameterSet.Config as cms

# -- update inputTag if given sample is miniAOD
def customizeModule_miniAOD(ntupler):
    ntupler.isMiniAOD             = cms.bool(True)
    ntupler.offlineVertex         = cms.untracked.InputTag("offlineSlimmedPrimaryVertices")
    ntupler.offlineMuon           = cms.untracked.InputTag("slimmedMuons")
    ntupler.triggerObject_miniAOD = cms.untracked.InputTag("slimmedPatTrigger")
    ntupler.PUSummaryInfo         = cms.untracked.InputTag("slimmedAddPileupInfo")
    ntupler.genParticle           = cms.untracked.InputTag("prunedGenParticles")

# -- add ntuple module (HLT-rerun mode) to given process: 
# ---- it is usually used at the end of the HLT menu to produce ntuples
# -- add two lines in the HLT menu config.:
# from MuonHLTTool.MuonHLTNtupler.Customizer_MuonHLTNtuple import *
# process = addNtupleModuleToProcess(process, "MYHLT", isMiniAOD=False)
def addNtupleModuleToProcess(process, newProcessName = "MYHLT", isMiniAOD=False):
    if hasattr(process, "DQMOutput"):
        del process.DQMOutput

    # -- for the extrapolation of offlie muon to 2nd muon station
    process.load("TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorAny_cfi")
    process.load("TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorAlong_cfi")
    process.load("TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorOpposite_cfi")
    
    from MuonHLTTool.MuonHLTNtupler.ntupler_cfi import ntuplerBase,ntuplerBaseRerunHLT
    # process.ntupler = ntuplerBase.clone()
    process.ntupler = ntuplerBaseRerunHLT.clone()

    # -- set to the new process name
    process.ntupler.myTriggerResults = cms.untracked.InputTag("TriggerResults",          "",     newProcessName)
    process.ntupler.myTriggerEvent   = cms.untracked.InputTag("hltTriggerSummaryAOD",    "",     newProcessName)
    process.ntupler.lumiScaler       = cms.untracked.InputTag("hltScalersRawToDigi",     "",     newProcessName)

    process.ntupler.L1Muon           = cms.untracked.InputTag("hltGtStage2Digis",        "Muon", newProcessName)
    process.ntupler.L2Muon           = cms.untracked.InputTag("hltL2MuonCandidates",     "",     newProcessName)
    process.ntupler.L3Muon           = cms.untracked.InputTag("hltIterL3MuonCandidates", "",     newProcessName)
    process.ntupler.TkMuon           = cms.untracked.InputTag("hltHighPtTkMuonCands",    "",     newProcessName)

    process.ntupler.ECALIsoMap = cms.untracked.InputTag("hltMuonEcalMFPFClusterIsoForMuons",  "",               newProcessName)
    process.ntupler.HCALIsoMap = cms.untracked.InputTag("hltMuonHcalRegPFClusterIsoForMuons", "",               newProcessName)
    process.ntupler.trkIsoMap  = cms.untracked.InputTag("hltMuonTkRelIsolationCut0p08Map",    "trkIsoDeposits", newProcessName)

    process.ntupler.iterL3MuonNoID.iterL3OI        = cms.untracked.InputTag("hltL3MuonsIterL3OI",                   "", newProcessName)
    process.ntupler.iterL3MuonNoID.iterL3IOFromL2  = cms.untracked.InputTag("hltL3MuonsIterL3IO",                   "", newProcessName)
    process.ntupler.iterL3MuonNoID.iterL3FromL2    = cms.untracked.InputTag("hltIterL3MuonsFromL2LinksCombination", "", newProcessName)
    process.ntupler.iterL3MuonNoID.iterL3IOFromL1  = cms.untracked.InputTag("hltIter3IterL3FromL1MuonMerged",       "", newProcessName)
    process.ntupler.iterL3MuonNoID                 = cms.untracked.InputTag("hltIterL3MuonsNoID",                   "", newProcessName)

    if isMiniAOD:
        customizeModule_miniAOD(process.ntupler)


    process.TFileService = cms.Service("TFileService",
      fileName = cms.string("ntuple.root"),
      closeFileFast = cms.untracked.bool(False),
      )

    process.mypath = cms.EndPath(process.ntupler)
    process.schedule.extend([process.mypath])

    return process

