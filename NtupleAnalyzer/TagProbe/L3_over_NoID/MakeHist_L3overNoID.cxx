#include <TStopwatch.h>

#include <TagProbe/TnPTool.h>

namespace TnPTool
{

// -- define Tag & Probe condition outside of TnPPair: 
// ---- to use it in HistProducer (only loop over the muons passing tag or probe condition to reduce the runtime)
Bool_t IsTag(MuonHLT::Muon mu, MuonHLT::NtupleHandle* ntuple)
{
  // -- IsoMu24 final filter: hltL3crIsoL1sSingleMu22L1f0L2f10QL3f24QL3trkIsoFiltered0p07 -- //
  // -- IsoMu27 final filter: hltL3crIsoL1sMu22Or25L1f0L2f10QL3f27QL3trkIsoFiltered0p07 -- //
  // -- Mu50 final filter: hltL3fL1sMu22Or25L1f0L2f10QL3Filtered50Q -- //
  Bool_t flag = kFALSE;

  if( MuonHLT::dRMatching_HLTObj(mu.vecP, ntuple, "hltL3crIsoL1sSingleMu22L1f0L2f10QL3f24QL3trkIsoFiltered0p07::HLT", 0.1) &&
      mu.pt > 26 &&
      fabs(mu.eta) < 2.4 &&
      mu.isTight && 
      mu.relPFIso_dBeta < 0.15 )
    flag = kTRUE;

  return flag;
}

Bool_t IsProbe(MuonHLT::Muon mu, MuonHLT::NtupleHandle* ntuple)
{
  Bool_t flag = kFALSE;

  if( mu.isTight && 
      mu.relPFIso_dBeta < 0.15 && 
      MuonHLT::dRMatching_IterL3MuonNoID(mu.vecP, ntuple, -1.0, 0.1) )
    flag = kTRUE;

  return flag;
}

Bool_t IsPassingProbe(MuonHLT::Muon mu, MuonHLT::NtupleHandle* ntuple)
{
  Bool_t flag = kFALSE;
  if( MuonHLT::dRMatching_L3Muon(mu.vecP, ntuple, -1.0, 0.1) )
    flag = kTRUE;

  return flag;
}

};



class TnPPair: public MuonHLT::TnPPairBase
{
public:
  TnPPair( MuonHLT::Muon tagCand, MuonHLT::Muon probeCand, MuonHLT::NtupleHandle* ntuple ): 
  TnPPairBase( tagCand, probeCand, ntuple )
  {

  }

  // -- user-defined tag condition -- //
  Bool_t IsTag() 
  {
    return TnPTool::IsTag(tag_, ntuple_);
  }

  Bool_t IsProbe()
  {
    return TnPTool::IsProbe(probe_, ntuple_);
  }

  Bool_t IsPassingProbe()
  {
    return TnPTool::IsPassingProbe(probe_, ntuple_);
  }
};

class HistProducer
{
public:
  void Set_OutputFileName( TString outputFileName )
  {
    outputFileName_ = outputFileName;
  }

  // void AddDataPath( TString dataPath )
  // {
  //   printf("[Add data path: %s]\n", dataPath.Data() );
  //   vec_dataPath.push_back( dataPath );
  // }

  void Set_minPt( Double_t value ) { minPt_ = value; }

  void Set_ntupleListFile( TString fileName ) { fileName_ntupleList_ = fileName; }

  void Produce()
  {
    StartTimer();

    TChain* chain = new TChain("ntupler/ntuple");
    // for(const auto& dataPath : vec_dataPath )
    //   chain->Add(dataPath);
    MuonHLT::AddNtupleToChain( chain, fileName_ntupleList_ );

    MuonHLT::NtupleHandle* ntuple = new MuonHLT::NtupleHandle( chain );
    // ntuple->TurnOnBranches_GenParticle();
    ntuple->TurnOnBranches_Muon();
    ntuple->TurnOnBranches_HLTMuon();
    ntuple->TurnOnBranches_IterL3Muon();

    MuonHLT::TnPHistProducer* tnpHist = new MuonHLT::TnPHistProducer(minPt_);

    Int_t nEvent = chain->GetEntries();
    std::cout << "[Total event: " << nEvent << "]" << std::endl;

    for(Int_t i_ev=0; i_ev<nEvent; i_ev++)
    {
      MuonHLT::loadBar(i_ev+1, nEvent, 100, 100);
      ntuple->GetEvent(i_ev);

      Double_t weight = ntuple->isRealData? 1.0 : ntuple->genEventWeight;

      // -- make tag&pobe pair
      for(Int_t i_tagCand=0; i_tagCand<ntuple->nMuon; i_tagCand++)
      {
        MuonHLT::Muon mu_tagCand( ntuple, i_tagCand );
        if( !TnPTool::IsTag(mu_tagCand, ntuple) ) continue; // -- check here to save runtime

        // -- collect the probes sharing same tag
        vector<TnPPair*> vec_tnpPairs_sameTag;

        for(Int_t i_probeCand=0; i_probeCand<ntuple->nMuon; i_probeCand++)
        {
          // -- remove the case when tag muon == probe muon
          if( i_tagCand == i_probeCand ) continue;

          MuonHLT::Muon mu_probeCand( ntuple, i_probeCand );
          if( !TnPTool::IsProbe(mu_probeCand, ntuple) ) continue; // -- check here to save runtime

          TnPPair *tnpPair_ij = new TnPPair( mu_tagCand, mu_probeCand, ntuple );
          if( tnpPair_ij->IsValid() )
            vec_tnpPairs_sameTag.push_back( tnpPair_ij );
          else
            delete tnpPair_ij;
        } // -- end of iteration for the probe candidate

        // -- fill TnP histogram only when probeMultiplicity == 1
        if( (Int_t)vec_tnpPairs_sameTag.size() == 1 ) 
          tnpHist->Fill( vec_tnpPairs_sameTag[0], weight );

        for( auto tnpPair : vec_tnpPairs_sameTag )
          delete tnpPair;

      } // -- end of iteration for tag candidate

    } // -- end of event iteration

    TFile *f_output = TFile::Open(outputFileName_, "RECREATE");
    tnpHist->Save( f_output );
    f_output->Close();

    delete ntuple;
    delete chain;

    PrintRunTime();
  }

private:
  TStopwatch timer_;

  TString outputFileName_;
  vector<TString> vec_dataPath;
  Double_t minPt_;

  TString fileName_ntupleList_ = "";

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
};


void MakeHist_L3overNoID(TString textFile_ntupleList)
{
  TString tag = textFile_ntupleList.Data();
  tag.ReplaceAll(".txt", "");
  TString outputFileName = TString::Format("ROOTFile_TnPHist_%s.root", tag.Data());

  HistProducer* histProducer = new HistProducer();
  histProducer->Set_OutputFileName(outputFileName);
  histProducer->Set_ntupleListFile(textFile_ntupleList);
  histProducer->Set_minPt( 26 ); // -- min pT applied for eta, phi and vtx

  histProducer->Produce();
}
