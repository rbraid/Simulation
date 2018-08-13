//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Mar 28 15:50:16 2014 by ROOT version 5.34/04
// from TTree ryandata/More resonable data format
// found on file: EventOut.root
//////////////////////////////////////////////////////////

#ifndef NewRyanSelector_h
#define NewRyanSelector_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>

#include <TH2D.h>
#include <TH2I.h>
#include <TCutG.h>
#include <TH3D.h>
#include <TVector3.h>
#include <TMath.h>

#include<iostream>
#include<iomanip>
#include<cmath>

#include "angleheader.h"
#include "include/GlobalSettings.hh"

void SetHistos();
void SetCuts();
void SetParticleBranches();
void ResetTreeVars();
void SetAlpha(double,double,double);
void SetBe(double,double,double);
void PrintStatus();
double* findxyz(double, double, double);
double findtheta(double,double,double,double,double,double,int);
double findtheta_avg(double,double,double,double,double,double);
double toDegrees(double);
double toRadians(double);
double* CorrParticle(double, double, double);
double GetExciteE(double,double);

TFile *OutputFile;
TTree *ParticleTree;
TList *Cuts;

TH2D *EvTheta[6];
TH2D *EvThetaCut[2];
TH2D *builtEvTheta;
TH2D *MathEvTheta[4];
TH2D *dEvE;
TH2I *HitPattern[6];
TH2I *HitPatCut[2];
TH1I *HitPlot;
TH3D *XYZ;
TH1D *AlphaDiffMath[2];

TH1D *ExciteE;

TH2D *TvP[2];
TH2D *be8valphas;

TH2D *CorrParMath;
TH2D *CorrParMathCheck;
TH2D *AlphaAngDiff;

TH2D *cone;
TH1D *Etot;

TH2D *proj[2];

TH2D *angVang[3];

Double_t Theta[6][2];
Double_t Phi[6][2];
Double_t R[6][2];

Double_t BeEnergy;
Double_t BeEnergyErr;
Double_t BeTheta;
Double_t BeThetaErr;
Double_t BePhi;
Double_t BePhiErr;
Bool_t BeBool;

Double_t AlphaEnergy[2];
Double_t AlphaEnergyErr[2];
Double_t AlphaTheta[2];
Double_t AlphaThetaErr[2];
Double_t AlphaPhi[2];
Double_t AlphaPhiErr[2];
Bool_t AlphaBool[2];

Double_t 	   BeThetaMathNext;
Double_t	   BeThetaCOMMathNext;
Double_t 	   BeEnergyMathNext;
Double_t 	   Alpha1EnergyMathNext;
Double_t 	   Alpha1ThetaMathNext;
Double_t 	   Alpha2EnergyMathNext;
Double_t 	   Alpha2ThetaMathNext;
Double_t 	   Be8ThetaMathNext;
Double_t 	   Be8EnergyMathNext;
Double_t 	   Be8ThetaCOMMathNext;
Double_t 	   BePhiMathNext;
Double_t 	   Alpha1PhiMathNext;
Double_t 	   Alpha2PhiMathNext;
Double_t 	   Be8PhiMathNext;

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class NewRyanSelector : public TSelector {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain

   // Declaration of leaf types
   Double_t        Energy[6][2];
   Int_t           XStrip[6][2];
   Int_t           YStrip[6][2];
   Int_t           Hits[6];
   Double_t 	   BeThetaMath;
   Double_t 	   BeThetaCOMMath;
   Double_t 	   BeEnergyMath;
   Double_t 	   BeExciteE;
   Double_t 	   Alpha1EnergyMath;
   Double_t 	   Alpha1ThetaMath;
   Double_t 	   Alpha2EnergyMath;
   Double_t 	   Alpha2ThetaMath;
   Double_t 	   Be8ThetaMath;
   Double_t 	   Be8ThetaCOMMath;
   Double_t 	   Be8EnergyMath;
   Double_t 	   Be8ExciteE;
   Double_t 	   BePhiMath;
   Double_t 	   Alpha1PhiMath;
   Double_t 	   Alpha2PhiMath;
   Double_t 	   Be8PhiMath;
   
   // List of branches
   TBranch        *b_Energy;   //!
   TBranch        *b_XStrip;   //!
   TBranch        *b_YStrip;   //!
   TBranch        *b_Hits;   //!
   TBranch 	  *b_BeThetaMath;   //!
   TBranch 	  *b_BeEnergyMath;   //!
   TBranch 	  *b_BeExciteE;
   TBranch 	  *b_BeThetaCOMMath;
   TBranch 	  *b_Alpha1EnergyMath;
   TBranch 	  *b_Alpha1ThetaMath;
   TBranch 	  *b_Alpha2EnergyMath;
   TBranch 	  *b_Alpha2ThetaMath;
   TBranch 	  *b_Be8ThetaMath;   //!
   TBranch 	  *b_Be8EnergyMath;   //!
   TBranch 	  *b_Be8ExciteE;
   TBranch 	  *b_Be8ThetaCOMMath;
   TBranch 	  *b_BePhiMath;   //!
   TBranch 	  *b_Alpha1PhiMath;
   TBranch 	  *b_Alpha2PhiMath;
   TBranch 	  *b_Be8PhiMath;   //!
   
   NewRyanSelector(TTree * /*tree*/ =0) : fChain(0) { }
   virtual ~NewRyanSelector() { }
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
   
   TCutG *FindCut(const char* pName) { return (TCutG*)Cuts->FindObject(pName); }
   
   ClassDef(NewRyanSelector,0);
};

