// -- ntuple maker for Muon HLT study
// -- author: Kyeongpil Lee (Seoul National University, kplee@cern.ch)

#include "MuonHLTTool/MuonHLTNtupler/interface/MuonHLTNtupler.h"

// #include "FWCore/Framework/interface/stream/EDAnalyzer.h"
// #include "FWCore/Framework/interface/Frameworkfwd.h"
// #include "FWCore/Framework/interface/MakerMacros.h"
// #include "FWCore/ServiceRegistry/interface/Service.h"
// #include "FWCore/Common/interface/TriggerNames.h"
// #include "FWCore/Common/interface/TriggerResultsByName.h"
// #include "FWCore/Framework/interface/ConsumesCollector.h"
// #include "FWCore/Framework/interface/ESHandle.h"
// #include "FWCore/Framework/interface/EventSetup.h"

// #include "DataFormats/Common/interface/Handle.h"
// #include "DataFormats/Common/interface/View.h"
// #include "DataFormats/Common/interface/TriggerResults.h"
// #include "DataFormats/HLTReco/interface/TriggerEvent.h"
// #include "DataFormats/HLTReco/interface/TriggerObject.h"
// #include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
// #include "DataFormats/L1Trigger/interface/Muon.h"
// #include "DataFormats/Luminosity/interface/LumiDetails.h"
// #include "DataFormats/Math/interface/deltaR.h"
// #include "DataFormats/MuonReco/interface/Muon.h"
// #include "DataFormats/MuonReco/interface/MuonSelectors.h"
// #include "DataFormats/MuonReco/interface/MuonTrackLinks.h"
// #include "DataFormats/PatCandidates/interface/Muon.h"
// #include "DataFormats/RecoCandidate/interface/IsoDeposit.h"
// #include "DataFormats/RecoCandidate/interface/IsoDepositFwd.h"
// #include "DataFormats/RecoCandidate/interface/RecoChargedCandidate.h"
// #include "DataFormats/RecoCandidate/interface/RecoChargedCandidateFwd.h"
// #include "DataFormats/RecoCandidate/interface/RecoChargedCandidateIsolation.h"
// #include "DataFormats/TrackReco/interface/Track.h"
// #include "DataFormats/VertexReco/interface/Vertex.h"
// #include "DataFormats/VertexReco/interface/VertexFwd.h"
// #include "DataFormats/Scalers/interface/LumiScalers.h"

// #include "CommonTools/UtilAlgos/interface/TFileService.h"
// #include "HLTrigger/HLTcore/interface/HLTPrescaleProvider.h"
// #include "HLTrigger/HLTcore/interface/HLTEventAnalyzerAOD.h"
// #include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

// #include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
// #include "DataFormats/HepMCCandidate/interface/GenParticle.h"
// #include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"


#include <map>
#include <string>
#include <iomanip>
#include "TTree.h"
#include <Math/VectorUtil.h>

using namespace std;
using namespace reco;
using namespace edm;
using namespace pat;

MuonHLTNtupler::MuonHLTNtupler(const edm::ParameterSet& iConfig):
hltPreConfig_(iConfig, consumesCollector(), *this),
myHLTPreConfig_(iConfig, consumesCollector(), *this),
t_offlineMuon_       ( consumes< edm::View<reco::Muon> >                  (iConfig.getUntrackedParameter<edm::InputTag>("offlineMuon"       )) ),
t_offlinePATMuon_    ( consumes< std::vector<pat::Muon> >                 (iConfig.getUntrackedParameter<edm::InputTag>("offlineMuon"       )) ), // -- use same inputTag with t_offlineMuon
t_offlineVertex_     ( consumes< reco::VertexCollection >                 (iConfig.getUntrackedParameter<edm::InputTag>("offlineVertex"     )) ),
t_generalTrack_      ( consumes< reco::TrackCollection >                  (iConfig.getUntrackedParameter<edm::InputTag>("generalTrack"      )) ),
t_trackTime_           ( consumes< edm::ValueMap<float> >                 (iConfig.getUntrackedParameter<edm::InputTag>("trackTime"         )) ),
t_trackTimeError_      ( consumes< edm::ValueMap<float> >                 (iConfig.getUntrackedParameter<edm::InputTag>("trackTimeError"         )) ),
t_trackTimeQualityMVA_ ( consumes< edm::ValueMap<float> >                 (iConfig.getUntrackedParameter<edm::InputTag>("trackTimeQualityMVA"         )) ),
t_triggerResults_    ( consumes< edm::TriggerResults >                    (iConfig.getUntrackedParameter<edm::InputTag>("triggerResults"    )) ),
t_triggerEvent_      ( mayConsume< trigger::TriggerEvent >                (iConfig.getUntrackedParameter<edm::InputTag>("triggerEvent"      )) ), // -- not used in miniAOD case
t_myTriggerResults_  ( consumes< edm::TriggerResults >                    (iConfig.getUntrackedParameter<edm::InputTag>("myTriggerResults"  )) ),
t_myTriggerEvent_    ( mayConsume< trigger::TriggerEvent >                (iConfig.getUntrackedParameter<edm::InputTag>("myTriggerEvent"    )) ), // -- not used in miniAOD case
t_L3Muon_            ( consumes< reco::RecoChargedCandidateCollection >   (iConfig.getUntrackedParameter<edm::InputTag>("L3Muon"            )) ),
t_rho_ECAL_          ( consumes< double >                                 (iConfig.getUntrackedParameter<edm::InputTag>("rho_ECAL"          )) ),
t_rho_HCAL_          ( consumes< double >                                 (iConfig.getUntrackedParameter<edm::InputTag>("rho_HCAL"          )) ),
t_ECALIsoMap_        ( consumes< reco::RecoChargedCandidateIsolationMap > (iConfig.getUntrackedParameter<edm::InputTag>("ECALIsoMap"        )) ),
t_HCALIsoMap_        ( consumes< reco::RecoChargedCandidateIsolationMap > (iConfig.getUntrackedParameter<edm::InputTag>("HCALIsoMap"        )) ),
t_trkIsoMap_         ( consumes< reco::IsoDepositMap >                    (iConfig.getUntrackedParameter<edm::InputTag>("trkIsoMap"         )) ),
t_L2Muon_            ( consumes< reco::RecoChargedCandidateCollection >   (iConfig.getUntrackedParameter<edm::InputTag>("L2Muon"            )) ),
t_L1Muon_            ( consumes< l1t::MuonBxCollection  >                 (iConfig.getUntrackedParameter<edm::InputTag>("L1Muon"            )) ),
t_TkMuon_            ( consumes< reco::RecoChargedCandidateCollection >   (iConfig.getUntrackedParameter<edm::InputTag>("TkMuon"            )) ),

t_iterL3OI_          ( consumes< std::vector<reco::MuonTrackLinks> >      (iConfig.getUntrackedParameter<edm::InputTag>("iterL3OI"          )) ),
t_iterL3IOFromL2_    ( consumes< std::vector<reco::MuonTrackLinks> >      (iConfig.getUntrackedParameter<edm::InputTag>("iterL3IOFromL2"    )) ),
t_iterL3FromL2_      ( consumes< std::vector<reco::MuonTrackLinks> >      (iConfig.getUntrackedParameter<edm::InputTag>("iterL3FromL2"      )) ),
t_iterL3IOFromL1_    ( consumes< std::vector<reco::Track> >               (iConfig.getUntrackedParameter<edm::InputTag>("iterL3IOFromL1"    )) ),
t_iterL3MuonNoID_    ( consumes< std::vector<reco::Muon> >                (iConfig.getUntrackedParameter<edm::InputTag>("iterL3MuonNoID"    )) ),

t_lumiScaler_        ( consumes< LumiScalersCollection >                  (iConfig.getUntrackedParameter<edm::InputTag>("lumiScaler"        )) ),
t_offlineLumiScaler_ ( consumes< LumiScalersCollection >                  (iConfig.getUntrackedParameter<edm::InputTag>("offlineLumiScaler" )) ),
t_PUSummaryInfo_     ( consumes< std::vector<PileupSummaryInfo> >         (iConfig.getUntrackedParameter<edm::InputTag>("PUSummaryInfo"     )) ),
t_genEventInfo_      ( consumes< GenEventInfoProduct >                    (iConfig.getUntrackedParameter<edm::InputTag>("genEventInfo"      )) ),
t_genParticle_       ( consumes< reco::GenParticleCollection >            (iConfig.getUntrackedParameter<edm::InputTag>("genParticle"       )) ),

isMiniAOD_               ( iConfig.existsAs<bool>("isMiniAOD")         ? iConfig.getParameter<bool>("isMiniAOD")         : false),
rerunHLT_                ( iConfig.existsAs<bool>("rerunHLT")          ? iConfig.getParameter<bool>("rerunHLT")          : false),
doSaveRerunObject_       ( iConfig.existsAs<bool>("doSaveRerunObject") ? iConfig.getParameter<bool>("doSaveRerunObject") : false),
t_triggerObject_miniAOD_ ( mayConsume< std::vector<pat::TriggerObjectStandAlone> > (iConfig.getUntrackedParameter<edm::InputTag>("triggerObject_miniAOD")) ), // -- not used in AOD case
propSetup_(iConfig, consumesCollector()) {
  cout << "isMiniAOD_ = "         << isMiniAOD_         << endl;
  cout << "rerunHLT_ = "          << rerunHLT_          << endl;
  cout << "doSaveRerunObject_ = " << doSaveRerunObject_ << endl;
}

void MuonHLTNtupler::analyze(const edm::Event &iEvent, const edm::EventSetup &iSetup) {
  Init();

  propagatorToMuon_ = propSetup_.init(iSetup);

  // -- basic info.
  isRealData_ = iEvent.isRealData();

  runNum_       = iEvent.id().run();
  lumiBlockNum_ = iEvent.id().luminosityBlock();
  eventNum_     = iEvent.id().event();

  // -- vertex
  edm::Handle<reco::VertexCollection> h_offlineVertex;
  if( iEvent.getByToken(t_offlineVertex_, h_offlineVertex) ) {
    // -- count # vertex
    bool isPVFound = false;
    int nGoodVtx = 0;
    for(reco::VertexCollection::const_iterator it = h_offlineVertex->begin(); it != h_offlineVertex->end(); ++it) {
      if( it->isValid() ) {
        nGoodVtx++;

        if( !isPVFound ) { // -- the first "valid" vertex
          isPVFound = true;
          pv_time_      = it->t();
          pv_timeError_ = it->tError();
        }
      }
    }

    nVertex_ = nGoodVtx;
  }

  if( isRealData_ ) {
    bunchID_ = iEvent.bunchCrossing();

    // -- lumi scaler @ HLT
    edm::Handle<LumiScalersCollection> h_lumiScaler;
    if( iEvent.getByToken(t_lumiScaler_, h_lumiScaler) && h_lumiScaler->begin() != h_lumiScaler->end() )
    {
      instLumi_  = h_lumiScaler->begin()->instantLumi();
      dataPU_    = h_lumiScaler->begin()->pileup();
      dataPURMS_ = h_lumiScaler->begin()->pileupRMS();
      bunchLumi_ = h_lumiScaler->begin()->bunchLumi();
    }

    // -- lumi scaler @ offline
    edm::Handle<LumiScalersCollection> h_offlineLumiScaler;
    if( iEvent.getByToken(t_offlineLumiScaler_, h_offlineLumiScaler) && h_offlineLumiScaler->begin() != h_offlineLumiScaler->end() )
    {
      offlineInstLumi_  = h_offlineLumiScaler->begin()->instantLumi();
      offlineDataPU_    = h_offlineLumiScaler->begin()->pileup();
      offlineDataPURMS_ = h_offlineLumiScaler->begin()->pileupRMS();
      offlineBunchLumi_ = h_offlineLumiScaler->begin()->bunchLumi();
    }
  }

  // -- True PU info: only for MC -- //
  if( !isRealData_ ) {
    edm::Handle<std::vector< PileupSummaryInfo > > h_PUSummaryInfo;

    if( iEvent.getByToken(t_PUSummaryInfo_,h_PUSummaryInfo) )
    {
      std::vector<PileupSummaryInfo>::const_iterator PVI;
      for(PVI = h_PUSummaryInfo->begin(); PVI != h_PUSummaryInfo->end(); ++PVI)
      {
        if(PVI->getBunchCrossing()==0)
        {
          truePU_ = PVI->getTrueNumInteractions();
          continue;
        }
      } // -- end of PU iteration -- //
    } // -- end of if ( token exists )
  } // -- end of isMC -- //

  // -- rerun objects
  if( rerunHLT_ && doSaveRerunObject_ ) {
    // -- rho @ HLT
    edm::Handle<double> h_rho_ECAL;
    rho_ECAL_ = *(h_rho_ECAL.product());

    edm::Handle<double> h_rho_HCAL;
    rho_HCAL_ = *(h_rho_HCAL.product());
  }

  // -- fill each object
  Fill_Muon(iEvent);
  Fill_GeneralTrack(iEvent);
  Fill_HLT(iEvent, iSetup, 0); // -- original HLT objects saved in data taking
  if( doSaveRerunObject_ ) Fill_HLT(iEvent, iSetup, 1); // -- rerun objects
  Fill_HLTMuon(iEvent);
  Fill_L1Muon(iEvent);
  Fill_IterL3(iEvent);
  if( !isRealData_ ) Fill_GenParticle(iEvent);

  ntuple_->Fill();
}

