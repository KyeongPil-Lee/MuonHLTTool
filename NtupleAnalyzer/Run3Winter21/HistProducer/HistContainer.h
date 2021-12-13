#include <Include/MuonHLTTool.h>
#include <Run3Winter21/HistProducer/Utility_TnPHist.h>

#include <TStopwatch.h>
#include <TSystem.h>

class HistContainer
{
public:
  HistContainer(TString tag)
  {
    tag_ = tag;
  }

  HistContainer(TString tag, TString sampleType)
  {
    tag_ = tag;
    Set_SampleType(sampleType);
  }

  void Set_NewWP_ReadFromUtil( Bool_t flag = kTRUE ) { 
    newWP_readFromUtil_ = flag;
    if( newWP_readFromUtil_ ) {
      cout << "[HistContainer] New WP values (only used for iso. histograms, not TnP) are read from Utility_NewWP.h..." << endl;
      // -- value: defined in Utility_NewWP.h
      WP_new_ECAL_EB_ = MuonHLT::WP_new_ECAL_EB;
      WP_new_ECAL_EE_ = MuonHLT::WP_new_ECAL_EE;

      WP_new_HCAL_EB_ = MuonHLT::WP_new_HCAL_EB;
      WP_new_HCAL_EE_ = MuonHLT::WP_new_HCAL_EE;
    }
    else {
      cout << "[HistContainer] New WP values (only used for iso. histograms, not TnP) are should be provided by hand" << endl;
    }

  }

  void Set_NewWP_ECAL( Double_t WP_EB, Double_t WP_EE ) {
    if( newWP_readFromUtil_ )
      cout << "[HistContainer::Set_NewWP_ECAL] new WP values were already read from Utility_NewWP.h... they will be overwritten" << endl;

    WP_new_ECAL_EB_ = WP_EB;
    WP_new_ECAL_EE_ = WP_EE;
  }

  void Set_NewWP_HCAL( Double_t WP_EB, Double_t WP_EE ) {
    if( newWP_readFromUtil_ )
      cout << "[HistContainer::Set_NewWP_HCAL] new WP values were already read from Utility_NewWP.h... they will be overwritten" << endl;

    WP_new_HCAL_EB_ = WP_EB;
    WP_new_HCAL_EE_ = WP_EE;
  }

  void Set_SampleType( TString type ) { 
    sampleType_ = type;

    if( sampleType_.Contains("ZMuMu_M50to120") || sampleType_.Contains("DYLL_M50") ) isDY_M50_ = kTRUE;
    if( sampleType_.Contains("QCD") )                                                isQCD_    = kTRUE;
  }

  void Set_GenMatchingForDYSample( Bool_t flag = kTRUE) { doGenMatchingForDY_ = flag; }

  void Set_MinPt_ForTnPEff(Double_t minPt ) { minPt_ = minPt; }

  void Set_ProduceTnPHist( Bool_t flag = kTRUE ) { produceTnPHist_ = flag; }

  // -- should be called before filling the histograms
  // -- initialze histograms after all conditions are provided by "Set_" functions
  void Initialize() { 
    Init_Hist();
    isInit_ = kTRUE;
  }

  // -- split eventWeight and normFactor: TnP hist doesn't need normalization
  void Fill_Event(MuonHLT::NtupleHandle* ntuple, Double_t eventWeight, Double_t normFactor)
  {
    if( !isInit_ ) {
      cout << "HistContainer::Initialize is not called before filling the histograms! return" << endl;
      return;
    }

    // -- sum(weight)
    h_sumWeight_->Fill( 0.5, eventWeight );

    Double_t weight = eventWeight*normFactor;

    h_rho_ECAL_->Fill( ntuple->rho_ECAL, weight );
    h_rho_HCAL_->Fill( ntuple->rho_HCAL, weight );

    // -- if at least one fiinal trigger object exists: trigger is fired
    vector<MuonHLT::MYHLTObject> vec_MYHLTObj_IsoMu24 = MuonHLT::GetAllMYHLTObject(ntuple, "hltL3crIsoL1sSingleMu22L1f0L2f10QL3f24QL3trkIsoFiltered0p07::MYHLT");
    if( vec_MYHLTObj_IsoMu24.size() > 0 ) h_IsoMu24_->Fill( 1.5, weight );
    else                                  h_IsoMu24_->Fill( 0.5, weight );

    // -- check isolation cut one by one;
    vector<MuonHLT::MYHLTObject> vec_MYHLTObj_IsoMu24_oldWP = MuonHLT::GetAllMYHLTObj_Mu24_PassIso_OldWP(ntuple);
    if( vec_MYHLTObj_IsoMu24_oldWP.size() > 0 ) h_IsoMu24_oldWP_->Fill( 1.5, weight );
    else                                        h_IsoMu24_oldWP_->Fill( 0.5, weight );

    vector<MuonHLT::MYHLTObject> vec_MYHLTObj_IsoMu24_newWP = MuonHLT::GetAllMYHLTObj_Mu24_PassIso_NewWP(ntuple);
    if( vec_MYHLTObj_IsoMu24_newWP.size() > 0 ) h_IsoMu24_newWP_->Fill( 1.5, weight );
    else                                        h_IsoMu24_newWP_->Fill( 0.5, weight );

    // -- use eventWeight instead of eventWeight*normFactor
    if( produceTnPHist_ && isDY_M50_ ) {
      Fill_TnPHist<TnPTool::TnPPair_FullIsoOverMu24_OldWP>(ntuple, eventWeight, tnpHist_IsoOverL3_oldWP_);
      Fill_TnPHist<TnPTool::TnPPair_FullIsoOverMu24_NewWP>(ntuple, eventWeight, tnpHist_IsoOverL3_newWP_);
    }
  }

