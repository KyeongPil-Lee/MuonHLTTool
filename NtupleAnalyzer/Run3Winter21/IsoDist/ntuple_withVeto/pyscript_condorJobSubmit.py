from Include.CondorJobSubmitter import *

submitter = MultiCondorJobSubmitter()

submitter.ROOTCodeName = "MakeHist_Validation_CustomIsoFilter.cxx"

submitter.sampleInfoYAML = "sampleInfo_v1.yml" # -- $MUONHLT_ANALYZER_PATH/Include
submitter.dic_sample_nJob = {
    "ZMuMu_M50to120": 5,
    "ZMuMu_M120to200": 2,
    "ZMuMu_M200to400": 2,
}

submitter.Submit()