void MuonHLTNtupler::beginJob()
{
  ntuple_ = nullptr;
  edm::Service<TFileService> fs;
  ntuple_ = fs->make<TTree>("ntuple", "ntuple");
  Make_Branch();
}

void MuonHLTNtupler::Init()
{
  isRealData_ = false;

  runNum_       = -999;
  lumiBlockNum_ = -999;
  eventNum_     = 0;

  bunchID_ = -999;

  nVertex_ = -999;
  pv_time_ = -999;
  pv_timeError_ = -999;

  instLumi_  = -999;
  dataPU_    = -999;
  dataPURMS_ = -999;
  bunchLumi_ = -999;

  rho_ECAL_ = -999;
  rho_HCAL_ = -999;

  offlineInstLumi_  = -999;
  offlineDataPU_    = -999;
  offlineDataPURMS_ = -999;
  offlineBunchLumi_ = -999;

  truePU_ = -999;

  genEventWeight_ = -999;

  nGenParticle_ = 0;
  for( int i=0; i<arrSize_; i++)
  {
    genParticle_ID_[i] = -999;
    genParticle_status_[i] = -999;
    genParticle_mother_[i] = -999;

    genParticle_pt_[i]     = -999;
    genParticle_eta_[i]    = -999;
    genParticle_phi_[i]    = -999;
    genParticle_px_[i]     = -999;
    genParticle_py_[i]     = -999;
    genParticle_pz_[i]     = -999;
    genParticle_energy_[i] = -999;
    genParticle_charge_[i] = -999;

    genParticle_isPrompt_[i] = 0;
    genParticle_isPromptFinalState_[i] = 0;
    genParticle_isTauDecayProduct_[i] = 0;
    genParticle_isPromptTauDecayProduct_[i] = 0;
    genParticle_isDirectPromptTauDecayProductFinalState_[i] = 0;
    genParticle_isHardProcess_[i] = 0;
    genParticle_isLastCopy_[i] = 0;
    genParticle_isLastCopyBeforeFSR_[i] = 0;
    genParticle_isPromptDecayed_[i] = 0;
    genParticle_isDecayedLeptonHadron_[i] = 0;
    genParticle_fromHardProcessBeforeFSR_[i] = 0;
    genParticle_fromHardProcessDecayed_[i] = 0;
    genParticle_fromHardProcessFinalState_[i] = 0;
    genParticle_isMostlyLikePythia6Status3_[i] = 0;
  }

  // -- original trigger objects- - //
  vec_firedTrigger_.clear();
  vec_prescale_.clear();
  vec_filterName_.clear();
  vec_HLTObj_pt_.clear();
  vec_HLTObj_eta_.clear();
  vec_HLTObj_phi_.clear();

  // -- HLT rerun objects -- //
  vec_myFiredTrigger_.clear();
  vec_myPrescale_.clear();
  vec_myFilterName_.clear();
  vec_myHLTObj_pt_.clear();
  vec_myHLTObj_eta_.clear();
  vec_myHLTObj_phi_.clear();


  nMuon_ = 0;
  for( int i=0; i<arrSize_; i++)
  {
    muon_pt_[i] = -999;
    muon_eta_[i] = -999;
    muon_phi_[i] = -999;
    muon_px_[i] = -999;
    muon_py_[i] = -999;
    muon_pz_[i] = -999;
    muon_dB_[i] = -999;
    muon_charge_[i] = -999;
    muon_isGLB_[i] = 0;
    muon_isSTA_[i] = 0;
    muon_isTRK_[i] = 0;
    muon_isPF_[i] = 0;
    muon_isTight_[i] = 0;
    muon_isMedium_[i] = 0;
    muon_isLoose_[i] = 0;
    muon_isHighPt_[i] = 0;
    muon_isHighPtNew_[i] = 0;
    muon_isLooseTrigger_[i] = 0;
    muon_isSoft_[i] = 0;

    muon_iso03_sumPt_[i] = -999;
    muon_iso03_hadEt_[i] = -999;
    muon_iso03_emEt_[i] = -999;

    muon_PFIso03_charged_[i] = -999;
    muon_PFIso03_neutral_[i] = -999;
    muon_PFIso03_photon_[i] = -999;
    muon_PFIso03_sumPU_[i] = -999;

    muon_PFIso04_charged_[i] = -999;
    muon_PFIso04_neutral_[i] = -999;
    muon_PFIso04_photon_[i] = -999;
    muon_PFIso04_sumPU_[i] = -999;

    muon_PFCluster03_ECAL_[i] = -999;
    muon_PFCluster03_HCAL_[i] = -999;

    muon_PFCluster04_ECAL_[i] = -999;
    muon_PFCluster04_HCAL_[i] = -999;

    muon_normChi2_global_[i] = -999;
    muon_nTrackerHit_global_[i] = -999;
    muon_nTrackerLayer_global_[i] = -999;
    muon_nPixelHit_global_[i] = -999;
    muon_nMuonHit_global_[i] = -999;

    muon_inner_pt_[i] = -999;
    muon_inner_eta_[i] = -999;
    muon_inner_phi_[i] = -999;
    muon_inner_time_[i] = -999;
    muon_inner_timeError_[i] = -999;
    muon_inner_timeQualMVA_[i] = -999;
    muon_normChi2_inner_[i] = -999;
    muon_nTrackerHit_inner_[i] = -999;
    muon_nTrackerLayer_inner_[i] = -999;
    muon_nPixelHit_inner_[i] = -999;

    muon_pt_tuneP_[i] = -999;
    muon_ptError_tuneP_[i] = -999;

    muon_dxyVTX_best_[i] = -999;
    muon_dzVTX_best_[i] = -999;

    muon_nMatchedStation_[i] = -999;
    muon_nMatchedRPCLayer_[i] = -999;
    muon_stationMask_[i] = -999;

    muon_propEta_[i] = -999;
    muon_propPhi_[i] = -999;

    muon_simType_[i] = -999;
    muon_simExtType_[i] = -999;
    muon_simPdgId_[i] = -999;
    muon_simFlavour_[i] = -999;
    muon_simPt_[i] = -999;
    muon_simEta_[i] = -999;
    muon_simPhi_[i] = -999;
    muon_simBX_[i] = -999;
    muon_simProdZ_[i] = -999;
    muon_simProdRho_[i] = -999;
    muon_simMotherPdgId_[i] = -999;
    muon_simHeaviestMotherFlavour_[i] = -999;
  }

  nGeneralTrack_ = 0;
  for( int i=0; i<arrSize_; i++) {
    generalTrack_pt_[i] = -999;
    generalTrack_eta_[i] = -999;
    generalTrack_phi_[i] = -999;
    generalTrack_dxy_[i] = -999;
    generalTrack_dz_[i] = -999;
    generalTrack_time_[i] = -999;
    generalTrack_timeError_[i] = -999;
    generalTrack_timeQualMVA_[i] = -999;
  }

  nL3Muon_ = 0;
  for( int i=0; i<arrSize_; i++)
  {
    L3Muon_pt_[i] = -999;
    L3Muon_eta_[i] = -999;
    L3Muon_phi_[i] = -999;
    L3Muon_charge_[i] = -999;
    L3Muon_trkPt_[i] = -999;
    L3Muon_ECALIso_[i] = -999;
    L3Muon_HCALIso_[i] = -999;
    L3Muon_trkIso_[i] = -999;
  }

  nL2Muon_ = 0;
  for( int i=0; i<arrSize_; i++)
  {
    L2Muon_pt_[i] = -999;
    L2Muon_eta_[i] = -999;
    L2Muon_phi_[i] = -999;
    L2Muon_charge_[i] = -999;
    L2Muon_trkPt_[i] = -999;
  }

  nTkMuon_ = 0;
  for( int i=0; i<arrSize_; i++)
  {
    TkMuon_pt_[i] = -999;
    TkMuon_eta_[i] = -999;
    TkMuon_phi_[i] = -999;
    TkMuon_charge_[i] = -999;
    TkMuon_trkPt_[i] = -999;
  }

  nL1Muon_ = 0;
  for( int i=0; i<arrSize_; i++)
  {
    L1Muon_pt_[i] = -999;
    L1Muon_eta_[i] = -999;
    L1Muon_phi_[i] = -999;
    L1Muon_charge_[i] = -999;
    L1Muon_quality_[i] = -999;
  }

  nIterL3OI_ = 0;
  for( int i=0; i<arrSize_; i++)
  {
    iterL3OI_inner_pt_[i] = -999;
    iterL3OI_inner_eta_[i] = -999;
    iterL3OI_inner_phi_[i] = -999;
    iterL3OI_inner_charge_[i] = -999;
    iterL3OI_outer_pt_[i] = -999;
    iterL3OI_outer_eta_[i] = -999;
    iterL3OI_outer_phi_[i] = -999;
    iterL3OI_outer_charge_[i] = -999;
    iterL3OI_global_pt_[i] = -999;
    iterL3OI_global_eta_[i] = -999;
    iterL3OI_global_phi_[i] = -999;
    iterL3OI_global_charge_[i] = -999;
  }

  nIterL3IOFromL2_ = 0;
  for( int i=0; i<arrSize_; i++)
  {
    iterL3IOFromL2_inner_pt_[i] = -999;
    iterL3IOFromL2_inner_eta_[i] = -999;
    iterL3IOFromL2_inner_phi_[i] = -999;
    iterL3IOFromL2_inner_charge_[i] = -999;
    iterL3IOFromL2_outer_pt_[i] = -999;
    iterL3IOFromL2_outer_eta_[i] = -999;
    iterL3IOFromL2_outer_phi_[i] = -999;
    iterL3IOFromL2_outer_charge_[i] = -999;
    iterL3IOFromL2_global_pt_[i] = -999;
    iterL3IOFromL2_global_eta_[i] = -999;
    iterL3IOFromL2_global_phi_[i] = -999;
    iterL3IOFromL2_global_charge_[i] = -999;
  }

  nIterL3FromL2_ = 0;
  for( int i=0; i<arrSize_; i++)
  {
    iterL3FromL2_inner_pt_[i] = -999;
    iterL3FromL2_inner_eta_[i] = -999;
    iterL3FromL2_inner_phi_[i] = -999;
    iterL3FromL2_inner_charge_[i] = -999;
    iterL3FromL2_outer_pt_[i] = -999;
    iterL3FromL2_outer_eta_[i] = -999;
    iterL3FromL2_outer_phi_[i] = -999;
    iterL3FromL2_outer_charge_[i] = -999;
    iterL3FromL2_global_pt_[i] = -999;
    iterL3FromL2_global_eta_[i] = -999;
    iterL3FromL2_global_phi_[i] = -999;
    iterL3FromL2_global_charge_[i] = -999;
  }

  nIterL3IOFromL1_ = 0;
  for( int i=0; i<arrSize_; i++)
  {
    iterL3IOFromL1_pt_[i] = -999;
    iterL3IOFromL1_eta_[i] = -999;
    iterL3IOFromL1_phi_[i] = -999;
    iterL3IOFromL1_charge_[i] = -999;
  }


  nIterL3MuonNoID_ = 0;
  for (int i=0; i<arrSize_; ++i)
  {
    iterL3MuonNoID_pt_[i] = -999;
    iterL3MuonNoID_eta_[i] = -999;
    iterL3MuonNoID_phi_[i] = -999;
    iterL3MuonNoID_charge_[i] = -999;

    iterL3MuonNoID_isGLB_[i] = 0;
    iterL3MuonNoID_isSTA_[i] = 0;
    iterL3MuonNoID_isTRK_[i] = 0;
  }
}

