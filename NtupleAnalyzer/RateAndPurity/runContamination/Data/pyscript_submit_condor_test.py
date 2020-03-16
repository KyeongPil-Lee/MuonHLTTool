import os
from RateAndPurity.CondorJobGenerator import *

baseJobDir = "/data9/Users/kplee/MUO19001Jobs"

cwd = os.getcwd()

generator = JobGenerator()
generator.rootMacro = cwd+"/MuonTriggerPurity.cxx" # -- macro name: should be sync. with the shell script to run
generator.shellScript = cwd+"/script_runMacro_forCondor.sh"
generator.baseJobDir = baseJobDir
generator.trigger = "HLT_IsoMu24_v"
generator.dataset = "Run2016H"
generator.version = "v200"
generator.nJob = 50

generator.Generate()


# -- list of triggers
# list_trigger = [
#   'HLT_IsoMu24_v',
#   'HLT_Mu50_v',
#   'hltL1fL1sMu22L1Filtered0',
#   'hltL2fL1sMu22L1f0L2Filtered10Q'

#   'HLT_IsoTkMu24_v',
#   'HLT_TkMu50_v',

#   'HLT_IsoMu24_v_Or_HLT_IsoTkMu24_v',
#   'HLT_Mu50_v_Or_HLT_TkMu50_v'
# ]

