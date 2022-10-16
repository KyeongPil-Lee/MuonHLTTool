#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>
#include <Math/Vector4D.h>

namespace MuonHLT {

const Double_t M_mu = 0.1056583715;  // -- GeV -- //

class MuonHandle {
public:
  TTreeReaderValue<Int_t> nMuon;
  TTreeReaderArray<Double_t> muon_pt;
  TTreeReaderArray<Double_t> muon_eta;
  TTreeReaderArray<Double_t> muon_phi;
  TTreeReaderArray<Double_t> muon_charge;
  TTreeReaderArray<Double_t> muon_px;
  TTreeReaderArray<Double_t> muon_py;
  TTreeReaderArray<Double_t> muon_pz;
  TTreeReaderArray<Double_t> muon_dB;

  TTreeReaderArray<Int_t> muon_isGLB;
  TTreeReaderArray<Int_t> muon_isSTA;
  TTreeReaderArray<Int_t> muon_isTRK;
  TTreeReaderArray<Int_t> muon_isPF;
  TTreeReaderArray<Int_t> muon_isTight;
  TTreeReaderArray<Int_t> muon_isMedium;
  TTreeReaderArray<Int_t> muon_isLoose;
  TTreeReaderArray<Int_t> muon_isHighPt;
  TTreeReaderArray<Int_t> muon_isHighPtNew;
  TTreeReaderArray<Int_t> muon_isSoft;

  TTreeReaderArray<Double_t> muon_iso03_sumPt;
  TTreeReaderArray<Double_t> muon_iso03_hadEt;
  TTreeReaderArray<Double_t> muon_iso03_emEt;

  TTreeReaderArray<Double_t> muon_PFIso03_charged;
  TTreeReaderArray<Double_t> muon_PFIso03_neutral;
  TTreeReaderArray<Double_t> muon_PFIso03_photon;
  TTreeReaderArray<Double_t> muon_PFIso03_sumPU;

  TTreeReaderArray<Double_t> muon_PFIso04_charged;
  TTreeReaderArray<Double_t> muon_PFIso04_neutral;
  TTreeReaderArray<Double_t> muon_PFIso04_photon;
  TTreeReaderArray<Double_t> muon_PFIso04_sumPU;

  TTreeReaderArray<Double_t> muon_PFCluster03_ECAL;
  TTreeReaderArray<Double_t> muon_PFCluster03_HCAL;

  TTreeReaderArray<Double_t> muon_PFCluster04_ECAL;
  TTreeReaderArray<Double_t> muon_PFCluster04_HCAL;

  TTreeReaderArray<Double_t> muon_normChi2_global;
  TTreeReaderArray<Int_t> muon_nTrackerHit_global;
  TTreeReaderArray<Int_t> muon_nTrackerLayer_global;
  TTreeReaderArray<Int_t> muon_nPixelHit_global;
  TTreeReaderArray<Int_t> muon_nMuonHit_global;

  TTreeReaderArray<Double_t> muon_inner_pt;
  TTreeReaderArray<Double_t> muon_inner_eta;
  TTreeReaderArray<Double_t> muon_inner_phi;
  TTreeReaderArray<Double_t> muon_inner_time;
  TTreeReaderArray<Double_t> muon_normChi2_inner;
  TTreeReaderArray<Int_t> muon_nTrackerHit_inner;
  TTreeReaderArray<Int_t> muon_nTrackerLayer_inner;
  TTreeReaderArray<Int_t> muon_nPixelHit_inner;

  TTreeReaderArray<Double_t> muon_pt_tuneP;
  TTreeReaderArray<Double_t> muon_ptError_tuneP;

  TTreeReaderArray<Double_t> muon_dxyVTX_best;
  TTreeReaderArray<Double_t> muon_dzVTX_best;

  TTreeReaderArray<Int_t> muon_nMatchedStation;
  TTreeReaderArray<Int_t> muon_nMatchedRPCLayer;
  TTreeReaderArray<Int_t> muon_stationMask;

  TTreeReaderArray<Double_t> muon_propEta;
  TTreeReaderArray<Double_t> muon_propPhi;

