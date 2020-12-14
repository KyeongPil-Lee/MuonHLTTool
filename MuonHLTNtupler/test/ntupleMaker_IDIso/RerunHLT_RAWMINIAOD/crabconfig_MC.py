from CRABClient.UserUtilities import config
config = config()

config.General.requestName = ''
config.General.workArea = 'CRABDir'

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'HLTConfig_MC_RAWMINIAOD.py'
config.JobType.numCores = 4
# config.JobType.maxMemoryMB = 2500
# config.JobType.maxJobRuntimeMin = 2000

config.Data.inputDataset = ''
# config.Data.useParent = True

config.Data.inputDBS = 'global'
config.Data.publication = False

# config.Data.splitting = 'Automatic' # -- it is not preferred: test job is frequently failed due to memory
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1

config.Site.storageSite = 'T3_KR_KNU'

# config.Data.lumiMask = './Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON_Last5invfb.txt'

config.JobType.allowUndistributedCMSSW = True

version = 'v02'
config.Data.outLFNDirBase = '/store/user/kplee/MuonHLTTree_%s' % version

# 'MultiCRAB' part
if __name__ == '__main__':
    
    from CRABAPI.RawCommand import crabCommand

    # config.General.requestName = 'DYLL_M50_Pythia8'
    # config.Data.inputDataset = '/DYToLL_M-50_TuneCP5_14TeV-pythia8/Run3Winter20DRMiniAOD-DRFlatPU30to80_110X_mcRun3_2021_realistic_v6-v2/MINIAODSIM'
    # config.Data.secondaryInputDataset = '/DYToLL_M-50_TuneCP5_14TeV-pythia8/Run3Winter20DRMiniAOD-DRFlatPU30to80_110X_mcRun3_2021_realistic_v6-v2/GEN-SIM-RAW'
    # crabCommand('submit', config = config)

    config.General.requestName = 'DYLL_M50_Pythia8_HCAL'
    config.Data.inputDataset = '/DYToLL_M-50_TuneCP5_14TeV-pythia8_HCAL/Run3Winter20DRPremixMiniAOD-110X_mcRun3_2021_realistic_v6-v2/MINIAODSIM'
    config.Data.secondaryInputDataset = '/DYToLL_M-50_TuneCP5_14TeV-pythia8_HCAL/Run3Winter20DRPremixMiniAOD-110X_mcRun3_2021_realistic_v6-v2/GEN-SIM-DIGI-RAW'
    crabCommand('submit', config = config)

    config.General.requestName = 'QCD_Pt15to7000_MuEnriched_Flat_Pythia8'
    config.Data.inputDataset = '/QCD_Pt_15to7000_TuneCP5_muEnriched_Flat_14TeV_pythia8/Run3Winter20DRMiniAOD-FlatPU0to80ALCARECO_110X_mcRun3_2021_realistic_v6-v2/MINIAODSIM'
    config.Data.secondaryInputDataset = '/QCD_Pt_15to7000_TuneCP5_muEnriched_Flat_14TeV_pythia8/Run3Winter20DRMiniAOD-FlatPU0to80ALCARECO_110X_mcRun3_2021_realistic_v6-v2/GEN-SIM-DIGI-RAW'
    crabCommand('submit', config = config)