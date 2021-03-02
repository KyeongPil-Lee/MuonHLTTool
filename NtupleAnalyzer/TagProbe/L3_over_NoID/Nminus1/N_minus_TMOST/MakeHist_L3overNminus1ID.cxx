#include <TagProbe/L3_over_NoID/Nminus1/MakeHist_L3overNminus1ID.h>

void MakeHist_L3overNminus1ID(TString textFile_ntupleList)
{
  TString tag = textFile_ntupleList.Data();
  tag.ReplaceAll(".txt", "");
  TString outputFileName = TString::Format("ROOTFile_TnPHist_%s.root", tag.Data());

  HistProducer* histProducer = new HistProducer();
  histProducer->Set_IDType("N_minus_TMOST");
  histProducer->Set_OutputFileName(outputFileName);
  histProducer->Set_ntupleListFile(textFile_ntupleList);
  histProducer->Set_minPt( 26 ); // -- min pT applied for eta, phi and vtx

  histProducer->Produce();
}