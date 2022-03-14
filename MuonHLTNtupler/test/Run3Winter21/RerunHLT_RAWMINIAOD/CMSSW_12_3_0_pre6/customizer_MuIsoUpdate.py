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
        hbheRecHitsTag = cms.InputTag( "hltHbhereco" ),
        ebRecHitsTag = cms.InputTag( 'hltEcalRecHit','EcalRecHitsEB' ),
        eeRecHitsTag = cms.InputTag( 'hltEcalRecHit','EcalRecHitsEE' ),
        eThresHB = cms.vdouble(0.1, 0.2, 0.3, 0.3),
        eThresHE = cms.vdouble(0.1, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2),
        maxRapidity = cms.double( 2.5 ),
        gridSpacing = cms.double( 0.55 ),
        skipHCAL = cms.bool(True), # -- skip HCAL
        skipECAL = cms.bool(False)
    )

    process.hltFixedGridRhoFastjetHCAL = cms.EDProducer( "FixedGridRhoProducerFastjetFromRecHit",
        hbheRecHitsTag = cms.InputTag( "hltHbhereco" ),
        ebRecHitsTag = cms.InputTag( 'hltEcalRecHit','EcalRecHitsEB' ),
        eeRecHitsTag = cms.InputTag( 'hltEcalRecHit','EcalRecHitsEE' ),
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

    # -- update the WP for the tracker isolation: from 0.07 to 0.09
    # -- name of the modules are also updated properly
    def customizerForMuonHLT_newTrkIsoWP(process):
        if hasattr(process, "hltMuonTkRelIsolationCut0p07Map"):
            process.hltMuonTkRelIsolationCut0p09Map = process.hltMuonTkRelIsolationCut0p07Map.clone()
            process.hltMuonTkRelIsolationCut0p09Map.CutsPSet.Thresholds = cms.vdouble( 0.09 )
            # replace_with(process.hltMuonTkRelIsolationCut0p07Map, process.hltMuonTkRelIsolationCut0p09Map) # -- it does not change the module name (label)

            insert_modules_after(process, process.hltMuonTkRelIsolationCut0p07Map, process.hltMuonTkRelIsolationCut0p09Map)
            delattr(process, "hltMuonTkRelIsolationCut0p07Map")


        dic_module_before_after = {}
        for module in filters_by_type(process, "HLTMuonIsoFilter"):
            if hasattr(module, "DepTag"):
                # print(module._Labelable__label)

                isFound = False
                _vinputtag = module.DepTag.value()
                for index in range(0, len(_vinputtag)):
                    if _vinputtag[index] == "hltMuonTkRelIsolationCut0p07Map":
                        _vinputtag[index] = "hltMuonTkRelIsolationCut0p09Map"
                        isFound = True

                if isFound:
                    module_new = module.clone()

                    # -- update VInputTag                
                    module_new.DepTag = cms.VInputTag( *_vinputtag )

                    dic_module_before_after[module] = module_new


        for module in dic_module_before_after.keys():
            module_new = dic_module_before_after[module]

            # -- register on process with a new name
            label_new = module._Labelable__label.replace("0p07", "0p09")
            setattr(process, label_new, module_new)

            insert_modules_after(process, module, module_new)
            delattr(process, module._Labelable__label)

        return process

# -- proposal: nRegionsFactor = 4
def customizeTrkIso(process, nRegionsFactor = 1):
    # override tracking parameters...
    process.hltIter0L3MuonPixelSeedsFromPixelTracks = cms.EDProducer("SeedGeneratorFromProtoTracksEDProducer",
        InputCollection = cms.InputTag("hltPixelTracksInRegionIter0L3Muon"),
        InputVertexCollection = cms.InputTag( "hltTrimmedPixelVertices" ),
        originHalfLength = cms.double( 0.3 ),
        originRadius = cms.double( 0.1 ),
        useProtoTrackKinematics = cms.bool( False ),
        useEventsWithNoVertex = cms.bool( True ),
        TTRHBuilder = cms.string( "hltESPTTRHBuilderPixelOnly" ),
        usePV = cms.bool( False ),
        includeFourthHit = cms.bool( True ),
        SeedCreatorPSet = cms.PSet(  refToPSet_ = cms.string( "HLTSeedFromProtoTracks" ) )
    )

    process.hltIter0L3MuonCkfTrackCandidates = cms.EDProducer("CkfTrackCandidateMaker",
        cleanTrajectoryAfterInOut = cms.bool( False ),
        doSeedingRegionRebuilding = cms.bool( False ),
        onlyPixelHitsForSeedCleaner = cms.bool( False ),
        reverseTrajectories = cms.bool( False ),
        useHitsSplitting = cms.bool( False ),
        MeasurementTrackerEvent = cms.InputTag( "hltSiStripClusters" ),
        src = cms.InputTag( "hltIter0L3MuonPixelSeedsFromPixelTracks" ),
        clustersToSkip = cms.InputTag( "" ),
        phase2clustersToSkip = cms.InputTag( "" ),
        TrajectoryBuilderPSet = cms.PSet(  refToPSet_ = cms.string( "HLTIter0GroupedCkfTrajectoryBuilderIT" ) ),
        TransientInitialStateEstimatorParameters = cms.PSet( 
          propagatorAlongTISE = cms.string( "PropagatorWithMaterialParabolicMf" ),
          numberMeasurementsForFit = cms.int32( 4 ),
          propagatorOppositeTISE = cms.string( "PropagatorWithMaterialParabolicMfOpposite" )
        ),
        numHitsForSeedCleaner = cms.int32( 4 ),
        NavigationSchool = cms.string( "SimpleNavigationSchool" ),
        RedundantSeedCleaner = cms.string( "CachingSeedCleanerBySharedInput" ),
        TrajectoryCleaner = cms.string( "hltESPTrajectoryCleanerBySharedHits" ),
        maxNSeeds = cms.uint32( 100000 ),
        maxSeedsBeforeCleaning = cms.uint32( 1000 )
    )

    process.hltIter0L3MuonCtfWithMaterialTracks = cms.EDProducer("TrackProducer",
        useSimpleMF = cms.bool( True ),
        SimpleMagneticField = cms.string( "ParabolicMf" ),
        src = cms.InputTag( "hltIter0L3MuonCkfTrackCandidates" ),
        clusterRemovalInfo = cms.InputTag( "" ),
        beamSpot = cms.InputTag( "hltOnlineBeamSpot" ),
        Fitter = cms.string( "hltESPFittingSmootherIT" ),
        useHitsSplitting = cms.bool( False ),
        alias = cms.untracked.string( "ctfWithMaterialTracks" ),
        TrajectoryInEvent = cms.bool( False ),
        TTRHBuilder = cms.string( "hltESPTTRHBWithTrackAngle" ),
        AlgorithmName = cms.string( "hltIter0" ),
        Propagator = cms.string( "hltESPRungeKuttaTrackerPropagator" ),
        GeometricInnerState = cms.bool( True ),
        NavigationSchool = cms.string( "" ),
        MeasurementTracker = cms.string( "" ),
        MeasurementTrackerEvent = cms.InputTag( "hltSiStripClusters" )
    )

    process.hltIter0L3MuonTrackCutClassifier = cms.EDProducer("TrackCutClassifier",
        src = cms.InputTag( "hltIter0L3MuonCtfWithMaterialTracks" ),
        beamspot = cms.InputTag( "hltOnlineBeamSpot" ),
        vertices = cms.InputTag( "hltTrimmedPixelVertices" ),
        ignoreVertices = cms.bool( False ),
        qualityCuts = cms.vdouble( -0.7, 0.1, 0.7 ),
        mva = cms.PSet( 
          minPixelHits = cms.vint32( 0, 0, 0 ),
          maxDzWrtBS = cms.vdouble( 3.40282346639E38, 24.0, 15.0 ),
          dr_par = cms.PSet( 
            d0err = cms.vdouble( 0.003, 0.003, 0.003 ),
            dr_par2 = cms.vdouble( 3.40282346639E38, 0.6, 0.6 ),
            dr_par1 = cms.vdouble( 3.40282346639E38, 0.8, 0.8 ),
            dr_exp = cms.vint32( 4, 4, 4 ),
            d0err_par = cms.vdouble( 0.001, 0.001, 0.001 )
          ),
          maxLostLayers = cms.vint32( 1, 1, 1 ),
          min3DLayers = cms.vint32( 0, 0, 0 ),
          dz_par = cms.PSet( 
            dz_par1 = cms.vdouble( 3.40282346639E38, 0.75, 0.75 ),
            dz_par2 = cms.vdouble( 3.40282346639E38, 0.5, 0.5 ),
            dz_exp = cms.vint32( 4, 4, 4 )
          ),
          minNVtxTrk = cms.int32( 3 ),
          maxDz = cms.vdouble( 0.5, 0.2, 3.40282346639E38 ),
          minNdof = cms.vdouble( 1.0E-5, 1.0E-5, 1.0E-5 ),
          maxChi2 = cms.vdouble( 9999.0, 25.0, 16.0 ),
          maxChi2n = cms.vdouble( 1.2, 1.0, 0.7 ),
          maxDr = cms.vdouble( 0.5, 0.03, 3.40282346639E38 ),
          minLayers = cms.vint32( 3, 3, 3 )
        )
    )

    process.hltIter0L3MuonTrackSelectionHighPurity = cms.EDProducer("TrackCollectionFilterCloner",
        copyExtras = cms.untracked.bool(True),
        copyTrajectories = cms.untracked.bool(False),
        minQuality = cms.string('highPurity'),
        originalMVAVals = cms.InputTag("hltIter0L3MuonTrackCutClassifier","MVAValues"),
        originalQualVals = cms.InputTag("hltIter0L3MuonTrackCutClassifier","QualityMasks"),
        originalSource = cms.InputTag("hltIter0L3MuonCtfWithMaterialTracks")
    )

    # update ROIs
    process.hltPixelTracksTrackingRegionsForSeedsL3Muon.RegionPSet.maxNVertices = cms.int32( 1 * nRegionsFactor )
    process.hltPixelTracksTrackingRegionsForSeedsL3Muon.RegionPSet.maxNRegions  = cms.int32( 10 * nRegionsFactor )


def customizeTrkIsoNoVtx(process, nRegionsFactor = 1):
    # override tracking parameters...
    process.hltIter0L3MuonPixelSeedsFromPixelTracksNoVtx = cms.EDProducer("SeedGeneratorFromProtoTracksEDProducer",
        InputCollection = cms.InputTag("hltPixelTracksInRegionIter0L3MuonNoVtx"),
        InputVertexCollection = cms.InputTag( "hltTrimmedPixelVertices" ),
        originHalfLength = cms.double( 0.3 ),
        originRadius = cms.double( 0.1 ),
        useProtoTrackKinematics = cms.bool( False ),
        useEventsWithNoVertex = cms.bool( True ),
        TTRHBuilder = cms.string( "hltESPTTRHBuilderPixelOnly" ),
        usePV = cms.bool( False ),
        includeFourthHit = cms.bool( True ),
        SeedCreatorPSet = cms.PSet(  refToPSet_ = cms.string( "HLTSeedFromProtoTracks" ) )
    )

    process.hltIter0L3MuonCkfTrackCandidatesNoVtx = cms.EDProducer("CkfTrackCandidateMaker",
        cleanTrajectoryAfterInOut = cms.bool( False ),
        doSeedingRegionRebuilding = cms.bool( False ),
        onlyPixelHitsForSeedCleaner = cms.bool( False ),
        reverseTrajectories = cms.bool( False ),
        useHitsSplitting = cms.bool( False ),
        MeasurementTrackerEvent = cms.InputTag( "hltSiStripClusters" ),
        src = cms.InputTag( "hltIter0L3MuonPixelSeedsFromPixelTracksNoVtx" ),
        clustersToSkip = cms.InputTag( "" ),
        phase2clustersToSkip = cms.InputTag( "" ),
        TrajectoryBuilderPSet = cms.PSet(  refToPSet_ = cms.string( "HLTIter0GroupedCkfTrajectoryBuilderIT" ) ),
        TransientInitialStateEstimatorParameters = cms.PSet( 
          propagatorAlongTISE = cms.string( "PropagatorWithMaterialParabolicMf" ),
          numberMeasurementsForFit = cms.int32( 4 ),
          propagatorOppositeTISE = cms.string( "PropagatorWithMaterialParabolicMfOpposite" )
        ),
        numHitsForSeedCleaner = cms.int32( 4 ),
        NavigationSchool = cms.string( "SimpleNavigationSchool" ),
        RedundantSeedCleaner = cms.string( "CachingSeedCleanerBySharedInput" ),
        TrajectoryCleaner = cms.string( "hltESPTrajectoryCleanerBySharedHits" ),
        maxNSeeds = cms.uint32( 100000 ),
        maxSeedsBeforeCleaning = cms.uint32( 1000 )
    )

    process.hltIter0L3MuonCtfWithMaterialTracksNoVtx = cms.EDProducer("TrackProducer",
        useSimpleMF = cms.bool( True ),
        SimpleMagneticField = cms.string( "ParabolicMf" ),
        src = cms.InputTag( "hltIter0L3MuonCkfTrackCandidatesNoVtx" ),
        clusterRemovalInfo = cms.InputTag( "" ),
        beamSpot = cms.InputTag( "hltOnlineBeamSpot" ),
        Fitter = cms.string( "hltESPFittingSmootherIT" ),
        useHitsSplitting = cms.bool( False ),
        alias = cms.untracked.string( "ctfWithMaterialTracks" ),
        TrajectoryInEvent = cms.bool( False ),
        TTRHBuilder = cms.string( "hltESPTTRHBWithTrackAngle" ),
        AlgorithmName = cms.string( "hltIter0" ),
        Propagator = cms.string( "hltESPRungeKuttaTrackerPropagator" ),
        GeometricInnerState = cms.bool( True ),
        NavigationSchool = cms.string( "" ),
        MeasurementTracker = cms.string( "" ),
        MeasurementTrackerEvent = cms.InputTag( "hltSiStripClusters" )
    )

    process.hltIter0L3MuonTrackCutClassifierNoVtx = cms.EDProducer("TrackCutClassifier",
        src = cms.InputTag( "hltIter0L3MuonCtfWithMaterialTracksNoVtx" ),
        beamspot = cms.InputTag( "hltOnlineBeamSpot" ),
        vertices = cms.InputTag( "hltTrimmedPixelVertices" ),
        ignoreVertices = cms.bool( False ),
        qualityCuts = cms.vdouble( -0.7, 0.1, 0.7 ),
        mva = cms.PSet( 
          minPixelHits = cms.vint32( 0, 0, 0 ),
          maxDzWrtBS = cms.vdouble( 3.40282346639E38, 24.0, 15.0 ),
          dr_par = cms.PSet( 
            d0err = cms.vdouble( 0.003, 0.003, 0.003 ),
            dr_par2 = cms.vdouble( 3.40282346639E38, 0.6, 0.6 ),
            dr_par1 = cms.vdouble( 3.40282346639E38, 0.8, 0.8 ),
            dr_exp = cms.vint32( 4, 4, 4 ),
            d0err_par = cms.vdouble( 0.001, 0.001, 0.001 )
          ),
          maxLostLayers = cms.vint32( 1, 1, 1 ),
          min3DLayers = cms.vint32( 0, 0, 0 ),
          dz_par = cms.PSet( 
            dz_par1 = cms.vdouble( 3.40282346639E38, 0.75, 0.75 ),
            dz_par2 = cms.vdouble( 3.40282346639E38, 0.5, 0.5 ),
            dz_exp = cms.vint32( 4, 4, 4 )
          ),
          minNVtxTrk = cms.int32( 3 ),
          maxDz = cms.vdouble( 0.5, 0.2, 3.40282346639E38 ),
          minNdof = cms.vdouble( 1.0E-5, 1.0E-5, 1.0E-5 ),
          maxChi2 = cms.vdouble( 9999.0, 25.0, 16.0 ),
          maxChi2n = cms.vdouble( 1.2, 1.0, 0.7 ),
          maxDr = cms.vdouble( 0.5, 0.03, 3.40282346639E38 ),
          minLayers = cms.vint32( 3, 3, 3 )
        )
    )

    process.hltIter0L3MuonTrackSelectionHighPurityNoVtx = cms.EDProducer("TrackCollectionFilterCloner",
        copyExtras = cms.untracked.bool(True),
        copyTrajectories = cms.untracked.bool(False),
        minQuality = cms.string('highPurity'),
        originalMVAVals = cms.InputTag("hltIter0L3MuonTrackCutClassifierNoVtx","MVAValues"),
        originalQualVals = cms.InputTag("hltIter0L3MuonTrackCutClassifierNoVtx","QualityMasks"),
        originalSource = cms.InputTag("hltIter0L3MuonCtfWithMaterialTracksNoVtx")
    )

    # update ROIs
    process.hltPixelTracksTrackingRegionsForSeedsL3MuonNoVtx.RegionPSet.maxNVertices = cms.int32( 1 * nRegionsFactor )
    process.hltPixelTracksTrackingRegionsForSeedsL3MuonNoVtx.RegionPSet.maxNRegions  = cms.int32( 10 * nRegionsFactor )

    return process