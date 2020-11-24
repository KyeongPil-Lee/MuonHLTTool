#include <TChain.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TGraph.h>
#include <TLorentzVector.h>
#include <TStopwatch.h>
#include <TTimeStamp.h>
#include <TString.h>
#include <TLegend.h>
#include <THStack.h>
#include <TPad.h>
#include <TCanvas.h>
#include <TColor.h>
#include <TAttMarker.h>
#include <TF1.h>
#include <TStyle.h>
#include <TROOT.h>
#include <TApplication.h>
#include <vector>
#include <TMath.h>
#include <TSystem.h>

#include <Include/NtupleHandle.h>
#include <Include/Object.h>

#include <iostream>
#include <fstream>

using namespace MuonHLT;

const Int_t nPtBin = 24;
const double arr_ptBinEdge[25]  = { 0, 5, 10, 20, 24, 27, 30, 35, 40, 45, 50, 55, 60, 65, 70, 80, 90, 100, 120, 150, 200, 250, 500, 1000, 10000 };
const Int_t nEtaBin = 15;
const double arr_etaBinEdge[16] = {-5, -2.4, -2.1, -1.6, -1.2, -0.9, -0.3, -0.2,  0.2, 0.3, 0.9, 1.2, 1.6, 2.1, 2.4, 5};


class HistContainer
{
public:
  // -- type
  // -- nonMatched:
  // ---- nonMuon: not matched to any reco muon (h_matchedRecoMu_* are not filled)
  // ---- matchedToReco: matched to a reco muon failing to pass the loose ID
  // -- nonIsolated: matched to nonIsolated reco muon (only passing loose ID)
  // -- isolated: matched to isolated reco muon (passing loose ID and isolation)
  TString type_;

  vector<TH1D*> vec_hist_;
  TH1D* h_trigObj_pt_;
  TH1D* h_trigObj_eta_;
  TH1D* h_trigObj_phi_;
  TH1D* h_trigObj_quality_;
  TH1D* h_trigObj_pt_barrel_;
  TH1D* h_trigObj_pt_overlap_;
  TH1D* h_trigObj_pt_endcap_;

  TH1D* h_matchedRecoMu_pt_;
  TH1D* h_matchedRecoMu_eta_;
  TH1D* h_matchedRecoMu_phi_;
  TH1D* h_matchedRecoMu_pt_barrel_;
  TH1D* h_matchedRecoMu_pt_overlap_;
  TH1D* h_matchedRecoMu_pt_endcap_;
  TH1D* h_matchedRecoMu_dxyVTX_;
  TH1D* h_matchedRecoMu_dzVTX_;


  HistContainer(TString type)
  {
    type_ = type;
    Init();
  }

  void Fill( HLTObject *trigObj, Double_t quality, Muon *mu, Double_t weight)
  {
    h_trigObj_pt_->Fill( trigObj->pt, weight);
    h_trigObj_eta_->Fill( trigObj->eta, weight);
    h_trigObj_phi_->Fill( trigObj->phi, weight);
    h_trigObj_quality_->Fill( quality, weight); // -- only meaningful for L1 muon only; -1 for the other trigger objects

    if( fabs(trigObj->eta) < 0.9 )                             h_trigObj_pt_barrel_->Fill( trigObj->pt, weight);
    if( 0.9 < fabs(trigObj->eta) && fabs(trigObj->eta) < 1.2 ) h_trigObj_pt_overlap_->Fill( trigObj->pt, weight);
    if( fabs(trigObj->eta) > 1.2 )                             h_trigObj_pt_endcap_->Fill( trigObj->pt, weight);

    // -- only if there is a matched reco (offline) muon
    if( mu != nullptr )
    {
      h_matchedRecoMu_pt_->Fill( mu->pt, weight);
      h_matchedRecoMu_eta_->Fill( mu->eta, weight);
      h_matchedRecoMu_phi_->Fill( mu->phi, weight);

      if( fabs(mu->eta) < 0.9 )                        h_matchedRecoMu_pt_barrel_->Fill( mu->pt, weight);
      if( 0.9 < fabs(mu->eta) && fabs(mu->eta) < 1.2 ) h_matchedRecoMu_pt_overlap_->Fill( mu->pt, weight);
      if( fabs(mu->eta) > 1.2 )                        h_matchedRecoMu_pt_endcap_->Fill( mu->pt, weight);

      h_matchedRecoMu_dxyVTX_->Fill( mu->dxyVTX_best, weight);
      h_matchedRecoMu_dzVTX_->Fill( mu->dzVTX_best, weight);
    }
  }

