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

  void SetRange_PU_LowPU(Double_t min, Double_t max) {
    minPU_lowPU_ = min;
    maxPU_lowPU_ = max;
  }

  void SetRange_PU_HighPU(Double_t min, Double_t max) {
    minPU_highPU_ = min;
    maxPU_highPU_ = max;
  }

  void Set_NewWP_ECAL( Double_t WP_EB, Double_t WP_EE ) {
    WP_new_ECAL_EB_ = WP_EB;
    WP_new_ECAL_EE_ = WP_EE;
  }

  void Set_NewWP_HCAL( Double_t WP_EB, Double_t WP_EE ) {
    WP_new_HCAL_EB_ = WP_EB;
    WP_new_HCAL_EE_ = WP_EE;
  }

  void Produce()
  {
    StartTimer();

    HistContainer *histContainer = new HistContainer();
    HistContainer *histContainer_lowPU  = new HistContainer("lowPU");
    HistContainer *histContainer_highPU = new HistContainer("highPU");

    histContainer->Set_GenMatchingForDYSample();
    histContainer_lowPU->Set_GenMatchingForDYSample();
    histContainer_highPU->Set_GenMatchingForDYSample();

    histContainer->Set_NewWP_ECAL( WP_new_ECAL_EB_, WP_new_ECAL_EE_ );
    histContainer_lowPU->Set_NewWP_ECAL( WP_new_ECAL_EB_, WP_new_ECAL_EE_ );
    histContainer_highPU->Set_NewWP_ECAL( WP_new_ECAL_EB_, WP_new_ECAL_EE_ );

    histContainer->Set_NewWP_HCAL( WP_new_HCAL_EB_, WP_new_HCAL_EE_ );
    histContainer_lowPU->Set_NewWP_HCAL( WP_new_HCAL_EB_, WP_new_HCAL_EE_ );
    histContainer_highPU->Set_NewWP_HCAL( WP_new_HCAL_EB_, WP_new_HCAL_EE_ );

    histContainer->Set_MinPt_ForTnPEff( 26 );
    histContainer_lowPU->Set_MinPt_ForTnPEff( 26 );
    histContainer_highPU->Set_MinPt_ForTnPEff( 26 );

    TChain* chain = new TChain("ntupler/ntuple");
    MuonHLT::AddNtupleToChain( chain, fileName_ntupleList_ );

    Double_t xSec      = MuonHLT::GetInfo("xSec", fileName_ntupleList_);
    Double_t sumWeight = MuonHLT::GetInfo("sumWeight", fileName_ntupleList_);
    TString sampleType = MuonHLT::GetInfo_String("sampleType", fileName_ntupleList_);
    cout << TString::Format("[Sample information] Sample type = %s, (xSec, sumWeight) = (%.3lf, %.1lf)", sampleType.Data(), xSec, sumWeight);

    Double_t normFactor = (1.0 * xSec ) / sumWeight; // -- norm. to 1 /pb data ---> easliy scale to arbitrary lumi.

    histContainer->Set_SampleType( sampleType );
    histContainer_lowPU->Set_SampleType( sampleType );
    histContainer_highPU->Set_SampleType( sampleType );

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

      Double_t truePU = ntuple->truePU;

      histContainer->Fill_Event(ntuple, totWeight );
      if( minPU_lowPU_ < truePU  && truePU < maxPU_lowPU_ )  histContainer_lowPU->Fill_Event(ntuple, totWeight );
      if( minPU_highPU_ < truePU && truePU < maxPU_highPU_ ) histContainer_highPU->Fill_Event(ntuple, totWeight );

      vector<MuonHLT::MYHLTObject> vec_MYHLTObj = MuonHLT::GetAllMYHLTObject(ntuple, "hltL3fL1sSingleMu22L1f0L2f10QL3Filtered24Q::MYHLT");

      for(auto& MYHLTObj : vec_MYHLTObj )
      {
        MYHLTObj.FillIsolationVariable(ntuple);

        histContainer->Fill_Mu24Obj( MYHLTObj, ntuple, totWeight );
        if( minPU_lowPU_ < truePU  && truePU < maxPU_lowPU_ )  histContainer_lowPU->Fill_Mu24Obj( MYHLTObj, ntuple, totWeight );
        if( minPU_highPU_ < truePU && truePU < maxPU_highPU_ ) histContainer_highPU->Fill_Mu24Obj( MYHLTObj, ntuple, totWeight );
      }
    } // -- end of event iteration

    TFile *f_output = TFile::Open(outputFileName_, "RECREATE");
    f_output->cd();
    histContainer->Save();
    histContainer_lowPU->Save();
    histContainer_highPU->Save();
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

  Double_t minPU_lowPU_ = 0;
  Double_t maxPU_lowPU_ = 40;

  Double_t minPU_highPU_ = 40;
  Double_t maxPU_highPU_ = 60;

  Double_t WP_new_ECAL_EB_ = 0;
  Double_t WP_new_ECAL_EE_ = 0;

  Double_t WP_new_HCAL_EB_ = 0;
  Double_t WP_new_HCAL_EE_ = 0;

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