void MuonHLTNtupler::Make_Branch()
{
  ntuple_->Branch("isRealData", &isRealData_, "isRealData/O"); // -- O: boolean -- //
  ntuple_->Branch("runNum", &runNum_, "runNum/I");
  ntuple_->Branch("lumiBlockNum",&lumiBlockNum_,"lumiBlockNum/I");
  ntuple_->Branch("eventNum",&eventNum_,"eventNum/l"); // -- unsigned long long -- //
  ntuple_->Branch("nVertex", &nVertex_, "nVertex/I");
  ntuple_->Branch("pv_time",      &pv_time_,      "pv_time/D");
  ntuple_->Branch("pv_timeError", &pv_timeError_, "pv_timeError/D");
  ntuple_->Branch("bunchID", &bunchID_, "bunchID/D");
  ntuple_->Branch("instLumi", &instLumi_, "instLumi/D");
  ntuple_->Branch("dataPU", &dataPU_, "dataPU/D");
  ntuple_->Branch("dataPURMS", &dataPURMS_, "dataPURMS/D");
  ntuple_->Branch("bunchLumi", &bunchLumi_, "bunchLumi/D");
  ntuple_->Branch("offlineInstLumi", &offlineInstLumi_, "offlineInstLumi/D");
  ntuple_->Branch("offlineDataPU", &offlineDataPU_, "offlineDataPU/D");
  ntuple_->Branch("offlineDataPURMS", &offlineDataPURMS_, "offlineDataPURMS/D");
  ntuple_->Branch("offlineBunchLumi", &offlineBunchLumi_, "offlineBunchLumi/D");
  ntuple_->Branch("truePU", &truePU_, "truePU/I");

  ntuple_->Branch("rho_ECAL", &rho_ECAL_, "rho_ECAL/D");
  ntuple_->Branch("rho_HCAL", &rho_HCAL_, "rho_HCAL/D");

  ntuple_->Branch("genEventWeight", &genEventWeight_, "genEventWeight/D");
  ntuple_->Branch("nGenParticle", &nGenParticle_, "nGenParticle/I");
  ntuple_->Branch("genParticle_ID", &genParticle_ID_, "genParticle_ID[nGenParticle]/I");
  ntuple_->Branch("genParticle_status", &genParticle_status_, "genParticle_status[nGenParticle]/I");
  ntuple_->Branch("genParticle_mother", &genParticle_mother_, "genParticle_mother[nGenParticle]/I");
  ntuple_->Branch("genParticle_pt", &genParticle_pt_, "genParticle_pt[nGenParticle]/D");
  ntuple_->Branch("genParticle_eta", &genParticle_eta_, "genParticle_eta[nGenParticle]/D");
  ntuple_->Branch("genParticle_phi", &genParticle_phi_, "genParticle_phi[nGenParticle]/D");
  ntuple_->Branch("genParticle_px", &genParticle_px_, "genParticle_px[nGenParticle]/D");
  ntuple_->Branch("genParticle_py", &genParticle_py_, "genParticle_py[nGenParticle]/D");
  ntuple_->Branch("genParticle_pz", &genParticle_pz_, "genParticle_pz[nGenParticle]/D");
  ntuple_->Branch("genParticle_energy", &genParticle_energy_, "genParticle_energy[nGenParticle]/D");
  ntuple_->Branch("genParticle_charge", &genParticle_charge_, "genParticle_charge[nGenParticle]/D");
  ntuple_->Branch("genParticle_isPrompt", &genParticle_isPrompt_, "genParticle_isPrompt[nGenParticle]/I");
  ntuple_->Branch("genParticle_isPromptFinalState", &genParticle_isPromptFinalState_, "genParticle_isPromptFinalState[nGenParticle]/I");
  ntuple_->Branch("genParticle_isTauDecayProduct", &genParticle_isTauDecayProduct_, "genParticle_isTauDecayProduct[nGenParticle]/I");
  ntuple_->Branch("genParticle_isPromptTauDecayProduct", &genParticle_isPromptTauDecayProduct_, "genParticle_isPromptTauDecayProduct[nGenParticle]/I");
  ntuple_->Branch("genParticle_isDirectPromptTauDecayProductFinalState", &genParticle_isDirectPromptTauDecayProductFinalState_, "genParticle_isDirectPromptTauDecayProductFinalState[nGenParticle]/I");
  ntuple_->Branch("genParticle_isHardProcess", &genParticle_isHardProcess_, "genParticle_isHardProcess[nGenParticle]/I");
  ntuple_->Branch("genParticle_isLastCopy", &genParticle_isLastCopy_, "genParticle_isLastCopy[nGenParticle]/I");
  ntuple_->Branch("genParticle_isLastCopyBeforeFSR", &genParticle_isLastCopyBeforeFSR_, "genParticle_isLastCopyBeforeFSR[nGenParticle]/I");
  ntuple_->Branch("genParticle_isPromptDecayed", &genParticle_isPromptDecayed_, "genParticle_isPromptDecayed[nGenParticle]/I");
  ntuple_->Branch("genParticle_isDecayedLeptonHadron", &genParticle_isDecayedLeptonHadron_, "genParticle_isDecayedLeptonHadron[nGenParticle]/I");
  ntuple_->Branch("genParticle_fromHardProcessBeforeFSR", &genParticle_fromHardProcessBeforeFSR_, "genParticle_fromHardProcessBeforeFSR[nGenParticle]/I");
  ntuple_->Branch("genParticle_fromHardProcessDecayed", &genParticle_fromHardProcessDecayed_, "genParticle_fromHardProcessDecayed[nGenParticle]/I");
  ntuple_->Branch("genParticle_fromHardProcessFinalState", &genParticle_fromHardProcessFinalState_, "genParticle_fromHardProcessFinalState[nGenParticle]/I");
  ntuple_->Branch("genParticle_isMostlyLikePythia6Status3", &genParticle_isMostlyLikePythia6Status3_, "genParticle_isMostlyLikePythia6Status3[nGenParticle]/I");

  ntuple_->Branch("vec_firedTrigger", &vec_firedTrigger_);
  ntuple_->Branch("vec_prescale", &vec_prescale_);
  ntuple_->Branch("vec_filterName", &vec_filterName_);
  ntuple_->Branch("vec_HLTObj_pt", &vec_HLTObj_pt_);
  ntuple_->Branch("vec_HLTObj_eta", &vec_HLTObj_eta_);
  ntuple_->Branch("vec_HLTObj_phi", &vec_HLTObj_phi_);

  ntuple_->Branch("vec_myFiredTrigger", &vec_myFiredTrigger_);
  ntuple_->Branch("vec_myPrescale", &vec_myPrescale_);
  ntuple_->Branch("vec_myFilterName", &vec_myFilterName_);
  ntuple_->Branch("vec_myHLTObj_pt", &vec_myHLTObj_pt_);
  ntuple_->Branch("vec_myHLTObj_eta", &vec_myHLTObj_eta_);
  ntuple_->Branch("vec_myHLTObj_phi", &vec_myHLTObj_phi_);

  ntuple_->Branch("nMuon", &nMuon_, "nMuon/I");

  ntuple_->Branch("muon_pt", &muon_pt_, "muon_pt[nMuon]/D");
  ntuple_->Branch("muon_eta", &muon_eta_, "muon_eta[nMuon]/D");
  ntuple_->Branch("muon_phi", &muon_phi_, "muon_phi[nMuon]/D");
  ntuple_->Branch("muon_px", &muon_px_, "muon_px[nMuon]/D");
  ntuple_->Branch("muon_py", &muon_py_, "muon_py[nMuon]/D");
  ntuple_->Branch("muon_pz", &muon_pz_, "muon_pz[nMuon]/D");
  ntuple_->Branch("muon_dB", &muon_dB_, "muon_dB[nMuon]/D");
  ntuple_->Branch("muon_charge", &muon_charge_, "muon_charge[nMuon]/D");
  ntuple_->Branch("muon_isGLB", &muon_isGLB_, "muon_isGLB[nMuon]/I");
  ntuple_->Branch("muon_isSTA", &muon_isSTA_, "muon_isSTA[nMuon]/I");
  ntuple_->Branch("muon_isTRK", &muon_isTRK_, "muon_isTRK[nMuon]/I");
  ntuple_->Branch("muon_isPF", &muon_isPF_, "muon_isPF[nMuon]/I");
  ntuple_->Branch("muon_isTight", &muon_isTight_, "muon_isTight[nMuon]/I");
  ntuple_->Branch("muon_isMedium", &muon_isMedium_, "muon_isMedium[nMuon]/I");
  ntuple_->Branch("muon_isLoose", &muon_isLoose_, "muon_isLoose[nMuon]/I");
  ntuple_->Branch("muon_isHighPt", &muon_isHighPt_, "muon_isHighPt[nMuon]/I");
  ntuple_->Branch("muon_isHighPtNew", &muon_isHighPtNew_, "muon_isHighPtNew[nMuon]/I");
  ntuple_->Branch("muon_isLooseTrigger", &muon_isLooseTrigger_, "muon_isLooseTrigger[nMuon]/I");
  ntuple_->Branch("muon_isSoft", &muon_isSoft_, "muon_isSoft[nMuon]/I");

  ntuple_->Branch("muon_iso03_sumPt", &muon_iso03_sumPt_, "muon_iso03_sumPt[nMuon]/D");
  ntuple_->Branch("muon_iso03_hadEt", &muon_iso03_hadEt_, "muon_iso03_hadEt[nMuon]/D");
  ntuple_->Branch("muon_iso03_emEt", &muon_iso03_emEt_, "muon_iso03_emEt[nMuon]/D");
  ntuple_->Branch("muon_PFIso03_charged", &muon_PFIso03_charged_, "muon_PFIso03_charged[nMuon]/D");
  ntuple_->Branch("muon_PFIso03_neutral", &muon_PFIso03_neutral_, "muon_PFIso03_neutral[nMuon]/D");
  ntuple_->Branch("muon_PFIso03_photon", &muon_PFIso03_photon_, "muon_PFIso03_photon[nMuon]/D");
  ntuple_->Branch("muon_PFIso03_sumPU", &muon_PFIso03_sumPU_, "muon_PFIso03_sumPU[nMuon]/D");
  ntuple_->Branch("muon_PFIso04_charged", &muon_PFIso04_charged_, "muon_PFIso04_charged[nMuon]/D");
  ntuple_->Branch("muon_PFIso04_neutral", &muon_PFIso04_neutral_, "muon_PFIso04_neutral[nMuon]/D");
  ntuple_->Branch("muon_PFIso04_photon", &muon_PFIso04_photon_, "muon_PFIso04_photon[nMuon]/D");
  ntuple_->Branch("muon_PFIso04_sumPU", &muon_PFIso04_sumPU_, "muon_PFIso04_sumPU[nMuon]/D");

  ntuple_->Branch("muon_PFCluster03_ECAL", &muon_PFCluster03_ECAL_, "muon_PFCluster03_ECAL[nMuon]/D");
  ntuple_->Branch("muon_PFCluster03_HCAL", &muon_PFCluster03_HCAL_, "muon_PFCluster03_HCAL[nMuon]/D");
  ntuple_->Branch("muon_PFCluster04_ECAL", &muon_PFCluster04_ECAL_, "muon_PFCluster04_ECAL[nMuon]/D");
  ntuple_->Branch("muon_PFCluster04_HCAL", &muon_PFCluster04_HCAL_, "muon_PFCluster04_HCAL[nMuon]/D");
  ntuple_->Branch("muon_normChi2_global", &muon_normChi2_global_, "muon_normChi2_global[nMuon]/D");
  ntuple_->Branch("muon_nTrackerHit_global", &muon_nTrackerHit_global_, "muon_nTrackerHit_global[nMuon]/I");
  ntuple_->Branch("muon_nTrackerLayer_global", &muon_nTrackerLayer_global_, "muon_nTrackerLayer_global[nMuon]/I");
  ntuple_->Branch("muon_nPixelHit_global", &muon_nPixelHit_global_, "muon_nPixelHit_global[nMuon]/I");
  ntuple_->Branch("muon_nMuonHit_global", &muon_nMuonHit_global_, "muon_nMuonHit_global[nMuon]/I");
  ntuple_->Branch("muon_inner_pt",  &muon_inner_pt_, "muon_inner_pt[nMuon]/D");
  ntuple_->Branch("muon_inner_eta", &muon_inner_eta_, "muon_inner_eta[nMuon]/D");
  ntuple_->Branch("muon_inner_phi", &muon_inner_phi_, "muon_inner_phi[nMuon]/D");
  ntuple_->Branch("muon_inner_time",        &muon_inner_time_,        "muon_inner_time[nMuon]/D");
  ntuple_->Branch("muon_inner_timeError",   &muon_inner_timeError_,   "muon_inner_timeError[nMuon]/D");
  ntuple_->Branch("muon_inner_timeQualMVA", &muon_inner_timeQualMVA_, "muon_inner_timeQualMVA[nMuon]/D");
  ntuple_->Branch("muon_normChi2_inner", &muon_normChi2_inner_, "muon_normChi2_inner[nMuon]/D");
  ntuple_->Branch("muon_nTrackerHit_inner", &muon_nTrackerHit_inner_, "muon_nTrackerHit_inner[nMuon]/I");
  ntuple_->Branch("muon_nTrackerLayer_inner", &muon_nTrackerLayer_inner_, "muon_nTrackerLayer_inner[nMuon]/I");
  ntuple_->Branch("muon_nPixelHit_inner", &muon_nPixelHit_inner_, "muon_nPixelHit_inner[nMuon]/I");
  ntuple_->Branch("muon_pt_tuneP", &muon_pt_tuneP_, "muon_pt_tuneP[nMuon]/D");
  ntuple_->Branch("muon_ptError_tuneP", &muon_ptError_tuneP_, "muon_ptError_tuneP[nMuon]/D");
  ntuple_->Branch("muon_dxyVTX_best", &muon_dxyVTX_best_, "muon_dxyVTX_best[nMuon]/D");
  ntuple_->Branch("muon_dzVTX_best", &muon_dzVTX_best_, "muon_dzVTX_best[nMuon]/D");
  ntuple_->Branch("muon_nMatchedStation", &muon_nMatchedStation_, "muon_nMatchedStation[nMuon]/I");
  ntuple_->Branch("muon_nMatchedRPCLayer", &muon_nMatchedRPCLayer_, "muon_nMatchedRPCLayer[nMuon]/I");
  ntuple_->Branch("muon_stationMask", &muon_stationMask_, "muon_stationMask[nMuon]/I");

  ntuple_->Branch("muon_propEta", &muon_propEta_, "muon_propEta[nMuon]/D");
  ntuple_->Branch("muon_propPhi", &muon_propPhi_, "muon_propPhi[nMuon]/D");

  ntuple_->Branch("muon_simType",    &muon_simType_,    "muon_simType[nMuon]/I");
  ntuple_->Branch("muon_simExtType", &muon_simExtType_, "muon_simExtType[nMuon]/I");
  ntuple_->Branch("muon_simPdgId",   &muon_simPdgId_,   "muon_simPdgId[nMuon]/I");
  ntuple_->Branch("muon_simFlavour", &muon_simFlavour_, "muon_simFlavour[nMuon]/I");
  ntuple_->Branch("muon_simPt",  &muon_simPt_,  "muon_simPt[nMuon]/D");
  ntuple_->Branch("muon_simEta", &muon_simEta_, "muon_simEta[nMuon]/D");
  ntuple_->Branch("muon_simPhi", &muon_simPhi_, "muon_simPhi[nMuon]/D");
  ntuple_->Branch("muon_simBX",  &muon_simBX_,  "muon_simBX[nMuon]/I");
  ntuple_->Branch("muon_simProdZ",       &muon_simProdZ_,       "muon_simProdZ[nMuon]/D");
  ntuple_->Branch("muon_simProdRho",     &muon_simProdRho_,     "muon_simProdRho[nMuon]/D");  
  ntuple_->Branch("muon_simMotherPdgId", &muon_simMotherPdgId_, "muon_simMotherPdgId[nMuon]/I");
  ntuple_->Branch("muon_simHeaviestMotherFlavour", &muon_simHeaviestMotherFlavour_, "muon_simHeaviestMotherFlavour[nMuon]/I");


  ntuple_->Branch("nGeneralTrack",    &nGeneralTrack_, "nGeneralTrack/I");
  ntuple_->Branch("generalTrack_pt",  &generalTrack_pt_, "generalTrack_pt[nGeneralTrack]/D");
  ntuple_->Branch("generalTrack_eta", &generalTrack_eta_, "generalTrack_eta[nGeneralTrack]/D");
  ntuple_->Branch("generalTrack_phi", &generalTrack_phi_, "generalTrack_phi[nGeneralTrack]/D");
  ntuple_->Branch("generalTrack_dxy", &generalTrack_dxy_, "generalTrack_dxy[nGeneralTrack]/D");
  ntuple_->Branch("generalTrack_dz",  &generalTrack_dz_, "generalTrack_dz[nGeneralTrack]/D");
  ntuple_->Branch("generalTrack_time",  &generalTrack_time_, "generalTrack_time[nGeneralTrack]/D");
  ntuple_->Branch("generalTrack_timeError",  &generalTrack_timeError_, "generalTrack_timeError[nGeneralTrack]/D");
  ntuple_->Branch("generalTrack_timeQualMVA",  &generalTrack_timeQualMVA_, "generalTrack_timeQualMVA[nGeneralTrack]/D");

  ntuple_->Branch("nL3Muon", &nL3Muon_, "nL3Muon/I");
  ntuple_->Branch("L3Muon_pt", &L3Muon_pt_, "L3Muon_pt[nL3Muon]/D");
  ntuple_->Branch("L3Muon_eta", &L3Muon_eta_, "L3Muon_eta[nL3Muon]/D");
  ntuple_->Branch("L3Muon_phi", &L3Muon_phi_, "L3Muon_phi[nL3Muon]/D");
  ntuple_->Branch("L3Muon_charge", &L3Muon_charge_, "L3Muon_charge[nL3Muon]/D");
  ntuple_->Branch("L3Muon_trkPt", &L3Muon_trkPt_, "L3Muon_trkPt[nL3Muon]/D");
  ntuple_->Branch("L3Muon_ECALIso", &L3Muon_ECALIso_, "L3Muon_ECALIso[nL3Muon]/D");
  ntuple_->Branch("L3Muon_HCALIso", &L3Muon_HCALIso_, "L3Muon_HCALIso[nL3Muon]/D");
  ntuple_->Branch("L3Muon_trkIso",  &L3Muon_trkIso_,  "L3Muon_trkIso[nL3Muon]/D");

  ntuple_->Branch("nL2Muon", &nL2Muon_, "nL2Muon/I");
  ntuple_->Branch("L2Muon_pt", &L2Muon_pt_, "L2Muon_pt[nL2Muon]/D");
  ntuple_->Branch("L2Muon_eta", &L2Muon_eta_, "L2Muon_eta[nL2Muon]/D");
  ntuple_->Branch("L2Muon_phi", &L2Muon_phi_, "L2Muon_phi[nL2Muon]/D");
  ntuple_->Branch("L2Muon_charge", &L2Muon_charge_, "L2Muon_charge[nL2Muon]/D");
  ntuple_->Branch("L2Muon_trkPt", &L2Muon_trkPt_, "L2Muon_trkPt[nL2Muon]/D");

  ntuple_->Branch("nTkMuon", &nTkMuon_, "nTkMuon/I");
  ntuple_->Branch("TkMuon_pt", &TkMuon_pt_, "TkMuon_pt[nTkMuon]/D");
  ntuple_->Branch("TkMuon_eta", &TkMuon_eta_, "TkMuon_eta[nTkMuon]/D");
  ntuple_->Branch("TkMuon_phi", &TkMuon_phi_, "TkMuon_phi[nTkMuon]/D");
  ntuple_->Branch("TkMuon_charge", &TkMuon_charge_, "TkMuon_charge[nTkMuon]/D");
  ntuple_->Branch("TkMuon_trkPt", &TkMuon_trkPt_, "TkMuon_trkPt[nTkMuon]/D");

  ntuple_->Branch("nL1Muon", &nL1Muon_, "nL1Muon/I");
  ntuple_->Branch("L1Muon_pt", &L1Muon_pt_, "L1Muon_pt[nL1Muon]/D");
  ntuple_->Branch("L1Muon_eta", &L1Muon_eta_, "L1Muon_eta[nL1Muon]/D");
  ntuple_->Branch("L1Muon_phi", &L1Muon_phi_, "L1Muon_phi[nL1Muon]/D");
  ntuple_->Branch("L1Muon_charge", &L1Muon_charge_, "L1Muon_charge[nL1Muon]/D");
  ntuple_->Branch("L1Muon_quality", &L1Muon_quality_, "L1Muon_quality[nL1Muon]/D");

  ntuple_->Branch("nIterL3OI", &nIterL3OI_, "nIterL3OI/I");
  ntuple_->Branch("iterL3OI_inner_pt", &iterL3OI_inner_pt_, "iterL3OI_inner_pt[nIterL3OI]/D");
  ntuple_->Branch("iterL3OI_inner_eta", &iterL3OI_inner_eta_, "iterL3OI_inner_eta[nIterL3OI]/D");
  ntuple_->Branch("iterL3OI_inner_phi", &iterL3OI_inner_phi_, "iterL3OI_inner_phi[nIterL3OI]/D");
  ntuple_->Branch("iterL3OI_inner_charge", &iterL3OI_inner_charge_, "iterL3OI_inner_charge[nIterL3OI]/D");
  ntuple_->Branch("iterL3OI_outer_pt", &iterL3OI_outer_pt_, "iterL3OI_outer_pt[nIterL3OI]/D");
  ntuple_->Branch("iterL3OI_outer_eta", &iterL3OI_outer_eta_, "iterL3OI_outer_eta[nIterL3OI]/D");
  ntuple_->Branch("iterL3OI_outer_phi", &iterL3OI_outer_phi_, "iterL3OI_outer_phi[nIterL3OI]/D");
  ntuple_->Branch("iterL3OI_outer_charge", &iterL3OI_outer_charge_, "iterL3OI_outer_charge[nIterL3OI]/D");
  ntuple_->Branch("iterL3OI_global_pt", &iterL3OI_global_pt_, "iterL3OI_global_pt[nIterL3OI]/D");
  ntuple_->Branch("iterL3OI_global_eta", &iterL3OI_global_eta_, "iterL3OI_global_eta[nIterL3OI]/D");
  ntuple_->Branch("iterL3OI_global_phi", &iterL3OI_global_phi_, "iterL3OI_global_phi[nIterL3OI]/D");
  ntuple_->Branch("iterL3OI_global_charge", &iterL3OI_global_charge_, "iterL3OI_global_charge[nIterL3OI]/D");

  ntuple_->Branch("nIterL3IOFromL2", &nIterL3IOFromL2_, "nIterL3IOFromL2/I");
  ntuple_->Branch("iterL3IOFromL2_inner_pt", &iterL3IOFromL2_inner_pt_, "iterL3IOFromL2_inner_pt[nIterL3IOFromL2]/D");
  ntuple_->Branch("iterL3IOFromL2_inner_eta", &iterL3IOFromL2_inner_eta_, "iterL3IOFromL2_inner_eta[nIterL3IOFromL2]/D");
  ntuple_->Branch("iterL3IOFromL2_inner_phi", &iterL3IOFromL2_inner_phi_, "iterL3IOFromL2_inner_phi[nIterL3IOFromL2]/D");
  ntuple_->Branch("iterL3IOFromL2_inner_charge", &iterL3IOFromL2_inner_charge_, "iterL3IOFromL2_inner_charge[nIterL3IOFromL2]/D");
  ntuple_->Branch("iterL3IOFromL2_outer_pt", &iterL3IOFromL2_outer_pt_, "iterL3IOFromL2_outer_pt[nIterL3IOFromL2]/D");
  ntuple_->Branch("iterL3IOFromL2_outer_eta", &iterL3IOFromL2_outer_eta_, "iterL3IOFromL2_outer_eta[nIterL3IOFromL2]/D");
  ntuple_->Branch("iterL3IOFromL2_outer_phi", &iterL3IOFromL2_outer_phi_, "iterL3IOFromL2_outer_phi[nIterL3IOFromL2]/D");
  ntuple_->Branch("iterL3IOFromL2_outer_charge", &iterL3IOFromL2_outer_charge_, "iterL3IOFromL2_outer_charge[nIterL3IOFromL2]/D");
  ntuple_->Branch("iterL3IOFromL2_global_pt", &iterL3IOFromL2_global_pt_, "iterL3IOFromL2_global_pt[nIterL3IOFromL2]/D");
  ntuple_->Branch("iterL3IOFromL2_global_eta", &iterL3IOFromL2_global_eta_, "iterL3IOFromL2_global_eta[nIterL3IOFromL2]/D");
  ntuple_->Branch("iterL3IOFromL2_global_phi", &iterL3IOFromL2_global_phi_, "iterL3IOFromL2_global_phi[nIterL3IOFromL2]/D");
  ntuple_->Branch("iterL3IOFromL2_global_charge", &iterL3IOFromL2_global_charge_, "iterL3IOFromL2_global_charge[nIterL3IOFromL2]/D");

  ntuple_->Branch("nIterL3IOFromL1", &nIterL3IOFromL1_, "nIterL3IOFromL1/I");
  ntuple_->Branch("iterL3IOFromL1_pt", &iterL3IOFromL1_pt_, "iterL3IOFromL1_pt[nIterL3IOFromL1]/D");
  ntuple_->Branch("iterL3IOFromL1_eta", &iterL3IOFromL1_eta_, "iterL3IOFromL1_eta[nIterL3IOFromL1]/D");
  ntuple_->Branch("iterL3IOFromL1_phi", &iterL3IOFromL1_phi_, "iterL3IOFromL1_phi[nIterL3IOFromL1]/D");
  ntuple_->Branch("iterL3IOFromL1_charge", &iterL3IOFromL1_charge_, "iterL3IOFromL1_charge[nIterL3IOFromL1]/D");

  ntuple_->Branch("nIterL3FromL2", &nIterL3FromL2_, "nIterL3FromL2/I");
  ntuple_->Branch("iterL3FromL2_inner_pt", &iterL3FromL2_inner_pt_, "iterL3FromL2_inner_pt[nIterL3FromL2]/D");
  ntuple_->Branch("iterL3FromL2_inner_eta", &iterL3FromL2_inner_eta_, "iterL3FromL2_inner_eta[nIterL3FromL2]/D");
  ntuple_->Branch("iterL3FromL2_inner_phi", &iterL3FromL2_inner_phi_, "iterL3FromL2_inner_phi[nIterL3FromL2]/D");
  ntuple_->Branch("iterL3FromL2_inner_charge", &iterL3FromL2_inner_charge_, "iterL3FromL2_inner_charge[nIterL3FromL2]/D");
  ntuple_->Branch("iterL3FromL2_outer_pt", &iterL3FromL2_outer_pt_, "iterL3FromL2_outer_pt[nIterL3FromL2]/D");
  ntuple_->Branch("iterL3FromL2_outer_eta", &iterL3FromL2_outer_eta_, "iterL3FromL2_outer_eta[nIterL3FromL2]/D");
  ntuple_->Branch("iterL3FromL2_outer_phi", &iterL3FromL2_outer_phi_, "iterL3FromL2_outer_phi[nIterL3FromL2]/D");
  ntuple_->Branch("iterL3FromL2_outer_charge", &iterL3FromL2_outer_charge_, "iterL3FromL2_outer_charge[nIterL3FromL2]/D");
  ntuple_->Branch("iterL3FromL2_global_pt", &iterL3FromL2_global_pt_, "iterL3FromL2_global_pt[nIterL3FromL2]/D");
  ntuple_->Branch("iterL3FromL2_global_eta", &iterL3FromL2_global_eta_, "iterL3FromL2_global_eta[nIterL3FromL2]/D");
  ntuple_->Branch("iterL3FromL2_global_phi", &iterL3FromL2_global_phi_, "iterL3FromL2_global_phi[nIterL3FromL2]/D");
  ntuple_->Branch("iterL3FromL2_global_charge", &iterL3FromL2_global_charge_, "iterL3FromL2_global_charge[nIterL3FromL2]/D");

  ntuple_->Branch("nIterL3MuonNoID",       &nIterL3MuonNoID_,       "nIterL3MuonNoID/I");
  ntuple_->Branch("iterL3MuonNoID_pt",     &iterL3MuonNoID_pt_,     "iterL3MuonNoID_pt[nIterL3MuonNoID]/D");
  ntuple_->Branch("iterL3MuonNoID_eta",    &iterL3MuonNoID_eta_,    "iterL3MuonNoID_eta[nIterL3MuonNoID]/D");
  ntuple_->Branch("iterL3MuonNoID_phi",    &iterL3MuonNoID_phi_,    "iterL3MuonNoID_phi[nIterL3MuonNoID]/D");
  ntuple_->Branch("iterL3MuonNoID_charge", &iterL3MuonNoID_charge_, "iterL3MuonNoID_charge[nIterL3MuonNoID]/D");
  ntuple_->Branch("iterL3MuonNoID_isGLB",  &iterL3MuonNoID_isGLB_,  "iterL3MuonNoID_isGLB[nIterL3MuonNoID]/I");
  ntuple_->Branch("iterL3MuonNoID_isSTA",  &iterL3MuonNoID_isSTA_,  "iterL3MuonNoID_isSTA[nIterL3MuonNoID]/I");
  ntuple_->Branch("iterL3MuonNoID_isTRK",  &iterL3MuonNoID_isTRK_,  "iterL3MuonNoID_isTRK[nIterL3MuonNoID]/I");
}