#endif

#ifdef NewRyanSelector_cxx
void NewRyanSelector::Init(TTree *tree)
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

   fChain->SetBranchAddress("Energy", Energy, &b_Energy);
   fChain->SetBranchAddress("XStrip", XStrip, &b_XStrip);
   fChain->SetBranchAddress("YStrip", YStrip, &b_YStrip);
   fChain->SetBranchAddress("Hits", Hits, &b_Hits);
   fChain->SetBranchAddress("BeEnergyMath", &BeEnergyMath, &b_BeEnergyMath);
   fChain->SetBranchAddress("BeThetaMath", &BeThetaMath, &b_BeThetaMath);
   fChain->SetBranchAddress("BeThetaCOMMath", &BeThetaCOMMath, &b_BeThetaCOMMath);
   fChain->SetBranchAddress("BeExciteE", &BeExciteE, &b_BeExciteE);
   fChain->SetBranchAddress("Alpha1ThetaMath", &Alpha1ThetaMath, &b_Alpha1ThetaMath);
   fChain->SetBranchAddress("Alpha1EnergyMath", &Alpha1EnergyMath, &b_Alpha1EnergyMath);
   fChain->SetBranchAddress("Alpha2ThetaMath", &Alpha2ThetaMath, &b_Alpha2ThetaMath);
   fChain->SetBranchAddress("Alpha2EnergyMath", &Alpha2EnergyMath, &b_Alpha2EnergyMath);
   fChain->SetBranchAddress("Be8EnergyMath", &Be8EnergyMath, &b_Be8EnergyMath);
   fChain->SetBranchAddress("Be8ExciteE", &Be8ExciteE, &b_Be8ExciteE);
   fChain->SetBranchAddress("Be8ThetaMath", &Be8ThetaMath, &b_Be8ThetaMath);
   fChain->SetBranchAddress("Be8ThetaCOMMath", &BeThetaCOMMath, &b_BeThetaCOMMath);
   fChain->SetBranchAddress("BePhiMath", &BePhiMath, &b_BePhiMath);
   fChain->SetBranchAddress("Alpha1PhiMath", &Alpha1PhiMath, &b_Alpha1PhiMath);
   fChain->SetBranchAddress("Alpha2PhiMath", &Alpha2PhiMath, &b_Alpha2PhiMath);
   fChain->SetBranchAddress("Be8PhiMath", &Be8PhiMath, &b_Be8PhiMath);
   
}

Bool_t NewRyanSelector::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef NewRyanSelector_cxx