  void Save( TFile *f_output )
  {
    f_output->cd();

    for( const auto& h : vec_hist_ ) 
    {
      h->Write();
      delete h;
    }
    vector<TH1D*>().swap(vec_hist_);

    cout << "[HistContainer::Save] Histograms are saved in: " << f_output->GetName() << endl;
  }

private:
  void Init()
  {
    TH1::SetDefaultSumw2(kTRUE);
    TH1::AddDirectory(kFALSE);

    h_trigObj_pt_  = new TH1D("h_trigObj_pt_"+type_, "", nPtBin, arr_ptBinEdge );              vec_hist_.push_back( h_trigObj_pt_ );
    h_trigObj_eta_ = new TH1D("h_trigObj_eta_"+type_, "", nEtaBin, arr_etaBinEdge );           vec_hist_.push_back( h_trigObj_eta_ );
    h_trigObj_phi_ = new TH1D("h_trigObj_phi_"+type_, "", 20, (-1)*TMath::Pi(), TMath::Pi() ); vec_hist_.push_back( h_trigObj_phi_ );

    h_trigObj_quality_ = new TH1D("h_trigObj_quality_"+type_, "", 21, -1, 20); vec_hist_.push_back( h_trigObj_quality_ );

    h_trigObj_pt_barrel_  = new TH1D("h_trigObj_pt_barrel_"+type_, "", nPtBin, arr_ptBinEdge );  vec_hist_.push_back( h_trigObj_pt_barrel_ );
    h_trigObj_pt_overlap_ = new TH1D("h_trigObj_pt_overlap_"+type_, "", nPtBin, arr_ptBinEdge ); vec_hist_.push_back( h_trigObj_pt_overlap_ );
    h_trigObj_pt_endcap_  = new TH1D("h_trigObj_pt_endcap_"+type_, "", nPtBin, arr_ptBinEdge );  vec_hist_.push_back( h_trigObj_pt_endcap_ );


    h_matchedRecoMu_pt_  = new TH1D("h_matchedRecoMu_pt_"+type_, "", nPtBin, arr_ptBinEdge );              vec_hist_.push_back( h_matchedRecoMu_pt_ );
    h_matchedRecoMu_eta_ = new TH1D("h_matchedRecoMu_eta_"+type_, "", nEtaBin, arr_etaBinEdge );           vec_hist_.push_back( h_matchedRecoMu_eta_ );
    h_matchedRecoMu_phi_ = new TH1D("h_matchedRecoMu_phi_"+type_, "", 20, (-1)*TMath::Pi(), TMath::Pi() ); vec_hist_.push_back( h_matchedRecoMu_phi_ );

    h_matchedRecoMu_pt_barrel_  = new TH1D("h_matchedRecoMu_pt_barrel_"+type_, "", nPtBin, arr_ptBinEdge );  vec_hist_.push_back( h_matchedRecoMu_pt_barrel_ );
    h_matchedRecoMu_pt_overlap_ = new TH1D("h_matchedRecoMu_pt_overlap_"+type_, "", nPtBin, arr_ptBinEdge ); vec_hist_.push_back( h_matchedRecoMu_pt_overlap_ );
    h_matchedRecoMu_pt_endcap_  = new TH1D("h_matchedRecoMu_pt_endcap_"+type_, "", nPtBin, arr_ptBinEdge );  vec_hist_.push_back( h_matchedRecoMu_pt_endcap_ );

    h_matchedRecoMu_dxyVTX_ = new TH1D("h_matchedRecoMu_dxyVTX_"+type_, "", 60, -0.3, 0.3 ); vec_hist_.push_back( h_matchedRecoMu_dxyVTX_ );
    h_matchedRecoMu_dzVTX_  = new TH1D("h_matchedRecoMu_dzVTX_"+type_, "", 200, -1, 1 );     vec_hist_.push_back( h_matchedRecoMu_dzVTX_ );
  }
};

