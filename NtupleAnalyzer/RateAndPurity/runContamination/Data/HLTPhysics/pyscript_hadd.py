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
    # RunHadd( "HLTPhysicsRun2016Hv2", "v202", "hltL1fL1sMu22L1Filtered0" )
    # RunHadd( "HLTPhysicsRun2016Hv2", "v202", "hltL2fL1sMu22L1f0L2Filtered10Q" )
    # RunHadd( "HLTPhysicsRun2016Hv2", "v202", "HLT_IsoMu24_v" )
    # RunHadd( "HLTPhysicsRun2016Hv2", "v202", "HLT_IsoTkMu24_v" )
    # RunHadd( "HLTPhysicsRun2016Hv2", "v202", "HLT_Mu24_v" )
    # RunHadd( "HLTPhysicsRun2016Hv2", "v202", "HLT_TkMu24_v" )

    RunHadd( "HLTPhysicsRun2018D", "v202", "hltL1fL1sMu22L1Filtered0" )
    RunHadd( "HLTPhysicsRun2018D", "v202", "hltL2fL1sSingleMu22L1f0L2Filtered10Q" )
    RunHadd( "HLTPhysicsRun2018D", "v202", "HLT_IsoMu24_v" )
    RunHadd( "HLTPhysicsRun2018D", "v202", "HLT_Mu24_v" )
