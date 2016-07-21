//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sat May 14 20:15:36 2016 by ROOT version 6.04/02
// from TTree zeevalidation_13TeV_All/zeevalidation_13TeV_All
// found on file: output_data_single.root
//////////////////////////////////////////////////////////

#ifndef nVtxweight_h
#define nVtxweight_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class nVtxweight {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           candidate_id;
   Float_t         weight;
   Float_t         leadPhIso;
   Float_t         leadNeuIso;
   Float_t         leadsieie;
   Float_t         leadcovieip;
   Float_t         leadetawidth;
   Float_t         leadphiwidth;
   Float_t         leads4ratio;
   Float_t         leadr9;
   Float_t         leadfull5x5r9;
   Float_t         leadPt;
   Float_t         leadEta;
   Float_t         leadIDMVA;
   Float_t         leadChIsoRv;
   Float_t         leadChIsoWv;
   Float_t         leadESSigma;
   Float_t         subleadPhIso;
   Float_t         subLeadNeuIso;
   Float_t         subleadsieie;
   Float_t         subleadcovieip;
   Float_t         subleadetawidth;
   Float_t         subleadphiwidth;
   Float_t         subleads4ratio;
   Float_t         subleadr9;
   Float_t         subleadfull5x5r9;
   Float_t         subleadPt;
   Float_t         subleadEta;
   Float_t         subIDMVA;
   Float_t         subleadChIsoRv;
   Float_t         subleadChIsoWv;
   Float_t         subleadESSigma;
   Float_t         scEta;
   Float_t         sigmaEoE1;
   Float_t         sigmaEoE2;
   Float_t         sigmaMoM;
   Float_t         vtxProb;
   Float_t         cosdphi;
   Float_t         dipho_pt;
   Float_t         nVtx;
   Float_t         rho;
   ULong64_t       event;
   UInt_t          lumi;
   Int_t           processIndex;
   UInt_t          run;
   Int_t           nvtx;
   Float_t         npu;

   // List of branches
   TBranch        *b_candidate_id;   //!
   TBranch        *b_weight;   //!
   TBranch        *b_leadPhIso;   //!
   TBranch        *b_leadNeuIso;   //!
   TBranch        *b_leadsieie;   //!
   TBranch        *b_leadcovieip;   //!
   TBranch        *b_leadetawidth;   //!
   TBranch        *b_leadphiwidth;   //!
   TBranch        *b_leads4ratio;   //!
   TBranch        *b_leadr9;   //!
   TBranch        *b_leadfull5x5r9;   //!
   TBranch        *b_leadPt;   //!
   TBranch        *b_leadEta;   //!
   TBranch        *b_leadIDMVA;   //!
   TBranch        *b_leadChIsoRv;   //!
   TBranch        *b_leadChIsoWv;   //!
   TBranch        *b_leadESSigma;   //!
   TBranch        *b_subleadPhIso;   //!
   TBranch        *b_subLeadNeuIso;   //!
   TBranch        *b_subleadsieie;   //!
   TBranch        *b_subleadcovieip;   //!
   TBranch        *b_subleadetawidth;   //!
   TBranch        *b_subleadphiwidth;   //!
   TBranch        *b_subleads4ratio;   //!
   TBranch        *b_subleadr9;   //!
   TBranch        *b_subleadfull5x5r9;   //!
   TBranch        *b_subleadPt;   //!
   TBranch        *b_subleadEta;   //!
   TBranch        *b_subIDMVA;   //!
   TBranch        *b_subleadChIsoRv;   //!
   TBranch        *b_subleadChIsoWv;   //!
   TBranch        *b_subleadESSigma;   //!
   TBranch        *b_scEta;   //!
   TBranch        *b_sigmaEoE1;   //!
   TBranch        *b_sigmaEoE2;   //!
   TBranch        *b_sigmaMoM;   //!
   TBranch        *b_vtxProb;   //!
   TBranch        *b_cosdphi;   //!
   TBranch        *b_dipho_pt;   //!
   TBranch        *b_rho;   //!
   TBranch        *b_nvtx;   //!
   TBranch        *b_event;   //!
   TBranch        *b_lumi;   //!
   TBranch        *b_processIndex;   //!
   TBranch        *b_run;   //!
   TBranch        *b_npu;   //!

   nVtxweight(TTree *tree=0);
   virtual ~nVtxweight();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef nVtxweight_cxx
nVtxweight::nVtxweight(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
#ifdef SINGLE_TREE
     // The following code should be used if you want this class to access
     // a single tree instead of a chain
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("output_data_double.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("output_data_double.root");
      }
      TDirectory * dir = (TDirectory*)f->Get("output_data_double.root:/diphotonDumper/trees");
      dir->GetObject("zeevalidation_13TeV_All",tree);
#else // SINGLE_TREE
      TChain *chain= new TChain("diphotonDumper/trees/zeevalidation_13TeV_All");
      //      chain->Add("output_data_single.root/diphotonDumper/trees/zeevalidation_13TeV_All");
      //chain->Add("output_data_double.root/diphotonDumper/trees/zeevalidation_13TeV_All");
      //chain->Add("output_mc_double.root/diphotonDumper/trees/zeevalidation_13TeV_All");
      chain->Add("output_data_single.root/diphotonDumper/trees/zeevalidation_13TeV_All");
      chain->Add("output_mc_single.root/diphotonDumper/trees/zeevalidation_13TeV_All");
      tree = chain;
#endif // SINGLE_TREE
   }
   Init(tree);
}




