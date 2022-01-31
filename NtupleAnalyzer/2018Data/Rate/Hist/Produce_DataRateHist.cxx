#include <2018Data/Rate/DataRateHistProducer.h>

void Produce_DataRateHist(TString textFile_ntupleList) {
  DataRateHistProducer* producer = new DataRateHistProducer();
  producer->Set_ntupleListFile( textFile_ntupleList );

  producer->Set_TriggerList_HLT( {"IsoMu24_v", "Mu50_v"} );
  producer->Set_TriggerList_MYHLT( {"IsoMu24_v", "Mu50_v"} );

  // -- when you run over just 1 HLT physics dataset
  // Double_t prescale_HLTPhysics = 1100;
  // Double_t nHLTPhysicsDataset = 8;
  // producer->Set_Weight( prescale_HLTPhysics*nHLTPhysicsDataset );

  // -- when you run over full HLTPhysics dataset (total 8 datasets)
  Double_t prescale_HLTPhysics = 1100;
  producer->Set_Weight( prescale_HLTPhysics );

  producer->Produce();
}