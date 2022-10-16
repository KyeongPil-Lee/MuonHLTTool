#include <TTreeReader.h>
#include <Iso_with_MTD/Include/MuonHandle.h>
#include <Iso_with_MTD/Include/GeneralTrackHandle.h>
#include <iostream>

using namespace MuonHLT;

void Test() {
  TChain *chain = new TChain("ntupler/ntuple");
  chain->Add("ntuple_example.root");

  TTreeReader* reader = new TTreeReader(chain);

  MuonHandle* muonHandle       = new MuonHandle(*reader);
  GeneralTrackHandle* GTHandle = new GeneralTrackHandle(*reader);

  Int_t nEvent_tot = reader->GetEntries(kTRUE);
  cout << "nEvent_tot = " << nEvent_tot << endl;

  for(Int_t i_ev=0; i_ev<nEvent_tot; i_ev++) {
    reader->SetEntry(i_ev);

    vector<MuonHLT::Muon> vec_muon = GetAll_Muon(muonHandle);
    cout << "nMuon = " << vec_muon.size() << endl;
    for(const auto muon : vec_muon ) {

      if( !muon.isTight ) continue;

      TString muonInfo = TString::Format("  (pt, eta, phi) = (%.1lf, %.3lf, %.3lf)", muon.pt, muon.eta, muon.phi);
      cout << muonInfo;
      if( muon.simType == 4 )
        cout << " ---> true muon" << endl;
      else
        cout << endl;

    } // -- end of muon iteration

    vector<MuonHLT::GeneralTrack> vec_GT = GetAll_GeneralTrack(GTHandle);
    cout << "nGeneralTrack = " << vec_GT.size() << endl;
    for(const auto track : vec_GT ) {
      if( track.pt < 0.9 ) continue;
      TString trackInfo = TString::Format("  (pt, eta, phi) = (%.1lf, %.3lf, %.3lf, %.3lf)", track.pt, track.eta, track.phi, track.time);
      cout << trackInfo << endl;
    }

  } // -- end of event iteration
}