nVtxweight::~nVtxweight()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t nVtxweight::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t nVtxweight::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (!fChain->InheritsFrom(TChain::Class()))  return centry;
   TChain *chain = (TChain*)fChain;
   if (chain->GetTreeNumber() != fCurrent) {
      fCurrent = chain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void nVtxweight::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("candidate_id", &candidate_id, &b_candidate_id);
   fChain->SetBranchAddress("weight", &weight, &b_weight);
   fChain->SetBranchAddress("leadPhIso", &leadPhIso, &b_leadPhIso);
   fChain->SetBranchAddress("leadNeuIso", &leadNeuIso, &b_leadNeuIso);
   fChain->SetBranchAddress("leadsieie", &leadsieie, &b_leadsieie);
   fChain->SetBranchAddress("leadcovieip", &leadcovieip, &b_leadcovieip);
   fChain->SetBranchAddress("leadetawidth", &leadetawidth, &b_leadetawidth);
   fChain->SetBranchAddress("leadphiwidth", &leadphiwidth, &b_leadphiwidth);
   fChain->SetBranchAddress("leads4ratio", &leads4ratio, &b_leads4ratio);
   fChain->SetBranchAddress("leadr9", &leadr9, &b_leadr9);
   fChain->SetBranchAddress("leadfull5x5r9", &leadfull5x5r9, &b_leadfull5x5r9);
   fChain->SetBranchAddress("leadPt", &leadPt, &b_leadPt);
   fChain->SetBranchAddress("leadEta", &leadEta, &b_leadEta);
   fChain->SetBranchAddress("leadIDMVA", &leadIDMVA, &b_leadIDMVA);
   fChain->SetBranchAddress("leadChIsoRv", &leadChIsoRv, &b_leadChIsoRv);
   fChain->SetBranchAddress("leadChIsoWv", &leadChIsoWv, &b_leadChIsoWv);
   fChain->SetBranchAddress("leadESSigma", &leadESSigma, &b_leadESSigma);
   fChain->SetBranchAddress("subleadPhIso", &subleadPhIso, &b_subleadPhIso);
   fChain->SetBranchAddress("subLeadNeuIso", &subLeadNeuIso, &b_subLeadNeuIso);
   fChain->SetBranchAddress("subleadsieie", &subleadsieie, &b_subleadsieie);
   fChain->SetBranchAddress("subleadcovieip", &subleadcovieip, &b_subleadcovieip);
   fChain->SetBranchAddress("subleadetawidth", &subleadetawidth, &b_subleadetawidth);
   fChain->SetBranchAddress("subleadphiwidth", &subleadphiwidth, &b_subleadphiwidth);
   fChain->SetBranchAddress("subleads4ratio", &subleads4ratio, &b_subleads4ratio);
   fChain->SetBranchAddress("subleadr9", &subleadr9, &b_subleadr9);
   fChain->SetBranchAddress("subleadfull5x5r9", &subleadfull5x5r9, &b_subleadfull5x5r9);
   fChain->SetBranchAddress("subleadPt", &subleadPt, &b_subleadPt);
   fChain->SetBranchAddress("subleadEta", &subleadEta, &b_subleadEta);
   fChain->SetBranchAddress("subIDMVA", &subIDMVA, &b_subIDMVA);
   fChain->SetBranchAddress("subleadChIsoRv", &subleadChIsoRv, &b_subleadChIsoRv);
   fChain->SetBranchAddress("subleadChIsoWv", &subleadChIsoWv, &b_subleadChIsoWv);
   fChain->SetBranchAddress("subleadESSigma", &subleadESSigma, &b_subleadESSigma);
   fChain->SetBranchAddress("scEta", &scEta, &b_scEta);
   fChain->SetBranchAddress("sigmaEoE1", &sigmaEoE1, &b_sigmaEoE1);
   fChain->SetBranchAddress("sigmaEoE2", &sigmaEoE2, &b_sigmaEoE2);
   fChain->SetBranchAddress("sigmaMoM", &sigmaMoM, &b_sigmaMoM);
   fChain->SetBranchAddress("vtxProb", &vtxProb, &b_vtxProb);
   fChain->SetBranchAddress("cosdphi", &cosdphi, &b_cosdphi);
   fChain->SetBranchAddress("dipho_pt", &dipho_pt, &b_dipho_pt);
   fChain->SetBranchAddress("rho", &rho, &b_rho);
   fChain->SetBranchAddress("nvtx", &nvtx, &b_nvtx);
   fChain->SetBranchAddress("event", &event, &b_event);
   fChain->SetBranchAddress("lumi", &lumi, &b_lumi);
   fChain->SetBranchAddress("processIndex", &processIndex, &b_processIndex);
   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("nvtx", &nvtx, &b_nvtx);
   fChain->SetBranchAddress("npu", &npu, &b_npu);
   Notify();
}

Bool_t nVtxweight::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void nVtxweight::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t nVtxweight::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef nVtxweight_cxx
