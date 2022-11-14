#include <Iso_with_MTD/Include/CommonTool.h>
#include <Iso_with_MTD/Include/MuonHandle.h>
#include <Iso_with_MTD/Include/GeneralTrackHandle.h>
#include <Iso_with_MTD/Opt_dtdz/IsoHistProducer.h>

#include <TTreeReader.h>
#include <TH1D.h>
#include <TChain.h>
#include <TFile.h>
#include <Math/VectorUtil.h>

#include <iostream>

using namespace MuonHLT;


TString GetFilePath(TString sampleType, TString splitNum);
void Setup_HistSet(MuonHLT::HistSet* histSet);
Double_t RelTrkIso(Double_t pt_mu, Int_t i_matchedTrack, vector<MuonHLT::GeneralTrack>& vec_GT, Double_t dRCut_inner, Double_t dRCut_outer, Double_t dzCut);


void MakeHist_Muon(TString sampleType, TString splitNum) {
  cout << "sampleType = " << sampleType << endl;
  cout << "splitNum =   " << splitNum << endl;

  // -- isolation setting
  Double_t dRCut_inner = 0.01;
  Double_t dRCut_outer = 0.3;
  Double_t dzCut = 0.2; // -- default value
  Double_t min_timeQualMVA = 0.5; // -- TimeQualMVA cut

  // -- histogram setting
  MuonHLT::HistSet* histSet_all    = new HistSet("");
  MuonHLT::HistSet* histSet_prompt = new HistSet("prompt");
  MuonHLT::HistSet* histSet_nonprompt = new HistSet("nonprompt");

  Setup_HistSet(histSet_all);
  Setup_HistSet(histSet_prompt);
  Setup_HistSet(histSet_nonprompt);

  TChain* chain = new TChain("ntupler/ntuple");
  if( sampleType == "test" )   chain->Add("../Example/ntuple_example.root");
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
      if( mu.pt < 8.0 ) continue; // -- exclude too low muons (to have same pt phase space between prompt and nonprompt & realistic range in phase-2 muon HLT)

      // -- to ensure the muon has the inner track (and corresponding time information)
      if( !mu.isLoose ) continue;

      Bool_t isPrompt = (mu.simType == 4);
      Int_t i_matchedTrack = Find_MatchedGeneralTrackIndex(mu, vec_GT);
      Bool_t hasTime = vec_GT[i_matchedTrack].timeQualMVA > min_timeQualMVA;

      Double_t relTrkIso = RelTrkIso(mu.pt, i_matchedTrack, vec_GT, dRCut_inner, dRCut_outer, dzCut);

      if( relTrkIso == 0 ) {
        histSet_all->Fill("frac_Iso0", 1.5); // -- true
        if( isPrompt ) histSet_prompt->Fill("frac_Iso0", 1.5);
        else           histSet_nonprompt->Fill("frac_Iso0", 1.5);

        if( hasTime ) {
          histSet_all->Fill("frac_Iso0_time", 1.5); // -- true
          if( isPrompt ) histSet_prompt->Fill("frac_Iso0_time", 1.5);
          else           histSet_nonprompt->Fill("frac_Iso0_time", 1.5);
        }
        else {
          histSet_all->Fill("frac_Iso0_noTime", 1.5); // -- true
          if( isPrompt ) histSet_prompt->Fill("frac_Iso0_noTime", 1.5);
          else           histSet_nonprompt->Fill("frac_Iso0_noTime", 1.5);
        }

      }
      else { // -- relTrkIso != 0
        histSet_all->Fill("frac_Iso0", 0.5); // -- false
        if( isPrompt ) histSet_prompt->Fill("frac_Iso0", 0.5);
        else           histSet_nonprompt->Fill("frac_Iso0", 0.5);

        if( hasTime ) {
          histSet_all->Fill("frac_Iso0_time", 0.5); // -- true
          if( isPrompt ) histSet_prompt->Fill("frac_Iso0_time", 0.5);
          else           histSet_nonprompt->Fill("frac_Iso0_time", 0.5);
        }
        else {
          histSet_all->Fill("frac_Iso0_noTime", 0.5); // -- true
          if( isPrompt ) histSet_prompt->Fill("frac_Iso0_noTime", 0.5);
          else           histSet_nonprompt->Fill("frac_Iso0_noTime", 0.5);
        }
      } // -- end of else(relTrkIso==0)

      histSet_all->Fill("relTrkIso", relTrkIso);
      if( isPrompt ) histSet_prompt->Fill("relTrkIso", relTrkIso);
      else           histSet_nonprompt->Fill("relTrkIso", relTrkIso);

      if( hasTime ) {
        histSet_all->Fill("relTrkIso_time", relTrkIso);
        if( isPrompt ) histSet_prompt->Fill("relTrkIso_time", relTrkIso);
        else           histSet_nonprompt->Fill("relTrkIso_time", relTrkIso);
      }
      else { // -- no time
        histSet_all->Fill("relTrkIso_noTime", relTrkIso);
        if( isPrompt ) histSet_prompt->Fill("relTrkIso_noTime", relTrkIso);
        else           histSet_nonprompt->Fill("relTrkIso_noTime", relTrkIso);
      }

    } // -- end of muon iteration
  } // -- end of event iteration

  TString outputFName = TString::Format("ROOTFile_%s_%s.root", sampleType.Data(), splitNum.Data());
  TFile *f_output = TFile::Open(outputFName, "RECREATE");
  f_output->cd();
  histSet_all->Write();
  histSet_prompt->Write();
  histSet_nonprompt->Write();
  f_output->Close();
}


