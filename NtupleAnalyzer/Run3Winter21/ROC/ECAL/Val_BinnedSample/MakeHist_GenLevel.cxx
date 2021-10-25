#include <TStopwatch.h>

#include <TagProbe/TnPTool.h>

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
    Init_Hist();

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

      Double_t genWeight = ntuple->isRealData? 1.0 : ntuple->genEventWeight;
      Double_t totWeight = normFactor * genWeight;

      if( sampleType.Contains("ZMuMu") ) // -- signal sample
      {
        vector<MuonHLT::GenParticle> vec_genMuon_HPFS = GetAllGenLeptons(ntuple, 13, "fromHardProcessFinalState");
        if( vec_genMuon_HPFS.size() != 2 )
        {
          cout << "# gen muons with fromHardProcessFinalState in ZMuMu samples = " << vec_genMuon_HPFS.size() << "is not 2 ... need to check" << endl;
          return;
        }

        // -- fill HPFS muons
        for(auto& genMu : vec_genMuon_HPFS )
          Fill_DoubleMu( vec_genMuon_HPFS[0], vec_genMuon_HPFS[1], totWeight );

        vector<MuonHLT::GenParticle> vec_genMuon_HP = GetAllGenLeptons(ntuple, 13, "isHardProcess");
        if( vec_genMuon_HP.size() != 2 )
        {
          cout << "# gen muons with isHardProcess in ZMuMu samples = " << vec_genMuon_HP.size() << "is not 2 ... need to check" << endl;
          return;
        }

        // -- dimuon variable: HP (especially for the dimuon mass)
        for(auto& genMu : vec_genMuon_HP )
          Fill_HP( vec_genMuon_HP[0], vec_genMuon_HP[1], totWeight );
      }
      else // -- others e.g. QCD
      {
        // -- put all muons with status = 1 (final state particle)
        for(Int_t i_gen=0; i_gen<ntuple->nGenParticle; i_gen++)
        {
          MuonHLT::GenParticle genMu( ntuple, i_gen );
          if( genMu.status == 1 && abs(genMu.ID) == 13 )
            Fill_SingleMu( genMu, totWeight );
        }
      } // -- end of else sampleType.Contains("ZMuMu")

    } // -- end of event iteration

    TFile *f_output = TFile::Open(outputFileName_, "RECREATE");
    Save();
    f_output->Close();

    delete ntuple;
    delete chain;

    PrintRunTime();
  }