  // -- sim-hit
  TTreeReaderArray<Int_t> muon_simType;
  TTreeReaderArray<Int_t> muon_simExtType;
  TTreeReaderArray<Int_t> muon_simPdgId;
  TTreeReaderArray<Int_t> muon_simFlavour;
  TTreeReaderArray<Double_t> muon_simPt;
  TTreeReaderArray<Double_t> muon_simEta;
  TTreeReaderArray<Double_t> muon_simPhi;
  TTreeReaderArray<Int_t> muon_simBX;
  TTreeReaderArray<Double_t> muon_simProdZ;
  TTreeReaderArray<Double_t> muon_simProdRho;
  TTreeReaderArray<Int_t> muon_simMotherPdgId;
  TTreeReaderArray<Int_t> muon_simHeaviestMotherFlavour;

  MuonHandle(TTreeReader& reader):
  nMuon(reader, "nMuon"),
  muon_pt(reader, "muon_pt"),
  muon_eta(reader, "muon_eta"),
  muon_phi(reader, "muon_phi"),
  muon_charge(reader, "muon_charge"),
  muon_px(reader, "muon_px"),
  muon_py(reader, "muon_py"),
  muon_pz(reader, "muon_pz"),
  muon_dB(reader, "muon_dB"),
  muon_isGLB(reader, "muon_isGLB"),
  muon_isSTA(reader, "muon_isSTA"),
  muon_isTRK(reader, "muon_isTRK"),
  muon_isPF(reader, "muon_isPF"),
  muon_isTight(reader, "muon_isTight"),
  muon_isMedium(reader, "muon_isMedium"),
  muon_isLoose(reader, "muon_isLoose"),
  muon_isHighPt(reader, "muon_isHighPt"),
  muon_isHighPtNew(reader, "muon_isHighPtNew"),
  muon_isSoft(reader, "muon_isSoft"),
  muon_iso03_sumPt(reader, "muon_iso03_sumPt"),
  muon_iso03_hadEt(reader, "muon_iso03_hadEt"),
  muon_iso03_emEt(reader, "muon_iso03_emEt"),
  muon_PFIso03_charged(reader, "muon_PFIso03_charged"),
  muon_PFIso03_neutral(reader, "muon_PFIso03_neutral"),
  muon_PFIso03_photon(reader, "muon_PFIso03_photon"),
  muon_PFIso03_sumPU(reader, "muon_PFIso03_sumPU"),
  muon_PFIso04_charged(reader, "muon_PFIso04_charged"),
  muon_PFIso04_neutral(reader, "muon_PFIso04_neutral"),
  muon_PFIso04_photon(reader, "muon_PFIso04_photon"),
  muon_PFIso04_sumPU(reader, "muon_PFIso04_sumPU"),
  muon_PFCluster03_ECAL(reader, "muon_PFCluster03_ECAL"),
  muon_PFCluster03_HCAL(reader, "muon_PFCluster03_HCAL"),
  muon_PFCluster04_ECAL(reader, "muon_PFCluster04_ECAL"),
  muon_PFCluster04_HCAL(reader, "muon_PFCluster04_HCAL"),
  muon_normChi2_global(reader, "muon_normChi2_global"),
  muon_nTrackerHit_global(reader, "muon_nTrackerHit_global"),
  muon_nTrackerLayer_global(reader, "muon_nTrackerLayer_global"),
  muon_nPixelHit_global(reader, "muon_nPixelHit_global"),
  muon_nMuonHit_global(reader, "muon_nMuonHit_global"),
  muon_inner_pt(reader, "muon_inner_pt"),
  muon_inner_eta(reader, "muon_inner_eta"),
  muon_inner_phi(reader, "muon_inner_phi"),
  muon_inner_time(reader, "muon_inner_time"),
  muon_normChi2_inner(reader, "muon_normChi2_inner"),
  muon_nTrackerHit_inner(reader, "muon_nTrackerHit_inner"),
  muon_nTrackerLayer_inner(reader, "muon_nTrackerLayer_inner"),
  muon_nPixelHit_inner(reader, "muon_nPixelHit_inner"),
  muon_pt_tuneP(reader, "muon_pt_tuneP"),
  muon_ptError_tuneP(reader, "muon_ptError_tuneP"),
  muon_dxyVTX_best(reader, "muon_dxyVTX_best"),
  muon_dzVTX_best(reader, "muon_dzVTX_best"),
  muon_nMatchedStation(reader, "muon_nMatchedStation"),
  muon_nMatchedRPCLayer(reader, "muon_nMatchedRPCLayer"),
  muon_stationMask(reader, "muon_stationMask"),
  muon_propEta(reader, "muon_propEta"),
  muon_propPhi(reader, "muon_propPhi"),
  muon_simType(reader, "muon_simType"),
  muon_simExtType(reader, "muon_simExtType"),
  muon_simPdgId(reader, "muon_simPdgId"),
  muon_simFlavour(reader, "muon_simFlavour"),
  muon_simPt(reader, "muon_simPt"),
  muon_simEta(reader, "muon_simEta"),
  muon_simPhi(reader, "muon_simPhi"),
  muon_simBX(reader, "muon_simBX"),
  muon_simProdZ(reader, "muon_simProdZ"),
  muon_simProdRho(reader, "muon_simProdRho"),
  muon_simMotherPdgId(reader, "muon_simMotherPdgId"),
  muon_simHeaviestMotherFlavour(reader, "muon_simHeaviestMotherFlavour")
  {}

