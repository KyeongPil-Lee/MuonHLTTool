from CRABClient.UserUtilities import config
config = config()

config.General.requestName = ''
config.General.workArea = 'CRABDir'

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'HLTConfig_withoutPT.py' # -- should be filled
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
# config.Data.splitting = 'EventAwareLumiBased'
# config.Data.unitsPerJob = 3000 # -- ~3000 events per job --> 1000 jobs for ZMuMu_M50to120 (3M events)
config.Data.splitting = 'LumiBased'
config.Data.unitsPerJob = 5

config.Data.runRange = '323775'

config.Site.storageSite = 'T2_BE_IIHE'

# config.Data.lumiMask = './Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON_Last5invfb.txt'
# config.JobType.allowUndistributedCMSSW = True

version = 'v10'
config.Data.outLFNDirBase = '/store/user/kplee/MuonHLTTree_%s' % version

# 'MultiCRAB' part
if __name__ == '__main__':
    
    from CRABAPI.RawCommand import crabCommand

    # config.General.requestName = 'EphemeralHLTPhysics1_Run2018D_Run323775_withoutPT'
    # config.Data.inputDataset = '/EphemeralHLTPhysics1/Run2018D-v1/RAW'
    # crabCommand('submit', config = config)

    config.General.requestName = 'EphemeralHLTPhysics2_Run2018D_Run323775_withoutPT'
    config.Data.inputDataset = '/EphemeralHLTPhysics2/Run2018D-v1/RAW'
    crabCommand('submit', config = config)

    config.General.requestName = 'EphemeralHLTPhysics3_Run2018D_Run323775_withoutPT'
    config.Data.inputDataset = '/EphemeralHLTPhysics3/Run2018D-v1/RAW'
    crabCommand('submit', config = config)

    config.General.requestName = 'EphemeralHLTPhysics4_Run2018D_Run323775_withoutPT'
    config.Data.inputDataset = '/EphemeralHLTPhysics4/Run2018D-v1/RAW'
    crabCommand('submit', config = config)

    config.General.requestName = 'EphemeralHLTPhysics5_Run2018D_Run323775_withoutPT'
    config.Data.inputDataset = '/EphemeralHLTPhysics5/Run2018D-v1/RAW'
    crabCommand('submit', config = config)

    config.General.requestName = 'EphemeralHLTPhysics6_Run2018D_Run323775_withoutPT'
    config.Data.inputDataset = '/EphemeralHLTPhysics6/Run2018D-v1/RAW'
    crabCommand('submit', config = config)

    config.General.requestName = 'EphemeralHLTPhysics7_Run2018D_Run323775_withoutPT'
    config.Data.inputDataset = '/EphemeralHLTPhysics7/Run2018D-v1/RAW'
    crabCommand('submit', config = config)

    config.General.requestName = 'EphemeralHLTPhysics8_Run2018D_Run323775_withoutPT'
    config.Data.inputDataset = '/EphemeralHLTPhysics8/Run2018D-v1/RAW'
    crabCommand('submit', config = config)

