// #include <PlotTools.h>
#include "PlotTools.h"

vector<TString> vec_Sel = {
  // "Mat1_IdPfOrTrkOrGlb_IsoNo_SimNo_HLT"
  "Mat1_IdTight_IsoPFTight_SimNo_HLT"

  // "Mat1_IdNo_IsoNo_HLT",

  // "Mat1_IdPfOrTrkOrGlb_IsoNo_HLT",
  // "Mat1_IdTrk_IsoNo_HLT",
  // "Mat1_IdGlb_IsoNo_HLT",
  // "Mat1_IdPf_IsoNo_HLT",

  // "Mat1_IdTrkPlus1MS_IsoNo_HLT",

  // "Mat1_IdTrkAndTrkQual_IsoNo_HLT",
  // "Mat1_IdGlbAndTrkQual_IsoNo_HLT",
  // "Mat1_IdGlbAndTrkQualPlus1MS_IsoNo_HLT",
  // "Mat1_IdGlbAndTrkQualPlus2MS_IsoNo_HLT",
  // "Mat1_IdGlbAndMuonQual_IsoNo_HLT",

  // "Mat1_IdTight_IsoNo_HLT",
  // "Mat1_IdTight_IsoTkLoose_HLT",
  // "Mat1_IdTight_IsoTkTight_HLT",
  // "Mat1_IdTight_IsoPFLoose_HLT",
  // "Mat1_IdTight_IsoPFTight_HLT",

  // "Mat1_IdHighPt_IsoNo_HLT",
  // "Mat1_IdHighPt_IsoTkLoose_HLT",
  // "Mat1_IdHighPt_IsoTkTight_HLT",
  // "Mat1_IdHighPt_IsoPFLoose_HLT",
  // "Mat1_IdHighPt_IsoPFTight_HLT"
};


//-- Setting Purity Seq --//
vector<TString> vec_Seq = {
  "Mat1_IdNo_IsoNo_HLT",
  "Mat1_IdPfOrTrkOrGlb_IsoNo_HLT",
  "Mat1_IdGlb_IsoNo_HLT",
  "Mat1_IdGlbAndTrkQual_IsoNo_HLT",
  "Mat1_IdTight_IsoNo_HLT",
  "Mat1_IdTight_IsoTkLoose_HLT",
  "Mat1_IdTight_IsoPFTight_HLT"
};

vector<Color_t> vec_Color_Seq = {
  kBlack,
  kRed,
  kGreen+2,
  kViolet,
  kBlue,
  kCyan,
  kMagenta+3
};

vector<Int_t>   vec_Marker_Seq = {
  20,
  21,
  22,
  23,
  24,
  25,
  26
};
//-- Setting Purity Seq --//

class DrawingTool
{
public:
  TString FileName;

  TString Dataset;
  TString Version;
  TString RunTag;
  TString Trigger;
  TString EtaBin; // ex) "0.0_0.9"

  Bool_t IsPrescaled;
  TString PrescalName;

  DrawingTool() {
    this->Init();
  }

  DrawingTool( TString _FileName, TString _RunTag, Bool_t _IsPrescaled ): DrawingTool()
  {
    this->FileName = _FileName;

    /*
    0      1                 2   3     4   5       6 7   8
    Output MuonTriggerPurity QCD v2018 HLT IsoMu24 v 1.2 1.6.root

    0      1                 2         3     4   5       6 7   8
    Output MuonTriggerPurity SigleMuon v2018 HLT IsoMu24 v 0.9 1.2
    Output MuonTriggerPurity Data2018D v23 hltL2fL1sSingleMu22L1f0L2Filtered10Q.root"
    */

    TObjArray *split  = (this->FileName).Tokenize("-");
    TString _Dataset = ((TObjString*)(split->At(2)))->String();
    TString _Version = ((TObjString*)(split->At(3)))->String();
    TString _Trigger = ((TObjString*)(split->At(4)))->String();
    TString _EtaLo = "0.0";  // ((TObjString*)(split->At(5)))->String();
    TString _EtaUp = "5.0";  // ((TObjString*)(split->At(6)))->String();
            // _EtaUp = _EtaUp.Remove(3,5);
    TString _EtaBin = _EtaLo+"_"+_EtaUp;

    this->Dataset = _Dataset;
    this->Version = _Version;
    this->RunTag = _RunTag;
    this->Trigger = _Trigger.ReplaceAll(".root", "");  // _Trigger;
    this->EtaBin = _EtaBin;
    this->IsPrescaled = _IsPrescaled;

    cout << endl;
    cout << "FileName : " << this->FileName << endl;
    cout << "EtaBin : " << this->EtaBin << endl;
    cout << "IsPrescaled : " << this->IsPrescaled << endl;

    if(this->IsPrescaled) {
      this->PrescalName = "_UnPS";
    }
  }

