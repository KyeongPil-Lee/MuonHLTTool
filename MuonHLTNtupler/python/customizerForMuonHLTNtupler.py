# -- custoimzer for ntupler that can be added to the HLT configuration for re-running HLT
# -- add two lines in the HLT config.:
# from MuonHLTTool.MuonHLTNtupler.customizerForMuonHLTNtupler import *
# process = customizerFuncForMuonHLTNtupler(process, "MYHLT")

import FWCore.ParameterSet.Config as cms

def customizerFuncForMuonHLTNtupler(process, newProcessName = "MYHLT"):
    if hasattr(process, "DQMOutput"):
        del process.DQMOutput


    # -- for the extrapolation of offlie muon to 2nd muon station
    process.load("TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorAny_cfi")
    process.load("TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorAlong_cfi")
    process.load("TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorOpposite_cfi")
    
    from MuonHLTTool.MuonHLTNtupler.ntupler_cfi import ntuplerBase
    process.ntupler = ntuplerBase.clone()

    # -- set to the new process name
    process.ntupler.myTriggerResults = cms.untracked.InputTag("TriggerResults",          "",     newProcessName)
    process.ntupler.myTriggerEvent   = cms.untracked.InputTag("hltTriggerSummaryAOD",    "",     newProcessName)
    process.ntupler.lumiScaler       = cms.untracked.InputTag("hltScalersRawToDigi",     "",     newProcessName)

    process.ntupler.L1Muon           = cms.untracked.InputTag("hltGtStage2Digis",        "Muon", newProcessName)
    process.ntupler.L2Muon           = cms.untracked.InputTag("hltL2MuonCandidates",     "",     newProcessName)
    process.ntupler.L3Muon           = cms.untracked.InputTag("hltIterL3MuonCandidates", "",     newProcessName)
    process.ntupler.TkMuon           = cms.untracked.InputTag("hltHighPtTkMuonCands",    "",     newProcessName)

    process.ntupler.iterL3MuonNoID.iterL3OI        = cms.untracked.InputTag("hltL3MuonsIterL3OI",                   "", newProcessName),
    process.ntupler.iterL3MuonNoID.iterL3IOFromL2  = cms.untracked.InputTag("hltL3MuonsIterL3IO",                   "", newProcessName),
    process.ntupler.iterL3MuonNoID.iterL3FromL2    = cms.untracked.InputTag("hltIterL3MuonsFromL2LinksCombination", "", newProcessName),
    process.ntupler.iterL3MuonNoID.iterL3IOFromL1  = cms.untracked.InputTag("hltIter3IterL3FromL1MuonMerged",       "", newProcessName),
    process.ntupler.iterL3MuonNoID                 = cms.untracked.InputTag("hltIterL3MuonsNoID",                   "", newProcessName)

    process.TFileService = cms.Service("TFileService",
      fileName = cms.string("ntuple.root"),
      closeFileFast = cms.untracked.bool(False),
      )

    process.mypath = cms.EndPath(process.ntupler)

    return process