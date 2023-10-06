# ----------------------#
flag_HLTRerun = False
flag_miniAOD = False
# ----------------------#

import FWCore.ParameterSet.Config as cms

process = cms.Process("ntupler")

exampleFile = ""
if flag_miniAOD:
    # exampleFile = "/store/mc/PhaseIISpring22DRMiniAOD/DYToLL_M-50_TuneCP5_14TeV-pythia8/GEN-SIM-RECO/PU200_BSzpz35_123X_mcRun4_realistic_v11_ext1-v1/2560000/d0ff9320-6ae0-4e8d-a9ae-936c2aec1f4d.root"
    # exampleFile = "/store/mc/PhaseIISpring22DRMiniAOD/DYToLL_M-50_TuneCP5_14TeV-pythia8/GEN-SIM-RECO/PU200_BSzpz35_123X_mcRun4_realistic_v11_ext1-v1/2560001/046707b0-dc03-47c1-86d9-fb720422bead.root"
    # exampleFile = "/store/mc/PhaseIISpring22DRMiniAOD/DYToLL_M-50_TuneCP5_14TeV-pythia8/GEN-SIM-RECO/PU200_BSzpz35_123X_mcRun4_realistic_v11_ext1-v1/2560000/7e04f9a3-4bf9-459f-b8a6-532cd1ceb071.root"
    # exampleFile = "/store/mc/PhaseIISpring22DRMiniAOD/QCD_Pt-15To3000_MuEnrichedPt5_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW-MINIAOD/PU200_123X_mcRun4_realistic_v11-v1/2560000/9dceb0c4-fd50-4ae1-a528-7902514a9d21.root"
    # exampleFile = "/store/mc/PhaseIISpring22DRMiniAOD/QCD_Pt-15To20_MuEnrichedPt5_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW-MINIAOD/PU200_123X_mcRun4_realistic_v11-v1/2560000/de74c8da-a1af-40a6-96a4-bc5a9d1a1567.root"
    # exampleFile = "/store/mc/PhaseIISpring22DRMiniAOD/DYToLL_M-50_TuneCP5_14TeV-pythia8/GEN-SIM-RECO/PU200_BSzpz35_123X_mcRun4_realistic_v11_ext1-v1/2560001/fe36920d-652e-438e-a507-8497da3bc8a9.root"
    exampleFile = "/store/mc/Phase2Spring23DIGIRECOMiniAOD/DYToLL_M-50_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW-MINIAOD/PU200_Trk1GeV_131X_mcRun4_realistic_v5-v1/30001/66c73ce1-44ac-4f0c-afc4-97656eb26951.root"
else:
  exampleFile = "/store/relval/CMSSW_13_1_0/RelValZMM_14/GEN-SIM-RECO/PU_131X_mcRun4_realistic_v5_2026D95PU200-v1/2590000/44be928e-2b59-4501-8b9c-d505695dbd84.root"

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(exampleFile),
    secondaryFileNames = cms.untracked.vstring(),
    # lumisToProcess = cms.untracked.VLuminosityBlockRange('258158:1-258158:1786'),
)

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1000))

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
# process.GlobalTag.globaltag = '123X_mcRun4_realistic_v11'
process.GlobalTag.globaltag = '131X_mcRun4_realistic_v5'

process.load("Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff")
process.load('Configuration.Geometry.GeometryRecoDB_cff')

# -- ntupler -- #
if flag_HLTRerun:
  newProcessName = "MYHLT"
  
  from MuonHLTTool.MuonHLTNtupler.Customizer_MuonHLTNtuple import addNtupleModuleToProcess
  process = addNtupleModuleToProcess(process, newProcessName, flag_miniAOD)

else: # -- without HLT rerun

  # -- for the extrapolation of offlie muon to 2nd muon station
  process.load("TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorAny_cfi")
  process.load("TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorAlong_cfi")
  process.load("TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorOpposite_cfi")

  from MuonHLTTool.MuonHLTNtupler.ntupler_cfi import ntuplerBase
  process.ntupler = ntuplerBase.clone()

  if flag_miniAOD:
    from MuonHLTTool.MuonHLTNtupler.Customizer_MuonHLTNtuple import customizeModule_miniAOD
    customizeModule_miniAOD(process.ntupler)

  process.TFileService = cms.Service("TFileService",
    fileName = cms.string("ntuple.root"),
    closeFileFast = cms.untracked.bool(False),
    )

  process.mypath = cms.EndPath(process.ntupler)


process.MessageLogger = cms.Service( "MessageLogger",
	destinations = cms.untracked.vstring("cerr"),
	cerr = cms.untracked.PSet( threshold = cms.untracked.string('ERROR'), ),
	)

