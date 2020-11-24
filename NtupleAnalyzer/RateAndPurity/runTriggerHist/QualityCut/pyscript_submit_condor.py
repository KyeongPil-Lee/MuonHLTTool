import os
cwd = os.getcwd()

from RateAndPurity.CondorJobGenerator import *

theBaseJobDir = "/data9/Users/kplee/MUO19001Jobs"
theRootMacro = cwd+"/MuonTriggerHist.cxx" # -- macro name: should be sync. with the shell script to run
theShellScript = cwd+"/script_runMacro_forCondor.sh"
theVersion = "v209"

# list_dataset = ["Run2016H", "Run2018D"]
list_trigger_2016 = [
    'hltL1fL1sMu22L1Filtered0',
    # 'hltL2fL1sMu22L1f0L2Filtered10Q',
    # 'hltL3fL1sMu22L1f0L2f10QL3Filtered24Q', # -- Mu24, 2016
    # 'hltL3crIsoL1sMu22L1f0L2f10QL3f24QL3trkIsoFiltered0p09', # -- IsoMu24, 2016
    # 'hltL3fL1sMu22f0TkFiltered24Q', # -- TkMu24, 2016
    # 'hltL3fL1sMu22L1f0Tkf24QL3trkIsoFiltered0p09', # -- IsoTkMu24, 2016
]

list_trigger_2018 = [
    'hltL1fL1sMu22L1Filtered0',
    # 'hltL2fL1sSingleMu22L1f0L2Filtered10Q', # -- different L2 filter name with 2016
    # 'hltL3fL1sSingleMu22L1f0L2f10QL3Filtered24Q', # -- Mu24, 2018
    # 'hltL3crIsoL1sSingleMu22L1f0L2f10QL3f24QL3trkIsoFiltered0p07', # -- IsoMu24, 2018
]

dic_dataset_trigger = {
    "HLTPhysicsRun2016Hv2": list_trigger_2016, # -- now it also contains 2016Hv3 data
    "HLTPhysicsRun2018D":   list_trigger_2018,
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
        # generator.nJob = 40 # -- nJob = 20: does not run! why...?
        generator.nJob = 80 # -- nJob = 40: Run2016D runs only on small fraction of jobs... why?

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

