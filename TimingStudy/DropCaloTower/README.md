# Timing study for dropping calo tower & moving to rechit based rho

## Purpose

* As a part of the study to drop the calo tower sequences in muon HLT
* rho, used to correct muon HLT isolation, is calculated based on calo tower information: test to use rechit information instead
* Compare the timing between calo tower based rho vs. rechit based rho



## Setup

* Reference for timing study: https://twiki.cern.ch/twiki/bin/view/CMS/TriggerStudiesTiming
* The setup listed in this [README](https://github.com/KyeongPil-Lee/MuonHLTTool/blob/Run3_IDIso/MuonHLTNtupler/test/Run3Winter21/README.md) should be done first



```shell
# -- lxplus
cd /afs/cern.ch/user/k/kplee/work/private/Detector/MuonHLTTool/Run3_IDIso/CMSSW_12_0_1/src
cmsenv

# -- update the rho producer with the latest one
# -- https://github.com/cms-sw/cmssw/blob/3d014b7eb0ef23b178b0acb1b34c62c19959bfca/RecoJets/JetProducers/plugins/FixedGridRhoProducerFastjetFromRecHit.cc
scram b -j 10 >&scram.log&

cd MuonHLTTool/TimingStudy/DropCaloTower

# -- getting the HLT menu to measure the timing
# -- "--timing" does not needed: it will be controlled by the timing script
# -- "--input" and "--max-events" will also be controlled by the timing script later
# -- HLTrigger/Configuration/MuonHLTForRun3/customizeMuonHLTForRun3.customizeDoubleMuIsoFix: removed (as double muon trigger is not used for this test)
hltGetConfiguration /dev/CMSSW_12_0_0/GRun/V6 \
--process MYHLT \
--eras Run3 \
--data --globaltag auto:run3_hlt \
--unprescale \
--path HLTriggerFirstPath,HLT_IsoMu24_v*,HLT_Mu50_v*,HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v*,HLTriggerFinalPath,HLTAnalyzerEndpath \
--customise \
HLTrigger/Configuration/MuonHLTForRun3/customizeMuonHLTForRun3.customizeMuonHLTForDoubletRemoval,\
HLTrigger/Configuration/MuonHLTForRun3/customizeMuonHLTForRun3.customizeMuonHLTForCscSegment,\
HLTrigger/Configuration/MuonHLTForRun3/customizeMuonHLTForRun3.customizeMuonHLTForGEM \
--input /store/data/Run2018D/EphemeralHLTPhysics1/RAW/v1/000/323/775/00000/0244D183-F28D-2741-9DBF-1638BEDC734E.root \
--max-events 100 \
--full --offline --no-output >HLTConfig_caloTower.py

# -- copy HLTConfig_caloTower.py as HLTConfig_rechit.py
# -- add the two more lines below in HLTConfig_rechit.py:
# ---- from MuonHLTTool.MuonHLTNtupler.customizerForMuonHLTIsolation_rhoFromPFRechit import *
# ---- process = customizeForMuonHLTIsolation_differentRhoForECALHCAL(process)

ssh -Y vocms007
cd /afs/cern.ch/user/k/kplee/work/private/Detector/MuonHLTTool/Run3_IDIso/CMSSW_12_0_1/src
source /cvmfs/cms.cern.ch/cmsset_default.sh
cmsenv

cd MuonHLTTool/TimingStudy/DropCaloTower

python3 /data/timing/scripts/timing/submit.py HLTConfig_caloTower.py
python3 /data/timing/scripts/timing/submit.py HLTConfig_rechit.py
```

