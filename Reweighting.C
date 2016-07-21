#define Reweighting_cxx
#include "Reweighting.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <cmath> 
#include <iostream>
void Reweighting::Loop()
{
//   In a ROOT session, you can do:
//      root> .L Reweighting.C
//      root> Reweighting t
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

 
   TH1F *diPhotonptZ = new TH1F("diPhotonptZ","diPhotonptZ",300,0.,300.);
   TH1F *diPhotonptH = new TH1F("diPhotonptH","diPhotonptH",300,0.,300.);
   TH1F *r9ZEB = new TH1F("r9ZEB","r9ZEB",100,0.,1.);
   TH1F *r9HEB = new TH1F("r9HEB","r9HEB",100,0.,1.);
   TH1F *r9ZEE = new TH1F("r9ZEE","r9ZEE",100,0.,1.);
   TH1F *r9HEE = new TH1F("r9HEE","r9HEE",100,0.,1.);
   
   Long64_t nentries = fChain->GetEntriesFast();
   
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      if (fCurrent==0){
	diPhotonptH->Fill(dipho_pt);
	if (abs(scEta) > 0.0 && abs(scEta) <= 1.456)
	  r9HEB->Fill(leadr9);
	else if (abs(scEta) >= 1.547 && abs(scEta) < 2.5)
	  r9HEE->Fill(leadr9);
      }
      if (fCurrent==1){
	diPhotonptZ->Fill(dipho_pt);   
	if (abs(scEta) > 0.0 && abs(scEta) <= 1.456)
	  r9ZEB->Fill(leadr9);
	else if (abs(scEta) >= 1.547 && abs(scEta) < 2.5)
	  r9ZEE->Fill(leadr9);
      }
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
   }
   diPhotonptZ->Scale(1.0/diPhotonptZ->Integral("width"));
   diPhotonptH->Scale(1.0/diPhotonptH->Integral("width"));
   r9ZEB->Scale(1.0/r9ZEB->Integral("width"));
   r9HEB->Scale(1.0/r9HEB->Integral("width"));
   r9ZEE->Scale(1.0/r9ZEE->Integral("width"));
   r9HEE->Scale(1.0/r9HEE->Integral("width"));

   TH1F *ptWeight = (TH1F*)diPhotonptH->Clone("ptWeight");
   TH1F *r9WeightEB = (TH1F*)r9HEB->Clone("r9WeightEB");
   TH1F *r9WeightEE = (TH1F*)r9HEE->Clone("r9WeightEE");
   ptWeight->Divide(diPhotonptZ);
   r9WeightEB->Divide(r9ZEB);
   r9WeightEE->Divide(r9ZEE);
   TFile f("weightsElePho.root","RECREATE");
   diPhotonptH->Write();
   diPhotonptZ->Write();
   r9HEB->Write();
   r9ZEB->Write();
   r9HEE->Write();
   r9ZEE->Write();
   ptWeight->Write();
   r9WeightEB->Write();
   r9WeightEE->Write();
   f.Write();
}
