#include <Iso_with_MTD/Include/MuonHandle.h>
#include <Iso_with_MTD/Include/GeneralTrackHandle.h>

#include <TTreeReader.h>
#include <TH1D.h>
#include <TChain.h>
#include <TFile.h>
#include <Math/VectorUtil.h>

#include <iostream>

using namespace MuonHLT;

Int_t Find_MatchedGeneralTrackIndex(Muon mu, vector<MuonHLT::GeneralTrack>& vec_GT) {

  // -- find the track with the smallest dR (at least less than 0.01)
  Int_t theIndex = -1;
  Double_t dR_smallest = 1e10;

  Int_t nTrack = (Int_t)vec_GT.size();
  for(Int_t i=0; i<nTrack; i++) {

    ROOT::Math::PtEtaPhiMVector vecP_innerTrack(mu.inner_pt, mu.inner_eta, mu.inner_phi, MuonHLT::M_mu);

    Double_t dR = ROOT::Math::VectorUtil::DeltaR(vecP_innerTrack, vec_GT[i].vecP);

    if( dR < 0.01 && dR < dR_smallest ) {
      dR_smallest = dR;
      theIndex = i;
    }
  }

  return theIndex;
}


class IsoHistProducer_1DScan {
public:

  ///////////////////////
  // -- inner class -- //
  class IsoHist {
  public:
    IsoHist() {}

    IsoHist(TString type, TString scanVarName, Double_t varCut) { 
      type_ = type;
      scanVarName_ = scanVarName;
      varCut_ = varCut;
      Init();
    }

    void Fill(Double_t relTrkIso, Bool_t isPrompt) {

      if( isPrompt ) h_prompt_->Fill( relTrkIso );
      else           h_nonprompt_->Fill( relTrkIso );

    }

    void Save() {
      h_prompt_->Write();
      h_nonprompt_->Write();
    }

  private:
    TString type_;
    TString scanVarName_;
    Double_t varCut_;
    TH1D* h_prompt_;
    TH1D* h_nonprompt_;

    void Init() {
      TString histName_base = TString::Format("h_relTrkIso_%s_%s%.2lf", type_.Data(), scanVarName_.Data(), varCut_);
      histName_base.ReplaceAll(".", "p");

      h_prompt_    = new TH1D(histName_base+"_prompt",    "", 1000, 0, 1);
      h_nonprompt_ = new TH1D(histName_base+"_nonprompt", "", 1000, 0, 1);
    }

  }; 
  // -- end of the class IsoHist -- //
  ////////////////////////////////////

  IsoHistProducer_1DScan(TString type, TString scanVarName, Int_t nBin, Double_t varMin, Double_t varMax) {
    type_ = type;
    scanVarName_ = scanVarName;
    nBin_ = nBin;
    varMin_ = varMin;
    varMax_ = varMax;

    Init();
  }

  void Set_dRCut(Double_t min, Double_t max) {
    dRCut_inner_ = min;
    dRCut_outer_ = max;
  }

  void Set_DZCut(Double_t dzCut) {
    applyDzCut_ = kTRUE;
    dzCut_ = dzCut;
  }

  void Set_TimeMVACheck(Bool_t flag=kTRUE) { timeMVACheck_ = flag; }

  void Fill(Muon& mu, vector<GeneralTrack>& vec_GT) {

    if( timeMVACheck_ ) {
      Int_t i_matchedTrack = Find_MatchedGeneralTrackIndex(mu, vec_GT);
      if( vec_GT[i_matchedTrack].timeQualMVA < timeMVACut_ ) // -- if the muon fails to pass MVA: do not include
        return;
    }

    // calc. relTrkIso for each scan
    vector<Double_t> vec_relTrkIso = Calc_Iso(mu, vec_GT);

    Bool_t isPrompt = (mu.simType == 4);

    for(Int_t i=0; i<nBin_; i++)
      vec_isoHist_[i].Fill( vec_relTrkIso[i], isPrompt);
  }

  void Save() {
    for(auto isoHist : vec_isoHist_ )
      isoHist.Save();
  }

private:

