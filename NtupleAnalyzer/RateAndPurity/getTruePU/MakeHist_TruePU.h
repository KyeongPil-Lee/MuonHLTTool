#include <Include/NtupleHandle.h>
#include <Include/Object.h>

#include <iostream>
#include <fstream>

using namespace MuonHLT;

class HistProducer
{
public:
  TString type_;

  vector<TString> vec_dataPath_;

  Bool_t verbose_ = kFALSE;

  HistProducer( TString type )
  {
    type_ = type;
  }

  void SetVector_DataPath( vector<TString> vec ) { vec_dataPath_ = vec; }

  void Set_Verbose( Bool_t flag = kTRUE ) { verbose_ = flag; }
  void Produce()
  {
    TH1::AddDirectory(kFALSE);
    StartTimer();

    // -- histogram to be filled
    TH1D* h_truePU = new TH1D("h_truePU", "", 100, 0, 200);

    TChain *chain = new TChain("ntupler/ntuple");
    for(auto &dataPath : vec_dataPath_ )
    {
      cout << "Adding...   " << dataPath << endl;
      chain->Add( dataPath );
    }

    Int_t nEvent = chain->GetEntries();
    cout << "[Total event: " << nEvent << "]" << endl;

    NtupleHandle *ntuple = new NtupleHandle( chain );
    ntuple->TurnOnBranches_Event();

    for(Int_t i_ev=0; i_ev<nEvent; i_ev++)
    {
      loadBar(i_ev+1, nEvent, 100, 100);
      if( verbose_ ) cout << "[" << i_ev << "] event" << endl;

      ntuple->GetEvent( i_ev );

      Int_t truePU = ntuple->truePU;
      h_truePU->Fill( truePU );
      if( verbose_ )
        cout << " ---> true PU = " << truePU << endl;

      if( verbose_ ) cout << endl;
    }

    TString outputName = "ROOTFile_TruePU_"+type_+".root";
    TFile *f_output = TFile::Open(outputName, "RECREATE");
    f_output->cd();
    h_truePU->Write();
    f_output->Close();

    cout << "[Produce] " << outputName << " is produced" << endl;
    PrintRunTime();
  }

private:

  TStopwatch timer_;
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

  static inline void loadBar(int x, int n, int r, int w)
  {
      // Only update r times.
      if( x == n )
        cout << endl;

      if ( x % (n/r +1) != 0 ) return;


      // Calculuate the ratio of complete-to-incomplete.
      float ratio = x/(float)n;
      int   c     = ratio * w;

      // Show the percentage complete.
      printf("%3d%% [", (int)(ratio*100) );

      // Show the load bar.
      for (int x=0; x<c; x++) cout << "=";

      for (int x=c; x<w; x++) cout << " ";

      // ANSI Control codes to go back to the
      // previous line and clear it.
    cout << "]\r" << flush;

  }

};

void TESTRun()
{
  HistProducer* producer = new HistProducer("TEST");
  producer->SetVector_DataPath( {"../ntuple_77.root"} );
  producer->Set_Verbose();
  producer->Produce();
}


