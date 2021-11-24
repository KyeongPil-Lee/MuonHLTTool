#include <TagProbe/TnPTool.h>

TGraphAsymmErrors* GetEffGraph( TString fileName, TString var, TString dirName = "" )
{
  MuonHLT::TnPEffTool *tool = new MuonHLT::TnPEffTool(fileName);
  
  TGraphAsymmErrors* gEff = tool->CalcTnPEff_CutAndCount( var, dirName );
  return gEff;
}

// -- var: Pt, Eta, Phi, Vtx, TruePU
void DrawEfficiencyPlot(TString var)
{
  TString fileName_base = "ROOTFile_MakeHist_ISOoverL3_ZMuMu_M50to120_Default.root";
  TString fileName_ECAL    = "./ECAL/"+fileName_base;
  TString fileName_HCAL    = "./HCAL/"+fileName_base;
  TString fileName_tracker = "./Tracker/"+fileName_base;

  TGraphAsymmErrors* g_ECAL     = GetEffGraph(fileName_ECAL, var);
  TGraphAsymmErrors* g_HCAL     = GetEffGraph(fileName_HCAL, var);
  TGraphAsymmErrors* g_tracker  = GetEffGraph(fileName_tracker, var);

  TString canvasName = "c_IsoEff_"+var;
  PlotTool::GraphCanvas *canvas = new PlotTool::GraphCanvas(canvasName, 0, 0);

  canvas->Register(g_ECAL, "ECAL Iso.", kGreen+2);
  canvas->Register(g_HCAL, "ECAL+HCAL Iso.", kBlue);
  canvas->Register(g_tracker, "ECAL+HCAL+Tracker Iso.", kBlack);

  TString titleX = "";
  if( var == "Pt" )  titleX = "P_{T}(#mu) [GeV]";
  if( var == "Eta" ) titleX = "#eta(#mu)";
  if( var == "Phi" ) titleX = "#phi(#mu)";
  if( var == "Vtx" ) titleX = "# vtx";
  if( var == "TruePU" ) titleX = "True PU";
  
  canvas->SetTitle( titleX, "Efficiency");
  canvas->SetLegendPosition( 0.40, 0.16, 0.95, 0.30 );

  if( var == "Pt" ) canvas->SetRangeX(24, 120);
  canvas->SetRangeY( 0.85, 1.02 );

  canvas->Latex_CMSInternal();
  canvas->RegisterLatex( 0.16, 0.91, "#font[42]{#scale[0.6]{HLT Iso. efficiency over Mu24 objects}}");
  if( var != "Pt" )
    canvas->RegisterLatex( 0.16, 0.87, "#font[42]{#scale[0.6]{p_{T} > 26 GeV}}");

  canvas->Draw();

  // -- zoom-out version
  if( var == "Pt" )
  {
    canvas->SetRangeX(0, 500);
    canvas->SetRangeY( 0, 1.1 );
    canvas->canvasName_ = canvasName + "_zoomOut";
    canvas->Draw();
  }
}

// -- effType = ECAL, HCAL, Tracker
void DrawEfficiencyPlot_CompareDifferentPURange(TString effType, TString var)
{
  TString fileName_base = "ROOTFile_MakeHist_ISOoverL3_ZMuMu_M50to120_Default.root";

  TString fileName = "./"+effType+"/"+fileName_base;

  TGraphAsymmErrors* g_belowPU50 = GetEffGraph(fileName, var, "belowPU50");
  TGraphAsymmErrors* g_abovePU50 = GetEffGraph(fileName, var, "abovePU50");

  TString canvasName = "c_IsoEff_CompPU_"+effType+"_"+var;
  PlotTool::GraphCanvas *canvas = new PlotTool::GraphCanvas(canvasName, 0, 0);

  canvas->Register(g_belowPU50, "True PU < 50", kBlack);
  canvas->Register(g_abovePU50, "True PU #geq 50", kBlue);

  TString titleX = "";
  if( var == "Pt" )  titleX = "P_{T}(#mu) [GeV]";
  if( var == "Eta" ) titleX = "#eta(#mu)";
  if( var == "Phi" ) titleX = "#phi(#mu)";
  if( var == "Vtx" ) titleX = "# vtx";
  if( var == "TruePU" ) titleX = "True PU";
  
  canvas->SetTitle( titleX, "Efficiency");
  canvas->SetLegendPosition( 0.40, 0.16, 0.95, 0.30 );

  if( var == "Pt" ) canvas->SetRangeX(24, 120);
  canvas->SetRangeY( 0.85, 1.02 );

  canvas->Latex_CMSInternal();
  if( effType == "ECAL" )    canvas->RegisterLatex( 0.16, 0.91, "#font[42]{#scale[0.6]{ECAL / Mu24 efficiency}}");
  if( effType == "HCAL" )    canvas->RegisterLatex( 0.16, 0.91, "#font[42]{#scale[0.6]{ECAL+HCAL / ECAL efficiency}}");
  if( effType == "Tracker" ) canvas->RegisterLatex( 0.16, 0.91, "#font[42]{#scale[0.6]{ECAL+HCAL+Tracker / ECAL+HCAL efficiency}}");

  if( var != "Pt" )
    canvas->RegisterLatex( 0.16, 0.87, "#font[42]{#scale[0.6]{p_{T} > 26 GeV}}");

  canvas->Draw();

  // -- zoom-out version
  if( var == "Pt" )
  {
    canvas->SetRangeX(0, 500);
    canvas->SetRangeY( 0, 1.1 );
    canvas->canvasName_ = canvasName + "_zoomOut";
    canvas->Draw();
  }
}

void DrawPlot()
{
  DrawEfficiencyPlot("Pt");
  DrawEfficiencyPlot("Eta");
  DrawEfficiencyPlot("Phi");
  DrawEfficiencyPlot("Vtx");
  DrawEfficiencyPlot("TruePU");

  DrawEfficiencyPlot_CompareDifferentPURange("ECAL", "Pt");
  DrawEfficiencyPlot_CompareDifferentPURange("ECAL", "Eta");
  DrawEfficiencyPlot_CompareDifferentPURange("ECAL", "Phi");
  DrawEfficiencyPlot_CompareDifferentPURange("ECAL", "Vtx");
  DrawEfficiencyPlot_CompareDifferentPURange("ECAL", "TruePU");

  DrawEfficiencyPlot_CompareDifferentPURange("HCAL", "Pt");
  DrawEfficiencyPlot_CompareDifferentPURange("HCAL", "Eta");
  DrawEfficiencyPlot_CompareDifferentPURange("HCAL", "Phi");
  DrawEfficiencyPlot_CompareDifferentPURange("HCAL", "Vtx");
  DrawEfficiencyPlot_CompareDifferentPURange("HCAL", "TruePU");

  DrawEfficiencyPlot_CompareDifferentPURange("Tracker", "Pt");
  DrawEfficiencyPlot_CompareDifferentPURange("Tracker", "Eta");
  DrawEfficiencyPlot_CompareDifferentPURange("Tracker", "Phi");
  DrawEfficiencyPlot_CompareDifferentPURange("Tracker", "Vtx");
  DrawEfficiencyPlot_CompareDifferentPURange("Tracker", "TruePU");
}