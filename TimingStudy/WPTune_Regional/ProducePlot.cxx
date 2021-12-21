#include <Include/SimplePlotTools.h>

void ProducePlot()
{
  TString fileName_default = "./result_default/DQM_V0001_R000323775__HLT__FastTimerService__All.root";
  TString fileName_newWP   = "./result_newWP/DQM_V0001_R000323775__HLT__FastTimerService__All.root";

  TString histName = "DQMData/Run 323775/HLT/Run summary/TimerService/process MYHLT paths/path HLT_IsoMu24_v13/path time_real";

  TH1D* h_default = PlotTool::Get_Hist(fileName_default, histName);
  TH1D* h_newWP   = PlotTool::Get_Hist(fileName_newWP, histName);

  Double_t mean_default = h_default->GetMean();
  Double_t mean_newWP   = h_newWP->GetMean();

  TString canvasName = "comp_IsoMu24_real";

  PlotTool::HistCanvas* canvas = new PlotTool::HistCanvas(canvasName, 0, 1);
  canvas->SetTitle("processing time [ms]", "# events");
  canvas->Register(h_default, "old WP", kBlack);
  canvas->Register(h_newWP,   "new WP", kBlue);

  canvas->SetLegendPosition(0.50, 0.73, 0.94, 0.85);

  canvas->Latex_CMSInternal();
  canvas->RegisterLatex(0.16, 0.91, "#font[42]{#scale[0.5]{IsoMu24 processing time on Run2018D EphemeralHLTPhysics (run 323775)}}");
  TString meanInfo = TString::Format("Mean: (old WP, new WP) = (%.2lfs, %.2lfs)", mean_default, mean_newWP);
  canvas->RegisterLatex(0.16, 0.87, "#font[42]{#scale[0.5]{"+meanInfo+"}}");

  canvas->Draw();
}