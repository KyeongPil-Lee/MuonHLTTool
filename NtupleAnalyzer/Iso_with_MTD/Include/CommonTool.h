#pragma once

#include <map>

namespace MuonHLT {

/*

// -- HistSet: easily control many histograms
HistSet *histSet = new HistSet("type"); // -- or put ""
histSet->Register("pt",  10000, 0,   10000); // -- it will make "h_pt" histogram with given bin info
histSet->Register("eta", 200,   -10, 10); // -- it will make "h_eta" histogram with given bin info
...
// -- end of register -- //

// -- fill part
for(auto& mu : vec_muon ) {
  ...
  histSet-->Fill( "pt", mu.pt, eventWeight );
  histSet-->Fill( "eta", mu.pt, eventWeight );
  ...
}

// -- save part
histSet->Write();

*/

class HistSet { 
public:
  HistSet(TString type) { type_ = type; }

  void Register(TString var, Int_t nBin, Double_t min, Double_t max) {

    TString histName = "h_"+var+"_"+type_;
    if( type_ == "" )
      histName = "h_"+var;

    TH1D* h_temp = new TH1D(histName, "", nBin, min, max);

    map_var_hist_.insert( std::make_pair(var, h_temp) );
  }

  void Fill(TString var, Double_t value, Double_t weight = 1.0 ) {

    auto it_pair = map_var_hist_.find(var);
    if( it_pair == map_var_hist_.end() )
      cout << "no histogram corresponding to " << var << endl;
    else
      it_pair->second->Fill( value, weight );
  }

  void Write() {
    for(auto pair : map_var_hist_ )
      pair.second->Write();
  }

private:
  TString type_;
  map<TString, TH1D*> map_var_hist_;
};

};