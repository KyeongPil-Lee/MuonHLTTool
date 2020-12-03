# Modification needed for 2016 Data Processing

## Ntupler code

* ```triggerObj.unpackNamesAndLabels(iEvent, *h_triggerResults);``` -> 
  ```triggerObj.unpackPathNames(names);```
* Comment out ```MuonHLTNtupler::isNewHighPtMuon``` part (just return false)
  * ```expectedNnumberOfMatchedStations``` doesn't work in 80X


## Python configuration

* ```process.ntupler.triggerObject_miniAOD = cms.untracked.InputTag("slimmedPatTrigger")``` ->
  ```process.ntupler.triggerObject_miniAOD = cms.untracked.InputTag("selectedPatTrigger")```
  * InputTag is different



## Issue on 2020.12.03

* Rerunning HLT of 2016 data shows an error (terminated with segmentation violation)
  * 80X CMSSW with SLC7 architecture & running on SLC7 machine could be the problem
  * But there is no SLC6 machine available

# Modification needed for 2018 Data Processing

## Ntupler code
* ```triggerObj.unpackPathNames(names);``` ->
  ```triggerObj.unpackNamesAndLabels(iEvent, *h_triggerResults);```
  
