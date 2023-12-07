#pragma once

#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>
// #include <Math/Vector4D.h>

#ifndef M_MU
#define M_MU 0.1056583715 // -- GeV -- //
#endif

namespace MuonHLT {

class L3MuonHandle {
public:
  TTreeReaderValue<Int_t>    nL3Muon;
  TTreeReaderArray<Double_t> L3Muon_pt;
  TTreeReaderArray<Double_t> L3Muon_eta;
  TTreeReaderArray<Double_t> L3Muon_phi;
  TTreeReaderArray<Double_t> L3Muon_trkPt;
  TTreeReaderArray<Double_t> L3Muon_ECALIso;
  TTreeReaderArray<Double_t> L3Muon_HCALIso;
  TTreeReaderArray<Double_t> L3Muon_trkIso;


  L3MuonHandle(TTreeReader& reader):
  nL3Muon(reader, "nL3Muon"),
  L3Muon_pt(reader, "L3Muon_pt"),
  L3Muon_eta(reader, "L3Muon_eta"),
  L3Muon_phi(reader, "L3Muon_phi"),
  L3Muon_trkPt(reader, "L3Muon_trkPt"),
  L3Muon_ECALIso(reader, "L3Muon_ECALIso"),
  L3Muon_HCALIso(reader, "L3Muon_HCALIso"),
  L3Muon_trkIso(reader, "L3Muon_trkIso")
  {}

  Int_t nObject() {return *nL3Muon; };

}; // -- end of the class L3MuonHandle

class L3Muon {
public:
  Double_t pt;
  Double_t eta;
  Double_t phi;
  // -- ROOT::Math::PtEtaPhiMVector: replacement for TLorentzVector
  // -- reference: https://root.cern.ch/doc/master/group__GenVector.html
  // ------------- https://root.cern.ch/doc/master/classROOT_1_1Math_1_1LorentzVector.html
  ROOT::Math::PtEtaPhiMVector vecP; 

  Double_t trkPt;
  Double_t ECALIso;
  Double_t HCALIso;
  Double_t trkIso;

  Double_t relECALIso;
  Double_t relHCALIso;
  Double_t relTrkIso;

  L3Muon() = delete;

  L3Muon(const L3MuonHandle& handle, const Int_t index) {
    FillVariable(handle, index);
  }

  void FillVariable(const L3MuonHandle& handle, const Int_t index) {
    pt = handle.L3Muon_pt[index];
    eta = handle.L3Muon_eta[index];
    phi = handle.L3Muon_phi[index];

    vecP = ROOT::Math::PtEtaPhiMVector(pt, eta, phi, M_MU);

    trkPt = handle.L3Muon_trkPt[index];
    ECALIso = handle.L3Muon_ECALIso[index];
    HCALIso = handle.L3Muon_HCALIso[index];
    trkIso = handle.L3Muon_trkIso[index];

    relECALIso = ECALIso / pt;
    relHCALIso = HCALIso / pt;
    relTrkIso = trkIso / pt;
  }

}; // -- end of the class L3Muon

vector<MuonHLT::L3Muon> GetAll_L3Muon(L3MuonHandle* handle) {
  vector<MuonHLT::L3Muon> vec_object;
  for(Int_t i_obj=0; i_obj<handle->nObject(); ++i_obj) {
    vec_object.push_back( MuonHLT::L3Muon(*handle, i_obj) );
  }

  return vec_object;
}

}; // -- end of the namespace MuonHLT

