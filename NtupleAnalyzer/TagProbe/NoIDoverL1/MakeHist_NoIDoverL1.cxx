#include <TagProbe/TnPTool.h>
#include <TagProbe/IDoverL1/MuonIDEffTool.h>

class TnPPair: public MuonHLT::TnPPairBase
{
public:
  TnPPair( MuonHLT::Muon tagCand, MuonHLT::Muon probeCand, MuonHLT::NtupleHandle* ntuple ): 
  TnPPairBase( tagCand, probeCand, ntuple )
  {

  }

  // -- user-defined tag condition -- //
  Bool_t IsTag() {
    // -- IsoMu24 final filter: hltL3crIsoL1sSingleMu22L1f0L2f10QL3f24QL3trkIsoFiltered0p07 -- //
    // -- IsoMu27 final filter: hltL3crIsoL1sMu22Or25L1f0L2f10QL3f27QL3trkIsoFiltered0p07 -- //
    // -- Mu50 final filter: hltL3fL1sMu22Or25L1f0L2f10QL3Filtered50Q -- //
    Bool_t flag = kFALSE;

    if( MuonHLT::dRMatching_HLTObj(tag_.vecP, ntuple_, "hltL3crIsoL1sSingleMu22L1f0L2f10QL3f24QL3trkIsoFiltered0p07::HLT", 0.1) &&
        tag_.pt > 26 &&
        fabs(tag_.eta) < 2.4 &&
        tag_.isTight && 
        tag_.relPFIso_dBeta < 0.15 )
      flag = kTRUE;

    // if( flag )
    //   cout << "Tag is found" << endl;

    return flag;
  }

  Bool_t IsProbe()
  {
    Bool_t flag = kFALSE;

    if( probe_.isTight && 
        probe_.relPFIso_dBeta < 0.15 && 
        MuonHLT::dRMatching_L1Muon(probe_.vecP, ntuple_, 22, 0.3) )
      flag = kTRUE;

    return flag;
  }

  Bool_t IsPassingProbe()
  {
    Bool_t flag = kFALSE;
    if( dRMatching_IterL3MuonNoID(probe_.vecP, ntuple_, -1, 0.1) )
      flag = kTRUE;

    return flag;
  }

  Bool_t dRMatching_IterL3MuonNoID( TLorentzVector vecP_ref, MuonHLT::NtupleHandle* ntuple, Double_t minPt, Double_t minDR )
  {
    vector<MuonHLT::IterL3MuonNoID> vec_IterL3MuonNoID = MuonHLT::GetAllIterL3MuonNoID( ntuple, minPt );
    vector<TLorentzVector> vec_vecP_IterL3MuonNoID;
    for(const auto& IterL3MuonNoID : vec_IterL3MuonNoID )
      vec_vecP_IterL3MuonNoID.push_back( IterL3MuonNoID.vecP );

    return MuonHLT::dRMatching( vecP_ref, vec_vecP_IterL3MuonNoID, minDR );
  }
};

class HistProducer
{
public:
  void Set_OutputFileName( TString outputFileName )
  {
    outputFileName_ = outputFileName;
  }

  void AddDataPath( TString dataPath )
  {
    printf("[Add data path: %s]\n", dataPath.Data() );
    vec_dataPath.push_back( dataPath );
  }

  void Set_minPt( Double_t value ) { minPt_ = value; }

  void Produce()
  {
    StartTimer();

    TChain* chain = new TChain("ntupler/ntuple");
    for(const auto& dataPath : vec_dataPath )
      chain->Add(dataPath);

    MuonHLT::NtupleHandle* ntuple = new MuonHLT::NtupleHandle( chain );
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

        // -- collect the probes sharing same tag
        vector<TnPPair*> vec_tnpPairs_sameTag;

        for(Int_t i_probeCand=0; i_probeCand<ntuple->nMuon; i_probeCand++)
        {
          // -- remove the case when tag muon == probe muon
          if( i_tagCand == i_probeCand ) continue;

          MuonHLT::Muon mu_probeCand( ntuple, i_probeCand );

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

  void StartTimer()
  {
    timer_.Start();
  }

  void PrintRunTime()
  {
    Double_t cpuTime = timer_.CpuTime();
    Double_t realTime = timer_.RealTime();

    cout << "************************************************" << endl;
    cout << "Total real time: " << realTime << " seconds = " << realTime / 60.0 << " minutes = " << realTime / (60.0*60.0) << " hours" << endl;
    cout << "Total CPU time:  " << cpuTime  << " seconds = " << cpuTime / 60.0  << " minutes = " << cpuTime / (60.0*60.0)  << " hours" << endl;
    cout << "  CPU time / real time = " << cpuTime / realTime << endl;
    cout << "************************************************" << endl;
  }
};


void MakeHist_NoIDoverL1()
{
  HistProducer* histProducer = new HistProducer();
  histProducer->Set_OutputFileName("ROOTFile_TnPHist.root");
  // histProducer->AddDataPath("/Users/KyeongPil_Lee/ServiceWorks/MuonHLTTool/v04_MuonHLTID/NtupleAnalyzer/TagProbe/IDoverL1/ntuple_99.root");
  histProducer->AddDataPath(MuonIDEffTool::ntuplePath);
  histProducer->Set_minPt( 24 ); // -- min pT applied for eta, phi and vtx

  histProducer->Produce();
}
