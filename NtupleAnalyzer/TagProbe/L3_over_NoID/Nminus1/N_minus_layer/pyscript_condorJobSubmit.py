from Include.CondorJobSubmitter import *

submitter = CondorJobSubmitter()

submitter.ROOTCodeName = "MakeHist_L3overNoID.cxx"
submitter.list_ntupleDirPath = ["/gv0/Users/kplee/MuonHLT/Run3_IDIso/Run3Winter20/ZToMuMu_M50to120_Powheg"]
submitter.nJob = 30

submitter.Submit()

