# Setup @ CMSSW_12_1_0

CMSSW setup is same with MC case ([link](https://github.com/KyeongPil-Lee/MuonHLTTool/tree/Run3_IDIso/MuonHLTNtupler/test/Run3Winter21#setup--cmssw_12_1_0))

```bash
# -- slightly different with MC case; a few parameters are adjusted to run it on data
hltGetConfiguration /dev/CMSSW_12_1_0/GRun \
--process MYHLT \
--eras Run3 \
--data --globaltag auto:run3_hlt \
--prescale 2.0e34+ZB+HLTPhysics \
--paths \
HLTriggerFirstPath,\
HLT_Mu24_v*,\
HLT_IsoMu24_v*,\
HLT_Mu50_v*,\
HLT_OldMu100_v*,\
HLT_TkMu100_v*,\
HLTriggerFinalPath,\
HLTAnalyzerEndpath \
--input file:/eos/cms/store/data/Run2018D/EphemeralHLTPhysics1/RAW/v1/000/323/775/00000/D5D2CF9C-2557-4243-B42E-4345100839DA.root \
--max-events 100 \
--full --offline --no-output >HLTConfig.py
```

## Customizers

```pyhton
from HLTrigger.Configuration.customizeHLTforCMSSW import *
process = customiseFor2018Input(process)

from MuonHLTTool.MuonHLTNtupler.customizerForMuonHLTIsolationStudy import *
process = customizeForMuonHLTIsolation_ignoreIsoFilter_IsoMu24(process)

# -- turn off OI and IO seeding part
# -- 3rd option: patatrack
from HLTrigger.Configuration.MuonHLTForRun3.customizeMuonHLTForRun3 import customizeMuonHLTForAll
process = customizeMuonHLTForAll(process, "MYHLT", True, True, True, False, False)

# -- rechit-based rho
from MuonHLTTool.MuonHLTNtupler.customizerForMuonHLTIsolation_rhoFromPFRechit import *
process = customizeForMuonHLTIsolation_differentRhoForECALHCAL(process)

process.options.numberOfThreads=cms.untracked.uint32(1) # -- for CRAB job
process.options.numberOfConcurrentLuminosityBlocks = 1
from MuonHLTTool.MuonHLTNtupler.customizerForMuonHLTNtupler import *
customizerFuncForMuonHLTNtupler(process, "MYHLT")
```

