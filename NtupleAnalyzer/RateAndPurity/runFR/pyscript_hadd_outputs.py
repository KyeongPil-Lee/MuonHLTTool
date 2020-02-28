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
    RunHadd( "TTSemiLep80X", "v01", "hltL2fL1sMu22L1f0L2Filtered10Q" )
    RunHadd( "TTSemiLep102X", "v01", "hltL2fL1sSingleMu22L1f0L2Filtered10Q" )