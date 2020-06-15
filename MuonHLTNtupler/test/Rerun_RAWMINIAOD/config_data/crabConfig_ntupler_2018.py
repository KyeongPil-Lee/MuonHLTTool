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

config.Data.lumiMask = './Cert_314472-325175_13TeV_17SeptEarlyReReco2018ABC_PromptEraD_Collisions18_JSON_Last5invfb.txt'

version = '_v20200615_'

# 'MultiCRAB' part
if __name__ == '__main__':
    
    from CRABAPI.RawCommand import crabCommand

    # -- for efficiency: useParent = True
    config.General.requestName = 'MuonHLTNtuple'+version+'HLTPhysics_Run2018D_Rerun'
    config.Data.inputDataset = '/HLTPhysics/Run2018D-PromptReco-v2/MINIAOD'
    config.Data.secondaryInputDataset = '/HLTPhysics/Run2018D-v1/RAW'
    config.JobType.psetName = 'HLTCfg_2018_Data.py'
    crabCommand('submit', config = config)

