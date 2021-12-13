#pragma once

#include <Include/MuonHLTTool.h>

namespace MuonHLT {

Double_t WP_new_ECAL_EB = 0.23020;
Double_t WP_new_ECAL_EE = 0.19300;

Double_t WP_new_HCAL_EB = 0.20590;
Double_t WP_new_HCAL_EE = 0.26710;

Double_t WP_new_Tracker = 0.09664;

Bool_t Pass_HLTIsoFilter_ECAL_NewWP(MuonHLT::MYHLTObject HLTObj) {
  return MuonHLT::Pass_HLTIsoFilter_ECAL( HLTObj, WP_new_ECAL_EB, WP_new_ECAL_EE);
}

Bool_t Pass_HLTIsoFilter_HCAL_NewWP(MuonHLT::MYHLTObject HLTObj) {
  return MuonHLT::Pass_HLTIsoFilter_HCAL( HLTObj, WP_new_HCAL_EB, WP_new_HCAL_EE);
}

Bool_t Pass_HLTIsoFilter_Tracker_NewWP(MuonHLT::MYHLTObject HLTObj) {
  return MuonHLT::Pass_HLTIsoFilter_HCAL( HLTObj, WP_new_Tracker);
}

vector<MuonHLT::MYHLTObject> GetAllMYHLTObj_Mu24_PassIso_OldWP(MuonHLT::NtupleHandle* ntuple) {
  vector<MuonHLT::MYHLTObject> vec_selectedObj;

  vector<MuonHLT::MYHLTObject> vec_Mu24Obj = MuonHLT::GetAllMYHLTObject(ntuple, "hltL3fL1sSingleMu22L1f0L2f10QL3Filtered24Q::MYHLT");
  for( auto& Mu24Obj : vec_Mu24Obj ) {
    if( MuonHLT::Pass_HLTIsoFilter_ECAL( Mu24Obj ) &&
        MuonHLT::Pass_HLTIsoFilter_HCAL( Mu24Obj ) &&
        MuonHLT::Pass_HLTIsoFilter_Tracker( Mu24Obj ) )
      vec_selectedObj.push_back( Mu24Obj );
  }

  return vec_selectedObj;
}

vector<MuonHLT::MYHLTObject> GetAllMYHLTObj_Mu24_PassIso_NewWP(MuonHLT::NtupleHandle* ntuple) {
  vector<MuonHLT::MYHLTObject> vec_selectedObj;

  vector<MuonHLT::MYHLTObject> vec_Mu24Obj = MuonHLT::GetAllMYHLTObject(ntuple, "hltL3fL1sSingleMu22L1f0L2f10QL3Filtered24Q::MYHLT");
  for( auto& Mu24Obj : vec_Mu24Obj ) {
    if( MuonHLT::Pass_HLTIsoFilter_ECAL_NewWP( Mu24Obj ) &&
        MuonHLT::Pass_HLTIsoFilter_HCAL_NewWP( Mu24Obj ) &&
        MuonHLT::Pass_HLTIsoFilter_Tracker_NewWP( Mu24Obj ) )
      vec_selectedObj.push_back( Mu24Obj );
  }

  return vec_selectedObj;
}

};