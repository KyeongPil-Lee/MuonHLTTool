from CRABClient.UserUtilities import config
config = config()

config.General.requestName = ''
config.General.workArea = 'CRABDir'

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = ''
config.JobType.numCores = 4
# config.JobType.maxMemoryMB = 2500
# config.JobType.maxJobRuntimeMin = 2000

config.Data.inputDataset = ''
# config.Data.useParent = True

config.Data.inputDBS = 'global'
config.Data.publication = False

# config.Data.splitting = 'Automatic' # -- it is not preferred: test job is frequently failed due to memory
config.Data.splitting = 'LumiBased'
config.Data.unitsPerJob = 5

config.Data.outLFNDirBase = '/store/user/kplee/'
config.Site.storageSite = 'T2_KR_KNU'

config.Data.lumiMask = './Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON_Last5invfb.txt'

version = '_v20200615_'

# 'MultiCRAB' part
if __name__ == '__main__':
    
    from CRABAPI.RawCommand import crabCommand

    config.General.requestName = 'MuonHLTNtuple'+version+'HLTPhysics_Run2016Hv2_Rerun'
    config.Data.inputDataset = '/HLTPhysics/Run2016H-PromptReco-v2/MINIAOD'
    config.Data.secondaryInputDataset = '/HLTPhysics/Run2016H-v1/RAW'
    config.JobType.psetName = 'HLTCfg_2016_Data.py'
    crabCommand('submit', config = config)

    config.General.requestName = 'MuonHLTNtuple'+version+'HLTPhysics_Run2016Hv3_Rerun'
    config.Data.inputDataset = '/HLTPhysics/Run2016H-PromptReco-v3/MINIAOD'
    config.Data.secondaryInputDataset = '/HLTPhysics/Run2016H-v1/RAW'
    config.JobType.psetName = 'HLTCfg_2016_Data.py'
    crabCommand('submit', config = config)