  void DrawPurity_All()
  {
    Bool_t dynamic_range = kTRUE;  //kFALSE;

    Int_t nSel = (Int_t)vec_Sel.size();

    for(Int_t i=0; i<nSel; ++i) {
      this->DrawPurity( vec_Sel[i], "nVertices", 2, 65,   0.3, 1.25,   dynamic_range );
      // this->DrawPurity( vec_Sel[i], "Pt",       24, 100,  0.1, 1.25,   dynamic_range );
      this->DrawPurity( vec_Sel[i], "Pt",       24, 200,  0.1, 1.25,   dynamic_range );
      // this->DrawPurity( vec_Sel[i], "Pt",       24, 500,  0.1, 1.25,   dynamic_range );
      // this->DrawPurity( vec_Sel[i], "Pt",       24, 1000, 0.1, 1.25,   dynamic_range );
      this->DrawPurity( vec_Sel[i], "Eta",    -2.4, 2.4, 0.83, 1.04,  dynamic_range );
    }
  }

  void DrawPuritySeq_All()
  {
    Bool_t dynamic_range = kFALSE;  //kFALSE;

    this->DrawPuritySeq( "nVertices", 2, 65,   0.1, 1.29, dynamic_range );
    this->DrawPuritySeq( "Pt",       24, 100,  0.0, 1.39, dynamic_range );
    this->DrawPuritySeq( "Pt",       24, 200,  0.0, 1.39, dynamic_range );
    this->DrawPuritySeq( "Pt",       24, 500,  0.0, 1.39, dynamic_range );
    this->DrawPuritySeq( "Pt",       24, 1000, 0.0, 1.39, dynamic_range );
    this->DrawPuritySeq( "Eta",    -2.4, 2.4,  0.5, 1.15, dynamic_range );
  }

private:
  void Init() {
    this->FileName = "";
    this->Dataset = "";
    this->Version = "";
    this->RunTag = "";
    this->Trigger = "";
    this->EtaBin = "0.0_5.0";
    this->IsPrescaled = kFALSE;
    this->PrescalName = "";
  }

