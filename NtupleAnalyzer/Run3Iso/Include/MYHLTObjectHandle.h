#pragma once

#include <vector>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
// #include <TTreeReaderArray.h>
// #include <Math/Vector4D.h>

#ifndef M_MU
#define M_MU 0.1056583715  // -- GeV -- //
#endif

namespace MuonHLT {

class MYHLTObjectHandle {
public:
  // TTreeReaderValue< vector<std::string> >  vec_filterName;
  // TTreeReaderValue< vector<double> >  vec_HLTObj_pt;
  // TTreeReaderValue< vector<double> >  vec_HLTObj_eta;
  // TTreeReaderValue< vector<double> >  vec_HLTObj_phi;

  // MYHLTObjectHandle(TTreeReader& reader):
  // vec_filterName(reader, "vec_myFilterName"),
  // vec_HLTObj_pt(reader, "vec_myHLTObj_pt"),
  // vec_HLTObj_eta(reader, "vec_myHLTObj_eta"),
  // vec_HLTObj_phi(reader, "vec_myHLTObj_phi")
  // {}

  // -- for vector branch, it works only when they are pointer, not object - not sure why...
  TTreeReaderValue< vector<std::string> >* vec_filterName;
  TTreeReaderValue< vector<double> >* vec_HLTObj_pt;
  TTreeReaderValue< vector<double> >* vec_HLTObj_eta;
  TTreeReaderValue< vector<double> >* vec_HLTObj_phi;


  MYHLTObjectHandle(TTreeReader& reader):
  vec_filterName( new TTreeReaderValue< vector<std::string> >(reader, "vec_myFilterName") ),
  vec_HLTObj_pt( new TTreeReaderValue< vector<double> >(reader, "vec_myHLTObj_pt") ),
  vec_HLTObj_eta( new TTreeReaderValue< vector<double> >(reader, "vec_myHLTObj_eta") ),
  vec_HLTObj_phi( new TTreeReaderValue< vector<double> >(reader, "vec_myHLTObj_phi") )
  {}

  Int_t nObject() { return (Int_t)vec_HLTObj_pt->Get()->size(); }

}; // -- end of the class MYHLTObjectHandle

class MYHLTObject {
public:
  Double_t pt;
  Double_t eta;
  Double_t phi;
  // -- ROOT::Math::PtEtaPhiMVector: replacement for TLorentzVector
  // -- reference: https://root.cern.ch/doc/master/group__GenVector.html
  // ------------- https://root.cern.ch/doc/master/classROOT_1_1Math_1_1LorentzVector.html
  ROOT::Math::PtEtaPhiMVector vecP;

  TString filterName;

  MYHLTObject() = delete;

  MYHLTObject(const MYHLTObjectHandle& handle, const Int_t index) {
    FillVariable(handle, index);
  }

  void FillVariable(const MYHLTObjectHandle& handle, const Int_t index) {
    pt = handle.vec_HLTObj_pt->Get()->at(index);
    eta = handle.vec_HLTObj_eta->Get()->at(index);
    phi = handle.vec_HLTObj_phi->Get()->at(index);

    vecP = ROOT::Math::PtEtaPhiMVector(pt, eta, phi, M_MU);

    filterName = handle.vec_filterName->Get()->at(index);
  }

}; // -- end of the class MYHLTObject

vector<MuonHLT::MYHLTObject> GetAll_MYHLTObject(MYHLTObjectHandle* handle, TString theFilterName = "") {
  // cout << "GetAll_MYHLTObject" << endl;

  vector<MuonHLT::MYHLTObject> vec_object;
  Int_t nObject = handle->nObject();
  // cout <<"--> # objects = " << nObject << endl;

  for(Int_t i_obj=0; i_obj<nObject; ++i_obj) {
    MuonHLT::MYHLTObject obj(*handle, i_obj);

    if( theFilterName == "" ||
       (theFilterName != "" && obj.filterName == theFilterName) )
      vec_object.push_back( obj );
  }

  // cout << "GetAll_MYHLTObject: finished" << endl;
  return vec_object;
}

}; // -- end of the namespace MuonHLT

