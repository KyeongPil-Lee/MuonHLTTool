from CRABClient.UserUtilities import config
config = config()

config.General.requestName = ''
config.General.workArea = 'CRABDir'

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = ''
# config.JobType.numCores = 4
# config.JobType.maxMemoryMB = 2500
# config.JobType.maxJobRuntimeMin = 2000

config.Data.inputDataset = ''
# config.Data.useParent = True

config.Data.inputDBS = 'global'
config.Data.publication = False

# config.Data.splitting = 'Automatic' # -- it is not preferred: test job is frequently failed due to memory
config.Data.splitting = 'LumiBased'
config.Data.unitsPerJob = 20

config.Data.outLFNDirBase = '/store/user/kplee/'
config.Site.storageSite = 'T3_KR_KNU'

config.Data.lumiMask = './Cert_314472-325175_13TeV_17SeptEarlyReReco2018ABC_PromptEraD_Collisions18_JSON_Last5invfb.txt'

config.JobType.allowUndistributedCMSSW = True

version = '_v20201203_'

# 'MultiCRAB' part
if __name__ == '__main__':
    
    from CRABAPI.RawCommand import crabCommand

    # -- for efficiency: useParent = True
    # config.General.requestName = 'MuonHLTNtuple'+version+'DoubleMuon_Run2018D'
    # config.Data.inputDataset = '/DoubleMuon/Run2018D-PromptReco-v2/MINIAOD'
    # config.JobType.psetName = 'Run_ntupler_2018.py'
    # crabCommand('submit', config = config)

    config.General.requestName = 'MuonHLTNtuple'+version+'HLTPhysics_Run2018D_v2'
    config.Data.inputDataset = '/HLTPhysics/Run2018D-PromptReco-v2/MINIAOD'
    config.JobType.psetName = 'Run_ntupler_2018.py'
    crabCommand('submit', config = config)