  Int_t nObject() { return *nMuon; }
}; // -- end of the class MuonHandle

class Muon {
public:
  Double_t pt;
  Double_t eta;
  Double_t phi;
  Double_t mass;
  // -- ROOT::Math::PtEtaPhiMVector: replacement for TLorentzVector
  // -- reference: https://root.cern.ch/doc/master/group__GenVector.html
  // ------------- https://root.cern.ch/doc/master/classROOT_1_1Math_1_1LorentzVector.html
  ROOT::Math::PtEtaPhiMVector vecP; 

  Double_t px;
  Double_t py;
  Double_t pz;
  Double_t energy;
  Double_t dB;
  Double_t charge;
  Int_t    isGLB;
  Int_t    isSTA;
  Int_t    isTRK;
  Int_t    isPF;
  Int_t    isTight;
  Int_t    isMedium;
  Int_t    isLoose;
  Int_t    isHighPt;
  Int_t    isSoft;
  Double_t iso03_sumPt;
  Double_t iso03_hadEt;
  Double_t iso03_emEt;
  Double_t PFIso03_charged;
  Double_t PFIso03_neutral;
  Double_t PFIso03_photon;
  Double_t PFIso03_sumPU;
  Double_t PFIso04_charged;
  Double_t PFIso04_neutral;
  Double_t PFIso04_photon;
  Double_t PFIso04_sumPU;
  Double_t PFCluster03_ECAL;
  Double_t PFCluster03_HCAL;
  Double_t PFCluster04_ECAL;
  Double_t PFCluster04_HCAL;
  Double_t normChi2_global;
  Int_t    nTrackerHit_global;
  Int_t    nTrackerLayer_global;
  Int_t    nPixelHit_global;
  Int_t    nMuonHit_global;
  Double_t inner_pt;
  Double_t inner_eta;
  Double_t inner_phi;
  Double_t inner_time;
  Double_t normChi2_inner;
  Int_t    nTrackerHit_inner;
  Int_t    nTrackerLayer_inner;
  Int_t    nPixelHit_inner;
  Double_t pt_tuneP;
  Double_t ptError_tuneP;
  Double_t dxyVTX_best;
  Double_t dzVTX_best;
  Int_t    nMatchedStation;
  Int_t    nMatchedRPCLayer;
  Int_t    stationMask;

  Double_t propEta;
  Double_t propPhi;
  Int_t simType;
  Int_t simExtType;
  Int_t simPdgId;
  Int_t simFlavour;
  Double_t simPt;
  Double_t simEta;
  Double_t simPhi;
  Int_t simBX;
  Double_t simProdZ;
  Double_t simProdRho;
  Int_t simMotherPdgId;
  Int_t simHeaviestMotherFlavour;

  Double_t relPFIso_dBeta;
  Double_t relTrkIso;

  // -- no use without initialization (trash value can be added in member variables)
  Muon() = delete;

  Muon(const MuonHandle& handle, const Int_t index) {
    FillVariable(handle, index);
  }

