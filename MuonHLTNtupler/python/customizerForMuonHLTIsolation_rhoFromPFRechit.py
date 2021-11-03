# -- customizer for muon HLT isolation sequence
# -- Delete the calo tower sequences which only used for rho calculation
# -- Instead, rho is calculated from PF rechits via the producer made by Swagata
# ---- https://github.com/swagata87/cmssw/blob/3f4de291f884946e3ed3866473e843c0b7abc159/RecoJets/JetProducers/plugins/FixedGridRhoProducerFastjetFromRecHit.cc

# -- usage:
# ---- use different rho values for ECAL and HCAL isolation:
# from MuonHLTTool.MuonHLTNtupler.customizerForMuonHLTIsolation_rhoFromPFRechit import *
# process = customizeForMuonHLTIsolation_differentRhoForECALHCAL(process)

# ---- use same rho value for ECAL and HCAL isolation (rho module name: hltFixedGridRhoFastjetForMuons)
# from MuonHLTTool.MuonHLTNtupler.customizerForMuonHLTIsolation_rhoFromPFRechit import *
# process = customizeForMuonHLTIsolation_sameRhoForECALHCAL(process)

import FWCore.ParameterSet.Config as cms

def removeCaloTowerInfo(process):
    if hasattr(process, "hltTowerMakerForECALMF"):
        del process.hltTowerMakerForECALMF

    if hasattr(process, "hltTowerMakerForHCAL"):
        del process.hltTowerMakerForHCAL

    if hasattr(process, "hltFixedGridRhoFastjetECALMFForMuons"):
        del process.hltFixedGridRhoFastjetECALMFForMuons

    if hasattr(process, "hltFixedGridRhoFastjetHCAL"):
        del process.hltFixedGridRhoFastjetHCAL

    return process

def addRhoProducerFromPFRechit(process):
    # -- keep the same module name with the legacy one

    process.hltFixedGridRhoFastjetECALMFForMuons = cms.EDProducer( "FixedGridRhoProducerFastjetFromRecHit",
        hbheRecHitsTag1 = cms.InputTag( "hltHbhereco" ),
        ecalRecHitsTag1 = cms.InputTag( 'hltEcalRecHit','EcalRecHitsEB' ),
        ecalRecHitsTag2 = cms.InputTag( 'hltEcalRecHit','EcalRecHitsEE' ),
        eThresHB = cms.vdouble(0.1, 0.2, 0.3, 0.3),
        eThresHE = cms.vdouble(0.1, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2),
        maxRapidity = cms.double( 2.5 ),
        gridSpacing = cms.double( 0.55 ),
        skipHCAL = cms.bool(True), # -- skip HCAL
        skipECAL = cms.bool(False)
    )

    process.hltFixedGridRhoFastjetHCAL = cms.EDProducer( "FixedGridRhoProducerFastjetFromRecHit",
        hbheRecHitsTag1 = cms.InputTag( "hltHbhereco" ),
        ecalRecHitsTag1 = cms.InputTag( 'hltEcalRecHit','EcalRecHitsEB' ),
        ecalRecHitsTag2 = cms.InputTag( 'hltEcalRecHit','EcalRecHitsEE' ),
        eThresHB = cms.vdouble(0.1, 0.2, 0.3, 0.3),
        eThresHE = cms.vdouble(0.1, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2),
        maxRapidity = cms.double( 2.5 ),
        gridSpacing = cms.double( 0.55 ),
        skipHCAL = cms.bool(False),
        skipECAL = cms.bool(True) # -- skip ECAL
    )

    return process

def customizeForMuonHLTIsolation_differentRhoForECALHCAL(process):
    process = removeCaloTowerInfo(process)

    process = addRhoProducerFromPFRechit(process)

    process.HLTL3muonEcalPFisorecoSequenceNoBoolsForMuons = cms.Sequence( 
      process.HLTDoFullUnpackingEgammaEcalMFSequence + 
      process.HLTDoLocalHcalSequence + 
      process.hltFixedGridRhoFastjetECALMFForMuons + 
      process.hltFixedGridRhoFastjetHCAL + 
      process.HLTPFClusteringEcalMFForMuons + 
      process.hltMuonEcalMFPFClusterIsoForMuons )

    return process


def customizeForMuonHLTIsolation_sameRhoForECALHCAL(process):
    process = removeCaloTowerInfo(process)

    process.hltFixedGridRhoFastjetForMuons = cms.EDProducer( "FixedGridRhoProducerFastjetFromRecHit",
        hbheRecHitsTag1 = cms.InputTag( "hltHbhereco" ),
        ecalRecHitsTag1 = cms.InputTag( 'hltEcalRecHit','EcalRecHitsEB' ),
        ecalRecHitsTag2 = cms.InputTag( 'hltEcalRecHit','EcalRecHitsEE' ),
        eThresHB = cms.vdouble(0.1, 0.2, 0.3, 0.3),
        eThresHE = cms.vdouble(0.1, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2),
        maxRapidity = cms.double( 2.5 ),
        gridSpacing = cms.double( 0.55 ),
        skipHCAL = cms.bool(False), # -- nothing skipped
        skipECAL = cms.bool(False)  # -- nothing skipped
    )

    process.hltMuonEcalMFPFClusterIsoForMuons.rhoProducer  = cms.InputTag( "hltFixedGridRhoFastjetForMuons" )
    process.hltMuonHcalRegPFClusterIsoForMuons.rhoProducer = cms.InputTag( "hltFixedGridRhoFastjetForMuons" )

    process.HLTL3muonEcalPFisorecoSequenceNoBoolsForMuons = cms.Sequence( 
      process.HLTDoFullUnpackingEgammaEcalMFSequence + 
      process.HLTDoLocalHcalSequence + 
      process.hltFixedGridRhoFastjetForMuons + 
      process.HLTPFClusteringEcalMFForMuons + 
      process.hltMuonEcalMFPFClusterIsoForMuons )

    return process