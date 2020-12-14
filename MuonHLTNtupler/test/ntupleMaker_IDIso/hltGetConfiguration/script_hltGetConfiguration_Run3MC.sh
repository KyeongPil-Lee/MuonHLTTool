#!/bin/bash

# -- L1 is re-emulated following the reference below:
# -- https://twiki.cern.ch/twiki/bin/view/CMS/TriggerStudies#Trigger_samples
# ---- "Note there was a problem with the EMTF and therefore the L1 must be re-emulated to study triggers involving L1 endcap muons."

hltGetConfiguration /dev/CMSSW_11_0_0/GRun \
--globaltag 110X_mcRun3_2021_realistic_v6 \
--path HLTriggerFirstPath,HLT_Mu24_v*,HLT_IsoMu24_v*,HLT_Mu50_v*,HLTriggerFinalPath \
--input="root://xrootd-cms.infn.it//store/mc/Run3Winter20DRPremixMiniAOD/DYToLL_M-50_TuneCP5_14TeV-pythia8_HCAL/GEN-SIM-DIGI-RAW/110X_mcRun3_2021_realistic_v6-v2/280000/F2982585-CFEB-2345-BB4F-75447796B2F9.root" \
--process MYHLT --full --offline --mc \
--l1-emulator FullMC \
--prescale none --max-events 100 --output none > HLTConfig_MC.py

