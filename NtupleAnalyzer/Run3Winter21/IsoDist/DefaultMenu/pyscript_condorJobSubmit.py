from Include.CondorJobSubmitter import *

submitter = CondorJobSubmitter()

submitter.ROOTCodeName = "MakeHist_Isolation_IsoMu24.cxx"
submitter.list_ntupleDirPath = [
"/pnfs/iihe/cms/store/user/kplee/MuonHLTTree_v01/ZToMuMu_M-50To120_TuneCP5_14TeV-powheg-pythia8/crab_ZToMuMu_M50to120_Powheg_FlatPU30to80/210922_100725/0000"
]
submitter.nJob = 5

submitter.Submit()

