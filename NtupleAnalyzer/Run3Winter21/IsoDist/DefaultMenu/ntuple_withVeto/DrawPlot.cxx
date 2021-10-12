#include <Include/SimplePlotTools.h>

// -- region: EB or EE
void DrawIsolation(TString region, Bool_t isRelative)
{
  TString fileName = "ROOTFile_MakeHist_Validation_CustomIsoFilter_ZMuMu_M50to120_Default.root";

  TString histName_ECALIso = "";
  TString histName_HCALIso = "";
  TString histName_trkIso = "";
  TString histName_trkIso_trkPt = "";

  if( isRelative )
  {
    histName_ECALIso = "h_relECALIso_"+region;
    histName_HCALIso = "h_relHCALIso_"+region;
    histName_trkIso  = "h_relTrkIso_"+region;
    histName_trkIso_trkPt = "h_relTrkIso_"+region;
  }
  else
  {
    histName_ECALIso = "h_ECALIso_"+region;
    histName_HCALIso = "h_HCALIso_"+region;
    histName_trkIso  = "h_trkIso_"+region;
  }

  TH1D* h_ECALIso = PlotTool::Get_Hist( fileName, histName_ECALIso );
  TH1D* h_HCALIso = PlotTool::Get_Hist( fileName, histName_HCALIso );
  TH1D* h_trkIso  = PlotTool::Get_Hist( fileName, histName_trkIso );
  TH1D* h_trkIso_trkPt = nullptr;
  if( isRelative ) h_trkIso_trkPt = PlotTool::Get_Hist( fileName, histName_trkIso_trkPt );

  TString canvasName = "c_HLTIso_"+region;
  if( isRelative ) canvasName = "c_relHLTIso_"+region;

  PlotTool::HistCanvas* canvas = new PlotTool::HistCanvas(canvasName, 0, 1);

  TString titleX = "Isolation";
  if( isRelative ) titleX = "Rel. isolation";
  canvas->SetTitle(titleX, "# muons");

  canvas->Register( h_ECALIso, "ECAL",    kGreen+2);
  canvas->Register( h_HCALIso, "HCAL",    kBlue);
  canvas->Register( h_trkIso,  "Tracker", kBlack);
  // if( isRelative )
  //   canvas->Register( h_trkIso_trkPt,  "Tracker (divided by track pt)", kViolet);

  canvas->SetLegendPosition(0.60, 0.75, 0.95, 0.95);

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

void DrawPlot_Comparison_Pt_TrackPt()
{
  TString fileName = "ROOTFile_MakeHist_Isolation_IsoMu24.root";

  TH1D* h_pt    = PlotTool::Get_Hist(fileName, "h_pt");
  TH1D* h_trkPt = PlotTool::Get_Hist(fileName, "h_trkPt");

  TString canvasName = "c_pt_vs_trkPt";

  PlotTool::HistCanvaswRatio* canvas = new PlotTool::HistCanvaswRatio(canvasName, 0, 1);
  canvas->SetTitle("p_{T}(#mu) [GeV]", "# muons", "trkpT/pT");

  canvas->Register(h_pt, "Default p_{T}", kBlack);
  canvas->Register(h_trkPt, "Track p_{T}", kViolet);

  canvas->SetLegendPosition(0.75, 0.82, 0.95, 0.95);

  canvas->SetRangeX(0, 1000);
  canvas->SetRangeY(1e-1, 2e6);

  canvas->SetRangeRatio(0.97, 1.03);

  canvas->Latex_CMSInternal();

  canvas->Draw();
}

// -- var = pt, eta, phi
void DrawPlot_stMu(TString var)
{
  TString fileName = "ROOTFile_MakeHist_Isolation_IsoMu24.root";

  TString histName = "";
  if( )

  TH1D* h_stMu = PlotTool::Get_Hist( fileName, histName );


  TH1D* h_pt    = PlotTool::Get_Hist(fileName, "h_pt");
  TH1D* h_trkPt = PlotTool::Get_Hist(fileName, "h_trkPt");

  TString canvasName = "c_pt_vs_trkPt";

  PlotTool::HistCanvaswRatio* canvas = new PlotTool::HistCanvaswRatio(canvasName, 0, 1);
  canvas->SetTitle("p_{T}(#mu) [GeV]", "# muons", "trkpT/pT");

  canvas->Register(h_pt, "Default p_{T}", kBlack);
  canvas->Register(h_trkPt, "Track p_{T}", kViolet);

  canvas->SetLegendPosition(0.75, 0.82, 0.95, 0.95);

  canvas->SetRangeX(0, 1000);
  canvas->SetRangeY(1e-1, 2e6);

  canvas->SetRangeRatio(0.97, 1.03);

  canvas->Latex_CMSInternal();

  canvas->Draw();
}

void DrawPlot()
{
  DrawIsolation("EB", 0);
  DrawIsolation("EE", 0);

  DrawIsolation("EB", 1);
  DrawIsolation("EE", 1);

  DrawPlot_Comparison_Pt_TrackPt();
}