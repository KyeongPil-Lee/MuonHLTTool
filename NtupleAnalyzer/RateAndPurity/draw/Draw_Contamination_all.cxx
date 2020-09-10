#include "PlotTools.h"

class PlotProducer
{
public:
  TString type_; // -- for canvas name

  Bool_t verbose_ = kFALSE;

  TString runTag_ = "";

  TString fileName_L1_  = "";
  TString fileName_L2_  = "";
  TString fileName_L3_  = "";
  TString fileName_iso_ = "";

  TString matchingType_ = "genFlag";

  // -- for the output dir name
  TString version_;
  TString dataset_;

  Bool_t isTkMu_ = kFALSE; // -- no L2 point in the bar plot

  Bool_t saveResults_ = kFALSE;

  PlotProducer(TString runTag, TString matchingType)
  {
    runTag_ = runTag;
    matchingType_ = matchingType;
  }

  void Set_FileNames(TString L1, TString L2, TString L3, TString iso)
  {
    fileName_L1_ = L1;
    fileName_L2_ = L2;
    fileName_L3_ = L3;
    fileName_iso_ = iso;
  }

  void Produce()
  {
    gStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
    gStyle->SetPadTickY(1);
    
    if( fileName_L3_.Contains("TkMu") || fileName_iso_.Contains("IsoTkMu") ) isTkMu_ = kTRUE;

    if( fileName_L1_  != "" ) DrawPlot_vs_Var_All(fileName_L1_,  matchingType_ );
    if( fileName_L2_  != "" ) DrawPlot_vs_Var_All(fileName_L2_,  matchingType_ );
    if( fileName_L3_  != "" ) DrawPlot_vs_Var_All(fileName_L3_,  matchingType_ );
    if( fileName_iso_ != "" ) DrawPlot_vs_Var_All(fileName_iso_, matchingType_ );

    Bool_t doBarPlot = kFALSE;
    if( isTkMu_ )
    {
      if( fileName_L1_ != "" && fileName_L3_ != "" &&fileName_iso_ != "" ) doBarPlot = kTRUE;
    }
    else
    {
      if( fileName_L1_ != "" && fileName_L2_ != "" && fileName_L3_ != "" &&fileName_iso_ != "" ) doBarPlot = kTRUE;
    }

    if( doBarPlot ) DrawBarPlot(matchingType_);
  }

private:
  void DrawPlot_vs_Var_All( TString fileName, TString matchingType )
  {
    DrawPlot_vs_Var( fileName, matchingType_, "Pt" );
    DrawPlot_vs_Var( fileName, matchingType_, "Eta" );
    DrawPlot_vs_Var( fileName, matchingType_, "nVertices" );
  }

