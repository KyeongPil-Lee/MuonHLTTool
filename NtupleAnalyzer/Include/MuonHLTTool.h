#pragma once

#include <Include/Object.h>
#include <fstream>
#include <iostream>
#include <vector>

#include <TH1.h>
#include <TSystem.h>

namespace MuonHLT
{
// -- some useful variables, functions, classes ...

vector<MuonHLT::HLTObject> GetAllHLTObject(NtupleHandle *ntuple, TString filterName = "", Double_t minPt = -1)
{
  vector<MuonHLT::HLTObject> vec_HLTObj;
  Int_t nHLTObj = (Int_t)ntuple->vec_filterName->size();
  for(Int_t i_obj=0; i_obj<nHLTObj; i_obj++)
  {
    MuonHLT::HLTObject hltObject(ntuple, i_obj);

    if( hltObject.pt < minPt ) continue;
    if( filterName != "" && hltObject.filterName != filterName ) continue;

    // -- if filterName is not specified: save all HLT object
    // -- if filterName is speficied: save HLT object with same filter name only
    vec_HLTObj.push_back( hltObject );
  }

  return vec_HLTObj;
}

vector<MuonHLT::MYHLTObject> GetAllMYHLTObject(NtupleHandle *ntuple, TString filterName = "", Double_t minPt = -1)
{
  vector<MuonHLT::MYHLTObject> vec_MYHLTObj;
  Int_t nHLTObj = (Int_t)ntuple->vec_myFilterName->size();
  for(Int_t i_obj=0; i_obj<nHLTObj; i_obj++)
  {
    MuonHLT::MYHLTObject myHLTObject(ntuple, i_obj);

    if( myHLTObject.pt < minPt ) continue;
    if( filterName != "" && myHLTObject.filterName != filterName ) continue;

    // -- if filterName is not specified: save all HLT object
    // -- if filterName is speficied: save HLT object with same filter name only
    vec_MYHLTObj.push_back( myHLTObject );
  }

  return vec_MYHLTObj;
}

vector<MuonHLT::L1Muon> GetAllL1Muon(NtupleHandle* ntuple, Double_t minPt = -1 )
{
  vector<MuonHLT::L1Muon> vec_L1Muon;
  for(Int_t i_L1=0; i_L1<ntuple->nL1Muon; i_L1++)
  {
    MuonHLT::L1Muon l1mu(ntuple, i_L1);
    if( l1mu.pt > minPt )
      vec_L1Muon.push_back( l1mu );
  }

  return vec_L1Muon;
}

vector<MuonHLT::L2Muon> GetAllL2Muon(NtupleHandle* ntuple, Double_t minPt = -1 )
{
  vector<MuonHLT::L2Muon> vec_muon;
  for(Int_t i_obj=0; i_obj<ntuple->nL2Muon; i_obj++)
  {
    MuonHLT::L2Muon mu(ntuple, i_obj);
    if( mu.pt > minPt )
      vec_muon.push_back( mu );
  }

  return vec_muon;
}

vector<MuonHLT::L3Muon> GetAllL3Muon(NtupleHandle* ntuple, Double_t minPt = -1 )
{
  vector<MuonHLT::L3Muon> vec_muon;
  for(Int_t i_obj=0; i_obj<ntuple->nL3Muon; i_obj++)
  {
    MuonHLT::L3Muon mu(ntuple, i_obj);
    if( mu.pt > minPt )
      vec_muon.push_back( mu );
  }

  return vec_muon;
}

vector<MuonHLT::IterL3MuonNoID> GetAllIterL3MuonNoID(NtupleHandle* ntuple, Double_t minPt = -1 )
{
  vector<MuonHLT::IterL3MuonNoID> vec_muon;
  for(Int_t i_obj=0; i_obj<ntuple->nIterL3MuonNoID; i_obj++)
  {
    MuonHLT::IterL3MuonNoID mu(ntuple, i_obj);
    if( mu.pt > minPt )
      vec_muon.push_back( mu );
  }

  return vec_muon;
}

vector<MuonHLT::Muon> GetAllOfflineMuon(NtupleHandle* ntuple, Double_t minPt = -1 )
{
  vector<MuonHLT::Muon> vec_muon;
  for(Int_t i_obj=0; i_obj<ntuple->nMuon; i_obj++)
  {
    MuonHLT::Muon mu(ntuple, i_obj);
    if( mu.pt > minPt )
      vec_muon.push_back( mu );
  }

  return vec_muon;
}

vector<MuonHLT::GenParticle> GetAllGenLeptons(NtupleHandle *ntuple, Int_t pdgID, TString genFlagName)
{
  vector< MuonHLT::GenParticle > vec_genLepton;
  vec_genLepton.clear();

  if( genFlagName != "isHardProcess" && 
      genFlagName != "fromHardProcessFinalState" )
  {
    cout << "[MuonHLT::GetAllGenLeptons] genFlagName = " << genFlagName << " is not supported -> empty vector is returned" << endl;
    cout << "  Update the code if you want to use " << genFlagName << endl;
    return vec_genLepton;
  }

  if( pdgID != 11 && pdgID != 13 )
  {
    cout << "[MuonHLT::GetAllGenLeptons] pdgID = " << pdgID << "is not supported -> empty vector is returned" << endl;
    cout << "  Update the code if you want to use " << pdgID << endl;
    return vec_genLepton;
  }

  for(Int_t i_gen=0; i_gen<ntuple->nGenParticle; i_gen++)
  {
    MuonHLT::GenParticle genLepton( ntuple, i_gen );

    if( abs(genLepton.ID) == pdgID )
    {
      if( genFlagName == "isHardProcess" )
      {
        if( genLepton.isHardProcess ) 
          vec_genLepton.push_back( genLepton );
      }
      else if( genFlagName == "fromHardProcessFinalState" )
      {
        if( genLepton.fromHardProcessFinalState ) 
          vec_genLepton.push_back( genLepton );
      }
    }
  }

  return vec_genLepton;
}

Bool_t dRMatching( TLorentzVector vecP_ref, vector<TLorentzVector> vec_vecP, Double_t minDR )
{
  bool flag = kFALSE;

  Int_t nObj = (Int_t)vec_vecP.size();
  for(const auto& vecP_target: vec_vecP )
  {
    Double_t dR = vecP_ref.DeltaR( vecP_target );
    if( dR < minDR )
    {
      flag = kTRUE;
      break;
    }
  }

  return flag;
}

Bool_t dRMatching_L1Muon( TLorentzVector vecP_ref, NtupleHandle* ntuple, Double_t minPt, Double_t minDR )
{
  vector<MuonHLT::L1Muon> vec_L1Muon = MuonHLT::GetAllL1Muon( ntuple, minPt );
  vector<TLorentzVector> vec_vecP_L1Muon;
  for(const auto& L1Muon : vec_L1Muon )
    vec_vecP_L1Muon.push_back( L1Muon.vecP );

  return MuonHLT::dRMatching( vecP_ref, vec_vecP_L1Muon, minDR );
}

Bool_t dRMatching_L3Muon( TLorentzVector vecP_ref, NtupleHandle* ntuple, Double_t minPt, Double_t minDR )
{
  vector<MuonHLT::L3Muon> vec_L3Muon = MuonHLT::GetAllL3Muon( ntuple, minPt );
  vector<TLorentzVector> vec_vecP_L3Muon;
  for(const auto& L3Muon : vec_L3Muon )
    vec_vecP_L3Muon.push_back( L3Muon.vecP );

  return MuonHLT::dRMatching( vecP_ref, vec_vecP_L3Muon, minDR );
}

Bool_t dRMatching_IterL3MuonNoID( TLorentzVector vecP_ref, NtupleHandle* ntuple, Double_t minPt, Double_t minDR )
{
  vector<MuonHLT::IterL3MuonNoID> vec_IterL3MuonNoID = MuonHLT::GetAllIterL3MuonNoID( ntuple, minPt );
  vector<TLorentzVector> vec_vecP_IterL3MuonNoID;
  for(const auto& IterL3MuonNoID : vec_IterL3MuonNoID )
    vec_vecP_IterL3MuonNoID.push_back( IterL3MuonNoID.vecP );

  return MuonHLT::dRMatching( vecP_ref, vec_vecP_IterL3MuonNoID, minDR );
}

Bool_t dRMatching_HLTObj( TLorentzVector vecP_ref, MuonHLT::NtupleHandle* ntuple, TString filterName, Double_t minDR )
{
  vector<MuonHLT::HLTObject> vec_HLTObj = MuonHLT::GetAllHLTObject(ntuple, filterName);
  vector<TLorentzVector> vec_vecP_HLTObj;
  for(const auto& HLTObj : vec_HLTObj )
    vec_vecP_HLTObj.push_back( HLTObj.vecP );

  return MuonHLT::dRMatching( vecP_ref, vec_vecP_HLTObj, minDR );
}

// -- same with above function, but take MYHLT object
Bool_t dRMatching_MYHLTObj( TLorentzVector vecP_ref, MuonHLT::NtupleHandle* ntuple, TString filterName, Double_t minDR )
{
  vector<MuonHLT::MYHLTObject> vec_HLTObj = MuonHLT::GetAllMYHLTObject(ntuple, filterName);
  vector<TLorentzVector> vec_vecP_HLTObj;
  for(const auto& HLTObj : vec_HLTObj )
    vec_vecP_HLTObj.push_back( HLTObj.vecP );

  return MuonHLT::dRMatching( vecP_ref, vec_vecP_HLTObj, minDR );
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

Double_t GetArg(TString fullArg)
{
  // -- extract the part with the numbers only from the full argument
  // -- example
  // ---- fullArg = "cross section: 1976.0"
  // ---- subArg = "1976.0" (the strings after : )
  TString subArg = fullArg(fullArg.Index(":") + 1, fullArg.Length());
  return TString(subArg).Atof();
}

void AddNtupleToChain(TChain* chain, TString textFileName)
{
  ifstream openFile( textFileName.Data() );
  cout << "===============================" << endl;
  cout << "Read " << textFileName << endl;
  cout << "===============================" << endl;

  if( openFile.is_open() )
  {
    string line;
    while(getline(openFile, line))
    {
      cout << line << endl;

      TString tstr_line(line);
      if( !tstr_line.BeginsWith("#") ) // -- ignore the lines starting with # (these lines could have cross section & sum(weight) info.)
        chain->Add(line.data());
    }
    openFile.close();
  }

  cout << "==================================" << endl;
  cout << "All ROOT Files are put into TChain" << endl;
  cout << "==================================" << endl;
}

// -- type = "xSec", "sumWeight"
Double_t GetInfo(TString type, TString textFileName)
{
  Bool_t isFound = kFALSE;
  Double_t value = -1.0;

  ifstream openFile( textFileName.Data() );

  if( openFile.is_open() )
  {
    string line;
    while(getline(openFile, line))
    {
      TString tstr_line(line);
      if( tstr_line.Contains("#") )
      {
        if( (type == "xSec"      && tstr_line.Contains("cross section")) ||
            (type == "sumWeight" && tstr_line.Contains("sum of weights")) )
        {
          value = GetArg(tstr_line);
          isFound = kTRUE;
        }
      }
    }
    openFile.close();
  }

  if( !isFound )
    cout << "[MuonHLT::GetInfo] Information for type " << type << " in " << textFileName << " is not found! ... return -1.0" << endl;

  return value;
}

// -- type = "sampleType"
TString GetInfo_String(TString type, TString textFileName)
{
  Bool_t isFound = kFALSE;
  TString value = "";

  ifstream openFile( textFileName.Data() );

  if( openFile.is_open() )
  {
    string line;
    while(getline(openFile, line))
    {
      TString tstr_line(line);
      if( tstr_line.Contains("#") )
      {
        if( (type == "sampleType" && tstr_line.Contains("sample type")) )
        {
          value = tstr_line(tstr_line.Index(":") + 2, tstr_line.Length()); // -- +2, not +1: remove an empty space
          isFound = kTRUE;
        }
      }
    }
    openFile.close();
  }

  if( !isFound )
    cout << "[MuonHLT::GetInfo_String] Information for type " << type << " in " << textFileName << " is not found! ... return empty string" << endl;

  return value;
}

// -- find the same L3 muon with the given MYHLT object
Bool_t Find_CorrespondingL3Muon(MuonHLT::MYHLTObject HLTObj, MuonHLT::NtupleHandle* ntuple, MuonHLT::L3Muon& theL3Muon)
{
  Bool_t isFound = kFALSE;

  vector<MuonHLT::L3Muon> vec_L3Muon = GetAllL3Muon(ntuple);

  for(auto& L3Muon : vec_L3Muon )
  {
    Double_t relDiff_pt = (L3Muon.pt - HLTObj.pt) / HLTObj.pt;
    Double_t dR = HLTObj.vecP.DeltaR( L3Muon.vecP );

    if( relDiff_pt < 0.001 && dR < 0.01 )
    {
      theL3Muon = L3Muon;
      isFound = kTRUE;
      break;
    }
  }

  return isFound;
}

Bool_t Pass_HLTIsolationFilter(MuonHLT::MYHLTObject HLTObj)
{
  Bool_t flag = kFALSE;

  if( !HLTObj.isIsoValid ) return flag;

  if( fabs(HLTObj.eta) < 1.479 ) // -- barrel
  {
    if( HLTObj.relECALIso < 0.14 && HLTObj.relHCALIso < 0.16 && HLTObj.relTrkIso < 0.07 ) flag = kTRUE;
  }
  else // -- endcap
  {
    if( HLTObj.relECALIso < 0.10 && HLTObj.relHCALIso < 0.20 && HLTObj.relTrkIso < 0.07 ) flag = kTRUE;
  }

  return flag;
}

Bool_t Pass_HLTIsoFilter_ECAL(MuonHLT::MYHLTObject HLTObj, Double_t WP_EB = 0.14, Double_t WP_EE = 0.10)
{
  Bool_t flag = kFALSE;

  if( !HLTObj.isIsoValid ) return flag;

  if( fabs(HLTObj.eta) < 1.479 ) // -- barrel
  {
    if( HLTObj.relECALIso < WP_EB ) flag = kTRUE;
  }
  else // -- endcap
  {
    if( HLTObj.relECALIso < WP_EE ) flag = kTRUE;
  }

  return flag;
}

Bool_t Pass_HLTIsoFilter_HCAL(MuonHLT::MYHLTObject HLTObj, Double_t WP_EB = 0.16, Double_t WP_EE = 0.20)
{
  Bool_t flag = kFALSE;

  if( !HLTObj.isIsoValid ) return flag;

  if( fabs(HLTObj.eta) < 1.479 ) // -- barrel
  {
    if( HLTObj.relHCALIso < WP_EB ) flag = kTRUE;
  }
  else // -- endcap
  {
    if( HLTObj.relHCALIso < WP_EE ) flag = kTRUE;
  }

  return flag;
}

Bool_t Pass_HLTIsoFilter_Tracker(MuonHLT::MYHLTObject HLTObj, Double_t WP = 0.07)
{
  Bool_t flag = kFALSE;

  if( !HLTObj.isIsoValid ) return flag;

  if( HLTObj.relTrkIso < WP ) flag = kTRUE;

  return flag;
}


}; // -- end of namespace
