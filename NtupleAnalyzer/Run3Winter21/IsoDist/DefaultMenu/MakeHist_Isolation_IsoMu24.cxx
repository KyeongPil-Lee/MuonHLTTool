#include <TStopwatch.h>
#include <Include/MuonHLTTool.h>

class HistProducer
{
public:
  void Set_OutputFileName( TString outputFileName )
  {
    outputFileName_ = outputFileName;
  }

  void Set_ntupleListFile( TString fileName ) { fileName_ntupleList_ = fileName; }

  void Produce()
  {
    StartTimer();

    TChain* chain = new TChain("ntupler/ntuple");
    MuonHLT::AddNtupleToChain( chain, fileName_ntupleList_ );

    MuonHLT::NtupleHandle* ntuple = new MuonHLT::NtupleHandle( chain );
    // ntuple->TurnOnBranches_GenParticle();
    // ntuple->TurnOnBranches_Muon();
    ntuple->TurnOnBranches_HLTMuon();
    // ntuple->TurnOnBranches_IterL3Muon();

    TH1D* h_ECALIso_EE = new TH1D("h_ECALIso_EE", "", 10000, 0, 10000);
    TH1D* h_ECALIso_EB = new TH1D("h_ECALIso_EB", "", 10000, 0, 10000);
    TH1D* h_relECALIso_EE = new TH1D("h_relECALIso_EE", "", 1000, 0, 10);
    TH1D* h_relECALIso_EB = new TH1D("h_relECALIso_EB", "", 1000, 0, 10);

    TH1D* h_HCALIso_EE = new TH1D("h_HCALIso_EE", "", 10000, 0, 10000);
    TH1D* h_HCALIso_EB = new TH1D("h_HCALIso_EB", "", 10000, 0, 10000);
    TH1D* h_relHCALIso_EE = new TH1D("h_relHCALIso_EE", "", 1000, 0, 10);
    TH1D* h_relHCALIso_EB = new TH1D("h_relHCALIso_EB", "", 1000, 0, 10);

    TH1D* h_trkIso_EE = new TH1D("h_trkIso_EE", "", 10000, 0, 10000);
    TH1D* h_trkIso_EB = new TH1D("h_trkIso_EB", "", 10000, 0, 10000);
    TH1D* h_relTrkIso_EE = new TH1D("h_relTrkIso_EE", "", 1000, 0, 10);
    TH1D* h_relTrkIso_EB = new TH1D("h_relTrkIso_EB", "", 1000, 0, 10);

    Int_t nEvent = chain->GetEntries();
    std::cout << "[Total event: " << nEvent << "]" << std::endl;

    for(Int_t i_ev=0; i_ev<nEvent; i_ev++)
    {
      MuonHLT::loadBar(i_ev+1, nEvent, 100, 100);
      ntuple->GetEvent(i_ev);

      Double_t weight = ntuple->isRealData? 1.0 : ntuple->genEventWeight;

      vector<MuonHLT::MYHLTObject> vec_IsoMu24Obj = GetAllMYHLTObject(ntuple, "hltL3crIsoL1sSingleMu22L1f0L2f10QL3f24QL3trkIsoFiltered0p07::MYHLT");
      for( auto& obj : vec_IsoMu24Obj )
      {
        MuonHLT::L3Muon L3Mu;
        if( Find_CorrespondingL3Muon(obj, ntuple, L3Mu) )
        {
          Double_t ECALIso = L3Mu.ECALIso;
          Double_t HCALIso = L3Mu.HCALIso;
          Double_t trkIso  = L3Mu.trkIso;

          Double_t relECALIso = ECALIso / L3Mu.pt;
          Double_t relHCALIso = HCALIso / L3Mu.pt;
          Double_t relTrkIso  = trkIso / L3Mu.pt;

          if( fabs(obj.eta) < 1.479 )
          {
            h_ECALIso_EB->Fill( ECALIso, weight );
            h_HCALIso_EB->Fill( HCALIso, weight );
            h_trkIso_EB->Fill( trkIso, weight );

            h_relECALIso_EB->Fill( relECALIso, weight );
            h_relHCALIso_EB->Fill( relHCALIso, weight );
            h_relTrkIso_EB->Fill( relTrkIso, weight );
          }
          else
          {
            h_ECALIso_EE->Fill( ECALIso, weight );
            h_HCALIso_EE->Fill( HCALIso, weight );
            h_trkIso_EE->Fill( trkIso, weight );

            h_relECALIso_EE->Fill( relECALIso, weight );
            h_relHCALIso_EE->Fill( relHCALIso, weight );
            h_relTrkIso_EE->Fill( relTrkIso, weight );
          }

        } // -- end of Find_CorrespondingL3Muon(obj, L3Mu)
        else
        {
          cout << "No corresponding L3 muon is found" << endl;
          cout << TString::Format("IsoMu24 object: (pt, eta, phi) = (%.1lf, %.3lf, %.3lf)", obj.pt, obj.eta, obj.phi) << endl;
          cout << "L3 muons:" << endl;
          vector<MuonHLT::L3Muon> vec_L3Muon = GetAllL3Muon(ntuple);
          for( auto& L3Muon : vec_L3Muon )
            cout << TString::Format("   (pt, eta, phi) = (%.1lf, %.3lf, %.3lf)", L3Muon.pt, L3Muon.eta, L3Muon.phi) << endl;
          cout << endl;
        }

      } // -- end of IsoMu24 object iteration

    } // -- end of event iteration

    TFile *f_output = TFile::Open(outputFileName_, "RECREATE");

    h_ECALIso_EE->Write();
    h_ECALIso_EB->Write();
    h_relECALIso_EE->Write(); 
    h_relECALIso_EB->Write();

    h_HCALIso_EE->Write(); 
    h_HCALIso_EB->Write();
    h_relHCALIso_EE->Write(); 
    h_relHCALIso_EB->Write();

    h_trkIso_EE->Write(); 
    h_trkIso_EB->Write();
    h_relTrkIso_EE->Write(); 
    h_relTrkIso_EB->Write();

    f_output->Close();

    delete ntuple;
    delete chain;

    PrintRunTime();
  }

private:
  TStopwatch timer_;

  TString outputFileName_;
  vector<TString> vec_dataPath;
  Double_t minPt_;

  TString fileName_ntupleList_ = "";

  void StartTimer()
  {
    timer_.Start();
  }

  void PrintRunTime()
  {
    Double_t cpuTime = timer_.CpuTime();
    Double_t realTime = timer_.RealTime();

    cout << "************************************************" << endl;
    cout << "Total real time: " << realTime << " (seconds)" << endl;
    cout << "Total CPU time:  " << cpuTime << " (seconds)" << endl;
    cout << "  CPU time / real time = " << cpuTime / realTime << endl;
    cout << "************************************************" << endl;
  }
};


void MakeHist_Isolation_IsoMu24(TString textFile_ntupleList)
{
  TString tag = textFile_ntupleList.Data();
  tag.ReplaceAll(".txt", "");
  TString outputFileName = TString::Format("ROOTFile_MakeHist_Isolation_IsoMu24_%s.root", tag.Data());

  HistProducer* histProducer = new HistProducer();
  histProducer->Set_OutputFileName(outputFileName);
  histProducer->Set_ntupleListFile(textFile_ntupleList);

  histProducer->Produce();
}