  void DrawPurity(  TString Selection, TString Var,
                    Double_t minX, Double_t maxX,
                    Double_t minY, Double_t maxY,
                    Bool_t dynamic_ratio = kFALSE )
  {
    Bool_t isLogX = kFALSE;
    if(Var == "Pt") {
      isLogX = (maxX>400);
      if((this->Trigger).Contains("Mu50"))
        minX = 50.;
      else if((this->Trigger).Contains("hltL1"))
        minX = 0.;
      else if((this->Trigger).Contains("hltL2"))
        minX = 0.;
    }

    TString NumName_Purity = "";
    TString DenName_Purity = "";
    TString TitleX = "";
    TString etaCategoryLong = "";
    TString EtaLo = "";
    TString EtaUp = "";

    TString IdSelLong = Get_Selection_Latex(Selection);

    TObjArray *split2  = (this->EtaBin).Tokenize("_");
    EtaLo = ((TObjString*)(split2->At(0)))->String();
    EtaUp = ((TObjString*)(split2->At(1)))->String();
    etaCategoryLong = EtaLo + " < |#eta| < " + EtaUp;
    if(this->EtaBin == "0.0_5.0")
      etaCategoryLong = "";

    (this->Trigger).ReplaceAll("_v", "").ReplaceAll("HLT_","");

    //--- Output file
    TString Dir = "./Purity_HLTPaperPlots";  //  "./Plots_"+this->Version+"/Purity/"+Selection;
    if (gSystem->mkdir(Dir,kTRUE) != -1)
      gSystem->mkdir(Dir,kTRUE);

    if(Var=="Pt") {
      TitleX = "p_{T} (L3) [GeV]";
    }
    else if(Var=="Eta") {
      TitleX = "#eta (L3)";
    }
    else if(Var=="Phi") {
      TitleX = "#phi (L3)";
    }
    else if(Var=="nVertices") {
      TitleX = "#scale[0.8]{Number of reconstructed primary vertices}";
    }
    if( (this->Trigger).Contains("hltL1")) {
      TitleX.ReplaceAll("L3", "L1");
    }
    if( (this->Trigger).Contains("hltL2")) {
      TitleX.ReplaceAll("L3", "L2");
    }

    vector<TString> vec_Tag;
    vector<TString> vec_Str;
    vector<Color_t> vec_Color;
    vector<Int_t>   vec_Marker;

    vec_Tag    = { "Data2016H", "Data2018D" };
    vec_Str    = { "2016 Data", "2018 Data" };
    vec_Color  = { kBlack, kBlue };
    vec_Marker = { 20, 22 };

    // vec_Tag    = { "Data2016H",  "DY80X", "Data2018D",  "DY102X" };
    // vec_Str    = { "2016 Data", "2016 DY MC", "2018 Data", "2018 DY MC" };
    // vec_Color  = { kBlack, kRed, kBlue, kMagenta };
    // vec_Marker = { 20, 25, 22, 32 };

    const Int_t n_tag = (Int_t)vec_Tag.size();
    TH1D* Purity[n_tag];

    // -- Canvas -- //
    TCanvas *c;
    TString canvasName = "Purity_"+this->Trigger+"_"+Selection+"_"+Var;
    if(Var=="Pt") canvasName += maxX;
    if( this->Version.Contains("Dimuon") )  canvasName = canvasName + "_offlineDimuon";
    SetCanvas_Square( c, canvasName, isLogX, 0 );
    c->cd();

    TH1D *h_Frame = new TH1D("h_Frame", "", 1, minX, maxX);
    h_Frame->SetStats(kFALSE);
    h_Frame->SetLineWidth(0);
    h_Frame->SetMarkerSize(0);

    TLegend *legend;
    SetLegend( legend, 0.46, 0.16, 0.85, 0.36 );
    // SetLegend( legend, 0.61, 0.77, 0.95, 0.92 );

    for(Int_t i=0; i<n_tag; ++i) {

      TString RunTagTemp = this->RunTag;

      NumName_Purity = "h_L3_"+Var+"_"+RunTagTemp+"_"+Selection+"_"+this->EtaBin;
      DenName_Purity = "h_L3_"+Var+"_"+RunTagTemp+"_Mat0_IdNo_IsoNo_SimNo_HLT"+"_"+this->EtaBin;

      TString FileNameTemp = this->FileName;
      FileNameTemp.ReplaceAll(this->Dataset,vec_Tag[i]);
      if( this->Trigger.Contains("hltL2fL1sMu22L1f0L2Filtered10Q") && vec_Tag[i].Contains("Data2018"))
        FileNameTemp.ReplaceAll("hltL2fL1sMu22L1f0L2Filtered10Q","hltL2fL1sSingleMu22L1f0L2Filtered10Q");

      TH1D *h_Num = Get_Hist("./Outputs/"+FileNameTemp, NumName_Purity, "h_Num");
      TH1D *h_Den = Get_Hist("./Outputs/"+FileNameTemp, DenName_Purity, "h_Den");
      Double_t nNum = (Double_t)(h_Num->Integral());
      Double_t nDen = (Double_t)(h_Den->Integral());
      printf("\n\t[%s, %s, %s] (nNum, nDen, Purity) = (%.6f, %.6f, %.6f)\n",
                (vec_Tag[i]).Data(), Selection.Data(), Var.Data(), nNum, nDen, (nNum/nDen) );

      Purity[i] = (TH1D*)h_Num->Clone("Purity_"+vec_Tag[i]);

      for(Int_t iB=1; iB<=h_Num->GetNbinsX(); ++iB) {
        Double_t nPass = h_Num->GetBinContent(iB);
        Double_t ePass = h_Num->GetBinError(iB);
        Double_t nTota = h_Den->GetBinContent(iB);
        Double_t eTota = h_Den->GetBinError(iB);
        Double_t nFail = nTota - nPass;
        Double_t eFail = sqrt(eTota*eTota - ePass*ePass);

        Double_t Pur = 0;
        Double_t Err = 0;
        if( !(nTota==0) ) {
          Pur = nPass / nTota;
          Err = this->CalcEffciencyError( nPass, ePass, nFail, eFail );
        }

        // cout << "\t\t[" << h_Num->GetBinLowEdge(iB) << ", " << h_Num->GetBinLowEdge(iB+1) << "]" << nPass << "/" << nTota << " = " << Pur << endl;

        Purity[i]->SetBinContent( iB, Pur );
        Purity[i]->SetBinError( iB, Err );
      }
      Purity[i]->SetLineWidth(1);
      Purity[i]->SetLineColor(vec_Color[i]);
      Purity[i]->SetMarkerSize(1.3);
      Purity[i]->SetMarkerStyle(vec_Marker[i]);
      Purity[i]->SetMarkerColor(vec_Color[i]);

      legend->AddEntry( Purity[i], "#bf{ "+vec_Str[i]+TString::Format(" (%.1f%%)", (nNum/nDen)*100.)+"}", "lep" );
    }

    // -- Draw -- //
    if(dynamic_ratio) {
      Double_t minY_temp = Get_NonZeroMin( Purity[0], minX, maxX ) < Get_NonZeroMin( Purity[1], minX, maxX ) ?
                           Get_NonZeroMin( Purity[0], minX, maxX ) : Get_NonZeroMin( Purity[1], minX, maxX );
      Double_t maxY_temp = Get_MaxInRange( Purity[1], minX, maxX, 0 ) > Get_MaxInRange( Purity[0], minX, maxX, 0 ) ?
                           Get_MaxInRange( Purity[1], minX, maxX, 0 ) : Get_MaxInRange( Purity[0], minX, maxX, 0 );
      Double_t diff     = (maxY_temp - minY_temp);
      Double_t minExtra = diff > 0.5 ? 0.3*diff : 1.0*diff;
      Double_t maxExtra = diff > 0.5 ? 0.4*diff : 0.7*diff;
      minY = minY_temp - minExtra;
      maxY = maxY_temp + maxExtra;
      if(minY<0)  minY = 0;
    }

    h_Frame->GetYaxis()->SetRangeUser(minY, maxY);
    SetAxis_SinglePad( h_Frame->GetXaxis(), h_Frame->GetYaxis(), TitleX, "Purity" );
    h_Frame->Draw();
    for(Int_t i=0; i<n_tag; ++i) {
      Purity[i]->Draw("PE SAME");
    }

    legend->Draw();

    TLatex latex;
    Latex_Preliminary_NoDataInfo( latex );
    // lumis:  35.922 (2016) + 59.69 (2018)
    // XX fb-1 (YEAR 1), YY fb-1 (YEAR 2) (13 TeV)
    // latex.DrawLatexNDC( 0.50, 0.96, "#scale[0.7]{#font[42]{95.6 fb^{-1} (13 TeV, 2016 and 2018)}}" );
    latex.DrawLatexNDC( 0.46, 0.96, "#scale[0.6]{#font[42]{35.9 fb^{-1} (2016), 59.7 fb^{-1} (2018) (13 TeV)}}" );
    latex.DrawLatexNDC( 0.17, 0.88, "#scale[0.8]{#font[42]{"+Get_HLT_Latex(this->Trigger)+"}}" );
    // latex.DrawLatexNDC( 0.17, 0.82, "#scale[0.8]{#font[42]{Offline: "+IdSelLong+"}}");

    gROOT->ProcessLine( "gErrorIgnoreLevel = 2001;");
    c->SaveAs(Dir+"/"+canvasName+".pdf","pdf");
    // if( Selection == "Mat1_IdPfOrTrkOrGlb_IsoNo_HLT" )
    //   c->SaveAs(Dir+"/"+canvasName+".C","C");
    c->Close();
    gROOT->ProcessLine( "gErrorIgnoreLevel = kPrint;");

    delete h_Frame;
  }