class TriggerHistProducer
{
public:
  TString trigger_ = "";
  TString triggerType_ = "";
  vector<TString> vec_dataPath_;
  TFile* f_output_;

  Bool_t isHLTPhysics_ = kFALSE;

  Bool_t usePtMatching_ = kFALSE;

  Bool_t applyL1QualityCut_ = kFALSE;

  void Set_Trigger(TString trigger)                 { trigger_ = trigger; }
  void Set_HLTPhysicsDataset(Bool_t flag = kTRUE)   { isHLTPhysics_ = flag; }
  void Set_DataPath(vector<TString> vec_dataPath)   { vec_dataPath_ = vec_dataPath; }
  void Set_OutputFile(TFile* f_output)              { f_output_ = f_output; }
  void Set_UsePtMatching( Bool_t flag = kTRUE )     { usePtMatching_ = flag; }
  void Set_ApplyL1QualityCut( Bool_t flag = kTRUE ) { applyL1QualityCut_ = flag; }

  void Produce()
  {
    triggerType_ = Get_TriggerType(trigger_);
    Double_t dRMax = 0; // -- dR cone size for matching to a offline muon
    if( triggerType_ == "L1" )      dRMax = 0.5;
    else if( triggerType_ == "L2" ) dRMax = 0.3;
    else                            dRMax = 0.1; // -- L3 or isolation

    // -- histograms
    HistContainer* hist_nonMuon       = new HistContainer("nonMuon");
    HistContainer* hist_matchedToReco = new HistContainer("matchedToReco");
    HistContainer* hist_nonIsolated   = new HistContainer("nonIsolated");
    HistContainer* hist_isolated      = new HistContainer("isolated");


    // -- make a chain
    TChain *chain = new TChain("ntupler/ntuple");
    for( const auto& dataPath : vec_dataPath_ )
    {
      cout << "Adding...   " << dataPath << endl;
      chain->Add( dataPath );
    }

    // -- initialize NtupleHandle
    NtupleHandle *ntuple = new NtupleHandle( chain );
    ntuple->TurnOnBranches_Event();
    ntuple->TurnOnBranches_Muon();
    ntuple->TurnOnBranches_simMuon();
    ntuple->TurnOnBranches_HLTMuon();
    ntuple->TurnOnBranches_Trigger();
    ntuple->TurnOnBranches_GenParticle();



    Int_t nEvent = chain->GetEntries();
    cout << "[Total event: " << nEvent << "]" << endl;
    for(Int_t i_ev=0; i_ev<nEvent; i_ev++)
    {
      ntuple->GetEvent( i_ev );
      loadBar(i_ev+1, nEvent, 100, 100);

      Double_t weight = 1.0;
      if( !ntuple->isRealData && ntuple->genEventWeight < 0 ) weight = -1.0 * weight;

      if( isHLTPhysics_ )
      {
        Double_t prescale = FindPrescale_PhysicsTrigger(ntuple);
        if( prescale == -1 ) continue; // -- HLT_Physics trigger is not fired: skip this event
        else weight *= 107 * prescale; // -- 107: period in hltL1EventNumberL1Fat (considered as L1 prescale)
      }

      // -- iteration over HLT objects
      for(size_t i_hlt=0; i_hlt<ntuple->vec_filterName->size(); i_hlt++)
      {
        HLTObject *HLTObj = new HLTObject( ntuple, i_hlt );

        TString filterName = HLTObj->filterName;
        if( filterName.Contains(trigger_) ) // -- corresponding HLT object
        {
          // -- 2018 L2: skip if L2 pT < 10 GeV (to synchronize with 2016 L2, which internally has 10 GeV cut)
          if( filterName.Contains("hltL2fL1sSingleMu22L1f0L2Filtered10Q") && HLTObj->pt < 10 ) continue;

          Double_t quality;
          if( triggerType_ == "L1" ) quality = Get_L1QualityOfL1TriggerObject(ntuple, HLTObj);
          else                       quality = -1;

          if( applyL1QualityCut_ )
          {
            if( quality < 12 ) continue;
          }

          TString matchingType = "";
          Muon* matchedMu = TrigObj_Offline_Matching( ntuple, HLTObj, dRMax, matchingType );
          if( matchingType == "nonMuon" )       hist_nonMuon->Fill( HLTObj, quality, nullptr, weight );
          if( matchingType == "matchedToReco" ) hist_matchedToReco->Fill( HLTObj, quality, matchedMu, weight );
          if( matchingType == "nonIsolated" )   hist_nonIsolated->Fill( HLTObj, quality, matchedMu, weight );
          if( matchingType == "isolated" )      hist_isolated->Fill( HLTObj, quality, matchedMu, weight );
        }

      } // -- end of HLT object iteration

    } // -- end of event iteration

    f_output_->cd();
    hist_nonMuon->Save(f_output_);
    hist_matchedToReco->Save(f_output_);
    hist_nonIsolated->Save(f_output_);
    hist_isolated->Save(f_output_);
    f_output_->Close();
  }
private:
  Muon* TrigObj_Offline_Matching( NtupleHandle* ntuple, HLTObject *HLTObj, Double_t dRMax, TString& matchingType )
  {
    // -- trigger object - offline muon matching

    Int_t index_matchedMu = -1;
    Double_t dR_best = 999.0;

    for(Int_t i_mu=0; i_mu<ntuple->nMuon; i_mu++)
    {
      Muon Mu(ntuple, i_mu);
      Double_t dR = (HLTObj->vecP).DeltaR( Mu.vecP );

      Double_t pt_HLTObj = HLTObj->pt;
      Double_t pt_reco   = Mu.pt;
      Double_t ptDiff = fabs(  (pt_HLTObj - pt_reco) / pt_reco  );
      Bool_t ptCheck;
      if( usePtMatching_ ) ptCheck = ptDiff < 0.3;
      else                 ptCheck = kTRUE;

      if( dR < dRMax && dR < dR_best && ptCheck )
      {
        index_matchedMu = i_mu;
        dR_best = dR;
      }
    }

    Muon *matchedMu;
    if( index_matchedMu == -1 ) // -- there is no matched offline muon: return nullptr;
    {
      matchingType = "nonMuon";
      matchedMu = nullptr;
    }
    else // -- find matched muon;
    {
      matchedMu = new Muon(ntuple, index_matchedMu);

      if( matchedMu->isLoose )
      {
        if( matchedMu->relPFIso_dBeta < 0.15 ) matchingType = "isolated";
        else                                   matchingType = "nonIsolated";
      }
      else // -- failed to pass loose ID
        matchingType = "matchedToReco";
    }

    return matchedMu;
  }

