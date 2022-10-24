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

  void Set_DZCut(Double_t dzCut) { dzCut_ = dzCut; }

  // -- for the tracks without time information
  void Set_DZCut_noTrackTime(Double_t dzCut) { dzCut_noTrackTime_ = dzCut; }

  // -- for the case when the muon has no time information
  void Set_DZCut_noMuonTime(Double_t dzCut) { dzCut_noMuonTime_ = dzCut; }

  void Set_TimeMVACut(Double_t cut) { timeMVACut_ = cut; }

  void Set_IsoType(TString type) { isoType_ = type; }

  void Set_DefaultCut_dt( Double_t value ) {
    applyDefaultCut_dt = kTRUE;
    defaultCut_dt = value;
  }

  void Fill(Muon& mu, vector<GeneralTrack>& vec_GT) {

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

  Double_t dzCut_ = -1;
  Double_t dzCut_noTrackTime_ = -1;
  Double_t dzCut_noMuonTime_ = -1;

  Double_t timeMVACut_ = 0.5;

  Bool_t applyDefaultCut_dt = kFALSE;
  Double_t defaultCut_dt = -1.0;

  // -- Simplest: dz < max_dz & dt < max_dt / if dt is not availble: just put in the isolation cut
  // -- SimpleCut: dz < max_dz & dt < max_dt / if dt is not available: dz < max_dz_noDt ( apply the other dz cut )
  // -- LinearCut: dz/max_dz + dt/max_dt < 2 / if dt is not available: dz < max_dz_noDt ( apply another dz cut )
  TString isoType_ = "SimpleCut";

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
    if( isoType_ == "Simplest" )       return Calc_Iso_Simplest(mu, vec_GT);
    else if( isoType_ == "SimpleCut" ) return Calc_Iso_SimpleCut(mu, vec_GT);
    else if( isoType_ == "Default" )   return Calc_Iso_Default(mu, vec_GT);
    else if( isoType_ == "LinearCut" ) return Calc_Iso_LinearCut(mu, vec_GT);
    else
      cout << "[Calc_Iso] IsoType = " << isoType_ << " is not recognized... return empty vector" << endl;

    return {};
  }

  // -- dt scan on top of the give dzCut & dzCut_noDt
  // -- 1) if dt available:        dz < dzCut & dt < dtCut (scan over dt)
  // -- 2) if dt is not available: dz < dzCut_noDt (no scan on dt)
  vector<Double_t> Calc_Iso_SimpleCut(Muon& mu, vector<GeneralTrack>& vec_GT) {
    vector<Double_t> vec_relTrkIso;

    // -- put 0 in vec_relTrkIso_ first
    for(Int_t i=0; i<nBin_; i++)
      vec_relTrkIso.push_back(0.0);

    Int_t i_matchedTrack = Find_MatchedGeneralTrackIndex(mu, vec_GT);
    Double_t MVA_muTrack = vec_GT[i_matchedTrack].timeQualMVA;
    Bool_t muonHasTimeInfo = MVA_muTrack < timeMVACut_;

    if( muonHasTimeInfo ) {

      // -- loop over general tracks
      for(auto& track : vec_GT ) {

        Double_t dR = ROOT::Math::VectorUtil::DeltaR(vec_GT[i_matchedTrack].vecP, track.vecP);
        if( !(dRCut_inner_ < dR && dR < dRCut_outer_) )
          continue;

        Bool_t trackHasTimeInfo = track.timeQualMVA < timeMVACut_;

        Double_t dz = std::abs(vec_GT[i_matchedTrack].dz - track.dz);
        if( trackHasTimeInfo ) {
          // -- baseline cut on dt (if necessary)
          if( applyDefaultCut_dt ) {
            Double_t dt = std::abs(vec_GT[i_matchedTrack].time - track.time);
            if( dt > defaultCut_dt ) continue;
          }

          if( dz > dzCut_ ) continue;

          Double_t pt_track = track.pt;
          Double_t theScanVar = ScanVariable(vec_GT[i_matchedTrack], track); // -- e.g. dt or dtSig, etc

          // -- loop over each scan point & calc. pt sum for each case
          for(Int_t i=0; i<nBin_; i++) {
            Double_t theVarCut = vec_varCut_[i];
            if( theScanVar < theVarCut )
              vec_relTrkIso[i] = vec_relTrkIso[i] + pt_track;
          } // -- end of loop over scan points

        }
        else { // -- track has no reliable time info: only rely on dz

          if( dz > dzCut_noTrackTime_ ) continue;

          Double_t pt_track = track.pt;

          // -- no scan on the time variable: just put the same values for all scan points
          for(Int_t i=0; i<nBin_; i++) {
            vec_relTrkIso[i] = vec_relTrkIso[i] + pt_track;
          } // -- end of loop over scan points

        } // -- end of else( trackHasTimeInfo )

      } // -- end of track iteration
    }
    else { // -- if muon has no reliable time info: only rely on dz cut

      // -- loop over general tracks
      for(auto& track : vec_GT ) {

        Double_t dR = ROOT::Math::VectorUtil::DeltaR(vec_GT[i_matchedTrack].vecP, track.vecP);
        if( !(dRCut_inner_ < dR && dR < dRCut_outer_) )
          continue;

        // -- apply DZ cut: skip if it has large dz(muon, track)
        Double_t dz = std::abs(vec_GT[i_matchedTrack].dz - track.dz);
        if( dz > dzCut_noMuonTime_ ) continue;

        Double_t pt_track = track.pt;

        // -- no scan on the time variable: just put the same values for all scan points
        for(Int_t i=0; i<nBin_; i++) {
          vec_relTrkIso[i] = vec_relTrkIso[i] + pt_track;
        } // -- end of loop over scan points

      }

    } // -- end of else( muonHasTimeInfo )

    // -- change to relative values
    for(Int_t i=0; i<nBin_; i++) {
      vec_relTrkIso[i] = vec_relTrkIso[i] / mu.pt;
    }

    return vec_relTrkIso;
  }

  // -- when dt is available: apply (dz/dz_cut + dt/dt_cut <= 2)
  // -- when dt is not available: same with SimpleCut case
  vector<Double_t> Calc_Iso_LinearCut(Muon& mu, vector<GeneralTrack>& vec_GT) {
    vector<Double_t> vec_relTrkIso;

    // -- put 0 in vec_relTrkIso_ first
    for(Int_t i=0; i<nBin_; i++)
      vec_relTrkIso.push_back(0.0);

    Int_t i_matchedTrack = Find_MatchedGeneralTrackIndex(mu, vec_GT);
    Double_t MVA_muTrack = vec_GT[i_matchedTrack].timeQualMVA;
    Bool_t muonHasTimeInfo = MVA_muTrack < timeMVACut_;

    if( muonHasTimeInfo ) {

      // -- loop over general tracks
      for(auto& track : vec_GT ) {

        Double_t dR = ROOT::Math::VectorUtil::DeltaR(vec_GT[i_matchedTrack].vecP, track.vecP);
        if( !(dRCut_inner_ < dR && dR < dRCut_outer_) )
          continue;

        Bool_t trackHasTimeInfo = track.timeQualMVA < timeMVACut_;

        Double_t dz = std::abs(vec_GT[i_matchedTrack].dz - track.dz);
        if( trackHasTimeInfo ) {
          // -- baseline cut on dt (if necessary)
          if( applyDefaultCut_dt ) {
            Double_t dt = std::abs(vec_GT[i_matchedTrack].time - track.time);
            if( dt > defaultCut_dt ) continue;
          }

          if( dz > dzCut_ ) continue;

          Double_t pt_track = track.pt;
          Double_t theScanVar = ScanVariable(vec_GT[i_matchedTrack], track); // -- e.g. dt or dtSig, etc

          // -- loop over each scan point & calc. pt sum for each case
          for(Int_t i=0; i<nBin_; i++) {
            Double_t theVarCut = vec_varCut_[i];

            Double_t combinedValue = dz/dzCut_ + theScanVar/theVarCut;

            if( combinedValue < 2.0 )
              vec_relTrkIso[i] = vec_relTrkIso[i] + pt_track;
          } // -- end of loop over scan points

        }
        else { // -- track has no reliable time info: only rely on dz

          if( dz > dzCut_noTrackTime_ ) continue;

          Double_t pt_track = track.pt;

          // -- no scan on the time variable: just put the same values for all scan points
          for(Int_t i=0; i<nBin_; i++) {
            vec_relTrkIso[i] = vec_relTrkIso[i] + pt_track;
          } // -- end of loop over scan points

        } // -- end of else( trackHasTimeInfo )

      } // -- end of track iteration
    }
    else { // -- if muon has no reliable time info: only rely on dz cut

      // -- loop over general tracks
      for(auto& track : vec_GT ) {

        Double_t dR = ROOT::Math::VectorUtil::DeltaR(vec_GT[i_matchedTrack].vecP, track.vecP);
        if( !(dRCut_inner_ < dR && dR < dRCut_outer_) )
          continue;

        // -- apply DZ cut: skip if it has large dz(muon, track)
        Double_t dz = std::abs(vec_GT[i_matchedTrack].dz - track.dz);
        if( dz > dzCut_noMuonTime_ ) continue;

        Double_t pt_track = track.pt;

        // -- no scan on the time variable: just put the same values for all scan points
        for(Int_t i=0; i<nBin_; i++) {
          vec_relTrkIso[i] = vec_relTrkIso[i] + pt_track;
        } // -- end of loop over scan points

      }

    } // -- end of else( muonHasTimeInfo )

    // -- change to relative values
    for(Int_t i=0; i<nBin_; i++) {
      vec_relTrkIso[i] = vec_relTrkIso[i] / mu.pt;
    }

    return vec_relTrkIso;
  }


  // -- the results shown in 18th Oct. 2022 (https://indico.cern.ch/event/1211853/)
  // -- dt scan on top of a fixed dz cut (0.2)
  // -- if the track has no time info or its time is not reliable -> just add in the isolation (if it is within dz_cut)
  vector<Double_t> Calc_Iso_Simplest(Muon& mu, vector<GeneralTrack>& vec_GT) {
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
      Double_t dz = std::abs(vec_GT[i_matchedTrack].dz - track.dz);
      if( dz > dzCut_ ) continue;

      Double_t dR = ROOT::Math::VectorUtil::DeltaR(vec_GT[i_matchedTrack].vecP, track.vecP);
      if( !(dRCut_inner_ < dR && dR < dRCut_outer_) )
        continue;

      Double_t pt_track = track.pt;
      Double_t theScanVar = ScanVariable(vec_GT[i_matchedTrack], track);

      if( track.timeQualMVA > timeMVACut_ ) // -- if it fails to pass MVA cut:
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

  // -- just return the isolation with the default algorithm: no scan at all
  vector<Double_t> Calc_Iso_Default(Muon& mu, vector<GeneralTrack>& vec_GT) {
    vector<Double_t> vec_relTrkIso;

    // -- put 0 in vec_relTrkIso_ first
    for(Int_t i=0; i<nBin_; i++)
      vec_relTrkIso.push_back(0.0);

    Int_t i_matchedTrack = Find_MatchedGeneralTrackIndex(mu, vec_GT);

    // -- loop over general tracks
    for(auto& track : vec_GT ) {

      Double_t dR = ROOT::Math::VectorUtil::DeltaR(vec_GT[i_matchedTrack].vecP, track.vecP);
      if( !(dRCut_inner_ < dR && dR < dRCut_outer_) )
        continue;

      // -- apply DZ cut: skip if it has large dz(muon, track)
      Double_t dz = std::abs(vec_GT[i_matchedTrack].dz - track.dz);
      if( dz > dzCut_ ) continue;

      Double_t pt_track = track.pt;

      // -- no scan on the time variable: just put the same values for all scan points
      for(Int_t i=0; i<nBin_; i++) {
        vec_relTrkIso[i] = vec_relTrkIso[i] + pt_track;
      } // -- end of loop over scan points

    } // -- end of track iteration

    // -- change to relative values
    for(Int_t i=0; i<nBin_; i++) {
      vec_relTrkIso[i] = vec_relTrkIso[i] / mu.pt;
    }

    return vec_relTrkIso;

  }

};