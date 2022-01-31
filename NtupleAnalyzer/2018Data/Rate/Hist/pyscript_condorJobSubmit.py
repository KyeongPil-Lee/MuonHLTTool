from Include.CondorJobSubmitter import *

submitter = MultiCondorJobSubmitter()

submitter.ROOTCodeName = "Produce_DataRateHist.cxx"

submitter.sampleInfoYAML = "sampleInfo_v3.yml" # -- $MUONHLT_ANALYZER_PATH/Include
submitter.dic_sample_nJob = {
    "EphemeralHLTPhysicsAll_Run2018D_Run323775_withoutPT": 20,
    "EphemeralHLTPhysicsAll_Run2018D_Run323775": 20,
}

submitter.doMergeBinnedSample = True

submitter.Submit()