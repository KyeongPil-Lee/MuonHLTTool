from Include.CondorJobSubmitter import *

submitter = MultiCondorJobSubmitter()

submitter.ROOTCodeName = "ProduceTnPHist_TRKoverL3.cxx"

submitter.sampleInfoYAML = "sampleInfo_v2_diffRho.yml" # -- $MUONHLT_ANALYZER_PATH/Include
submitter.dic_sample_nJob = {
    "ZMuMu_M50to120": 10,
}

submitter.Submit()