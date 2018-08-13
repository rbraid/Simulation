//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Apr  9 15:51:03 2014 by ROOT version 5.34/04
// from TTree particletree/Particles
// found on file: processed_event.root
//////////////////////////////////////////////////////////

#ifndef FinalSelector_h
#define FinalSelector_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>

#include <TH2D.h>
#include <TH2I.h>
#include <TCutG.h>

#include<iostream>
#include<iomanip>
#include<cmath>
#include<assert.h>
#include "/home/ryan/nuclear/be9/summed/include/GlobalSettings.hh"

void SetFinalHistos();
double GetExciteE_Heavy(double,double);
double GetExciteE_Light(double,double,double,double,double,double);
double checkKins(double,double,double,double,double,double,double,double,double);
double CorrectE_Alpha(double, double);
double CorrectE_Be(double, double);
double Calc8from12(double,double,double);
double* CalcBe8fromAlpha(double, double, double, double, double, double);


TH1I *MultiplicityPlot;
TH1D *ExcitePlot[3];
TFile *OutputFile;
TH2D *EvTheta;
TH2D *EvThetaMulti[3];
TH1D *ThetaPlots[3];
TH2D *AlphaEvTheta[2];
TH2D *BeEvTheta;
TH1D *CheckEx[2];
TH2D *CheckBe8;
TH2D *CheckAlpha;
TH2D *AlphaVAlpha;
TH2D *AlphaVAlphaMath;
TH1D *TotalEPlot;
// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class FinalSelector : public TSelector {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain

   // Declaration of leaf types
   Double_t        BeEnergy;
   Double_t        BeEnergyErr;
   Double_t        BeTheta;
   Double_t        BeThetaErr;
   Double_t        BePhi;
   Double_t        BePhiErr;
   Bool_t          BeBool;
   Double_t        AlphaEnergy[2];
   Double_t        AlphaEnergyErr[2];
   Double_t        AlphaTheta[2];
   Double_t        AlphaThetaErr[2];
   Double_t        AlphaPhi[2];
   Double_t        AlphaPhiErr[2];
   Bool_t          AlphaBool[2];
   Double_t 	   BeThetaMathNext;
   Double_t 	   BeEnergyMathNext;
   Double_t 	   Alpha1EnergyMathNext;
   Double_t 	   Alpha1ThetaMathNext;
   Double_t 	   Alpha2EnergyMathNext;
   Double_t 	   Alpha2ThetaMathNext;
   Double_t 	   Be8ThetaMathNext;
   Double_t 	   Be8EnergyMathNext;
   Double_t 	   BePhiMathNext;
   Double_t 	   Alpha1PhiMathNext;
   Double_t 	   Alpha2PhiMathNext;
   Double_t 	   Be8PhiMathNext;
   
   // List of branches
   TBranch        *b_BeEnergy;   //!
   TBranch        *b_BeEnergyErr;   //!
   TBranch        *b_BeTheta;   //!
   TBranch        *b_BeThetaErr;   //!
   TBranch        *b_BePhi;   //!
   TBranch        *b_BePhiErr;   //!
   TBranch        *b_BeBool;   //!
   TBranch        *b_AlphaEnergy;   //!
   TBranch        *b_AlphaEnergyErr;   //!
   TBranch        *b_AlphaTheta;   //!
   TBranch        *b_AlphaThetaErr;   //!
   TBranch        *b_AlphaPhi;   //!
   TBranch        *b_AlphaPhiErr;   //!
   TBranch        *b_AlphaBool;   //!
   TBranch 	  *b_BeThetaMathNext;   //!
   TBranch 	  *b_BeEnergyMathNext;   //!
   TBranch 	  *b_Alpha1EnergyMathNext;
   TBranch 	  *b_Alpha1ThetaMathNext;
   TBranch 	  *b_Alpha2EnergyMathNext;
   TBranch 	  *b_Alpha2ThetaMathNext;
   TBranch 	  *b_Be8ThetaMathNext;   //!
   TBranch 	  *b_Be8EnergyMathNext;   //!
   TBranch 	  *b_BePhiMathNext;   //!
   TBranch 	  *b_Alpha1PhiMathNext;
   TBranch 	  *b_Alpha2PhiMathNext;
   TBranch 	  *b_Be8PhiMathNext;   //!
   
   FinalSelector(TTree * /*tree*/ =0) : fChain(0) { }
   virtual ~FinalSelector() { }
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

   ClassDef(FinalSelector,0);
};

