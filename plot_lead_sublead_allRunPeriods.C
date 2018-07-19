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

//#include "tdrstyle.C"

using namespace std;
//setTDRStyle();

// TString whichPhoton = "lead"

void comparison(TString dataRootFile = "singleEle_2017B.root", TString mcRootFile = "zee_DYToLL_v1.root", TString category = "eb", TString observable1 = "leadfull5x5r9", TString observable2 = "subleadfull5x5r9", TString xtitle = "GeV", int nBins = 80, double observableMin = 20.0, double observableMax = 400.0, bool log=false, bool ratio=false, bool drawsignal=false, bool axisrange=false, double xmin=0, double xmax=10, bool label=false )
{
  //  gROOT->ProcessLine(".L tdrstyle.C");
  //setTDRStyle();
  gStyle->SetOptStat(0);

  TString runPeriod = "2017B";
  
  if(dataRootFile.Contains("2017B")){
    runPeriod = "2017B";
    std::cout << runPeriod << std::endl;
  }else if(dataRootFile.Contains("2017C")){
    runPeriod = "2017C";
    std::cout << runPeriod << std::endl;
  }else if(dataRootFile.Contains("2017D")){
    runPeriod = "2017D";
    std::cout << runPeriod << std::endl;
  }else if(dataRootFile.Contains("2017E")){
    runPeriod = "2017E";
    std::cout << runPeriod << std::endl;
  }else if(dataRootFile.Contains("2017F")){
    runPeriod = "2017F";
    std::cout << runPeriod << std::endl;
  }else{ 
    runPeriod = "2017";
    std::cout << runPeriod << std::endl;
  }

  TString initialPath = "/eos/user/k/kmondal/public/FLASHgg/ZeeValidation/April2018_v1/ReReco2017/ZeeOptTree_wcorr/";

  TString inFileData = initialPath+dataRootFile;
  cout << "inFileData: " << inFileData << endl;

  TString inFileMC = initialPath+mcRootFile;
  cout << "inFileMC: " << inFileMC << endl;

  TString cutebpart1 = "mass > 70 && mass < 110 && subIDMVA >-0.9 && leadIDMVA > -0.9 && abs(leadEta)<1.479";
  TString cuteepart1 = "mass > 70 && mass < 110 && subIDMVA >-0.9 && leadIDMVA > -0.9 && abs(leadEta)>1.556&&abs(leadEta)<2.5";

  TString cutMCeb1 = "weight*("+cutebpart1+")";
  TString cutMCee1 = "weight*("+cuteepart1+")";
  TString cutDataeb1 = "("+cutebpart1+")";
  TString cutDataee1 = "("+cuteepart1+")";

  
  TString finalCutMC1 = cutMCeb1;
  TString finalCutData1 = cutDataeb1;

  if (category.Contains("ee")){
    finalCutMC1 = cutMCee1;
    finalCutData1 = cutDataee1;
  }

  cout << "cutData1:  " << finalCutData1 << endl;
  cout << "cutMC1: " << finalCutMC1 << endl;


  // for sublead 

  TString cutebpart2 = "mass > 70 && mass < 110 && subIDMVA >-0.9 && leadIDMVA > -0.9 && abs(subleadEta)<1.479";
  TString cuteepart2 = "mass > 70 && mass < 110 && subIDMVA >-0.9 && leadIDMVA > -0.9 && abs(subleadEta)>1.556&&abs(subleadEta)<2.5";

  TString cutMCeb2 = "weight*("+cutebpart2+")";
  TString cutMCee2 = "weight*("+cuteepart2+")";
  TString cutDataeb2 = "("+cutebpart2+")";
  TString cutDataee2 = "("+cuteepart2+")";

  
  TString finalCutMC2 = cutMCeb2;
  TString finalCutData2 = cutDataeb2;

  if (category.Contains("ee")){
    finalCutMC2 = cutMCee2;
    finalCutData2 = cutDataee2;
  }

  cout << "cutData2:  " << finalCutData2 << endl;
  cout << "cutMC2: " << finalCutMC2 << endl;


  cout.precision(3);

  TFile* f1 = TFile::Open(inFileData); // Data
  if(f1 == 0) return;
  if(f1->IsZombie()){f1->Close(); return;}


  TH1F *data1  = new TH1F("data1","data1",nBins,observableMin,observableMax);
  TH1F *data2  = new TH1F("data2","data2",nBins,observableMin,observableMax);
  ((TTree*)f1->Get("diphotonDumper/trees/zeevalidation_13TeV_All"))->Project("data1",observable1,finalCutData1);
  ((TTree*)f1->Get("diphotonDumper/trees/zeevalidation_13TeV_All"))->Project("data2",observable2,finalCutData2);
  cout << "Mean of data1  :" << data1->GetMean() << endl;
  cout << "Integral of data1: " << data1->Integral() << endl;
  cout << "Mean of data2  :" << data2->GetMean() << endl;
  cout << "Integral of data2: " << data2->Integral() << endl;

  
  TFile* f2 = TFile::Open(inFileMC); // MC
  if(f2 == 0) return;
  if(f2->IsZombie()){f2->Close(); return;}
  
  TH1F *MC1  = new TH1F("MC1","MC1",nBins,observableMin,observableMax);
  TH1F *MC2  = new TH1F("MC2","MC2",nBins,observableMin,observableMax);
  ((TTree*)f2->Get("diphotonDumper/trees/zeevalidation_13TeV_All"))->Project("MC1",observable1,finalCutMC1);
  ((TTree*)f2->Get("diphotonDumper/trees/zeevalidation_13TeV_All"))->Project("MC2",observable2,finalCutMC2);
  cout << "Mean of MC1 :" << MC1->GetMean() << endl;
  cout << "Integral of MC1: " << MC1->Integral() << endl;
  cout << "Mean of MC2 :" << MC2->GetMean() << endl;
  cout << "Integral of MC2: " << MC2->Integral() << endl;


  // Drawing
  // customize for data
  data1->SetLineColor(kBlack);
  data1->SetLineWidth(3);
  data1->SetMarkerStyle(20); 
  data1->SetMarkerSize(0.7);

  data2->SetLineColor(kBlack);
  data2->SetLineWidth(3);
  data2->SetMarkerStyle(20); 
  data2->SetMarkerSize(0.7);

  // customize for MC
  MC1->SetLineColor(kBlue);
  MC1->SetLineWidth(3);
  MC1->SetFillColor(0);

  MC2->SetLineColor(kBlue);
  MC2->SetLineWidth(3);
  MC2->SetFillColor(0);
  //  MC->SetMarkerStyle(20);
  //  MC->SetMarkerSize(0.7);

  // Merge sublead to lead
  MC1->Add(MC2);
  data1->Add(data2);

  data1->GetXaxis()->SetTitle(xtitle);

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
  
  MC1->Scale(data1->Integral()/MC1->Integral());

  MC1->Draw("HIST");
  data1->Draw("PESAME");
  
  

  TLegend * leg = new TLegend(0.25,0.37,0.40,0.55);
  leg->SetTextSize(0.037);
  leg->SetBorderSize(0);
  leg->SetLineStyle(0);
  leg->SetTextSize(0.027);
  leg->SetFillStyle(0);
  leg->SetFillColor(0);

  leg->AddEntry(data1,"data","PE");
  leg->AddEntry(MC1,"MC","F");

  leg->Draw();

  c3.Update();

  if (ratio){
    c3.cd(2);
    gPad->SetTopMargin(0);
    gPad->SetBottomMargin(0.3);
    gPad->SetGridy();
    gPad->SetGridy();
    gPad->SetGridx();
    gPad->SetPad(xpad[0],ypad[0],xpad[1],ypad[2]);
    TH1F *hRatio = (TH1F*)data1->Clone("hRatio");
    hRatio->Reset();
    hRatio->Add(data1);

    if (data1->Integral() > 0)hRatio->Divide(MC1);
    hRatio->GetYaxis()->SetTitle("#frac{data}{MC}");
    hRatio->SetLineColor(kRed);
    hRatio->GetYaxis()->SetRangeUser(0.5, 1.5);
    hRatio->GetYaxis()->SetLabelSize(0.06);
    hRatio->GetYaxis()->SetTitleSize(0.1);
    hRatio->GetYaxis()->SetTitleOffset(0.45);
    hRatio->GetYaxis()->SetTitle("Data/MC");
    hRatio->GetXaxis()->SetLabelSize(0.1);
    hRatio->GetXaxis()->SetTitleSize(0.1);
    hRatio->GetXaxis()->SetTitleOffset(1.2);
    hRatio->Draw("P");
    c3.Update();
  }
  
  
  //  TString outFile("");
  //outFile += observable;
  //outFile += "_comp.pdf";                                                                                              
  //outFile += "comp.png";
  observable1 = observable1+"_and_"+observable2;
  observable1 += "_"+category+"_"+runPeriod;//"Run2017B";
  if(log) observable1 += "_log";
  c3.SaveAs(observable1+".root");
  c3.SaveAs(observable1+".png");



  /*

  tex->SetLineWidth(2);
  tex->Draw();
  */

  //C3.SaveAs(mode+observable+"_Stack.png");

  
}