void MuonHLTNtupler::Fill_Muon(const edm::Event &iEvent)
{
  // -- check whether the muon collection is pat::muon, which has more information than reco::muon
  edm::Handle<std::vector<pat::Muon>> h_offlinePATMuon;
  Bool_t isPATMuon = iEvent.getByToken(t_offlinePATMuon_, h_offlinePATMuon);

  // -- universal handle (reco::muon, pat::muon, etc)
  edm::Handle< edm::View<reco::Muon> > h_offlineMuon;
  if( iEvent.getByToken(t_offlineMuon_, h_offlineMuon) ) // -- only when the dataset has offline muon collection (e.g. AOD) -- //
  {
    edm::Handle<reco::VertexCollection> h_offlineVertex;
    iEvent.getByToken(t_offlineVertex_, h_offlineVertex);
    const reco::Vertex & pv = h_offlineVertex->at(0);

    int _nMuon = 0;
    for(auto mu=h_offlineMuon->begin(); mu!=h_offlineMuon->end(); ++mu)
    {
      muon_pt_[_nMuon]  = mu->pt();
      muon_eta_[_nMuon] = mu->eta();
      muon_phi_[_nMuon] = mu->phi();
      muon_px_[_nMuon]  = mu->px();
      muon_py_[_nMuon]  = mu->py();
      muon_pz_[_nMuon]  = mu->pz();
      muon_charge_[_nMuon] = mu->charge();

      if( mu->isGlobalMuon() )     muon_isGLB_[_nMuon] = 1;
      if( mu->isStandAloneMuon() ) muon_isSTA_[_nMuon] = 1;
      if( mu->isTrackerMuon() )    muon_isTRK_[_nMuon] = 1;
      if( mu->isPFMuon() )         muon_isPF_[_nMuon] = 1;

      // -- https://github.com/cms-sw/cmssw/blob/master/DataFormats/MuonReco/src/MuonSelectors.cc
      if( muon::isTightMuon( (*mu), pv ) )   muon_isTight_[_nMuon] = 1;
      if( muon::isMediumMuon( (*mu) ) )      muon_isMedium_[_nMuon] = 1;
      if( muon::isLooseMuon( (*mu) ) )       muon_isLoose_[_nMuon] = 1;
      if( muon::isHighPtMuon( (*mu), pv ) )  muon_isHighPt_[_nMuon] = 1;
      if( isNewHighPtMuon( (*mu), pv ) )     muon_isHighPtNew_[_nMuon] = 1;
      if( muon::isLooseTriggerMuon( (*mu)) ) muon_isLooseTrigger_[_nMuon] = 1;

      // -- bool muon::isSoftMuon(const reco::Muon& muon, const reco::Vertex& vtx, bool run2016_hip_mitigation)
      // -- it is different under CMSSW_8_0_29: bool muon::isSoftMuon(const reco::Muon& muon, const reco::Vertex& vtx)
      // -- Remove this part to avoid compile error (and soft muon would not be used for now) - need to be fixed at some point
      if( muon::isSoftMuon( (*mu), pv, 0) ) muon_isSoft_[_nMuon] = 1;

      muon_iso03_sumPt_[_nMuon] = mu->isolationR03().sumPt;
      muon_iso03_hadEt_[_nMuon] = mu->isolationR03().hadEt;
      muon_iso03_emEt_[_nMuon]  = mu->isolationR03().emEt;

      muon_PFIso03_charged_[_nMuon] = mu->pfIsolationR03().sumChargedHadronPt;
      muon_PFIso03_neutral_[_nMuon] = mu->pfIsolationR03().sumNeutralHadronEt;
      muon_PFIso03_photon_[_nMuon]  = mu->pfIsolationR03().sumPhotonEt;
      muon_PFIso03_sumPU_[_nMuon]   = mu->pfIsolationR03().sumPUPt;

      muon_PFIso04_charged_[_nMuon] = mu->pfIsolationR04().sumChargedHadronPt;
      muon_PFIso04_neutral_[_nMuon] = mu->pfIsolationR04().sumNeutralHadronEt;
      muon_PFIso04_photon_[_nMuon]  = mu->pfIsolationR04().sumPhotonEt;
      muon_PFIso04_sumPU_[_nMuon]   = mu->pfIsolationR04().sumPUPt;

      reco::TrackRef globalTrk = mu->globalTrack();
      if( globalTrk.isNonnull() )
      {
        muon_normChi2_global_[_nMuon] = globalTrk->normalizedChi2();

        const reco::HitPattern & globalTrkHit = globalTrk->hitPattern();
        muon_nTrackerHit_global_[_nMuon]   = globalTrkHit.numberOfValidTrackerHits();
        muon_nTrackerLayer_global_[_nMuon] = globalTrkHit.trackerLayersWithMeasurement();
        muon_nPixelHit_global_[_nMuon]     = globalTrkHit.numberOfValidPixelHits();
        muon_nMuonHit_global_[_nMuon]      = globalTrkHit.numberOfValidMuonHits();
      }

      reco::TrackRef innerTrk = mu->innerTrack();
      if( innerTrk.isNonnull() )
      {
        // -- to find muon track in general track collection by matching
        muon_inner_pt_[_nMuon]    = innerTrk->pt();
        muon_inner_eta_[_nMuon]   = innerTrk->eta();
        muon_inner_phi_[_nMuon]   = innerTrk->phi();
        Fill_Muon_TrackMTDTime(iEvent, innerTrk, _nMuon);

        muon_normChi2_inner_[_nMuon] = innerTrk->normalizedChi2();

        const reco::HitPattern & innerTrkHit = innerTrk->hitPattern();
        muon_nTrackerHit_inner_[_nMuon]      = innerTrkHit.numberOfValidTrackerHits();
        muon_nTrackerLayer_inner_[_nMuon]    = innerTrkHit.trackerLayersWithMeasurement();
        muon_nPixelHit_inner_[_nMuon]        = innerTrkHit.numberOfValidPixelHits();
      }

      reco::TrackRef tunePTrk = mu->tunePMuonBestTrack();
      if( tunePTrk.isNonnull() )
      {
        muon_pt_tuneP_[_nMuon]      = tunePTrk->pt();
        muon_ptError_tuneP_[_nMuon] = tunePTrk->ptError();
      }

      muon_dxyVTX_best_[_nMuon] = mu->muonBestTrack()->dxy( pv.position() );
      muon_dzVTX_best_[_nMuon]  = mu->muonBestTrack()->dz( pv.position() );

      muon_nMatchedStation_[_nMuon] = mu->numberOfMatchedStations();
      muon_nMatchedRPCLayer_[_nMuon] = mu->numberOfMatchedRPCLayers();
      muon_stationMask_[_nMuon] = mu->stationMask();

      // -- propagation to the 2nd station and get eta and phi value (for the matching with L1 muons)
      TrajectoryStateOnSurface prop = propagatorToMuon_.extrapolate( *(mu->muonBestTrack()) );
      if( prop.isValid() )
      {
        muon_propEta_[_nMuon] = prop.globalPosition().eta();
        muon_propPhi_[_nMuon] = prop.globalPosition().phi();
        // printf("[Propagation: suceeded (isGLB, isTRK, isSTA, isPF) = (%d, %d, %d, %d)]\n", muon_isGLB_[_nMuon], muon_isTRK_[_nMuon], muon_isSTA_[_nMuon], muon_isPF_[_nMuon]);
        // printf("  (eta, propagated eta) = (%lf, %lf)\n", muon_eta_[_nMuon], muon_propEta_[_nMuon]);
        // printf("  (phi, propagated phi) = (%lf, %lf)\n", muon_phi_[_nMuon], muon_propPhi_[_nMuon]);
      }
      else
      {
        // printf("[Propagation: failed (isGLB, isTRK, isSTA, isPF) = (%d, %d, %d, %d)]\n", muon_isGLB_[_nMuon], muon_isTRK_[_nMuon], muon_isSTA_[_nMuon], muon_isPF_[_nMuon]);
      }

      // -- additional info available only in pat::muon
      if( isPATMuon ) {
        auto PATMu = dynamic_cast<const pat::Muon*>(&*mu);
        muon_dB_[_nMuon] = PATMu->dB();

        // -- sim-hit matching information
        if( !isRealData_ ) {
          muon_simType_[_nMuon]    = PATMu->simType();
          muon_simExtType_[_nMuon] = PATMu->simExtType();

          muon_simPdgId_[_nMuon]   = PATMu->simPdgId();
          muon_simFlavour_[_nMuon] = PATMu->simFlavour();

          muon_simPt_[_nMuon]  = PATMu->simPt();
          muon_simEta_[_nMuon] = PATMu->simEta();
          muon_simPhi_[_nMuon] = PATMu->simPhi();

          muon_simBX_[_nMuon]      = PATMu->simBX();
          muon_simProdZ_[_nMuon]   = PATMu->simProdZ();
          muon_simProdRho_[_nMuon] = PATMu->simProdRho();

          muon_simMotherPdgId_[_nMuon]           = PATMu->simMotherPdgId();
          muon_simHeaviestMotherFlavour_[_nMuon] = PATMu->simHeaviestMotherFlavour();
        }

      } // -- end of isPATMuon

      _nMuon++;
    }

    nMuon_ = _nMuon;
  }
}

