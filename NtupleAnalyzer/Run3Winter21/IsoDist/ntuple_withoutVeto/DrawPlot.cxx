#include <Include/SimplePlotTools.h>

// -- var = pt, eta, phi
void DrawComparisonPlot(TString var)
{
  TString fileName = "ROOTFile_MakeHist_Validation_CustomIsoFilter.root";

  TString histName_IsoMu24Obj   = "h_IsoMu24Obj_"+var;
  TString histName_Mu24Obj      = "h_Mu24Obj_"+var;
  TString histName_Mu24Obj_IsoF = "h_Mu24Obj_IsoF_"+var;

  TH1D* h_IsoMu24Obj   = PlotTool::Get_Hist(fileName, histName_IsoMu24Obj);
  TH1D* h_Mu24Obj      = PlotTool::Get_Hist(fileName, histName_Mu24Obj);
  TH1D* h_Mu24Obj_IsoF = PlotTool::Get_Hist(fileName, histName_Mu24Obj_IsoF);

  TString canvasName = "c_IsoMu24_vs_Mu24_"+var;
  PlotTool::HistCanvaswRatio* canvas = new PlotTool::HistCanvaswRatio(canvasName, 0, 1);

  TString titleX = "";
  if( var == "pt" )  titleX = "p_{T}(#mu) [GeV]";
  if( var == "eta" ) titleX = "#eta(#mu)";
  if( var == "phi" ) titleX = "#phi(#mu)";
  canvas->SetTitle(titleX, "# objects", "Mu24/IsoMu24");

  canvas->Register( h_IsoMu24Obj,   "IsoMu24 objects w/ ignored iso. filter", kBlack);
  canvas->Register( h_Mu24Obj,      "Mu24 objects", kBlue);
  canvas->Register( h_Mu24Obj_IsoF, "Mu24 objects + Isolation filter", kRed);

  canvas->Latex_CMSInternal();
  canvas->SetLegendPosition(0.50, 0.82, 0.95, 0.95);

  if( var == "pt" ) canvas->SetRangeX(0, 200);
  canvas->SetRangeRatio(0.8, 1.2);

  canvas->Draw();
}

// -- objType = IsoMu24Obj, Mu24Obj, Mu24Obj_IsoF
// -- region: EB or EE
void DrawIsolation(TString objType, TString region, Bool_t isRelative)
{
  TString fileName = "ROOTFile_MakeHist_Validation_CustomIsoFilter.root";

  TString histName_ECALIso = "";
  TString histName_HCALIso = "";
  TString histName_trkIso = "";

  if( isRelative )
  {
    histName_ECALIso = "h_"+objType+"_relECALIso_"+region;
    histName_HCALIso = "h_"+objType+"_relHCALIso_"+region;
    histName_trkIso  = "h_"+objType+"_relTrkIso_"+region;
  }
  else
  {
    histName_ECALIso = "h_"+objType+"_ECALIso_"+region;
    histName_HCALIso = "h_"+objType+"_HCALIso_"+region;
    histName_trkIso  = "h_"+objType+"_trkIso_"+region;
  }

  TH1D* h_ECALIso = PlotTool::Get_Hist( fileName, histName_ECALIso );
  TH1D* h_HCALIso = PlotTool::Get_Hist( fileName, histName_HCALIso );
  TH1D* h_trkIso  = PlotTool::Get_Hist( fileName, histName_trkIso );

  TString canvasName = "c_HLTIso_"+objType+"_"+region;
  if( isRelative ) canvasName = "c_relHLTIso_"+objType+"_"+region;

  PlotTool::HistCanvas* canvas = new PlotTool::HistCanvas(canvasName, 0, 1);

  TString titleX = "Isolation";
  if( isRelative ) titleX = "Rel. isolation";
  canvas->SetTitle(titleX, "# muons");

  canvas->Register( h_ECALIso, "ECAL",    kGreen+2);
  canvas->Register( h_HCALIso, "HCAL",    kBlue);
  canvas->Register( h_trkIso,  "Tracker", kBlack);

  canvas->SetLegendPosition(0.75, 0.82, 0.95, 0.95);

  if( isRelative ) canvas->SetRangeX(0, 0.3);
  else             canvas->SetRangeX(0, 70);
  canvas->SetRangeY(1e-1, 2e6);

  TString regionInfo = "";
  if( region == "EE" ) regionInfo = "Endcap";
  if( region == "EB" ) regionInfo = "Barrel";
  canvas->RegisterLatex(0.16, 0.91, "#font[42]{#scale[0.6]{"+regionInfo+"}}");
  canvas->Latex_CMSInternal();


  canvas->Draw();
}

void DrawPlot()
{
  DrawComparisonPlot("pt");
  DrawComparisonPlot("eta");
  DrawComparisonPlot("phi");

  DrawIsolation("IsoMu24Obj", "EB", 1);
  DrawIsolation("IsoMu24Obj", "EE", 1);

  DrawIsolation("Mu24Obj_IsoF", "EB", 1);
  DrawIsolation("Mu24Obj_IsoF", "EE", 1);
}
