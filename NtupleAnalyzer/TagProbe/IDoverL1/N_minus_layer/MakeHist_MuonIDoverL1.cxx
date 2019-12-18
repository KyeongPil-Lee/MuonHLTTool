#include <TagProbe/IDoverL1/MakeHist_MuonIDoverL1.h>

void MakeHist_MuonIDoverL1()
{
  HistProducer* histProducer = new HistProducer();
  histProducer->Set_IDType("N_minus_layer");
  histProducer->Set_OutputFileName("ROOTFile_TnPHist.root");
  histProducer->AddDataPath(MuonIDEffTool::ntuplePath);
  histProducer->Set_minPt( 24 ); // -- min pT applied for eta, phi and vtx

  histProducer->Produce();
}
