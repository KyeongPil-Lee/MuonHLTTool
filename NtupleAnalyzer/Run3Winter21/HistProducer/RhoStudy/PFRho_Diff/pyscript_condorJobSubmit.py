from Include.CondorJobSubmitter import *

submitter = MultiCondorJobSubmitter()

submitter.ROOTCodeName = "ProduceHist.cxx"

submitter.sampleInfoYAML = "sampleInfo_v2_diffRho.yml" # -- $MUONHLT_ANALYZER_PATH/Include
submitter.dic_sample_nJob = {
    "ZMuMu_M50to120": 10,
    "ZMuMu_M120to200": 3,
    "ZMuMu_M200to400": 3,

    "QCDMuEnriched_Pt20to30": 3,
    "QCDMuEnriched_Pt30to50": 3,
    "QCDMuEnriched_Pt50to80": 3,
    "QCDMuEnriched_Pt80to120": 3,
    "QCDMuEnriched_Pt120to170": 3,

    "ttbar": 20,
    "WJets": 10,
}

submitter.doMergeBinnedSample = True

submitter.Submit()