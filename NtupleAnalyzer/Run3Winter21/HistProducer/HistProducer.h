#include <Run3Winter21/HistProducer/HistContainer.h>

class HistProducer
{
public:
  void Set_OutputFileName( TString outputFileName )
  {
    outputFileName_ = outputFileName;
  }

  void Set_Debug( Bool_t flag = kTRUE ) { debug_ = flag; }

  void Set_ntupleListFile( TString fileName ) { fileName_ntupleList_ = fileName; }

  void Produce()
  {
    StartTimer();

    HistContainer *histContainer = new HistContainer();

    TChain* chain = new TChain("ntupler/ntuple");
    MuonHLT::AddNtupleToChain( chain, fileName_ntupleList_ );

    Double_t xSec      = MuonHLT::GetInfo("xSec", fileName_ntupleList_);
    Double_t sumWeight = MuonHLT::GetInfo("sumWeight", fileName_ntupleList_);
    TString sampleType = MuonHLT::GetInfo_String("sampleType", fileName_ntupleList_);
    cout << TString::Format("[Sample information] Sample type = %s, (xSec, sumWeight) = (%.3lf, %.1lf)", sampleType.Data(), xSec, sumWeight);

    Double_t normFactor = (1.0 * xSec ) / sumWeight; // -- norm. to 1 /pb data ---> easliy scale to arbitrary lumi.

    MuonHLT::NtupleHandle* ntuple = new MuonHLT::NtupleHandle( chain );
    ntuple->TurnOnBranches_GenParticle();
    ntuple->TurnOnBranches_Muon();
    ntuple->TurnOnBranches_HLTMuon();
    ntuple->TurnOnBranches_IterL3Muon();

    Int_t nEvent = chain->GetEntries();
    std::cout << "[Total event: " << nEvent << "]" << std::endl;

    for(Int_t i_ev=0; i_ev<nEvent; i_ev++)
    {
      MuonHLT::loadBar(i_ev+1, nEvent, 100, 100);
      ntuple->GetEvent(i_ev);

      if( debug_ ) cout << "[" << i_ev << "th event]" << endl;

      Double_t genWeight = ntuple->isRealData? 1.0 : ntuple->genEventWeight;
      Double_t totWeight = normFactor * genWeight;

      histContainer->Fill_Event(ntuple, totWeight );

      vector<MuonHLT::MYHLTObject> vec_MYHLTObj = MuonHLT::GetAllMYHLTObject(ntuple, "hltL3fL1sSingleMu22L1f0L2f10QL3Filtered24Q::MYHLT");

      for(auto& MYHLTObj : vec_MYHLTObj )
      {
        MYHLTObj.FillIsolationVariable(ntuple);

        histContainer->Fill_Mu24Obj( MYHLTObj, ntuple, totWeight );
      }
    } // -- end of event iteration

    TFile *f_output = TFile::Open(outputFileName_, "RECREATE");
    f_output->cd();
    histContainer->Save();
    f_output->Close();
    
    delete ntuple;
    delete chain;

    PrintRunTime();
  }

private:
  TStopwatch timer_;

  TString outputFileName_;

  TString fileName_ntupleList_ = "";

  Bool_t debug_ = kFALSE;

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