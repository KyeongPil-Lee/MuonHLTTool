#include <Run3Iso/Include/CommonTool.h>
#include <Run3Iso/Include/MuonHandle.h>
// #include <Run3Iso/Include/GeneralTrackHandle.h>
#include <Run3Iso/Include/L3MuonHandle.h>
#include <Run3Iso/Include/MYHLTObjectHandle.h>

#include <TTreeReader.h>
#include <TH1D.h>
#include <TChain.h>
#include <TFile.h>
#include <Math/VectorUtil.h>

#include <iostream>

using namespace MuonHLT;

class HistContainer {
public:
  HistContainer(TString type): type_(type) {
    Init();
  }

  void Save() { histSet_->Write(); }

  void Fill(const MuonHLT::L3Muon& mu, Double_t weight) {
    histSet_->Fill("pt",    mu.pt, weight);
    histSet_->Fill("trkPt", mu.trkPt, weight);
    histSet_->Fill("eta",   mu.eta, weight);
    histSet_->Fill("phi",   mu.phi, weight);

    histSet_->Fill("relECALIso", mu.relECALIso, weight);
    histSet_->Fill("relHCALIso", mu.relHCALIso, weight);
    histSet_->Fill("relTrkIso",  mu.relTrkIso, weight);

    Double_t combIso = mu.ECALIso + mu.HCALIso + mu.trkIso;
    Double_t relCombIso = (mu.pt == 0) ? 1e10 : combIso / mu.pt;

    histSet_->Fill("relCombIso", relCombIso, weight);
  }

private:
  TString type_ = "";
  HistSet* histSet_;

  void Init() {
    histSet_ = new HistSet(type_);

    histSet_->Register("pt", 10000, 0, 10000);
    histSet_->Register("trkPt", 10000, 0, 10000);
    histSet_->Register("eta", 60, -3, 3);
    histSet_->Register("phi", 80, -4, 4);

    histSet_->Register("relECALIso", 2000, -1, 1);
    histSet_->Register("relHCALIso", 2000, -1, 1);
    histSet_->Register("relTrkIso", 2000, -1, 1);
    histSet_->Register("relCombIso", 2000, -1, 1);
  }

};

vector<MuonHLT::L3Muon> Get_MatchedL3Muons(vector<MuonHLT::L3Muon> vec_l3Muon, vector<MuonHLT::MYHLTObject> vec_obj, Double_t minDR) {
  vector<MuonHLT::L3Muon> vec_matched;
  vec_matched.clear();

  Int_t nL3Muon = (Int_t)vec_l3Muon.size();

  for(const auto& obj : vec_obj ) {
    Double_t dR_best = 1e10;
    Int_t index_best = -1;

    for(Int_t i_l3=0; i_l3<nL3Muon; ++i_l3) {
      Double_t dR = ROOT::Math::VectorUtil::DeltaR(obj.vecP, vec_l3Muon[i_l3].vecP);

      if( dR < minDR && dR < dR_best ) {
        dR_best = dR;
        index_best = i_l3;
      }
    } // -- iteration over L3

    if( index_best > 0 )
      vec_matched.push_back( vec_l3Muon[index_best] );
  } // -- iteration over objects

  return vec_matched;
}

