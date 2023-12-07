#pragma once

#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>
#include <Math/Vector4D.h>

namespace MuonHLT {

class GeneralTrackHandle {
public:
  TTreeReaderValue<Int_t>    nGeneralTrack;
  TTreeReaderArray<Double_t> generalTrack_pt;
  TTreeReaderArray<Double_t> generalTrack_eta;
  TTreeReaderArray<Double_t> generalTrack_phi;
  TTreeReaderArray<Double_t> generalTrack_dxy;
  TTreeReaderArray<Double_t> generalTrack_dz;
  TTreeReaderArray<Double_t> generalTrack_time;
  TTreeReaderArray<Double_t> generalTrack_timeError;
  TTreeReaderArray<Double_t> generalTrack_timeQualMVA;

  GeneralTrackHandle(TTreeReader& reader):
  nGeneralTrack(reader, "nGeneralTrack"),
  generalTrack_pt(reader, "generalTrack_pt"),
  generalTrack_eta(reader, "generalTrack_eta"),
  generalTrack_phi(reader, "generalTrack_phi"),
  generalTrack_dxy(reader, "generalTrack_dxy"),
  generalTrack_dz(reader, "generalTrack_dz"),
  generalTrack_time(reader, "generalTrack_time"),
  generalTrack_timeError(reader, "generalTrack_timeError"),
  generalTrack_timeQualMVA(reader, "generalTrack_timeQualMVA")
  {}

  Int_t nObject() {return *nGeneralTrack; };

}; // -- end of the class GeneralTrackHandle

class GeneralTrack {
public:
  Double_t pt;
  Double_t eta;
  Double_t phi;
  // -- ROOT::Math::PtEtaPhiMVector: replacement for TLorentzVector
  // -- reference: https://root.cern.ch/doc/master/group__GenVector.html
  // ------------- https://root.cern.ch/doc/master/classROOT_1_1Math_1_1LorentzVector.html
  ROOT::Math::PtEtaPhiMVector vecP; 

  Double_t dxy;
  Double_t dz;
  Double_t time;
  Double_t timeError;
  Double_t timeQualMVA;

  GeneralTrack() = delete;

  GeneralTrack(const GeneralTrackHandle& handle, const Int_t index) {
    FillVariable(handle, index);
  }

  void FillVariable(const GeneralTrackHandle& handle, const Int_t index) {
    pt = handle.generalTrack_pt[index];
    eta = handle.generalTrack_eta[index];
    phi = handle.generalTrack_phi[index];

    vecP = ROOT::Math::PtEtaPhiMVector(pt, eta, phi, 0); // -- mass is set to 0 for general track

    dxy = handle.generalTrack_dxy[index];
    dz = handle.generalTrack_dz[index];
    time = handle.generalTrack_time[index];
    timeError = handle.generalTrack_timeError[index];
    timeQualMVA = handle.generalTrack_timeQualMVA[index];
  }

}; // -- end of the class GeneralTrack

vector<MuonHLT::GeneralTrack> GetAll_GeneralTrack(GeneralTrackHandle* handle) {
  vector<MuonHLT::GeneralTrack> vec_object;
  for(Int_t i_obj=0; i_obj<handle->nObject(); i_obj++) {
    vec_object.push_back( MuonHLT::GeneralTrack(*handle, i_obj) );
  }

  return vec_object;
}

}; // -- end of the namespace MuonHLT

