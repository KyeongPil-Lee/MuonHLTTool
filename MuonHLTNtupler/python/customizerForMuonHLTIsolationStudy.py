# -- customizer for muon HLT isolation study
# -- ignore ECAL and HCAL isolation filters: to make all L3 muons have its isolation values for ECAL, HCAL and tracker
# ---- if not, L3 muons rejected at ECAL isolation filter will not have HCAL and tracker isolation value. Similarly, No tracker isolation is available for the L3 muons rejected at the HCAL isolation filter.
# -- usage
# from MuonHLTTool.MuonHLTNtupler.customizerForMuonHLTIsolationStudy import *
# process = customizeForMuonHLTIsolation_ignoreIsoFilter_IsoMu24(process)

import FWCore.ParameterSet.Config as cms

def customizeForMuonHLTIsolation_ignoreIsoFilter_IsoMu24(process):
    # if hasattr(process, "HLTMu24IsolationSequence"):
    #     process.HLTMu24IsolationSequence = cms.Sequence( 
    #         process.HLTL3muonEcalPFisorecoSequenceNoBoolsForMuons + 
    #         cms.ignore(process.hltL3crIsoL1sSingleMu22L1f0L2f10QL3f24QL3pfecalIsoRhoFilteredEB0p14EE0p10) + # -- ignored
    #         process.HLTL3muonHcalPFisorecoSequenceNoBoolsForMuons + 
    #         cms.ignore(process.hltL3crIsoL1sSingleMu22L1f0L2f10QL3f24QL3pfhcalIsoRhoFilteredHB0p16HE0p20) + # -- ignored
    #         process.HLTTrackReconstructionForIsoL3MuonIter02 + 
    #         process.hltMuonTkRelIsolationCut0p07Map 
    #     )

    if hasattr(process, "HLTMu24IsolationSequence"):
        process.HLTMu24IsolationSequence = cms.Sequence( 
          process.HLTL3muonEcalPFisorecoSequenceNoBoolsForMuons + 
          cms.ignore(process.hltL3crIsoL1sSingleMu22L1f0L2f10QL3f24QL3pfecalIsoRhoFilteredEB0p14EE0p10) + 
          process.HLTL3muonHcalPFisorecoSequenceNoBoolsForMuons + 
          cms.ignore(process.hltL3crIsoL1sSingleMu22L1f0L2f10QL3f24QL3pfhcalIsoRhoFilteredHB0p16HE0p20) + 
          process.HLTTrackReconstructionForIsoL3MuonIter02 + 
          process.hltMuonTkRelIsolationCut0p08Map )



    return process