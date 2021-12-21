# Timing study for tuning of working point of Muon HLT isolation (w/ regional tracking, patatrack)

## Purpose

* As a part of tuning of muon HLT isolation
* comparison before and after changing the working point

## Setup

* Reference for timing study: https://twiki.cern.ch/twiki/bin/view/CMS/TriggerStudiesTiming
* The setup listed in this [Setup @ CMSSW_12_1_0](https://github.com/KyeongPil-Lee/MuonHLTTool/tree/Run3_IDIso/MuonHLTNtupler/test/Run3Winter21#setup--cmssw_12_1_0) should be done first



### Preparation for the configuration

```shell
# -- lxplus
cd /afs/cern.ch/user/k/kplee/work/private/Detector/MuonHLTTool/Run3_IDIso/CMSSW_12_1_0/src
cmsenv

# -- update the rho producer with the latest one
# -- https://github.com/cms-sw/cmssw/blob/3d014b7eb0ef23b178b0acb1b34c62c19959bfca/RecoJets/JetProducers/plugins/FixedGridRhoProducerFastjetFromRecHit.cc
scram b -j 10 >&scram.log&

cd MuonHLTTool/TimingStudy/WPTune_Regional

# -- getting the HLT menu to measure the timing
# -- "--timing" does not needed: it will be controlled by the timing script
# -- "--input" and "--max-events" will also be controlled by the timing script later
hltGetConfiguration /dev/CMSSW_12_1_0/GRun/V14 \
--process MYHLT \
--eras Run3 \
--data --globaltag auto:run3_hlt \
--unprescale \
--path HLTriggerFirstPath,HLT_IsoMu24_v*,HLT_Mu50_v*,HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v*,HLTriggerFinalPath,HLTAnalyzerEndpath \
--input /store/data/Run2018D/EphemeralHLTPhysics1/RAW/v1/000/323/775/00000/0244D183-F28D-2741-9DBF-1638BEDC734E.root \
--max-events 100 \
--full --offline --no-output >HLTConfig_default.py

```

copy & paste below python lines at the end of ```HLTConfig_default.py```:

```python
# -- ignore isolation filters
from MuonHLTTool.MuonHLTNtupler.customizerForMuonHLTIsolationStudy import *
process = customizeForMuonHLTIsolation_ignoreIsoFilter_IsoMu24(process)

# -- turn off OI and IO seeding part
from HLTrigger.Configuration.MuonHLTForRun3.customizeMuonHLTForRun3 import customizeMuonHLTForAll
process = customizeMuonHLTForAll(process, "MYHLT", True, True, True, False, False)

# -- rechit-based rho
from MuonHLTTool.MuonHLTNtupler.customizerForMuonHLTIsolation_rhoFromPFRechit import *
process = customizeForMuonHLTIsolation_differentRhoForECALHCAL(process)
```

duplicate ```HLTConfig_default.py``` for the new configuration with the updated working point

```shell
cp HLTConfig_default.py HLTConfig_newWP.py
```

Open ```HLTConfig_newWP.py``` and put the lines at the end of the file for the new WP value

```python
# -- ECAL
process.hltL3crIsoL1sSingleMu22L1f0L2f10QL3f24QL3pfecalIsoRhoFilteredEB0p14EE0p10.thrOverEEB = cms.vdouble( 0.23020 )
process.hltL3crIsoL1sSingleMu22L1f0L2f10QL3f24QL3pfecalIsoRhoFilteredEB0p14EE0p10.thrOverEEE = cms.vdouble( 0.19300 )

# -- HCAL
process.hltL3crIsoL1sSingleMu22L1f0L2f10QL3f24QL3pfhcalIsoRhoFilteredHB0p16HE0p20.thrOverEEB = cms.vdouble( 0.20590 )
process.hltL3crIsoL1sSingleMu22L1f0L2f10QL3f24QL3pfhcalIsoRhoFilteredHB0p16HE0p20.thrOverEEE = cms.vdouble( 0.26710 )

# -- Tracker
process.hltMuonTkRelIsolationCut0p07Map.CutsPSet.Thresholds = cms.vdouble( 0.09658 )
```



### Run the timing script

```shell
ssh -Y vocms007
cd /afs/cern.ch/user/k/kplee/work/private/Detector/MuonHLTTool/Run3_IDIso/CMSSW_12_1_0/src
source /cvmfs/cms.cern.ch/cmsset_default.sh
cmsenv

cd MuonHLTTool/TimingStudy/WPTune_Regional

python3 /data/timing/scripts/timing/submit.py HLTConfig_default.py
python3 /data/timing/scripts/timing/submit.py HLTConfig_newWP.py
```



# Results

Saved under the directory ```/data/timing/output/kplee``` in vocms007 machine

Copy the results from vocms007

```
scp -r kplee@vocms007:/data/timing/output/kplee/CMSSW_12_1_0.20211220* ./
```

