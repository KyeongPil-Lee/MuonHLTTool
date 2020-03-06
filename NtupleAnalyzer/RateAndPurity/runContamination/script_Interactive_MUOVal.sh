#!/bin/bash

root -l -b -q 'MuonTriggerPurity.cxx+("hltL1fL1sMu22L1Filtered0", "TTSemiLep102XMUOVal", "v101", 0.0, 5.0, "Job0",{"/pnfs/knu.ac.kr/data/cms/store/user/jskim/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/crab_MuonHLTNtuple_SimMatchingInfo_2018_TTToSemiLeptonic__PUAvg50ForMUOVal/200227_162718/0000/ntuple_*.root"})' \
>&MuonTriggerPurity_hltL1fL1sMu22L1Filtered0.log&

root -l -b -q 'MuonTriggerPurity.cxx+("hltL2fL1sSingleMu22L1f0L2Filtered10Q", "TTSemiLep102XMUOVal", "v101", 0.0, 5.0, "Job0",{"/pnfs/knu.ac.kr/data/cms/store/user/jskim/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/crab_MuonHLTNtuple_SimMatchingInfo_2018_TTToSemiLeptonic__PUAvg50ForMUOVal/200227_162718/0000/ntuple_*.root"})' \
>&MuonTriggerPurity_hltL2fL1sSingleMu22L1f0L2Filtered10Q.log&

root -l -b -q 'MuonTriggerPurity.cxx+("HLT_Mu24_v", "TTSemiLep102XMUOVal", "v101", 0.0, 5.0, "Job0",{"/pnfs/knu.ac.kr/data/cms/store/user/jskim/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/crab_MuonHLTNtuple_SimMatchingInfo_2018_TTToSemiLeptonic__PUAvg50ForMUOVal/200227_162718/0000/ntuple_*.root"})' \
>&MuonTriggerPurity_HLT_Mu24_v.log&

root -l -b -q 'MuonTriggerPurity.cxx+("HLT_IsoMu24_v", "TTSemiLep102XMUOVal", "v101", 0.0, 5.0, "Job0",{"/pnfs/knu.ac.kr/data/cms/store/user/jskim/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/crab_MuonHLTNtuple_SimMatchingInfo_2018_TTToSemiLeptonic__PUAvg50ForMUOVal/200227_162718/0000/ntuple_*.root"})' \
>&MuonTriggerPurity_HLT_IsoMu24_v.log&

root -l -b -q 'MuonTriggerPurity.cxx+("HLT_Mu50_v", "TTSemiLep102XMUOVal", "v101", 0.0, 5.0, "Job0",{"/pnfs/knu.ac.kr/data/cms/store/user/jskim/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/crab_MuonHLTNtuple_SimMatchingInfo_2018_TTToSemiLeptonic__PUAvg50ForMUOVal/200227_162718/0000/ntuple_*.root"})' \
>&MuonTriggerPurity_HLT_Mu50_v.log&