void MuonHLTNtupler::Fill_HLT(const edm::Event &iEvent, const edm::EventSetup &iSetup, bool isMYHLT)
{
  edm::Handle<edm::TriggerResults>  h_triggerResults;

  if( isMYHLT ) iEvent.getByToken(t_myTriggerResults_, h_triggerResults);
  else          iEvent.getByToken(t_triggerResults_,   h_triggerResults);

  edm::TriggerNames triggerNames = iEvent.triggerNames(*h_triggerResults);

  for(unsigned int itrig=0; itrig<triggerNames.size(); ++itrig)
  {
    LogDebug("triggers") << triggerNames.triggerName(itrig);

    if( h_triggerResults->accept(itrig) )
    {
      std::string pathName = triggerNames.triggerName(itrig);
      if( SavedTriggerCondition(pathName) )
      {
        if( isMYHLT )
        {
          vec_myFiredTrigger_.push_back( pathName );
          // myHLTPreConfig_.prescaleSet(iEvent, iSetup);
          vec_myPrescale_.push_back( myHLTPreConfig_.prescaleValue<double>(iEvent, iSetup, pathName) );
        }
        else
        {
          vec_firedTrigger_.push_back( pathName );
          // hltPreConfig_.prescaleSet(iEvent, iSetup);
          vec_prescale_.push_back( hltPreConfig_.prescaleValue<double>(iEvent, iSetup, pathName) );
        }
      }
    } // -- end of if fired -- //

  } // -- end of iteration over all trigger names -- //

  if( !isMiniAOD_ || isMYHLT ) // -- AOD case or rerun object (rerun object always saved as AOD format)
  {
    edm::Handle<trigger::TriggerEvent> h_triggerEvent;
    
    if( isMYHLT ) iEvent.getByToken(t_myTriggerEvent_, h_triggerEvent);
    else          iEvent.getByToken(t_triggerEvent_,   h_triggerEvent);

    const trigger::size_type nFilter(h_triggerEvent->sizeFilters());
    for( trigger::size_type i_filter=0; i_filter<nFilter; i_filter++)
    {
      std::string filterName = h_triggerEvent->filterTag(i_filter).encode();

      if( SavedFilterCondition(filterName) )
      {
        trigger::Keys objectKeys = h_triggerEvent->filterKeys(i_filter);
        const trigger::TriggerObjectCollection& triggerObjects(h_triggerEvent->getObjects());

        for( trigger::size_type i_key=0; i_key<objectKeys.size(); i_key++)
        {
          trigger::size_type objKey = objectKeys.at(i_key);
          const trigger::TriggerObject& triggerObj(triggerObjects[objKey]);

          if( isMYHLT )
          {
            vec_myFilterName_.push_back( filterName );
            vec_myHLTObj_pt_.push_back( triggerObj.pt() );
            vec_myHLTObj_eta_.push_back( triggerObj.eta() );
            vec_myHLTObj_phi_.push_back( triggerObj.phi() );
          }
          else
          {
            vec_filterName_.push_back( filterName );
            vec_HLTObj_pt_.push_back( triggerObj.pt() );
            vec_HLTObj_eta_.push_back( triggerObj.eta() );
            vec_HLTObj_phi_.push_back( triggerObj.phi() );
          }
        }
      } // -- end of if( muon filters )-- //
    } // -- end of filter iteration -- //

  }
  else // -- miniAOD case
  {
    edm::Handle< std::vector<pat::TriggerObjectStandAlone> > h_triggerObject;
    iEvent.getByToken(t_triggerObject_miniAOD_, h_triggerObject);

    const edm::TriggerNames names = iEvent.triggerNames(*h_triggerResults);
    for( pat::TriggerObjectStandAlone triggerObj : *h_triggerObject)
    {
      triggerObj.unpackNamesAndLabels(iEvent, *h_triggerResults); // -- does not work under 80X
      // triggerObj.unpackPathNames(names);

      for( size_t i_filter = 0; i_filter < triggerObj.filterLabels().size(); ++i_filter )
      {
        std::string filterName = triggerObj.filterLabels()[i_filter];
        if( SavedFilterCondition(filterName) )
        {
          // -- does not support MYHLT for miniAOD for now
          vec_filterName_.push_back( filterName );
          vec_HLTObj_pt_.push_back( triggerObj.pt() );
          vec_HLTObj_eta_.push_back( triggerObj.eta() );
          vec_HLTObj_phi_.push_back( triggerObj.phi() );
        }

      } // -- loop over filters

    } // -- loop over trigger objects

  } // -- end of miniAOD case

}

