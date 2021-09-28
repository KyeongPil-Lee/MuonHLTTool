#include <Include/MuonHLTTool.h>

#include <TStopwatch.h>
#include <TSystem.h>

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

    // -- IsoMu24 object distribution
    TH1D* h_IsoMu24Obj_pt  = new TH1D("h_IsoMu24Obj_pt", "", 10000, 0, 10000);
    TH1D* h_IsoMu24Obj_eta = new TH1D("h_IsoMu24Obj_eta", "", 60, -3, 3);
    TH1D* h_IsoMu24Obj_phi = new TH1D("h_IsoMu24Obj_phi", "", 80, -4, 4);
    TString filterName_IsoMu24 = "hltL3crIsoL1sSingleMu22L1f0L2f10QL3f24QL3trkIsoFiltered0p07::MYHLT";

    // -- Mu24 + custom isolation filter distribution
    TH1D* h_Mu24Obj_IsoF_pt  = new TH1D("h_Mu24Obj_IsoF_pt", "", 10000, 0, 10000);
    TH1D* h_Mu24Obj_IsoF_eta = new TH1D("h_Mu24Obj_IsoF_eta", "", 60, -3, 3);
    TH1D* h_Mu24Obj_IsoF_phi = new TH1D("h_Mu24Obj_IsoF_phi", "", 80, -4, 4);
    TString filterName_Mu24 = "hltL3fL1sSingleMu22L1f0L2f10QL3Filtered24Q::MYHLT";

    Int_t nEvent = chain->GetEntries();
    std::cout << "[Total event: " << nEvent << "]" << std::endl;

    for(Int_t i_ev=0; i_ev<nEvent; i_ev++)
    {
      MuonHLT::loadBar(i_ev+1, nEvent, 100, 100);
      ntuple->GetEvent(i_ev);

      Double_t weight = ntuple->isRealData? 1.0 : ntuple->genEventWeight;

      vector<MuonHLT::MYHLTObject> vec_IsoMu24Obj = GetAllMYHLTObject(ntuple, filterName_IsoMu24);
      for( auto& obj : vec_IsoMu24Obj )
      {
        h_IsoMu24Obj_pt->Fill( obj.pt, weight );
        h_IsoMu24Obj_eta->Fill( obj.eta, weight );
        h_IsoMu24Obj_phi->Fill( obj.phi, weight );
      }

      vector<MuonHLT::MYHLTObject> vec_Mu24Obj = GetAllMYHLTObject(ntuple, filterName_Mu24);
      for( auto& obj : vec_Mu24Obj )
      {
        MuonHLT::L3Muon L3Mu;
        if( Find_CorrespondingL3Muon(obj, ntuple, L3Mu) )
        {
          Double_t relECALIso = L3Mu.relECALIso;
          Double_t relHCALIso = L3Mu.relHCALIso;
          Double_t relTrkIso  = L3Mu.relTrkIso;

          if( HLTIsolationFilter(obj.eta, relECALIso, relHCALIso, relTrkIso) )
          {
            h_Mu24Obj_IsoF_pt->Fill( obj.pt );
            h_Mu24Obj_IsoF_eta->Fill( obj.eta );
            h_Mu24Obj_IsoF_phi->Fill( obj.phi );
          }
        }
        else
          Print_When_L3Muon_Is_Not_Found("Mu24", obj, ntuple);
      }

    } // -- end of event iteration

    TFile *f_output = TFile::Open(outputFileName_, "RECREATE");

    h_IsoMu24Obj_pt->Write();
    h_IsoMu24Obj_eta->Write();
    h_IsoMu24Obj_phi->Write();

    h_Mu24Obj_IsoF_pt->Write();
    h_Mu24Obj_IsoF_eta->Write();
    h_Mu24Obj_IsoF_phi->Write();

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

  void Print_When_L3Muon_Is_Not_Found(TString tag, MuonHLT::MYHLTObject obj, MuonHLT::NtupleHandle* ntuple)
  {
    cout << "No corresponding L3 muon is found" << endl;
    cout << TString::Format("filter(%s) object: (pt, eta, phi) = (%.1lf, %.3lf, %.3lf)", tag.Data(), obj.pt, obj.eta, obj.phi) << endl;
    cout << "L3 muons:" << endl;
    vector<MuonHLT::L3Muon> vec_L3Muon = GetAllL3Muon(ntuple);
    for( auto& L3Muon : vec_L3Muon )
      cout << TString::Format("   (pt, eta, phi) = (%.1lf, %.3lf, %.3lf)", L3Muon.pt, L3Muon.eta, L3Muon.phi) << endl;
    cout << endl;
  }

  Bool_t HLTIsolationFilter(Double_t eta, Double_t relECALIso, Double_t relHCALIso, Double_t relTrkIso)
  {
    Bool_t flag = kFALSE;
    if( fabs(eta) < 1.479 ) // -- barrel
    {
      if( relECALIso < 0.14 && relHCALIso < 0.16 && relTrkIso < 0.07 ) flag = kTRUE;
    }
    else // -- endcap
    {
      if( relECALIso < 0.10 && relHCALIso < 0.20 && relTrkIso < 0.07 ) flag = kTRUE;
    }

    return flag;
  }
};


void MakeHist_Validation_CustomIsoFilter(TString textFile_ntupleList)
{
  TString tag = textFile_ntupleList.Data();
  tag = gSystem->BaseName(tag);
  tag.ReplaceAll(".txt", "");
  TString outputFileName = TString::Format("ROOTFile_MakeHist_Isolation_IsoMu24_%s.root", tag.Data());

  HistProducer* histProducer = new HistProducer();
  histProducer->Set_OutputFileName(outputFileName);
  histProducer->Set_ntupleListFile(textFile_ntupleList);

  histProducer->Produce();
}