  void DrawPuritySeq( TString Var,
                    Double_t minX, Double_t maxX,
                    Double_t minY, Double_t maxY,
                    Bool_t dynamic_ratio = kTRUE )
  {
    TString EventLevelTag = "";
    if( (this->Version).Contains("Dimuon") )
      EventLevelTag = "| offline M_{#mu#mu} - 91 | < 10";

    Bool_t isLogX = kFALSE;
    if(Var == "Pt") {
      isLogX = (maxX>400);
      if((this->Trigger).Contains("Mu50"))
        minX = 50.;
    }

    TString NumName_Purity = "";
    TString DenName_Purity = "";
    TString TitleX = "";
    TString etaCategoryLong = "";
    TString EtaLo = "";
    TString EtaUp = "";

    TObjArray *split2  = (this->EtaBin).Tokenize("_");
    EtaLo = ((TObjString*)(split2->At(0)))->String();
    EtaUp = ((TObjString*)(split2->At(1)))->String();
    etaCategoryLong = EtaLo + " < |#eta| < " + EtaUp;
    if(this->EtaBin == "0.0_5.0")
      etaCategoryLong = "";

    (this->Trigger).ReplaceAll("_v", "").ReplaceAll("HLT_","");

    TString strDataset = "";
    if(this->Dataset.Contains("2016"))  strDataset = "Data 2016";
    if(this->Dataset.Contains("2018"))  strDataset = "Data 2018";
    if(this->Dataset.Contains("80X"))   strDataset = "DY MC 2016";
    if(this->Dataset.Contains("102X"))  strDataset = "DY MC 2018";

    //--- Output file
    TString Dir = "./Purity_HLTPaperPlots";  // "./Plots_"+this->Version+"/Purity/"+Dataset+"/"+this->Trigger;
    if (gSystem->mkdir(Dir,kTRUE) != -1)
      gSystem->mkdir(Dir,kTRUE);

    if(Var=="Pt") {
      TitleX = "p_{T} (L3) [GeV]";
    }
    else if(Var=="Eta") {
      TitleX = "#eta (L3)";
    }
    else if(Var=="Phi") {
      TitleX = "#phi (L3)";
    }
    else if(Var=="nVertices") {
      TitleX = "# VTX";
    }


    const Int_t n_tag = (this->Trigger).Contains("IsoMu") ? (Int_t)vec_Seq.size() : (Int_t)vec_Seq.size() - 2 ;
    TH1D* Purity[n_tag];

    // -- Canvas -- //
    TCanvas *c;
    TString canvasName = "Purity_"+this->Dataset+"_"+this->Trigger+"_"+Var;
    if(Var=="Pt") canvasName += maxX;
    if( this->Version.Contains("Dimuon") )  canvasName = canvasName + "_offlineDimuon";
    SetCanvas_Square( c, canvasName, isLogX, 0 );
    c->cd();

    TH1D *h_Frame = new TH1D("h_Frame", "", 1, minX, maxX);
    h_Frame->SetStats(kFALSE);
    h_Frame->SetLineWidth(0);
    h_Frame->SetMarkerSize(0);

    TLegend *legend;
    SetLegend( legend, 0.50, 0.77, 0.95, 0.92 );

    for(Int_t i=0; i<n_tag; ++i) {

      TString RunTagTemp = this->RunTag;

      TString strSeq = vec_Seq[i];
      if(this->Trigger.Contains("Mu50"))
        strSeq.ReplaceAll("IdTight", "IdHighPt");

      NumName_Purity = "h_L3_"+Var+"_"+RunTagTemp+"_"+strSeq+"_"+this->EtaBin;
      DenName_Purity = "h_L3_"+Var+"_"+RunTagTemp+"_Mat0_IdNo_IsoNo_SimNo_HLT"+"_"+this->EtaBin;

      TString FileNameTemp = this->FileName;

      TH1D *h_Num = Get_Hist("./Outputs/"+FileNameTemp, NumName_Purity, "h_Num");
      TH1D *h_Den = Get_Hist("./Outputs/"+FileNameTemp, DenName_Purity, "h_Den");
      Double_t nNum = (Double_t)(h_Num->Integral());
      Double_t nDen = (Double_t)(h_Den->Integral());
      printf("\n\t[%s, %s, %s] (nNum, nDen, Purity) = (%.6f, %.6f, %.6f)\n",
                (strSeq).Data(), strSeq.Data(), Var.Data(), nNum, nDen, (nNum/nDen) );

      Purity[i] = (TH1D*)h_Num->Clone("Purity_"+strSeq);

      for(Int_t iB=1; iB<=h_Num->GetNbinsX(); ++iB) {
        Double_t nPass = h_Num->GetBinContent(iB);
        Double_t ePass = h_Num->GetBinError(iB);
        Double_t nTota = h_Den->GetBinContent(iB);
        Double_t eTota = h_Den->GetBinError(iB);
        Double_t nFail = nTota - nPass;
        Double_t eFail = sqrt(eTota*eTota - ePass*ePass);

        Double_t Pur = 0;
        Double_t Err = 0;
        if( !(nTota==0) ) {
          Pur = nPass / nTota;
          Err = this->CalcEffciencyError( nPass, ePass, nFail, eFail );
        }

        // cout << "\t\t[" << h_Num->GetBinLowEdge(iB) << ", " << h_Num->GetBinLowEdge(iB+1) << "]" << nPass << "/" << nTota << " = " << Pur << endl;

        Purity[i]->SetBinContent( iB, Pur );
        Purity[i]->SetBinError( iB, Err );
      }
      Purity[i]->SetLineWidth(1);
      Purity[i]->SetLineColor(vec_Color_Seq[i]);
      Purity[i]->SetMarkerSize(1.3);
      Purity[i]->SetMarkerStyle(vec_Marker_Seq[i]);
      Purity[i]->SetMarkerColor(vec_Color_Seq[i]);

      TString IdSelLong = Get_Selection_Latex(strSeq);
      legend->AddEntry( Purity[i], "#bf{"+IdSelLong+TString::Format(" (%.1f%%)", (nNum/nDen)*100.)+"}", "lep" );
    }

    // -- Draw -- //
    if(dynamic_ratio) {
      Double_t minY_temp = Get_NonZeroMin( Purity[n_tag-1], minX, maxX );
      Double_t maxY_temp = Get_MaxInRange( Purity[0], minX, maxX, 0 );
      Double_t diff     = (maxY_temp - minY_temp);
      Double_t minExtra = diff > 0.5 ? 0.3*diff : 1.0*diff;
      Double_t maxExtra = diff > 0.5 ? 0.4*diff : 0.7*diff;
      minY = minY_temp - minExtra;
      maxY = maxY_temp + maxExtra;
      if(minY<0)  minY = 0;
    }

    h_Frame->GetYaxis()->SetRangeUser(minY, maxY);
    SetAxis_SinglePad( h_Frame->GetXaxis(), h_Frame->GetYaxis(), TitleX, "Offline/L3" );
    h_Frame->Draw();
    for(Int_t i=0; i<n_tag; ++i) {
      Purity[i]->Draw("PE SAME");
    }

    legend->Draw();

    TLatex latex;
    Latex_Preliminary_NoDataInfo( latex );
    latex.DrawLatexNDC( 0.82, 0.96, "#scale[0.8]{#font[42]{13 TeV}}" );
    //latex.DrawLatexNDC( 0.16, 0.91, "#scale[0.6]{#font[42]{#bf{"+this->RunTag+"  "+this->Trigger+"}}}" );
    latex.DrawLatexNDC( 0.16, 0.88, "#scale[0.8]{#font[42]{#bf{"+this->Trigger+"}}}" );
    // latex.DrawLatexNDC( 0.16, 0.87, "#scale[0.6]{#font[42]{#bf{"+this->Dataset+"}}}" );
    latex.DrawLatexNDC( 0.16, 0.83,"#scale[0.6]{#font[42]{#bf{"+strDataset+"  "+etaCategoryLong+"}}}");
    latex.DrawLatexNDC( 0.16, 0.79, "#scale[0.6]{#font[42]{#bf{"+EventLevelTag+"}}}" );

    gROOT->ProcessLine( "gErrorIgnoreLevel = 2001;");
    c->SaveAs(Dir+"/"+canvasName+".pdf","pdf");
    c->Close();
    gROOT->ProcessLine( "gErrorIgnoreLevel = kPrint;");

    delete h_Frame;
  }