bool MuonHLTNtupler::SavedTriggerCondition( std::string& pathName )
{
  bool flag = false;

  // -- muon triggers
  if( pathName.find("HLT_IsoMu")    != std::string::npos ||
      pathName.find("HLT_Mu")       != std::string::npos ||
      pathName.find("HLT_OldMu")    != std::string::npos ||
      pathName.find("HLT_TkMu")     != std::string::npos ||
      pathName.find("HLT_IsoTkMu")  != std::string::npos ||
      pathName.find("HLT_DoubleMu") != std::string::npos ||
      pathName.find("HLT_Mu8_T")    != std::string::npos || 
      pathName.find("HLT_Physics_") != std::string::npos ) flag = true;

  return flag;
}

bool MuonHLTNtupler::SavedFilterCondition( std::string& filterName )
{
  bool flag = false;

  // -- muon filters
  if( (filterName.find("sMu") != std::string::npos || filterName.find("SingleMu") != std::string::npos) &&
       filterName.find("Tau")      == std::string::npos &&
       filterName.find("EG")       == std::string::npos &&
       filterName.find("MultiFit") == std::string::npos ) flag = true;

  return flag;
}

void MuonHLTNtupler::Fill_HLTMuon(const edm::Event &iEvent)
{
  ///////////////////
  // -- L3 Muon -- //
  ///////////////////
  edm::Handle<reco::RecoChargedCandidateCollection> h_L3Muon;
  if( iEvent.getByToken( t_L3Muon_, h_L3Muon ) )
  {
    edm::Handle<reco::RecoChargedCandidateIsolationMap> h_ECALIsoMap;
    edm::Handle<reco::RecoChargedCandidateIsolationMap> h_HCALIsoMap;
    edm::Handle<reco::IsoDepositMap> h_trkIsoMap;

    // -- vetos for calculating the tracker isolation: needed for tracker isolation calculation
    // -- typedef std::vector<Veto> Vetos;
    IsoDeposit::Vetos vec_trkVeto(h_L3Muon->size());
    if( iEvent.getByToken(t_trkIsoMap_, h_trkIsoMap) )
    {
      for(unsigned int i_L3=0; i_L3<h_L3Muon->size(); i_L3++)
      {
        reco::RecoChargedCandidateRef ref_L3Mu(h_L3Muon, i_L3);
        reco::IsoDeposit trkIsoDeposit = (*h_trkIsoMap)[ref_L3Mu];
        vec_trkVeto[i_L3] = trkIsoDeposit.veto();
      }
    }

    // -- fill L3 muons
    int _nL3Muon = 0;
    for(unsigned int i_L3=0; i_L3<h_L3Muon->size(); i_L3++)
    {
      reco::RecoChargedCandidateRef ref_L3Mu(h_L3Muon, _nL3Muon);

      L3Muon_pt_[_nL3Muon]     = ref_L3Mu->pt();
      L3Muon_eta_[_nL3Muon]    = ref_L3Mu->eta();
      L3Muon_phi_[_nL3Muon]    = ref_L3Mu->phi();
      L3Muon_charge_[_nL3Muon] = ref_L3Mu->charge();

      reco::TrackRef trackRef = ref_L3Mu->track();
      L3Muon_trkPt_[_nL3Muon] = trackRef->pt();

      if( iEvent.getByToken(t_ECALIsoMap_, h_ECALIsoMap) )
      {
        reco::RecoChargedCandidateIsolationMap::const_iterator iter_ECALIsoMap = (*h_ECALIsoMap).find( ref_L3Mu );
        L3Muon_ECALIso_[_nL3Muon] = iter_ECALIsoMap->val;
      }
      if( iEvent.getByToken(t_HCALIsoMap_, h_HCALIsoMap) )
      {
        reco::RecoChargedCandidateIsolationMap::const_iterator iter_HCALIsoMap = (*h_HCALIsoMap).find( ref_L3Mu );
        L3Muon_HCALIso_[_nL3Muon] = iter_HCALIsoMap->val;
      }
      if( iEvent.getByToken(t_trkIsoMap_, h_trkIsoMap) )
      {
        reco::IsoDeposit trkIsoDeposit = (*h_trkIsoMap)[ref_L3Mu];
        // L3Muon_trkIso_[_nL3Muon] = trkIsoDeposit.depositWithin(0.3);

        double conSize = 0.3;
        double theTrackPt_Min = -1.0;
        std::pair<double, int> trkIsoSumAndCount = trkIsoDeposit.depositAndCountWithin(conSize, vec_trkVeto, theTrackPt_Min);
        L3Muon_trkIso_[_nL3Muon] = trkIsoSumAndCount.first;
      }

      _nL3Muon++;

    }
    nL3Muon_ = _nL3Muon;
  } // -- if( L3 handle is valid ) -- //


  ///////////////////
  // -- L2 Muon -- //
  ///////////////////
  edm::Handle<reco::RecoChargedCandidateCollection> h_L2Muon;
  if( iEvent.getByToken( t_L2Muon_, h_L2Muon ) )
  {
    int _nL2Muon = 0;
    for( unsigned int i_L2=0; i_L2<h_L2Muon->size(); i_L2++)
    {
      reco::RecoChargedCandidateRef ref_L2Mu(h_L2Muon, _nL2Muon);

      L2Muon_pt_[_nL2Muon]     = ref_L2Mu->pt();
      L2Muon_eta_[_nL2Muon]    = ref_L2Mu->eta();
      L2Muon_phi_[_nL2Muon]    = ref_L2Mu->phi();
      L2Muon_charge_[_nL2Muon] = ref_L2Mu->charge();

      reco::TrackRef trackRef = ref_L2Mu->track();
      L2Muon_trkPt_[_nL2Muon] = trackRef->pt();

      _nL2Muon++;
    }
    nL2Muon_ = _nL2Muon;
  }

  ///////////////////
  // -- Tk Muon -- //
  ///////////////////
  edm::Handle<reco::RecoChargedCandidateCollection> h_TkMuon;
  if( iEvent.getByToken( t_TkMuon_, h_TkMuon ) )
  {
    int _nTkMuon = 0;
    for( unsigned int i_Tk=0; i_Tk<h_TkMuon->size(); i_Tk++)
    {
      reco::RecoChargedCandidateRef ref_TkMu(h_TkMuon, _nTkMuon);

      TkMuon_pt_[_nTkMuon]     = ref_TkMu->pt();
      TkMuon_eta_[_nTkMuon]    = ref_TkMu->eta();
      TkMuon_phi_[_nTkMuon]    = ref_TkMu->phi();
      TkMuon_charge_[_nTkMuon] = ref_TkMu->charge();

      reco::TrackRef trackRef = ref_TkMu->track();
      TkMuon_trkPt_[_nTkMuon] = trackRef->pt();

      _nTkMuon++;
    }
    nTkMuon_ = _nTkMuon;
  }
}

