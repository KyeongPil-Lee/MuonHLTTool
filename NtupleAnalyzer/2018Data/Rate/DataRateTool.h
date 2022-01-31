#include <Include/MuonHLTTool.h>
#include <Include/SimplePlotTools.h>

// -- works only for a single run!
class DataRateHistContainer {
public:
  void Set_TriggerList_HLT(vector<TString> vec_trigger)   { vec_trigger_HLT_ = vec_trigger; }
  void Set_TriggerList_MYHLT(vector<TString> vec_trigger) { vec_trigger_MYHLT_ = vec_trigger; }

  // -- weight = HLT_Physics prescale * # HLTPhysics dataset
  void Set_Weight( Double_t weight ) { weight_ = weight; }

  void Init() { 
    if( weight_ < 0 ) {
      cout << " weight = " << weight_ << " < 0! ... it should properly be set" << endl;
      return;
    }
    Init_Hist(); 
    isInit_ = kTRUE;
  }

  void Fill(MuonHLT::NtupleHandle *ntuple) {
    if( !isInit_ ) {
      cout << "DataRateHistProducer::Init() is not called before calling Fill!" << endl;
      return;
    }

    // -- fill inst. lumi information
    Int_t nLS = ntuple->lumiBlockNum;
    Int_t index = nLS-1; // -- to start from 0 (0th element = 1st LS)
    arr_LS_nCount_[index]      = arr_LS_nCount_[index] + 1; 
    arr_LS_sumInstLumi_[index] = arr_LS_sumInstLumi_[index] + ntuple->instLumi;

    Fill_nLSEvent(ntuple->vec_firedTrigger,   ntuple->lumiBlockNum, vec_trigger_HLT_,   vec_hist_nLSEvent_HLT_);
    Fill_nLSEvent(ntuple->vec_myFiredTrigger, ntuple->lumiBlockNum, vec_trigger_MYHLT_, vec_hist_nLSEvent_MYHLT_);
  }

  void Save() {
    FillHist_fromArray( h_LS_nCount_,      arr_LS_nCount_ );
    FillHist_fromArray( h_LS_sumInstLumi_, arr_LS_sumInstLumi_ );

    h_LS_nCount_->Write();
    h_LS_sumInstLumi_->Write();

    for(auto h : vec_hist_nLSEvent_HLT_ )
      h->Write();

    for(auto h : vec_hist_nLSEvent_MYHLT_ )
      h->Write();
  }

private:
  Bool_t isInit_ = kFALSE;

  Double_t weight_ = -1.0;

  vector<TString> vec_trigger_HLT_ = {};
  vector<TString> vec_trigger_MYHLT_ = {};

  vector<TH1D*> vec_hist_nLSEvent_HLT_   = {};
  vector<TH1D*> vec_hist_nLSEvent_MYHLT_ = {};

  // -- needed to scale to an arbitrary lumi. (linear extrapolation in the case of the muon trigger) 
  Double_t* arr_LS_nCount_ = nullptr; // -- # conuts per LS
  Double_t* arr_LS_sumInstLumi_ = nullptr; // -- sum of inst. lumi per LS (will be averaged with h_LS_nCount_ later) 

  TH1D* h_LS_nCount_ = nullptr;
  TH1D* h_LS_sumInstLumi_ = nullptr;

  void FillHist_fromArray( TH1D* h, Double_t* arr ) {
    Int_t nBin = h->GetNbinsX();
    for(Int_t i=0; i<nBin; i++) {
      Int_t i_bin = i+1;

      Double_t value = arr[i];

      h->SetBinContent(i_bin, value);
      h->SetBinError(i_bin, 0); // -- just set as 0; it is not important for rate calculation now
    }

  }

  void Fill_nLSEvent(vector<string>* vec_firedTrigger, Int_t LSNum, vector<TString> vec_triggerToCheck, vector<TH1D*> vec_hist) {
    Int_t nTrig = (Int_t)vec_triggerToCheck.size();

    for(Int_t i_trig=0; i_trig<nTrig; i_trig++) {
      TString trigger = vec_triggerToCheck[i_trig];

      for(auto firedTrigger : *vec_firedTrigger ) {
        TString tstr_firedTrigger = firedTrigger;
        if( tstr_firedTrigger.Contains(trigger) ) {
          vec_hist[i_trig]->Fill( LSNum-0.5, weight_ ); // -- fill the corresponding LS bin (e.g. LSNum = 1 --> fill the first bin)
        }
      } // -- end of fired trigger iteration
    } // -- end of triggerToCheck iteration
  }

  void Init_Hist() {
    Int_t nMaxLS = 1000;

    // -- same order: vec_trigger_HLT_ and vec_hist_nLSEvent_HLT_
    for(Int_t i=0; i<(Int_t)vec_trigger_HLT_.size(); i++) {
      TString trigger = vec_trigger_HLT_[i];
      TString histName = "h_nLSEvent_HLT_"+trigger;
      TH1D* h_temp = new TH1D(histName, "", nMaxLS, 0, nMaxLS);
      vec_hist_nLSEvent_HLT_.push_back( h_temp );
    }

    for(Int_t i=0; i<(Int_t)vec_trigger_MYHLT_.size(); i++) {
      TString trigger = vec_trigger_MYHLT_[i];
      TString histName = "h_nLSEvent_MYHLT_"+trigger;
      TH1D* h_temp = new TH1D(histName, "", nMaxLS, 0, nMaxLS);
      vec_hist_nLSEvent_MYHLT_.push_back( h_temp );
    }

    arr_LS_nCount_      = new Double_t[nMaxLS];
    arr_LS_sumInstLumi_ = new Double_t[nMaxLS];

    // -- initialize: essential!
    for(Int_t i=0; i<nMaxLS; i++) {
      arr_LS_nCount_[i] = 0;
      arr_LS_sumInstLumi_[i] = 0;
    }

    h_LS_nCount_      = new TH1D("h_LS_nCount",      "", nMaxLS, 0, nMaxLS);
    h_LS_sumInstLumi_ = new TH1D("h_LS_sumInstLumi", "", nMaxLS, 0, nMaxLS);
  }

};

