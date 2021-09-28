from Include.CondorJobSubmitter import *

submitter = CondorJobSubmitter()

submitter.ROOTCodeName = "MakeHist_Validation_CustomIsoFilter.cxx"
submitter.list_ntupleDirPath = [
"/pnfs/iihe/cms/store/user/kplee/MuonHLTTree_v02/ZToMuMu_M-50To120_TuneCP5_14TeV-powheg-pythia8/crab_ZToMuMu_M50to120_Powheg_FlatPU30to80/210922_125120/0000"
]
submitter.nJob = 10

submitter.Submit()

