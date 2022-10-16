#include <Iso_with_MTD/Include/MuonHandle.h>
#include <Iso_with_MTD/Include/GeneralTrackHandle.h>

#include <TTreeReader.h>
#include <TH1D.h>
#include <TChain.h>
#include <TFile.h>
#include <Math/VectorUtil.h>

#include <iostream>

using namespace MuonHLT;

TString GetFilePath(TString sampleType, TString splitNum) {
  TString basePath;
  if( sampleType == "DY" )
    basePath = "/user/kplee/SE/MuonHLTTree_phase2MTD_v1/DYToLL_M-50_TuneCP5_14TeV-pythia8/crab_DYLL_M50_Pythia8/221015_125544/0000";
  else if( sampleType == "TT" )
    basePath = "/user/kplee/SE/MuonHLTTree_phase2MTD_v1/TTToSemileptonic_TuneCP5_14TeV-powheg-pythia8/crab_TTSemiLeptonic_Powheg/221016_102409/0000";

  return TString::Format("%s/ntuple_%s.root", basePath.Data(), splitNum.Data());
}

Int_t Find_MatchedGeneralTrackIndex(Muon mu, vector<MuonHLT::GeneralTrack> vec_GT);

void MakeHist_Isolation(TString sampleType, TString splitNum) {
  cout << "sampleType = " << sampleType << endl;
  cout << "splitNum =   " << splitNum << endl;

  // -- isolation setting
  Double_t dRCut_inner = 0.001;
  Double_t dRCut_outer = 0.3;

  // -- histogram
  TH1D* h_relTrkIso_prompt    = new TH1D("h_relTrkIso_prompt",    "", 1000, 10, 0);
  TH1D* h_relTrkIso_nonprompt = new TH1D("h_relTrkIso_nonprompt", "", 1000, 10, 0);

  TH1D* h_dz_track_vs_muon_prompt    = new TH1D("h_dz_track_vs_muon_prompt",    "", 2000, -10, 10);
  TH1D* h_dz_track_vs_muon_nonprompt = new TH1D("h_dz_track_vs_muon_nonprompt", "", 2000, -10, 10);

  TChain* chain = new TChain("ntupler/ntuple");
  if( sampleType == "test" )   chain->Add("Example/ntuple_example.root");
  else                         chain->Add( GetFilePath(sampleType, splitNum) );

  TTreeReader* reader = new TTreeReader(chain);

  MuonHandle* muonHandle       = new MuonHandle(*reader);
  GeneralTrackHandle* GTHandle = new GeneralTrackHandle(*reader);

  Int_t nEvent_tot = reader->GetEntries(kTRUE);
  cout << "nEvent_tot = " << nEvent_tot << endl;

  for(Int_t i_ev=0; i_ev<nEvent_tot; i_ev++) {
    reader->SetEntry(i_ev);

    vector<MuonHLT::Muon> vec_muon = GetAll_Muon(muonHandle);
    vector<MuonHLT::GeneralTrack> vec_GT = GetAll_GeneralTrack(GTHandle);

    for(auto mu : vec_muon ) {
      Bool_t isPrompt = (mu.simType == 4);

      // -- find the matched track and get the information
      Int_t i_matchedTrack = Find_MatchedGeneralTrackIndex(mu, vec_GT);
      Double_t mu_inner_dz = 1e10;
      // if( i_matchedTrack < 0 ) cout << "no matched track found for muon! ... isPrompt = " << isPrompt << endl;
      // else                     mu_inner_dz = vec_GT[i_matchedTrack].dz;
      if( i_matchedTrack >= 0 )
        mu_inner_dz = vec_GT[i_matchedTrack].dz;
      
      // -- calc. track pT sum (iso.)
      Double_t ptSum = 0;

      Int_t nTrack = (Int_t)vec_GT.size();
      for(Int_t i=0; i<nTrack; i++) {

        Double_t dR = ROOT::Math::VectorUtil::DeltaR(mu.vecP, vec_GT[i].vecP);

        if( dRCut_inner < dR && dR < dRCut_outer ) {
          ptSum = ptSum + vec_GT[i].pt;

          Double_t dz_track_vs_muon = fabs(mu_inner_dz - vec_GT[i].dz);
          if( isPrompt ) h_dz_track_vs_muon_prompt->Fill( dz_track_vs_muon );
          else           h_dz_track_vs_muon_nonprompt->Fill( dz_track_vs_muon );
        }
      } // -- end of the loop over the tracks
      
      Double_t relTrkIso = ptSum / mu.pt;

      if( isPrompt ) h_relTrkIso_prompt->Fill(relTrkIso);
      else           h_relTrkIso_nonprompt->Fill(relTrkIso);
    } // -- end of the loop over the muons


  } // -- end of event iteration

  TString outputFName = TString::Format("ROOTFile_%s_%s.root", sampleType.Data(), splitNum.Data());
  TFile *f_output = TFile::Open(outputFName, "RECREATE");
  f_output->cd();

  h_relTrkIso_prompt->Write();
  h_relTrkIso_nonprompt->Write();
  h_dz_track_vs_muon_prompt->Write();
  h_dz_track_vs_muon_nonprompt->Write();

  f_output->Close();
}

Int_t Find_MatchedGeneralTrackIndex(Muon mu, vector<MuonHLT::GeneralTrack> vec_GT) {
  // Double_t time_mu = mu.inner_time;

  // Int_t theIndex = -1;
  // Int_t nTrack = (Int_t)vec_GT.size();
  // for(Int_t i=0; i<nTrack; i++) {
  //   if( time_mu == vec_GT[i].time) {
  //     theIndex = i;
  //     break;
  //   }
  // }

  // -- find the track with the smallest dR (at least less than 0.001)
  Int_t theIndex = -1;
  Double_t dR_smallest = 1e10;

  Int_t nTrack = (Int_t)vec_GT.size();
  for(Int_t i=0; i<nTrack; i++) {

    ROOT::Math::PtEtaPhiMVector vecP_innerTrack(mu.inner_pt, mu.inner_eta, mu.inner_phi, MuonHLT::M_mu);

    Double_t dR = ROOT::Math::VectorUtil::DeltaR(vecP_innerTrack, vec_GT[i].vecP);

    if( dR < 0.001 && dR < dR_smallest ) {
      dR_smallest = dR;
      theIndex = i;
    }
  }

  return theIndex;
}