  void FillVariable(const MuonHandle& handle, const Int_t index) {
    pt     = handle.muon_pt[index];
    eta    = handle.muon_eta[index];
    phi    = handle.muon_phi[index];
    charge = handle.muon_charge[index];

    px  = handle.muon_px[index];
    py  = handle.muon_py[index];
    pz  = handle.muon_pz[index];
    mass = MuonHLT::M_mu;
    // energy = sqrt(px*px + py*py + pz*pz + mass*mass);
    // vecP.SetPxPyPzE(px, py, pz, energy);
    vecP = ROOT::Math::PtEtaPhiMVector(pt, eta, phi, mass);

    isGLB = handle.muon_isGLB[index];
    isSTA = handle.muon_isSTA[index];
    isTRK = handle.muon_isTRK[index];
    isPF  = handle.muon_isPF[index];

    isTight  = handle.muon_isTight[index];
    isMedium = handle.muon_isMedium[index];
    isLoose  = handle.muon_isLoose[index];
    isHighPt = handle.muon_isHighPt[index];
    isSoft   = handle.muon_isSoft[index];

    iso03_sumPt = handle.muon_iso03_sumPt[index];
    iso03_hadEt = handle.muon_iso03_hadEt[index];
    iso03_emEt  = handle.muon_iso03_emEt[index];

    PFIso03_charged = handle.muon_PFIso03_charged[index];
    PFIso03_neutral = handle.muon_PFIso03_neutral[index];
    PFIso03_photon  = handle.muon_PFIso03_photon[index];
    PFIso03_sumPU   = handle.muon_PFIso03_sumPU[index];
    PFIso04_charged = handle.muon_PFIso04_charged[index];
    PFIso04_neutral = handle.muon_PFIso04_neutral[index];
    PFIso04_photon  = handle.muon_PFIso04_photon[index];
    PFIso04_sumPU   = handle.muon_PFIso04_sumPU[index];

    PFCluster03_ECAL = handle.muon_PFCluster03_ECAL[index];
    PFCluster03_HCAL = handle.muon_PFCluster03_HCAL[index];
    PFCluster04_ECAL = handle.muon_PFCluster04_ECAL[index];
    PFCluster04_HCAL = handle.muon_PFCluster04_HCAL[index];

    normChi2_global      = handle.muon_normChi2_global[index];
    nTrackerHit_global   = handle.muon_nTrackerHit_global[index];
    nTrackerLayer_global = handle.muon_nTrackerLayer_global[index];
    nPixelHit_global     = handle.muon_nPixelHit_global[index];
    nMuonHit_global      = handle.muon_nMuonHit_global[index];

    inner_pt   = handle.muon_inner_pt[index];
    inner_eta  = handle.muon_inner_eta[index];
    inner_phi  = handle.muon_inner_phi[index];
    inner_time = handle.muon_inner_time[index];
    normChi2_inner      = handle.muon_normChi2_inner[index];
    nTrackerHit_inner   = handle.muon_nTrackerHit_inner[index];
    nTrackerLayer_inner = handle.muon_nTrackerLayer_inner[index];
    nPixelHit_inner     = handle.muon_nPixelHit_inner[index];

    pt_tuneP      = handle.muon_pt_tuneP[index];
    ptError_tuneP = handle.muon_ptError_tuneP[index];

    dxyVTX_best = handle.muon_dxyVTX_best[index];
    dzVTX_best  = handle.muon_dzVTX_best[index];
    dB          = handle.muon_dB[index];

    nMatchedStation  = handle.muon_nMatchedStation[index];
    nMatchedRPCLayer = handle.muon_nMatchedRPCLayer[index];
    stationMask      = handle.muon_stationMask[index];

    propEta = handle.muon_propEta[index];
    propPhi = handle.muon_propPhi[index];
    simType = handle.muon_simType[index];
    simExtType = handle.muon_simExtType[index];
    simPdgId = handle.muon_simPdgId[index];
    simFlavour = handle.muon_simFlavour[index];
    simPt = handle.muon_simPt[index];
    simEta = handle.muon_simEta[index];
    simPhi = handle.muon_simPhi[index];
    simBX = handle.muon_simBX[index];
    simProdZ = handle.muon_simProdZ[index];
    simProdRho = handle.muon_simProdRho[index];
    simMotherPdgId = handle.muon_simMotherPdgId[index];
    simHeaviestMotherFlavour = handle.muon_simHeaviestMotherFlavour[index];

    relPFIso_dBeta = (PFIso04_charged + max(0., PFIso04_neutral + PFIso04_photon - 0.5*PFIso04_sumPU))/pt;
    relTrkIso = iso03_sumPt / pt;
  }
}; // -- end of the class Muon

// -- function
vector<MuonHLT::Muon> GetAll_Muon(MuonHandle* handle) {
  vector<MuonHLT::Muon> vec_object;
  for(Int_t i_obj=0; i_obj<handle->nObject(); i_obj++) {
    vec_object.push_back( MuonHLT::Muon(*handle, i_obj) );
  }

  return vec_object;
}

};