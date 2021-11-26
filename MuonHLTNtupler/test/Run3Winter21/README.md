# Setup @ CMSSW_12_1_0

Similar with CMSSW_12_0_1, but some updates was needed for the ntupler to be compatible with CMSSW_12_1_0

* Update ```propagatorToMuon(iConfig)``` to ```propagatorToMuon(iConfig, consumesCollector())```
* Add ```process.options.numberOfConcurrentLuminosityBlocks = 1``` in the configuration file to make it run on RAW+MINIAOD

```
cmsrel CMSSW_12_1_0
cd CMSSW_12_1_0/src
cmsenv
git cms-init

git cms-addpkg HLTrigger/Configuration
git cms-addpkg RecoJets/JetProducers
git clone https://github.com/khaosmos93/MuonHLTForRun3.git HLTrigger/Configuration/python/MuonHLTForRun3
git clone git@github.com:KyeongPil-Lee/MuonHLTTool.git -b Run3_IDIso

# -- rechit-based rho producer
cd $CMSSW_BASE/src/MuonHLTTool/MuonHLTNtupler/test/Run3Winter21/File_RechitBasedRho
cp BuildFile.xml FixedGridRhoProducerFastjetFromRecHit.cc \
$CMSSW_BASE/src/RecoJets/JetProducers/plugins
cd $CMSSW_BASE/src

scram b -j 8 >&scram.log&

cd MuonHLTTool/MuonHLTNtupler/test/Run3Winter21/RerunHLT_RAWMINIAOD/CMSSW_12_1_0/IsoFilterIgnored/RechitRho

# -- hltGetConfiguration (MC)
hltGetConfiguration /dev/CMSSW_12_1_0/GRun \
--process MYHLT \
--eras Run3 \
--mc --globaltag auto:run3_mc_GRun \
--unprescale \
--paths \
HLTriggerFirstPath,\
HLT_Mu24_v*,\
HLT_IsoMu24_v*,\
HLT_Mu50_v*,\
HLT_OldMu100_v*,\
HLT_TkMu100_v*,\
HLTriggerFinalPath,\
HLTAnalyzerEndpath \
--input /store/mc/Run3Winter21DRMiniAOD/ZToMuMu_M-50To120_TuneCP5_14TeV-powheg-pythia8/GEN-SIM-DIGI-RAW/FlatPU30to80FEVT_112X_mcRun3_2021_realistic_v16-v2/130002/87cbcfff-3766-457c-9c5b-c5150664699d.root \
--max-events 100 \
--full --offline --no-output >HLTConfig_default.py

cp HLTConfig_default.py HLTConfig_IsoFilterIgnored_RechitRho.py

# -- update the configuration
# -- 1) ignore isolation filters
# -- 2) add muon HLT customizers
# -- 3) add the customizer for rechit based rho
# -- 4) add the customizer for ntupler

# -- test it
cmsRun HLTConfig_IsoFilterIgnored_RechitRho.py >&HLTConfig_IsoFilterIgnored_RechitRho.log&

# -- crab submit
source /cvmfs/cms.cern.ch/common/crab-setup.sh
python3 crabconfig_MC.py
```

### 1) customizer for ignoring isolation filters

```python
from MuonHLTTool.MuonHLTNtupler.customizerForMuonHLTIsolationStudy import *
process = customizeForMuonHLTIsolation_ignoreIsoFilter_IsoMu24(process)
```

### 2) customizer for muon HLT

```python
# -- turn off OI and IO seeding part
from HLTrigger.Configuration.MuonHLTForRun3.customizeMuonHLTForRun3 import customizeMuonHLTForAll
process = customizeMuonHLTForAll(process, "MYHLT", True, True, True, False, False)
```

### 3) customizer for rechit based rho

```python
# -- rechit-based rho
from MuonHLTTool.MuonHLTNtupler.customizerForMuonHLTIsolation_rhoFromPFRechit import *
process = customizeForMuonHLTIsolation_differentRhoForECALHCAL(process)
```

### 4) customizer for ntupler

```python
# -- RAW+MINIAOD test
process.source.fileNames = cms.untracked.vstring("/store/mc/Run3Winter21DRMiniAOD/ZToMuMu_M-50To120_TuneCP5_14TeV-powheg-pythia8/MINIAODSIM/FlatPU30to80FEVT_112X_mcRun3_2021_realistic_v16-v2/130000/e891d16a-78a3-4c5e-aabe-1b6f8c1f3b58.root")
process.source.secondaryFileNames = cms.untracked.vstring("/store/mc/Run3Winter21DRMiniAOD/ZToMuMu_M-50To120_TuneCP5_14TeV-powheg-pythia8/GEN-SIM-DIGI-RAW/FlatPU30to80FEVT_112X_mcRun3_2021_realistic_v16-v2/130001/0cf6e16c-7a35-47f4-9935-0f5df2d9a23f.root")
process.maxEvents.input = cms.untracked.int32( 100 )

from MuonHLTTool.MuonHLTNtupler.customizerForMuonHLTNtupler import *
customizerFuncForMuonHLTNtupler(process, "MYHLT")
process.ntupler.isMiniAOD             = cms.bool(True)
process.ntupler.offlineVertex         = cms.untracked.InputTag("offlineSlimmedPrimaryVertices")
process.ntupler.offlineMuon           = cms.untracked.InputTag("slimmedMuons")
process.ntupler.triggerObject_miniAOD = cms.untracked.InputTag("slimmedPatTrigger")
process.ntupler.PUSummaryInfo         = cms.untracked.InputTag("slimmedAddPileupInfo")
process.ntupler.genParticle           = cms.untracked.InputTag("prunedGenParticles")
```



