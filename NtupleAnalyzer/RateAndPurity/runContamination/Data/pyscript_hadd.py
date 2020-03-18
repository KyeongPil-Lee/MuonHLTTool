import os

def RunHadd(dataset, version, trigger, etaMin = "0.0", etaMax = "5.0"):

    mergedFileName = "./Output-MuonTriggerPurity-{dataset_}-{version_}-{trigger_}-{etaMin_}-{etaMax_}.root".format(
        dataset_=dataset, version_=version, trigger_=trigger, etaMin_=etaMin, etaMax_=etaMax)


    outputFileNameForm_condor = "Output-MuonTriggerPurity-{dataset_}-{version_}-{trigger_}-0.0-5.0--Job*.root".format(
        dataset_=dataset, version_=version, trigger_=trigger)

    cmd_hadd = """
    hadd {mergedFileName_} \\
    {outputFileNameForm_condor_}""".format(mergedFileName_=mergedFileName, outputFileNameForm_condor_=outputFileNameForm_condor)

    print cmd_hadd
    os.system(cmd_hadd)
    print "[Done]\n"


if __name__ == '__main__':
    # RunHadd( "Run2016H", "v201", "hltL1fL1sMu22L1Filtered0" )
    # RunHadd( "Run2016H", "v201", "hltL2fL1sMu22L1f0L2Filtered10Q" )
    # RunHadd( "Run2016H", "v201", "HLT_IsoMu24_v" )
    # RunHadd( "Run2016H", "v201", "HLT_IsoTkMu24_v" )
    RunHadd( "Run2016H", "v201", "HLT_Mu24_v" )

    # RunHadd( "Run2018D", "v201", "hltL1fL1sMu22L1Filtered0" )
    # RunHadd( "Run2018D", "v201", "hltL2fL1sSingleMu22L1f0L2Filtered10Q" )
    # RunHadd( "Run2018D", "v201", "HLT_IsoMu24_v" )
    RunHadd( "Run2018D", "v201", "HLT_Mu24_v" )