void ProduceHist_Isolation(TString sampleType, TString i_job = "") {  
  // -- isolation setting
  // Double_t dRCut_inner = 0.01;
  // Double_t dRCut_outer = 0.3;
  TString filterName_ECAL = "hltL3crIsoL1sSingleMu22L1f0L2f10QL3f24QL3pfecalIsoRhoFiltered::MYHLT";
  TString filterName_HCAL = "hltL3crIsoL1sSingleMu22L1f0L2f10QL3f24QL3pfhcalIsoRhoFiltered::MYHLT";
  TString filterName_last = "hltL3crIsoL1sSingleMu22L1f0L2f10QL3f24QL3trkIsoFiltered::MYHLT";

  // -- histogram setting
  HistContainer hist_prompt("prompt");
  HistContainer hist_nonprompt("nonprompt");

  // -- passing the last filter as well (for validation)
  HistContainer hist_last_prompt("last_prompt");
  HistContainer hist_last_nonprompt("last_nonprompt");

  // -- sumWeight
  TH1D* h_sumWeight = new TH1D("h_sumWeight", "", 3, 0, 3);
  h_sumWeight->GetXaxis()->SetBinLabel(1, "sumWeight");
  h_sumWeight->GetXaxis()->SetBinLabel(2, "sumWeight_pos");
  h_sumWeight->GetXaxis()->SetBinLabel(3, "sumWeight_neg");


  TChain* chain = new TChain("ntupler/ntuple");
  if( sampleType == "test" )   { chain->Add("Example/ntuple_DY.root"); i_job = "test"; }
  else                         AddNtupleToChain(chain, "fileList.txt"); // -- txt file name: fixed

  TTreeReader* reader = new TTreeReader(chain);
  TTreeReaderValue<double> genEventWeight(*reader, "genEventWeight");

  MuonHandle* muonHandle            = new MuonHandle(*reader);
  L3MuonHandle* l3MuonHandle        = new L3MuonHandle(*reader);
  MYHLTObjectHandle* myHLTObjHandle = new MYHLTObjectHandle(*reader);

  Int_t nEvent_tot = reader->GetEntries(kTRUE);
  cout << "nEvent_tot = " << nEvent_tot << endl;

  // nEvent_tot = 100;
  for(Int_t i_ev=0; i_ev<nEvent_tot; ++i_ev) {
    // printf("\n[%d event]\n", i_ev);

    reader->SetEntry(i_ev);

    Double_t genWeight = (*genEventWeight < 0 ) ? -1.0 : 1.0; // -- just take sign
    h_sumWeight->Fill("sumWeight", genWeight);
    if( genWeight > 0 ) h_sumWeight->Fill("sumWeight_pos", genWeight);
    else                h_sumWeight->Fill("sumWeight_neg", genWeight);

    // vector<MuonHLT::MYHLTObject> vec_myHLTObj = GetAll_MYHLTObject(myHLTObjHandle);
    // for(const auto& myHLTObj : vec_myHLTObj) {
    //   printf("  filterName = %s\n", myHLTObj.filterName.Data());
    // }

    vector<MuonHLT::Muon> vec_muon = GetAll_Muon(muonHandle);
    vector<MuonHLT::L3Muon> vec_l3Muon = GetAll_L3Muon(l3MuonHandle);
    vector<MuonHLT::MYHLTObject> vec_myHLTObj_passHCAL = GetAll_MYHLTObject(myHLTObjHandle, filterName_HCAL);
    vector<MuonHLT::MYHLTObject> vec_myHLTObj_passLast = GetAll_MYHLTObject(myHLTObjHandle, filterName_last);

    vector<ROOT::Math::PtEtaPhiMVector> vec_vecP_offMu_prompt;
    for(const auto& mu : vec_muon) {
      Bool_t isPrompt = (mu.simType == 4);
      if( isPrompt )
        vec_vecP_offMu_prompt.push_back( mu.vecP );
    }

    // vector<ROOT::Math::PtEtaPhiMVector> vec_vecP_passHCAL;
    // for(const auto& myHLTObj : vec_myHLTObj_passHCAL) {
    //   // printf("  [passHCAL] (pt, eta, phi) = (%.3lf, %.3lf, %.3lf)\n", myHLTObj.pt, myHLTObj.eta, myHLTObj.phi);
    //   vec_vecP_passHCAL.push_back( myHLTObj.vecP );
    // }

    // vector<ROOT::Math::PtEtaPhiMVector> vec_vecP_passLast;
    // for(const auto& myHLTObj : vec_myHLTObj_passLast) {
    //   // printf("  [passLast] (pt, eta, phi) = (%.3lf, %.3lf, %.3lf)\n", myHLTObj.pt, myHLTObj.eta, myHLTObj.phi);
    //   vec_vecP_passLast.push_back( myHLTObj.vecP );
    // }

    vector<MuonHLT::L3Muon> vec_l3Muon_passHCAL = Get_MatchedL3Muons(vec_l3Muon, vec_myHLTObj_passHCAL, 0.1);
    for(const auto& l3Mu : vec_l3Muon_passHCAL) {
      Bool_t matched_prompt = MuonHLT::dRMatching(l3Mu.vecP, vec_vecP_offMu_prompt, 0.1);
      if( matched_prompt ) hist_prompt.Fill(l3Mu, genWeight);
      else                 hist_nonprompt.Fill(l3Mu, genWeight);

      // if( l3Mu.pt < 24.0 ) {
      //   printf("  [L3 muon] (pt, eta, phi, relECALIso, relHCALIso, relTrkIso) = (%.3lf, %.3lf, %.3lf, %.3lf, %.3lf, %.3lf)", 
      //          l3Mu.pt, l3Mu.eta, l3Mu.phi, l3Mu.relECALIso, l3Mu.relHCALIso, l3Mu.relTrkIso);
      // }
    }

    vector<MuonHLT::L3Muon> vec_l3Muon_passLast = Get_MatchedL3Muons(vec_l3Muon, vec_myHLTObj_passLast, 0.1);
    for(const auto& l3Mu : vec_l3Muon_passLast) {
      Bool_t matched_prompt = MuonHLT::dRMatching(l3Mu.vecP, vec_vecP_offMu_prompt, 0.1);
      if( matched_prompt ) hist_last_prompt.Fill(l3Mu, genWeight);
      else                 hist_last_nonprompt.Fill(l3Mu, genWeight);
    }



    // Bool_t strangeEvent = kFALSE;
    // for(const auto& l3Mu : vec_l3Muon) {
      
      // Bool_t matched_passHCAL = MuonHLT::dRMatching(l3Mu.vecP, vec_vecP_passHCAL, 0.1);
      // if( !matched_passHCAL ) continue;

      // Bool_t matched_prompt = MuonHLT::dRMatching(l3Mu.vecP, vec_vecP_offMu_prompt, 0.1);
      // if( matched_prompt ) hist_prompt.Fill(l3Mu, genWeight);
      // else                 hist_nonprompt.Fill(l3Mu, genWeight);


      // Bool_t matched_passLast = MuonHLT::dRMatching(l3Mu.vecP, vec_vecP_passLast, 0.1);
      // if( matched_passLast ) {
      //   if( matched_prompt ) hist_last_prompt.Fill(l3Mu, genWeight);
      //   else                 hist_last_nonprompt.Fill(l3Mu, genWeight);
      // }


      // printf("[L3 muon]");
      // if( matched_passHCAL ) printf(" *matched_passHCAL*");
      // if( matched_passLast ) printf(" *matched_passLast*");
      // if( matched_prompt )   printf(" *matched_prompt*");
      // printf("\n");


      // Bool_t matched_passHCAL = MuonHLT::dRMatching(l3Mu.vecP, vec_vecP_passHCAL, 0.1);
      // if( matched_passHCAL && l3Mu.pt < 24.0 )
      //   strangeEvent = kTRUE;

    // }


    // if( strangeEvent ) {
    //   for(const auto& myHLTObj : vec_myHLTObj_passHCAL) {
    //     printf("  [passHCAL] (pt, eta, phi) = (%.3lf, %.3lf, %.3lf)\n", myHLTObj.pt, myHLTObj.eta, myHLTObj.phi);
    //   }

    //   for(const auto& l3Mu : vec_l3Muon) {
    //     printf("  [L3 muon] (pt, eta, phi, relECALIso, relHCALIso, relTrkIso) = (%.3lf, %.3lf, %.3lf, %.3lf, %.3lf, %.3lf)", 
    //            l3Mu.pt, l3Mu.eta, l3Mu.phi, l3Mu.relECALIso, l3Mu.relHCALIso, l3Mu.relTrkIso);
    //     if( MuonHLT::dRMatching(l3Mu.vecP, vec_vecP_passHCAL, 0.1) )
    //       printf(" --> matched to passHCAL\n");
    //     else
    //       printf("\n");
    //   }
    // }

  }

  TString outputFileName = TString::Format("IsolationHist_%s.root", i_job.Data());
  TFile *f_output = TFile::Open(outputFileName, "RECREATE");
  f_output->cd();

  h_sumWeight->Write();
  hist_prompt.Save();
  hist_nonprompt.Save();
  hist_last_prompt.Save();
  hist_last_nonprompt.Save();

  f_output->Close();

  printf("Done - saved in the file [%s]\n", outputFileName.Data());
}

void ProduceHist_Isolation() { ProduceHist_Isolation("test"); }