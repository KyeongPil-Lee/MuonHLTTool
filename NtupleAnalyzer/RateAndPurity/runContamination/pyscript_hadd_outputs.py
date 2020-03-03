import os

# -- settings -- #
scratchPath = "/d0/scratch/kplee"

def RunHadd(dataset, version, trigger, etaMin = "0.0", etaMax = "5.0"):

    outputName = "./Output-MuonTriggerPurity-{dataset_}-{version_}-{trigger_}-{etaMin_}-{etaMax_}.root".format(
        dataset_=dataset, version_=version, trigger_=trigger, etaMin_=etaMin, etaMax_=etaMax)

    batchDirPath = "{scratchPath_}/BatchDir_{version_}/{dataset_}/{trigger_}/Job*/Output*.root".format(
        scratchPath_=scratchPath, version_=version, dataset_=dataset, trigger_=trigger)

    cmd_hadd = """
    hadd {outputName_} \\
    {batchDirPath_}""".format(outputName_=outputName, batchDirPath_=batchDirPath)

    print cmd_hadd
    os.system(cmd_hadd)
    print "[Done]\n"


if __name__ == '__main__':
    RunHadd( "TTSemiLep102X", "v100", "hltL1fL1sMu22L1Filtered0" )
    RunHadd( "TTSemiLep102X", "v100", "hltL2fL1sSingleMu22L1f0L2Filtered10Q" )
    RunHadd( "TTSemiLep102X", "v100", "HLT_IsoMu24_v" )
    RunHadd( "TTSemiLep102X", "v100", "HLT_Mu50_v" )
    RunHadd( "TTSemiLep102X", "v100", "HLT_Mu24_v" )

    RunHadd( "TTSemiLep102XMUOVal", "v100", "hltL1fL1sMu22L1Filtered0" )
    RunHadd( "TTSemiLep102XMUOVal", "v100", "hltL2fL1sSingleMu22L1f0L2Filtered10Q" )
    RunHadd( "TTSemiLep102XMUOVal", "v100", "HLT_IsoMu24_v" )
    RunHadd( "TTSemiLep102XMUOVal", "v100", "HLT_Mu50_v" )
    RunHadd( "TTSemiLep102XMUOVal", "v100", "HLT_Mu24_v" )
