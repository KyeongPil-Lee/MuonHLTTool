from Include.CondorJobSubmitter import *

submitter = MultiCondorJobSubmitter()

submitter.ROOTCodeName = "ProduceHist.cxx"

submitter.sampleInfoYAML = "sampleInfo_v4.yml" # -- $MUONHLT_ANALYZER_PATH/Include
submitter.dic_sample_nJob = {
    "ZMuMu_M50to120_nVtx_nRegion_1_10": 10,
    # "QCDMuEnriched_Pt20to30_nVtx_nRegion_1_10": 3,
    # "QCDMuEnriched_Pt30to50_nVtx_nRegion_1_10": 3,
    # "QCDMuEnriched_Pt50to80_nVtx_nRegion_1_10": 3,
    # "QCDMuEnriched_Pt80to120_nVtx_nRegion_1_10": 3,
    # "QCDMuEnriched_Pt120to170_nVtx_nRegion_1_10": 3,

    "ZMuMu_M50to120_nVtx_nRegion_2_20": 10,
    # "QCDMuEnriched_Pt20to30_nVtx_nRegion_2_20": 3,
    # "QCDMuEnriched_Pt30to50_nVtx_nRegion_2_20": 3,
    # "QCDMuEnriched_Pt50to80_nVtx_nRegion_2_20": 3,
    # "QCDMuEnriched_Pt80to120_nVtx_nRegion_2_20": 3,
    # "QCDMuEnriched_Pt120to170_nVtx_nRegion_2_20": 3,

    "ZMuMu_M50to120_nVtx_nRegion_3_30": 10,
    # "QCDMuEnriched_Pt20to30_nVtx_nRegion_3_30": 3,
    # "QCDMuEnriched_Pt30to50_nVtx_nRegion_3_30": 3,
    # "QCDMuEnriched_Pt50to80_nVtx_nRegion_3_30": 3,
    # "QCDMuEnriched_Pt80to120_nVtx_nRegion_3_30": 3,
    # "QCDMuEnriched_Pt120to170_nVtx_nRegion_3_30": 3,

    "ZMuMu_M50to120_nVtx_nRegion_4_40": 10,
    # "QCDMuEnriched_Pt20to30_nVtx_nRegion_4_40": 3,
    # "QCDMuEnriched_Pt30to50_nVtx_nRegion_4_40": 3,
    # "QCDMuEnriched_Pt50to80_nVtx_nRegion_4_40": 3,
    # "QCDMuEnriched_Pt80to120_nVtx_nRegion_4_40": 3,
    # "QCDMuEnriched_Pt120to170_nVtx_nRegion_4_40": 3,

    "ZMuMu_M50to120_nVtx_nRegion_6_60": 10,
    # "QCDMuEnriched_Pt20to30_nVtx_nRegion_6_60": 3,
    # "QCDMuEnriched_Pt30to50_nVtx_nRegion_6_60": 3,
    # "QCDMuEnriched_Pt50to80_nVtx_nRegion_6_60": 3,
    # "QCDMuEnriched_Pt80to120_nVtx_nRegion_6_60": 3,
    # "QCDMuEnriched_Pt120to170_nVtx_nRegion_6_60": 3,

    "ZMuMu_M50to120_nVtx_nRegion_8_80": 10,
    # "QCDMuEnriched_Pt20to30_nVtx_nRegion_8_80": 3,
    # "QCDMuEnriched_Pt30to50_nVtx_nRegion_8_80": 3,
    # "QCDMuEnriched_Pt50to80_nVtx_nRegion_8_80": 3,
    # "QCDMuEnriched_Pt80to120_nVtx_nRegion_8_80": 3,
    # "QCDMuEnriched_Pt120to170_nVtx_nRegion_8_80": 3,

    "ZMuMu_M50to120_nVtx_nRegion_10_100": 10,
    # "QCDMuEnriched_Pt20to30_nVtx_nRegion_10_100": 3,
    # "QCDMuEnriched_Pt30to50_nVtx_nRegion_10_100": 3,
    # "QCDMuEnriched_Pt50to80_nVtx_nRegion_10_100": 3,
    # "QCDMuEnriched_Pt80to120_nVtx_nRegion_10_100": 3,
    # "QCDMuEnriched_Pt120to170_nVtx_nRegion_10_100": 3,

    "ZMuMu_M50to120_nVtx_nRegion_20_200": 10,
    # "QCDMuEnriched_Pt20to30_nVtx_nRegion_20_200": 3,
    # "QCDMuEnriched_Pt30to50_nVtx_nRegion_20_200": 3,
    # "QCDMuEnriched_Pt50to80_nVtx_nRegion_20_200": 3,
    # "QCDMuEnriched_Pt80to120_nVtx_nRegion_20_200": 3,
    # "QCDMuEnriched_Pt120to170_nVtx_nRegion_20_200": 3,

    # -- new param.
    "ZMuMu_M50to120_newParam_nVtx_nRegion_1_10": 10,
    # "QCDMuEnriched_Pt20to30_newParam_nVtx_nRegion_1_10": 3,
    # "QCDMuEnriched_Pt30to50_newParam_nVtx_nRegion_1_10": 3,
    # "QCDMuEnriched_Pt50to80_newParam_nVtx_nRegion_1_10": 3,
    # "QCDMuEnriched_Pt80to120_newParam_nVtx_nRegion_1_10": 3,
    # "QCDMuEnriched_Pt120to170_newParam_nVtx_nRegion_1_10": 3,

    "ZMuMu_M50to120_newParam_nVtx_nRegion_2_20": 10,
    # "QCDMuEnriched_Pt20to30_newParam_nVtx_nRegion_2_20": 3,
    # "QCDMuEnriched_Pt30to50_newParam_nVtx_nRegion_2_20": 3,
    # "QCDMuEnriched_Pt50to80_newParam_nVtx_nRegion_2_20": 3,
    # "QCDMuEnriched_Pt80to120_newParam_nVtx_nRegion_2_20": 3,
    # "QCDMuEnriched_Pt120to170_newParam_nVtx_nRegion_2_20": 3,

    "ZMuMu_M50to120_newParam_nVtx_nRegion_3_30": 10,
    # "QCDMuEnriched_Pt20to30_newParam_nVtx_nRegion_3_30": 3,
    # "QCDMuEnriched_Pt30to50_newParam_nVtx_nRegion_3_30": 3,
    # "QCDMuEnriched_Pt50to80_newParam_nVtx_nRegion_3_30": 3,
    # "QCDMuEnriched_Pt80to120_newParam_nVtx_nRegion_3_30": 3,
    # "QCDMuEnriched_Pt120to170_newParam_nVtx_nRegion_3_30": 3,

    "ZMuMu_M50to120_newParam_nVtx_nRegion_4_40": 10,
    # "QCDMuEnriched_Pt20to30_newParam_nVtx_nRegion_4_40": 3,
    # "QCDMuEnriched_Pt30to50_newParam_nVtx_nRegion_4_40": 3,
    # "QCDMuEnriched_Pt50to80_newParam_nVtx_nRegion_4_40": 3,
    # "QCDMuEnriched_Pt80to120_newParam_nVtx_nRegion_4_40": 3,
    # "QCDMuEnriched_Pt120to170_newParam_nVtx_nRegion_4_40": 3,

    "ZMuMu_M50to120_newParam_nVtx_nRegion_5_50": 10,
    # "QCDMuEnriched_Pt20to30_newParam_nVtx_nRegion_5_50": 3,
    # "QCDMuEnriched_Pt30to50_newParam_nVtx_nRegion_5_50": 3,
    # "QCDMuEnriched_Pt50to80_newParam_nVtx_nRegion_5_50": 3,
    # "QCDMuEnriched_Pt80to120_newParam_nVtx_nRegion_5_50": 3,
    # "QCDMuEnriched_Pt120to170_newParam_nVtx_nRegion_5_50": 3,

    "ZMuMu_M50to120_woPatatrack": 10,
    # "QCDMuEnriched_Pt20to30_woPatatrack": 3,
    # "QCDMuEnriched_Pt30to50_woPatatrack": 3,
    # "QCDMuEnriched_Pt50to80_woPatatrack": 3,
    # "QCDMuEnriched_Pt80to120_woPatatrack": 3,
    # "QCDMuEnriched_Pt120to170_woPatatrack": 3,
}

submitter.doMergeBinnedSample = True

submitter.Submit()