#include <Include/Object.h>

namespace MuonIDEffTool
{

// TString ntuplePath = "/Users/KyeongPil_Lee/ServiceWorks/MuonHLTTool/v04_MuonHLTID/NtupleAnalyzer/TagProbe/IDoverL1/ntuple_99.root";
TString ntuplePath = "/scratch/kplee/MuonHLTNtuple/v20191218_SingleMuon_Run2018D_Last5Invfb/*.root";

// -- reference: https://github.com/cms-sw/cmssw/blob/master/DataFormats/MuonReco/src/MuonSelectors.cc#L899-L911
Bool_t MuonID_N( MuonHLT::IterL3MuonNoID muon )
{
  Bool_t flag = kFALSE;

  if( !muon.TMOST ) return flag;

  Bool_t layer_requirements = (muon.nTrackerLayer > 5 && muon.nPixelLayer > 0);
  Bool_t match_requirements = (muon.expectedNMatchedStation < 2 || muon.nMatchedStation > 1 || muon.pt < 8 );

  return layer_requirements && match_requirements;
}

Bool_t MuonID_N_minus_TMOST( MuonHLT::IterL3MuonNoID muon )
{
  Bool_t flag = kFALSE;

  // if( !muon.TMOST ) return flag;

  Bool_t layer_requirements = (muon.nTrackerLayer > 5 && muon.nPixelLayer > 0);
  Bool_t match_requirements = (muon.expectedNMatchedStation < 2 || muon.nMatchedStation > 1 || muon.pt < 8 );

  return layer_requirements && match_requirements;
}

Bool_t MuonID_N_minus_layer( MuonHLT::IterL3MuonNoID muon )
{
  Bool_t flag = kFALSE;

  if( !muon.TMOST ) return flag;

  // Bool_t layer_requirements = (muon.nTrackerLayer > 5 && muon.nPixelLayer > 0);
  Bool_t match_requirements = (muon.expectedNMatchedStation < 2 || muon.nMatchedStation > 1 || muon.pt < 8 );

  // return layer_requirements && match_requirements;
  return match_requirements;
}

Bool_t MuonID_N_minus_match( MuonHLT::IterL3MuonNoID muon )
{
  Bool_t flag = kFALSE;

  if( !muon.TMOST ) return flag;

  Bool_t layer_requirements = (muon.nTrackerLayer > 5 && muon.nPixelLayer > 0);
  // Bool_t match_requirements = (muon.expectedNMatchedStation < 2 || muon.nMatchedStation > 1 || muon.pt < 8 );

  // return layer_requirements && match_requirements;
  return layer_requirements;
}

Bool_t MuonID_N_minus_nTrackerLayer( MuonHLT::IterL3MuonNoID muon )
{
  Bool_t flag = kFALSE;

  if( !muon.TMOST ) return flag;

  // Bool_t layer_requirements = (muon.nTrackerLayer > 5 && muon.nPixelLayer > 0);
  Bool_t layer_requirements = muon.nPixelLayer > 0;
  Bool_t match_requirements = (muon.expectedNMatchedStation < 2 || muon.nMatchedStation > 1 || muon.pt < 8 );

  return layer_requirements && match_requirements;
}

Bool_t MuonID_N_minus_nPixelLayer( MuonHLT::IterL3MuonNoID muon )
{
  Bool_t flag = kFALSE;

  if( !muon.TMOST ) return flag;

  // Bool_t layer_requirements = (muon.nTrackerLayer > 5 && muon.nPixelLayer > 0);
  Bool_t layer_requirements = muon.nTrackerLayer > 5;
  Bool_t match_requirements = (muon.expectedNMatchedStation < 2 || muon.nMatchedStation > 1 || muon.pt < 8 );

  return layer_requirements && match_requirements;
}

Bool_t MuonID( TString type, MuonHLT::IterL3MuonNoID muon )
{
  if( type == "N" )                          return MuonID_N( muon );
  else if( type == "N_minus_TMOST" )         return MuonID_N_minus_TMOST( muon );
  else if( type == "N_minus_layer" )         return MuonID_N_minus_layer( muon );
  else if( type == "N_minus_match" )         return MuonID_N_minus_match( muon );
  else if( type == "N_minus_nTrackerLayer" ) return MuonID_N_minus_nTrackerLayer( muon );
  else if( type == "N_minus_nPixelLayer" )   return MuonID_N_minus_nPixelLayer( muon );
  else
  {
    cout << "type = " << type << "is not recognizable ... return false" << endl;
    return kFALSE;
  }

  return kFALSE;
}

}; // -- end of namespace