from Include.CondorJobSubmitter import *

submitter = MultiCondorJobSubmitter()

submitter.ROOTCodeName = "ProduceHist.cxx"

submitter.sampleInfoYAML = "sampleInfo_v5.yml" # -- $MUONHLT_ANALYZER_PATH/Include
submitter.dic_sample_nJob = {
    # "ZMuMu_M50to120": 10,
    "ZMuMu_M50to120_newEHCAL": 10,
}

submitter.doMergeBinnedSample = True

submitter.Submit()