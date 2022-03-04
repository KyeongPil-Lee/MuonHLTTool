from Include.CondorJobSubmitter import *

submitter = MultiCondorJobSubmitter()

submitter.ROOTCodeName = "MakeHist_SumWeight.cxx"

submitter.sampleInfoYAML = "sampleInfo_v4.yml" # -- $MUONHLT_ANALYZER_PATH/Include
submitter.dic_sample_nJob = {
    "ZMuMu_M50to120_woPatatrack": 10,
    "QCDMuEnriched_Pt20to30_woPatatrack": 3,
    "QCDMuEnriched_Pt30to50_woPatatrack": 3,
    "QCDMuEnriched_Pt50to80_woPatatrack": 3,
    "QCDMuEnriched_Pt80to120_woPatatrack": 3,
    "QCDMuEnriched_Pt120to170_woPatatrack": 3,
}

submitter.Submit()