void compareAll2017B(){

//
//  comparison("eb","leadPhIso",100,0.0,5.0,false,true);
//  comparison("ee","leadPhIso",100,0.0,5.0,false,true);
//
//  comparison("eb","leadIDMVA",100,-1.0,1.0,false,true);
//  comparison("ee","leadIDMVA",100,-1.0,1.0,false,true);
//

// Date: 6th March 2017

// ======= start of plots =====
// Date: 13May 2018
  comparison("singleEle_2017B.root","zee_DYToLL_v1_with-newPU.root","eb","leadIDMVA","subIDMVA","IDMVA",100,-1.0,1.0,false,true);
  comparison("singleEle_2017B.root","zee_DYToLL_v1_with-newPU.root","ee","leadIDMVA","subIDMVA","IDMVA",100,-1.0,1.0,false,true);
  
  comparison("singleEle_2017B.root","zee_DYToLL_v1_with-newPU.root","eb","leadChIsoRv","subleadChIsoRv","Pho_chIso_Chosen_Vtx",100,0.0,10.,true,true);
  comparison("singleEle_2017B.root","zee_DYToLL_v1_with-newPU.root","ee","leadChIsoRv","subleadChIsoRv","Pho_chIso_Chosen_Vtx",100,0.0,10.,true,true); 
  
  comparison("singleEle_2017B.root","zee_DYToLL_v1_with-newPU.root","eb","leadChIsoWv","subleadChIsoWv","Pho_chIso_Worst_Vtx",100,0.0,10.,false,true);
  comparison("singleEle_2017B.root","zee_DYToLL_v1_with-newPU.root","ee","leadChIsoWv","subleadChIsoWv","Pho_chIso_Worst_Vtx",100,0.0,10.,false,true);
  
  comparison("singleEle_2017B.root","zee_DYToLL_v1_with-newPU.root","eb","leadPhIso","subleadPhIso","phoIso",100,0.0,20.,true,true); 
  comparison("singleEle_2017B.root","zee_DYToLL_v1_with-newPU.root","ee","leadPhIso","subleadPhIso","phoIso",100,0.0,20.,true,true); 
  
// ======= END of plots =====




//  comparison("eb","leadfull5x5r9","subleadfull5x5r9","R9",100,0.5,1.0,false,true);
//  comparison("ee","leadfull5x5r9","subleadfull5x5r9","R9",100,0.8,1.0,false,true);
//
//  comparison("eb","sigmaEoE1","sigmaEoE2","#sigma_{E}/E",100,0.005,0.06,false,true);
//  comparison("ee","sigmaEoE1","sigmaEoE2","#sigma_{E}/E",100,0.005,0.06,false,true);
//
//  comparison("eb","leadoldr9","subleadoldr9","R9",100,0.5,1.0,false,true);
//  comparison("ee","leadoldr9","subleadoldr9","R9",100,0.8,1.0,false,true);
  
  /*
  comparison("eb","leadoldsieie", "subleadoldsieie","#sigma_{i#etai#eta}",100,0.0,0.015,false,true);
  comparison("ee","leadoldsieie", "subleadoldsieie","#sigma_{i#etai#eta}",100,0.015,0.04,false,true);

  comparison("eb","leadoldcovieip", "subleadoldcovieip","#sigma_{i#etai#phi}",50,-0.0001, 0.0001,false,true);
  comparison("ee","leadoldcovieip", "subleadoldcovieip","#sigma_{i#etai#phi}",50,-0.0005, 0.0005,false,true);

  comparison("eb","leadoldcovipip","subleadoldcovipip","sigma_{i#phii#phi}",50, 0.0, 0.03, false,true);
  comparison("ee","leadoldcovipip","subleadoldcovipip","sigma_{i#phii#phi}",50, 0.0, 0.07, false,true);
  */	     



//  comparison("eb","leadetawidth",100,0.0,0.02,false,true);
//  comparison("ee","leadetawidth",100,0.0,0.05,false,true);




//  comparison("eb","mass",120,60.0,120.0,true); 
//  comparison("eb","mass",120,60.0,120.0,false);

}