  TString Get_HLT_Latex(TString _name )
  {
    if(_name.Contains("IsoMu24"))
      return "Isolated single #mu trigger with p_{T} > 24 GeV";
    else if(_name.Contains("Mu50"))
      return "Non-isolated single #mu trigger with p_{T} > 50 GeV";
    else if(_name.Contains("hltL2fL1sMu22L1f0L2Filtered10Q"))
      return "hltL2fL1sMu22L1f0L2Filtered10Q";
    else if(_name.Contains("hltL2fL1sSingleMu22L1f0L2Filtered10Q"))
      return "hltL2fL1sSingleMu22L1f0L2Filtered10Q";
    else if(_name.Contains("hltL1fL1sMu22L1Filtered0"))
      return "hltL1fL1sMu22L1Filtered0";
    else
      return "Trigger name not found!!";
  }

  TString Get_Selection_Latex( TString _sel )
  {
    TString SelLong="";
    TString OffId  = "Not defined";
    TString OffIso = "Not defined";

    TString sel = _sel;
    TObjArray *split  = sel.Tokenize("_");
    TString Id  = ((TObjString*)(split->At(1)))->String();
    TString Iso = ((TObjString*)(split->At(2)))->String();

    //-- Id
    if( Id == "IdNo" )                    OffId = "no ID";
    if( Id == "IdPfOrTrkOrGlb" )          OffId = "isPF || isTrk || isGlb";
    if( Id == "IdTrk" )                   OffId = "isTrk";
    if( Id == "IdGlb" )                   OffId = "isGlb";
    if( Id == "IdPf" )                    OffId = "isPF";
    if( Id == "IdLoose" )                 OffId = "Loose ID";
    if( Id == "IdTight" )                 OffId = "Tight ID";
    if( Id == "IdHighPt" )                OffId = "High-pT ID";
    if( Id == "IdTrkAndTrkQual" )         OffId = "isTrk + TrkQual";
    if( Id == "IdGlbAndTrkQual" )         OffId = "isGlb + TrkQual";
    if( Id == "IdGlbAndTrkQualPlus1MS" )  OffId = "isGlb + TrkQual + #MS>0";
    if( Id == "IdGlbAndTrkQualPlus2MS" )  OffId = "isGlb + TrkQual + #MS>1";
    if( Id == "IdGlbAndMuonQual" )        OffId = "isGlb + MuonQual";
    if( Id == "IdTrkPlus1MS" )            OffId = "isTrk + #MS>0";

    //-- Iso
    if( Iso == "IsoNo" )       OffIso = "no Iso";
    if( Iso == "IsoPFLoose" )  OffIso = "rel.PF.Iso<0.25";
    if( Iso == "IsoPFTight" )  OffIso = "rel.PF.Iso<0.15";
    if( Iso == "IsoTkLoose" )  OffIso = "rel.Tk.Iso<0.1";
    if( Iso == "IsoTkTight" )  OffIso = "rel.Tk.Iso<0.05";

    if( Id == "IdNo" && Iso == "IsoNo" )
      return "any offline muon";

    if( Id != "IdNo" && Iso == "IsoNo" )
      return OffId;

    if( Id == "IdNo" && Iso != "IsoNo" )
      return OffIso;

    SelLong = OffId+" + "+OffIso;
    return SelLong;
  }

