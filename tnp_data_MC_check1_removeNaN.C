#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "TH1F.h"
#include "TFile.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TTree.h"
#include "TMath.h"

//#include "tdrstyle.C"

using namespace std;
//setTDRStyle();

// TString whichPhoton = "lead"

void comparison(TString observable = "leadPt", int nBins = 80, double observableMin = 20.0, double observableMax = 400.0, bool log=false, bool ratio=false, bool drawsignal=false, bool axisrange=false, double xmin=0, double xmax=10, bool label=false )
{
  //gROOT->ProcessLine(".L tdrstyle.C");
  //setTDRStyle();
  gStyle->SetOptStat(0);

  // Data file
  // TString inFileData("/eos/user/k/kmondal/public/FLASHgg/ZeeValidation/April2018_v1/ReReco2017/ZeeTnP_wcor/output_SingleElectron_Rereco2017.root");
  // TString inFileData("/eos/user/k/kmondal/public/FLASHgg/ZeeValidation/April2018_v1/ReReco2017/ZeeTnP_wcor/output_SingleElectron_EOY2017B.root"); // 2017B
  // TString inFileData("/eos/user/k/kmondal/public/FLASHgg/ZeeValidation/April2018_v1/ReReco2017/ZeeTnP_wcor/output_SingleElectron_EOY2017C.root"); // 2017C
  // TString inFileData("/eos/user/k/kmondal/public/FLASHgg/ZeeValidation/April2018_v1/ReReco2017/ZeeTnP_wcor/output_SingleElectron_EOY2017D.root"); // 2017D
  // TString inFileData("/eos/user/k/kmondal/public/FLASHgg/ZeeValidation/April2018_v1/ReReco2017/ZeeTnP_wcor/output_SingleElectron_EOY2017E.root"); // 2017E
  TString inFileData("/eos/user/k/kmondal/public/FLASHgg/ZeeValidation/April2018_v1/ReReco2017/ZeeTnP_wcor/output_SingleElectron_EOY2017F.root"); // 2017F

  // MC file
  TString inFileMC_normal("/eos/user/k/kmondal/public/FLASHgg/ZeeValidation/April2018_v1/ReReco2017/ZeeTnP_wcor/output_DYJetsToLL_Rereco2017_wcorr.root"); // wcorr
  // TString inFileMC_normal("/eos/user/k/kmondal/public/FLASHgg/ZeeValidation/April2018_v1/ReReco2017/ZeeTnP_wocor/output_DYJetsToLL_Rereco2017_wocor.root"); // wocorr

  TString cut = "totWeight";
  // For EB
  // TString cutMC = "totWeight*(fabs(probe_sc_eta) < 1.4442 && probe_Pho_sieie < 0.0105 && probe_Pho_chIso < 20 && (probe_Pho_chIso/probe_Pho_pt) < 0.3 && mNPV > 1.0)";
  // TString cutData = "(fabs(probe_sc_eta) < 1.4442 && probe_Pho_sieie < 0.0105 && probe_Pho_chIso < 20 && (probe_Pho_chIso/probe_Pho_pt) < 0.3)";
  // For EE
  TString cutMC = "totWeight*(fabs(probe_sc_eta) > 1.566 && probe_Pho_sieie < 0.028 && probe_Pho_chIso < 20 && (probe_Pho_chIso/probe_Pho_pt) < 0.3 && mNPV > 1.0)";
  TString cutData = "(fabs(probe_sc_eta) > 1.566 && probe_Pho_sieie < 0.028 && probe_Pho_chIso < 20 && (probe_Pho_chIso/probe_Pho_pt) < 0.3)";

  cout << "cutData:  " << cutData << endl;
  cout << "cutMC: " << cutMC << endl;

  cout.precision(3);

  TFile* f1 = TFile::Open(inFileData); // Data
  if(f1 == 0) return;
  if(f1->IsZombie()){f1->Close(); return;}
  TH1F *data  = new TH1F("data","data",nBins,observableMin,observableMax);
  ((TTree*)f1->Get("PhotonToRECO/fitter_tree"))->Project("data",observable,cutData);
  cout << "Mean of data  :" << data->GetMean() << endl;
  cout << "Integral of data: " << data->Integral() << endl;

  
  TFile* f2 = TFile::Open(inFileMC_normal); // inFileMC_normal
  if(f2 == 0) return;
  if(f2->IsZombie()){f2->Close(); return;}
  ((TTree*)f2->Get("PhotonToRECO/fitter_tree"))->Draw(">>event_list","!TMath::IsNaN(totWeight)");
  ((TTree*)f2->Get("PhotonToRECO/fitter_tree"))->SetEventList((TEventList*) gDirectory->Get("event_list"));

  TH1F *MC_normal  = new TH1F("MC_normal","MC_normal",nBins,observableMin,observableMax);
  ((TTree*)f2->Get("PhotonToRECO/fitter_tree"))->Project("MC_normal",observable,cutMC);
  cout << "Mean of MC_normal :" << MC_normal->GetMean() << endl;
  cout << "Integral of MC_normal: " << MC_normal->Integral() << endl;

  data->SetLineColor(kBlack);
  data->SetLineWidth(3);
  data->SetMarkerStyle(20); 
  data->SetMarkerSize(0.7);

  // customize for MC_normal
  MC_normal->SetLineColor(kBlue);
  MC_normal->SetLineWidth(3);
  //MC_normal->SetFillColor(kBlue);

  data->GetXaxis()->SetTitle("GeV");

  TCanvas c3("c3","c3",600,600);
  //  C3.SetLogy(log);

  const float xpad[2] = {0.,1};
  const float ypad[4] = {0.,0.3,0.3,1.0};
  if(ratio){
    c3.Divide(1,2); 
    c3.cd(1);
    gPad->SetPad(xpad[0],ypad[2],xpad[1],ypad[3]);
    gPad->SetLogy(log);
    gPad->SetGridy();
    gPad->SetGridx();
  }                   
  
  //  C3.cd()->SetRightMargin(0.04);
  //  C3.SetLeftMargin(1);
  //  C3.SetRightMargin(0.2);  
  
  MC_normal->Scale(data->Integral()/MC_normal->Integral());

  cout << "new integral" << endl;
  cout << "MC: " << MC_normal->Integral() << endl;
  cout << "data: " << data->Integral() << endl;
  
  MC_normal->Draw("HIST");
  data->Draw("PESAME");
  
  

  TLegend * leg = new TLegend(0.25,0.70,0.45,0.90);
  leg->SetTextSize(0.05);
  leg->SetBorderSize(0);
  leg->SetLineStyle(0);
  leg->SetFillStyle(0);
  leg->SetFillColor(0);

  leg->AddEntry(data,"data","PE");
  leg->AddEntry(MC_normal,"MC_wcor","L");

  leg->Draw();

  c3.Update();

  if (ratio){
    c3.cd(2);
    gPad->SetTopMargin(0);
    gPad->SetBottomMargin(0.3);
    gPad->SetGridy();
    gPad->SetGridx();
    gPad->SetPad(xpad[0],ypad[0],xpad[1],ypad[2]);

    TH1F *hRatio1 = (TH1F*)data->Clone("hRatio1");
    hRatio1->Reset();
    hRatio1->Add(data);

    TH1F *hRatio2 = (TH1F*)data->Clone("hRatio2");
    hRatio2->Reset();
    hRatio2->Add(data);

    TH1F *hRatio3 = (TH1F*)data->Clone("hRatio3");
    hRatio3->Reset();
    hRatio3->Add(data);

    if (data->Integral() > 0){
      hRatio1->Divide(MC_normal);
    }
    hRatio1->GetYaxis()->SetTitle("#frac{data}{MC}");
    hRatio1->SetLineColor(kRed);
    hRatio1->GetYaxis()->SetRangeUser(0.0, 2.0);
    hRatio1->GetYaxis()->SetLabelSize(0.06);
    hRatio1->GetYaxis()->SetTitleSize(0.1);
    hRatio1->GetYaxis()->SetTitleOffset(0.45);
    hRatio1->GetYaxis()->SetTitle("Data/MC");
    hRatio1->GetXaxis()->SetLabelSize(0.1);
    hRatio1->GetXaxis()->SetTitleSize(0.1);
    hRatio1->GetXaxis()->SetTitleOffset(1.2);


    hRatio1->Draw("L");
    c3.Update();
  }
  
  
  // TString outFile("");
  // outFile += observable;
  // outFile += "_comp.pdf";                                                                                              
  // outFile += "comp.png";
  // observable += "_19Mar2018_";
  // observable += category;
  // if(log) observable += "_log";
  c3.SaveAs(observable+"_2017F_EE_wcor.root");
  c3.SaveAs(observable+"_2017F_EE_wcor.png");

  /*

  tex->SetLineWidth(2);
  tex->Draw();
  */

  //C3.SaveAs(mode+observable+"_Stack.png");

  
}

void compareAll(){
  comparison("probe_Pho_r9",100,0.8,1.0,false,true);
  comparison("probe_Pho_s4",70,0.3,1.0,false,true);
  comparison("probe_Pho_etawidth",100,0.,0.05,false,true);
  comparison("probe_Pho_sieie",100,0.018,0.028,false,true);
  comparison("probe_Pho_mva",100,-1.0,1.0,false,true);
  comparison("mNPV",100,0.,100.,false,true);
  comparison("probe_Pho_phoIso03",100,0.,10.,true,true);
  comparison("probe_Pho_chIso",100,0.,10.,true,true);
  comparison("probe_Pho_chIsoWv",100,0.,10.,true,true);
}
