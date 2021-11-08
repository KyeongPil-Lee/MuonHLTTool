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
    // for(const auto& dataPath : vec_dataPath )
    //   chain->Add(dataPath);
    MuonHLT::AddNtupleToChain( chain, fileName_ntupleList_ );

    MuonHLT::NtupleHandle* ntuple = new MuonHLT::NtupleHandle( chain );
    // ntuple->TurnOnBranches_GenParticle();
    // ntuple->TurnOnBranches_Muon();
    // ntuple->TurnOnBranches_HLTMuon();
    // ntuple->TurnOnBranches_IterL3Muon();


    Int_t nEvent = chain->GetEntries();
    std::cout << "[Total event: " << nEvent << "]" << std::endl;

    Double_t sumWeight = 0;
    for(Int_t i_ev=0; i_ev<nEvent; i_ev++)
    {
      MuonHLT::loadBar(i_ev+1, nEvent, 100, 100);
      ntuple->GetEvent(i_ev);

      if( ntuple->isRealData )
      {
        cout << "sum(weight) need not to be checked for the DATA!" << endl;
        return;
      }

      Double_t weight = ntuple->genEventWeight > 0 ? 1.0 : -1.0;
      sumWeight = sumWeight + weight;
    } // -- end of event iteration

    TH1D* h_sumWeight = new TH1D("h_sumWeight", "", 1, 0, 1);
    h_sumWeight->SetBinContent(1, sumWeight);
    h_sumWeight->SetBinError(1, 0);

    TFile *f_output = TFile::Open(outputFileName_, "RECREATE");
    f_output->cd();
    h_sumWeight->Write();
    f_output->Close();

    PrintRunTime();
  }

private:
  TStopwatch timer_;

  TString outputFileName_;
  vector<TString> vec_dataPath;

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


void MakeHist_SumWeight(TString textFile_ntupleList)
{
  TString tag = textFile_ntupleList.Data();
  tag = gSystem->BaseName(tag);
  tag.ReplaceAll(".txt", "");
  TString outputFileName = TString::Format("ROOTFile_%s.root", tag.Data());

  HistProducer* histProducer = new HistProducer();
  histProducer->Set_OutputFileName(outputFileName);
  histProducer->Set_ntupleListFile(textFile_ntupleList);

  histProducer->Produce();
}