  void Fill_Mu24Obj(MuonHLT::MYHLTObject Mu24Obj, MuonHLT::NtupleHandle* ntuple, Double_t weight)
  {
    if( !isInit_ ) {
      cout << "HistContainer::Initialize is not called before filling the histograms! return" << endl;
      return;
    }

    if( isDY_M50_ && doGenMatchingForDY_ ) { // -- DY sample: check gen matching; only the object matched to a gen lepton will be filled
      if( !GenMatching(Mu24Obj.vecP, ntuple) ) return;
    }

    if( !Mu24Obj.isIsoValid ) Mu24Obj.FillIsolationVariable(ntuple);

    h_Mu24Obj_ECALIso_->Fill( Mu24Obj.ECALIso, weight );
    h_Mu24Obj_HCALIso_->Fill( Mu24Obj.HCALIso, weight );
    h_Mu24Obj_trkIso_->Fill( Mu24Obj.trkIso, weight );

    h_Mu24Obj_relECALIso_->Fill( Mu24Obj.relECALIso, weight );
    h_Mu24Obj_relHCALIso_->Fill( Mu24Obj.relHCALIso, weight );
    h_Mu24Obj_relTrkIso_->Fill( Mu24Obj.relTrkIso, weight );

    if( fabs(Mu24Obj.eta) < 1.479 ) // -- barrel
    {
      h_Mu24Obj_ECALIso_EB_->Fill( Mu24Obj.ECALIso, weight );
      h_Mu24Obj_HCALIso_EB_->Fill( Mu24Obj.HCALIso, weight );
      h_Mu24Obj_trkIso_EB_->Fill( Mu24Obj.trkIso, weight );

      h_Mu24Obj_relECALIso_EB_->Fill( Mu24Obj.relECALIso, weight );
      h_Mu24Obj_relHCALIso_EB_->Fill( Mu24Obj.relHCALIso, weight );
      h_Mu24Obj_relTrkIso_EB_->Fill( Mu24Obj.relTrkIso, weight );

      h_Mu24Obj_relECALIso_fineBin_EB_->Fill( Mu24Obj.relECALIso, weight );
    }
    else // -- endcap
    {
      h_Mu24Obj_ECALIso_EE_->Fill( Mu24Obj.ECALIso, weight );
      h_Mu24Obj_HCALIso_EE_->Fill( Mu24Obj.HCALIso, weight );
      h_Mu24Obj_trkIso_EE_->Fill( Mu24Obj.trkIso, weight );

      h_Mu24Obj_relECALIso_EE_->Fill( Mu24Obj.relECALIso, weight );
      h_Mu24Obj_relHCALIso_EE_->Fill( Mu24Obj.relHCALIso, weight );
      h_Mu24Obj_relTrkIso_EE_->Fill( Mu24Obj.relTrkIso, weight );

      h_Mu24Obj_relECALIso_fineBin_EE_->Fill( Mu24Obj.relECALIso, weight );
    }

    // -- if it passes ECAL isolation
    if( MuonHLT::Pass_HLTIsoFilter_ECAL(Mu24Obj) )
    {
      h_Mu24Obj_passECAL_HCALIso_->Fill( Mu24Obj.HCALIso, weight );
      h_Mu24Obj_passECAL_relHCALIso_->Fill( Mu24Obj.relHCALIso, weight );

      if( fabs(Mu24Obj.eta) < 1.479 ) // -- barrel
      {
        h_Mu24Obj_passECAL_HCALIso_EB_->Fill( Mu24Obj.HCALIso, weight );
        h_Mu24Obj_passECAL_relHCALIso_EB_->Fill( Mu24Obj.relHCALIso, weight );

        h_Mu24Obj_passECAL_relHCALIso_fineBin_EB_->Fill( Mu24Obj.relHCALIso, weight );
      }
      else // -- endcap
      {
        h_Mu24Obj_passECAL_HCALIso_EE_->Fill( Mu24Obj.HCALIso, weight );
        h_Mu24Obj_passECAL_relHCALIso_EE_->Fill( Mu24Obj.relHCALIso, weight );

        h_Mu24Obj_passECAL_relHCALIso_fineBin_EE_->Fill( Mu24Obj.relHCALIso, weight );
      }

      // -- if it also passes HCAL isolation
      if( MuonHLT::Pass_HLTIsoFilter_HCAL(Mu24Obj) )
      {
        h_Mu24Obj_passEHCAL_trkIso_->Fill( Mu24Obj.trkIso, weight );
        h_Mu24Obj_passEHCAL_relTrkIso_->Fill( Mu24Obj.relTrkIso, weight );

        if( fabs(Mu24Obj.eta) < 1.479 ) // -- barrel
        {
          h_Mu24Obj_passEHCAL_trkIso_EB_->Fill( Mu24Obj.trkIso, weight );
          h_Mu24Obj_passEHCAL_relTrkIso_EB_->Fill( Mu24Obj.relTrkIso, weight );
        }
        else // -- endcap
        {
          h_Mu24Obj_passEHCAL_trkIso_EE_->Fill( Mu24Obj.trkIso, weight );
          h_Mu24Obj_passEHCAL_relTrkIso_EE_->Fill( Mu24Obj.relTrkIso, weight );
        }

        h_Mu24Obj_passEHCAL_relTrkIso_fineBin_->Fill( Mu24Obj.relTrkIso, weight );
      } // -- end of HCAL isolation cut

    } // -- end of ECAL isolation cut

    // -- if it passes new isolation WP
    if( MuonHLT::Pass_HLTIsoFilter_ECAL(Mu24Obj, WP_new_ECAL_EB_, WP_new_ECAL_EE_) ) {

      h_Mu24Obj_passECAL_newWP_HCALIso_->Fill( Mu24Obj.HCALIso, weight );
      h_Mu24Obj_passECAL_newWP_relHCALIso_->Fill( Mu24Obj.relHCALIso, weight );

      if( fabs(Mu24Obj.eta) < 1.479 ) // -- barrel
      {
        h_Mu24Obj_passECAL_newWP_HCALIso_EB_->Fill( Mu24Obj.HCALIso, weight );
        h_Mu24Obj_passECAL_newWP_relHCALIso_EB_->Fill( Mu24Obj.relHCALIso, weight );

        h_Mu24Obj_passECAL_newWP_relHCALIso_fineBin_EB_->Fill( Mu24Obj.relHCALIso, weight );
      }
      else // -- endcap
      {
        h_Mu24Obj_passECAL_newWP_HCALIso_EE_->Fill( Mu24Obj.HCALIso, weight );
        h_Mu24Obj_passECAL_newWP_relHCALIso_EE_->Fill( Mu24Obj.relHCALIso, weight );

        h_Mu24Obj_passECAL_newWP_relHCALIso_fineBin_EE_->Fill( Mu24Obj.relHCALIso, weight );
      }

      // -- if it also passes HCAL isolation
      if( MuonHLT::Pass_HLTIsoFilter_HCAL(Mu24Obj, WP_new_HCAL_EB_, WP_new_HCAL_EE_) )
      {
        h_Mu24Obj_passEHCAL_newWP_trkIso_->Fill( Mu24Obj.trkIso, weight );
        h_Mu24Obj_passEHCAL_newWP_relTrkIso_->Fill( Mu24Obj.relTrkIso, weight );

        if( fabs(Mu24Obj.eta) < 1.479 ) // -- barrel
        {
          h_Mu24Obj_passEHCAL_newWP_trkIso_EB_->Fill( Mu24Obj.trkIso, weight );
          h_Mu24Obj_passEHCAL_newWP_relTrkIso_EB_->Fill( Mu24Obj.relTrkIso, weight );
        }
        else // -- endcap
        {
          h_Mu24Obj_passEHCAL_newWP_trkIso_EE_->Fill( Mu24Obj.trkIso, weight );
          h_Mu24Obj_passEHCAL_newWP_relTrkIso_EE_->Fill( Mu24Obj.relTrkIso, weight );
        }

        h_Mu24Obj_passEHCAL_newWP_relTrkIso_fineBin_->Fill( Mu24Obj.relTrkIso, weight );
      } // -- end of new HCAL isolation WP
    } // -- end of new ECAL isolation WP
  }