  Double_t CalcEffciencyError( Double_t nPass, Double_t ePass, Double_t nFail, Double_t eFail )
  {
    Double_t nTota = nPass+nFail;
    Double_t Err = 1./(nTota*nTota) * sqrt( nPass*nPass* eFail*eFail + nFail*nFail * ePass*ePass );
    return Err;
  }

  pair<Double_t, Double_t> GetClopperPearsonErrorLU( Double_t nPass, Double_t nTota, Double_t alpha = 1-0.6827, bool equal_tailed = true ) {
    Double_t alpha_min = -999.;
    if( equal_tailed )
      alpha_min = alpha/2;
    else
      alpha_min = alpha;

    Double_t Eff = 0;
    Double_t lower = 0;
    Double_t upper = 1;
    if( nPass > 0 ) {
      Eff = ( nPass / nTota );
      lower = ROOT::Math::beta_quantile(alpha_min, nPass, nTota - nPass + 1);
    }
    if( nTota - nPass > 0 ) {
      Eff = ( nPass / nTota );
      upper = ROOT::Math::beta_quantile_c(alpha_min, nPass + 1, nTota - nPass);
    }
    else if( nTota == 0 ) {
      Eff = 0;
      lower = 0;
      upper = 0;
    }

    return make_pair(Eff-lower, upper-Eff);
  }
};


