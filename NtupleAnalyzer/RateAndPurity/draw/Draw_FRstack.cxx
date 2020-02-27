#include "PlotTools.h"

//-- Setting Purity Seq --//

vector<TString> vec_Stack = {
  "Mat1_IdNo_IsoNo_SimGenHard_HLT",
  "Mat1_IdNo_IsoNo_SimGen_HLT"
};

vector<Color_t> vec_Color_Seq = {
  kRed-7,
  kYellow-7,

  // kBlack,
  kViolet+4,
  kRed+4,
  kGreen+2,
  kMagenta,
  // kViolet,
  kCyan+2,
  kMagenta+3
};

vector<Int_t>   vec_Marker_Seq = {
  20,
  // 21,
  22,
  23,
  24,
  25,
  26,
  27,
  30,
  32
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
    Output-MuonTriggerPurity-TTSemiLep102X-v25-hltL2fL1sSingleMu22L1f0L2Filtered10Q-0.0-5.0.root
    */

    TObjArray *split  = (this->FileName).Tokenize("-");
    TString _Dataset = ((TObjString*)(split->At(2)))->String();
    TString _Version = ((TObjString*)(split->At(3)))->String();
    TString _Trigger = ((TObjString*)(split->At(4)))->String();
    TString _EtaLo = ((TObjString*)(split->At(5)))->String();
    TString _EtaUp = ((TObjString*)(split->At(6)))->String();
            _EtaUp = _EtaUp.Remove(3,5);
    TString _EtaBin = _EtaLo+"_"+_EtaUp;

    this->Dataset = _Dataset;
    this->Version = _Version;
    this->RunTag = _RunTag;
    this->Trigger = _Trigger;
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

  void DrawPurityStack_All()
  {
      this->DrawPurityStack( "nVertices", "Mat0",      4, 65,   2e-2, 4000. );
      this->DrawPurityStack( "Pt",        "Mat0",     24, 1000, 2e-2, 4000. );
      this->DrawPurityStack( "Eta",       "Mat0",   -2.4, 2.4,  2e-2, 4000. );
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

  void DrawPurityStack(
    TString Var, TString DenTag,
    Double_t minX, Double_t maxX,
    Double_t minY, Double_t maxY
  ) {

    // -- Setting
      Bool_t isLogX = kFALSE;
      Bool_t isLogY = kFALSE;
      if(Var == "Pt") {
        isLogX = kTRUE;
        isLogY = kTRUE;
        if((this->Trigger).Contains("Mu50"))
          minX = 50.;
        else if((this->Trigger).Contains("hltL1fL1sMu"))
          minX = 22.;
        else if((this->Trigger).Contains("hltL2fL1s"))
          minX = 0.5;

        // HERE
        isLogX = kFALSE;
        minX = 3.;
        maxX = 200.;
      }
      if(!this->Dataset.Contains("DY")) {
        isLogY = kTRUE;
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
      if(this->Version.Contains("Dimuon"))
        strDataset = strDataset.ReplaceAll("DY ", "DY#rightarrow#mu#mu ");
      if(this->Dataset.Contains("TTSemiLep"))
        strDataset = strDataset.ReplaceAll("DY ", "t#bar{t} semi-lep ");

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

      if(this->Trigger.Contains("hltL1fL1sMu"))
        TitleX.ReplaceAll("L3","L1");
      if(this->Trigger.Contains("hltL2fL1s"))
        TitleX.ReplaceAll("L3","L2");

      vector<TString> vec_Seq = vec_Stack;
      const Int_t n_tag = (Int_t)vec_Seq.size();
      TH1D* Purity[n_tag];

    //--- Output file
      TString Dir = "./Plots_Stack_"+this->Version+"/FR/"+Dataset+"/"+this->Trigger;
      if (gSystem->mkdir(Dir,kTRUE) != -1)
        gSystem->mkdir(Dir,kTRUE);

    // -- Canvas -- //
    TCanvas *c;
    TString canvasName = "FR_DenL3_"+this->Dataset+"_"+this->Trigger+"_"+DenTag+"_"+Var;
    if( this->Version.Contains("Dimuon") )  canvasName = canvasName + "_DYMuMu";
    if( this->Dataset.Contains("TTSemiLep") )  canvasName = canvasName + "_TTSemiLep";
    SetCanvas_Square( c, canvasName, isLogX, isLogY );
    c->cd();

    TH1D *h_Frame = new TH1D("h_Frame", "", 1, minX, maxX);
    h_Frame->SetStats(kFALSE);
    h_Frame->SetLineWidth(0);
    h_Frame->SetLineColor(kBlack);
    h_Frame->SetMarkerSize(0);
    // h_Frame->SetFillColor( kAzure-9 );
    // h_Frame->SetBinContent(1, 100.);
    // h_Frame->SetBinError(1, 0.);

    double nDen = -1.;
    vector<double> count = {};

    for(Int_t i=0; i<n_tag; ++i) {

      TString RunTagTemp = this->RunTag;

      TString strSeq = vec_Seq[i];

      NumName_Purity = "h_L3_"+Var+"_"+RunTagTemp+"_"+strSeq+"_"+this->EtaBin;
      DenName_Purity = "h_L3_"+Var+"_"+RunTagTemp+"_"+DenTag+"_IdNo_IsoNo_SimNo_HLT"+"_"+this->EtaBin;
      if(strSeq.Contains("_MYHLT"))
        DenName_Purity = "h_L3_"+Var+"_"+DenTag+"_IdNo_IsoNo_SimNo_MYHLT";

      TString FileNameTemp = this->FileName;

      TH1D *h_Num = Get_Hist("./Outputs/"+FileNameTemp, NumName_Purity, "h_Num");
      TH1D *h_Den = Get_Hist("./Outputs/"+FileNameTemp, DenName_Purity, "h_Den");
      Double_t nNum = (Double_t)(h_Den->Integral()) - (Double_t)(h_Num->Integral());
      nDen = (Double_t)(h_Den->Integral());
      printf("\n\t[%s, %s, %s] (nNum, nDen, Purity) = (%.6f, %.6f, %.6f)\n",
                (strSeq).Data(), strSeq.Data(), Var.Data(), nNum, nDen, (nNum/nDen) );
      count.push_back(nNum);

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
        else {
          Pur = 1.;
          Err = 0.;
        }

        // cout << "\t\t[" << h_Num->GetBinLowEdge(iB) << ", " << h_Num->GetBinLowEdge(iB+1) << "]" << nPass << "/" << nTota << " = " << Pur << endl;

        Purity[i]->SetBinContent( iB, 1. - Pur );
        Purity[i]->SetBinError( iB, Err );
      }
      Purity[i]->SetLineWidth(1);
      Purity[i]->SetLineColor(kBlack);
      Purity[i]->SetMarkerSize(0);
      Purity[i]->SetMarkerStyle(0);
      Purity[i]->SetMarkerColor(vec_Color_Seq[i]);
      Purity[i]->SetFillColor(vec_Color_Seq[i]);
      Purity[i]->Scale(100.);
    }

    TH1D *Prompt = (TH1D*)Purity[0]->Clone("Prompt");
    Prompt->SetFillColor( kAzure-9 );
    for(int ip = 0; ip <= Prompt->GetNbinsX()+1; ++ip) {
      if(Prompt->GetBinContent(ip) != 0.) {
        Prompt->SetBinContent(ip, 100.);
        Prompt->SetBinError(ip, 0.);
      }
    }

    // -- Draw -- //
    h_Frame->GetYaxis()->SetRangeUser(minY, maxY);
    SetAxis_SinglePad( h_Frame->GetXaxis(), h_Frame->GetYaxis(), TitleX, "Contamination [\%]" );
    h_Frame->Draw();
    Prompt->Draw("HIST SAME");
    for(Int_t i=0; i<n_tag; ++i) {
      Purity[i]->Draw("HIST SAME");
    }

    TLegend *legend;
    SetLegend( legend, 0.15, 0.72, 0.90, 0.86);
    legend->AddEntry( Prompt, "#bf{Prompt muon"+TString::Format(" (%.1f%%)}", 100.*(nDen - count[0])/nDen), "f" );
    legend->AddEntry( Purity[0], "#bf{Non-prompt muon"+TString::Format(" (%.1f%%)}", 100.*(count[0] - count[1])/nDen), "f" );
    legend->AddEntry( Purity[1], "#bf{Non-muon"+TString::Format(" (%.1f%%)}", 100.*(count[1])/nDen), "f" );
    legend->Draw();

    TLatex latex;
    Latex_Simulation( latex );
    // Latex_Preliminary_NoDataInfo( latex );
    latex.DrawLatexNDC( 0.82, 0.96, "#scale[0.8]{#font[42]{13 TeV}}" );
    latex.DrawLatexNDC( 0.17, 0.88, "#scale[0.8]{#font[42]{"+Get_HLT_Latex(this->Trigger)+"}}" );
    latex.DrawLatexNDC( 0.45, 0.96,"#scale[0.8]{#font[42]{"+strDataset+"}}");

    c->Modified();
    c->Update();
    c->RedrawAxis();

    gROOT->ProcessLine( "gErrorIgnoreLevel = 2001;");
    c->SaveAs(Dir+"/"+canvasName+".pdf","pdf");
    c->Close();
    gROOT->ProcessLine( "gErrorIgnoreLevel = kPrint;");

    delete h_Frame;
  }

  TString Get_HLT_Latex(TString _name )
  {
    TString out = "";
    if(_name.Contains("IsoMu24"))
      out = "Isolated single #mu trigger with p_{T} > 24 GeV";
    else if(_name.Contains("IsoTkMu24"))
      out = "Isolated single #mu trigger with p_{T} > 24 GeV";
    else if(_name.Contains("Mu24"))
      out = "Non-isolated single #mu trigger with p_{T} > 24 GeV";
    else if(_name.Contains("Mu50"))
      out = "Non-isolated single #mu trigger with p_{T} > 50 GeV";
    else if(_name.Contains("hltL1fL1sMu"))
      out = "L1, p_{T} > 22 GeV";
    else if(_name.Contains("hltL2fL1s"))
      out = "L2, p_{T} > 10 (0) GeV for 2016 (2018)";
    else
      out = "Trigger name not found!!";

    if(_name.Contains("Tk"))
      out = "(Tk) " + out;

    return out;
  }

  TString Get_Selection_Latex( TString _sel )
  {
    TString SelLong="";
    TString OffId  = "Not defined";
    TString OffIso = "Not defined";
    TString OffSim = "Not defined";

    TString sel = _sel;
    TObjArray *split  = sel.Tokenize("_");
    TString Id  = ((TObjString*)(split->At(1)))->String();
    TString Iso = ((TObjString*)(split->At(2)))->String();
    TString Sim = ((TObjString*)(split->At(3)))->String();

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

    //-- Sim
    if( Sim == "SimNo" )             OffSim = "no Sim ";
    if( Sim == "SimGen" )            OffSim = "non-muon";
    if( Sim == "SimGenHard" )        OffSim = "non-prompt muon and non-muon";
    // if( Sim == "SimGen" )            OffSim = "Gen: prompt and non-prompt muons";
    // if( Sim == "SimGenHard" )        OffSim = "Gen: prompt muons";
    if( Sim == "SimPromptHad" )   OffSim = "Sim: prompt and non-prompt muons";
    if( Sim == "SimPromptGauge" ) OffSim = "Sim: prompt muons";
    if( Sim == "SimPromptHadGho" )   OffSim = "Sim (|| ghost): prompt and non-prompt muons";
    if( Sim == "SimPromptGaugeGho" ) OffSim = "Sim (|| ghost): prompt muons";
    // if( Sim == "SimGen" )            OffSim = "Prompt and decay-in-flight (gen matching)";
    // if( Sim == "SimGenHard" )        OffSim = "Prompt muon only (gen matching)";
    // if( Sim == "SimPromptHadGho" )   OffSim = "Prompt and decay-in-flight (sim matching)";
    // if( Sim == "SimPromptGaugeGho" ) OffSim = "Prompt muon only (sim matching)";

    if( Id == "IdNo" && Iso == "IsoNo" ) {
      if( Sim != "SimNo"){
        return OffSim;
      }
      else{
        // return "no offline match";
        return "Any offline match";
      }
    }

    if( Id != "IdNo" && Iso == "IsoNo" )
      return OffId;

    if( Id == "IdNo" && Iso != "IsoNo" )
      return OffIso;

    SelLong = OffId+", "+OffIso+", "+OffSim;
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


void Draw_FRstack() {

  Bool_t IsPrescaled = kFALSE;

  vector<TString> vec_FileList =
  {
    "Output-MuonTriggerPurity-TTSemiLep102X-v25-HLT_IsoMu24_v-0.0-5.0.root",
    "Output-MuonTriggerPurity-TTSemiLep102X-v25-HLT_Mu24_v-0.0-5.0.root",
    "Output-MuonTriggerPurity-TTSemiLep102X-v25-HLT_Mu50_v-0.0-5.0.root",
    "Output-MuonTriggerPurity-TTSemiLep102X-v25-hltL1fL1sMu22L1Filtered0-0.0-5.0.root",
    "Output-MuonTriggerPurity-TTSemiLep102X-v25-hltL2fL1sSingleMu22L1f0L2Filtered10Q-0.0-5.0.root",

    "Output-MuonTriggerPurity-TTSemiLep80X-v25-HLT_IsoMu24_v-0.0-5.0.root",
    "Output-MuonTriggerPurity-TTSemiLep80X-v25-HLT_IsoTkMu24_v-0.0-5.0.root",
    "Output-MuonTriggerPurity-TTSemiLep80X-v25-HLT_Mu24_v-0.0-5.0.root",
    "Output-MuonTriggerPurity-TTSemiLep80X-v25-HLT_Mu50_v-0.0-5.0.root",
    "Output-MuonTriggerPurity-TTSemiLep80X-v25-HLT_TkMu24_v-0.0-5.0.root",
    "Output-MuonTriggerPurity-TTSemiLep80X-v25-HLT_TkMu50_v-0.0-5.0.root",
    "Output-MuonTriggerPurity-TTSemiLep80X-v25-hltL1fL1sMu22L1Filtered0-0.0-5.0.root",
    "Output-MuonTriggerPurity-TTSemiLep80X-v25-hltL2fL1sMu22L1f0L2Filtered10Q-0.0-5.0.root"
  };

  vector<TString> vec_RunTag =
  {
    "Run1to1"
  };

  for(Int_t i=0; i<vec_FileList.size(); ++i) {
    DrawingTool *tool = new DrawingTool(vec_FileList[i], vec_RunTag[0], IsPrescaled);
    tool->DrawPurityStack_All();
    delete tool;
  }
}
