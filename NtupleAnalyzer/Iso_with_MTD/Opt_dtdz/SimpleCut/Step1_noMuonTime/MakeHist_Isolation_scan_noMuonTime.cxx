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


TString GetFilePath(TString sampleType, TString splitNum) {
  TString basePath;
  if( sampleType == "DY" )
    basePath = "/dcache_mnt/dcache/pnfs/iihe/cms/store/user/kplee/MuonHLTTree_phase2MTD_v2/DYToLL_M-50_TuneCP5_14TeV-pythia8/crab_DYLL_M50_Pythia8/221017_134428/0000";
  else if( sampleType == "TT" )
    basePath = "/dcache_mnt/dcache/pnfs/iihe/cms/store/user/kplee/MuonHLTTree_phase2MTD_v2/TTToSemileptonic_TuneCP5_14TeV-powheg-pythia8/crab_TTSemiLeptonic_Powheg/221017_134431/0000";

  return TString::Format("%s/ntuple_%s.root", basePath.Data(), splitNum.Data());
}

// -- step 1: assume dzCut = dzCut_noTrackTime and perform scan
void MakeHist_Isolation_scan_noMuonTime(TString sampleType, TString splitNum) {
  cout << "sampleType = " << sampleType << endl;
  cout << "splitNum =   " << splitNum << endl;

  // -- isolation setting
  Double_t dRCut_inner = 0.01;
  Double_t dRCut_outer = 0.3;

  // -- TimeQualMVA cut
  Double_t min_timeQualMVA = 0.5;

  // -- setting the scanning tool
  Int_t scanRange_dz_nBin = 50;
  Double_t scanRange_dz_min = 0;
  Double_t scanRange_dz_max = 0.5;

  Double_t step_dz = (scanRange_dz_max - scanRange_dz_min) / scanRange_dz_nBin;
  vector<Double_t> vec_dzCut_no_muonTime = {};
  for(Int_t i=0; i<scanRange_dz_nBin; i++)
    vec_dzCut_no_muonTime.push_back( scanRange_dz_min + (i+1)*step_dz );

  vector<IsoHistProducer_1DScan*> vec_producer_no_muTime;
  for( auto dzCut_no_muonTime :  vec_dzCut_no_muonTime ) {
    TString type = TString::Format("dz%.2lf_no_muTime", dzCut_no_muonTime);
    type.ReplaceAll(".", "p");

    IsoHistProducer_1DScan* producer = new IsoHistProducer_1DScan(type, "dt", 1, 0, 1000); // -- 1 bin for dt: not used
    producer->Set_DZCut(dzCut_no_muonTime); // -- not used anyway
    producer->Set_DZCut_noTrackTime(dzCut_no_muonTime); // -- not used anyway
    producer->Set_DZCut_noMuonTime(dzCut_no_muonTime); // -- will only be used
    producer->Set_TimeMVACut(min_timeQualMVA);
    producer->Set_IsoType("SimpleCut");
    vec_producer_no_muTime.push_back( producer );
  }

  IsoHistProducer_1DScan* producer_default_no_muTime = new IsoHistProducer_1DScan("default", "dt", 1, 0, 1000); // -- 1 bin for dt: not used
  producer_default_no_muTime->Set_DZCut(0.2);
  producer_default_no_muTime->Set_IsoType("Default");

  TChain* chain = new TChain("ntupler/ntuple");
  if( sampleType == "test" )   chain->Add("../../../Example/ntuple_example.root");
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
      if( mu.pt < 24.0 ) continue; // -- exclude too low muons (to have same pt phase space between prompt and nonprompt & realistic range in phase-2 muon HLT)

      // -- to ensure the muon has the inner track (and corresponding time information)
      if( !mu.isLoose ) continue;

      Int_t i_matchedTrack = Find_MatchedGeneralTrackIndex(mu, vec_GT);
      Bool_t muonHasTimeInfo = (vec_GT[i_matchedTrack].timeQualMVA < max_timeQualMVA);

      if( !muonHasTimeInfo ) {
        producer_default_no_muTime->Fill( mu, vec_GT );
        for(auto producer : vec_producer_no_muTime)
          producer->Fill( mu, vec_GT );
      }

    } // -- end of the loop over the muons

  } // -- end of event iteration

  TString outputFName = TString::Format("ROOTFile_IsoScan_NoMuonTime_%s_%s.root", sampleType.Data(), splitNum.Data());
  TFile *f_output = TFile::Open(outputFName, "RECREATE");
  f_output->cd();
  producer_default_no_muTime->Save();
  for(auto producer : vec_producer_no_muTime)
    producer->Save();
  f_output->Close();
}