  void Save()
  {
    for(auto& h : vec_hist_ )
      h->Write();

    if( produceTnPHist_ && isDY_M50_ ) {
      for(auto& tnpHist : vec_tnpHist_ )
        tnpHist->Save();
    }
  }

private:
  TString tag_ = "";

  Bool_t newWP_readFromUtil_ = kFALSE;

  Double_t WP_new_ECAL_EB_ = 0;
  Double_t WP_new_ECAL_EE_ = 0;

  Double_t WP_new_HCAL_EB_ = 0;
  Double_t WP_new_HCAL_EE_ = 0;

  TString sampleType_ = "";
  Bool_t isDY_M50_ = kFALSE;
  Bool_t isQCD_ = kFALSE;

  Bool_t doGenMatchingForDY_ = kFALSE;

  Bool_t produceTnPHist_ = kFALSE;
  // -- for TnP efficiency histograms vs. eta, phi, #vtx...
  Double_t minPt_ = -999;

  Bool_t isInit_ = kFALSE;

  vector<TH1D*> vec_hist_;

  //////////////////////////
  // -- event by event -- //
  //////////////////////////
  TH1D* h_sumWeight_;

  // -- rho information
  TH1D* h_rho_ECAL_;
  TH1D* h_rho_HCAL_;

  // -- is IsoMu24 fired
  TH1D* h_IsoMu24_;
  TH1D* h_IsoMu24_oldWP_;
  TH1D* h_IsoMu24_newWP_;