void Draw_Purity_Paper() {

  Bool_t IsPrescaled = kFALSE;

  vector<TString> vec_FileList =
  {
    "Output-MuonTriggerPurity-Data2016H-v23-HLT_IsoMu24_v.root",
    "Output-MuonTriggerPurity-Data2016H-v23-HLT_Mu50_v.root",
    "Output-MuonTriggerPurity-Data2016H-v23-hltL1fL1sMu22L1Filtered0.root",
    "Output-MuonTriggerPurity-Data2016H-v23-hltL2fL1sMu22L1f0L2Filtered10Q.root",

    // "Output-MuonTriggerPurity-Data2018D-v23-HLT_IsoMu24_v.root",
    // "Output-MuonTriggerPurity-Data2018D-v23-HLT_Mu50_v.root",
    // "Output-MuonTriggerPurity-Data2018D-v23-hltL1fL1sMu22L1Filtered0.root",
    // "Output-MuonTriggerPurity-Data2018D-v23-hltL2fL1sSingleMu22L1f0L2Filtered10Q.root"
  };

  vector<TString> vec_RunTag =
  {
    "Run0to999999"
  };

  for(Int_t i=0; i<vec_FileList.size(); ++i) {
    DrawingTool *tool = new DrawingTool(vec_FileList[i], vec_RunTag[0], IsPrescaled);
    // if(i==0 || i==1 || i==8 || i==9 )
    tool->DrawPurity_All();
    // tool->DrawPuritySeq_All();
    delete tool;
  }
}
