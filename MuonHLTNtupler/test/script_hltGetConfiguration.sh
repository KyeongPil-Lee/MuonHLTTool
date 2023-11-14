# -- for MC

# -- pure HLT rerun
hltGetConfiguration /dev/CMSSW_13_2_0/GRun \
--globaltag 126X_mcRun3_2023_forPU65_v1 \
--path HLTriggerFirstPath,\
HLT_IsoMu24_v*,\
HLT_Mu50_v*,\
HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v*,\
HLTriggerFinalPath,\
HLTAnalyzerEndpath \
--process MYHLT \
--mc \
--unprescale \
--max-events 100 \
--input /store/mc/Run3Winter23Digi/DYTo2L_MLL-50_TuneCP5_13p6TeV_pythia8/GEN-SIM-RAW/KeepSi_RnD_126X_mcRun3_2023_forPU65_v1-v2/50004/cf2170cc-33e7-4795-9d8c-58f2be77492d.root \
--eras Run3 --l1-emulator FullMC --l1 L1Menu_Collisions2023_v1_3_0_xml \
--output full \
> hltConfig_MC_output_full.py 
# -- output = full requires some modification
# ---- https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideGlobalHLT#hlt_GetConfiguration

# cmsRun hltConfig_MC_output_full.py >&hltConfig_MC_output_full.log&


# -- HLT rerun + ntupler
hltGetConfiguration /dev/CMSSW_13_2_0/GRun \
--globaltag 126X_mcRun3_2023_forPU65_v1 \
--path HLTriggerFirstPath,\
HLT_IsoMu24_v*,\
HLT_Mu50_v*,\
HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v*,\
HLTriggerFinalPath,\
HLTAnalyzerEndpath \
--process MYHLT \
--mc \
--unprescale \
--max-events 100 \
--input /store/mc/Run3Winter23Digi/DYTo2L_MLL-50_TuneCP5_13p6TeV_pythia8/GEN-SIM-RAW/KeepSi_RnD_126X_mcRun3_2023_forPU65_v1-v2/50004/cf2170cc-33e7-4795-9d8c-58f2be77492d.root \
--eras Run3 --l1-emulator FullMC --l1 L1Menu_Collisions2023_v1_3_0_xml \
--customise MuonHLTTool/MuonHLTNtupler/Customizer_MuonHLTNtuple.addNtupleModuleToProcess \
--output none \
> hltConfig_MC_ntupler.py
# -- modify to include secondary dataset (to run on RAW + MINIAOD sample) & turn-on "miniAOD" option of the ntupler

# cmsRun hltConfig_MC_ntupler.py >&hltConfig_MC_ntupler.log&