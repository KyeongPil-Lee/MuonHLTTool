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

// -- default IsoMu24 efficiency over L3 (in HLT, not MYHLT)
class TnPPair_IsoMu24OverL3: public MuonHLT::TnPPairBase {
  public:
    TnPPair_IsoMu24OverL3( MuonHLT::Muon tagCand, MuonHLT::Muon probeCand, MuonHLT::NtupleHandle* ntuple ): 
    TnPPairBase( tagCand, probeCand, ntuple )
    { }

    TnPPair_IsoMu24OverL3(): 
    TnPPairBase()
    { }

    // -- user-defined tag condition -- //
    Bool_t IsTag(MuonHLT::Muon tagCand, MuonHLT::NtupleHandle* ntuple) {
      // cout << "[TnPPair_IsoMu24OverL3::IsTag]" << endl;      
      return TnPTool::CommonTagCondition(tagCand, ntuple);
    }

    Bool_t IsProbe(MuonHLT::Muon probeCand, MuonHLT::NtupleHandle* ntuple) {
      // cout << TString::Format("[TnPPair_IsoMu24OverL3::IsProbe]\nProbe (pt, eta, phi, isTight, relPFIso_dBeta) = (%.3lf, %.1lf, %.1lf, %d, %.3lf)", probeCand.vecP.Pt(), probeCand.vecP.Eta(), probeCand.vecP.Phi(), probeCand.isTight, probeCand.relPFIso_dBeta) << endl;
      Bool_t flag = kFALSE;

      if( probeCand.isTight && 
          probeCand.relPFIso_dBeta < 0.15 && 
          MuonHLT::dRMatching_HLTObj(probeCand.vecP, ntuple, "hltL3fL1sSingleMu22L1f0L2f10QL3Filtered24Q", 0.1) )
        flag = kTRUE;

      // cout << "  --> IsProbe = " << flag << endl;
      return flag;
    }

    Bool_t IsPassingProbe(MuonHLT::Muon probeCand, MuonHLT::NtupleHandle* ntuple)
    {
      return MuonHLT::dRMatching_HLTObj(probeCand.vecP, ntuple, "hltL3crIsoL1sSingleMu22L1f0L2f10QL3f24QL3trkIsoFiltered0p07", 0.1);
      
      // Bool_t flag = kFALSE;

      // cout << TString::Format("[TnPPair_IsoMu24OverL3::IsPassingProbe]\nProbe (pt, eta, phi) = (%.3lf, %.3lf, %.3lf)", probeCand.vecP.Pt(), probeCand.vecP.Eta(), probeCand.vecP.Phi()) << endl;

      // vector<MuonHLT::HLTObject> vec_HLTObj = MuonHLT::GetAllHLTObject(ntuple, "hltL3crIsoL1sSingleMu22L1f0L2f10QL3f24QL3trkIsoFiltered0p07");
      // vector<TLorentzVector> vec_vecP_HLTObj;
      // for(const auto& HLTObj : vec_HLTObj )
      //   vec_vecP_HLTObj.push_back( HLTObj.vecP );

      // Double_t minDR = 0.1;
      // for(const auto& vecP_target: vec_vecP_HLTObj ) {
      //   Double_t dR = probeCand.vecP.DeltaR( vecP_target );
      //   cout << TString::Format("  IsoMu24 object (pt, eta, phi) = (%.1lf, %.3lf, %.3lf) --> dR = %.3lf", vecP_target.Pt(), vecP_target.Eta(), vecP_target.Phi(), dR) << endl;
      //   if( dR < minDR ) {
      //     cout << "    ---> Pass" << endl;
      //     flag = kTRUE;
      //     break;
      //   }
      // }

      // vector<MuonHLT::HLTObject> vec_Mu24Obj = MuonHLT::GetAllHLTObject(ntuple, "hltL3fL1sSingleMu22L1f0L2f10QL3Filtered24Q");
      // vector<MuonHLT::HLTObject> vec_IsoMu24Obj = MuonHLT::GetAllHLTObject(ntuple, "hltL3crIsoL1sSingleMu22L1f0L2f10QL3f24QL3trkIsoFiltered0p07");

      // if( vec_Mu24Obj.size() != vec_IsoMu24Obj.size() ) {
      //   cout << "process=HLT, Mu24 obejcts" << endl;

      //   for(auto& obj : vec_Mu24Obj ) {
      //     cout << TString::Format("  (pt, eta, phi) = (%.1lf, %.3lf, %.3lf)", obj.pt, obj.eta, obj.phi) << endl;
      //   }

      //   cout << "process=HLT, IsoMu24 obejcts" << endl;

      //   for(auto& obj : vec_IsoMu24Obj ) {
      //     cout << TString::Format("  (pt, eta, phi) = (%.1lf, %.3lf, %.3lf)", obj.pt, obj.eta, obj.phi) << endl;
      //   }

      //   cout << endl;
      // } // -- if # Mu24 obj != # IsoMu24 obj

      // return flag;
    }
};

};
