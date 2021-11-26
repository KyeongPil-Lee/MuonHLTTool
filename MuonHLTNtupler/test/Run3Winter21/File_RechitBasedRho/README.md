# Files for rechit-based rho

* Reference: https://github.com/cms-sw/cmssw/pull/36157/



## Usage

To produce rechit-based rho value in muon HLT

Two files should be copied under ```RecoJets/JetProducers```

```
cd $CMSSW_BASE/src/MuonHLTTool/Run3_IDIso/MuonHLTNtupler/test/Run3Winter21/File_RechitBasedRho
cp BuildFile.xml FixedGridRhoProducerFastjetFromRecHit \
$CMSSW_BASE/src/RecoJets/JetProducers/plugins
```

