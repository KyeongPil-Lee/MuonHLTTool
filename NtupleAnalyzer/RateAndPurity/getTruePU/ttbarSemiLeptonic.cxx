#include "MakeHist_TruePU.h"

void ttbarSemiLeptonic()
{
  vector<TString> vec_dataPath = {
    "/pnfs/knu.ac.kr/data/cms/store/user/moh/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/crab_MuonHLTNtuple_SimMatchingInfo_2018_TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/191124_074614/0000/ntuple_*.root"
  };

  HistProducer *producer = new HistProducer("ttbarSemiLeptonic_102X");
  producer->SetVector_DataPath(vec_dataPath);
  producer->Produce();

}