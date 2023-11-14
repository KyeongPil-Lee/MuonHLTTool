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
config.Data.unitsPerJob = 4000 # -- ~4000 events per job --> ~2500 jobs for DY (~10M events)

config.Site.storageSite = 'T2_BE_IIHE'

# config.Data.lumiMask = './Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON_Last5invfb.txt'

# config.JobType.allowUndistributedCMSSW = True

config.JobType.psetName = 'hltConfig_MC_ntupler.py' # -- should be filled
version = 'v1'

config.General.workArea = 'CRABDir_%s' % version
config.Data.outLFNDirBase = '/store/user/kplee/MuonHLTTree/Run3_IDIso_132X_%s' % version


# 'MultiCRAB' part
if __name__ == '__main__':
    
    from CRABAPI.RawCommand import crabCommand

    config.General.requestName = 'DYTo2L_M50_Winter23'
    config.Data.inputDataset = '/DYTo2L_MLL-50_TuneCP5_13p6TeV_pythia8/Run3Winter23MiniAOD-KeepSi_RnD_126X_mcRun3_2023_forPU65_v1-v2/MINIAODSIM'
    config.Data.secondaryInputDataset = '/DYTo2L_MLL-50_TuneCP5_13p6TeV_pythia8/Run3Winter23Digi-KeepSi_RnD_126X_mcRun3_2023_forPU65_v1-v2/GEN-SIM-RAW'
    crabCommand('submit', config = config)

    # -- QCD
    config.General.requestName = 'QCDMuEnriched_Pt20to30_Winter23'
    config.Data.inputDataset = '/QCD_PT-20to30_MuEnrichedPT5_TuneCP5_13p6TeV_pythia8/Run3Winter23MiniAOD-126X_mcRun3_2023_forPU65_v1-v2/MINIAODSIM'
    config.Data.secondaryInputDataset = '/QCD_PT-20to30_MuEnrichedPT5_TuneCP5_13p6TeV_pythia8/Run3Winter23Digi-126X_mcRun3_2023_forPU65_v1-v2/GEN-SIM-RAW'
    crabCommand('submit', config = config)

    config.General.requestName = 'QCDMuEnriched_Pt30to50_Winter23'
    config.Data.inputDataset = '/QCD_PT-30to50_MuEnrichedPT5_TuneCP5_13p6TeV_pythia8/Run3Winter23MiniAOD-126X_mcRun3_2023_forPU65_v1-v2/MINIAODSIM'
    config.Data.secondaryInputDataset = '/QCD_PT-30to50_MuEnrichedPT5_TuneCP5_13p6TeV_pythia8/Run3Winter23Digi-126X_mcRun3_2023_forPU65_v1-v2/GEN-SIM-RAW'
    crabCommand('submit', config = config)

    config.General.requestName = 'QCDMuEnriched_Pt50to80_Winter23'
    config.Data.inputDataset = '/QCD_PT-50to80_MuEnrichedPT5_TuneCP5_13p6TeV_pythia8/Run3Winter23MiniAOD-126X_mcRun3_2023_forPU65_v1-v2/MINIAODSIM'
    config.Data.secondaryInputDataset = '/QCD_PT-50to80_MuEnrichedPT5_TuneCP5_13p6TeV_pythia8/Run3Winter23Digi-126X_mcRun3_2023_forPU65_v1-v2/GEN-SIM-RAW'
    crabCommand('submit', config = config)

    config.General.requestName = 'QCDMuEnriched_Pt80to120_Winter23'
    config.Data.inputDataset = '/QCD_PT-80to120_MuEnrichedPT5_TuneCP5_13p6TeV_pythia8/Run3Winter23MiniAOD-126X_mcRun3_2023_forPU65_v1-v2/MINIAODSIM'
    config.Data.secondaryInputDataset = '/QCD_PT-80to120_MuEnrichedPT5_TuneCP5_13p6TeV_pythia8/Run3Winter23Digi-126X_mcRun3_2023_forPU65_v1-v2/GEN-SIM-RAW'
    crabCommand('submit', config = config)

    config.General.requestName = 'QCDMuEnriched_Pt120to170_Winter23'
    config.Data.inputDataset = '/QCD_PT-120to170_MuEnrichedPT5_TuneCP5_13p6TeV_pythia8/Run3Winter23MiniAOD-126X_mcRun3_2023_forPU65_v1-v2/MINIAODSIM'
    config.Data.secondaryInputDataset = '/QCD_PT-120to170_MuEnrichedPT5_TuneCP5_13p6TeV_pythia8/Run3Winter23Digi-126X_mcRun3_2023_forPU65_v1-v2/GEN-SIM-RAW'
    crabCommand('submit', config = config)