  void DrawPlot_vs_Var( TString fileName, TString matchingType, TString var )
  {
    if( verbose_ ) cout << "[DrawPlot] matchingType = " << matchingType << ", var = " << var << endl;

    TString dataset = "";
    TString version = "";
    TString trigger = "";
    GetInfoFromFileName( fileName, dataset, version, trigger );

    // -- make directory if not exists
    TString outputDir = "./Plots_Contamination_"+version +"/"+dataset+"/"+trigger;
    if (gSystem->mkdir(outputDir, kTRUE) != -1)
      gSystem->mkdir(outputDir, kTRUE);

    TH1D* h_nMuon_prompt    = nullptr;
    TH1D* h_nMuon_nonPrompt = nullptr;
    TH1D* h_nMuon_nonMuon   = nullptr;
    GetNMuonHist( fileName, matchingType, var, h_nMuon_prompt, h_nMuon_nonPrompt, h_nMuon_nonMuon);

    if( !h_nMuon_prompt || !h_nMuon_nonPrompt || !h_nMuon_nonMuon ) return;

    // -- make fraction

    // ---- overall fraction (number)
    Double_t frac_prompt    = 0;
    Double_t frac_nonPrompt = 0;
    Double_t frac_nonMuon   = 0;
    GetOverallFraction( h_nMuon_prompt, h_nMuon_nonPrompt, h_nMuon_nonMuon, frac_prompt, frac_nonPrompt, frac_nonMuon );

    TH1D* h_nMuon_tot = (TH1D*)h_nMuon_prompt->Clone("h_nMuon_tot");
    h_nMuon_tot->Add( h_nMuon_nonPrompt );
    h_nMuon_tot->Add( h_nMuon_nonMuon );

    // ---- fraction histogram
    TH1D* h_frac_prompt    = CalcFracHist( h_nMuon_prompt,    h_nMuon_tot, "h_frac_prompt" );    
    TH1D* h_frac_nonPrompt = CalcFracHist( h_nMuon_nonPrompt, h_nMuon_tot, "h_frac_nonPrompt" );
    TH1D* h_frac_nonMuon   = CalcFracHist( h_nMuon_nonMuon,   h_nMuon_tot, "h_frac_nonMuon" );


    // -- scale by 100: %
    h_frac_prompt->Scale(100);
    h_frac_nonPrompt->Scale(100);
    h_frac_nonMuon->Scale(100);

    // -- histogram style
    Set_HistAttribute(h_frac_prompt);
    Set_HistAttribute(h_frac_nonPrompt);
    Set_HistAttribute(h_frac_nonMuon);

    h_frac_prompt->SetMarkerColor(kAzure-9);
    h_frac_prompt->SetFillColor(kAzure-9);

    h_frac_nonPrompt->SetMarkerColor(kRed-7);
    h_frac_nonPrompt->SetFillColor(kRed-7);

    h_frac_nonMuon->SetMarkerColor(kYellow-7);
    h_frac_nonMuon->SetFillColor(kYellow-7);

    Bool_t isLogX = kFALSE;
    Bool_t isLogY = kTRUE;

    // -- Stack
    THStack *hStack = new THStack("hStack", "");
    hStack->Add( h_frac_nonMuon );
    hStack->Add( h_frac_nonPrompt );
    hStack->Add( h_frac_prompt );

    TCanvas *c;
    TString canvasName = "c_Contamination_"+matchingType+"_"+var;
    SetCanvas_Square( c, canvasName, isLogX, isLogY );
    c->cd();

    // -- dummy hist: define axis ranges
    Double_t minX = 0;
    Double_t maxX = 0;
    if( var == "Pt" )        { minX = 3.0;  maxX = 200.0; }
    if( var == "Eta" )       { minX = -2.4; maxX = 2.4; }
    // if( var == "nVertices" ) { minX = 4;    maxX = 100; }
    if( var == "nVertices" ) { minX = 4;    maxX = 65; }

    TH1D *h_frame = new TH1D("h_frame", "", 1, minX, maxX);
    h_frame->SetStats(kFALSE);
    h_frame->SetLineWidth(0);
    h_frame->SetLineColor(kBlack);
    h_frame->SetMarkerSize(0);

    Double_t minY = 2e-2;
    Double_t maxY = 4000.;
    h_frame->GetYaxis()->SetRangeUser(minY, maxY);

    // -- draw
    h_frame->Draw();
    hStack->Draw("HISTSAME");
    h_frame->Draw("AXISSAME");

    SetAxis_SinglePad( h_frame->GetXaxis(), h_frame->GetYaxis(), GetTitleX(var, trigger), "Composition [\%]" );

    TLegend *legend;
    // SetLegend( legend, 0.15, 0.72, 0.90, 0.86);
    SetLegend( legend, 0.50, 0.81, 0.95, 0.93);
    legend->AddEntry( h_frac_prompt,    "#bf{Isolated muon"+TString::Format(" (%.1f%%)}",     100.*frac_prompt),    "f" );
    legend->AddEntry( h_frac_nonPrompt, "#bf{Non isolated muon"+TString::Format(" (%.1f%%)}", 100.*frac_nonPrompt), "f" );
    legend->AddEntry( h_frac_nonMuon,   "#bf{Unmatched"+TString::Format(" (%.1f%%)}",     100.*frac_nonMuon),   "f" );
    legend->Draw();

    TLatex latex;
    if( matchingType == "data" )
    {
      // Latex_Preliminary_NoDataInfo( latex );
      latex.DrawLatexNDC( 0.13, 0.96, "#font[62]{CMS}" );
      if( dataset.Contains("2016") )
        latex.DrawLatexNDC( 0.67 , 0.96, "#scale[0.6]{#font[42]{5 fb^{-1} (2016, 13 TeV)}}" );
      else if( dataset.Contains("2018") )
        latex.DrawLatexNDC( 0.67, 0.96, "#scale[0.6]{#font[42]{5 fb^{-1} (2018, 13 TeV)}}" );
    }
    else // -- MC
    {
      Latex_Simulation( latex );
      latex.DrawLatexNDC( 0.82, 0.96, "#scale[0.8]{#font[42]{13 TeV}}" );
      if(dataset.Contains("MUOVal"))
        latex.DrawLatexNDC( 0.45, 0.96,"#scale[0.55]{#font[42]{"+GetLatex_DatasetInfo(dataset)+"}}");
      else
        latex.DrawLatexNDC( 0.45, 0.96,"#scale[0.8]{#font[42]{"+GetLatex_DatasetInfo(dataset)+"}}");
    }

    latex.DrawLatexNDC( 0.17, 0.88, "#scale[0.8]{#font[42]{"+GetLatex_HLTInfo(trigger)+"}}" );


    c->Modified();
    c->Update();
    c->RedrawAxis();

    // gROOT->ProcessLine( "gErrorIgnoreLevel = 2001;");
    c->SaveAs(outputDir+"/"+canvasName+".pdf","pdf");
    // c->Close();
    // gROOT->ProcessLine( "gErrorIgnoreLevel = kPrint;");

    c->SetLogy(kFALSE);
    h_frame->GetYaxis()->SetRangeUser(0, 125);
    c->SaveAs(outputDir+"/"+canvasName+"_linear.pdf","pdf");
  }

