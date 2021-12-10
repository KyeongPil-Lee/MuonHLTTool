#pragma once

#include <Include/MuonHLTTool.h>
#include <Run3Winter21/HistProducer/TnPTool.h>
#include <Run3Winter21/HistProducer/WPTune_Regional/Utility_NewWP.h>

namespace TnPTool {

// -- Mu24 final filter: hltL3fL1sSingleMu22L1f0L2f10QL3Filtered24Q
// -- IsoMu24 final filter: hltL3crIsoL1sSingleMu22L1f0L2f10QL3f24QL3trkIsoFiltered0p07 -- //
// -- IsoMu27 final filter: hltL3crIsoL1sMu22Or25L1f0L2f10QL3f27QL3trkIsoFiltered0p07 -- //
// -- Mu50 final filter: hltL3fL1sMu22Or25L1f0L2f10QL3Filtered50Q -- //


Bool_t CommonTagCondition( MuonHLT::Muon tagCand, MuonHLT::NtupleHandle* ntuple ) {
  Bool_t flag = kFALSE;

  if( MuonHLT::dRMatching_HLTObj(tagCand.vecP, ntuple, "hltL3crIsoL1sSingleMu22L1f0L2f10QL3f24QL3trkIsoFiltered0p07", 0.1) &&
      tagCand.pt > 26 &&
      fabs(tagCand.eta) < 2.4 &&
      tagCand.isTight && 
      tagCand.relPFIso_dBeta < 0.15 )
    flag = kTRUE;

  return flag;
}

Bool_t TightIDIso_MYHLT_Mu24Obj(MuonHLT::Muon probeCand, MuonHLT::NtupleHandle* ntuple) {
  Bool_t flag = kFALSE;

  if( probeCand.isTight && 
      probeCand.relPFIso_dBeta < 0.15 && 
      MuonHLT::dRMatching_MYHLTObj(probeCand.vecP, ntuple, "hltL3fL1sSingleMu22L1f0L2f10QL3Filtered24Q::MYHLT", 0.1) )
    flag = kTRUE;

  return flag;
}

// -- combined PFIso / L3 (old working point)
class TnPPair_FullIsoOverMu24_OldWP: public MuonHLT::TnPPairBase {
  public:
    TnPPair_FullIsoOverMu24_OldWP( MuonHLT::Muon tagCand, MuonHLT::Muon probeCand, MuonHLT::NtupleHandle* ntuple ): 
    TnPPairBase( tagCand, probeCand, ntuple )
    { }

    TnPPair_FullIsoOverMu24_OldWP(): 
    TnPPairBase()
    { }

    // -- user-defined tag condition -- //
    Bool_t IsTag(MuonHLT::Muon tagCand, MuonHLT::NtupleHandle* ntuple) {
      return TnPTool::CommonTagCondition(tagCand, ntuple);
    }

    Bool_t IsProbe(MuonHLT::Muon probeCand, MuonHLT::NtupleHandle* ntuple) {
      return TnPTool::TightIDIso_MYHLT_Mu24Obj(probeCand, ntuple);
    }

    Bool_t IsPassingProbe(MuonHLT::Muon probeCand, MuonHLT::NtupleHandle* ntuple)
    {
      vector<MuonHLT::MYHLTObject> vec_Mu24Obj_passIso = MuonHLT::GetAllMYHLTObj_Mu24_PassIso_OldWP(ntuple);

      vector<TLorentzVector> vec_vecP_Mu24Obj;
      for(auto& Mu24Obj : vec_Mu24Obj_passIso ) {
        vec_vecP_Mu24Obj.push_back( Mu24Obj.vecP );
      }

      return MuonHLT::dRMatching( probeCand.vecP, vec_vecP_Mu24Obj, 0.1 );
    }
};

// -- combined PFIso / L3 (new working point)
class TnPPair_FullIsoOverMu24_NewWP: public MuonHLT::TnPPairBase {
  public:
    TnPPair_FullIsoOverMu24_NewWP( MuonHLT::Muon tagCand, MuonHLT::Muon probeCand, MuonHLT::NtupleHandle* ntuple ): 
    TnPPairBase( tagCand, probeCand, ntuple )
    { }

    TnPPair_FullIsoOverMu24_NewWP(): 
    TnPPairBase()
    { }

    // -- user-defined tag condition -- //
    Bool_t IsTag(MuonHLT::Muon tagCand, MuonHLT::NtupleHandle* ntuple) {
      return CommonTagCondition(tagCand, ntuple);
    }

    Bool_t IsProbe(MuonHLT::Muon probeCand, MuonHLT::NtupleHandle* ntuple) {
      return TnPTool::TightIDIso_MYHLT_Mu24Obj(probeCand, ntuple);
    }

    Bool_t IsPassingProbe(MuonHLT::Muon probeCand, MuonHLT::NtupleHandle* ntuple)
    {
      vector<MuonHLT::MYHLTObject> vec_Mu24Obj_passIso = MuonHLT::GetAllMYHLTObj_Mu24_PassIso_NewWP(ntuple);

      vector<TLorentzVector> vec_vecP_Mu24Obj;
      for(auto& Mu24Obj : vec_Mu24Obj_passIso ) {
        vec_vecP_Mu24Obj.push_back( Mu24Obj.vecP );
      }

      return MuonHLT::dRMatching( probeCand.vecP, vec_vecP_Mu24Obj, 0.1 );
    }
};



};
