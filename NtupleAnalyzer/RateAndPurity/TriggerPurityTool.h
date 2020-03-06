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
const double PtBin[25]  = { 0, 5, 10, 20, 24, 27, 30, 35, 40, 45, 50, 55, 60, 65, 70, 80, 90, 100, 120, 150, 200, 250, 500, 1000, 10000 };
const Int_t nEtaBin = 15;
const double EtaBin[16] = {-5, -2.4, -2.1, -1.6, -1.2, -0.9, -0.3, -0.2,  0.2, 0.3, 0.9, 1.2, 1.6, 2.1, 2.4, 5};

class PurityHistContainer
{
public:
  TString Tag;  // Including Run number and Rerun object and eta range ex) RunXXXXXX_HLT_EtaLo_EtaUp

  vector<TH1D*> vec_Hist;
  TH1D* h_L3_Pt;
  TH1D* h_L3_Eta;
  TH1D* h_L3_Phi;
  TH1D* h_L3_nVertices;
  TH1D* h_L3_EtaFine;
  TH2D* h_L3_EtaPhi;

  PurityHistContainer( TString _Tag )
  {
    this->Tag = _Tag;
    this->Init();
  }

  ~PurityHistContainer() {}

  template <class T>  // HLTObject or MYHLTObject
  void Fill( NtupleHandle *ntuple, T *L3Mu, Double_t _weight = 1.0 )
  {
    Double_t theWeight = 1.0*_weight;

    if( !ntuple->isRealData && ntuple->genEventWeight < 0 )
      theWeight = -1.0*theWeight;

    this->h_L3_Pt->Fill(L3Mu->pt, theWeight);
    this->h_L3_Eta->Fill(L3Mu->eta, theWeight);
    this->h_L3_Phi->Fill(L3Mu->phi, theWeight);
    this->h_L3_nVertices->Fill(ntuple->nVertex, theWeight);
    this->h_L3_EtaFine->Fill(L3Mu->eta, theWeight);
    this->h_L3_EtaPhi->Fill(L3Mu->eta, L3Mu->phi, theWeight);
  }

  void Save( TFile *f_output )
  {
    f_output->cd();

    for( const auto& h : this->vec_Hist ) {
      h->Write();
      delete h;
    }
    vector<TH1D*>().swap(this->vec_Hist);

    this->h_L3_EtaPhi->Write();
    delete this->h_L3_EtaPhi;
  }

private:
  void Init()
  {
    TH1::SetDefaultSumw2(kTRUE);
    TH1::AddDirectory(kFALSE);

    this->h_L3_Pt = new TH1D("h_L3_Pt_"+this->Tag, "", nPtBin, PtBin );  this->vec_Hist.push_back( this->h_L3_Pt );
    this->h_L3_Eta = new TH1D("h_L3_Eta_"+this->Tag, "", nEtaBin, EtaBin );  this->vec_Hist.push_back( this->h_L3_Eta );
    this->h_L3_Phi = new TH1D("h_L3_Phi_"+this->Tag, "", 20, (-1)*TMath::Pi(), TMath::Pi() );  this->vec_Hist.push_back( this->h_L3_Phi );
    this->h_L3_nVertices = new TH1D("h_L3_nVertices_"+this->Tag, "", 200, 0, 200 );  this->vec_Hist.push_back( this->h_L3_nVertices );
    this->h_L3_EtaFine = new TH1D("h_L3_EtaFine_"+this->Tag, "", 1000, -5, 5 );  this->vec_Hist.push_back( this->h_L3_EtaFine );
    this->h_L3_EtaPhi  = new TH2D("h_L3_EtaPhi_"+this->Tag, "", 1000, -5, 5, 50, (-1)*TMath::Pi(), TMath::Pi() );
  }
};



class TriggerPurityTool
{
public:
  Int_t maxEvents;
  vector<TString> vec_DataPath;
  Int_t RunMin;
  Int_t RunMax;
  TString TriggerTag;

