import FWCore.ParameterSet.Config as cms

# -- ref: https://its.cern.ch/jira/browse/CMSHLT-2250
def customizerForMuonHLT_HCALThreshold(process):
    if hasattr(process, "hltParticleFlowRecHitHBHE"):
        process.hltParticleFlowRecHitHBHE.producers[0].qualityTests[0].name = cms.string( "PFRecHitQTestHCALThresholdVsDepth" )

    return process