  void DrawBarPlot(TString matchingType)
  {
    TString dataset = "";
    TString version = "";
    TString trigger = "";
    GetInfoFromFileName( fileName_L1_, dataset, version, trigger );

    // -- make directory if not exists
    TString outputDir = "./Plots_Contamination_"+version +"/"+dataset;
    if (gSystem->mkdir(outputDir, kTRUE) != -1)
      gSystem->mkdir(outputDir, kTRUE);

    TH1D* h_bar_prompt       = nullptr;
    TH1D* h_bar_nonPrompt = nullptr;
    TH1D* h_bar_nonMuon   = nullptr;

    GetBarHistogram( h_bar_prompt, h_bar_nonPrompt, h_bar_nonMuon );

    Set_HistAttribute( h_bar_prompt );
    Set_HistAttribute( h_bar_nonPrompt );
    Set_HistAttribute( h_bar_nonMuon );

    h_bar_prompt->SetMarkerColor(kAzure-9);
    h_bar_prompt->SetFillColor(kAzure-9);

    h_bar_nonPrompt->SetMarkerColor(kRed-7);
    h_bar_nonPrompt->SetFillColor(kRed-7);

    h_bar_nonMuon->SetMarkerColor(kYellow-7);
    h_bar_nonMuon->SetFillColor(kYellow-7);

    // -- Stack
    THStack *hStack = new THStack("hStack", "");
    hStack->Add( h_bar_prompt );
    hStack->Add( h_bar_nonPrompt );
    hStack->Add( h_bar_nonMuon );

    // -- draw
    TCanvas *c;
    TString canvasName = "c_barPlot_Contamination";
    if( isTkMu_ ) canvasName = canvasName + "_TkMu";
    SetCanvas_Square( c, canvasName, 0, 1 );
    c->cd();

    TH1D* h_axis = (TH1D*)h_bar_prompt->Clone(); // -- dummy histogram for axis definition
    h_axis->Reset("ICES");
    h_axis->Draw();
    hStack->Draw("SAME");
    h_axis->Draw("AXISSAME");

    SetAxis_SinglePad( h_axis->GetXaxis(), h_axis->GetYaxis(), "#mu trigger step", "Composition [\%]" );

    h_axis->GetYaxis()->SetRangeUser(5e-2, 1e+3);

    TLegend *legend;
    SetLegend( legend, 0.50, 0.81, 0.95, 0.93);
    legend->AddEntry( h_bar_prompt,    "#bf{Isolated muon}",     "f" );
    legend->AddEntry( h_bar_nonPrompt, "#bf{Non isolated muon}", "f" );
    legend->AddEntry( h_bar_nonMuon,   "#bf{Unmatched}",     "f" );
    legend->Draw();

    TLatex latex;

    TString info_triggerType = "";
    Bool_t is2016 = dataset.Contains("80X")  || dataset.Contains("2016");
    Bool_t is2018 = dataset.Contains("102X") || dataset.Contains("2018");

    if( is2016 && fileName_iso_.Contains("IsoMu") )   info_triggerType = "Cascade";
    if( is2016 && fileName_iso_.Contains("IsoTkMu") ) info_triggerType = "Tracker muon";
    if( is2018 )                                      info_triggerType = "Iterative";
    latex.DrawLatexNDC( 0.17, 0.90, "#scale[0.8]{#font[42]{"+info_triggerType+"}}" );

    if( matchingType == "data" )
    {
      // Latex_Preliminary_NoDataInfo( latex );
      latex.DrawLatexNDC( 0.13, 0.96, "#font[62]{CMS}" );
      if( is2016 )
        latex.DrawLatexNDC( 0.67 , 0.96, "#scale[0.6]{#font[42]{5 fb^{-1} (2016, 13 TeV)}}" );
      else if( is2018 )
        latex.DrawLatexNDC( 0.67, 0.96, "#scale[0.6]{#font[42]{5 fb^{-1} (2018, 13 TeV)}}" );
    }
    else
    {
      Latex_Simulation( latex );
      latex.DrawLatexNDC( 0.82, 0.96, "#scale[0.8]{#font[42]{13 TeV}}" );

      TString info_dataset = "t#bar{t} semi-lep MC";
      if( is2016 ) info_dataset = info_dataset + " (2016)";
      if( is2018 ) info_dataset = info_dataset + " (2018)";
      latex.DrawLatexNDC( 0.45, 0.96,"#scale[0.8]{#font[42]{"+info_dataset+"}}");
    }

    c->Modified();
    c->Update();
    c->RedrawAxis();

    c->SaveAs(outputDir+"/"+canvasName+".pdf","pdf");

    c->SetLogy(kFALSE);
    h_axis->GetYaxis()->SetRangeUser(0, 125);
    c->SaveAs(outputDir+"/"+canvasName+"_linear.pdf","pdf");

    if( saveResults_ )
    {
      TString fileName = outputDir+"/ROOTFile_BarPlot_Contamination_"+type_+".root";
      TFile *f_output = TFile::Open(fileName, "RECREATE");
      f_output->cd();
      h_bar_prompt->Write("h_bar_prompt");
      h_bar_nonPrompt->Write("h_bar_nonPrompt");
      h_bar_nonMuon->Write("h_bar_nonMuon");
      f_output->Close();
      cout << fileName << " is created" << endl;
    }

    delete c;
  }

