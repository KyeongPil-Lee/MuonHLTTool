#!bin/bash
# -- submit using crab-dev (for CMSSW >= 12_0_X)

crab-dev submit -c crabconfig_MC_QCDMuEnriched_Pt120to170_Pythia8_FlatPU30to80.py
crab-dev submit -c crabconfig_MC_QCDMuEnriched_Pt20to30_Pythia8_FlatPU30to80.py
crab-dev submit -c crabconfig_MC_QCDMuEnriched_Pt30to50_Pythia8_FlatPU30to80.py
crab-dev submit -c crabconfig_MC_QCDMuEnriched_Pt50to80_Pythia8_FlatPU30to80.py
crab-dev submit -c crabconfig_MC_QCDMuEnriched_Pt80to120_Pythia8_FlatPU30to80.py
crab-dev submit -c crabconfig_MC_TT_Powheg_FlatPU30to80.py
crab-dev submit -c crabconfig_MC_WJetsToLNu_aMCNLO_FlatPU30to80.py
crab-dev submit -c crabconfig_MC_ZToMuMu_M120to200_Powheg_FlatPU30to80.py
crab-dev submit -c crabconfig_MC_ZToMuMu_M200to400_Powheg_FlatPU30to80.py
crab-dev submit -c crabconfig_MC_ZToMuMu_M50to120_Powheg_FlatPU30to80.py