class DataRateTool {
public:  
  DataRateTool(TString fileName) {
    Set_InputFile(fileName);
  }

  void Set_InputFile( TString fileName ) { inputFile_ = fileName; }

  TH1D* Get_h_LS_avgInstLumi() {
    if( h_LS_avgInstLumi_ == nullptr )
      MakeHist_AveragedLumi_PerLS();

    return h_LS_avgInstLumi_;
  }

  // -- HLTType = "HLT", "MYHLT"
  // -- return rate vs. LS histogram and totalRate
  TH1D* Calc_Rate( TString HLTType, TString trigger, Bool_t doScaleTo2e34, Double_t& totalRate) {
    TString histName = TString::Format("h_nLSEvent_%s_%s", HLTType.Data(), trigger.Data());
    TH1D* h_nLSEvent = PlotTool::Get_Hist(inputFile_, histName);

    Double_t overFlow = h_nLSEvent->GetBinContent(h_nLSEvent->GetNbinsX()+1);
    if( overFlow != 0 ) {
      cout << "There is overflow = " << overFlow << endl;
      cout << "---> it means there are more # lumi sections in a run: the x-range of the histogram should increase!" << endl;
    }

    if( doScaleTo2e34 ) ScaleToArbitaryInstLumi(h_nLSEvent, 20000);

    Double_t nTotEvent = 0;
    for(Int_t i=0; i<h_nLSEvent->GetNbinsX(); i++) {
      Int_t i_bin = i+1;
      nTotEvent = nTotEvent + h_nLSEvent->GetBinContent(i_bin);
    }

    // -- divide by the lumi-section time
    Double_t time_LS = 23.31;
    h_nLSEvent->Scale( 1.0 / time_LS );

    totalRate = nTotEvent / (time_LS*nActiveLS_);

    cout << TString::Format("(nTotEvent, time_LS*nActiveLS_) = (%.1lf*1100 = %.1lf, %.2lf*%d = %.2lf) --> totalRate = %.3lf", nTotEvent/1100.0, nTotEvent, time_LS, nActiveLS_, time_LS*nActiveLS_, totalRate) << endl;

    return h_nLSEvent;
  }

private:
  TString inputFile_ = "";

  TH1D* h_LS_avgInstLumi_ = nullptr;

  Int_t nActiveLS_ = 0;

  void ScaleToArbitaryInstLumi(TH1D* h_nLSEvent, Double_t givenInstLumi) {
    if( h_LS_avgInstLumi_ == nullptr )
      MakeHist_AveragedLumi_PerLS();

    Int_t nBin = h_LS_avgInstLumi_->GetNbinsX();
    for(Int_t i=0; i<nBin; i++) {
      Int_t i_bin = i+1;

      Double_t instLumi = h_LS_avgInstLumi_->GetBinContent(i_bin); // -- averaged lumi.

      if( instLumi > 0 ) { // -- for only active LS
        Double_t nEvent = h_nLSEvent->GetBinContent(i_bin);
        Double_t scale = givenInstLumi / instLumi;
        Double_t nEvent_scaled = nEvent * scale;

        h_nLSEvent->SetBinContent(i_bin, nEvent_scaled);

        Double_t error = h_nLSEvent->GetBinError(i_bin);
        Double_t error_scaled = error * scale;
        h_nLSEvent->SetBinError(i_bin, error_scaled);

        // cout << TString::Format("[%03dth bin] (instLumi, scale, nEvent, nEvent_scaled) = (%.1lf, %.3lf, %.1lf, %.1lf)", i_bin, instLumi, scale, nEvent, nEvent_scaled) << endl;
      }
    }
  }

  void MakeHist_AveragedLumi_PerLS() {
    TH1D* h_LS_nCount      = PlotTool::Get_Hist(inputFile_, "h_LS_nCount");
    TH1D* h_LS_sumInstLumi = PlotTool::Get_Hist(inputFile_, "h_LS_sumInstLumi");

    h_LS_avgInstLumi_ = (TH1D*)h_LS_sumInstLumi->Clone();

    Int_t nBin = h_LS_avgInstLumi_->GetNbinsX();
    for(Int_t i=0; i<nBin; i++) {
      Int_t i_bin = i+1;

      Double_t nCount      = h_LS_nCount->GetBinContent(i_bin);
      Double_t sumInstLumi = h_LS_sumInstLumi->GetBinContent(i_bin);

      Double_t avgInstLumi = 0;
      if( nCount != 0 )
        avgInstLumi = sumInstLumi / nCount;

      h_LS_avgInstLumi_->SetBinContent(i_bin, avgInstLumi);

      // cout << TString::Format("[%03dth bin] (nCount, sumInstLumi, avgInstLumi) = (%.1lf, %.1lf, %.1lf)", i_bin, nCount, sumInstLumi, avgInstLumi) << endl;

      if( avgInstLumi > 0 ) nActiveLS_++; // -- for total rate calculation
    }

    cout << "nActiveLS_ = " << nActiveLS_ << endl;
  }

};