void compareAll2017C(){
  
  comparison("singleEle_2017C.root","zee_DYToLL_v1_with-newPU.root","eb","leadIDMVA","subIDMVA","IDMVA",100,-1.0,1.0,false,true);
  comparison("singleEle_2017C.root","zee_DYToLL_v1_with-newPU.root","ee","leadIDMVA","subIDMVA","IDMVA",100,-1.0,1.0,false,true);
  
  comparison("singleEle_2017C.root","zee_DYToLL_v1_with-newPU.root","eb","leadChIsoRv","subleadChIsoRv","Pho_chIso_Chosen_Vtx",100,0.0,10.,true,true);
  comparison("singleEle_2017C.root","zee_DYToLL_v1_with-newPU.root","ee","leadChIsoRv","subleadChIsoRv","Pho_chIso_Chosen_Vtx",100,0.0,10.,true,true); 
  
  comparison("singleEle_2017C.root","zee_DYToLL_v1_with-newPU.root","eb","leadChIsoWv","subleadChIsoWv","Pho_chIso_Worst_Vtx",100,0.0,10.,false,true);
  comparison("singleEle_2017C.root","zee_DYToLL_v1_with-newPU.root","ee","leadChIsoWv","subleadChIsoWv","Pho_chIso_Worst_Vtx",100,0.0,10.,false,true);
  
  comparison("singleEle_2017C.root","zee_DYToLL_v1_with-newPU.root","eb","leadPhIso","subleadPhIso","phoIso",100,0.0,20.,true,true); 
  comparison("singleEle_2017C.root","zee_DYToLL_v1_with-newPU.root","ee","leadPhIso","subleadPhIso","phoIso",100,0.0,20.,true,true); 
}

