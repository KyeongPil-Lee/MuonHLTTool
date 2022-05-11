from Include.CondorJobSubmitter import *

submitter = MultiCondorJobSubmitter()

submitter.ROOTCodeName = "ProduceHist.cxx"

submitter.sampleInfoYAML = "sampleInfo_v5.yml" # -- $MUONHLT_ANALYZER_PATH/Include
submitter.dic_sample_nJob = {
"ZMuMu_M50to120": 10,
"QCDMuEnriched_Pt20to30": 3,
"QCDMuEnriched_Pt30to50": 3,
"QCDMuEnriched_Pt50to80": 3,
"QCDMuEnriched_Pt80to120": 3,
"QCDMuEnriched_Pt120to170": 3,

"ZMuMu_M50to120_newEHCAL": 10,
"QCDMuEnriched_Pt20to30_newEHCAL": 3,
"QCDMuEnriched_Pt30to50_newEHCAL": 3,
"QCDMuEnriched_Pt50to80_newEHCAL": 3,
"QCDMuEnriched_Pt80to120_newEHCAL": 3,
"QCDMuEnriched_Pt120to170_newEHCAL": 3,

}

submitter.doMergeBinnedSample = True

submitter.Submit()