  Double_t EtaLo;
  Double_t EtaUp;

  Bool_t doDimuon;

  Bool_t debug;

  TFile *f_output;

  vector< TString > vec_Offline_selection;

  Bool_t applyL2PtCut_ = kFALSE;

  Bool_t doPUReweighting_ = kFALSE;
  TString fileName_PUWeight_ = "";
  TString histName_PUWeight_ = "";

  TriggerPurityTool( Double_t _EtaLo, Double_t _EtaUp )
  {
    this->maxEvents = -1;
    this->doDimuon = kFALSE;
    this->debug = kFALSE;
    this->TriggerTag = "";
    this->f_output = 0;
    this->EtaLo = _EtaLo;
    this->EtaUp = _EtaUp;
    this->RunMin = 0;
    this->RunMax = 999999;

    // OffSel_HLT or OffSel_MYHLT
    // OffSel = Match(0, 1)_Id(String)_Iso(String)
    this->vec_Offline_selection = {};
  }

  ~TriggerPurityTool() {}

  void Set_DataList( vector<TString> _vec )
  {
    this->vec_DataPath = _vec;
  }

  void Set_MaxEvents( Int_t _maxEvents )
  {
    this->maxEvents = _maxEvents;
  }

  void Set_RunNumRange( vector<Int_t> _vec )
  {
    this->RunMin = _vec[0];
    this->RunMax = _vec[1];
  }

  void Set_Trigger( TString _TriggerTag )
  {
    this->TriggerTag = _TriggerTag;
  }

  void Set_Output( TFile* _f_output )
  {
    this->f_output = _f_output;
  }

  void Set_Selection( vector<TString> _selection )
  {
    this->vec_Offline_selection = _selection;
    for(size_t i=0; i<(this->vec_Offline_selection).size(); ++i) {
      TString Sel = (this->vec_Offline_selection)[i];

      if(!Sel.Contains("_HLT") && !Sel.Contains("_MYHLT")) {
        cout << "WARNING: Set_Selection: selection string does not contain _HLT or _MYHLT" << Sel << endl;
        return;
      }
    }
  }

  // -- apply L2 > 10 GeV cut (to synchronize 2016 and 2018 results)
  void Set_ApplyL2PtCut( Bool_t flag = kTRUE )
  {
    applyL2PtCut_ = flag;
  }

  // -- do PU reweighting (MUOVal PU -> Flat PU)
  void Set_PUReweighting( Bool_t flag = kTRUE, TString fileName, TString histName )
  {
    doPUReweighting_ = kTRUE;
    fileName_PUWeight_ = fileName;
    histName_PUWeight_ = histName;
  }