  void GetBarHistogram(TH1D*& h_bar_prompt, TH1D*& h_bar_nonPrompt, TH1D*& h_bar_nonMuon )
  {
    if( isTkMu_ ) // -- no L2 sequence
    {
      h_bar_nonMuon = new TH1D("h_bar_nonMuon", "", 3, 0, 3);
      h_bar_nonMuon->GetXaxis()->SetBinLabel(1, "L1 (p_{T} > 22 GeV)");
      h_bar_nonMuon->GetXaxis()->SetBinLabel(2, "L3 (p_{T} > 24 GeV)");
      h_bar_nonMuon->GetXaxis()->SetBinLabel(3, "Iso. (p_{T} > 24 GeV)");
    }
    else
    {
      h_bar_nonMuon = new TH1D("h_bar_nonMuon", "", 4, 0, 4);
      h_bar_nonMuon->GetXaxis()->SetBinLabel(1, "L1 (p_{T} > 22 GeV)");
      h_bar_nonMuon->GetXaxis()->SetBinLabel(2, "L2 (p_{T} > 10 GeV)");
      h_bar_nonMuon->GetXaxis()->SetBinLabel(3, "L3 (p_{T} > 24 GeV)");
      h_bar_nonMuon->GetXaxis()->SetBinLabel(4, "Iso. (p_{T} > 24 GeV)");
    }

    h_bar_nonPrompt = (TH1D*)h_bar_nonMuon->Clone("h_bar_nonPrompt");
    h_bar_prompt    = (TH1D*)h_bar_nonMuon->Clone("h_bar_prompt");

    if( isTkMu_ )
    {
      FillBarHist( fileName_L1_,  1, matchingType_, h_bar_prompt, h_bar_nonPrompt, h_bar_nonMuon);
      FillBarHist( fileName_L3_,  2, matchingType_, h_bar_prompt, h_bar_nonPrompt, h_bar_nonMuon);
      FillBarHist( fileName_iso_, 3, matchingType_, h_bar_prompt, h_bar_nonPrompt, h_bar_nonMuon);
    }
    else
    {
      FillBarHist( fileName_L1_,  1, matchingType_, h_bar_prompt, h_bar_nonPrompt, h_bar_nonMuon);
      FillBarHist( fileName_L2_,  2, matchingType_, h_bar_prompt, h_bar_nonPrompt, h_bar_nonMuon);
      FillBarHist( fileName_L3_,  3, matchingType_, h_bar_prompt, h_bar_nonPrompt, h_bar_nonMuon);
      FillBarHist( fileName_iso_, 4, matchingType_, h_bar_prompt, h_bar_nonPrompt, h_bar_nonMuon);
    }
  }

  void FillBarHist( TString fileName, Int_t i_bin, TString matchingType, TH1D* h_bar_prompt, TH1D* h_bar_nonPrompt, TH1D* h_bar_nonMuon )
  {
    // -- Get histograms
    TString var = "Eta"; // -- reference: vs. eta plot

    TH1D* h_nMuon_prompt    = nullptr;
    TH1D* h_nMuon_nonPrompt = nullptr;
    TH1D* h_nMuon_nonMuon   = nullptr;
    GetNMuonHist( fileName, matchingType, var, h_nMuon_prompt, h_nMuon_nonPrompt, h_nMuon_nonMuon);

    if( !h_nMuon_prompt || !h_nMuon_nonPrompt || !h_nMuon_nonMuon ) return;

    // -- make fraction

    // ---- overall fraction (number)
    Double_t frac_prompt    = 0;
    Double_t frac_nonPrompt = 0;
    Double_t frac_nonMuon   = 0;
    GetOverallFraction( h_nMuon_prompt, h_nMuon_nonPrompt, h_nMuon_nonMuon, frac_prompt, frac_nonPrompt, frac_nonMuon );

    h_bar_prompt->SetBinContent(i_bin, frac_prompt * 100.0); // -- convert to %
    h_bar_prompt->SetBinError(i_bin, 0);

    h_bar_nonPrompt->SetBinContent(i_bin, frac_nonPrompt * 100.0); // -- convert to %
    h_bar_nonPrompt->SetBinError(i_bin,   0); // -- for now

    h_bar_nonMuon->SetBinContent(i_bin, frac_nonMuon * 100.0); // -- convert to %
    h_bar_nonMuon->SetBinError(i_bin, 0); // -- for now
  }

  void GetInfoFromFileName(TString fileName, TString& dataset, TString& version, TString& trigger)
  {
    TObjArray *split  = (fileName).Tokenize("-");

    dataset = ((TObjString*)(split->At(2)))->String();
    version = ((TObjString*)(split->At(3)))->String();
    trigger = ((TObjString*)(split->At(4)))->String();

    if( verbose_ )
      printf("[Dataset, version, trigger] = [%s, %s, %s]\n", dataset.Data(), version.Data(), trigger.Data());    
  }

