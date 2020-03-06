#include "MakeHist_TruePU.h"

void ttbarSemiLeptonic_MUOVal()
{
  vector<TString> vec_dataPath = {
    "/pnfs/knu.ac.kr/data/cms/store/user/jskim/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/crab_MuonHLTNtuple_SimMatchingInfo_2018_TTToSemiLeptonic__PUAvg50ForMUOVal/200227_162718/0000/*.root"
  };
  
  HistProducer *producer = new HistProducer("ttbarSemiLeptonic_102X_MUOVal");
  producer->SetVector_DataPath(vec_dataPath);
  producer->Produce();
}