  void Analyze()
  {
    TH1::AddDirectory(kFALSE);
    printMemory();
    StartTimer();

    if( this->TriggerTag == "" ||
      this->f_output == 0 ||
      this->vec_DataPath.size() == 0 ||
      this->EtaLo < 0 ||
      this->EtaUp < this->EtaLo )
      {
        cout << "Setup is not done accordingly!" << endl;
        cout << this->TriggerTag << endl;
        cout << this->f_output << endl;
        cout << this->vec_DataPath.size() << endl;
        cout << this->EtaLo << endl;
        cout << this->EtaUp << endl;
        return;
      }

    cout << "[EtaRange : " << this->EtaLo << ", " << this->EtaUp << " ]" << endl;

    TChain *chain = new TChain("ntupler/ntuple");
    for( const auto& DataPath : this->vec_DataPath )
    {
      cout << "Adding...   " << DataPath << endl;
      chain->Add( DataPath );
    }

    Int_t nEvent = chain->GetEntries();
    cout << "[Total event: " << nEvent << "]" << endl;
    if(this->maxEvents > 0) {
      nEvent = this->maxEvents;
      cout << "\t --> Testing with Total event: " << nEvent << endl;
    }

    vector<PurityHistContainer*> vec_HistContainer = {};

    for(size_t i_sel=0; i_sel<(this->vec_Offline_selection).size(); ++i_sel) {
      TString strSel = (this->vec_Offline_selection)[i_sel];

      TString HistTag = TString::Format("Run%dto%d_%s_%.1f_%.1f", this->RunMin, this->RunMax, strSel.Data(), this->EtaLo, this->EtaUp);
      PurityHistContainer* HistContainer = new PurityHistContainer( HistTag );
      vec_HistContainer.push_back( HistContainer );
    }


    vector<TString> filters = {};
    if( (this->TriggerTag).Contains("+") ) {
      TObjArray *split  = (this->TriggerTag).Tokenize("+");
      for(Int_t ii=0; ii<split->GetEntriesFast(); ++ii)
        filters.push_back( (TString)(((TObjString*)(split->At(ii)))->String()) );
      delete split;
    }
    else {
      filters = { this->TriggerTag };
    }

    NtupleHandle *ntuple = new NtupleHandle( chain );
    ntuple->TurnOnBranches_Event();
    ntuple->TurnOnBranches_Muon();
    ntuple->TurnOnBranches_simMuon();
    ntuple->TurnOnBranches_Trigger();
    ntuple->TurnOnBranches_GenParticle();

    TH1D* h_PUWeight = nullptr;
    if( doPUReweighting_ )
    {
      TFile *f_PU = TFile::Open(fileName_PUWeight_);
      if( !f_PU )
        h_PUWeight = (TH1D*)f_PU->Get(histName_PUWeight_)->Clone();
      else
      {
        cout << fileName_PUWeight_ << " does not exist!" << endl;
        return;
      }
    }

    for(Int_t i_ev=0; i_ev<nEvent; i_ev++)
    {
      if( debug ) printf("\n ** event %d **\n", i_ev);
      ntuple->GetEvent( i_ev );
      loadBar(i_ev+1, nEvent, 100, 100);

      if( i_ev !=0 && i_ev % 70001 == 0 )  printMemory("\t");

      Int_t truePU = ntuple->truePU;
      if( truePU != 50 ) continue;

      Double_t weight = 1.0;
      if( doPUReweighting_ )
      {
        Int_t truePU = ntuple->truePU;

        // -- PU == 0 -> corresponding bin in PU weight histogram = 1
        // -- PU == 1 -> corresponding bin in PU weight histogram = i+1;
        weight = h_PUWeight->GetBinContent(truePU+1);
      }

      if(ntuple->runNum >= this->RunMin && ntuple->runNum <= this->RunMax)
      {

        if(doDimuon) {
          // Bool_t isPassDimuon = dimuonSelection( ntuple, 10. );
          Bool_t isPassDimuon = genDimuonSelection( ntuple );
          if(!isPassDimuon)  continue;  // skip event
        }

        for(size_t i_sel=0; i_sel<(this->vec_Offline_selection).size(); ++i_sel) {
          TString strSel = (this->vec_Offline_selection)[i_sel];

          vector<vector<Double_t>> eta_filled = {};
          vector<vector<Double_t>> phi_filled = {};
          for(Int_t ifilter = 0; ifilter<(Int_t)filters.size(); ++ifilter) {
            vector<Double_t> eta_filled_tmp = {};
            vector<Double_t> phi_filled_tmp = {};

            Bool_t isL2 = kFALSE;
            if( filters[ifilter].Contains("hltL2fL1sMu22L1f0L2Filtered10Q") || filters[ifilter].Contains("hltL2fL1sSingleMu22L1f0L2Filtered10Q") )
              isL2 = kTRUE;

            for(size_t i_hlt=0; i_hlt<ntuple->vec_filterName->size(); i_hlt++)
            {

              if(strSel.Contains("_HLT")) {
                HLTObject *HLTObj = new HLTObject( ntuple, i_hlt );

                if( fabs(HLTObj->eta) >= this->EtaLo && fabs(HLTObj->eta) < this->EtaUp ) {

                  Bool_t isFilled = false;
                  for(Int_t prev = 0; prev < (Int_t)eta_filled.size(); ++prev) {
                    for(Int_t p_mu = 0; p_mu < (Int_t)eta_filled[prev].size(); ++p_mu) {
                      Double_t dR = sqrt( (eta_filled[prev][p_mu] - HLTObj->eta)*(eta_filled[prev][p_mu] - HLTObj->eta) + (phi_filled[prev][p_mu] - HLTObj->phi)*(phi_filled[prev][p_mu] - HLTObj->phi) );
                      if( dR < 0.1 ) {
                        isFilled = true;
                        break;
                      }
                    }
                  }

                  if( !isFilled ) {
                    if( (HLTObj->filterName).Contains(filters[ifilter]) ) {
                      if(debug)  cout << "\n L3 Mu found: " << TString::Format("pT=%.2f eta=%.2f phi=%.2f", HLTObj->pt, HLTObj->eta, HLTObj->phi) << endl;
                      if(debug)  cout << "\t --> pass eta cut " << endl;
                      if(debug)  cout << "\t --> pass filter: " << TriggerTag << endl;

                      Bool_t isPass = this->offSelection( ntuple, HLTObj, strSel );

                      if(isPass) {
                        
                        if( isL2 && applyL2PtCut_ ) // -- only when L2 muons & require L2 pt cut: check pt
                        {
                          if(debug) cout << "It is the L2 filter = " << filters[ifilter] << endl;
                          if(debug) cout << "   ---> L2 pt: " << HLTObj->pt << endl;
                          if( HLTObj->pt > 10 )
                          {
                            if(debug) cout << "   ---> pass" << endl;
                            (vec_HistContainer[i_sel])->Fill( ntuple, HLTObj, weight );
                            eta_filled_tmp.push_back(HLTObj->eta);
                            phi_filled_tmp.push_back(HLTObj->phi);
                          }
                          else
                          {
                            if(debug) cout << "L2 muon is rejected due to its low pT" << endl;
                          }
                        }
                        else // -- the other case: no check on pt; just fill
                        {
                          (vec_HistContainer[i_sel])->Fill( ntuple, HLTObj, weight );
                          eta_filled_tmp.push_back(HLTObj->eta);
                          phi_filled_tmp.push_back(HLTObj->phi);
                        }
                      }
                    }
                  }

                }

                delete HLTObj;
              }

              /*
              else if(strSel.Contains("_MYHLT")) {
                MYHLTObject *HLTObj = new MYHLTObject( ntuple, i_hlt );

                if( fabs(HLTObj->eta) >= this->EtaLo && fabs(HLTObj->eta) < this->EtaUp ) {
                  if( (HLTObj->filterName).Contains(TriggerTag) ) {

                    if(debug)  cout << "\n L3 Mu found: " << TString::Format("pT=%.2f eta=%.2f phi=%.2f", HLTObj->pt, HLTObj->eta, HLTObj->phi) << endl;
                    if(debug)  cout << "\t --> pass eta cut " << endl;
                    if(debug)  cout << "\t --> pass filter: " << TriggerTag << endl;

                    Bool_t isPass = this->offSelection( ntuple, HLTObj, strSel );

                    if(isPass) {
                      (vec_HistContainer[i_sel])->Fill( ntuple, HLTObj );
                    }

                  }
                }

                delete HLTObj;
              }
              */

            }  // end of i_hlt
            eta_filled.push_back(eta_filled_tmp);
            phi_filled.push_back(phi_filled_tmp);

          }  // end of ifilter

        }  // end of vec_Offline_selection

      }  // if runNum in RunRange

      ntuple->Clear();
    }  // end of Events iteration

    //-- save histograms
    this->f_output->cd();
    for( const auto& hc : vec_HistContainer ) {
      cout << "\t Saved: " << hc->Tag << endl;
      hc->Save( this->f_output );
      delete hc;
      printMemory("\t\t");
    }

    delete chain;

    PrintRunTime();
    printMemory();
  }

private:

  void Init(){}

  Bool_t offSelectionId( Muon *Mu, TString Id )
  {
    if(Id == "IdNo")
      return kTRUE;

    else if(Id == "IdPfOrTrkOrGlb")
      return (Mu->isPF || Mu->isTRK || Mu->isGLB);
    else if(Id == "IdTrk")
      return Mu->isTRK;
    else if(Id == "IdGlb")
      return Mu->isGLB;
    else if(Id == "IdPf")
      return Mu->isPF;

    else if(Id == "IdLoose")
      return Mu->isLoose;
    else if(Id == "IdTight")
      return Mu->isTight;
    else if(Id == "IdHighPt")
      return Mu->isHighPtNew;

    else if(Id == "IdTrkAndTrkQual")
      return (Mu->isTRK && Mu->dxyVTX_best<0.2 && Mu->dzVTX_best<0.5 && Mu->nPixelHit_inner>0 && Mu->nTrackerLayer_inner>5);

    else if(Id == "IdGlbAndTrkQual")
      return (Mu->isGLB && Mu->dxyVTX_best<0.2 && Mu->dzVTX_best<0.5 && Mu->nPixelHit_inner>0 && Mu->nTrackerLayer_inner>5);
    else if(Id == "IdGlbAndTrkQualPlus1MS")
      return (Mu->isGLB && Mu->dxyVTX_best<0.2 && Mu->dzVTX_best<0.5 && Mu->nPixelHit_inner>0 && Mu->nTrackerLayer_inner>5 && Mu->nMatchedStation>0);
    else if(Id == "IdGlbAndTrkQualPlus2MS")
      return (Mu->isGLB && Mu->dxyVTX_best<0.2 && Mu->dzVTX_best<0.5 && Mu->nPixelHit_inner>0 && Mu->nTrackerLayer_inner>5 && Mu->nMatchedStation>1);
    else if(Id == "IdGlbAndMuonQual")
      return (Mu->isGLB && Mu->nMuonHit_global>0 && Mu->nMatchedStation>1);

    else if(Id == "IdTrkPlus1MS")
      return (Mu->isTRK && Mu->nMatchedStation>0);

    else {
      cout << "WARNING: offSelectionId: Wrong Id Tag  --> return false" << endl;
      return kFALSE;
    }
  }