#endif

#ifdef FinalSelector_cxx
void FinalSelector::Init(TTree *tree)
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
   fChain->SetMakeClass(1);
   
   fChain->SetBranchAddress("BeEnergy", &BeEnergy, &b_BeEnergy);
   fChain->SetBranchAddress("BeEnergyErr", &BeEnergyErr, &b_BeEnergyErr);
   fChain->SetBranchAddress("BeTheta", &BeTheta, &b_BeTheta);
   fChain->SetBranchAddress("BeThetaErr", &BeThetaErr, &b_BeThetaErr);
   fChain->SetBranchAddress("BePhi", &BePhi, &b_BePhi);
   fChain->SetBranchAddress("BePhiErr", &BePhiErr, &b_BePhiErr);
   fChain->SetBranchAddress("BeBool", &BeBool, &b_BeBool);
   fChain->SetBranchAddress("AlphaEnergy", AlphaEnergy, &b_AlphaEnergy);
   fChain->SetBranchAddress("AlphaEnergyErr", AlphaEnergyErr, &b_AlphaEnergyErr);
   fChain->SetBranchAddress("AlphaTheta", AlphaTheta, &b_AlphaTheta);
   fChain->SetBranchAddress("AlphaThetaErr", AlphaThetaErr, &b_AlphaThetaErr);
   fChain->SetBranchAddress("AlphaPhi", AlphaPhi, &b_AlphaPhi);
   fChain->SetBranchAddress("AlphaPhiErr", AlphaPhiErr, &b_AlphaPhiErr);
   fChain->SetBranchAddress("AlphaBool", AlphaBool, &b_AlphaBool);
   fChain->SetBranchAddress("BeEnergyMathNext", &BeEnergyMathNext, &b_BeEnergyMathNext);
   fChain->SetBranchAddress("BeThetaMathNext", &BeThetaMathNext, &b_BeThetaMathNext);
   fChain->SetBranchAddress("Alpha1ThetaMathNext", &Alpha1ThetaMathNext, &b_Alpha1ThetaMathNext);
   fChain->SetBranchAddress("Alpha1EnergyMathNext", &Alpha1EnergyMathNext, &b_Alpha1EnergyMathNext);
   fChain->SetBranchAddress("Alpha2ThetaMathNext", &Alpha2ThetaMathNext, &b_Alpha2ThetaMathNext);
   fChain->SetBranchAddress("Alpha2EnergyMathNext", &Alpha2EnergyMathNext, &b_Alpha2EnergyMathNext);
   fChain->SetBranchAddress("Be8EnergyMathNext", &Be8EnergyMathNext, &b_Be8EnergyMathNext);
   fChain->SetBranchAddress("Be8ThetaMathNext", &Be8ThetaMathNext, &b_Be8ThetaMathNext);
   fChain->SetBranchAddress("BePhiMathNext", &BePhiMathNext, &b_BePhiMathNext);
   fChain->SetBranchAddress("Alpha1PhiMathNext", &Alpha1PhiMathNext, &b_Alpha1PhiMathNext);
   fChain->SetBranchAddress("Alpha2PhiMathNext", &Alpha2PhiMathNext, &b_Alpha2PhiMathNext);
   fChain->SetBranchAddress("Be8PhiMathNext", &Be8PhiMathNext, &b_Be8PhiMathNext);
   
}

Bool_t FinalSelector::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef FinalSelector_cxx
