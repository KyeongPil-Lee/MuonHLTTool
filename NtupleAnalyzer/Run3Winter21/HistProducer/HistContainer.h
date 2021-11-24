#include <Include/MuonHLTTool.h>

#include <TStopwatch.h>
#include <TSystem.h>

class HistContainer
{
public:
  HistContainer(TString type = "")
  {
    type_ = type;
    Init_Hist();
  }

  void Fill_Event(MuonHLT::NtupleHandle* ntuple, Double_t weight)
  {
    h_rho_ECAL_->Fill( ntuple->rho_ECAL, weight );
    h_rho_HCAL_->Fill( ntuple->rho_HCAL, weight );

    // -- if at least one fiinal trigger object exists: trigger is fired
    vector<MuonHLT::MYHLTObject> vec_MYHLTObj_IsoMu24 = MuonHLT::GetAllMYHLTObject(ntuple, "hltL3crIsoL1sSingleMu22L1f0L2f10QL3f24QL3trkIsoFiltered0p07::MYHLT");
    if( vec_MYHLTObj_IsoMu24.size() > 0 ) h_IsoMu24_->Fill( 1.5, weight );
    else                                  h_IsoMu24_->Fill( 0.5, weight );
  }

  void Fill_Mu24Obj(MuonHLT::MYHLTObject Mu24Obj, MuonHLT::NtupleHandle* ntuple, Double_t weight)
  {
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

      h_Mu24Obj_relECALIso_low_fineBin_EB_->Fill( Mu24Obj.relECALIso, weight );
      h_Mu24Obj_relECALIso_high_fineBin_EB_->Fill( Mu24Obj.relECALIso, weight );
    }
    else // -- endcap
    {
      h_Mu24Obj_ECALIso_EE_->Fill( Mu24Obj.ECALIso, weight );
      h_Mu24Obj_HCALIso_EE_->Fill( Mu24Obj.HCALIso, weight );
      h_Mu24Obj_trkIso_EE_->Fill( Mu24Obj.trkIso, weight );

      h_Mu24Obj_relECALIso_EE_->Fill( Mu24Obj.relECALIso, weight );
      h_Mu24Obj_relHCALIso_EE_->Fill( Mu24Obj.relHCALIso, weight );
      h_Mu24Obj_relTrkIso_EE_->Fill( Mu24Obj.relTrkIso, weight );

      h_Mu24Obj_relECALIso_low_fineBin_EE_->Fill( Mu24Obj.relECALIso, weight );
      h_Mu24Obj_relECALIso_high_fineBin_EE_->Fill( Mu24Obj.relECALIso, weight );
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

        h_Mu24Obj_passECAL_relHCALIso_low_fineBin_EB_->Fill( Mu24Obj.relHCALIso, weight );
        h_Mu24Obj_passECAL_relHCALIso_high_fineBin_EB_->Fill( Mu24Obj.relHCALIso, weight );
      }
      else // -- endcap
      {
        h_Mu24Obj_passECAL_HCALIso_EE_->Fill( Mu24Obj.HCALIso, weight );
        h_Mu24Obj_passECAL_relHCALIso_EE_->Fill( Mu24Obj.relHCALIso, weight );

        h_Mu24Obj_passECAL_relHCALIso_low_fineBin_EE_->Fill( Mu24Obj.relHCALIso, weight );
        h_Mu24Obj_passECAL_relHCALIso_high_fineBin_EE_->Fill( Mu24Obj.relHCALIso, weight );
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

        h_Mu24Obj_passEHCAL_relTrkIso_low_fineBin_->Fill( Mu24Obj.relTrkIso, weight );
        h_Mu24Obj_passEHCAL_relTrkIso_high_fineBin_->Fill( Mu24Obj.relTrkIso, weight );
      } // -- end of HCAL isolation cut

    } // -- end of ECAL isolation cut
  }

  void Save()
  {
    for(auto& h : vec_hist_ )
      h->Write();
  }