  Double_t FindPrescale_PhysicsTrigger(NtupleHandle* ntuple)
  {
    Double_t thePrescale = -1;

    Int_t nFiredTrigger = (Int_t)ntuple->vec_firedTrigger->size();
    for(Int_t i_trig = 0; i_trig < nFiredTrigger; i_trig++)
    {
      TString triggerPath = ntuple->vec_firedTrigger->at(i_trig);
      if( triggerPath.Contains("HLT_Physics_v") )
        thePrescale = ntuple->vec_prescale->at(i_trig);
    }

    if( thePrescale == -1 )
    {
      cout << "HLT_Physics trigger is not fired in this event! ... the list of fired events:" << endl;
      Int_t nFiredTrigger = (Int_t)ntuple->vec_firedTrigger->size();
      for(Int_t i_trig = 0; i_trig < nFiredTrigger; i_trig++)
      {
        TString  triggerPath = ntuple->vec_firedTrigger->at(i_trig);
        Double_t prescale    = ntuple->vec_prescale->at(i_trig);
        printf("  [%dth trigger] name = %s, prescale = %.0lf\n", i_trig, triggerPath.Data(), prescale);
      }
    }

    return thePrescale;
  }

  TString Get_TriggerType(TString trigger)
  {
    TString type = "";
    if( trigger.Contains("hltL1fL1sMu22L1Filtered0") )             type = "L1";

    if( trigger.Contains("hltL2fL1sMu22L1f0L2Filtered10Q") || 
        trigger.Contains("hltL2fL1sSingleMu22L1f0L2Filtered10Q") ) type = "L2";

    if( trigger.Contains("hltL3crIsoL1sMu22L1f0L2f10QL3f24QL3trkIsoFiltered0p09") ||       // -- 2016, IsoMu24
        trigger.Contains("hltL3fL1sMu22L1f0Tkf24QL3trkIsoFiltered0p09") ||                 // -- 2016, IsoTkMu24
        trigger.Contains("hltL3fL1sMu22L1f0L2f10QL3Filtered24Q") ||                        // -- 2016, Mu24
        trigger.Contains("hltL3fL1sMu22f0TkFiltered24Q") ||                                // -- 2016, TkMu24
        trigger.Contains("hltL3fL1sMu25f0TkFiltered50Q") ||                                // -- 2016, TkMu50
        trigger.Contains("hltL3crIsoL1sSingleMu22L1f0L2f10QL3f24QL3trkIsoFiltered0p07") || // -- 2018, IsoMu24
        trigger.Contains("hltL3fL1sSingleMu22L1f0L2f10QL3Filtered24Q") ||                  // -- 2018, Mu24
        trigger.Contains("hltL3fL1sMu22Or25L1f0L2f10QL3Filtered50Q")                       // -- 2016 & 2018, Mu50
        ) type = "L3";

    return type;
  }