  TString GetHistName(TString var, TString runTag, TString tag)
  {
    return "h_L3_"+var+"_"+runTag+"_"+tag+"_0.0_5.0";
  }

  void GetNMuonHist( TString fileName, TString matchingType, TString var, TH1D*& h_nMuon_prompt, TH1D*& h_nMuon_nonPrompt, TH1D*& h_nMuon_nonMuon )
  {
    TH1D* h = nullptr;
    if( matchingType == "genFlag" )
    {
      TString tag_tot             = "Mat0_IdNo_IsoNo_SimNo_HLT";
      TString tag_prompt          = "Mat1_IdNo_IsoNo_SimGenHard_HLT"; 
      TString tag_promptNonPrompt = "Mat1_IdNo_IsoNo_SimGen_HLT"; 

      TString histName_tot             = GetHistName( var, runTag_, tag_tot );
      TString histName_prompt          = GetHistName( var, runTag_, tag_prompt );
      TString histName_promptNonPrompt = GetHistName( var, runTag_, tag_promptNonPrompt );

      TH1D* h_nMuon_tot             = Get_Hist(fileName, histName_tot);
      TH1D* h_nMuon_promptNonPrompt = Get_Hist(fileName, histName_promptNonPrompt);

      h_nMuon_prompt = Get_Hist(fileName, histName_prompt);

      h_nMuon_nonPrompt = (TH1D*)h_nMuon_promptNonPrompt->Clone();
      h_nMuon_nonPrompt->Add( h_nMuon_prompt, -1 ); // -- (prompt + non-prompt) - prompt

      h_nMuon_nonMuon = (TH1D*)h_nMuon_tot->Clone("h_nMuon_nonMuon");
      h_nMuon_nonMuon->Add( h_nMuon_promptNonPrompt, -1 ); // -- total - (prompt + non-prompt)
    }
    else if( matchingType == "simHit" )
    {
      TString tag_tot       = "Mat0_IdNo_IsoNo_SimNo_HLT";
      TString tag_prompt    = "Mat1_IdNo_IsoNo_SimPromptMuon_HLT";
      TString tag_nonPrompt = "Mat1_IdNo_IsoNo_SimNonPromptMuon_HLT";

      TString histName_tot       = GetHistName( var, runTag_, tag_tot );
      TString histName_prompt    = GetHistName( var, runTag_, tag_prompt );
      TString histName_nonPrompt = GetHistName( var, runTag_, tag_nonPrompt );

      TH1D* h_nMuon_tot = Get_Hist(fileName, histName_tot);

      h_nMuon_prompt    = Get_Hist(fileName, histName_prompt);
      h_nMuon_nonPrompt = Get_Hist(fileName, histName_nonPrompt);

      h_nMuon_nonMuon = (TH1D*)h_nMuon_tot->Clone("h_nMuon_nonMuon");
      h_nMuon_nonMuon->Add( h_nMuon_prompt, -1 ); // -- total - prompt
      h_nMuon_nonMuon->Add( h_nMuon_nonPrompt, -1 ); // -- (total - prompt) - non-prompt
    }
    else if( matchingType == "data" ) // -- contamination plot with data: prompt = ID+Iso / non-prompt = ID only / non-muon = else
    {
      TString tag_tot             = "Mat0_IdNo_IsoNo_SimNo_HLT";
      TString tag_prompt          = "Mat1_IdLoose_IsoPFTight_SimNo_HLT"; 
      TString tag_promptNonPrompt = "Mat1_IdLoose_IsoNo_SimNo_HLT"; 

      TString histName_tot             = GetHistName( var, runTag_, tag_tot );
      TString histName_prompt          = GetHistName( var, runTag_, tag_prompt );
      TString histName_promptNonPrompt = GetHistName( var, runTag_, tag_promptNonPrompt );

      TH1D* h_nMuon_tot             = Get_Hist(fileName, histName_tot);
      TH1D* h_nMuon_promptNonPrompt = Get_Hist(fileName, histName_promptNonPrompt);

      h_nMuon_prompt = Get_Hist(fileName, histName_prompt);

      h_nMuon_nonPrompt = (TH1D*)h_nMuon_promptNonPrompt->Clone();
      h_nMuon_nonPrompt->Add( h_nMuon_prompt, -1 ); // -- (prompt + non-prompt) - prompt

      h_nMuon_nonMuon = (TH1D*)h_nMuon_tot->Clone("h_nMuon_nonMuon");
      h_nMuon_nonMuon->Add( h_nMuon_promptNonPrompt, -1 ); // -- total - (prompt + non-prompt)
    }
    else
    {
      cout << "[GetNMuonHist] matching type = " << matchingType << "is not recognized" << endl;
      h_nMuon_prompt = nullptr;
      h_nMuon_nonPrompt = nullptr;
      h_nMuon_nonMuon = nullptr;
    }
  }