void compareAll2017D(){
  
  comparison("singleEle_2017D.root","zee_DYToLL_v1_with-newPU.root","eb","leadIDMVA","subIDMVA","IDMVA",100,-1.0,1.0,false,true);
  comparison("singleEle_2017D.root","zee_DYToLL_v1_with-newPU.root","ee","leadIDMVA","subIDMVA","IDMVA",100,-1.0,1.0,false,true);
  
  comparison("singleEle_2017D.root","zee_DYToLL_v1_with-newPU.root","eb","leadChIsoRv","subleadChIsoRv","Pho_chIso_Chosen_Vtx",100,0.0,10.,true,true);
  comparison("singleEle_2017D.root","zee_DYToLL_v1_with-newPU.root","ee","leadChIsoRv","subleadChIsoRv","Pho_chIso_Chosen_Vtx",100,0.0,10.,true,true); 
  
  comparison("singleEle_2017D.root","zee_DYToLL_v1_with-newPU.root","eb","leadChIsoWv","subleadChIsoWv","Pho_chIso_Worst_Vtx",100,0.0,10.,false,true);
  comparison("singleEle_2017D.root","zee_DYToLL_v1_with-newPU.root","ee","leadChIsoWv","subleadChIsoWv","Pho_chIso_Worst_Vtx",100,0.0,10.,false,true);
  
  comparison("singleEle_2017D.root","zee_DYToLL_v1_with-newPU.root","eb","leadPhIso","subleadPhIso","phoIso",100,0.0,20.,true,true); 
  comparison("singleEle_2017D.root","zee_DYToLL_v1_with-newPU.root","ee","leadPhIso","subleadPhIso","phoIso",100,0.0,20.,true,true); 
}