  Double_t Get_L1QualityOfL1TriggerObject(NtupleHandle* ntuple, HLTObject *HLTObj)
  {
    Double_t theQuality = -1;

    Int_t index_matchedL1Mu = -1;
    Double_t dR_best = 999.0;
    for(Int_t i_L1=0; i_L1<ntuple->nL1Muon; i_L1++)
    {
      L1Muon L1Mu(ntuple, i_L1);
      Double_t dR = (HLTObj->vecP).DeltaR( L1Mu.vecP );

      if( dR < 0.1 && dR < dR_best )
      {
        index_matchedL1Mu = i_L1;
        dR_best = dR;
      }
    }

    if( index_matchedL1Mu == -1 )
      cout << "[Get_L1QualityOfL1TriggerObject] Matched L1 muon is not found!! ... return -1 for the quality" << endl;
    else
    {
      L1Muon L1Mu(ntuple, index_matchedL1Mu);
      theQuality = L1Mu.quality;
      // printf("[Get_L1QualityOfL1TriggerObject] Matched L1 muon is found\n");
      // printf("   Trigger object: (pt, eta, phi)          = (%.3lf, %.3lf, %.3lf)\n", HLTObj->pt, HLTObj->eta, HLTObj->phi);
      // printf("   L1 muon:        (pt, eta, phi, quality) = (%.3lf, %.3lf, %.3lf, %.1lf)\n", L1Mu.pt, L1Mu.eta, L1Mu.phi, L1Mu.quality);
    }

    return theQuality;
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