void MuonHLTNtupler::Fill_L1Muon(const edm::Event &iEvent)
{
  edm::Handle<l1t::MuonBxCollection> h_L1Muon;
  if( iEvent.getByToken(t_L1Muon_, h_L1Muon) )
  {
    int _nL1Muon = 0;
    for(int ibx = h_L1Muon->getFirstBX(); ibx<=h_L1Muon->getLastBX(); ++ibx)
    {
      if(ibx != 0) continue; // -- only take when ibx == 0 -- //
      for(auto it=h_L1Muon->begin(ibx); it!=h_L1Muon->end(ibx); it++)
      {
        l1t::MuonRef ref_L1Mu(h_L1Muon, distance(h_L1Muon->begin(h_L1Muon->getFirstBX()), it) );

        L1Muon_pt_[_nL1Muon]      = ref_L1Mu->pt();
        L1Muon_eta_[_nL1Muon]     = ref_L1Mu->eta();
        L1Muon_phi_[_nL1Muon]     = ref_L1Mu->phi();
        L1Muon_charge_[_nL1Muon]  = ref_L1Mu->charge();
        L1Muon_quality_[_nL1Muon] = ref_L1Mu->hwQual();

        _nL1Muon++;
      }
    }
    nL1Muon_ = _nL1Muon;
  }
}

void MuonHLTNtupler::Fill_GenParticle(const edm::Event &iEvent)
{
  // -- Gen-weight info -- //
  edm::Handle<GenEventInfoProduct> h_genEventInfo;
  iEvent.getByToken(t_genEventInfo_, h_genEventInfo);
  genEventWeight_ = h_genEventInfo->weight();

  // -- Gen-particle info -- //
  edm::Handle<reco::GenParticleCollection> h_genParticle;
  iEvent.getByToken(t_genParticle_, h_genParticle);

  int _nGenParticle = 0;
  for( size_t i=0; i< h_genParticle->size(); ++i)
  {
    const reco::GenParticle &parCand = (*h_genParticle)[i];

    if( abs(parCand.pdgId()) == 13 ) // -- only muons -- //
    {
      genParticle_ID_[_nGenParticle]     = parCand.pdgId();
      genParticle_status_[_nGenParticle] = parCand.status();
      genParticle_mother_[_nGenParticle] = parCand.mother(0)->pdgId();

      genParticle_pt_[_nGenParticle]  = parCand.pt();
      genParticle_eta_[_nGenParticle] = parCand.eta();
      genParticle_phi_[_nGenParticle] = parCand.phi();
      genParticle_px_[_nGenParticle]  = parCand.px();
      genParticle_py_[_nGenParticle]  = parCand.py();
      genParticle_pz_[_nGenParticle]  = parCand.pz();
      genParticle_energy_[_nGenParticle] = parCand.energy();
      genParticle_charge_[_nGenParticle] = parCand.charge();

      if( parCand.statusFlags().isPrompt() )                genParticle_isPrompt_[_nGenParticle] = 1;
      if( parCand.statusFlags().isTauDecayProduct() )       genParticle_isTauDecayProduct_[_nGenParticle] = 1;
      if( parCand.statusFlags().isPromptTauDecayProduct() ) genParticle_isPromptTauDecayProduct_[_nGenParticle] = 1;
      if( parCand.statusFlags().isDecayedLeptonHadron() )   genParticle_isDecayedLeptonHadron_[_nGenParticle] = 1;

      if( parCand.isPromptFinalState() ) genParticle_isPromptFinalState_[_nGenParticle] = 1;
      if( parCand.isDirectPromptTauDecayProductFinalState() ) genParticle_isDirectPromptTauDecayProductFinalState_[_nGenParticle] = 1;
      if( parCand.isHardProcess() ) genParticle_isHardProcess_[_nGenParticle] = 1;
      if( parCand.isLastCopy() ) genParticle_isLastCopy_[_nGenParticle] = 1;
      if( parCand.isLastCopyBeforeFSR() ) genParticle_isLastCopyBeforeFSR_[_nGenParticle] = 1;

      if( parCand.isPromptDecayed() )           genParticle_isPromptDecayed_[_nGenParticle] = 1;
      if( parCand.fromHardProcessBeforeFSR() )  genParticle_fromHardProcessBeforeFSR_[_nGenParticle] = 1;
      if( parCand.fromHardProcessDecayed() )    genParticle_fromHardProcessDecayed_[_nGenParticle] = 1;
      if( parCand.fromHardProcessFinalState() ) genParticle_fromHardProcessFinalState_[_nGenParticle] = 1;
      // if( parCand.isMostlyLikePythia6Status3() ) this->genParticle_isMostlyLikePythia6Status3[_nGenParticle] = 1;

      _nGenParticle++;
    }
  }
  nGenParticle_ = _nGenParticle;
}

void MuonHLTNtupler::Fill_IterL3(const edm::Event &iEvent)
{
  ////////////////////
  // -- IterL3OI -- //
  ////////////////////
  edm::Handle< std::vector<reco::MuonTrackLinks> > h_iterL3OI;
  if( iEvent.getByToken( t_iterL3OI_, h_iterL3OI ) )
  {
    int _nIterL3OI = 0;
    for( unsigned int i=0; i<h_iterL3OI->size(); i++)
    {
      if( h_iterL3OI->at(i).trackerTrack().isNonnull() )
      {
        iterL3OI_inner_pt_[_nIterL3OI]     = h_iterL3OI->at(i).trackerTrack()->pt();
        iterL3OI_inner_eta_[_nIterL3OI]    = h_iterL3OI->at(i).trackerTrack()->eta();
        iterL3OI_inner_phi_[_nIterL3OI]    = h_iterL3OI->at(i).trackerTrack()->phi();
        iterL3OI_inner_charge_[_nIterL3OI] = h_iterL3OI->at(i).trackerTrack()->charge();
      }
      if( h_iterL3OI->at(i).standAloneTrack().isNonnull() )
      {
        iterL3OI_outer_pt_[_nIterL3OI]     = h_iterL3OI->at(i).standAloneTrack()->pt();
        iterL3OI_outer_eta_[_nIterL3OI]    = h_iterL3OI->at(i).standAloneTrack()->eta();
        iterL3OI_outer_phi_[_nIterL3OI]    = h_iterL3OI->at(i).standAloneTrack()->phi();
        iterL3OI_outer_charge_[_nIterL3OI] = h_iterL3OI->at(i).standAloneTrack()->charge();
      }
      if( h_iterL3OI->at(i).globalTrack().isNonnull() )
      {
        iterL3OI_global_pt_[_nIterL3OI]     = h_iterL3OI->at(i).globalTrack()->pt();
        iterL3OI_global_eta_[_nIterL3OI]    = h_iterL3OI->at(i).globalTrack()->eta();
        iterL3OI_global_phi_[_nIterL3OI]    = h_iterL3OI->at(i).globalTrack()->phi();
        iterL3OI_global_charge_[_nIterL3OI] = h_iterL3OI->at(i).globalTrack()->charge();
      }
      _nIterL3OI++;
    }
    nIterL3OI_ = _nIterL3OI;
  }

  //////////////////////////
  // -- IterL3IOFromL2 -- //
  //////////////////////////
  edm::Handle< std::vector<reco::MuonTrackLinks> > h_iterL3IOFromL2;
  if( iEvent.getByToken( t_iterL3IOFromL2_, h_iterL3IOFromL2 ) )
  {
    int _nIterL3IOFromL2 = 0;
    for( unsigned int i=0; i<h_iterL3IOFromL2->size(); i++)
    {
      if( h_iterL3IOFromL2->at(i).trackerTrack().isNonnull() )
      {
        iterL3IOFromL2_inner_pt_[_nIterL3IOFromL2]     = h_iterL3IOFromL2->at(i).trackerTrack()->pt();
        iterL3IOFromL2_inner_eta_[_nIterL3IOFromL2]    = h_iterL3IOFromL2->at(i).trackerTrack()->eta();
        iterL3IOFromL2_inner_phi_[_nIterL3IOFromL2]    = h_iterL3IOFromL2->at(i).trackerTrack()->phi();
        iterL3IOFromL2_inner_charge_[_nIterL3IOFromL2] = h_iterL3IOFromL2->at(i).trackerTrack()->charge();
      }
      if( h_iterL3IOFromL2->at(i).standAloneTrack().isNonnull() )
      {
        iterL3IOFromL2_outer_pt_[_nIterL3IOFromL2]     = h_iterL3IOFromL2->at(i).standAloneTrack()->pt();
        iterL3IOFromL2_outer_eta_[_nIterL3IOFromL2]    = h_iterL3IOFromL2->at(i).standAloneTrack()->eta();
        iterL3IOFromL2_outer_phi_[_nIterL3IOFromL2]    = h_iterL3IOFromL2->at(i).standAloneTrack()->phi();
        iterL3IOFromL2_outer_charge_[_nIterL3IOFromL2] = h_iterL3IOFromL2->at(i).standAloneTrack()->charge();
      }
      if( h_iterL3IOFromL2->at(i).globalTrack().isNonnull() )
      {
        iterL3IOFromL2_global_pt_[_nIterL3IOFromL2]     = h_iterL3IOFromL2->at(i).globalTrack()->pt();
        iterL3IOFromL2_global_eta_[_nIterL3IOFromL2]    = h_iterL3IOFromL2->at(i).globalTrack()->eta();
        iterL3IOFromL2_global_phi_[_nIterL3IOFromL2]    = h_iterL3IOFromL2->at(i).globalTrack()->phi();
        iterL3IOFromL2_global_charge_[_nIterL3IOFromL2] = h_iterL3IOFromL2->at(i).globalTrack()->charge();
      }
      _nIterL3IOFromL2++;
    }
    nIterL3IOFromL2_ = _nIterL3IOFromL2;
  }

  ////////////////////////////////
  // -- IterL3FromL2 (OI+IO) -- //
  ////////////////////////////////
  edm::Handle< std::vector<reco::MuonTrackLinks> > h_iterL3FromL2;
  if( iEvent.getByToken( t_iterL3FromL2_, h_iterL3FromL2 ) )
  {
    int _nIterL3FromL2 = 0;
    for( unsigned int i=0; i<h_iterL3FromL2->size(); i++)
    {
      if( h_iterL3FromL2->at(i).trackerTrack().isNonnull() )
      {
        iterL3FromL2_inner_pt_[_nIterL3FromL2]     = h_iterL3FromL2->at(i).trackerTrack()->pt();
        iterL3FromL2_inner_eta_[_nIterL3FromL2]    = h_iterL3FromL2->at(i).trackerTrack()->eta();
        iterL3FromL2_inner_phi_[_nIterL3FromL2]    = h_iterL3FromL2->at(i).trackerTrack()->phi();
        iterL3FromL2_inner_charge_[_nIterL3FromL2] = h_iterL3FromL2->at(i).trackerTrack()->charge();
      }
      if( h_iterL3FromL2->at(i).standAloneTrack().isNonnull() )
      {
        iterL3FromL2_outer_pt_[_nIterL3FromL2]     = h_iterL3FromL2->at(i).standAloneTrack()->pt();
        iterL3FromL2_outer_eta_[_nIterL3FromL2]    = h_iterL3FromL2->at(i).standAloneTrack()->eta();
        iterL3FromL2_outer_phi_[_nIterL3FromL2]    = h_iterL3FromL2->at(i).standAloneTrack()->phi();
        iterL3FromL2_outer_charge_[_nIterL3FromL2] = h_iterL3FromL2->at(i).standAloneTrack()->charge();
      }
      if( h_iterL3FromL2->at(i).globalTrack().isNonnull() )
      {
        iterL3FromL2_global_pt_[_nIterL3FromL2]     = h_iterL3FromL2->at(i).globalTrack()->pt();
        iterL3FromL2_global_eta_[_nIterL3FromL2]    = h_iterL3FromL2->at(i).globalTrack()->eta();
        iterL3FromL2_global_phi_[_nIterL3FromL2]    = h_iterL3FromL2->at(i).globalTrack()->phi();
        iterL3FromL2_global_charge_[_nIterL3FromL2] = h_iterL3FromL2->at(i).globalTrack()->charge();
      }
      _nIterL3FromL2++;
    }
    nIterL3FromL2_ = _nIterL3FromL2;
  }

  //////////////////////////
  // -- IterL3IOFromL1 -- //
  //////////////////////////
  edm::Handle< std::vector<reco::Track> > h_iterL3IOFromL1;
  if( iEvent.getByToken( t_iterL3IOFromL1_, h_iterL3IOFromL1 ) )
  {
    int _nIterL3IOFromL1 = 0;
    for( unsigned int i=0; i<h_iterL3IOFromL1->size(); i++)
    {
      iterL3IOFromL1_pt_[_nIterL3IOFromL1]     = h_iterL3IOFromL1->at(i).pt();
      iterL3IOFromL1_eta_[_nIterL3IOFromL1]    = h_iterL3IOFromL1->at(i).eta();
      iterL3IOFromL1_phi_[_nIterL3IOFromL1]    = h_iterL3IOFromL1->at(i).phi();
      iterL3IOFromL1_charge_[_nIterL3IOFromL1] = h_iterL3IOFromL1->at(i).charge();
      _nIterL3IOFromL1++;
    }
    nIterL3IOFromL1_ = _nIterL3IOFromL1;
  }

  //////////////////////////
  // -- IterL3MuonNoID -- //
  //////////////////////////
  edm::Handle< std::vector<reco::Muon> > h_iterL3MuonNoID;
  if( iEvent.getByToken( t_iterL3MuonNoID_, h_iterL3MuonNoID) )
  {
    int _nIterL3MuonNoID = 0;
    for( auto i=0U; i<h_iterL3MuonNoID->size(); ++i )
    {
      const auto& muon(h_iterL3MuonNoID->at(i));

      iterL3MuonNoID_pt_[_nIterL3MuonNoID]     = muon.pt();
      iterL3MuonNoID_eta_[_nIterL3MuonNoID]    = muon.eta();
      iterL3MuonNoID_phi_[_nIterL3MuonNoID]    = muon.phi();
      iterL3MuonNoID_charge_[_nIterL3MuonNoID] = muon.charge();

      if( muon.isGlobalMuon() )     iterL3MuonNoID_isGLB_[_nIterL3MuonNoID] = 1;
      if( muon.isStandAloneMuon() ) iterL3MuonNoID_isSTA_[_nIterL3MuonNoID] = 1;
      if( muon.isTrackerMuon() )    iterL3MuonNoID_isTRK_[_nIterL3MuonNoID] = 1;

      _nIterL3MuonNoID++;
    } // -- end of muon iteration

    nIterL3MuonNoID_ = _nIterL3MuonNoID;
  } // -- if getByToken is valid
}

