#include <Run3Winter21/Efficiency/TnPHistProducer_HCALoverL3.h>

void ProduceTnPHist_HCALoverL3(TString textFile_ntupleList)
{
  TString tag = textFile_ntupleList.Data();
  tag = gSystem->BaseName(tag);
  tag.ReplaceAll(".txt", "");
  TString outputFileName = TString::Format("ROOTFile_TnPHist_%s.root", tag.Data());

  HistProducer* histProducer = new HistProducer();
  histProducer->Set_OutputFileName(outputFileName);
  histProducer->Set_ntupleListFile(textFile_ntupleList);
  histProducer->Set_minPt( 26 ); // -- min pT applied for eta, phi and vtx

  histProducer->Produce();
}