  void GetOverallFraction( TH1D* h_nMuon_prompt, TH1D* h_nMuon_nonPrompt, TH1D* h_nMuon_nonMuon, Double_t& frac_prompt, Double_t& frac_nonPrompt, Double_t& frac_nonMuon )
  {
    // -- integral: should include under and overflow! to be consistent between variables
    Double_t nMuon_prompt    = h_nMuon_prompt->Integral(0,    h_nMuon_prompt->GetNbinsX()+1);
    Double_t nMuon_nonPrompt = h_nMuon_nonPrompt->Integral(0, h_nMuon_nonPrompt->GetNbinsX()+1);
    Double_t nMuon_nonMuon   = h_nMuon_nonMuon->Integral(0,   h_nMuon_nonMuon->GetNbinsX()+1);

    Double_t nMuon_tot = nMuon_prompt + nMuon_nonPrompt + nMuon_nonMuon;

    frac_prompt    = nMuon_prompt / nMuon_tot;
    frac_nonPrompt = nMuon_nonPrompt / nMuon_tot;
    frac_nonMuon   = nMuon_nonMuon / nMuon_tot;

    if( verbose_ )
    {
      printf("[frac_prompt]    %.1lf / %.1lf = %lf\n", nMuon_prompt, nMuon_tot, frac_prompt);
      printf("[frac_nonPrompt] %.1lf / %.1lf = %lf\n", nMuon_nonPrompt, nMuon_tot, frac_nonPrompt);
      printf("[frac_nonMuon]   %.1lf / %.1lf = %lf\n", nMuon_nonMuon, nMuon_tot, frac_nonMuon);

      // Double_t overflow_tot = h_nMuon_tot->GetBinContent(h_nMuon_tot->GetNbinsX()+1);
      // printf("[overflow] total = %.1lf\n", overflow_tot);
      // printf("   total after adding overflow = %.1lf\n", nMuon_tot + overflow_tot);
    }
  }

  TH1D* CalcFracHist( TH1D* h_NUM, TH1D* h_DEN, TString histName )
  {
    TH1D* h_frac = (TH1D*)h_NUM->Clone(histName);

    Int_t nBin = h_NUM->GetNbinsX();
    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;

      Double_t NUM     = h_NUM->GetBinContent(i_bin);
      Double_t err_NUM = h_NUM->GetBinError(i_bin);

      Double_t DEN     = h_DEN->GetBinContent(i_bin);
      Double_t err_DEN = h_DEN->GetBinError(i_bin);

      Double_t nFail     = DEN - NUM;
      Double_t err_nFail = sqrt(err_DEN*err_DEN - err_NUM*err_NUM);

      Double_t frac     = 0;
      Double_t err_frac = 0;
      if( DEN != 0 )
      {
        frac     = NUM / DEN;
        err_frac = CalcEffciencyError( NUM, err_NUM, nFail, err_nFail );
      }
      // else
      //   if( verbose_ )
      //     printf("[%d bin (%.1lf < value < %.1lf)] DEN == 0\n", i_bin, h_NUM->GetBinLowEdge(i_bin), h_NUM->GetBinLowEdge(i_bin+1));


      h_frac->SetBinContent(i_bin, frac);
      h_frac->SetBinError(i_bin, err_frac);
    }

    return h_frac;
  }

  Double_t CalcEffciencyError( Double_t nPass, Double_t ePass, Double_t nFail, Double_t eFail )
  {
    Double_t nTota = nPass+nFail;
    Double_t Err = 1./(nTota*nTota) * sqrt( nPass*nPass* eFail*eFail + nFail*nFail * ePass*ePass );
    return Err;
  }

  void Set_HistAttribute( TH1D* h )
  {
    h->SetStats(kFALSE);
    h->SetLineWidth(1);
    h->SetLineColor(kBlack);
    h->SetMarkerSize(0);
    h->SetMarkerStyle(0);
  }

  TString GetTitleX(TString var, TString trigger)
  {
    TString titleX = "";
    if( var == "Pt" ) titleX = "p_{T} (L3) [GeV]";
    if( var == "Eta" ) titleX = "#eta (L3)";
    if( var == "Phi" ) titleX = "#phi (L3)";
    if( var == "nVertices" ) titleX = "#scale[0.8]{Number of reconstructed primary vertices}";

    if(trigger.Contains("hltL1fL1sMu")) titleX.ReplaceAll("L3","L1");
    if(trigger.Contains("hltL2fL1s"))   titleX.ReplaceAll("L3","L2");

    return titleX;
  }

  TString GetLatex_DatasetInfo(TString dataset)
  {
    TString strDataset = "";
    if(dataset.Contains("2016"))  strDataset = "Data 2016";
    if(dataset.Contains("2018"))  strDataset = "Data 2018";
    if(dataset.Contains("80X"))   strDataset = "DY MC 2016";
    if(dataset.Contains("102X"))  strDataset = "DY MC 2018";
    if(dataset.Contains("TTSemiLep"))
      strDataset = strDataset.ReplaceAll("DY ", "t#bar{t} semi-lep ");
    if(dataset.Contains("MUOVal"))
      strDataset = strDataset.ReplaceAll("MC ", "MC (MUOVal) ");

    return strDataset;
  }

  TString GetLatex_HLTInfo(TString _name )
  {
    TString out = "";
    if(_name.Contains("IsoMu24"))
      out = "Isolated single #mu trigger with p_{T} > 24 GeV";
    else if(_name.Contains("IsoTkMu24"))
      out = "Isolated single #mu trigger with p_{T} > 24 GeV";
    else if(_name.Contains("Mu24"))
      out = "Non isolated single #mu trigger with p_{T} > 24 GeV";
    else if(_name.Contains("Mu50"))
      out = "Non isolated single #mu trigger with p_{T} > 50 GeV";
    else if(_name.Contains("hltL1fL1sMu"))
      out = "L1, p_{T} > 22 GeV";
    else if(_name.Contains("hltL2fL1s"))
      out = "L2, p_{T} > 10 GeV";
    else
      out = "Trigger name not found!!";

    if(_name.Contains("Tk"))
      out = "(Tk) " + out;

    return out;
  }

};

