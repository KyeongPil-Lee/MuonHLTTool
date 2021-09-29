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

    // -- IsoMu24 object distribution: in this ntuple, Isolation filters are ignored --> it should be same with Mu24
    TH1D* h_nIsoMu24Obj    = new TH1D("h_nIsoMu24Obj", "", 100, 0, 100);
    TH1D* h_IsoMu24Obj_pt  = new TH1D("h_IsoMu24Obj_pt", "", 10000, 0, 10000);
    TH1D* h_IsoMu24Obj_eta = new TH1D("h_IsoMu24Obj_eta", "", 60, -3, 3);
    TH1D* h_IsoMu24Obj_phi = new TH1D("h_IsoMu24Obj_phi", "", 80, -4, 4);
    TString filterName_IsoMu24 = "hltL3crIsoL1sSingleMu22L1f0L2f10QL3f24QL3trkIsoFiltered0p07::MYHLT";

    // -- Mu24 object
    TH1D* h_nMu24Obj    = new TH1D("h_nMu24Obj", "", 100, 0, 100);
    TH1D* h_Mu24Obj_pt  = new TH1D("h_Mu24Obj_pt", "", 10000, 0, 10000);
    TH1D* h_Mu24Obj_eta = new TH1D("h_Mu24Obj_eta", "", 60, -3, 3);
    TH1D* h_Mu24Obj_phi = new TH1D("h_Mu24Obj_phi", "", 80, -4, 4);

    TH1D* h_Mu24Obj_ECALIso_EE = new TH1D("h_Mu24Obj_ECALIso_EE", "", 10000, 0, 10000);
    TH1D* h_Mu24Obj_ECALIso_EB = new TH1D("h_Mu24Obj_ECALIso_EB", "", 10000, 0, 10000);
    TH1D* h_Mu24Obj_relECALIso_EE = new TH1D("h_Mu24Obj_relECALIso_EE", "", 1000, 0, 10);
    TH1D* h_Mu24Obj_relECALIso_EB = new TH1D("h_Mu24Obj_relECALIso_EB", "", 1000, 0, 10);

    TH1D* h_Mu24Obj_HCALIso_EE = new TH1D("h_Mu24Obj_HCALIso_EE", "", 10000, 0, 10000);
    TH1D* h_Mu24Obj_HCALIso_EB = new TH1D("h_Mu24Obj_HCALIso_EB", "", 10000, 0, 10000);
    TH1D* h_Mu24Obj_relHCALIso_EE = new TH1D("h_Mu24Obj_relHCALIso_EE", "", 1000, 0, 10);
    TH1D* h_Mu24Obj_relHCALIso_EB = new TH1D("h_Mu24Obj_relHCALIso_EB", "", 1000, 0, 10);

    TH1D* h_Mu24Obj_trkIso_EE = new TH1D("h_Mu24Obj_trkIso_EE", "", 10000, 0, 10000);
    TH1D* h_Mu24Obj_trkIso_EB = new TH1D("h_Mu24Obj_trkIso_EB", "", 10000, 0, 10000);
    TH1D* h_Mu24Obj_relTrkIso_EE = new TH1D("h_Mu24Obj_relTrkIso_EE", "", 1000, 0, 10);
    TH1D* h_Mu24Obj_relTrkIso_EB = new TH1D("h_Mu24Obj_relTrkIso_EB", "", 1000, 0, 10);
    TString filterName_Mu24 = "hltL3fL1sSingleMu22L1f0L2f10QL3Filtered24Q::MYHLT";

    // -- Mu24 + custom isolation filter distribution
    TH1D* h_nMu24Obj_IsoF    = new TH1D("h_nMu24Obj_IsoF", "", 100, 0, 100);
    TH1D* h_Mu24Obj_IsoF_pt  = new TH1D("h_Mu24Obj_IsoF_pt", "", 10000, 0, 10000);
    TH1D* h_Mu24Obj_IsoF_eta = new TH1D("h_Mu24Obj_IsoF_eta", "", 60, -3, 3);
    TH1D* h_Mu24Obj_IsoF_phi = new TH1D("h_Mu24Obj_IsoF_phi", "", 80, -4, 4);

    TH1D* h_Mu24Obj_IsoF_ECALIso_EE = new TH1D("h_Mu24Obj_IsoF_ECALIso_EE", "", 10000, 0, 10000);
    TH1D* h_Mu24Obj_IsoF_ECALIso_EB = new TH1D("h_Mu24Obj_IsoF_ECALIso_EB", "", 10000, 0, 10000);
    TH1D* h_Mu24Obj_IsoF_relECALIso_EE = new TH1D("h_Mu24Obj_IsoF_relECALIso_EE", "", 1000, 0, 10);
    TH1D* h_Mu24Obj_IsoF_relECALIso_EB = new TH1D("h_Mu24Obj_IsoF_relECALIso_EB", "", 1000, 0, 10);

    TH1D* h_Mu24Obj_IsoF_HCALIso_EE = new TH1D("h_Mu24Obj_IsoF_HCALIso_EE", "", 10000, 0, 10000);
    TH1D* h_Mu24Obj_IsoF_HCALIso_EB = new TH1D("h_Mu24Obj_IsoF_HCALIso_EB", "", 10000, 0, 10000);
    TH1D* h_Mu24Obj_IsoF_relHCALIso_EE = new TH1D("h_Mu24Obj_IsoF_relHCALIso_EE", "", 1000, 0, 10);
    TH1D* h_Mu24Obj_IsoF_relHCALIso_EB = new TH1D("h_Mu24Obj_IsoF_relHCALIso_EB", "", 1000, 0, 10);

    TH1D* h_Mu24Obj_IsoF_trkIso_EE = new TH1D("h_Mu24Obj_IsoF_trkIso_EE", "", 10000, 0, 10000);
    TH1D* h_Mu24Obj_IsoF_trkIso_EB = new TH1D("h_Mu24Obj_IsoF_trkIso_EB", "", 10000, 0, 10000);
    TH1D* h_Mu24Obj_IsoF_relTrkIso_EE = new TH1D("h_Mu24Obj_IsoF_relTrkIso_EE", "", 1000, 0, 10);
    TH1D* h_Mu24Obj_IsoF_relTrkIso_EB = new TH1D("h_Mu24Obj_IsoF_relTrkIso_EB", "", 1000, 0, 10);

    Int_t nEvent = chain->GetEntries();
    std::cout << "[Total event: " << nEvent << "]" << std::endl;

    for(Int_t i_ev=0; i_ev<nEvent; i_ev++)
    {
      MuonHLT::loadBar(i_ev+1, nEvent, 100, 100);
      ntuple->GetEvent(i_ev);

      Double_t weight = ntuple->isRealData? 1.0 : ntuple->genEventWeight;

      vector<MuonHLT::MYHLTObject> vec_IsoMu24Obj = GetAllMYHLTObject(ntuple, filterName_IsoMu24);
      Int_t nIsoMu24Obj = (Int_t)vec_IsoMu24Obj.size();
      h_nIsoMu24Obj->Fill( nIsoMu24Obj, weight );
      for( auto& obj : vec_IsoMu24Obj )
      {
        h_IsoMu24Obj_pt->Fill( obj.pt, weight );
        h_IsoMu24Obj_eta->Fill( obj.eta, weight );
        h_IsoMu24Obj_phi->Fill( obj.phi, weight );
      }

      Int_t nMu24Obj_IsoF = 0;
      vector<MuonHLT::MYHLTObject> vec_Mu24Obj = GetAllMYHLTObject(ntuple, filterName_Mu24);
      Int_t nMu24Obj = (Int_t)vec_Mu24Obj.size();
      h_nMu24Obj->Fill( nMu24Obj, weight );
      for( auto& obj : vec_Mu24Obj )
      {
        h_Mu24Obj_pt->Fill( obj.pt, weight );
        h_Mu24Obj_eta->Fill( obj.eta, weight );
        h_Mu24Obj_phi->Fill( obj.phi, weight );

        MuonHLT::L3Muon L3Mu;
        if( Find_CorrespondingL3Muon(obj, ntuple, L3Mu) )
        {
          Double_t ECALIso = L3Mu.ECALIso;
          Double_t HCALIso = L3Mu.HCALIso;
          Double_t trkIso  = L3Mu.trkIso;

          Double_t relECALIso = L3Mu.relECALIso;
          Double_t relHCALIso = L3Mu.relHCALIso;
          Double_t relTrkIso  = L3Mu.relTrkIso;

          if( fabs(obj.eta) < 1.479 )
          {
            h_Mu24Obj_ECALIso_EB->Fill( ECALIso, weight );
            h_Mu24Obj_HCALIso_EB->Fill( HCALIso, weight );
            h_Mu24Obj_trkIso_EB->Fill( trkIso, weight );

            h_Mu24Obj_relECALIso_EB->Fill( relECALIso, weight );
            h_Mu24Obj_relHCALIso_EB->Fill( relHCALIso, weight );
            h_Mu24Obj_relTrkIso_EB->Fill( relTrkIso, weight );
          }
          else
          {
            h_Mu24Obj_ECALIso_EE->Fill( ECALIso, weight );
            h_Mu24Obj_HCALIso_EE->Fill( HCALIso, weight );
            h_Mu24Obj_trkIso_EE->Fill( trkIso, weight );

            h_Mu24Obj_relECALIso_EE->Fill( relECALIso, weight );
            h_Mu24Obj_relHCALIso_EE->Fill( relHCALIso, weight );
            h_Mu24Obj_relTrkIso_EE->Fill( relTrkIso, weight );
          }

          if( HLTIsolationFilter(obj.eta, relECALIso, relHCALIso, relTrkIso) )
          {
            nMu24Obj_IsoF++;
            h_Mu24Obj_IsoF_pt->Fill( obj.pt );
            h_Mu24Obj_IsoF_eta->Fill( obj.eta );
            h_Mu24Obj_IsoF_phi->Fill( obj.phi );

            if( fabs(obj.eta) < 1.479 )
            {
              h_Mu24Obj_IsoF_ECALIso_EB->Fill( ECALIso, weight );
              h_Mu24Obj_IsoF_HCALIso_EB->Fill( HCALIso, weight );
              h_Mu24Obj_IsoF_trkIso_EB->Fill( trkIso, weight );

              h_Mu24Obj_IsoF_relECALIso_EB->Fill( relECALIso, weight );
              h_Mu24Obj_IsoF_relHCALIso_EB->Fill( relHCALIso, weight );
              h_Mu24Obj_IsoF_relTrkIso_EB->Fill( relTrkIso, weight );
            }
            else
            {
              h_Mu24Obj_IsoF_ECALIso_EE->Fill( ECALIso, weight );
              h_Mu24Obj_IsoF_HCALIso_EE->Fill( HCALIso, weight );
              h_Mu24Obj_IsoF_trkIso_EE->Fill( trkIso, weight );

              h_Mu24Obj_IsoF_relECALIso_EE->Fill( relECALIso, weight );
              h_Mu24Obj_IsoF_relHCALIso_EE->Fill( relHCALIso, weight );
              h_Mu24Obj_IsoF_relTrkIso_EE->Fill( relTrkIso, weight );
            }
          }
        }
        else
          Print_When_L3Muon_Is_Not_Found("Mu24", obj, ntuple);

        h_nMu24Obj_IsoF->Fill( nMu24Obj_IsoF, weight );
      }

      // if( nIsoMu24Obj != nMu24Obj_IsoF )
      // {
      //   TString analyzerPath = gSystem->Getenv("MUONHLT_ANALYZER_PATH");
      //   TString basePath = analyzerPath+"/Run3Winter21/IsoDist";
      //   ofstream logEvent(basePath+"/Event_differentNumberOfObject.txt", std::ios_base::out | std::ios_base::app);

      //   logEvent << "===============================" << endl;
      //   logEvent << TString::Format("[# objects] (IsoMu24, Mu24+IsoF) = (%d, %d)", nIsoMu24Obj, nMu24Obj_IsoF) << endl;
      //   logEvent << "[IsoMu24 object]" << endl;
      //   for( auto& obj : vec_IsoMu24Obj )
      //   {
      //     MuonHLT::L3Muon L3Mu;
      //     if( Find_CorrespondingL3Muon(obj, ntuple, L3Mu) )
      //     {
      //       Double_t relECALIso = L3Mu.relECALIso;
      //       Double_t relHCALIso = L3Mu.relHCALIso;
      //       Double_t relTrkIso  = L3Mu.relTrkIso;

      //       logEvent << TString::Format("  (pt, eta, phi, relECALIso, relHCALIso, relTrkIso) = (%.1lf, %.3lf, %.3lf, %.3lf, %.3lf, %.3lf)", obj.pt, obj.eta, obj.phi, relECALIso, relHCALIso, relTrkIso) << endl;
      //     }
      //     else
      //       logEvent << TString::Format("  (pt, eta, phi, relECALIso, relHCALIso, relTrkIso) = (%.1lf, %.3lf, %.3lf, corresponding L3 is not found)", obj.pt, obj.eta, obj.phi) << endl;
      //   }

      //   logEvent << "[Mu24+IsoF object]" << endl;
      //   for( auto& obj : vec_Mu24Obj )
      //   {
      //     MuonHLT::L3Muon L3Mu;
      //     if( Find_CorrespondingL3Muon(obj, ntuple, L3Mu) )
      //     {
      //       Double_t relECALIso = L3Mu.relECALIso;
      //       Double_t relHCALIso = L3Mu.relHCALIso;
      //       Double_t relTrkIso  = L3Mu.relTrkIso;

      //       if( HLTIsolationFilter(obj.eta, relECALIso, relHCALIso, relTrkIso) )
      //         logEvent << TString::Format("  (pt, eta, phi, relECALIso, relHCALIso, relTrkIso) = (%.1lf, %.3lf, %.3lf, %.3lf, %.3lf, %.3lf)", obj.pt, obj.eta, obj.phi, relECALIso, relHCALIso, relTrkIso) << endl;
      //       else
      //         logEvent << TString::Format("  (pt, eta, phi, relECALIso, relHCALIso, relTrkIso) = (%.1lf, %.3lf, %.3lf, %.3lf, %.3lf, %.3lf) --> failed to pass the filter", obj.pt, obj.eta, obj.phi, relECALIso, relHCALIso, relTrkIso) << endl;
      //     }
      //     else
      //       logEvent << TString::Format("  (pt, eta, phi, relECALIso, relHCALIso, relTrkIso) = (%.1lf, %.3lf, %.3lf, corresponding L3 is not found)", obj.pt, obj.eta, obj.phi) << endl;
      //   }
      //   logEvent << "===============================\n" << endl;
      // }

    } // -- end of event iteration

    TFile *f_output = TFile::Open(outputFileName_, "RECREATE");

    h_nIsoMu24Obj->Write();
    h_IsoMu24Obj_pt->Write();
    h_IsoMu24Obj_eta->Write();
    h_IsoMu24Obj_phi->Write();


    h_nMu24Obj->Write();
    h_Mu24Obj_pt->Write();
    h_Mu24Obj_eta->Write();
    h_Mu24Obj_phi->Write();

    h_Mu24Obj_ECALIso_EE->Write();
    h_Mu24Obj_ECALIso_EB->Write();
    h_Mu24Obj_relECALIso_EE->Write(); 
    h_Mu24Obj_relECALIso_EB->Write();

    h_Mu24Obj_HCALIso_EE->Write(); 
    h_Mu24Obj_HCALIso_EB->Write();
    h_Mu24Obj_relHCALIso_EE->Write(); 
    h_Mu24Obj_relHCALIso_EB->Write();

    h_Mu24Obj_trkIso_EE->Write(); 
    h_Mu24Obj_trkIso_EB->Write();
    h_Mu24Obj_relTrkIso_EE->Write(); 
    h_Mu24Obj_relTrkIso_EB->Write();


    h_nMu24Obj_IsoF->Write();
    h_Mu24Obj_IsoF_pt->Write();
    h_Mu24Obj_IsoF_eta->Write();
    h_Mu24Obj_IsoF_phi->Write();

    h_Mu24Obj_IsoF_ECALIso_EE->Write();
    h_Mu24Obj_IsoF_ECALIso_EB->Write();
    h_Mu24Obj_IsoF_relECALIso_EE->Write(); 
    h_Mu24Obj_IsoF_relECALIso_EB->Write();

    h_Mu24Obj_IsoF_HCALIso_EE->Write(); 
    h_Mu24Obj_IsoF_HCALIso_EB->Write();
    h_Mu24Obj_IsoF_relHCALIso_EE->Write(); 
    h_Mu24Obj_IsoF_relHCALIso_EB->Write();

    h_Mu24Obj_IsoF_trkIso_EE->Write(); 
    h_Mu24Obj_IsoF_trkIso_EB->Write();
    h_Mu24Obj_IsoF_relTrkIso_EE->Write(); 
    h_Mu24Obj_IsoF_relTrkIso_EB->Write();

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
  TString outputFileName = TString::Format("ROOTFile_MakeHist_Validation_CustomIsoFilter_%s.root", tag.Data());

  HistProducer* histProducer = new HistProducer();
  histProducer->Set_OutputFileName(outputFileName);
  histProducer->Set_ntupleListFile(textFile_ntupleList);

  histProducer->Produce();
}
