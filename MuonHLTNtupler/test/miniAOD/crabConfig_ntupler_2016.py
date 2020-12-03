from CRABClient.UserUtilities import config
config = config()

config.General.requestName = ''
config.General.workArea = 'CRABDir'

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = ''
# config.JobType.numCores = 4
# config.JobType.maxMemoryMB = 3000
# config.JobType.maxJobRuntimeMin = 2000

config.Data.inputDataset = ''
# config.Data.useParent = True

config.Data.inputDBS = 'global'
config.Data.publication = False

# config.Data.splitting = 'Automatic' # -- it is not preferred: test job is frequently failed due to memory
config.Data.splitting = 'LumiBased'
config.Data.unitsPerJob = 20

config.Data.outLFNDirBase = '/store/user/kplee/'
config.Site.storageSite = 'T2_KR_KNU'

config.Data.lumiMask = './Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON_Last5invfb.txt'

# config.Data.ignoreLocality = True
# config.Site.whitelist = ['T2_KR_*', 'T3_KR_*', 'T2_US_*']

config.JobType.allowUndistributedCMSSW = True

version = '_v20201203_'

# 'MultiCRAB' part
if __name__ == '__main__':
    
    from CRABAPI.RawCommand import crabCommand

    # config.General.requestName = 'MuonHLTNtuple'+version+'DoubleMuon_07Aug17_Run2016Hv2'
    # config.Data.inputDataset = '/DoubleMuon/Run2016H-07Aug17-v1/MINIAOD'
    # config.JobType.psetName = 'Run_ntupler_2016.py'
    # crabCommand('submit', config = config)

    config.General.requestName = 'MuonHLTNtuple'+version+'HLTPhysics_Run2016Hv2'
    config.Data.inputDataset = '/HLTPhysics/Run2016H-PromptReco-v2/MINIAOD'
    config.JobType.psetName = 'Run_ntupler_2016.py'
    crabCommand('submit', config = config)

    config.General.requestName = 'MuonHLTNtuple'+version+'HLTPhysics_Run2016Hv3'
    config.Data.inputDataset = '/HLTPhysics/Run2016H-PromptReco-v3/MINIAOD'
    config.JobType.psetName = 'Run_ntupler_2016.py'
    crabCommand('submit', config = config)