void Draw_BarChart_2016MC_Cascade()
{
  TString fileName_L1  = "Outputs/Output-MuonTriggerPurity-TTSemiLep80X-v25-hltL1fL1sMu22L1Filtered0-0.0-5.0.root";
  TString fileName_L2  = "Outputs/Output-MuonTriggerPurity-TTSemiLep80X-v01-hltL2fL1sMu22L1f0L2Filtered10Q-0.0-5.0.root";
  TString fileName_L3  = "Outputs/Output-MuonTriggerPurity-TTSemiLep80X-v25-HLT_Mu24_v-0.0-5.0.root";
  TString fileName_iso = "Outputs/Output-MuonTriggerPurity-TTSemiLep80X-v25-HLT_IsoMu24_v-0.0-5.0.root";

  PlotProducer* producer = new PlotProducer("Run1to1", "genFlag");
  producer->Set_FileNames(fileName_L1, fileName_L2, fileName_L3, fileName_iso);
  producer->Produce();
}

void Draw_BarChart_2016MC_TrackerMuon()
{
  TString fileName_L1  = "Outputs/Output-MuonTriggerPurity-TTSemiLep80X-v25-hltL1fL1sMu22L1Filtered0-0.0-5.0.root";
  TString fileName_L2  = "";
  TString fileName_L3  = "Outputs/Output-MuonTriggerPurity-TTSemiLep80X-v25-HLT_TkMu24_v-0.0-5.0.root";
  TString fileName_iso = "Outputs/Output-MuonTriggerPurity-TTSemiLep80X-v25-HLT_IsoTkMu24_v-0.0-5.0.root";

  PlotProducer* producer = new PlotProducer("Run1to1", "genFlag");
  producer->Set_FileNames(fileName_L1, fileName_L2, fileName_L3, fileName_iso);
  producer->Produce();
}

void Draw_BarChart_2018MC_Iterative()
{
  TString fileName_L1  = "Outputs/Output-MuonTriggerPurity-TTSemiLep102X-v101-hltL1fL1sMu22L1Filtered0-0.0-5.0.root";
  TString fileName_L2  = "Outputs/Output-MuonTriggerPurity-TTSemiLep102X-v101-hltL2fL1sSingleMu22L1f0L2Filtered10Q-0.0-5.0.root";
  TString fileName_L3  = "Outputs/Output-MuonTriggerPurity-TTSemiLep102X-v101-HLT_Mu24_v-0.0-5.0.root";
  TString fileName_iso = "Outputs/Output-MuonTriggerPurity-TTSemiLep102X-v101-HLT_IsoMu24_v-0.0-5.0.root";

  PlotProducer* producer = new PlotProducer("Run1to1", "genFlag");
  producer->Set_FileNames(fileName_L1, fileName_L2, fileName_L3, fileName_iso);
  producer->Produce();
}

void Draw_BarChart_2016Data_Cascade()
{
  TString fileName_L1  = "Outputs/Output-MuonTriggerPurity-Run2016H-v201-hltL1fL1sMu22L1Filtered0-0.0-5.0.root";
  TString fileName_L2  = "Outputs/Output-MuonTriggerPurity-Run2016H-v201-hltL2fL1sMu22L1f0L2Filtered10Q-0.0-5.0.root";
  TString fileName_L3  = "Outputs/Output-MuonTriggerPurity-Run2016H-v201-HLT_Mu24_v-0.0-5.0.root";
  TString fileName_iso = "Outputs/Output-MuonTriggerPurity-Run2016H-v201-HLT_IsoMu24_v-0.0-5.0.root";

  PlotProducer* producer = new PlotProducer("Run0to999999", "data");
  producer->Set_FileNames(fileName_L1, fileName_L2, fileName_L3, fileName_iso);
  producer->Produce();
}

void Draw_BarChart_2016Data_TrackerMuon()
{
  TString fileName_L1  = "Outputs/Output-MuonTriggerPurity-Run2016H-v201-hltL1fL1sMu22L1Filtered0-0.0-5.0.root";
  TString fileName_L2  = "";
  TString fileName_L3  = "Outputs/Output-MuonTriggerPurity-Run2016H-v201-HLT_TkMu24_v-0.0-5.0.root";
  TString fileName_iso = "Outputs/Output-MuonTriggerPurity-Run2016H-v201-HLT_IsoTkMu24_v-0.0-5.0.root";

  PlotProducer* producer = new PlotProducer("Run0to999999", "data");
  producer->Set_FileNames(fileName_L1, fileName_L2, fileName_L3, fileName_iso);
  producer->Produce();
}