void compareAll2017E(){
  
  comparison("singleEle_2017E.root","zee_DYToLL_v1_with-newPU.root","eb","leadIDMVA","subIDMVA","IDMVA",100,-1.0,1.0,false,true);
  comparison("singleEle_2017E.root","zee_DYToLL_v1_with-newPU.root","ee","leadIDMVA","subIDMVA","IDMVA",100,-1.0,1.0,false,true);
  
  comparison("singleEle_2017E.root","zee_DYToLL_v1_with-newPU.root","eb","leadChIsoRv","subleadChIsoRv","Pho_chIso_Chosen_Vtx",100,0.0,10.,true,true);
  comparison("singleEle_2017E.root","zee_DYToLL_v1_with-newPU.root","ee","leadChIsoRv","subleadChIsoRv","Pho_chIso_Chosen_Vtx",100,0.0,10.,true,true); 
  
  comparison("singleEle_2017E.root","zee_DYToLL_v1_with-newPU.root","eb","leadChIsoWv","subleadChIsoWv","Pho_chIso_Worst_Vtx",100,0.0,10.,false,true);
  comparison("singleEle_2017E.root","zee_DYToLL_v1_with-newPU.root","ee","leadChIsoWv","subleadChIsoWv","Pho_chIso_Worst_Vtx",100,0.0,10.,false,true);
  
  comparison("singleEle_2017E.root","zee_DYToLL_v1_with-newPU.root","eb","leadPhIso","subleadPhIso","phoIso",100,0.0,20.,true,true); 
  comparison("singleEle_2017E.root","zee_DYToLL_v1_with-newPU.root","ee","leadPhIso","subleadPhIso","phoIso",100,0.0,20.,true,true); 
}