  TString type_;
  TString scanVarName_;
  Int_t nBin_;
  Double_t varMin_;
  Double_t varMax_;
  vector<Double_t> vec_varCut_; // -- cut value for scan
  vector<IsoHist> vec_isoHist_; // -- hist. for each scan

  Double_t dRCut_inner_ = 0.01;
  Double_t dRCut_outer_ = 0.3;
  Bool_t applyDzCut_ = kFALSE;
  Double_t dzCut_ = -1;

  Bool_t timeMVACheck_ = kFALSE;
  Double_t timeMVACut_ = 0.5;

  void Init() {
    // -- e.g. nBin=10, varMin=0, varMax=10 ---> step = 1 (as expected)
    Double_t step = (varMax_ - varMin_ ) / (Double_t)nBin_;

    for(Int_t i=0; i<nBin_; i++) {

      Double_t theVarCut = varMin_ + (i+1)*step;
      vec_varCut_.push_back( theVarCut );
      vec_isoHist_.push_back( IsoHist(type_, scanVarName_, theVarCut) );
    }

    TString dRCutInfo = TString::Format("[IsoHistProducer_1DScan::Init] dR condition: %.3lf < dR < %.3lf", dRCut_inner_, dRCut_outer_);
    cout << dRCutInfo << endl;
  }

  vector<Double_t> Calc_Iso(Muon& mu, vector<GeneralTrack>& vec_GT) {
    vector<Double_t> vec_relTrkIso;

    // -- put 0 in vec_relTrkIso_ first
    for(Int_t i=0; i<nBin_; i++)
      vec_relTrkIso.push_back(0.0);

    Int_t i_matchedTrack = Find_MatchedGeneralTrackIndex(mu, vec_GT);
    if( i_matchedTrack < 0 ) {
      cout << "[Calc_Iso] No matched track for given muon is found!" << endl;
      return vec_relTrkIso;
    }

    // -- loop over general tracks
    for(auto& track : vec_GT ) {
      
      // -- apply DZ cut: skip if it has large dz(muon, track)
      if( applyDzCut_ ) {
        Double_t dz = std::abs(vec_GT[i_matchedTrack].dz - track.dz);
        if( dz > dzCut_ )
          continue;
      }

      Double_t dR = ROOT::Math::VectorUtil::DeltaR(vec_GT[i_matchedTrack].vecP, track.vecP);
      if( !(dRCut_inner_ < dR && dR < dRCut_outer_) )
        continue;

      Double_t pt_track = track.pt;
      Double_t theScanVar = ScanVariable(vec_GT[i_matchedTrack], track);

      if( timeMVACheck_ && track.timeQualMVA > timeMVACut_ ) // -- if it fails to pass MVA cut:
        theScanVar = 0; // -- set it to the smallest value: it will always be added in the isolation

      // -- loop over each scan point & calc. pt sum for each case
      for(Int_t i=0; i<nBin_; i++) {
        Double_t theVarCut = vec_varCut_[i];
        if( theScanVar < theVarCut )
          vec_relTrkIso[i] = vec_relTrkIso[i] + pt_track;
      } // -- end of loop over scan points
    } // -- end of loop over tracks

    // -- change to relative values
    // cout << "mu.pt = " << mu.pt << endl;
    for(Int_t i=0; i<nBin_; i++) {
      vec_relTrkIso[i] = vec_relTrkIso[i] / mu.pt;
      // cout << "vec_relTrkIso[i] = " << vec_relTrkIso[i] << endl;
    }
    // cout << endl;

    return vec_relTrkIso;
  }

  Double_t ScanVariable(GeneralTrack& muTrack, GeneralTrack& theTrack) {
    Double_t theScanVar = -1;

    if( scanVarName_ == "dt" ) {
      theScanVar = std::abs(muTrack.time - theTrack.time);
    }
    else if( scanVarName_ == "dtSig" ) {
      theScanVar = std::abs(muTrack.time - theTrack.time) / sqrt(muTrack.timeError*muTrack.timeError + theTrack.timeError*theTrack.timeError);
    }
    else if( scanVarName_ == "dz" ) {
      theScanVar = std::abs(muTrack.dz - theTrack.dz);
    }

    return theScanVar;
  }

};


