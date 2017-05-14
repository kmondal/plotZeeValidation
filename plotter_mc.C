#include "TChain.h"
#include "TFile.h"
#include "TH1F.h"
#include "TGraph.h"
#include "TEntryList.h"
#include "TLeaf.h"
#include "TTreeFormula.h"
#include "TROOT.h"

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <sstream>
using std::ifstream;
float idmvaShift = 0.03; //0.03 in Moriond;
float sigmaEScale = 0.05; //0.05; // relative 5%

std::vector<std::pair<std::string, int> > samples;

typedef std::map<std::string, Int_t> TBranchesI;
typedef std::map<std::string, Float_t> TBranchesF;
typedef std::map<std::string, Double_t> TBranchesD;
typedef std::map<std::string, UInt_t> TBranchesUI;
typedef std::map<std::string, UChar_t> TBranchesUC;

struct HistoDefinition {
  int type;
  int ncat;
  int log;
  int xbins;
  int ybins;
  float xmax, ymax, xmin, ymin;
  std::string name, xaxis, yaxis, var;
};

TFile* weights;
TFile* nVtxweights;
TH1F *r9WeightEB, *r9WeightEE, *ptWeight, *nvtxweight;
bool firstTime = true;
bool isOpen = true;
float getWeights(float r9, float eta, float pt) {  
    
  if (firstTime) {
    firstTime = false;
    weights = TFile::Open("weightsElePho.root");

    r9WeightEB = (TH1F*)weights->Get("r9WeightEB");
    r9WeightEE = (TH1F*)weights->Get("r9WeightEE");
    ptWeight   = (TH1F*)weights->Get("ptWeight");
  }

  if (pt > 0) {
    int bin = ptWeight->FindBin(pt);
    return ptWeight->GetBinContent(bin);
  } else {
    if (fabs(eta)<1.479) {
      int bin = r9WeightEB->FindBin(r9);
      return r9WeightEB->GetBinContent(bin);
    } else {
      int bin = r9WeightEE->FindBin(r9);
      return r9WeightEE->GetBinContent(bin);
    }
  }
}
float getVtxWeights(int nvtx) {  
    
  if (isOpen) {
    isOpen = false;
    nVtxweights = TFile::Open("nVertexWeight.root");

    nvtxweight   = (TH1F*)nVtxweights->Get("nvtxweight");
  }

  if (nvtx > 0) {
    int bin = nvtxweight->FindBin(nvtx);
    return nvtxweight->GetBinContent(bin);
  }
  else return 0;
  //else {
    //if (fabs(eta)<1.479) {
    //  int bin = r9WeightEB->FindBin(r9);
    //  return r9WeightEB->GetBinContent(bin);
    //} else {
    //  int bin = r9WeightEE->FindBin(r9);
    //  return r9WeightEE->GetBinContent(bin);
    //}
  //}
}
  
class HistoContainer {
public:
  HistoContainer() {};
  ~HistoContainer() {};

  //std::map<std::string, TH1I> histoI;
  std::map<std::string, std::vector<TH1F> > histoF;
  //int categories;
};

void readTransformations(std::vector<TGraph*>& graphs) {
  TFile* fInput = TFile::Open("transformationIDMVA_v2.root");
  std::cout << "reading transformations file " << std::endl;
  graphs.push_back((TGraph*)fInput->Get("trasfhebdown"));
  graphs.push_back((TGraph*)fInput->Get("trasfheedown"));
  graphs.push_back((TGraph*)fInput->Get("trasfhebup"));
  graphs.push_back((TGraph*)fInput->Get("trasfheeup"));
  fInput->Close();
}