TString GetFilePath(TString sampleType, TString splitNum) {
  TString basePath;
  if( sampleType == "DY" )
    basePath = "/dcache_mnt/dcache/pnfs/iihe/cms/store/user/kplee/MuonHLTTree_phase2MTD_v2/DYToLL_M-50_TuneCP5_14TeV-pythia8/crab_DYLL_M50_Pythia8/221017_134428/0000";
  else if( sampleType == "TT" )
    basePath = "/dcache_mnt/dcache/pnfs/iihe/cms/store/user/kplee/MuonHLTTree_phase2MTD_v2/TTToSemileptonic_TuneCP5_14TeV-powheg-pythia8/crab_TTSemiLeptonic_Powheg/221017_134431/0000";

  return TString::Format("%s/ntuple_%s.root", basePath.Data(), splitNum.Data());
}

void Setup_HistSet(MuonHLT::HistSet* histSet) {
  histSet->Register("frac_Iso0", 2, 0, 2);
  histSet->Register("frac_Iso0_noTime", 2, 0, 2);
  histSet->Register("frac_Iso0_time", 2, 0, 2);

  histSet->Register("relTrkIso", 500, 0, 0.5);
  histSet->Register("relTrkIso_noTime", 500, 0, 0.5);
  histSet->Register("relTrkIso_time", 500, 0, 0.5);

}

Double_t RelTrkIso(Double_t pt_mu, Int_t i_matchedTrack, vector<MuonHLT::GeneralTrack>& vec_GT, Double_t dRCut_inner, Double_t dRCut_outer, Double_t dzCut) {

  Double_t pt_sum = 0;  
  for(auto& track : vec_GT ) {

    Double_t dR = ROOT::Math::VectorUtil::DeltaR(vec_GT[i_matchedTrack].vecP, track.vecP);
    if( !(dRCut_inner < dR && dR < dRCut_outer) )
      continue;

    Double_t dz = std::abs(vec_GT[i_matchedTrack].dz - track.dz);
    if( dz > dzCut ) continue;

    Double_t pt_track = track.pt;
    pt_sum = pt_sum + pt_track;
  }

  return pt_sum / pt_mu;
}