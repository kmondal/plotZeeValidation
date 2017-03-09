#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "TH1F.h"
#include "TFile.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TLegend.h"

//#include "tdrstyle.C"

using namespace std;
//setTDRStyle();

void comparison(TString category = "eb", TString observable = "leadPt", int nBins = 80, double observableMin = 20.0, double observableMax = 400.0, bool log=false, bool drawdata=true, bool ratio=false, bool drawsignal=false, bool axisrange=false, double xmin=0, double xmax=10, bool label=false )
{
  //  gROOT->ProcessLine(".L tdrstyle.C");
  //setTDRStyle();
  gStyle->SetOptStat(0);

  //  TString inFile("root://eoscms.cern.ch//store/user/gkole/Hgg/VH/METoptimization/MET45/");
  TString inFileData("/hadoop/cms/store/user/gkole/Hgg/Kuntal/Moriond17_AfterPreApproval/Zee_wocorr/output_data_SingleEle.root");
  TString inFileMC("/hadoop/cms/store/user/gkole/Hgg/Kuntal/Moriond17_AfterPreApproval/Zee_wocorr/output_DYJetsToLL.root");
  /*
  c1 = new TCanvas();
  const float xpad[2] = {0.,1};
  const float ypad[4] = {0.,0.3,0.3,1.0};
  if(ratio){
    c1->Divide(1,2);
    c1->cd(1);
    gPad->SetPad(xpad[0],ypad[2],xpad[1],ypad[3]);
    gPad->SetLogy(log);
  }
  */
  //double lumi = 12.89; // 12.89 inv /fb for ICHEP
  
  
  /*
  TString observable = "leadfull5x5r9"; // subleadfull5x5r9
  int nBins = 100;
  double observableMin = 0.8;
  double observableMax = 1.0;
  */



  /*
  //+++++++++++++++++
  TString observable = "subleadPt";
  int nBins = 34;
  double observableMin = 20.0;
  double observableMax = 150.0;
  //+++++++++++++++++
  */


  //  TString mode = "VHHadronicTag";

  //  cout << "Modes: " << mode << endl;

  TString cut = "weight";
  //  TString cut = "weight*((890)*leptonSF + (4428)*leptonSFB +(495.003+5109+1288)*leptonSFC)*bWeight*PUWeight"; // template
  TString cutMC = "weight*(mass > 70 && mass < 110 && subIDMVA >-0.9 && leadIDMVA > -0.9)";
  TString cutData = "(mass > 70 && mass < 110 && subIDMVA >-0.9 && leadIDMVA > -0.9)";

  TString cutMCeb = "weight*(mass > 70 && mass < 110 && subIDMVA >-0.9 && leadIDMVA > -0.9 && abs(leadEta)<1.479)";
  TString cutMCee = "weight*(mass > 70 && mass < 110 && subIDMVA >-0.9 && leadIDMVA > -0.9 && abs(leadEta)>1.556&&abs(leadEta)<2.5)";

  TString cutDataeb = "(mass > 70 && mass < 110 && subIDMVA >-0.9 && leadIDMVA > -0.9 && abs(leadEta)<1.479)";
  TString cutDataee = "(mass > 70 && mass < 110 && subIDMVA >-0.9 && leadIDMVA > -0.9 && abs(leadEta)>1.556&&abs(leadEta)<2.5)";
  

  TString finalCutMC = cut;
  TString finalCutData = cut;

  if (category.Contains("ee")){
    finalCutMC = cutMCee;
    finalCutData = cutDataee;
  }

  cout << "category: " << category << endl;
  cout << "finalCutMC: " << finalCutMC << endl;
  cout << "finalCutData: " << finalCutData << endl;

  cout.precision(3);

  TFile* f1 = TFile::Open(inFileMC); // MC DYToEE
  if(f1 == 0) return;
  if(f1->IsZombie()){f1->Close(); return;}

  //leadfull5x5r9

  TH1F *DYToEE  = new TH1F("DYToEE","DYToEE",nBins,observableMin,observableMax);
  ((TTree*)f1->Get("DiPhotonWithZeeMVADumper/trees/zeevalidation_13TeV_All"))->Project("DYToEE",observable,finalCutMC);
  cout << "Mean of DYToEE  :" << DYToEE->GetMean() << endl;

  
  TFile* f2 = TFile::Open(inFileData); // Data SingleElectron
  if(f2 == 0) return;
  if(f2->IsZombie()){f2->Close(); return;}
  
  TH1F *SingleEle  = new TH1F("SingleEle","SingleEle",nBins,observableMin,observableMax);
  ((TTree*)f2->Get("DiPhotonWithZeeMVADumper/trees/zeevalidation_13TeV_All"))->Project("SingleEle",observable,finalCutData);
  cout << "Mean of SingleEle :" << SingleEle->GetMean() << endl;
  
  //  DYToEE->Draw();


  // Drawing
  // customize for MC
  DYToEE->SetFillColor(kBlue-7);

  // customize for data
  SingleEle->SetLineColor(kBlack);
  SingleEle->SetMarkerStyle(20);
  SingleEle->SetMarkerSize(0.7);

  TCanvas C3("c3","c3");
  C3.SetGridx(1);
  C3.SetGridy(1);
  //  C3.cd()->SetRightMargin(0.04);
  //  C3.SetLeftMargin(1);
  //  C3.SetRightMargin(0.2);  
  
  DYToEE->Draw("HIST");
  SingleEle->Draw("PESAME");

  //  TString outFile("");
  //outFile += observable;
  //outFile += "_comp.pdf";                                                                                              
  //outFile += "comp.png";
  C3.SaveAs(observable+"_"+category+"_comp.png");




  /*

  //ggh_125_13TeV_WHLeptonicTag->Draw("weight");
  TH1F *gjet  = new TH1F("gjet","gjet",nBins,observableMin,observableMax);
  ((TTree*)f1->Get("tagsDumper/trees/gjet_13TeV_"+mode))->Project("gjet",observable,cut);
  gjet->Scale(lumi);
  gjet_WHLep = gjet->Integral()*gjet->GetMean()*lumi;
  //cout << "mean: " << gjet->GetMean() << endl;
  cout << "gjet_WHLep Yeild: " << gjet->Integral()*gjet->GetMean()*lumi << endl;
  //  gjet->Draw();

  // DY
  TFile* f2 = TFile::Open(inFile+"output_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8.root");
  if(f2 == 0) return;
  if(f2->IsZombie()){f2->Close(); return;}

  //ggh_125_13TeV_WHLeptonicTag->Draw("weight");
  TH1F *DYjet  = new TH1F("DYjet","DYjet",nBins,observableMin,observableMax);
  ((TTree*)f2->Get("tagsDumper/trees/DYjet_13TeV_"+mode))->Project("DYjet",observable,cut);
  DYjet->Scale(lumi);
  DYjet_WHLep = DYjet->Integral()*DYjet->GetMean()*lumi;
  cout << "mean: " <<DYjet->GetMean() << endl;
  cout << "DYjet_WHLep Yeild: " << DYjet_WHLep << endl;
  //  DYjet->Draw();


  // DiPhoton
  TFile* f3 = TFile::Open(inFile+"output_DiPhotonJetsBox_MGG-80toInf_13TeV-Sherpa.root");
  if(f3 == 0) return;
  if(f3->IsZombie()){f3->Close(); return;}

  //ggh_125_13TeV_WHLeptonicTag->Draw("weight");
  TH1F *dipho  = new TH1F("dipho","dipho",nBins,observableMin,observableMax);
  ((TTree*)f3->Get("tagsDumper/trees/dipho_13TeV_"+mode))->Project("dipho",observable,cut);
  dipho_WHLep = dipho->Integral()*dipho->GetMean()*lumi;
  cout << "mean: " <<dipho->GetMean() << endl;
  cout << "dipho_WHLep Yeild: " << dipho_WHLep << endl;
  dipho->Scale(lumi);
  //dipho->Draw();

  // QCD
  TFile* f4 = TFile::Open(inFile+"qcd.root");
  if(f4 == 0) return;
  if(f4->IsZombie()){f4->Close(); return;}

  //ggh_125_13TeV_WHLeptonicTag->Draw("weight");
  TH1F *qcd  = new TH1F("qcd","qcd",nBins,observableMin,observableMax);
  ((TTree*)f4->Get("tagsDumper/trees/qcd_13TeV_"+mode))->Project("qcd",observable,cut);
  qcd->Scale(lumi);
  qcd_WHLep = qcd->Integral()*qcd->GetMean()*lumi;
  //cout << "mean: " << gjet->GetMean() << endl;
  cout << "qcd_WHLep yeild:" << qcd_WHLep << endl;
  //qcd->Draw();

  TFile* f5 = TFile::Open(inFile+"VH_data_v3.root");
  if(f5 == 0) return;
  if(f5->IsZombie()){f5->Close(); return;}

  //ggh_125_13TeV_WHLeptonicTag->Draw("weight");
  TH1F *data  = new TH1F("data","data",nBins,observableMin,observableMax);
  //  ((TTree*)f5->Get("tagsDumper/trees/Data_13TeV_"+mode))->Project("data",observable,cut);

  // temp
  ((TTree*)f3->Get("tagsDumper/trees/dipho_13TeV_"+mode))->Project("data",observable,cut);

  //qcd_WHLep = qcd->Integral()*qcd->GetMean()*lumi;
  //cout << "mean: " << gjet->GetMean() << endl;
  //cout << "qcd_WHLep yeild:" << qcd_WHLep << endl;
  //data->Draw();
  

  TH1F *datapu  = new TH1F("datapu","datapu",nBins,observableMin,observableMax);
  //  ((TTree*)f5->Get("tagsDumper/trees/Data_13TeV_"+mode))->Project("datapu","puweight",cut);
  ((TTree*)f3->Get("tagsDumper/trees/dipho_13TeV_"+mode))->Project("datapu","puweight",cut);

  datapu->SetLineColor(kRed);
  if (datapu->Integral() > 0)
    data->Divide(datapu);

  data->Draw("HIST");


  */




  /*

  THStack hs1("hs1","VH distribution");
  data->SetLineColor(kBlack);
  //QCD
  qcd->SetFillColor(kGray);
  qcd->SetLineColor(kBlack);
  hs1.Add(qcd);
  // DYjet
  DYjet->SetFillColor(kGreen-2);
  DYjet->SetLineColor(kBlack);
  hs1.Add(DYjet);
  // gjet
  gjet->SetFillColor(kBlue-1);
  gjet->SetLineColor(kBlack);
  hs1.Add(gjet);
  // Diphotn
  dipho->SetFillColor(kOrange-3);
  dipho->SetLineColor(kBlack);
  hs1.Add(dipho);

  TCanvas C3("c3","c3");
  C3.cd()->SetRightMargin(0.04);
  C3.SetLeftMargin(1);
  C3.SetRightMargin(0.2);

  cout << "data Integral: " << data->Integral() << endl;

  
  

  if(drawdata) data->Draw("E1");
  hs1.Draw("hist");
  hs1.Draw("axissame");
  hs1.SetTitle(observable);
  if(drawdata) data->SetTitle(observable);
  if(drawdata)   data->SetMarkerStyle(20);
  hs1.GetXaxis()->SetTitle(observable);
  hs1.GetXaxis()->SetNdivisions(5, "X");
  if(drawdata)   data->GetXaxis()->SetTitle(observable);
  if(drawdata)   data->GetYaxis()->SetTitleFont(42);
  if(drawdata)   data->GetYaxis()->SetTitleOffset(1.);
  if(drawdata)   data->GetXaxis()->SetTitleFont(42);
  if(drawdata)   data-> GetYaxis()->SetRangeUser(0.,1.1*max(data->GetMaximum(),hs1.GetMaximum()));
  if(drawdata) data->Draw("E1 same");
  if(drawdata) data->Draw("axissame");
  cout << "test1"<<endl;

  TLegend * leg = new TLegend(0.81,0.27,0.93,0.90);
  leg = new TLegend(0.81,0.27,0.93,0.90);
  leg->SetTextSize(0.037);
  leg->SetBorderSize(0);
  leg->SetLineStyle(0);
  leg->SetTextSize(0.027);
  leg->SetFillStyle(0);
  leg->SetFillColor(0);

  if(drawdata)   leg->AddEntry(data,"data","pl");
  leg->AddEntry(qcd,"QCD","f");
  leg->AddEntry(DYjet,"DY","f");
  leg->AddEntry(gjet,"GJet","f");
  leg->AddEntry(dipho,"DiPhoton","f");
  
  leg->Draw();

  // header
  TString stringCharge=TString("");
  TLatex * tex = new TLatex(0.11, 0.99,"CMS Preliminary    12.9 fb^{-1} (13 TeV)");

  tex->SetNDC();
  tex->SetTextAlign(13);
  tex->SetTextFont(42);
  tex->SetTextSize(0.045);
  tex->SetLineWidth(2);
  tex->Draw();
  */

  //C3.SaveAs(mode+observable+"_Stack.png");

  
}

void compareAll(){

//
//  comparison("eb","leadfull5x5r9",100,0.8,1.0);
//  comparison("ee","leadfull5x5r9",100,0.8,1.0);
  comparison("eb","dipho_mva",100,-1.0,1.0);
  //  comparison("ee","leadIDMVA",100,-1.0,1.0);
    

//  comparison("VBFTag_1","mass",40,100.0,200.0);
//  comparison("TTHHadronicTag","mass",40,100.0,200.0);
//  comparison("TTHLeptonicTag","mass",40,100.0,200.0);
}