void Draw_BarChart_2018Data_Iterative()
{
  TString fileName_L1  = "Outputs/Output-MuonTriggerPurity-Run2018D-v201-hltL1fL1sMu22L1Filtered0-0.0-5.0.root";
  TString fileName_L2  = "Outputs/Output-MuonTriggerPurity-Run2018D-v201-hltL2fL1sSingleMu22L1f0L2Filtered10Q-0.0-5.0.root";
  TString fileName_L3  = "Outputs/Output-MuonTriggerPurity-Run2018D-v201-HLT_Mu24_v-0.0-5.0.root";
  TString fileName_iso = "Outputs/Output-MuonTriggerPurity-Run2018D-v201-HLT_IsoMu24_v-0.0-5.0.root";

  PlotProducer* producer = new PlotProducer("Run0to999999", "data");
  producer->Set_FileNames(fileName_L1, fileName_L2, fileName_L3, fileName_iso);
  producer->Produce();
}

void Draw_BarChart_HLTPhysics2016Data_Cascade()
{
  TString fileName_L1  = "Outputs/Output-MuonTriggerPurity-HLTPhysicsRun2016Hv2-v205-hltL1fL1sMu22L1Filtered0-0.0-5.0.root";
  TString fileName_L2  = "Outputs/Output-MuonTriggerPurity-HLTPhysicsRun2016Hv2-v205-hltL2fL1sMu22L1f0L2Filtered10Q-0.0-5.0.root";
  TString fileName_L3  = "Outputs/Output-MuonTriggerPurity-HLTPhysicsRun2016Hv2-v205-HLT_Mu24_v-0.0-5.0.root";
  TString fileName_iso = "Outputs/Output-MuonTriggerPurity-HLTPhysicsRun2016Hv2-v205-HLT_IsoMu24_v-0.0-5.0.root";

  PlotProducer* producer = new PlotProducer("Run0to999999", "data");
  producer->Set_FileNames(fileName_L1, fileName_L2, fileName_L3, fileName_iso);
  producer->verbose_ = kTRUE;
  producer->Produce();
}

void Draw_BarChart_HLTPhysics2016Data_TrackerMuon()
{
  TString fileName_L1  = "Outputs/Output-MuonTriggerPurity-HLTPhysicsRun2016Hv2-v205-hltL1fL1sMu22L1Filtered0-0.0-5.0.root";
  TString fileName_L2  = "";
  TString fileName_L3  = "Outputs/Output-MuonTriggerPurity-HLTPhysicsRun2016Hv2-v205-HLT_TkMu24_v-0.0-5.0.root";
  TString fileName_iso = "Outputs/Output-MuonTriggerPurity-HLTPhysicsRun2016Hv2-v205-HLT_IsoTkMu24_v-0.0-5.0.root";

  PlotProducer* producer = new PlotProducer("Run0to999999", "data");
  producer->Set_FileNames(fileName_L1, fileName_L2, fileName_L3, fileName_iso);
  producer->verbose_ = kTRUE;
  producer->Produce();
}

void Draw_BarChart_HLTPhysics2018Data_Iterative()
{

  TString fileName_L1  = "Outputs/Output-MuonTriggerPurity-HLTPhysicsRun2018D-v205-hltL1fL1sMu22L1Filtered0-0.0-5.0.root";
  TString fileName_L2  = "Outputs/Output-MuonTriggerPurity-HLTPhysicsRun2018D-v205-hltL2fL1sSingleMu22L1f0L2Filtered10Q-0.0-5.0.root";
  TString fileName_L3  = "Outputs/Output-MuonTriggerPurity-HLTPhysicsRun2018D-v205-HLT_Mu24_v-0.0-5.0.root";
  TString fileName_iso = "Outputs/Output-MuonTriggerPurity-HLTPhysicsRun2018D-v205-HLT_IsoMu24_v-0.0-5.0.root";

  PlotProducer* producer = new PlotProducer("Run0to999999", "data");
  producer->Set_FileNames(fileName_L1, fileName_L2, fileName_L3, fileName_iso);
  producer->verbose_ = kTRUE;
  producer->Produce();
}



void Draw_Contamination_all()
{
  // Draw_BarChart_2016MC_Cascade();
  // Draw_BarChart_2016MC_TrackerMuon();
  // Draw_BarChart_2018MC_Iterative();

  // Draw_BarChart_2016Data_Cascade();
  // Draw_BarChart_2016Data_TrackerMuon();
  // Draw_BarChart_2018Data_Iterative();

  Draw_BarChart_HLTPhysics2016Data_Cascade();
  Draw_BarChart_HLTPhysics2016Data_TrackerMuon();
  Draw_BarChart_HLTPhysics2018Data_Iterative();
}