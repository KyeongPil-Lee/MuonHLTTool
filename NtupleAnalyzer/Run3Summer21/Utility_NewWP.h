#pragma once

#include <Include/MuonHLTTool.h>

namespace MuonHLT {

// Double_t WP_new_ECAL_EB = 0.23020;
// Double_t WP_new_ECAL_EE = 0.19300;

// Double_t WP_new_HCAL_EB = 0.20590;
// Double_t WP_new_HCAL_EE = 0.26710;

// Double_t WP_new_Tracker = 0.09658;

Double_t WP_new_ECAL_EB = 0.14;
Double_t WP_new_ECAL_EE = 0.10;

// Double_t WP_new_HCAL_EB = 0.16;
// Double_t WP_new_HCAL_EE = 0.20;
Double_t WP_new_HCAL_EB = 0.177;
Double_t WP_new_HCAL_EE = 0.240;

// Double_t WP_new_Tracker = 0.09;
Double_t WP_new_Tracker = 0.08;

Bool_t Pass_HLTIsoFilter_ECAL_NewWP(MuonHLT::MYHLTObject HLTObj) {
  return MuonHLT::Pass_HLTIsoFilter_ECAL( HLTObj, WP_new_ECAL_EB, WP_new_ECAL_EE);
}

Bool_t Pass_HLTIsoFilter_HCAL_NewWP(MuonHLT::MYHLTObject HLTObj) {
  return MuonHLT::Pass_HLTIsoFilter_HCAL( HLTObj, WP_new_HCAL_EB, WP_new_HCAL_EE);
}

Bool_t Pass_HLTIsoFilter_Tracker_NewWP(MuonHLT::MYHLTObject HLTObj) {
  return MuonHLT::Pass_HLTIsoFilter_Tracker( HLTObj, WP_new_Tracker);
}

vector<MuonHLT::MYHLTObject> GetAllMYHLTObj_Mu24_PassIso_OldWP(MuonHLT::NtupleHandle* ntuple) {
  vector<MuonHLT::MYHLTObject> vec_selectedObj;

  vector<MuonHLT::MYHLTObject> vec_Mu24Obj = MuonHLT::GetAllMYHLTObject(ntuple, "hltL3fL1sSingleMu22L1f0L2f10QL3Filtered24Q::MYHLT");
  for( auto& Mu24Obj : vec_Mu24Obj ) {
    Mu24Obj.FillIsolationVariable(ntuple);
    // cout << "MuonHLT::Pass_HLTIsoFilter_ECAL( Mu24Obj ) =    " << MuonHLT::Pass_HLTIsoFilter_ECAL( Mu24Obj ) << endl;
    // cout << "MuonHLT::Pass_HLTIsoFilter_HCAL( Mu24Obj ) =    " << MuonHLT::Pass_HLTIsoFilter_HCAL( Mu24Obj ) << endl;
    // cout << "MuonHLT::Pass_HLTIsoFilter_Tracker( Mu24Obj ) = " << MuonHLT::Pass_HLTIsoFilter_Tracker( Mu24Obj ) << endl;
    if( MuonHLT::Pass_HLTIsoFilter_ECAL( Mu24Obj ) &&
        MuonHLT::Pass_HLTIsoFilter_HCAL( Mu24Obj ) &&
        MuonHLT::Pass_HLTIsoFilter_Tracker( Mu24Obj ) )
      vec_selectedObj.push_back( Mu24Obj );
  }
  // cout << "vec_selectedObj.size() = " << vec_selectedObj.size() << endl;

  return vec_selectedObj;
}

vector<MuonHLT::MYHLTObject> GetAllMYHLTObj_Mu24_PassIso_NewWP(MuonHLT::NtupleHandle* ntuple) {
  vector<MuonHLT::MYHLTObject> vec_selectedObj;

  vector<MuonHLT::MYHLTObject> vec_Mu24Obj = MuonHLT::GetAllMYHLTObject(ntuple, "hltL3fL1sSingleMu22L1f0L2f10QL3Filtered24Q::MYHLT");
  for( auto& Mu24Obj : vec_Mu24Obj ) {
    Mu24Obj.FillIsolationVariable(ntuple);
    
    if( MuonHLT::Pass_HLTIsoFilter_ECAL_NewWP( Mu24Obj ) &&
        MuonHLT::Pass_HLTIsoFilter_HCAL_NewWP( Mu24Obj ) &&
        MuonHLT::Pass_HLTIsoFilter_Tracker_NewWP( Mu24Obj ) )
      vec_selectedObj.push_back( Mu24Obj );
  }

  return vec_selectedObj;
}

};