# Setup @ CMSSW_12_0_1

* Reference: https://github.com/khaosmos93/MuonHLTForRun3

```shell
cmsrel CMSSW_12_0_1
cd CMSSW_12_0_1
cmsenv
git cms-init

git cms-addpkg HLTrigger/Configuration
git clone https://github.com/khaosmos93/MuonHLTForRun3.git HLTrigger/Configuration/python/MuonHLTForRun3
git clone git@github.com:KyeongPil-Lee/MuonHLTTool.git -b Run3_IDIso

scram b -j 8 >&scram.log&
```



# Rerunning HLT

## Step 1: hltGetConfiguration

* works only in **lxplus** (as far as I know)
  * ```/afs/cern.ch/user/k/kplee/work/private/Detector/MuonHLTTool/Run3_IDIso/CMSSW_12_0_1/src```

```shell
hltGetConfiguration /dev/CMSSW_12_0_0/GRun/V6 --type GRun \
--path HLTriggerFirstPath,HLT_IsoMu24_v*,HLT_Mu50_v*,HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v*,HLTriggerFinalPath,HLTAnalyzerEndpath \
--unprescale --cff >$CMSSW_BASE/src/HLTrigger/Configuration/python/HLT_MuonHLT_cff.py
```

-> once it is generated in lxplus, scp the file to your working machine from lxplus

## Step 2: cmsDriver

* works anywhere if the above configuration is available in the working space

### for MC:

```shell
cmsDriver.py hlt_muon \
--python_filename=hlt_muon_Run3_mc.py \
--step HLT:MuonHLT \
--process MYHLT --era=Run3 \
--mc --conditions=auto:phase1_2021_realistic \
--customise=HLTrigger/Configuration/MuonHLTForRun3/customizeMuonHLTForRun3.customizeDoubleMuIsoFix \
--customise=HLTrigger/Configuration/MuonHLTForRun3/customizeMuonHLTForRun3.customizeMuonHLTForDoubletRemoval \
--customise=HLTrigger/Configuration/MuonHLTForRun3/customizeMuonHLTForRun3.customizeMuonHLTForCscSegment \
--customise=HLTrigger/Configuration/MuonHLTForRun3/customizeMuonHLTForRun3.customizeMuonHLTForGEM \
--filein=/store/mc/Run3Winter21DRMiniAOD/DYToLL_M-50_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW/FlatPU30to80FEVT_112X_mcRun3_2021_realistic_v16-v2/120003/e786c41e-21ba-489f-880c-42d0a248e59e.root \
-n 100 --no_output --no_exec
```



### for DATA:

```shell
cmsDriver.py hlt_muon \
--python_filename=hlt_muon_Run3_data.py \
--step HLT:MuonHLT \
--process MYHLT --era=Run3 \
--data --conditions=auto:run3_hlt \
--customise=HLTrigger/Configuration/customizeHLTforCMSSW.customiseFor2018Input \
--customise=HLTrigger/Configuration/MuonHLTForRun3/customizeMuonHLTForRun3.customizeDoubleMuIsoFix \
--customise=HLTrigger/Configuration/MuonHLTForRun3/customizeMuonHLTForRun3.customizeMuonHLTForDoubletRemoval \
--customise=HLTrigger/Configuration/MuonHLTForRun3/customizeMuonHLTForRun3.customizeMuonHLTForCscSegment \
--filein=/store/data/Run2018D/EphemeralHLTPhysics1/RAW/v1/000/323/775/00000/D5D2CF9C-2557-4243-B42E-4345100839DA.root \
-n 100 --no_output --no_exec
```



## use of PF rechit for rho calculation 

### Rho producer from PF rechit

[FixedGridRhoProducerFastjetFromRecHit.cc](https://github.com/swagata87/cmssw/blob/3f4de291f884946e3ed3866473e843c0b7abc159/RecoJets/JetProducers/plugins/FixedGridRhoProducerFastjetFromRecHit.cc)



### Setup in CMSSW

```shell
# -- under $CMSSW_BASE/src

git cms-addpkg RecoJets/JetProducers

# -- copy FixedGridRhoProducerFastjetFromRecHit.cc under RecoJets/JetProducers/plugin
# -- update RecoJets/JetProducers/buildFile.xml
# ---- add <use name="CondFormats/EcalObjects"/>

scram b -j 10 >&scram.log&
```



### Customizer to use the rho from PF rechit in muon HLT isolation

Use ```Run3_IDIso/MuonHLTNtupler/python/customizerForMuonHLTIsolation_rhoFromPFRechit.py```



**Case 1: different rho value for ECAL and HCAL isolation**

```python
from MuonHLTTool.MuonHLTNtupler.customizerForMuonHLTIsolation_rhoFromPFRechit import *
process = customizeForMuonHLTIsolation_differentRhoForECALHCAL(process)
```



**Case 2: same rho value for ECAL and HCAL isolation**

```python
from MuonHLTTool.MuonHLTNtupler.customizerForMuonHLTIsolation_rhoFromPFRechit import *
process = customizeForMuonHLTIsolation_sameRhoForECALHCAL(process)
```