  Bool_t offSelectionIso( Muon *Mu, TString Iso )
  {
    if(Iso == "IsoNo")
      return kTRUE;

    else if(Iso == "IsoPFLoose")
      return (Mu->relPFIso_dBeta < 0.25);
    else if(Iso == "IsoPFTight")
      return (Mu->relPFIso_dBeta < 0.15);

    else if(Iso == "IsoTkLoose")
      return (Mu->relTrkIso < 0.1);
    else if(Iso == "IsoTkTight")
      return (Mu->relTrkIso < 0.05);

    else {
      cout << "WARNING: offSelectionIso: Wrong Iso Tag  --> return false" << endl;
      return kFALSE;
    }
  }

  Bool_t offSelectionSim( NtupleHandle *ntuple, Muon *Mu, TString Sim )
  {
    // https://github.com/cms-sw/cmssw/blob/master/DataFormats/MuonReco/interface/MuonSimInfo.h
    if(Sim == "SimNo")
      return kTRUE;

    else if(Sim == "SimPrompt")
      return (Mu->simType == 4);

    else if(Sim == "SimPromptGho")
      return (fabs(Mu->simType) == 4);

    else if(Sim == "SimPromptGauge")
      return (Mu->simType == 4 && Mu->simExtType == 10);

    else if(Sim == "SimPromptGaugeGho")
      return (fabs(Mu->simType) == 4 && fabs(Mu->simExtType) == 10);

    else if(Sim == "SimPromptHad")
      return (Mu->simType == 4 || Mu->simType == 3 || Mu->simType == 2);

    else if(Sim == "SimPromptHadGho")
      return (fabs(Mu->simType) == 4 || fabs(Mu->simType) == 3 || fabs(Mu->simType) == 2);

    else if(Sim == "SimPromptHadPun")
      return (Mu->simType == 4 || Mu->simType == 3 || Mu->simType == 2 || Mu->simType == 1);

    else if(Sim == "SimPromptHadPunGho")
      return (fabs(Mu->simType) == 4 || fabs(Mu->simType) == 3 || fabs(Mu->simType) == 2 || fabs(Mu->simType) == 1);

    // -- added by KP: for sim-hit matching study
    else if(Sim == "SimPromptMuon")
      return (fabs(Mu->simType) == 4); // -- MatchedPrimaryMuon || GhostPrimaryMuon

    else if(Sim == "SimNonPromptMuon")
      return (fabs(Mu->simType) == 2 || fabs(Mu->simType) == 3); // -- MatchedMuonFromHeavyFlavour || MatchedMuonFromLightFlavour ||  GhostMuonFromHeavyFlavour || GhostMuonFromLightFlavour


    else if(Sim == "SimGen") {
      //-- Find matched gen
      Double_t max_dR = 0.1;
      Double_t deltaR_best = 999.;
      for(Int_t i_gen=0; i_gen<ntuple->nGenParticle; i_gen++)
      {
        GenParticle *GenP = new GenParticle( ntuple, i_gen );
        if(GenP->status == 1) {
          Double_t deltaR = (GenP->vecP).DeltaR( Mu->vecP );
          if( deltaR < max_dR && deltaR < deltaR_best ) {
            deltaR_best = deltaR;
          }
        }

        delete GenP;
      }
      return (deltaR_best < max_dR);
    }

    else if(Sim == "SimGenHard") {
      //-- Find matched gen
      Double_t max_dR = 0.1;
      Double_t deltaR_best = 999.;
      for(Int_t i_gen=0; i_gen<ntuple->nGenParticle; i_gen++)
      {
        GenParticle *GenP = new GenParticle( ntuple, i_gen );
        if(GenP->status == 1 && GenP->fromHardProcessFinalState == 1) {
          Double_t deltaR = (GenP->vecP).DeltaR( Mu->vecP );
          if( deltaR < max_dR && deltaR < deltaR_best ) {
            deltaR_best = deltaR;
          }
        }

        delete GenP;
      }
      return (deltaR_best < max_dR);
    }

    else {
      cout << "WARNING: offSelectionSim: Wrong Sim Tag  --> return false" << endl;
      return kFALSE;
    }
  }

