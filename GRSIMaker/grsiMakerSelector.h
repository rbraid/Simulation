//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Nov 21 15:52:39 2017 by ROOT version 5.34/36
// from TTree ryandata/More resonable data format
// found on file: ../EventOut.root
//////////////////////////////////////////////////////////

#ifndef grsiMakerSelector_h
#define grsiMakerSelector_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TH2.h>
#include <TStyle.h>
#include <iostream>
#include <TVector3.h>
#include <TTree.h>
#include <vector>
#include "/home/ryan/newinstalldir/GRSISort/include/TCSMHit.h"
#include "/home/ryan/newinstalldir/GRSISort/include/TCSM.h"


// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

TFile *OutputFile;

TTree *AnalysisTree;

TCSM *csm;

class grsiMakerSelector : public TSelector {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain

   // Declaration of leaf types
   Double_t        Energy[6][2];
   Int_t           XStrip[6][2];
   Int_t           YStrip[6][2];
   Int_t           Hits[6];
   Double_t        BeEnergyMath;
   Double_t        BeThetaMath;
   Double_t        Alpha1EnergyMath;
   Double_t        Alpha1ThetaMath;
   Double_t        Alpha2EnergyMath;
   Double_t        Alpha2ThetaMath;
   Double_t        Be8EnergyMath;
   Double_t        Be8ThetaMath;
   Double_t        BeThetaCOMMath;
   Double_t        Be8ThetaCOMMath;
   Double_t        BePhiMath;
   Double_t        Be8PhiMath;
   Double_t        Alpha1PhiMath;
   Double_t        Alpha2PhiMath;
   Double_t        BeExciteE;
   Double_t        Be8ExciteE;

   // List of branches
   TBranch        *b_Energy;   //!
   TBranch        *b_XStrip;   //!
   TBranch        *b_YStrip;   //!
   TBranch        *b_Hits;   //!
   TBranch        *b_BeEnergyMath;   //!
   TBranch        *b_BeThetaMath;   //!
   TBranch        *b_Alpha1EnergyMath;   //!
   TBranch        *b_Alpha1ThetaMath;   //!
   TBranch        *b_Alpha2EnergyMath;   //!
   TBranch        *b_Alpha2ThetaMath;   //!
   TBranch        *b_Be8EnergyMath;   //!
   TBranch        *b_Be8ThetaMath;   //!
   TBranch        *b_BeThetaCOMMath;   //!
   TBranch        *b_Be8ThetaCOMMath;   //!
   TBranch        *b_BePhiMath;   //!
   TBranch        *b_Be8PhiMath;   //!
   TBranch        *b_Alpha1PhiMath;   //!
   TBranch        *b_Alpha2PhiMath;   //!
   TBranch        *b_BeExciteE;   //!
   TBranch        *b_Be8ExciteE;   //!

   grsiMakerSelector(TTree * /*tree*/ =0) : fChain(0) { }
   virtual ~grsiMakerSelector() { }
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

   ClassDef(grsiMakerSelector,1);
};

#endif

#ifdef grsiMakerSelector_cxx
void grsiMakerSelector::Init(TTree *tree)
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
   fChain->SetBranchAddress("Alpha1EnergyMath", &Alpha1EnergyMath, &b_Alpha1EnergyMath);
   fChain->SetBranchAddress("Alpha1ThetaMath", &Alpha1ThetaMath, &b_Alpha1ThetaMath);
   fChain->SetBranchAddress("Alpha2EnergyMath", &Alpha2EnergyMath, &b_Alpha2EnergyMath);
   fChain->SetBranchAddress("Alpha2ThetaMath", &Alpha2ThetaMath, &b_Alpha2ThetaMath);
   fChain->SetBranchAddress("Be8EnergyMath", &Be8EnergyMath, &b_Be8EnergyMath);
   fChain->SetBranchAddress("Be8ThetaMath", &Be8ThetaMath, &b_Be8ThetaMath);
   fChain->SetBranchAddress("BeThetaCOMMath", &BeThetaCOMMath, &b_BeThetaCOMMath);
   fChain->SetBranchAddress("Be8ThetaCOMMath", &Be8ThetaCOMMath, &b_Be8ThetaCOMMath);
   fChain->SetBranchAddress("BePhiMath", &BePhiMath, &b_BePhiMath);
   fChain->SetBranchAddress("Be8PhiMath", &Be8PhiMath, &b_Be8PhiMath);
   fChain->SetBranchAddress("Alpha1PhiMath", &Alpha1PhiMath, &b_Alpha1PhiMath);
   fChain->SetBranchAddress("Alpha2PhiMath", &Alpha2PhiMath, &b_Alpha2PhiMath);
   fChain->SetBranchAddress("BeExciteE", &BeExciteE, &b_BeExciteE);
   fChain->SetBranchAddress("Be8ExciteE", &Be8ExciteE, &b_Be8ExciteE);
}

Bool_t grsiMakerSelector::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef grsiMakerSelector_cxx