void plotter_mc(const char* datafilename, const char* mcfilename, const char *idmvaCorrectionFile = NULL) {

  // READ Transformations
  std::vector<TGraph*> graphs;
  if (idmvaCorrectionFile != NULL) readTransformations(graphs);
    //readTransformations(graphs, idmvaCorrectionFile);

  //std::vector<TGraph*> graphs;
  //readTransformations(graphs);
  
  // READ SAMPLES
  ifstream myReadFile;
  myReadFile.open("inputfiles.dat");
  char output[100];
  int itype = -1;
  if (myReadFile.is_open()) {
    while (!myReadFile.eof()) {
      float xsec;
      myReadFile >> output >> xsec >> itype;
      std::string init(output);
      if (init.substr(0,1) != "#") {
	samples.push_back(std::pair<std::string, int>(output, itype));
	//std::cout << output << " " << itype << std::endl;
      }
    }
  }
  myReadFile.close();
  std::cout<< "Debug level 1" << std::endl; 
  
  //std::string weight = "weight";
  
  for (int sampletype=0; sampletype<2; sampletype++) {
    
    TChain* chain = new TChain("diphotonDumper/trees/zeevalidation_13TeV_All");
    if (sampletype == 0)
      chain->Add(datafilename);
      //chain->Add("output_data_double.root/diphotonDumper/trees/zeevalidation_13TeV_All");
    else
      chain->Add(mcfilename);
      //chain->Add("output_mc_double.root/diphotonDumper/trees/zeevalidation_13TeV_All");
    
    TBranchesI branchesI;
    TBranchesF branchesF;
    TBranchesD branchesD;
    TBranchesUI branchesUI;
    TBranchesUC branchesUC;
    
    auto leafList = chain->GetListOfLeaves();
    for (auto leaf : *leafList) {
      std::string name =((TLeaf*)leaf)->GetName();
      std::string type = ((TLeaf*)leaf)->GetTypeName();
      
      std::cout << name << type << endl;
            if (type == "Int_t") {
	Int_t a = 0;
	branchesI[name] = a;
	chain->SetBranchAddress(name.c_str(), &(branchesI[name]));
      } else if (type == "Float_t") {
	Float_t a = 0;
	branchesF[name] = a;
	chain->SetBranchAddress(name.c_str(), &(branchesF[name]));
      } else if (type == "Double_t") {
	Double_t a = 0;
	branchesD[name] = a;
	chain->SetBranchAddress(name.c_str(), &(branchesD[name]));
      } else if (type == "UInt_t") {
	UInt_t a = 0;
	branchesUI[name] = a;
	chain->SetBranchAddress(name.c_str(), &(branchesUI[name]));
      } else if (type == "UChar_t") {
	UChar_t a = 0;
	branchesUC[name] = a;
	chain->SetBranchAddress(name.c_str(), &(branchesUC[name]));
	}
	    //std::cout<< "Debug level 1.1" << std::endl; 
    }
    //std::cout<< "Debug level 2" << std::endl; 
    
    std::map<int, std::vector<TTreeFormula*> > categories;
    std::cout << "Reading categories...." << std::endl;
    myReadFile.open("categories.dat");
    std::string line;
    int cat;
    while(!myReadFile.eof()) {
      myReadFile >> cat >> line;
      char a[100];
      if (categories.find(cat) == categories.end() ) {
	sprintf(a, "cat%d_%d", cat, 0);
	std::cout << a << " " << line << std::endl;
	categories[cat].push_back(new TTreeFormula(a, line.c_str(), chain));
      } else {
	sprintf(a, "cat%d_%zu", cat, categories[cat].size());
	std::cout << a << " " << line << std::endl;
	categories[cat].push_back(new TTreeFormula(a, line.c_str(), chain));
      }
    }
    myReadFile.close();
    //std::cout<< "Debug level 3" << std::endl; 

    
    std::vector<HistoDefinition> histoDef;
    // READING PLOT DEFINITION
    std::cout << "Reading plots..." << std::endl;
    myReadFile.open("plotvariables_AN.dat");
    std::map<int, HistoContainer> histos;
    histos[samples[sampletype].second] = HistoContainer();
    
    while(!myReadFile.eof()) {
      // type ncat==1 xbin ybins xmin xmax ymin ymax name xaxis yaxis var cat????
      HistoDefinition temp;
      myReadFile >> temp.type >> temp.log >> temp.ncat >> temp.xbins >> temp.ybins >> temp.xmin >> temp.xmax
		 >> temp.ymin >> temp.ymax >> temp.name >> temp.xaxis >> temp.yaxis >> temp.var;
      histoDef.push_back(temp);
      
      TH1F* h = new TH1F("h", "", temp.xbins, temp.xmin, temp.xmax);
      h->GetXaxis()->SetTitle(temp.xaxis.c_str());
      h->GetYaxis()->SetTitle(temp.yaxis.c_str());
      
      int ncats = categories[temp.ncat].size();
      for (int c=0; c<ncats; c++) {
	
	std::ostringstream convert; 
	convert << c;
	std::string name = temp.name + "_cat"+ convert.str() + "_" + samples[sampletype].first;  
	std::cout << c << " " << name << std::endl;
	histos[samples[sampletype].second].histoF[temp.name].push_back(*((TH1F*)h->Clone(name.c_str())));
      }
      delete h;
    }
    myReadFile.close();
    std::cout<< "Debug level 4 " << chain->GetEntries() << std::endl; 

    for (int z=0; z<chain->GetEntries(); z++) {
      if (z%10000 == 0)
	std::cout << z << std::endl;
      
      chain->GetEntry(z);
      float mass = branchesF["mass"];

      if ((mass > 70 && mass < 110) && (branchesF["subIDMVA"]>-0.9 && branchesF["leadIDMVA"]>-0.9) && (branchesF["leadPt"]>40. && branchesF["subleadPt"]>30.)) {
	
	float weight = 1; 
	if (sampletype == 1) {
	  weight = branchesF["weight"];
	}  
	for (unsigned int s=0; s<samples.size(); s++) {
	  for (unsigned int h=0; h<histoDef.size(); h++) {
	    //std::cout << histoDef[h].name << " " << histoDef[h].var << " " << histoDef[h].ncat << std::endl;
	    std::string name = histoDef[h].name;
	    std::string var = histoDef[h].var;
	    int category = histoDef[h].ncat;
	    
	    float final_weight = weight;
	    
	    for (unsigned int cat=0; cat<categories[category].size(); cat++) {
	      categories[category][cat]->UpdateFormulaLeaves();
	      //std::cout<< "Going to draw histogram " << name <<std::endl;
	      
	      if (categories[category][cat]->EvalInstance()) {
		if (name == "idmvaup1" || name == "idmvaup2")
 		  histos[samples[sampletype].second].histoF[name][cat].Fill(branchesF[var]+idmvaShift, final_weight);
		else if (name == "idmvadown1" || name == "idmvadown2")  
 		  histos[samples[sampletype].second].histoF[name][cat].Fill(branchesF[var]-idmvaShift, final_weight);
		else if (name == "idmvatop1" || name == "idmvatop2")
		  {
		  if (idmvaCorrectionFile != NULL)
		    {

		      if(cat == 0 || cat == 1){ histos[samples[sampletype].second].histoF[name][cat].Fill(graphs[cat+2]->Eval(branchesF[var]), final_weight*graphs[cat+2]->Eval(9999));}
		    }
		  }
		else if (name == "idmvabottom1" || name == "idmvabottom2")
		  {
		    if (idmvaCorrectionFile != NULL)
		      {
			if(cat == 0 || cat == 1){ histos[samples[sampletype].second].histoF[name][cat].Fill(graphs[cat]->Eval(branchesF[var]), final_weight*graphs[cat]->Eval(9999));}
		      }

		  }
		else if (name == "sigmaEoEup1" || name == "sigmaEoEup2")
 		  histos[samples[sampletype].second].histoF[name][cat].Fill(branchesF[var]*(1+sigmaEScale), final_weight);
		else if (name == "sigmaEoEdown1" || name == "sigmaEoEdown2")  
 		  histos[samples[sampletype].second].histoF[name][cat].Fill(branchesF[var]*(1-sigmaEScale), final_weight);
		else if (branchesF.find(var) != branchesF.end())
		  histos[samples[sampletype].second].histoF[name][cat].Fill(branchesF[var], final_weight);
		else if (branchesI.find(var) != branchesI.end())
		  histos[samples[sampletype].second].histoF[name][cat].Fill(branchesI[var], final_weight);
	      }	    
	      //std::cout<< "histo " << name << " drawn" << std::endl;
	      //std::cout<< "Debug level 5 " << z << " " << s << " " << h << " " << " " << cat << std::endl; 
	    }
	    //std::cout << "Debug level 6" << std::endl;
	  }
	  //std::cout << "Debug level 7" << std::endl;
	  break;
	}
	//std::cout << "Debug level 8" << std::endl;	
      }
      //std::cout << "Debug level 9" << std::endl;
      if(sampletype==0 && z>=0) break ;
      //if(sampletype==1 && z>=10000) break ;
    }
    
    std::cout << sampletype << std::endl;
    std::string rootOutputFile = "out_data_tmp.root";  
    if (sampletype == 1)
      rootOutputFile = "out_zee.root";
    
    TFile* out = new TFile(rootOutputFile.c_str(), "recreate");
    
    for (unsigned int h=0; h<histoDef.size(); h++) {
      std::string var = histoDef[h].name;
      int cat = categories[histoDef[h].ncat].size();
      for (int c=0; c<cat; c++) {
	histos[samples[sampletype].second].histoF[var][c].Write();
      }
      //std::cout << "Wrote histo successfully" << var << std::endl; 
    }
    out->Close();
  }
}
void runAllPlotter()
{
  plotter_mc("/hadoop/cms/store/user/gkole/Hgg/Kuntal/Moriond17_AfterPreApproval/February2017_v3/Moriond17_March5/output_ReMiniAOD.root","/hadoop/cms/store/user/gkole/Hgg/Kuntal/Moriond17_AfterPreApproval/February2017_v3/Moriond17_March5/output_DYJesToLL_HCALDebug_v2.root","transformationIDMVA_v2.root");
}