  template <class T>  // HLTObject or MYHLTObject
  Bool_t offSelection( NtupleHandle *ntuple, T *L3Mu, TString _sel )
  {
    TString sel = _sel;

    // OffSel = Mat(0,1)_Id(String)_Iso(String)_Sim(String)
    TObjArray *split  = sel.Tokenize("_");
    TString Mat = ((TObjString*)(split->At(0)))->String();
    TString Id  = ((TObjString*)(split->At(1)))->String();
    TString Iso = ((TObjString*)(split->At(2)))->String();
    TString Sim = ((TObjString*)(split->At(3)))->String();
    // split->Delete();
    delete split;

    Bool_t isMat = kFALSE;
    Bool_t isId  = (Id  == "IdNo");
    Bool_t isIso = (Iso == "IsoNo");
    Bool_t isSim = (Sim == "SimNo");

    // No offline matching  -> return true
    if(Mat == "Mat0")
      return kTRUE;

    else if(Mat == "Mat1") {
      //-- Find matched offline
      Double_t max_dR = 0.1;
      Int_t matched_mu = -999;
      Double_t deltaR_best = 999.;
      for(Int_t i_mu=0; i_mu<ntuple->nMuon; i_mu++)
      {
        Muon *Mu = new Muon( ntuple, i_mu );

        Double_t deltaR = (L3Mu->vecP).DeltaR( Mu->vecP );

        if( deltaR < max_dR && deltaR < deltaR_best ) {
          isMat       = kTRUE;
          deltaR_best = deltaR;
          matched_mu  = i_mu;
        }

        delete Mu;
      }

      //-- update isId, isIso, and isSim only if isMat==true
      if(isMat) {
        Muon *Matched_Mu = new Muon( ntuple, matched_mu );
        if(debug)  cout << "\t\t matched off mu found: " <<
                     TString::Format("pT=%.2f eta=%.2f phi=%.2f  --> dR=%.2f", Matched_Mu->pt, Matched_Mu->eta, Matched_Mu->phi, deltaR_best) << endl;

        if(!isId)
          isId = this->offSelectionId( Matched_Mu, Id );
        if(!isIso)
          isIso = this->offSelectionIso( Matched_Mu, Iso );
        if(!isSim)
          isSim = this->offSelectionSim( ntuple, Matched_Mu, Sim );

        delete Matched_Mu;
      }

    }

    if(debug)  cout << "\t\t   --> " << Mat << " isMat: " << isMat << endl;
    if(debug)  cout << "\t\t   --> " << Id  << " isId:  " << isId << endl;
    if(debug)  cout << "\t\t   --> " << Iso << " isIso: " << isIso << endl;
    if(debug)  cout << "\t\t   --> " << Sim << " isSim: " << isSim << endl;
    if(debug)  cout << "\t\t       --> final pass: " << (isMat && isId && isIso && isSim) << endl;

    return (isMat && isId && isIso && isSim);
  }

