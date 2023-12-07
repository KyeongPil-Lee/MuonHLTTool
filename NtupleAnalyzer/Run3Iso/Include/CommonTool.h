#pragma once

#include <TString.h>
#include <TH1D.h>
#include <map>
#include <iostream>
#include <fstream>

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
      std::cout << "no histogram corresponding to " << var << std::endl;
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

void AddNtupleToChain(TChain* chain, TString textFileName) {
  ifstream openFile( textFileName.Data() );
  cout << "===============================" << endl;
  cout << "Read " << textFileName << endl;
  cout << "===============================" << endl;

  if( openFile.is_open() ) {
    std::string line;
    while(getline(openFile, line)) {
      cout << line << endl;
      chain->Add(line.data());
    }
    openFile.close();
  }

  cout << "==================================" << endl;
  cout << "All ROOT Files are put into TChain" << endl;
  cout << "==================================" << endl;
}

Bool_t dRMatching( ROOT::Math::PtEtaPhiMVector vecP_ref, vector<ROOT::Math::PtEtaPhiMVector> vec_vecP, Double_t minDR ) {
  bool flag = kFALSE;

  Int_t nObj = (Int_t)vec_vecP.size();
  for(const auto& vecP_target: vec_vecP ) {
    // Double_t dR = vecP_ref.DeltaR( vecP_target );
    Double_t dR = ROOT::Math::VectorUtil::DeltaR(vecP_ref, vecP_target);
    if( dR < minDR ) {
      flag = kTRUE;
      break;
    }
  }

  return flag;
}

}; // -- end of namespace MuonHLT

