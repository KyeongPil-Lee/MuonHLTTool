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
    Int_t i_LSBin = ntuple->lumiBlockNum;
    if( h_LSInstLumi_->GetBinContent(i_LSBin) == -1.0 ) { // -- if it is not filled yet
      h_LSInstLumi_->SetBinContent(i_LSBin, ntuple->instLumi);
    }

    Fill_nLSEvent(ntuple->vec_firedTrigger,   ntuple->lumiBlockNum, vec_trigger_HLT_,   vec_hist_nLSEvent_HLT_);
    Fill_nLSEvent(ntuple->vec_myFiredTrigger, ntuple->lumiBlockNum, vec_trigger_MYHLT_, vec_hist_nLSEvent_MYHLT_);
  }

  void Save() {
    h_LSInstLumi_->Write();

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

  // -- inst. lumi per lumi section: needed to scale to an arbitrary lumi. (linear extrapolation in the case of the muon trigger)
  TH1D* h_LSInstLumi_ = nullptr;

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


    h_LSInstLumi_ = new TH1D("h_LSInstLumi", "", nMaxLS, 0, nMaxLS);
    // -- init. all bin contents with -1.0
    for(Int_t i=0; i<h_LSInstLumi_->GetNbinsX(); i++) {
      Int_t i_bin = i+1;
      h_LSInstLumi_->SetBinContent(i_bin, -1.0);
    }
  }

};

class DataRateTool {
public:  
  DataRateTool(TString fileName) {
    Set_InputFile(fileName);
  }

  void Set_InputFile( TString fileName ) { inputFile_ = fileName; }

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

    totalRate = nTotEvent / time_LS;

    return h_nLSEvent;
  }

private:
  TString inputFile_ = "";

  void ScaleToArbitaryInstLumi(TH1D* h_nLSEvent, Double_t givenInstLumi) {
    TH1D* h_LSInstLumi = PlotTool::Get_Hist(inputFile_, "h_LSInstLumi");
    Int_t nBin = h_LSInstLumi->GetNbinsX();
    for(Int_t i=0; i<nBin; i++) {
      Int_t i_bin = i+1;

      Double_t nEvent = h_nLSEvent->GetBinContent(i_bin);

      Double_t instLumi = h_LSInstLumi->GetBinContent(i_bin);
      Double_t scale = givenInstLumi / instLumi;

      Double_t nEvent_scaled = nEvent * scale;

      h_nLSEvent->SetBinContent(i_bin, nEvent_scaled);
    }
  }

};