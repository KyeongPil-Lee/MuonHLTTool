#include <Iso_with_MTD/Include/MuonHandle.h>
#include <Iso_with_MTD/Include/GeneralTrackHandle.h>

#include <TTreeReader.h>
#include <TH1D.h>
#include <TChain.h>
#include <TFile.h>
#include <Math/VectorUtil.h>

#include <iostream>

using namespace MuonHLT;

// TString GetFilePath(TString sampleType, TString splitNum) {
//   TString basePath;
//   if( sampleType == "DY" )
//     basePath = "/user/kplee/SE/MuonHLTTree_phase2MTD_v1/DYToLL_M-50_TuneCP5_14TeV-pythia8/crab_DYLL_M50_Pythia8/221015_125544/0000";
//   else if( sampleType == "TT" )
//     basePath = "/user/kplee/SE/MuonHLTTree_phase2MTD_v1/TTToSemileptonic_TuneCP5_14TeV-powheg-pythia8/crab_TTSemiLeptonic_Powheg/221016_102409/0000";

//   return TString::Format("%s/ntuple_%s.root", basePath.Data(), splitNum.Data());
// }

TString GetFilePath(TString sampleType, TString splitNum) {
  TString basePath;
  if( sampleType == "DY" )
    basePath = "/dcache_mnt/dcache/pnfs/iihe/cms/store/user/kplee/MuonHLTTree_phase2MTD_v2/DYToLL_M-50_TuneCP5_14TeV-pythia8/crab_DYLL_M50_Pythia8/221017_134428/0000";
  else if( sampleType == "TT" )
    basePath = "/dcache_mnt/dcache/pnfs/iihe/cms/store/user/kplee/MuonHLTTree_phase2MTD_v2/TTToSemileptonic_TuneCP5_14TeV-powheg-pythia8/crab_TTSemiLeptonic_Powheg/221017_134431/0000";

  return TString::Format("%s/ntuple_%s.root", basePath.Data(), splitNum.Data());
}

Int_t Find_MatchedGeneralTrackIndex(Muon mu, vector<MuonHLT::GeneralTrack> vec_GT);