  //////////////////////////
  // -- muon by muon   -- //
  //////////////////////////
  TH1D* h_Mu24Obj_ECALIso_;
  TH1D* h_Mu24Obj_ECALIso_EB_;
  TH1D* h_Mu24Obj_ECALIso_EE_;

  TH1D* h_Mu24Obj_relECALIso_;
  TH1D* h_Mu24Obj_relECALIso_EB_;
  TH1D* h_Mu24Obj_relECALIso_EE_;

  TH1D* h_Mu24Obj_HCALIso_;
  TH1D* h_Mu24Obj_HCALIso_EB_;
  TH1D* h_Mu24Obj_HCALIso_EE_;

  TH1D* h_Mu24Obj_relHCALIso_;
  TH1D* h_Mu24Obj_relHCALIso_EB_;
  TH1D* h_Mu24Obj_relHCALIso_EE_;

  TH1D* h_Mu24Obj_trkIso_;
  TH1D* h_Mu24Obj_trkIso_EB_;
  TH1D* h_Mu24Obj_trkIso_EE_;

  TH1D* h_Mu24Obj_relTrkIso_;
  TH1D* h_Mu24Obj_relTrkIso_EB_;
  TH1D* h_Mu24Obj_relTrkIso_EE_;

  // -- the muons passing the previous step
  TH1D* h_Mu24Obj_passECAL_HCALIso_;
  TH1D* h_Mu24Obj_passECAL_HCALIso_EB_;
  TH1D* h_Mu24Obj_passECAL_HCALIso_EE_;

  TH1D* h_Mu24Obj_passECAL_relHCALIso_;
  TH1D* h_Mu24Obj_passECAL_relHCALIso_EB_;
  TH1D* h_Mu24Obj_passECAL_relHCALIso_EE_;

  TH1D* h_Mu24Obj_passEHCAL_trkIso_;
  TH1D* h_Mu24Obj_passEHCAL_trkIso_EB_;
  TH1D* h_Mu24Obj_passEHCAL_trkIso_EE_;

  TH1D* h_Mu24Obj_passEHCAL_relTrkIso_;
  TH1D* h_Mu24Obj_passEHCAL_relTrkIso_EB_;
  TH1D* h_Mu24Obj_passEHCAL_relTrkIso_EE_;


  // -- finer bins for ROC curve
  // -- include minus region: some isolation values can be negative due to rho*effArea correction (iso - rho*effArea)
  TH1D* h_Mu24Obj_relECALIso_fineBin_EB_;
  TH1D* h_Mu24Obj_relECALIso_fineBin_EE_;

  TH1D* h_Mu24Obj_passECAL_relHCALIso_fineBin_EB_;
  TH1D* h_Mu24Obj_passECAL_relHCALIso_fineBin_EE_;

  // -- no separation for tracker isolation
  TH1D* h_Mu24Obj_passEHCAL_relTrkIso_fineBin_;

  // -- histogram passing new WP
  TH1D* h_Mu24Obj_passECAL_newWP_HCALIso_;
  TH1D* h_Mu24Obj_passECAL_newWP_HCALIso_EB_;
  TH1D* h_Mu24Obj_passECAL_newWP_HCALIso_EE_;

  TH1D* h_Mu24Obj_passECAL_newWP_relHCALIso_;
  TH1D* h_Mu24Obj_passECAL_newWP_relHCALIso_EB_;
  TH1D* h_Mu24Obj_passECAL_newWP_relHCALIso_EE_;

  TH1D* h_Mu24Obj_passECAL_newWP_relHCALIso_fineBin_EB_;
  TH1D* h_Mu24Obj_passECAL_newWP_relHCALIso_fineBin_EE_;


  TH1D* h_Mu24Obj_passEHCAL_newWP_trkIso_;
  TH1D* h_Mu24Obj_passEHCAL_newWP_trkIso_EB_;
  TH1D* h_Mu24Obj_passEHCAL_newWP_trkIso_EE_;

  TH1D* h_Mu24Obj_passEHCAL_newWP_relTrkIso_;
  TH1D* h_Mu24Obj_passEHCAL_newWP_relTrkIso_EB_;
  TH1D* h_Mu24Obj_passEHCAL_newWP_relTrkIso_EE_;

  TH1D* h_Mu24Obj_passEHCAL_newWP_relTrkIso_fineBin_;

  vector<MuonHLT::TnPHistProducer*> vec_tnpHist_;
  MuonHLT::TnPHistProducer* tnpHist_IsoOverL3_oldWP_;
  MuonHLT::TnPHistProducer* tnpHist_IsoOverL3_newWP_;  


