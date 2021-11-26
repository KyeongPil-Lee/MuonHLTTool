from CRABClient.UserUtilities import config
config = config()

config.General.requestName = ''
config.General.workArea = 'CRABDir'

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'hlt_muon_default_Run3_mc.py' # -- should be filled
config.JobType.numCores = 1
# config.JobType.maxMemoryMB = 2500
# config.JobType.maxJobRuntimeMin = 2000

config.Data.inputDataset = ''
# config.Data.useParent = True

config.Data.inputDBS = 'global'
config.Data.publication = False

# config.Data.splitting = 'Automatic' # -- it is not preferred: test job is frequently failed due to memory
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1

config.Site.storageSite = 'T2_BE_IIHE'

# config.Data.lumiMask = './Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON_Last5invfb.txt'

config.JobType.allowUndistributedCMSSW = True

version = 'v01'
config.Data.outLFNDirBase = '/store/user/kplee/MuonHLTTree_%s' % version

# 'MultiCRAB' part
if __name__ == '__main__':
    
    from CRABAPI.RawCommand import crabCommand

    # -- 40M events... it would be too large to rerun HLT
    # config.General.requestName = 'DYToLL_M50_Pythia8_FlatPU30to80'
    # config.Data.inputDataset = '/DYToLL_M-50_TuneCP5_14TeV-pythia8/Run3Winter21DRMiniAOD-FlatPU30to80FEVT_112X_mcRun3_2021_realistic_v16-v2/MINIAODSIM'
    # config.Data.secondaryInputDataset = '/DYToLL_M-50_TuneCP5_14TeV-pythia8/Run3Winter21DRMiniAOD-FlatPU30to80FEVT_112X_mcRun3_2021_realistic_v16-v2/GEN-SIM-DIGI-RAW'
    # crabCommand('submit', config = config)

    config.General.requestName = 'ZToMuMu_M50to120_Powheg_FlatPU30to80'
    config.Data.inputDataset = '/ZToMuMu_M-50To120_TuneCP5_14TeV-powheg-pythia8/Run3Winter21DRMiniAOD-FlatPU30to80FEVT_112X_mcRun3_2021_realistic_v16-v2/MINIAODSIM'
    config.Data.secondaryInputDataset = '/ZToMuMu_M-50To120_TuneCP5_14TeV-powheg-pythia8/Run3Winter21DRMiniAOD-FlatPU30to80FEVT_112X_mcRun3_2021_realistic_v16-v2/GEN-SIM-DIGI-RAW'
    crabCommand('submit', config = config)

    # config.General.requestName = 'ZToMuMu_M120to200_Powheg_FlatPU30to80'
    # config.Data.inputDataset = '/ZToMuMu_M-120To200_TuneCP5_14TeV-powheg-pythia8/Run3Winter21DRMiniAOD-FlatPU30to80FEVT_112X_mcRun3_2021_realistic_v16-v2/MINIAODSIM'
    # config.Data.secondaryInputDataset = '/ZToMuMu_M-120To200_TuneCP5_14TeV-powheg-pythia8/Run3Winter21DRMiniAOD-FlatPU30to80FEVT_112X_mcRun3_2021_realistic_v16-v2/GEN-SIM-DIGI-RAW'
    # crabCommand('submit', config = config)

    # config.General.requestName = 'ZToMuMu_M200to400_Powheg_FlatPU30to80'
    # config.Data.inputDataset = '/ZToMuMu_M-200To400_TuneCP5_14TeV-powheg-pythia8/Run3Winter21DRMiniAOD-FlatPU30to80FEVT_112X_mcRun3_2021_realistic_v16-v2/MINIAODSIM'
    # config.Data.secondaryInputDataset = '/ZToMuMu_M-200To400_TuneCP5_14TeV-powheg-pythia8/Run3Winter21DRMiniAOD-FlatPU30to80FEVT_112X_mcRun3_2021_realistic_v16-v2/GEN-SIM-DIGI-RAW'
    # crabCommand('submit', config = config)


    # # -- WJetsToLNu
    # config.General.requestName = 'WJetsToLNu_aMCNLO_FlatPU30to80'
    # config.Data.inputDataset = '/WJetsToLNu_TuneCP5_14TeV-amcatnloFXFX-pythia8/Run3Winter21DRMiniAOD-FlatPU30to80FEVT_112X_mcRun3_2021_realistic_v16-v2/MINIAODSIM'
    # config.Data.secondaryInputDataset = '/WJetsToLNu_TuneCP5_14TeV-amcatnloFXFX-pythia8/Run3Winter21DRMiniAOD-FlatPU30to80FEVT_112X_mcRun3_2021_realistic_v16-v2/GEN-SIM-DIGI-RAW'
    # crabCommand('submit', config = config)

    # # -- ttbar
    # config.General.requestName = 'TT_Powheg_FlatPU30to80'
    # config.Data.inputDataset = '/TT_TuneCP5_14TeV-powheg-pythia8/Run3Winter21DRMiniAOD-FlatPU30to80FEVT_112X_mcRun3_2021_realistic_v16-v2/MINIAODSIM'
    # config.Data.secondaryInputDataset = '/TT_TuneCP5_14TeV-powheg-pythia8/Run3Winter21DRMiniAOD-FlatPU30to80FEVT_112X_mcRun3_2021_realistic_v16-v2/GEN-SIM-DIGI-RAW'
    # crabCommand('submit', config = config)

    # # -- QCD
    # config.General.requestName = 'QCDMuEnriched_Pt20to30_Pythia8_FlatPU30to80'
    # config.Data.inputDataset = '/QCD_Pt-20To30_MuEnrichedPt5_TuneCP5_14TeV-pythia8/Run3Winter21DRMiniAOD-FlatPU30to80FEVT_112X_mcRun3_2021_realistic_v16-v2/MINIAODSIM'
    # config.Data.secondaryInputDataset = '/QCD_Pt-20To30_MuEnrichedPt5_TuneCP5_14TeV-pythia8/Run3Winter21DRMiniAOD-FlatPU30to80FEVT_112X_mcRun3_2021_realistic_v16-v2/GEN-SIM-DIGI-RAW'
    # crabCommand('submit', config = config)

    # config.General.requestName = 'QCDMuEnriched_Pt30to50_Pythia8_FlatPU30to80'
    # config.Data.inputDataset = '/QCD_Pt-30To50_MuEnrichedPt5_TuneCP5_14TeV-pythia8/Run3Winter21DRMiniAOD-FlatPU30to80FEVT_112X_mcRun3_2021_realistic_v16-v2/MINIAODSIM'
    # config.Data.secondaryInputDataset = '/QCD_Pt-30To50_MuEnrichedPt5_TuneCP5_14TeV-pythia8/Run3Winter21DRMiniAOD-FlatPU30to80FEVT_112X_mcRun3_2021_realistic_v16-v2/GEN-SIM-DIGI-RAW'
    # crabCommand('submit', config = config)

    # config.General.requestName = 'QCDMuEnriched_Pt50to80_Pythia8_FlatPU30to80'
    # config.Data.inputDataset = '/QCD_Pt-50To80_MuEnrichedPt5_TuneCP5_14TeV-pythia8/Run3Winter21DRMiniAOD-FlatPU30to80FEVT_112X_mcRun3_2021_realistic_v16-v2/MINIAODSIM'
    # config.Data.secondaryInputDataset = '/QCD_Pt-50To80_MuEnrichedPt5_TuneCP5_14TeV-pythia8/Run3Winter21DRMiniAOD-FlatPU30to80FEVT_112X_mcRun3_2021_realistic_v16-v2/GEN-SIM-DIGI-RAW'
    # crabCommand('submit', config = config)

    # config.General.requestName = 'QCDMuEnriched_Pt80to120_Pythia8_FlatPU30to80'
    # config.Data.inputDataset = '/QCD_Pt-80To120_MuEnrichedPt5_TuneCP5_14TeV-pythia8/Run3Winter21DRMiniAOD-FlatPU30to80FEVT_112X_mcRun3_2021_realistic_v16-v2/MINIAODSIM'
    # config.Data.secondaryInputDataset = '/QCD_Pt-80To120_MuEnrichedPt5_TuneCP5_14TeV-pythia8/Run3Winter21DRMiniAOD-FlatPU30to80FEVT_112X_mcRun3_2021_realistic_v16-v2/GEN-SIM-DIGI-RAW'
    # crabCommand('submit', config = config)

    # config.General.requestName = 'QCDMuEnriched_Pt120to170_Pythia8_FlatPU30to80'
    # config.Data.inputDataset = '/QCD_Pt-120To170_MuEnrichedPt5_TuneCP5_14TeV-pythia8/Run3Winter21DRMiniAOD-FlatPU30to80FEVT_112X_mcRun3_2021_realistic_v16-v3/MINIAODSIM'
    # config.Data.secondaryInputDataset = '/QCD_Pt-120To170_MuEnrichedPt5_TuneCP5_14TeV-pythia8/Run3Winter21DRMiniAOD-FlatPU30to80FEVT_112X_mcRun3_2021_realistic_v16-v3/GEN-SIM-DIGI-RAW'
    # crabCommand('submit', config = config)