void MakeHist_Isolation(TString sampleType, TString splitNum) {
  cout << "sampleType = " << sampleType << endl;
  cout << "splitNum =   " << splitNum << endl;

  // -- isolation setting
  Double_t dRCut_inner = 0.01;
  Double_t dRCut_outer = 0.3;

  // -- histogram
  TH1D* h_nMuon_timeInfo = new TH1D("h_nMuon_timeInfo", "", 2, 0, 2);
  h_nMuon_timeInfo->GetXaxis()->SetBinLabel(1, "timeInfo");
  h_nMuon_timeInfo->GetXaxis()->SetBinLabel(2, "no timeInfo");
  TH1D* h_relTrkIso_prompt    = new TH1D("h_relTrkIso_prompt",    "", 1000, 0, 10);
  TH1D* h_relTrkIso_nonprompt = new TH1D("h_relTrkIso_nonprompt", "", 1000, 0, 10);

  TH1D* h_dz_track_vs_muon_prompt    = new TH1D("h_dz_track_vs_muon_prompt",    "", 1000, 0, 10);
  TH1D* h_dz_track_vs_muon_nonprompt = new TH1D("h_dz_track_vs_muon_nonprompt", "", 1000, 0, 10);

  TH1D* h_dt_track_vs_muon_prompt    = new TH1D("h_dt_track_vs_muon_prompt",    "", 5000, 0, 50);
  TH1D* h_dt_track_vs_muon_nonprompt = new TH1D("h_dt_track_vs_muon_nonprompt", "", 5000, 0, 50);

  TH1D* h_dtSig_track_vs_muon_prompt    = new TH1D("h_dtSig_track_vs_muon_prompt",    "", 1000, 0, 10);
  TH1D* h_dtSig_track_vs_muon_nonprompt = new TH1D("h_dtSig_track_vs_muon_nonprompt", "", 1000, 0, 10);

  TH1D* h_relTrkIso_dz0p2_prompt    = new TH1D("h_relTrkIso_dz0p2_prompt",    "", 1000, 0, 1);
  TH1D* h_relTrkIso_dz0p2_nonprompt = new TH1D("h_relTrkIso_dz0p2_nonprompt", "", 1000, 0, 1);

  TH1D* h_dt_dz0p2_track_vs_muon_prompt    = new TH1D("h_dt_dz0p2_track_vs_muon_prompt",    "", 5000, 0, 50);
  TH1D* h_dt_dz0p2_track_vs_muon_nonprompt = new TH1D("h_dt_dz0p2_track_vs_muon_nonprompt", "", 5000, 0, 50);

  TH1D* h_dtSig_dz0p2_track_vs_muon_prompt    = new TH1D("h_dtSig_dz0p2_track_vs_muon_prompt",    "", 1000, 0, 10);
  TH1D* h_dtSig_dz0p2_track_vs_muon_nonprompt = new TH1D("h_dtSig_dz0p2_track_vs_muon_nonprompt", "", 1000, 0, 10);

  TH1D* h_nMuon_passMVA_prompt = new TH1D("h_nMuon_passMVA_prompt", "", 2, 0, 2);
  h_nMuon_passMVA_prompt->GetXaxis()->SetBinLabel(1, "pass MVA");
  h_nMuon_passMVA_prompt->GetXaxis()->SetBinLabel(2, "fail to pass MVA");

  TH1D* h_nMuon_passMVA_nonprompt = new TH1D("h_nMuon_passMVA_nonprompt", "", 2, 0, 2);
  h_nMuon_passMVA_nonprompt->GetXaxis()->SetBinLabel(1, "pass MVA");
  h_nMuon_passMVA_nonprompt->GetXaxis()->SetBinLabel(2, "fail to pass MVA");

  TH1D* h_nTrack_dz0p2_passMVA_prompt = new TH1D("h_nTrack_dz0p2_passMVA_prompt", "", 2, 0, 2);
  h_nTrack_dz0p2_passMVA_prompt->GetXaxis()->SetBinLabel(1, "pass MVA");
  h_nTrack_dz0p2_passMVA_prompt->GetXaxis()->SetBinLabel(2, "fail to pass MVA");

  TH1D* h_nTrack_dz0p2_passMVA_nonprompt = new TH1D("h_nTrack_dz0p2_passMVA_nonprompt", "", 2, 0, 2);
  h_nTrack_dz0p2_passMVA_nonprompt->GetXaxis()->SetBinLabel(1, "pass MVA");
  h_nTrack_dz0p2_passMVA_nonprompt->GetXaxis()->SetBinLabel(2, "fail to pass MVA");



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
      if( !mu.isLoose ) continue; // -- use the muon with time information (inner track)
      Bool_t isPrompt = (mu.simType == 4);

      // -- find the matched track and get the information
      Int_t i_matchedTrack = Find_MatchedGeneralTrackIndex(mu, vec_GT);
      Double_t mu_inner_time = 1e10;
      Double_t mu_inner_timeError = 1e10;
      Double_t mu_inner_timeQualMVA = -1e10; // -- negative direction
      Double_t mu_inner_dz = 1e10;
      if( i_matchedTrack < 0 ) h_nMuon_timeInfo->Fill("no timeInfo", 1);
      else {
        h_nMuon_timeInfo->Fill("timeInfo", 1);
        mu_inner_time        = vec_GT[i_matchedTrack].time;
        mu_inner_timeError   = vec_GT[i_matchedTrack].timeError;
        mu_inner_timeQualMVA = vec_GT[i_matchedTrack].timeQualMVA;
        mu_inner_dz = vec_GT[i_matchedTrack].dz;
      }

      if( mu_inner_timeQualMVA > 0.5 ) {
        if( isPrompt ) h_nMuon_passMVA_prompt->Fill("pass MVA", 1);
        else           h_nMuon_passMVA_nonprompt->Fill("pass MVA", 1);
      }
      else {
        if( isPrompt ) h_nMuon_passMVA_prompt->Fill("fail to pass MVA", 1);
        else           h_nMuon_passMVA_nonprompt->Fill("fail to pass MVA", 1);
      }
              
      // -- calc. track pT sum (iso.)
      Double_t ptSum = 0;
      Double_t ptSum_dz0p2 = 0;

      Int_t nTrack = (Int_t)vec_GT.size();
      for(Int_t i=0; i<nTrack; i++) {

        Double_t dR = ROOT::Math::VectorUtil::DeltaR(mu.vecP, vec_GT[i].vecP);

        if( dRCut_inner < dR && dR < dRCut_outer ) {
          ptSum = ptSum + vec_GT[i].pt;

          Double_t dz_track_vs_muon = fabs(mu_inner_dz - vec_GT[i].dz);
          Double_t dt_track_vs_muon = fabs(mu_inner_time - vec_GT[i].time);
          Double_t dtSig_track_vs_muon = dt_track_vs_muon / sqrt(mu_inner_timeError*mu_inner_timeError + vec_GT[i].timeError*vec_GT[i].timeError);

          if( isPrompt ) {
            h_dz_track_vs_muon_prompt->Fill( dz_track_vs_muon );
            h_dt_track_vs_muon_prompt->Fill( dt_track_vs_muon );
            h_dtSig_track_vs_muon_prompt->Fill( dtSig_track_vs_muon );
          }
          else {
            h_dz_track_vs_muon_nonprompt->Fill( dz_track_vs_muon );
            h_dt_track_vs_muon_nonprompt->Fill( dt_track_vs_muon );
            h_dtSig_track_vs_muon_nonprompt->Fill( dtSig_track_vs_muon );
          }

          if( dz_track_vs_muon < 0.2 ) {
            ptSum_dz0p2 = ptSum_dz0p2 + vec_GT[i].pt;

            if( isPrompt ) {
              h_dt_dz0p2_track_vs_muon_prompt->Fill( dt_track_vs_muon );
              h_dtSig_dz0p2_track_vs_muon_prompt->Fill( dtSig_track_vs_muon );
            }
            else {
              h_dt_dz0p2_track_vs_muon_nonprompt->Fill( dt_track_vs_muon );
              h_dtSig_dz0p2_track_vs_muon_nonprompt->Fill( dtSig_track_vs_muon );
            }

            if( vec_GT[i].timeQualMVA > 0.5 ) {
              if( isPrompt ) h_nTrack_dz0p2_passMVA_prompt->Fill( "pass MVA", 1 );
              else           h_nTrack_dz0p2_passMVA_nonprompt->Fill( "pass MVA", 1 );
            }
            else {
              if( isPrompt ) h_nTrack_dz0p2_passMVA_prompt->Fill( "fail to pass MVA", 1 );
              else           h_nTrack_dz0p2_passMVA_nonprompt->Fill( "fail to pass MVA", 1 );
            }

          } // -- end of if( dz < 0.2 )

        } // -- end of if ( dR < dRCut )

      } // -- end of the loop over the tracks
      
      Double_t relTrkIso = ptSum / mu.pt;
      if( isPrompt ) h_relTrkIso_prompt->Fill(relTrkIso);
      else           h_relTrkIso_nonprompt->Fill(relTrkIso);

      Double_t relTrkIso_dz0p2 = ptSum_dz0p2 / mu.pt;
      if( isPrompt ) h_relTrkIso_dz0p2_prompt->Fill(relTrkIso_dz0p2);
      else           h_relTrkIso_dz0p2_nonprompt->Fill(relTrkIso_dz0p2);
    } // -- end of the loop over the muons


  } // -- end of event iteration

  TString outputFName = TString::Format("ROOTFile_%s_%s.root", sampleType.Data(), splitNum.Data());
  TFile *f_output = TFile::Open(outputFName, "RECREATE");
  f_output->cd();

  h_nMuon_timeInfo->Write();
  h_relTrkIso_prompt->Write();
  h_relTrkIso_nonprompt->Write();
  h_dz_track_vs_muon_prompt->Write();
  h_dz_track_vs_muon_nonprompt->Write();
  h_dt_track_vs_muon_prompt->Write();
  h_dt_track_vs_muon_nonprompt->Write();
  h_dtSig_track_vs_muon_prompt->Write();
  h_dtSig_track_vs_muon_nonprompt->Write();

  h_relTrkIso_dz0p2_prompt->Write();
  h_relTrkIso_dz0p2_nonprompt->Write();
  h_dt_dz0p2_track_vs_muon_prompt->Write();
  h_dt_dz0p2_track_vs_muon_nonprompt->Write();
  h_dtSig_dz0p2_track_vs_muon_prompt->Write();
  h_dtSig_dz0p2_track_vs_muon_nonprompt->Write();

  h_nMuon_passMVA_prompt->Write();
  h_nMuon_passMVA_nonprompt->Write();
  h_nTrack_dz0p2_passMVA_prompt->Write();
  h_nTrack_dz0p2_passMVA_nonprompt->Write();

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