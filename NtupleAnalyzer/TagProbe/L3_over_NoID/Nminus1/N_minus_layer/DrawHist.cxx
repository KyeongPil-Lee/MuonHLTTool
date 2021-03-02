#include <TagProbe/TnPTool.h>

void DrawHistForEachVariable(TString var, Bool_t setZoomIn = kFALSE);
TGraphAsymmErrors* GetEffGraph( TString fileName, TString var );

void DrawHist() {
  DrawHistForEachVariable( "Pt" );
  DrawHistForEachVariable( "Pt", kTRUE );
  DrawHistForEachVariable( "Eta" );
  DrawHistForEachVariable( "Phi" );
  DrawHistForEachVariable( "Vtx" );
}

void DrawHistForEachVariable(TString var, Bool_t setZoomIn = kFALSE)
{
  TString fileName = "ROOTFile_TnPHist.root";

  TGraphAsymmErrors* g = GetEffGraph(fileName, var);

  // -- canvas with ratio
  TString canvasName = "c_vs"+var;
  if( setZoomIn ) canvasName = canvasName + "_zoomIn";
  PlotTool::GraphCanvas *canvas = new PlotTool::GraphCanvas(canvasName, 0, 0);
  canvas->Register(g, "MC (Z/#gamma*#rightarrow#mu#mu)", kBlue);

  TString titleX = "";
  if( var == "Pt" )  titleX = "P_{T}(#mu) [GeV]";
  if( var == "Eta" ) titleX = "#eta(#mu)";
  if( var == "Phi" ) titleX = "#phi(#mu)";
  if( var == "Vtx" ) titleX = "# vtx";

  canvas->SetTitle( titleX, "Efficiency");
  canvas->SetLegendPosition( 0.60, 0.16, 0.95, 0.30 );

  if( var == "Pt" && setZoomIn ) canvas->SetRangeX(24, 120);

  canvas->SetRangeY( 0.99, 1.005 );
  if( var == "Pt" && !setZoomIn) canvas->SetRangeY( 0.97, 1.01 );

  canvas->SetRangeRatio( 0.85, 1.15 );

  canvas->Latex_CMSSim();

  canvas->RegisterLatex( 0.16, 0.91, "#font[42]{#scale[0.6]{HLT ID efficiency (L3 / noID)}}");
  if( var != "Pt" )
    canvas->RegisterLatex( 0.16, 0.87, "#font[42]{#scale[0.6]{p_{T} > 26 GeV}}");

  canvas->Draw();
}

TGraphAsymmErrors* GetEffGraph( TString fileName, TString var )
{
  MuonHLT::TnPEffTool *tool = new MuonHLT::TnPEffTool(fileName);
  
  TGraphAsymmErrors* gEff = tool->CalcTnPEff_CutAndCount( var );
  return gEff;
}