private:
  TStopwatch timer_;

  TString outputFileName_;

  TString fileName_ntupleList_ = "";

  vector<TH1D*> vec_hist_;
  TH1D* h_gen_mu_pt_;
  TH1D* h_gen_mu_eta_;
  TH1D* h_gen_mu_phi_;

  TH1D* h_gen_mu_lead_pt_;
  TH1D* h_gen_mu_lead_eta_;
  TH1D* h_gen_mu_lead_phi_;

  TH1D* h_gen_mu_sub_pt_;
  TH1D* h_gen_mu_sub_eta_;
  TH1D* h_gen_mu_sub_phi_;

  TH1D* h_gen_isHP_diMu_mass_;
  TH1D* h_gen_isHP_diMu_pt_;
  TH1D* h_gen_isHP_diMu_rap_;

  void Init_Hist()
  {
    vec_hist_.clear();

    h_gen_mu_pt_  = new TH1D("h_gen_mu_pt", "", 10000, 0, 10000); vec_hist_.push_back( h_gen_mu_pt_ );
    h_gen_mu_eta_ = new TH1D("h_gen_mu_eta", "", 400, -20, 20);   vec_hist_.push_back( h_gen_mu_eta_ );
    h_gen_mu_phi_ = new TH1D("h_gen_mu_phi", "", 80, -4, 4);      vec_hist_.push_back( h_gen_mu_phi_ );

    h_gen_mu_lead_pt_  = new TH1D("h_gen_mu_lead_pt", "", 10000, 0, 10000); vec_hist_.push_back( h_gen_mu_lead_pt_ );
    h_gen_mu_lead_eta_ = new TH1D("h_gen_mu_lead_eta", "", 400, -20, 20);   vec_hist_.push_back( h_gen_mu_lead_eta_ );
    h_gen_mu_lead_phi_ = new TH1D("h_gen_mu_lead_phi", "", 80, -4, 4);      vec_hist_.push_back( h_gen_mu_lead_phi_ );

    h_gen_mu_sub_pt_  = new TH1D("h_gen_mu_sub_pt", "", 10000, 0, 10000); vec_hist_.push_back( h_gen_mu_sub_pt_ );
    h_gen_mu_sub_eta_ = new TH1D("h_gen_mu_sub_eta", "", 400, -20, 20);   vec_hist_.push_back( h_gen_mu_sub_eta_ );
    h_gen_mu_sub_phi_ = new TH1D("h_gen_mu_sub_phi", "", 80, -4, 4);      vec_hist_.push_back( h_gen_mu_sub_phi_ );

    h_gen_isHP_diMu_mass_ = new TH1D("h_gen_isHP_diMu_mass", "", 10000, 0, 10000); vec_hist_.push_back( h_gen_isHP_diMu_mass_ );
    h_gen_isHP_diMu_pt_   = new TH1D("h_gen_isHP_diMu_pt", "", 10000, 0, 10000);   vec_hist_.push_back( h_gen_isHP_diMu_pt_ );
    h_gen_isHP_diMu_rap_  = new TH1D("h_gen_isHP_diMu_rap", "", 400, -20, 20);     vec_hist_.push_back( h_gen_isHP_diMu_rap_ );
  }

  void Fill_SingleMu(MuonHLT::GenParticle genMu, Double_t weight)
  {
    h_gen_mu_pt_->Fill( genMu.pt, weight );
    h_gen_mu_eta_->Fill( genMu.eta, weight );
    h_gen_mu_phi_->Fill( genMu.phi, weight );
  }

  void Fill_DoubleMu(MuonHLT::GenParticle genMu1, MuonHLT::GenParticle genMu2, Double_t weight)
  {
    Fill_SingleMu( genMu1, weight );
    Fill_SingleMu( genMu2, weight );

    if( genMu1.pt > genMu2.pt )
    {
      h_gen_mu_lead_pt_->Fill( genMu1.pt );
      h_gen_mu_lead_eta_->Fill( genMu1.eta );
      h_gen_mu_lead_phi_->Fill( genMu1.phi );

      h_gen_mu_sub_pt_->Fill( genMu2.pt );
      h_gen_mu_sub_eta_->Fill( genMu2.eta );
      h_gen_mu_sub_phi_->Fill( genMu2.phi );
    }
    else
    {
      h_gen_mu_lead_pt_->Fill( genMu2.pt );
      h_gen_mu_lead_eta_->Fill( genMu2.eta );
      h_gen_mu_lead_phi_->Fill( genMu2.phi );

      h_gen_mu_sub_pt_->Fill( genMu1.pt );
      h_gen_mu_sub_eta_->Fill( genMu1.eta );
      h_gen_mu_sub_phi_->Fill( genMu1.phi );
    }
  }

  void Fill_HP(MuonHLT::GenParticle genMu1, MuonHLT::GenParticle genMu2, Double_t weight )
  {
    TLorentzVector vecP_diMu = (genMu1.vecP + genMu2.vecP);
    h_gen_isHP_diMu_mass_->Fill( vecP_diMu.M(), weight );
    h_gen_isHP_diMu_pt_->Fill( vecP_diMu.Pt(), weight );
    h_gen_isHP_diMu_rap_->Fill( vecP_diMu.Rapidity(), weight );
  }

  void Save()
  {
    for(auto& h : vec_hist_ )
      h->Write();
  }

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


void MakeHist_GenLevel(TString textFile_ntupleList)
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
