from CRABClient.UserUtilities import config
config = config()

config.General.requestName = ''


config.JobType.pluginName = 'Analysis'
config.JobType.numCores = 1
# config.JobType.maxMemoryMB = 2500
# config.JobType.maxJobRuntimeMin = 2000

config.Data.inputDataset = ''
# config.Data.useParent = True

config.Data.inputDBS = 'global'
config.Data.publication = False

# config.Data.splitting = 'Automatic' # -- it is not preferred: test job is frequently failed due to memory
# config.Data.splitting = 'FileBased'
# config.Data.unitsPerJob = 1
config.Data.splitting = 'EventAwareLumiBased'
config.Data.unitsPerJob = 3000 # -- ~3000 events per job --> 1000 jobs for ZMuMu_M50to120 (3M events)

config.Site.storageSite = 'T2_BE_IIHE'

# config.Data.lumiMask = './Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON_Last5invfb.txt'

# config.JobType.allowUndistributedCMSSW = True

config.JobType.psetName = 'HLTConfig_newParam_2_20.py' # -- should be filled
version = 'v11_newParam_nVtx_nRegion_2_20'

config.General.workArea = 'CRABDir_%s' % version
config.Data.outLFNDirBase = '/store/user/kplee/MuonHLTTree_%s' % version


# 'MultiCRAB' part
if __name__ == '__main__':
    
    from CRABAPI.RawCommand import crabCommand

    config.General.requestName = 'ZToMuMu_M50to120_Powheg_Summer21'
    config.Data.inputDataset = '/ZToMuMu_M-50To120_TuneCP5_14TeV-powheg-pythia8/Run3Summer21MiniAOD-120X_mcRun3_2021_realistic_v5-v2/MINIAODSIM'
    config.Data.secondaryInputDataset = '/ZToMuMu_M-50To120_TuneCP5_14TeV-powheg-pythia8/Run3Summer21DRPremix-120X_mcRun3_2021_realistic_v6-v2/GEN-SIM-DIGI-RAW'
    crabCommand('submit', config = config)

    # -- QCD
    config.General.requestName = 'QCDMuEnriched_Pt20to30_Pythia8_Summer21'
    config.Data.inputDataset = '/QCD_Pt-20To30_MuEnrichedPt5_TuneCP5_14TeV-pythia8/Run3Summer21MiniAOD-120X_mcRun3_2021_realistic_v5-v2/MINIAODSIM'
    config.Data.secondaryInputDataset = '/QCD_Pt-20To30_MuEnrichedPt5_TuneCP5_14TeV-pythia8/Run3Summer21DRPremix-120X_mcRun3_2021_realistic_v6-v2/GEN-SIM-DIGI-RAW'
    crabCommand('submit', config = config)

    config.General.requestName = 'QCDMuEnriched_Pt30to50_Pythia8_Summer21'
    config.Data.inputDataset = '/QCD_Pt-30To50_MuEnrichedPt5_TuneCP5_14TeV-pythia8/Run3Summer21MiniAOD-120X_mcRun3_2021_realistic_v5-v2/MINIAODSIM'
    config.Data.secondaryInputDataset = '/QCD_Pt-30To50_MuEnrichedPt5_TuneCP5_14TeV-pythia8/Run3Summer21DRPremix-120X_mcRun3_2021_realistic_v6-v2/GEN-SIM-DIGI-RAW'
    crabCommand('submit', config = config)

    config.General.requestName = 'QCDMuEnriched_Pt50to80_Pythia8_Summer21'
    config.Data.inputDataset = '/QCD_Pt-50To80_MuEnrichedPt5_TuneCP5_14TeV-pythia8/Run3Summer21MiniAOD-120X_mcRun3_2021_realistic_v5-v2/MINIAODSIM'
    config.Data.secondaryInputDataset = '/QCD_Pt-50To80_MuEnrichedPt5_TuneCP5_14TeV-pythia8/Run3Summer21DRPremix-120X_mcRun3_2021_realistic_v6-v2/GEN-SIM-DIGI-RAW'
    crabCommand('submit', config = config)

    config.General.requestName = 'QCDMuEnriched_Pt80to120_Pythia8_Summer21'
    config.Data.inputDataset = '/QCD_Pt-80To120_MuEnrichedPt5_TuneCP5_14TeV-pythia8/Run3Summer21MiniAOD-120X_mcRun3_2021_realistic_v5-v2/MINIAODSIM'
    config.Data.secondaryInputDataset = '/QCD_Pt-80To120_MuEnrichedPt5_TuneCP5_14TeV-pythia8/Run3Summer21DRPremix-120X_mcRun3_2021_realistic_v6-v2/GEN-SIM-DIGI-RAW'
    crabCommand('submit', config = config)

    config.General.requestName = 'QCDMuEnriched_Pt120to170_Pythia8_Summer21'
    config.Data.inputDataset = '/QCD_Pt-120To170_MuEnrichedPt5_TuneCP5_14TeV-pythia8/Run3Summer21MiniAOD-120X_mcRun3_2021_realistic_v5-v2/MINIAODSIM'
    config.Data.secondaryInputDataset = '/QCD_Pt-120To170_MuEnrichedPt5_TuneCP5_14TeV-pythia8/Run3Summer21DRPremix-120X_mcRun3_2021_realistic_v6-v2/GEN-SIM-DIGI-RAW'
    crabCommand('submit', config = config)

