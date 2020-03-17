import os
cwd = os.getcwd()

from RateAndPurity.CondorJobGenerator import *

theBaseJobDir = "/data9/Users/kplee/MUO19001Jobs"
theRootMacro = cwd+"/MuonTriggerPurity.cxx" # -- macro name: should be sync. with the shell script to run
theShellScript = cwd+"/script_runMacro_forCondor.sh"
theVersion = "v201"


# list_dataset = ["Run2016H", "Run2018D"]
list_trigger_2016 = [
    'HLT_Mu24_v',
]

list_trigger_2018 = [
    'HLT_Mu24_v',
]

dic_dataset_trigger = {
    "Run2016H": list_trigger_2016,
    "Run2018D": list_trigger_2018,
}

for theDataset in dic_dataset_trigger.keys():
    list_trigger = dic_dataset_trigger[theDataset]

    print "dataset = ", theDataset
    print "list_trigger = ", list_trigger

    for theTrigger in list_trigger:

        generator = JobGenerator()
        generator.rootMacro = theRootMacro
        generator.shellScript = theShellScript
        generator.baseJobDir = theBaseJobDir
        generator.trigger = theTrigger
        generator.dataset = theDataset
        generator.version = theVersion
        generator.nJob = 20

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

