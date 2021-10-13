from Include.CondorJobSubmitter import *

submitter = MultiCondorJobSubmitter()

submitter.ROOTCodeName = "MakeHist_ISOoverL3.cxx"

submitter.sampleInfoYAML = "sampleInfo_v1.yml" # -- $MUONHLT_ANALYZER_PATH/Include
submitter.dic_sample_nJob = {
    "ZMuMu_M50to120_Default": 10,
}

submitter.Submit()