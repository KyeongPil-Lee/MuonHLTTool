# ID and Isolation Improvement for Run 3

Branch to study ID and isolation improvement for Run 3 muon HLT



## Working area

### Ntuple maker

KNU

```
/u/user/kplee/scratch/ServiceWork/MuonHLTTool/Run3_IDIso/CMSSW_11_0_0_patch1/src/MuonHLTTool
```

* KNU is not stable (terminal freezes too frequently): move to lxplus



LXPLUS

```
/afs/cern.ch/user/k/kplee/work/private/Detector/MuonHLTTool/Run3_IDIso/CMSSW_11_0_0_patch1/src/MuonHLTTool
```



## DY Sample

* **DYToLL_M-50_TuneCP5_14TeV-pythia8**

  * RAW+MINIAOD: some of MINIAOD files do not have parent -> CRAB job is not submitted

  ```
  Task name:			201214_073624:kplee_crab_DYLL_M50_Pythia8
  Grid scheduler - Task Worker:	N/A yet - crab-prod-tw01
  Status on the CRAB server:	SUBMITFAILED
  Task URL to use for HELP:	https://cmsweb.cern.ch/crabserver/ui/task/201214_073624%3Akplee_crab_DYLL_M50_Pythia8
  Dashboard monitoring URL:	https://monit-grafana.cern.ch/d/cmsTMDetail/cms-task-monitoring-task-view?orgId=11&var-user=kplee&var-task=201214_073624%3Akplee_crab_DYLL_M50_Pythia8&from=1607927784000&to=now
  In case of issues with the dashboard, please provide feedback to hn-cms-computing-tools@cern.ch
  Failure message from server:	The CRAB3 server backend refuses to submit jobs to the Grid scheduler
  				because you specified useParents=True but some your files have noparents.
  				Example: /store/mc/Run3Winter20DRMiniAOD/DYToLL_M-50_TuneCP5_14TeV-pythia8/MINIAODSIM/DRFlatPU30to80_110X_mcRun3_2021_realistic_v6-v2/240000/60E027C5-A094-1F49-9555-5DFD417ED1FF.root
  Log file is /afs/cern.ch/work/k/kplee/private/Detector/MuonHLTTool/Run3_IDIso/CMSSW_11_0_0_patch1/src/MuonHLTTool/MuonHLTNtupler/test/ntupleMaker_IDIso/RerunHLT_RAWMINIAOD/CRABDir/crab_DYLL_M50_Pythia8/crab.log
  
  ```

  * RAW+AOD: AOD is not on DISK -> not submitted

=> use **DYToLL_M-50_TuneCP5_14TeV-pythia8_HCAL** instead

