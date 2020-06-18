#include <Include/NtupleHandle.h>
#include <Include/Object.h>

using namespace MuonHLT;

vector<TString> ExampleROOTFile(TString type);
void PrintTriggerList(TString type);

void PrintTriggerList()
{
  PrintTriggerList("2016");
  PrintTriggerList("2018");
}

void PrintTriggerList(TString type)
{
  cout << "=========================" << endl;
  cout << "Type = " << type << endl;

  vector<TString> vec_rootFiles = ExampleROOTFile(type);

  TChain *chain = new TChain("ntupler/ntuple");
  for(const auto rootFile : vec_rootFiles )
    chain->Add( rootFile );

  Int_t nEvent = chain->GetEntries();
  cout << "[Total event: " << nEvent << "]" << endl;

  NtupleHandle *ntuple = new NtupleHandle( chain );
  ntuple->TurnOnBranches_Event();
  // ntuple->TurnOnBranches_Muon();
  // ntuple->TurnOnBranches_simMuon();
  ntuple->TurnOnBranches_Trigger();
  // ntuple->TurnOnBranches_GenParticle();

  for(Int_t i_ev=0; i_ev<nEvent; i_ev++)
  {
    ntuple->GetEvent( i_ev );
    printf("[%dth event]\n", i_ev);

    Int_t nFiredTrigger = (Int_t)ntuple->vec_firedTrigger->size();
    for(Int_t i_trig = 0; i_trig < nFiredTrigger; i_trig++)
    {
      TString  triggerPath = ntuple->vec_firedTrigger->at(i_trig);
      Double_t prescale   = ntuple->vec_prescale->at(i_trig);
      printf("  [%dth trigger] name = %s, prescale = %.0lf\n", i_trig, triggerPath.Data(), prescale);
    }

    cout << endl;
  } // -- event iteration

  cout << endl;
  cout << endl;
}

vector<TString> ExampleROOTFile(TString type)
{
  vector<TString> vec_return = {};
  if( type == "2016" )
  {
    TString basePath = "/u/user/kplee/SE_UserHome/HLTPhysics/crab_MuonHLTNtuple_v20200617_HLTPhysics_Run2016Hv2_Rerun/200617_090302/0000";

    vector<TString> vec_rootFiles = {
      basePath+"/ntuple_923.root",
      basePath+"/ntuple_924.root",
      basePath+"/ntuple_929.root",
      basePath+"/ntuple_935.root",
      basePath+"/ntuple_954.root"
    };

    vec_return = vec_rootFiles;
  }
  else if( type == "2018" )
  {
    TString basePath = "/u/user/kplee/SE_UserHome/HLTPhysics/crab_MuonHLTNtuple_v20200615_HLTPhysics_Run2018D_Rerun/200615_135223/0000";
    vector<TString> vec_rootFiles = {
      basePath+"/ntuple_171.root",
      basePath+"/ntuple_172.root",
      basePath+"/ntuple_173.root",
      basePath+"/ntuple_174.root",
      basePath+"/ntuple_175.root"
    };

    vec_return = vec_rootFiles;
  }

  return vec_return;
}