#include <2018Data/Rate/DataRateTool.h>
#include <TStopwatch.h>
#include <TH1.h>
#include <TH2.h>

class DataRateHistProducer {
public:
  void Set_ntupleListFile( TString fileName ) { fileName_ntupleList_ = fileName; }

  void Set_TriggerList_HLT(vector<TString> vec_trigger)   { vec_trigger_HLT_ = vec_trigger; }
  void Set_TriggerList_MYHLT(vector<TString> vec_trigger) { vec_trigger_MYHLT_ = vec_trigger; }

  // -- weight = HLT_Physics prescale * # HLTPhysics dataset
  void Set_Weight( Double_t weight ) { weight_ = weight; }

  void Produce() {
    TH1::SetDefaultSumw2();
    TH2::SetDefaultSumw2();

    StartTimer();

    TString sampleType = MuonHLT::GetInfo_String("sampleType", fileName_ntupleList_);

    DataRateHistContainer* histContainer = new DataRateHistContainer();
    histContainer->Set_TriggerList_HLT( vec_trigger_HLT_ );
    histContainer->Set_TriggerList_MYHLT( vec_trigger_MYHLT_ );
    histContainer->Set_Weight( weight_ );
    histContainer->Init();

    TChain* chain = new TChain("ntupler/ntuple");
    MuonHLT::AddNtupleToChain( chain, fileName_ntupleList_ );

    MuonHLT::NtupleHandle* ntuple = new MuonHLT::NtupleHandle( chain );
    // ntuple->TurnOnBranches_GenParticle();
    // ntuple->TurnOnBranches_Muon();
    // ntuple->TurnOnBranches_HLTMuon();
    // ntuple->TurnOnBranches_IterL3Muon();

    Int_t nEvent = chain->GetEntries();
    std::cout << "[Total event: " << nEvent << "]" << std::endl;

    for(Int_t i_ev=0; i_ev<nEvent; i_ev++) {
      MuonHLT::loadBar(i_ev+1, nEvent, 100, 100);

      ntuple->GetEvent(i_ev);

      histContainer->Fill( ntuple );

      // cout << TString::Format("[%d event] (nLS, instLumi) = (%d, %.3lf)", i_ev, ntuple->lumiBlockNum, ntuple->instLumi) << endl;
    }

    TString outputFileName = MuonHLT::MakeOutputFileName("DataRateHistProducer", sampleType, fileName_ntupleList_);
    TFile *f_output = TFile::Open(outputFileName, "RECREATE");
    f_output->cd();
    histContainer->Save();
    f_output->Close();

    PrintRunTime();
  }
private:
  TStopwatch timer_;

  TString fileName_ntupleList_ = "";

  Double_t weight_ = -1.0;

  vector<TString> vec_trigger_HLT_ = {};
  vector<TString> vec_trigger_MYHLT_ = {};

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