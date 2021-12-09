#include <Run3Winter21/HistProducer/HistProducer.h>

void ProduceHist(TString textFile_ntupleList)
{
  TString tag = textFile_ntupleList.Data();
  tag = gSystem->BaseName(tag);
  tag.ReplaceAll(".txt", "");
  TString outputFileName = TString::Format("ROOTFile_%s.root", tag.Data());

  HistProducer* histProducer = new HistProducer();
  histProducer->Set_OutputFileName(outputFileName);
  histProducer->Set_ntupleListFile(textFile_ntupleList);

  histProducer->SetRange_PU_LowPU( 0, 40 );
  histProducer->SetRange_PU_HighPU( 40, 60 );

  histProducer->Produce();
}
