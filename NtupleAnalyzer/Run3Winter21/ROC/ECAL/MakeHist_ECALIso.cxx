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

    TH1D* h_ECALIso    = new TH1D("h_ECALIso", "", 10000, 0, 10000);
    TH1D* h_relECALIso = new TH1D("h_relECALIso", "", 10000, 0, 10);

    TH1D* h_ECALIso_EB    = new TH1D("h_ECALIso_EB", "", 10000, 0, 10000);
    TH1D* h_ECALIso_EE    = new TH1D("h_ECALIso_EE", "", 10000, 0, 10000);

    TH1D* h_relECALIso_EB = new TH1D("h_relECALIso_EB", "", 10000, 0, 10);
    TH1D* h_relECALIso_EE = new TH1D("h_relECALIso_EE", "", 10000, 0, 10);

    // -- finer bins for ROC curve
    TH1D* h_relECALIso_low_fineBin_EB = new TH1D("h_relECALIso_low_fineBin_EB", "", 10000, 0, 0.1);
    TH1D* h_relECALIso_low_fineBin_EE = new TH1D("h_relECALIso_low_fineBin_EE", "", 10000, 0, 0.1);

    TH1D* h_relECALIso_high_fineBin_EB = new TH1D("h_relECALIso_high_fineBin_EB", "", 9000, 0.1, 1);
    TH1D* h_relECALIso_high_fineBin_EE = new TH1D("h_relECALIso_high_fineBin_EE", "", 9000, 0.1, 1);

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

      vector<MuonHLT::MYHLTObject> vec_MYHLTObj = MuonHLT::GetAllMYHLTObject(ntuple, "hltL3fL1sSingleMu22L1f0L2f10QL3Filtered24Q::MYHLT");

      for(auto& MYHLTObj : vec_MYHLTObj )
      {
        MYHLTObj.FillIsolationVariable(ntuple);

        if( debug_ )
        {
          if( MYHLTObj.ECALIso < 0.0 )
            cout << TString::Format("  [Isolation is negative] (pt, eta, phi, ECALIso, relECALIso, relHCALIso, relTrkIso) = (%.1lf, %.3lf, %3.lf, %.3lf, %.3lf, %.3lf, %.3lf)", MYHLTObj.pt, MYHLTObj.eta, MYHLTObj.phi, MYHLTObj.ECALIso, MYHLTObj.relECALIso, MYHLTObj.relHCALIso, MYHLTObj.relTrkIso) << endl;
        }


        h_ECALIso->Fill( MYHLTObj.ECALIso, totWeight );
        h_relECALIso->Fill( MYHLTObj.relECALIso, totWeight );

        if( fabs(MYHLTObj.eta) < 1.479 ) // -- barrel
        {
          h_ECALIso_EB->Fill( MYHLTObj.ECALIso, totWeight );

          h_relECALIso_EB->Fill( MYHLTObj.relECALIso, totWeight );
          h_relECALIso_low_fineBin_EB->Fill( MYHLTObj.relECALIso, totWeight );
          h_relECALIso_high_fineBin_EB->Fill( MYHLTObj.relECALIso, totWeight );
        }
        else // -- endcap
        {
          h_ECALIso_EE->Fill( MYHLTObj.ECALIso, totWeight );

          h_relECALIso_EE->Fill( MYHLTObj.relECALIso, totWeight );
          h_relECALIso_low_fineBin_EE->Fill( MYHLTObj.relECALIso, totWeight );
          h_relECALIso_high_fineBin_EE->Fill( MYHLTObj.relECALIso, totWeight );
        }
      }

      if( debug_ ) cout << endl;

    } // -- end of event iteration

    TFile *f_output = TFile::Open(outputFileName_, "RECREATE");
    
    h_ECALIso->Write();
    h_relECALIso->Write();

    h_ECALIso_EB->Write();
    h_relECALIso_EB->Write();
    h_relECALIso_low_fineBin_EB->Write();
    h_relECALIso_high_fineBin_EB->Write();

    h_ECALIso_EE->Write();
    h_relECALIso_EE->Write();
    h_relECALIso_low_fineBin_EE->Write();
    h_relECALIso_high_fineBin_EE->Write();

    f_output->Close();

    delete ntuple;
    delete chain;

    PrintRunTime();
  }

private:
  TStopwatch timer_;

  TString outputFileName_;

  TString fileName_ntupleList_ = "";

  Bool_t debug_ = kTRUE;

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


void MakeHist_ECALIso(TString textFile_ntupleList)
{
  TString tag = textFile_ntupleList.Data();
  tag = gSystem->BaseName(tag);
  tag.ReplaceAll(".txt", "");
  TString outputFileName = TString::Format("ROOTFile__%s.root", tag.Data());

  HistProducer* histProducer = new HistProducer();
  histProducer->Set_OutputFileName(outputFileName);
  histProducer->Set_ntupleListFile(textFile_ntupleList);

  histProducer->Produce();
}