  void Init_Hist()
  {
    h_sumWeight_ = new TH1D("h_sumWeight", "", 1, 0, 1); vec_hist_.push_back( h_sumWeight_ );

    h_rho_ECAL_ = new TH1D("h_rho_ECAL", "", 10000, 0, 1000); vec_hist_.push_back( h_rho_ECAL_ );
    h_rho_HCAL_ = new TH1D("h_rho_HCAL", "", 10000, 0, 1000); vec_hist_.push_back( h_rho_HCAL_ );

    h_IsoMu24_ = new TH1D("h_IsoMu24", "", 2, 0, 2); vec_hist_.push_back( h_IsoMu24_ );
    h_IsoMu24_oldWP_ = new TH1D("h_IsoMu24_oldWP", "", 2, 0, 2); vec_hist_.push_back( h_IsoMu24_oldWP_ );
    h_IsoMu24_newWP_ = new TH1D("h_IsoMu24_newWP", "", 2, 0, 2); vec_hist_.push_back( h_IsoMu24_newWP_ );

    h_Mu24Obj_ECALIso_ = new TH1D("h_Mu24Obj_ECALIso", "", 20000, -10000, 10000); vec_hist_.push_back( h_Mu24Obj_ECALIso_ );
    h_Mu24Obj_ECALIso_EB_ = new TH1D("h_Mu24Obj_ECALIso_EB", "", 20000, -10000, 10000); vec_hist_.push_back( h_Mu24Obj_ECALIso_EB_ );
    h_Mu24Obj_ECALIso_EE_ = new TH1D("h_Mu24Obj_ECALIso_EE", "", 20000, -10000, 10000); vec_hist_.push_back( h_Mu24Obj_ECALIso_EE_ );

    h_Mu24Obj_relECALIso_ = new TH1D("h_Mu24Obj_relECALIso", "", 20000, -10, 10); vec_hist_.push_back( h_Mu24Obj_relECALIso_ );
    h_Mu24Obj_relECALIso_EB_ = new TH1D("h_Mu24Obj_relECALIso_EB", "", 20000, -10, 10); vec_hist_.push_back( h_Mu24Obj_relECALIso_EB_ );
    h_Mu24Obj_relECALIso_EE_ = new TH1D("h_Mu24Obj_relECALIso_EE", "", 20000, -10, 10); vec_hist_.push_back( h_Mu24Obj_relECALIso_EE_ );

    h_Mu24Obj_HCALIso_ = new TH1D("h_Mu24Obj_HCALIso", "", 20000, -10000, 10000); vec_hist_.push_back( h_Mu24Obj_HCALIso_ );
    h_Mu24Obj_HCALIso_EB_ = new TH1D("h_Mu24Obj_HCALIso_EB", "", 20000, -10000, 10000); vec_hist_.push_back( h_Mu24Obj_HCALIso_EB_ );
    h_Mu24Obj_HCALIso_EE_ = new TH1D("h_Mu24Obj_HCALIso_EE", "", 20000, -10000, 10000); vec_hist_.push_back( h_Mu24Obj_HCALIso_EE_ );

    h_Mu24Obj_relHCALIso_ = new TH1D("h_Mu24Obj_relHCALIso", "", 20000, -10, 10); vec_hist_.push_back( h_Mu24Obj_relHCALIso_ );
    h_Mu24Obj_relHCALIso_EB_ = new TH1D("h_Mu24Obj_relHCALIso_EB", "", 20000, -10, 10); vec_hist_.push_back( h_Mu24Obj_relHCALIso_EB_ );
    h_Mu24Obj_relHCALIso_EE_ = new TH1D("h_Mu24Obj_relHCALIso_EE", "", 20000, -10, 10); vec_hist_.push_back( h_Mu24Obj_relHCALIso_EE_ );

    h_Mu24Obj_trkIso_ = new TH1D("h_Mu24Obj_trkIso", "", 20000, -10000, 10000); vec_hist_.push_back( h_Mu24Obj_trkIso_ );
    h_Mu24Obj_trkIso_EB_ = new TH1D("h_Mu24Obj_trkIso_EB", "", 20000, -10000, 10000); vec_hist_.push_back( h_Mu24Obj_trkIso_EB_ );
    h_Mu24Obj_trkIso_EE_ = new TH1D("h_Mu24Obj_trkIso_EE", "", 20000, -10000, 10000); vec_hist_.push_back( h_Mu24Obj_trkIso_EE_ );

    h_Mu24Obj_relTrkIso_ = new TH1D("h_Mu24Obj_relTrkIso", "", 20000, -10, 10); vec_hist_.push_back( h_Mu24Obj_relTrkIso_ );
    h_Mu24Obj_relTrkIso_EB_ = new TH1D("h_Mu24Obj_relTrkIso_EB", "", 20000, -10, 10); vec_hist_.push_back( h_Mu24Obj_relTrkIso_EB_ );
    h_Mu24Obj_relTrkIso_EE_ = new TH1D("h_Mu24Obj_relTrkIso_EE", "", 20000, -10, 10); vec_hist_.push_back( h_Mu24Obj_relTrkIso_EE_ );


    h_Mu24Obj_passECAL_HCALIso_ = new TH1D("h_Mu24Obj_passECAL_HCALIso", "", 20000, -10000, 10000); vec_hist_.push_back( h_Mu24Obj_passECAL_HCALIso_ );
    h_Mu24Obj_passECAL_HCALIso_EB_ = new TH1D("h_Mu24Obj_passECAL_HCALIso_EB", "", 20000, -10000, 10000); vec_hist_.push_back( h_Mu24Obj_passECAL_HCALIso_EB_ );
    h_Mu24Obj_passECAL_HCALIso_EE_ = new TH1D("h_Mu24Obj_passECAL_HCALIso_EE", "", 20000, -10000, 10000); vec_hist_.push_back( h_Mu24Obj_passECAL_HCALIso_EE_ );

    h_Mu24Obj_passECAL_relHCALIso_ = new TH1D("h_Mu24Obj_passECAL_relHCALIso", "", 20000, -10, 10); vec_hist_.push_back( h_Mu24Obj_passECAL_relHCALIso_ );
    h_Mu24Obj_passECAL_relHCALIso_EB_ = new TH1D("h_Mu24Obj_passECAL_relHCALIso_EB", "", 20000, -10, 10); vec_hist_.push_back( h_Mu24Obj_passECAL_relHCALIso_EB_ );
    h_Mu24Obj_passECAL_relHCALIso_EE_ = new TH1D("h_Mu24Obj_passECAL_relHCALIso_EE", "", 20000, -10, 10); vec_hist_.push_back( h_Mu24Obj_passECAL_relHCALIso_EE_ );

    h_Mu24Obj_passEHCAL_trkIso_ = new TH1D("h_Mu24Obj_passEHCAL_trkIso", "", 20000, -10000, 10000); vec_hist_.push_back( h_Mu24Obj_passEHCAL_trkIso_ );
    h_Mu24Obj_passEHCAL_trkIso_EB_ = new TH1D("h_Mu24Obj_passEHCAL_trkIso_EB", "", 20000, -10000, 10000); vec_hist_.push_back( h_Mu24Obj_passEHCAL_trkIso_EB_ );
    h_Mu24Obj_passEHCAL_trkIso_EE_ = new TH1D("h_Mu24Obj_passEHCAL_trkIso_EE", "", 20000, -10000, 10000); vec_hist_.push_back( h_Mu24Obj_passEHCAL_trkIso_EE_ );

    h_Mu24Obj_passEHCAL_relTrkIso_ = new TH1D("h_Mu24Obj_passEHCAL_relTrkIso", "", 20000, -10, 10); vec_hist_.push_back( h_Mu24Obj_passEHCAL_relTrkIso_ );
    h_Mu24Obj_passEHCAL_relTrkIso_EB_ = new TH1D("h_Mu24Obj_passEHCAL_relTrkIso_EB", "", 20000, -10, 10); vec_hist_.push_back( h_Mu24Obj_passEHCAL_relTrkIso_EB_ );
    h_Mu24Obj_passEHCAL_relTrkIso_EE_ = new TH1D("h_Mu24Obj_passEHCAL_relTrkIso_EE", "", 20000, -10, 10); vec_hist_.push_back( h_Mu24Obj_passEHCAL_relTrkIso_EE_ );

    // -- custom fine isolation binning for ROC calculation
    vector<Double_t> vec_binEdge;
    Fill_BinEdge(vec_binEdge, 30000, -0.2, 0.1, kFALSE);
    Fill_BinEdge(vec_binEdge, 9000,   0.1, 1.0, kTRUE);

    Int_t nBinEdge = (Int_t)vec_binEdge.size();
    Double_t* arr_binEdge = new Double_t[nBinEdge]; // -- dynamic allocation   
    for(Int_t i=0; i<nBinEdge; i++) {
      arr_binEdge[i] = vec_binEdge[i];
      // cout << TString::Format("[%d] %lf", i, arr_binEdge[i]) << endl;
    }
    Int_t nBin = nBinEdge-1; // -- # bin = # bin edge - 1

    h_Mu24Obj_relECALIso_fineBin_EB_ = new TH1D("h_Mu24Obj_relECALIso_fineBin_EB", "", nBin, arr_binEdge); vec_hist_.push_back( h_Mu24Obj_relECALIso_fineBin_EB_ );
    h_Mu24Obj_relECALIso_fineBin_EE_ = new TH1D("h_Mu24Obj_relECALIso_fineBin_EE", "", nBin, arr_binEdge); vec_hist_.push_back( h_Mu24Obj_relECALIso_fineBin_EE_ );

    h_Mu24Obj_passECAL_relHCALIso_fineBin_EB_ = new TH1D("h_Mu24Obj_passECAL_relHCALIso_fineBin_EB", "", nBin, arr_binEdge); vec_hist_.push_back( h_Mu24Obj_passECAL_relHCALIso_fineBin_EB_ );
    h_Mu24Obj_passECAL_relHCALIso_fineBin_EE_ = new TH1D("h_Mu24Obj_passECAL_relHCALIso_fineBin_EE", "", nBin, arr_binEdge); vec_hist_.push_back( h_Mu24Obj_passECAL_relHCALIso_fineBin_EE_ );

    h_Mu24Obj_passEHCAL_relTrkIso_fineBin_ = new TH1D("h_Mu24Obj_passEHCAL_relTrkIso_fineBin", "", nBin, arr_binEdge); vec_hist_.push_back( h_Mu24Obj_passEHCAL_relTrkIso_fineBin_ );


    h_Mu24Obj_passECAL_newWP_HCALIso_ = new TH1D("h_Mu24Obj_passECAL_newWP_HCALIso", "", 20000, -10000, 10000); vec_hist_.push_back( h_Mu24Obj_passECAL_newWP_HCALIso_ );
    h_Mu24Obj_passECAL_newWP_HCALIso_EB_ = new TH1D("h_Mu24Obj_passECAL_newWP_HCALIso_EB", "", 20000, -10000, 10000); vec_hist_.push_back( h_Mu24Obj_passECAL_newWP_HCALIso_EB_ );
    h_Mu24Obj_passECAL_newWP_HCALIso_EE_ = new TH1D("h_Mu24Obj_passECAL_newWP_HCALIso_EE", "", 20000, -10000, 10000); vec_hist_.push_back( h_Mu24Obj_passECAL_newWP_HCALIso_EE_ );

    h_Mu24Obj_passECAL_newWP_relHCALIso_ = new TH1D("h_Mu24Obj_passECAL_newWP_relHCALIso", "", 20000, -10, 10); vec_hist_.push_back( h_Mu24Obj_passECAL_newWP_relHCALIso_ );
    h_Mu24Obj_passECAL_newWP_relHCALIso_EB_ = new TH1D("h_Mu24Obj_passECAL_newWP_relHCALIso_EB", "", 20000, -10, 10); vec_hist_.push_back( h_Mu24Obj_passECAL_newWP_relHCALIso_EB_ );
    h_Mu24Obj_passECAL_newWP_relHCALIso_EE_ = new TH1D("h_Mu24Obj_passECAL_newWP_relHCALIso_EE", "", 20000, -10, 10); vec_hist_.push_back( h_Mu24Obj_passECAL_newWP_relHCALIso_EE_ );

    h_Mu24Obj_passECAL_newWP_relHCALIso_fineBin_EB_ = new TH1D("h_Mu24Obj_passECAL_newWP_relHCALIso_fineBin_EB", "", nBin, arr_binEdge); vec_hist_.push_back( h_Mu24Obj_passECAL_newWP_relHCALIso_fineBin_EB_ );
    h_Mu24Obj_passECAL_newWP_relHCALIso_fineBin_EE_ = new TH1D("h_Mu24Obj_passECAL_newWP_relHCALIso_fineBin_EE", "", nBin, arr_binEdge); vec_hist_.push_back( h_Mu24Obj_passECAL_newWP_relHCALIso_fineBin_EE_ );


    h_Mu24Obj_passEHCAL_newWP_trkIso_ = new TH1D("h_Mu24Obj_passEHCAL_newWP_trkIso", "", 20000, -10000, 10000); vec_hist_.push_back( h_Mu24Obj_passEHCAL_newWP_trkIso_ );
    h_Mu24Obj_passEHCAL_newWP_trkIso_EB_ = new TH1D("h_Mu24Obj_passEHCAL_newWP_trkIso_EB", "", 20000, -10000, 10000); vec_hist_.push_back( h_Mu24Obj_passEHCAL_newWP_trkIso_EB_ );
    h_Mu24Obj_passEHCAL_newWP_trkIso_EE_ = new TH1D("h_Mu24Obj_passEHCAL_newWP_trkIso_EE", "", 20000, -10000, 10000); vec_hist_.push_back( h_Mu24Obj_passEHCAL_newWP_trkIso_EE_ );

    h_Mu24Obj_passEHCAL_newWP_relTrkIso_ = new TH1D("h_Mu24Obj_passEHCAL_newWP_relTrkIso", "", 20000, -10, 10); vec_hist_.push_back( h_Mu24Obj_passEHCAL_newWP_relTrkIso_ );
    h_Mu24Obj_passEHCAL_newWP_relTrkIso_EB_ = new TH1D("h_Mu24Obj_passEHCAL_newWP_relTrkIso_EB", "", 20000, -10, 10); vec_hist_.push_back( h_Mu24Obj_passEHCAL_newWP_relTrkIso_EB_ );
    h_Mu24Obj_passEHCAL_newWP_relTrkIso_EE_ = new TH1D("h_Mu24Obj_passEHCAL_newWP_relTrkIso_EE", "", 20000, -10, 10); vec_hist_.push_back( h_Mu24Obj_passEHCAL_newWP_relTrkIso_EE_ );

    h_Mu24Obj_passEHCAL_newWP_relTrkIso_fineBin_ = new TH1D("h_Mu24Obj_passEHCAL_newWP_relTrkIso_fineBin", "", nBin, arr_binEdge); vec_hist_.push_back( h_Mu24Obj_passEHCAL_newWP_relTrkIso_fineBin_ );

    for(auto& h : vec_hist_ )
    {
      // -- add "type" at the end of the histogram name
      if( tag_ != "" )
      {
        TString histName_before = h->GetName();
        TString histName_after = histName_before+"_"+tag_;
        h->SetName(histName_after);
      }
    }

    // cout << "[HistContainer::Init_Hist] produceTnPHist_ = " << produceTnPHist_ << endl;
    // cout << "[HistContainer::Init_Hist] isDY_M50_ =           " << isDY_M50_ << endl;

    // -- TnPHist: only needed for DY sample
    if( produceTnPHist_ && isDY_M50_ ) {
      if( tag_ == "" ) {
        tnpHist_IsoOverL3_oldWP_ = new MuonHLT::TnPHistProducer("oldWP", minPt_);
        tnpHist_IsoOverL3_newWP_ = new MuonHLT::TnPHistProducer("newWP", minPt_);
      }
      else {
        tnpHist_IsoOverL3_oldWP_ = new MuonHLT::TnPHistProducer("oldWP_"+tag_, minPt_);
        tnpHist_IsoOverL3_newWP_ = new MuonHLT::TnPHistProducer("newWP_"+tag_, minPt_);
      }

      vec_tnpHist_.push_back( tnpHist_IsoOverL3_oldWP_ );
      vec_tnpHist_.push_back( tnpHist_IsoOverL3_newWP_ );
    }
  }

