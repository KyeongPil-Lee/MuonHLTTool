#include <RateAndPurity/TriggerHistProducer.h>

// -- etaMin, etaMax is not useful in the code (just in the name of output file); but it is added to be coherent with (hard-coded) CondorJobGenerator setting
void MuonTriggerHist( TString trigger = "", TString dataset = "", TString version = "", Double_t etaMin = 0.0, Double_t etaMax = 5.0, TString jobID = "", vector< TString > vec_dataPath = {} )
{
  // -- test setup
  if( trigger == "" &&
      dataset == "" &&
      version == "" &&
      jobID == "" &&
      vec_dataPath.size() == 0)
  {
    trigger = "hltL1fL1sMu22L1Filtered0";
    dataset = "HLTPhysicsRun2018D";
    version = "TEST";
    jobID = "Job0";
    vec_dataPath = {"/gv0/Users/kplee/MUO19001/HLTPhysicsRun2018D/ntuple_3705.root"};
  }


  // TString fileName = TString::Format("Output-MuonTriggerPurity-%s-%s-%s-%.1f-%.1f.root", dataset.Data(), version.Data(), trigger.Data(), etaMin, etaMax );
  // if(jobID != "")
  //   fileName = TString::Format("Output-MuonTriggerPurity-%s-%s-%s-%.1f-%.1f--%s.root", dataset.Data(), version.Data(), trigger.Data(), etaMin, etaMax, jobID.Data() );

  TString fileName = TString::Format("Output-MuonTriggerHist-%s-%s-%s.root", dataset.Data(), version.Data(), trigger.Data() );
  if(jobID != "")
    fileName = TString::Format("Output-MuonTriggerHist-%s-%s-%s--%s.root", dataset.Data(), version.Data(), trigger.Data(), jobID.Data() );
  TFile *f_output = TFile::Open(fileName, "RECREATE");

  TriggerHistProducer* producer = new TriggerHistProducer();
  producer->Set_HLTPhysicsDataset(); // -- take into account HLTPhysics prescales
  producer->Set_Trigger(trigger);
  producer->Set_DataPath( vec_dataPath );
  producer->Set_OutputFile( f_output );
  producer->Set_UsePtMatching();
  producer->Produce();
}