TString GetFilePath(TString sampleType, TString splitNum) {
  TString basePath;
  if( sampleType == "DY" )
    basePath = "/dcache_mnt/dcache/pnfs/iihe/cms/store/user/kplee/MuonHLTTree_phase2MTD_v2/DYToLL_M-50_TuneCP5_14TeV-pythia8/crab_DYLL_M50_Pythia8/221017_134428/0000";
  else if( sampleType == "TT" )
    basePath = "/dcache_mnt/dcache/pnfs/iihe/cms/store/user/kplee/MuonHLTTree_phase2MTD_v2/TTToSemileptonic_TuneCP5_14TeV-powheg-pythia8/crab_TTSemiLeptonic_Powheg/221017_134431/0000";

  return TString::Format("%s/ntuple_%s.root", basePath.Data(), splitNum.Data());
}

void MakeHist_Isolation_scan(TString sampleType, TString splitNum) {
  cout << "sampleType = " << sampleType << endl;
  cout << "splitNum =   " << splitNum << endl;

  // -- isolation setting
  Double_t dRCut_inner = 0.01;
  Double_t dRCut_outer = 0.3;

  // -- histogram setting
  IsoHistProducer_1DScan* producer_dt = new IsoHistProducer_1DScan("dz0p2", "dt", 100, 0, 10);
  producer_dt->Set_DZCut(0.2);

  // -- run two producers independently is not time-efficient, but let's do for now as the total run time is not so long
  IsoHistProducer_1DScan* producer_dtSig = new IsoHistProducer_1DScan("dz0p2", "dtSig", 100, 0, 5);
  producer_dtSig->Set_DZCut(0.2);

  // -- MVA check
  IsoHistProducer_1DScan* producer_dt_MVACheck = new IsoHistProducer_1DScan("dz0p2_MVACheck", "dt", 100, 0, 10);
  producer_dt_MVACheck->Set_DZCut(0.2);
  producer_dt_MVACheck->Set_TimeMVACheck();

  IsoHistProducer_1DScan* producer_dtSig_MVACheck = new IsoHistProducer_1DScan("dz0p2_MVACheck", "dtSig", 100, 0, 5);
  producer_dtSig_MVACheck->Set_DZCut(0.2);
  producer_dtSig_MVACheck->Set_TimeMVACheck();


  TChain* chain = new TChain("ntupler/ntuple");
  if( sampleType == "test" )   chain->Add("Example/ntuple_example.root");
  else                         chain->Add( GetFilePath(sampleType, splitNum) );

  TTreeReader* reader = new TTreeReader(chain);

  MuonHandle* muonHandle       = new MuonHandle(*reader);
  GeneralTrackHandle* GTHandle = new GeneralTrackHandle(*reader);

  Int_t nEvent_tot = reader->GetEntries(kTRUE);
  cout << "nEvent_tot = " << nEvent_tot << endl;

  // nEvent_tot = 10;
  for(Int_t i_ev=0; i_ev<nEvent_tot; i_ev++) {
    reader->SetEntry(i_ev);

    vector<MuonHLT::Muon> vec_muon = GetAll_Muon(muonHandle);
    vector<MuonHLT::GeneralTrack> vec_GT = GetAll_GeneralTrack(GTHandle);

    for(auto mu : vec_muon ) {

      // -- to ensure the muon has the inner track (and corresponding time information)
      if( !mu.isLoose ) continue;

      producer_dt->Fill( mu, vec_GT );
      producer_dtSig->Fill( mu, vec_GT );

      producer_dt_MVACheck->Fill( mu, vec_GT );
      producer_dtSig_MVACheck->Fill( mu, vec_GT );

    } // -- end of the loop over the muons

  } // -- end of event iteration

  TString outputFName = TString::Format("ROOTFile_IsoScan_%s_%s.root", sampleType.Data(), splitNum.Data());
  TFile *f_output = TFile::Open(outputFName, "RECREATE");
  f_output->cd();
  producer_dt->Save();
  producer_dtSig->Save();
  producer_dt_MVACheck->Save();
  producer_dtSig_MVACheck->Save();
  f_output->Close();
}