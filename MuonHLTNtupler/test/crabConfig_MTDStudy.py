from CRABClient.UserUtilities import config
config = config()

config.General.requestName = ''


config.JobType.pluginName = 'Analysis'
config.JobType.numCores = 1
# config.JobType.maxMemoryMB = 2500
# config.JobType.maxJobRuntimeMin = 2000

config.Data.inputDataset = ''
# config.Data.useParent = True

# config.Data.inputDBS = 'global'
config.Data.inputDBS = 'phys03' # -- USER data
config.Data.publication = False

# config.Data.splitting = 'Automatic' # -- it is not preferred: test job is frequently failed due to memory
# config.Data.splitting = 'FileBased'
# config.Data.unitsPerJob = 1
config.Data.splitting = 'EventAwareLumiBased'
config.Data.unitsPerJob = 3000

config.Site.storageSite = 'T2_BE_IIHE'

# config.Data.lumiMask = './Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON_Last5invfb.txt'

# config.JobType.allowUndistributedCMSSW = True

config.JobType.psetName = 'Run_ntupler_MTDStudy.py' # -- should be filled
version = 'v2'

config.General.workArea = 'CRABDir_%s' % version
config.Data.outLFNDirBase = '/store/user/kplee/MuonHLTTree_phase2MTD_%s' % version


# 'MultiCRAB' part
if __name__ == '__main__':
    
    from CRABAPI.RawCommand import crabCommand

    # # -- DY
    # config.General.requestName = 'DYLL_M50_Pythia8'
    # config.Data.inputDataset = '/DYToLL_M-50_TuneCP5_14TeV-pythia8/PhaseIISpring22DRMiniAOD-PU200_BSzpz35_123X_mcRun4_realistic_v11_ext1-v1/GEN-SIM-RECO'
    # crabCommand('submit', config = config)

    # # -- ttbar
    # config.General.requestName = 'TTSemiLeptonic_Powheg'
    # config.Data.inputDataset = '/TTToSemileptonic_TuneCP5_14TeV-powheg-pythia8/PhaseIISpring22DRMiniAOD-PU200_123X_mcRun4_realistic_v11-v1/GEN-SIM-RECO'
    # crabCommand('submit', config = config)

    # -- rereco-d
    # -- DY
    config.General.requestName = 'DYLL_M50_Pythia8'
    config.Data.inputDataset = '/DYToLL_M-50_TuneCP5_14TeV-pythia8/phys_egamma-reRECO_MiniAODmod-7964208c5865353948ffb7081c695345/USER'
    crabCommand('submit', config = config)

    # -- ttbar
    config.General.requestName = 'TTSemiLeptonic_Powheg'
    config.Data.inputDataset = '/TTToSemileptonic_TuneCP5_14TeV-powheg-pythia8/phys_egamma-reRECO_MiniAODmod-7964208c5865353948ffb7081c695345/USER'
    crabCommand('submit', config = config)


    # -- no general track information: cannot calculate tracker isolation at the ntuple level
    # # -- QCD, inclusive
    # config.General.requestName = 'QCDMuEnriched_Pt15to3000_Pythia8'
    # config.Data.inputDataset = '/QCD_Pt-15To3000_MuEnrichedPt5_TuneCP5_14TeV-pythia8/PhaseIISpring22DRMiniAOD-PU200_123X_mcRun4_realistic_v11-v1/GEN-SIM-DIGI-RAW-MINIAOD'
    # crabCommand('submit', config = config)    

    # # -- QCD, pt-binned
    # config.General.requestName = 'QCDMuEnriched_Pt15to20_Pythia8'
    # config.Data.inputDataset = '/QCD_Pt-15To20_MuEnrichedPt5_TuneCP5_14TeV-pythia8/PhaseIISpring22DRMiniAOD-PU200_123X_mcRun4_realistic_v11-v1/GEN-SIM-DIGI-RAW-MINIAOD'
    # crabCommand('submit', config = config)

    # config.General.requestName = 'QCDMuEnriched_Pt20to30_Pythia8'
    # config.Data.inputDataset = '/QCD_Pt-20To30_MuEnrichedPt5_TuneCP5_14TeV-pythia8/PhaseIISpring22DRMiniAOD-PU200_123X_mcRun4_realistic_v11-v1/GEN-SIM-DIGI-RAW-MINIAOD'
    # crabCommand('submit', config = config)

    # config.General.requestName = 'QCDMuEnriched_Pt30to50_Pythia8'
    # config.Data.inputDataset = '/QCD_Pt-30To50_MuEnrichedPt5_TuneCP5_14TeV-pythia8/PhaseIISpring22DRMiniAOD-PU200_123X_mcRun4_realistic_v11-v1/GEN-SIM-DIGI-RAW-MINIAOD'
    # crabCommand('submit', config = config)

    # config.General.requestName = 'QCDMuEnriched_Pt50to80_Pythia8'
    # config.Data.inputDataset = '/QCD_Pt-50To80_MuEnrichedPt5_TuneCP5_14TeV-pythia8/PhaseIISpring22DRMiniAOD-PU200_123X_mcRun4_realistic_v11-v1/GEN-SIM-DIGI-RAW-MINIAOD'
    # crabCommand('submit', config = config)

    # config.General.requestName = 'QCDMuEnriched_Pt80to120_Pythia8'
    # config.Data.inputDataset = '/QCD_Pt-80To120_MuEnrichedPt5_TuneCP5_14TeV-pythia8/PhaseIISpring22DRMiniAOD-PU200_123X_mcRun4_realistic_v11-v1/GEN-SIM-DIGI-RAW-MINIAOD'
    # crabCommand('submit', config = config)

    # config.General.requestName = 'QCDMuEnriched_Pt120to170_Pythia8'
    # config.Data.inputDataset = '/QCD_Pt-120To170_MuEnrichedPt5_TuneCP5_14TeV-pythia8/PhaseIISpring22DRMiniAOD-PU200_123X_mcRun4_realistic_v11-v1/GEN-SIM-DIGI-RAW-MINIAOD'
    # crabCommand('submit', config = config)

    # config.General.requestName = 'QCDMuEnriched_Pt170to300_Pythia8'
    # config.Data.inputDataset = '/QCD_Pt-170To300_MuEnrichedPt5_TuneCP5_14TeV-pythia8/PhaseIISpring22DRMiniAOD-PU200_123X_mcRun4_realistic_v11-v1/GEN-SIM-DIGI-RAW-MINIAOD'
    # crabCommand('submit', config = config)

    # config.General.requestName = 'QCDMuEnriched_Pt300toInf_Pythia8'
    # config.Data.inputDataset = '/QCD_Pt-300ToInf_MuEnrichedPt5_TuneCP5_14TeV-pythia8/PhaseIISpring22DRMiniAOD-PU200_123X_mcRun4_realistic_v11-v1/GEN-SIM-DIGI-RAW-MINIAOD'
    # crabCommand('submit', config = config)

