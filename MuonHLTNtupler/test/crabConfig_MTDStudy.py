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
# config.Data.inputDBS = 'phys03' # -- USER data
config.Data.publication = False

# config.Data.splitting = 'Automatic' # -- it is not preferred: test job is frequently failed due to memory
# config.Data.splitting = 'FileBased'
# config.Data.unitsPerJob = 1
config.Data.splitting = 'EventAwareLumiBased'
config.Data.unitsPerJob = 1000

config.Site.storageSite = 'T2_BE_IIHE'

# config.Data.lumiMask = './Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON_Last5invfb.txt'

# config.JobType.allowUndistributedCMSSW = True

config.JobType.psetName = 'Run_ntupler_MTDStudy.py' # -- should be filled
version = 'v2'

config.General.workArea = 'CRABDir_%s' % version
config.Data.outLFNDirBase = '/store/user/kplee/MuonHLTTree_phase2MTD_131X_%s' % version


# 'MultiCRAB' part
if __name__ == '__main__':
    
    from CRABAPI.RawCommand import crabCommand

    config.General.requestName = 'RelValZMM'
    config.Data.inputDataset = '/RelValZMM_14/CMSSW_13_1_0-PU_131X_mcRun4_realistic_v5_2026D95PU200-v1/MINIAODSIM'
    config.Data.secondaryInputDataset = '/RelValZMM_14/CMSSW_13_1_0-PU_131X_mcRun4_realistic_v5_2026D95PU200-v1/GEN-SIM-RECO'
    crabCommand('submit', config = config)

    config.General.requestName = 'RelValTTbar'
    config.Data.inputDataset = '/RelValTTbar_14TeV/CMSSW_13_1_0-PU_131X_mcRun4_realistic_v5_2026D95PU200-v1/MINIAODSIM'
    config.Data.secondaryInputDataset = '/RelValTTbar_14TeV/CMSSW_13_1_0-PU_131X_mcRun4_realistic_v5_2026D95PU200-v1/GEN-SIM-RECO'
    crabCommand('submit', config = config)

    config.General.requestName = 'RelValQCD_Pt20toInfMuEnrichPt15'
    config.Data.inputDataset = '/RelValQCD_Pt20toInfMuEnrichPt15_14/CMSSW_13_1_0-PU_131X_mcRun4_realistic_v5_2026D95PU200-v1/MINIAODSIM'
    config.Data.secondaryInputDataset = '/RelValQCD_Pt20toInfMuEnrichPt15_14/CMSSW_13_1_0-PU_131X_mcRun4_realistic_v5_2026D95PU200-v1/GEN-SIM-RECO'
    crabCommand('submit', config = config)