  Bool_t dimuonSelection( NtupleHandle *ntuple, Double_t ZWindow )
  {
    const Double_t ZMass = 91.1876;
    Double_t bestMass = -999.;

    for(Int_t i_mu=0; i_mu<ntuple->nMuon-1; i_mu++)
    {
      Muon *Mu1 = new Muon( ntuple, i_mu );
      // if(debug)  cout << "\t Mu1(GLB)[" << i_mu << "]: " << Mu1->pt << endl;
      if(Mu1->isGLB) {
        for(Int_t j_mu=i_mu+1; j_mu<ntuple->nMuon; j_mu++)
        {
          Muon *Mu2 = new Muon( ntuple, j_mu );
          // if(debug)  cout << "\t\t Mu2(GLB)[" << j_mu << "]: " << Mu2->pt << endl;
          if(Mu2->isGLB) {

            Double_t tempMass = (Mu1->vecP + Mu2->vecP).M();
            // if(debug)  cout << "\t\t\t temp dimuon mass: " << tempMass << endl;
            if( fabs(tempMass - ZMass) < fabs(bestMass - ZMass) ) {
              // if(debug)  cout << "\t\t\t   --> " << tempMass << endl;

              bestMass = tempMass;
            }

          }
          delete Mu2;
        }
      }
      delete Mu1;
    }

    if(debug)  cout << "\t\t\t\t pass mass window: " << bestMass << " --> " << (fabs(bestMass - ZMass) < ZWindow) << endl;
    return (fabs(bestMass - ZMass) < ZWindow);
  }

  Bool_t genDimuonSelection( NtupleHandle *ntuple )
  {
    bool find0 = false;
    bool find1 = false;
    for(Int_t i_gen=0; i_gen<ntuple->nGenParticle; i_gen++)
    {
      GenParticle *GenP = new GenParticle( ntuple, i_gen );
      if(GenP->isHardProcess) {
        if(GenP->ID == 13)
          find0 = true;
        if(GenP->ID == -13)
          find1 = true;
        if(find0 && find1)
          break;
      }
      delete GenP;
    }

    return (find0 && find1);
  }

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

  static inline void printMemory( TString tab = "" )
  {
    ifstream proc_status("/proc/self/status");
    string buffer;
    while (proc_status.peek() != EOF) {
      getline(proc_status, buffer);
      TString str = buffer;
      if(str.Contains("RSS")) {
        cout << tab << str << endl;
        break;
      }
    }
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