private:
  TString type_ = "";

  vector<TH1D*> vec_hist_;

  //////////////////////////
  // -- event by event -- //
  //////////////////////////
  // -- rho information
  TH1D* h_rho_ECAL_;
  TH1D* h_rho_HCAL_;

  // -- is IsoMu24 fired
  TH1D* h_IsoMu24_;

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
  TH1D* h_Mu24Obj_relECALIso_low_fineBin_EB_;
  TH1D* h_Mu24Obj_relECALIso_low_fineBin_EE_;

  TH1D* h_Mu24Obj_relECALIso_high_fineBin_EB_;
  TH1D* h_Mu24Obj_relECALIso_high_fineBin_EE_;

  TH1D* h_Mu24Obj_passECAL_relHCALIso_low_fineBin_EB_;
  TH1D* h_Mu24Obj_passECAL_relHCALIso_low_fineBin_EE_;

  TH1D* h_Mu24Obj_passECAL_relHCALIso_high_fineBin_EB_;
  TH1D* h_Mu24Obj_passECAL_relHCALIso_high_fineBin_EE_;

  // -- no separation for tracker isolation
  TH1D* h_Mu24Obj_passEHCAL_relTrkIso_low_fineBin_;
  TH1D* h_Mu24Obj_passEHCAL_relTrkIso_high_fineBin_;


  void Init_Hist()
  {
    h_rho_ECAL_ = new TH1D("h_rho_ECAL", "", 10000, 0, 1000); vec_hist_.push_back( h_rho_ECAL_ );
    h_rho_HCAL_ = new TH1D("h_rho_HCAL", "", 10000, 0, 1000); vec_hist_.push_back( h_rho_HCAL_ );

    h_IsoMu24_ = new TH1D("h_IsoMu24", "", 2, 0, 2); vec_hist_.push_back( h_IsoMu24_ );

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


    h_Mu24Obj_relECALIso_low_fineBin_EB_ = new TH1D("h_Mu24Obj_relECALIso_low_fineBin_EB", "", 110000, -1, 0.1); vec_hist_.push_back( h_Mu24Obj_relECALIso_low_fineBin_EB_ );
    h_Mu24Obj_relECALIso_low_fineBin_EE_ = new TH1D("h_Mu24Obj_relECALIso_low_fineBin_EE", "", 110000, -1, 0.1); vec_hist_.push_back( h_Mu24Obj_relECALIso_low_fineBin_EE_ );

    h_Mu24Obj_relECALIso_high_fineBin_EB_ = new TH1D("h_Mu24Obj_relECALIso_high_fineBin_EB", "", 9000, 0.1, 1); vec_hist_.push_back( h_Mu24Obj_relECALIso_high_fineBin_EB_ );
    h_Mu24Obj_relECALIso_high_fineBin_EE_ = new TH1D("h_Mu24Obj_relECALIso_high_fineBin_EE", "", 9000, 0.1, 1); vec_hist_.push_back( h_Mu24Obj_relECALIso_high_fineBin_EE_ );

    h_Mu24Obj_passECAL_relHCALIso_low_fineBin_EB_ = new TH1D("h_Mu24Obj_passECAL_relHCALIso_low_fineBin_EB", "", 110000, -1, 0.1); vec_hist_.push_back( h_Mu24Obj_passECAL_relHCALIso_low_fineBin_EB_ );
    h_Mu24Obj_passECAL_relHCALIso_low_fineBin_EE_ = new TH1D("h_Mu24Obj_passECAL_relHCALIso_low_fineBin_EE", "", 110000, -1, 0.1); vec_hist_.push_back( h_Mu24Obj_passECAL_relHCALIso_low_fineBin_EE_ );

    h_Mu24Obj_passECAL_relHCALIso_high_fineBin_EB_ = new TH1D("h_Mu24Obj_passECAL_relHCALIso_high_fineBin_EB", "", 9000, 0.1, 1); vec_hist_.push_back( h_Mu24Obj_passECAL_relHCALIso_high_fineBin_EB_ );
    h_Mu24Obj_passECAL_relHCALIso_high_fineBin_EE_ = new TH1D("h_Mu24Obj_passECAL_relHCALIso_high_fineBin_EE", "", 9000, 0.1, 1); vec_hist_.push_back( h_Mu24Obj_passECAL_relHCALIso_high_fineBin_EE_ );

    h_Mu24Obj_passEHCAL_relTrkIso_low_fineBin_  = new TH1D("h_Mu24Obj_passEHCAL_relTrkIso_low_fineBin", "", 20000, -0.1, 0.1); vec_hist_.push_back( h_Mu24Obj_passEHCAL_relTrkIso_low_fineBin_ );
    h_Mu24Obj_passEHCAL_relTrkIso_high_fineBin_ = new TH1D("h_Mu24Obj_passEHCAL_relTrkIso_high_fineBin", "", 9000, 0.1, 1);    vec_hist_.push_back( h_Mu24Obj_passEHCAL_relTrkIso_high_fineBin_ );

    for(auto& h : vec_hist_ )
    {
      // -- add "type" at the end of the histogram name
      if( type_ != "" )
      {
        TString histName_before = h->GetName();
        TString histName_after = histName_before+"_"+type_;
        h->SetName(histName_after);
      }
    }

  }

};