#define nVtxweight_cxx
#include "nVtxweight.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <cmath> 
#include <iostream>
void nVtxweight::Loop()
{
//   In a ROOT session, you can do:
//      root> .L nVtxweight.C
//      root> nVtxweight t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

 
   TH1F *nvtxdata = new TH1F("nvtxdata","nvtxdata",300,0.,300.);
   TH1F *nvtxmc = new TH1F("nvtxmc","nvtxmc",300,0.,300.);
   //TH1F *r9ZEB = new TH1F("r9ZEB","r9ZEB",100,0.,1.);
   //TH1F *r9HEB = new TH1F("r9HEB","r9HEB",100,0.,1.);
   //TH1F *r9ZEE = new TH1F("r9ZEE","r9ZEE",100,0.,1.);
   //TH1F *r9HEE = new TH1F("r9HEE","r9HEE",100,0.,1.);
   
   Long64_t nentries = fChain->GetEntriesFast();
   
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      if (fCurrent==0){
	nvtxdata->Fill(nvtx);
	//if (abs(scEta) > 0.0 && abs(scEta) <= 1.456)
	//  r9HEB->Fill(leadr9);
	//else if (abs(scEta) >= 1.547 && abs(scEta) < 2.5)
	//  r9HEE->Fill(leadr9);
      }
      if (fCurrent==1){
	nvtxmc->Fill(nvtx);   
	//if (abs(scEta) > 0.0 && abs(scEta) <= 1.456)
	//  r9ZEB->Fill(leadr9);
	//else if (abs(scEta) >= 1.547 && abs(scEta) < 2.5)
	//  r9ZEE->Fill(leadr9);
      }
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
   }
   nvtxdata->Scale(1.0/nvtxdata->Integral("width"));
   nvtxmc->Scale(1.0/nvtxmc->Integral("width"));

   TH1F *nvtxweight = (TH1F*)nvtxdata->Clone("nvtxweight");
   //TH1F *r9WeightEB = (TH1F*)r9HEB->Clone("r9WeightEB");
   //TH1F *r9WeightEE = (TH1F*)r9HEE->Clone("r9WeightEE");
   nvtxweight->Divide(nvtxmc);
   //r9WeightEB->Divide(r9ZEB);
   //r9WeightEE->Divide(r9ZEE);
   TFile f("nVertexWeight.root","RECREATE");
   nvtxdata->Write();
   nvtxmc->Write();
   //r9HEB->Write();
   //r9ZEB->Write();
   //r9HEE->Write();
   //r9ZEE->Write();
   nvtxweight->Write();
   //r9WeightEB->Write();
   //r9WeightEE->Write();
   f.Write();
}
