import os

def RunHadd(dataset, version, trigger, etaMin = "0.0", etaMax = "5.0"):

    mergedFileName = "./Output-MuonTriggerHist-{dataset_}-{version_}-{trigger_}.root".format(
        dataset_=dataset, version_=version, trigger_=trigger, etaMin_=etaMin, etaMax_=etaMax)


    outputFileNameForm_condor = "Output-MuonTriggerHist-{dataset_}-{version_}-{trigger_}--Job*.root".format(
        dataset_=dataset, version_=version, trigger_=trigger)

    cmd_hadd = """
    hadd {mergedFileName_} \\
    {outputFileNameForm_condor_}""".format(mergedFileName_=mergedFileName, outputFileNameForm_condor_=outputFileNameForm_condor)

    print cmd_hadd
    os.system(cmd_hadd)
    print "[Done]\n"


if __name__ == '__main__':
    RunHadd( "HLTPhysicsRun2016Hv2", "v207", "hltL1fL1sMu22L1Filtered0" )
    # RunHadd( "HLTPhysicsRun2016Hv2", "v207", "hltL2fL1sMu22L1f0L2Filtered10Q" )
    # RunHadd( "HLTPhysicsRun2016Hv2", "v207", "hltL3crIsoL1sMu22L1f0L2f10QL3f24QL3trkIsoFiltered0p09" ) # -- IsoMu24
    # RunHadd( "HLTPhysicsRun2016Hv2", "v207", "hltL3fL1sMu22L1f0Tkf24QL3trkIsoFiltered0p09" ) # -- IsoTkMu24
    # RunHadd( "HLTPhysicsRun2016Hv2", "v207", "hltL3fL1sMu22L1f0L2f10QL3Filtered24Q" ) # -- Mu24
    # RunHadd( "HLTPhysicsRun2016Hv2", "v207", "hltL3fL1sMu22f0TkFiltered24Q" ) # -- TkMu24

    RunHadd( "HLTPhysicsRun2018D", "v207", "hltL1fL1sMu22L1Filtered0" )
    # RunHadd( "HLTPhysicsRun2018D", "v207", "hltL2fL1sSingleMu22L1f0L2Filtered10Q" )
    # RunHadd( "HLTPhysicsRun2018D", "v207", "hltL3crIsoL1sSingleMu22L1f0L2f10QL3f24QL3trkIsoFiltered0p07" ) # -- IsoMu24, 2018
    # RunHadd( "HLTPhysicsRun2018D", "v207", "hltL3fL1sSingleMu22L1f0L2f10QL3Filtered24Q" ) # -- Mu24, 2018