  void Fill_BinEdge(vector<Double_t> &vec_binEdge, Double_t nBin, Double_t min, Double_t max, Bool_t isLast = kTRUE)
  {
    Double_t step = (max - min) / nBin;
    for(Int_t i=0; i<nBin; i++)
      vec_binEdge.push_back( min + step*i );

    if( isLast )
      vec_binEdge.push_back( max );
  }

  Bool_t GenMatching(TLorentzVector vecP_target, MuonHLT::NtupleHandle* ntuple) {

    vector<MuonHLT::GenParticle> vec_genMuon_HPFS = MuonHLT::GetAllGenLeptons(ntuple, 13, "fromHardProcessFinalState");
    vector<TLorentzVector> vec_vecP_genMuon;
    for(auto& genMuon : vec_genMuon_HPFS )
      vec_vecP_genMuon.push_back( genMuon.vecP );

    Double_t dRCut = 0.3;

    return MuonHLT::dRMatching( vecP_target, vec_vecP_genMuon, dRCut);
  }

  template <class TnPPairTemp>
  void Fill_TnPHist(MuonHLT::NtupleHandle* ntuple, Double_t weight, MuonHLT::TnPHistProducer* tnpHist) {

    // cout << "  [Fill_TnPHist] start..." << endl;
    // -- make tag&pobe pair
    for(Int_t i_tagCand=0; i_tagCand<ntuple->nMuon; i_tagCand++)
    {
      MuonHLT::Muon mu_tagCand( ntuple, i_tagCand );
      // cout << "    consider " << i_tagCand << "th muon" << endl;

      // -- tnpPair_test: class to use IsTag() and IsProbe() earlier than making full TnP pair candidate
      TnPPairTemp *tnpPair_test = new TnPPairTemp();
      if( !tnpPair_test->IsTag(mu_tagCand, ntuple) ) continue; // -- check here to save runtime

      // cout << "    pass tag condition" << endl;

      // -- collect the probes sharing same tag
      vector<TnPPairTemp*> vec_tnpPairs_sameTag;

      for(Int_t i_probeCand=0; i_probeCand<ntuple->nMuon; i_probeCand++)
      {
        // -- remove the case when tag muon == probe muon
        if( i_tagCand == i_probeCand ) continue;

        MuonHLT::Muon mu_probeCand( ntuple, i_probeCand );

        // cout << "      check " << i_probeCand << "th muon as probe" << endl;

        if( !tnpPair_test->IsProbe(mu_probeCand, ntuple) ) continue; // -- check here to save runtime

        // cout << "      pass the probe condition" << endl;

        // -- make the TnP pair candidate
        TnPPairTemp *tnpPair_ij = new TnPPairTemp( mu_tagCand, mu_probeCand, ntuple );
        if( tnpPair_ij->IsValid() ) {
          // cout << "      pass the tag & probe pair condition" << endl;
          vec_tnpPairs_sameTag.push_back( tnpPair_ij );
        }
        else {
          // cout << "      fail to pass the tag & probe pair condition" << endl;
          delete tnpPair_ij;
        }
      } // -- end of iteration for the probe candidate

      // -- fill TnP histogram only when probeMultiplicity == 1
      if( (Int_t)vec_tnpPairs_sameTag.size() == 1 )  {
        // cout << "      --->fill the tnpHist" << endl;
        tnpHist->Fill( static_cast<MuonHLT::TnPPairBase*>(vec_tnpPairs_sameTag[0]), weight );
      }
      // else {
      //   cout << "      --->multiple pairs on same tag: skip" << endl;
      // }

      for( auto tnpPair : vec_tnpPairs_sameTag )
        delete tnpPair;

      delete tnpPair_test;
    } // -- end of iteration for tag candidate
  }

};