from Include.CondorJobSubmitter import *

submitter = CondorJobSubmitter()

submitter.ROOTCodeName = "MakeHist_L3overNoID.cxx"
submitter.list_ntupleDirPath = ["/gv0/Users/kplee/MuonHLT/Run3Winter20/noIDVar/ZToMuMu_M50to120_Powheg"]
submitter.nJob = 30

submitter.Submit()

