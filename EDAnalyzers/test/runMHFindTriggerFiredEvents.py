import FWCore.ParameterSet.Config as cms

process = cms.Process("MuonHLT")

process.load('FWCore.MessageLogger.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.source = cms.Source("PoolSource",
	fileNames = cms.untracked.vstring('file:/afs/cern.ch/user/k/kplee/work/private/ROOTFile_Test/GENSIMRAW_ttbar_92XForTSG.root'),
	secondaryFileNames = cms.untracked.vstring(),
)

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(100))

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
process.GlobalTag.globaltag = '92X_upgrade2017_realistic_v10'

process.load("Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff")
process.load('Configuration.Geometry.GeometryRecoDB_cff')

from MuonHLTTool.EDAnalyzers.MHFindTriggerFiredEvents_cfi import *

process.myMHFindTriggerFiredEvents = MHFindTriggerFiredEvents.clone()
# process.myMHFindTriggerFiredEvents.triggerResults = cms.untracked.InputTag("TriggerResults::HLT")
# process.myMHFindTriggerFiredEvents.trigName = cms.untracked.string("HLT_IsoMu27_v")

process.mypath = cms.Path(process.myMHFindTriggerFiredEvents)