#define nDataset (1)
#include <RateAndPurity/TriggerPurityTool.h>

void MuonTriggerPurity( TString Trigger, TString Dataset, TString Version, Double_t EtaLo = 0.0, Double_t EtaUp = 5.0,
                        TString JobId = "", vector< TString > vec_Dataset = {} )
{
  vector< TString > vec_Data = vec_Dataset;

  if(vec_Dataset.size()==0) {
    vec_Data = {"../ntuple_77.root"};  // {"../ntuple_2001.root"};
  }

  vector< TString > vec_Sel = {
    "Mat0_IdNo_IsoNo_SimNo_HLT",

    "Mat1_IdNo_IsoNo_SimNo_HLT",

    "Mat1_IdPfOrTrkOrGlb_IsoNo_SimNo_HLT",

    "Mat1_IdTight_IsoNo_SimNo_HLT",
    "Mat1_IdTight_IsoTkLoose_SimNo_HLT",
    "Mat1_IdTight_IsoPFTight_SimNo_HLT",

    "Mat1_IdHighPt_IsoNo_SimNo_HLT",
    "Mat1_IdHighPt_IsoTkLoose_SimNo_HLT",
    "Mat1_IdHighPt_IsoPFTight_SimNo_HLT"
  };



  vector< Int_t > vec_RunNum = { 0, 999999 };
  TString outDir = ".";
  TString fileName = TString::Format("%s/Output-MuonTriggerPurity-%s-%s-%s-%.1f-%.1f.root", outDir.Data(), Dataset.Data(), Version.Data(), Trigger.Data(), EtaLo, EtaUp );
  if(JobId != "")
    fileName = TString::Format("Output-MuonTriggerPurity-%s-%s-%s-%.1f-%.1f--%s.root", Dataset.Data(), Version.Data(), Trigger.Data(), EtaLo, EtaUp, JobId.Data() );
  TFile *f_output = TFile::Open(fileName, "RECREATE");

  TString Filter = "";
  if( Trigger == "HLT_IsoMu24_v" ){
    if( Dataset.Contains("Data2016") || Dataset.Contains("80X") ) {
      Filter = "hltL3crIsoL1sMu22L1f0L2f10QL3f24QL3trkIsoFiltered0p09";
    }
    else if( Dataset.Contains("Data2018") || Dataset.Contains("102X") ) {
      Filter = "hltL3crIsoL1sSingleMu22L1f0L2f10QL3f24QL3trkIsoFiltered0p07";
    }
  }

  else if( Trigger == "HLT_Mu24_v" ) {
    if( Dataset.Contains("Data2016") || Dataset.Contains("80X") ) {
      Filter = "hltL3fL1sMu22L1f0L2f10QL3Filtered24Q";
    }
    else if( Dataset.Contains("Data2018") || Dataset.Contains("102X") ) {
      Filter = "hltL3fL1sSingleMu22L1f0L2f10QL3Filtered24Q";
    }
  }

  else if( Trigger == "HLT_Mu50_v" ) {
    Filter = "hltL3fL1sMu22Or25L1f0L2f10QL3Filtered50Q";
  }

  else if( Trigger == "HLT_IsoTkMu24_v" ){
    Filter = "hltL3fL1sMu22L1f0Tkf24QL3trkIsoFiltered0p09";
  }

  else if( Trigger == "HLT_TkMu50_v" ){
    Filter = "hltL3fL1sMu25f0TkFiltered50Q";
  }

  else if( Trigger == "HLT_IsoMu24_v_Or_HLT_IsoTkMu24_v" ){
    Filter = "hltL3crIsoL1sMu22L1f0L2f10QL3f24QL3trkIsoFiltered0p09+hltL3fL1sMu22L1f0Tkf24QL3trkIsoFiltered0p09";
  }

  else if( Trigger == "HLT_Mu50_v_Or_HLT_TkMu50_v" ){
    Filter = "hltL3fL1sMu22Or25L1f0L2f10QL3Filtered50Q+hltL3fL1sMu25f0TkFiltered50Q";
  }

  else if( Trigger == "hltL1fL1sMu22L1Filtered0" ){
    Filter = "hltL1fL1sMu22L1Filtered0";
  }

  else if( Trigger == "hltL2fL1sMu22L1f0L2Filtered10Q" ){
    Filter = "hltL2fL1sMu22L1f0L2Filtered10Q";
  }

  else if( Trigger == "hltL2fL1sSingleMu22L1f0L2Filtered10Q" ){
    Filter = "hltL2fL1sSingleMu22L1f0L2Filtered10Q";
  }

  TriggerPurityTool* tool = new TriggerPurityTool(EtaLo, EtaUp);
  if(Version.Contains("TEST")) {
    // tool->debug = kTRUE;
    tool->Set_MaxEvents(1000);
  }
  if( Version.Contains("Dimuon") )
    tool->doDimuon = kTRUE;
  tool->Set_DataList( vec_Data );
  tool->Set_Selection( vec_Sel );
  tool->Set_RunNumRange( vec_RunNum );
  tool->Set_Trigger( Filter );
  tool->Set_Output( f_output );
  tool->Analyze();
  delete tool;

  cout << "[All finished]" << endl;
}
