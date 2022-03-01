from Include.CondorJobSubmitter import *

submitter = MultiCondorJobSubmitter()

submitter.ROOTCodeName = "ProduceHist.cxx"

submitter.sampleInfoYAML = "sampleInfo_v4.yml" # -- $MUONHLT_ANALYZER_PATH/Include
submitter.dic_sample_nJob = {
    "ZMuMu_M50to120_nVtx_nRegion_1_10": 10,
}

submitter.doMergeBinnedSample = True

submitter.Submit()