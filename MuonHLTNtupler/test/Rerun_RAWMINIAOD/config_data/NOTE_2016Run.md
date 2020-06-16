# Modification need for 2016 Data Processing

## Ntupler code

* ```triggerObj.unpackNamesAndLabels(iEvent, *h_triggerResults);``` -> 
  ```triggerObj.unpackPathNames(names);```
* Comment out ```MuonHLTNtupler::isNewHighPtMuon``` part (just return false)
  * ```expectedNnumberOfMatchedStations``` doesn't work in 80X



## Python configuration

* ```process.ntupler.triggerObject_miniAOD = cms.untracked.InputTag("slimmedPatTrigger")``` ->
  ```process.ntupler.triggerObject_miniAOD = cms.untracked.InputTag("selectedPatTrigger")```
  * InputTag is different