void compareAll2017F(){
  
  comparison("singleEle_2017F.root","zee_DYToLL_v1_with-newPU.root","eb","leadIDMVA","subIDMVA","IDMVA",100,-1.0,1.0,false,true);
  comparison("singleEle_2017F.root","zee_DYToLL_v1_with-newPU.root","ee","leadIDMVA","subIDMVA","IDMVA",100,-1.0,1.0,false,true);
  
  comparison("singleEle_2017F.root","zee_DYToLL_v1_with-newPU.root","eb","leadChIsoRv","subleadChIsoRv","Pho_chIso_Chosen_Vtx",100,0.0,10.,true,true);
  comparison("singleEle_2017F.root","zee_DYToLL_v1_with-newPU.root","ee","leadChIsoRv","subleadChIsoRv","Pho_chIso_Chosen_Vtx",100,0.0,10.,true,true); 
  
  comparison("singleEle_2017F.root","zee_DYToLL_v1_with-newPU.root","eb","leadChIsoWv","subleadChIsoWv","Pho_chIso_Worst_Vtx",100,0.0,10.,false,true);
  comparison("singleEle_2017F.root","zee_DYToLL_v1_with-newPU.root","ee","leadChIsoWv","subleadChIsoWv","Pho_chIso_Worst_Vtx",100,0.0,10.,false,true);
  
  comparison("singleEle_2017F.root","zee_DYToLL_v1_with-newPU.root","eb","leadPhIso","subleadPhIso","phoIso",100,0.0,20.,true,true); 
  comparison("singleEle_2017F.root","zee_DYToLL_v1_with-newPU.root","ee","leadPhIso","subleadPhIso","phoIso",100,0.0,20.,true,true); 
}
void compareAll2017(){
  comparison("output_SingleElectron_OptTreeDumper_Rereco2017.root","output_DYJetsToLL_OptTreeDumper_Rereco2017_wcorr.root","eb","leadIDMVA","subIDMVA","IDMVA",100,-1.0,1.0,false,true);
  comparison("output_SingleElectron_OptTreeDumper_Rereco2017.root","output_DYJetsToLL_OptTreeDumper_Rereco2017_wcorr.root","ee","leadIDMVA","subIDMVA","IDMVA",100,-1.0,1.0,false,true);
  
  comparison("output_SingleElectron_OptTreeDumper_Rereco2017.root","output_DYJetsToLL_OptTreeDumper_Rereco2017_wcorr.root","eb","leadChIsoRv","subleadChIsoRv","Pho_chIso_Chosen_Vtx",100,0.0,10.,true,true);
  comparison("output_SingleElectron_OptTreeDumper_Rereco2017.root","output_DYJetsToLL_OptTreeDumper_Rereco2017_wcorr.root","ee","leadChIsoRv","subleadChIsoRv","Pho_chIso_Chosen_Vtx",100,0.0,10.,true,true); 
  
  comparison("output_SingleElectron_OptTreeDumper_Rereco2017.root","output_DYJetsToLL_OptTreeDumper_Rereco2017_wcorr.root","eb","leadChIsoWv","subleadChIsoWv","Pho_chIso_Worst_Vtx",100,0.0,10.,false,true);
  comparison("output_SingleElectron_OptTreeDumper_Rereco2017.root","output_DYJetsToLL_OptTreeDumper_Rereco2017_wcorr.root","ee","leadChIsoWv","subleadChIsoWv","Pho_chIso_Worst_Vtx",100,0.0,10.,false,true);
  
  comparison("output_SingleElectron_OptTreeDumper_Rereco2017.root","output_DYJetsToLL_OptTreeDumper_Rereco2017_wcorr.root","eb","leadPhIso","subleadPhIso","phoIso",100,0.0,20.,true,true); 
  comparison("output_SingleElectron_OptTreeDumper_Rereco2017.root","output_DYJetsToLL_OptTreeDumper_Rereco2017_wcorr.root","ee","leadPhIso","subleadPhIso","phoIso",100,0.0,20.,true,true); 

  comparison("output_SingleElectron_OptTreeDumper_Rereco2017.root","output_DYJetsToLL_OptTreeDumper_Rereco2017_wcorr.root","eb","nvtx","nvtx","nVtx",100,0.0,100.,false,true); 
  comparison("output_SingleElectron_OptTreeDumper_Rereco2017.root","output_DYJetsToLL_OptTreeDumper_Rereco2017_wcorr.root","ee","nvtx","nvtx","nVtx",100,0.0,100.,false,true); 

  comparison("output_SingleElectron_OptTreeDumper_Rereco2017.root","output_DYJetsToLL_OptTreeDumper_Rereco2017_wcorr.root","eb","leadfull5x5r9","subleadfull5x5r9","full5x5r9",100,0.5,1.0,false,true); 
  comparison("output_SingleElectron_OptTreeDumper_Rereco2017.root","output_DYJetsToLL_OptTreeDumper_Rereco2017_wcorr.root","ee","leadfull5x5r9","subleadfull5x5r9","full5x5r9",100,0.8,1.0,false,true); 

  comparison("output_SingleElectron_OptTreeDumper_Rereco2017.root","output_DYJetsToLL_OptTreeDumper_Rereco2017_wcorr.root","eb","leadfull5x5sieie","subleadfull5x5sieie","full5x5sieie",100,0.0,0.011,false,true); 
  comparison("output_SingleElectron_OptTreeDumper_Rereco2017.root","output_DYJetsToLL_OptTreeDumper_Rereco2017_wcorr.root","ee","leadfull5x5sieie","subleadfull5x5sieie","full5x5sieie",100,0.016,0.035,false,true); 

  comparison("output_SingleElectron_OptTreeDumper_Rereco2017.root","output_DYJetsToLL_OptTreeDumper_Rereco2017_wcorr.root","eb","leadetawidth","subleadetawidth","etawidth",100,0.0,0.02,false,true); 
  comparison("output_SingleElectron_OptTreeDumper_Rereco2017.root","output_DYJetsToLL_OptTreeDumper_Rereco2017_wcorr.root","ee","leadetawidth","subleadetawidth","etawidth",100,0.0,0.05,false,true); 

  comparison("output_SingleElectron_OptTreeDumper_Rereco2017.root","output_DYJetsToLL_OptTreeDumper_Rereco2017_wcorr.root","eb","leads4ratio","subleads4ratio","s4ratio",70,0.3,1.0,false,true); 
  comparison("output_SingleElectron_OptTreeDumper_Rereco2017.root","output_DYJetsToLL_OptTreeDumper_Rereco2017_wcorr.root","ee","leads4ratio","subleads4ratio","s4ratio",70,0.3,1.0,false,true);
}