// -- reference: https://github.com/cms-sw/cmssw/blob/master/DataFormats/MuonReco/src/MuonSelectors.cc#L910-L938
// -- expectedNnumberOfMatchedStations() is not available under 80X: temporarily removed for the universality between 80X and 102X
bool MuonHLTNtupler::isNewHighPtMuon(const reco::Muon& muon, const reco::Vertex& vtx){
  if(!muon.isGlobalMuon()) return false;

  bool muValHits = ( muon.globalTrack()->hitPattern().numberOfValidMuonHits()>0 ||
                     muon.tunePMuonBestTrack()->hitPattern().numberOfValidMuonHits()>0 );

  bool muMatchedSt = muon.numberOfMatchedStations()>1;
  if(!muMatchedSt) {
    if( muon.isTrackerMuon() && muon.numberOfMatchedStations()==1 ) {
      if( muon.expectedNnumberOfMatchedStations()<2 ||
          !(muon.stationMask()==1 || muon.stationMask()==16) ||
          muon.numberOfMatchedRPCLayers()>2
        )
        muMatchedSt = true;
    }
  }

  bool muID = muValHits && muMatchedSt;

  bool hits = muon.innerTrack()->hitPattern().trackerLayersWithMeasurement() > 5 &&
    muon.innerTrack()->hitPattern().numberOfValidPixelHits() > 0; 

  bool momQuality = muon.tunePMuonBestTrack()->ptError()/muon.tunePMuonBestTrack()->pt() < 0.3;

  bool ip = fabs(muon.innerTrack()->dxy(vtx.position())) < 0.2 && fabs(muon.innerTrack()->dz(vtx.position())) < 0.5;

  return muID && hits && momQuality && ip;
  // return 0; // -- remove it when above lines are retrieved

}

void MuonHLTNtupler::Fill_GeneralTrack(const edm::Event &iEvent) {


  edm::Handle< edm::ValueMap<float> > h_trackTime;
  edm::Handle< edm::ValueMap<float> > h_trackTimeError;
  edm::Handle< edm::ValueMap<float> > h_trackTimeQualityMVA;

  bool timeInfo = iEvent.getByToken(t_trackTime_,           h_trackTime) &&
                  iEvent.getByToken(t_trackTimeError_,      h_trackTimeError) &&
                  iEvent.getByToken(t_trackTimeQualityMVA_, h_trackTimeQualityMVA);


  edm::Handle< reco::TrackCollection > h_generalTrack;

  int _nTrack = 0;
  if( iEvent.getByToken(t_generalTrack_, h_generalTrack) ) {

    // cout << "h_generalTrack->size() = " << h_generalTrack->size() << endl;
    for( size_t i=0; i<h_generalTrack->size(); ++i) {

      reco::TrackRef generalTrack = reco::TrackRef(h_generalTrack, i);

      generalTrack_pt_[_nTrack]  = generalTrack->pt();
      generalTrack_eta_[_nTrack] = generalTrack->eta();
      generalTrack_phi_[_nTrack] = generalTrack->phi();
      generalTrack_dxy_[_nTrack] = generalTrack->dxy(); // -- UPDATE: w.r.t. beamspot?
      generalTrack_dz_[_nTrack]  = generalTrack->dz();

      if( timeInfo ) {
        generalTrack_time_[_nTrack]        = (*h_trackTime)[generalTrack];
        generalTrack_timeError_[_nTrack]   = (*h_trackTimeError)[generalTrack];
        generalTrack_timeQualMVA_[_nTrack] = (*h_trackTimeQualityMVA)[generalTrack];

      } // -- end of if( timeInfo )

      _nTrack++;

      if( _nTrack >= arrSize_ ) break; // -- do not save more than maximum array size (seg. fault)
    }

  } // -- end of if( track available? )

  nGeneralTrack_ = _nTrack;

}


void MuonHLTNtupler::Fill_Muon_TrackMTDTime(const edm::Event& iEvent, const reco::TrackRef& theTrack, const int& _nMuon) {
  math::XYZVector theMomentum = theTrack->momentum();

  edm::Handle< reco::TrackCollection > h_generalTrack;
  iEvent.getByToken(t_generalTrack_, h_generalTrack);

  double dR_smallest = 1e10;
  int i_matchedTrack = -1;

  for( size_t i=0; i< h_generalTrack->size(); ++i) {
    const auto generalTrack = (*h_generalTrack)[i];
    double dR = ROOT::Math::VectorUtil::DeltaR(generalTrack.momentum(), theMomentum);
    if( dR < 0.3 && dR < dR_smallest ) {
      dR_smallest = dR;
      i_matchedTrack = i;
    }
  }

  if( i_matchedTrack >= 0 ) {

    auto matchedTrack = reco::TrackRef(h_generalTrack, i_matchedTrack);

    // cout << "muon - genreral track is matched" << endl;
    // cout << "[muon]          (pt, eta, phi) = (" << theTrack->pt()     << ", " << theTrack->eta()     << ", " << theTrack->phi()     << ")" << endl;
    // cout << "[general track] (pt, eta, phi) = (" << matchedTrack->pt() << ", " << matchedTrack->eta() << ", " << matchedTrack->phi() << ")" << endl;
    // cout << "---> dR = " << dR_smallest << endl;
    
    edm::Handle< edm::ValueMap<float> > h_trackTime;
    edm::Handle< edm::ValueMap<float> > h_trackTimeError;
    edm::Handle< edm::ValueMap<float> > h_trackTimeQualityMVA;

    iEvent.getByToken(t_trackTime_,           h_trackTime);
    iEvent.getByToken(t_trackTimeError_,      h_trackTimeError);
    iEvent.getByToken(t_trackTimeQualityMVA_, h_trackTimeQualityMVA);

    muon_inner_time_[_nMuon]        = (*h_trackTime)[matchedTrack];
    muon_inner_timeError_[_nMuon]   = (*h_trackTimeError)[matchedTrack];
    muon_inner_timeQualMVA_[_nMuon] = (*h_trackTimeQualityMVA)[matchedTrack];
  }
  else {
    // cout << "matched general track is not found!" << endl;
    // cout << "[muon] (pt, eta, phi) = (" << theTrack->pt() << ", " << theTrack->eta() << ", " << theTrack->phi() << ")" << endl;
    // cout << "[general tracks]" << endl;

    // for( size_t i=0; i< h_generalTrack->size(); ++i) {
    //   const auto generalTrack = (*h_generalTrack)[i];
    //   double dR = ROOT::Math::VectorUtil::DeltaR(generalTrack.momentum(), theMomentum);
    //   cout << "  " << i << "th: (pt, eta, phi) = (" << generalTrack.pt() << ", " << generalTrack.eta() << ", " << generalTrack.phi() << ") --> dR = " << dR << endl;
    // }
  }
}

void MuonHLTNtupler::beginRun(const edm::Run &iRun, const edm::EventSetup &iSetup) {
  bool changedConfig;
  if(!hltPreConfig_.init(iRun, iSetup, "HLT", changedConfig)) {
    cout << "Initialization of HLTPrescaleProvider (processName = HLT) failed!!" << endl;
    return;
  }

  if( doSaveRerunObject_ ) {
    bool myChangedConfig;
    if(!myHLTPreConfig_.init(iRun, iSetup, "MYHLT", myChangedConfig)) {
      cout << "Initialization of HLTPrescaleProvider (processName = MYHLT) failed: in case there was no rerunning HLT" << endl;
    }
  } // -- if( doSaveRerunObject_ )
}

void MuonHLTNtupler::endJob() {}
void MuonHLTNtupler::endRun(const edm::Run &iRun, const edm::EventSetup &iSetup) {}

DEFINE_FWK_MODULE(MuonHLTNtupler);