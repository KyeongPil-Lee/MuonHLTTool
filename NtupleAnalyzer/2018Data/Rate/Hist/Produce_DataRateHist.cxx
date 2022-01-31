#include <2018Data/Rate/DataRateHistProducer.h>

void Produce_DataRateHist(TString textFile_ntupleList) {
  DataRateHistProducer* producer = new DataRateHistProducer();
  producer->Set_ntupleListFile( textFile_ntupleList );

  producer->Set_TriggerList_HLT( {"IsoMu24_v", "Mu50_v"} );
  producer->Set_TriggerList_MYHLT( {"IsoMu24_v", "Mu50_v"} );

  Double_t prescale_HLTPhysics = 1100;
  Double_t nHLTPhysicsDataset = 8;
  producer->Set_Weight( prescale_HLTPhysics*nHLTPhysicsDataset );

  producer->Produce();
}