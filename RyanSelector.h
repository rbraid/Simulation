//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Mar 28 14:00:09 2014 by ROOT version 5.34/04
// from TTree ryandata/More resonable data format
// found on file: EventOut.root
//////////////////////////////////////////////////////////

#ifndef RyanSelector_h
#define RyanSelector_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>

#include <TH2D.h>
#include <TH2I.h>
#include <TCutG.h>


#include "/home/ryan/nuclear/be9/summed/angleheader.h"

void SetHistos();
void SetCuts();
void SetThetas();

TFile *OutputFile;
TList *Cuts;


TH2D *EvTheta[6];
TH2D *EvThetaCut[2];
TH2D *builtEvTheta;
TH2D *dEvE;
TH2I *HitPattern[6];
TH1I *Multiplicity;

Double_t S_R_Theta[2];
Double_t dE_R_Theta[2];
Double_t E_R_Theta[2];
Double_t S_L_Theta[2];
Double_t dE_L_Theta[2];
Double_t E_L_Theta[2];


// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class RyanSelector : public TSelector {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain

   // Declaration of leaf types
   Double_t        dE_R_Energy[2];
   Double_t        E_R_Energy[2];
   Double_t        S_R_Energy[2];
   Double_t        dE_L_Energy[2];
   Double_t        E_L_Energy[2];
   Double_t        S_L_Energy[2];
   Int_t           dE_R_X[2];
   Int_t           E_R_X[2];
   Int_t           S_R_X[2];
   Int_t           dE_L_X[2];
   Int_t           E_L_X[2];
   Int_t           S_L_X[2];
   Int_t           dE_R_Y[2];
   Int_t           E_R_Y[2];
   Int_t           S_R_Y[2];
   Int_t           dE_L_Y[2];
   Int_t           E_L_Y[2];
   Int_t           S_L_Y[2];
   Int_t           dE_R_Hits;
   Int_t           E_R_Hits;
   Int_t           S_R_Hits;
   Int_t           dE_L_Hits;
   Int_t           E_L_Hits;
   Int_t           S_L_Hits;

   // List of branches
   TBranch        *b_dE_R_Energy;   //!
   TBranch        *b_eE_R_Energy;   //!
   TBranch        *b_S_R_Energy;   //!
   TBranch        *b_dE_L_Energy;   //!
   TBranch        *b_E_L_Energy;   //!
   TBranch        *b_S_L_Energy;   //!
   TBranch        *b_dE_R_X;   //!
   TBranch        *b_eE_R_X;   //!
   TBranch        *b_S_R_X;   //!
   TBranch        *b_dE_L_X;   //!
   TBranch        *b_E_L_X;   //!
   TBranch        *b_S_L_X;   //!
   TBranch        *b_dE_R_Y;   //!
   TBranch        *b_eE_R_Y;   //!
   TBranch        *b_S_R_Y;   //!
   TBranch        *b_dE_L_Y;   //!
   TBranch        *b_E_L_Y;   //!
   TBranch        *b_S_L_Y;   //!
   TBranch        *b_dE_R_Hits;   //!
   TBranch        *b_eE_R_Hits;   //!
   TBranch        *b_S_R_Hits;   //!
   TBranch        *b_dE_L_Hits;   //!
   TBranch        *b_E_L_Hits;   //!
   TBranch        *b_S_L_Hits;   //!

   RyanSelector(TTree * /*tree*/ =0) : fChain(0) { }
   virtual ~RyanSelector() { }
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

   ClassDef(RyanSelector,0);
};

#endif

#ifdef RyanSelector_cxx
void RyanSelector::Init(TTree *tree)
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

   fChain->SetBranchAddress("dE_R_Energy", dE_R_Energy, &b_dE_R_Energy);
   fChain->SetBranchAddress("E_R_Energy", E_R_Energy, &b_eE_R_Energy);
   fChain->SetBranchAddress("S_R_Energy", S_R_Energy, &b_S_R_Energy);
   fChain->SetBranchAddress("dE_L_Energy", dE_L_Energy, &b_dE_L_Energy);
   fChain->SetBranchAddress("E_L_Energy", E_L_Energy, &b_E_L_Energy);
   fChain->SetBranchAddress("S_L_Energy", S_L_Energy, &b_S_L_Energy);
   fChain->SetBranchAddress("dE_R_X", dE_R_X, &b_dE_R_X);
   fChain->SetBranchAddress("E_R_X", E_R_X, &b_eE_R_X);
   fChain->SetBranchAddress("S_R_X", S_R_X, &b_S_R_X);
   fChain->SetBranchAddress("dE_L_X", dE_L_X, &b_dE_L_X);
   fChain->SetBranchAddress("E_L_X", E_L_X, &b_E_L_X);
   fChain->SetBranchAddress("S_L_X", S_L_X, &b_S_L_X);
   fChain->SetBranchAddress("dE_R_Y", dE_R_Y, &b_dE_R_Y);
   fChain->SetBranchAddress("E_R_Y", E_R_Y, &b_eE_R_Y);
   fChain->SetBranchAddress("S_R_Y", S_R_Y, &b_S_R_Y);
   fChain->SetBranchAddress("dE_L_Y", dE_L_Y, &b_dE_L_Y);
   fChain->SetBranchAddress("E_L_Y", E_L_Y, &b_E_L_Y);
   fChain->SetBranchAddress("S_L_Y", S_L_Y, &b_S_L_Y);
   fChain->SetBranchAddress("dE_R_Hits", &dE_R_Hits, &b_dE_R_Hits);
   fChain->SetBranchAddress("E_R_Hits", &E_R_Hits, &b_eE_R_Hits);
   fChain->SetBranchAddress("S_R_Hits", &S_R_Hits, &b_S_R_Hits);
   fChain->SetBranchAddress("dE_L_Hits", &dE_L_Hits, &b_dE_L_Hits);
   fChain->SetBranchAddress("E_L_Hits", &E_L_Hits, &b_E_L_Hits);
   fChain->SetBranchAddress("S_L_Hits", &S_L_Hits, &b_S_L_Hits);
}

Bool_t RyanSelector::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef RyanSelector_cxx
