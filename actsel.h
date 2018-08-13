//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Jul 10 16:39:14 2013 by ROOT version 5.34/04
// from TTree actualdata/Event action tree of real values
// found on file: EventOut.root
//////////////////////////////////////////////////////////

#ifndef actsel_h
#define actsel_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>

#include <TRandom.h>

#include <TGraph.h>
#include <TH2D.h>
#include <TH1D.h>
#include <TCutG.h>
#include <TH3D.h>
#include <TH1I.h>
//#include <THStack.h>

TRandom *randgen;

void SetHistos();
void SetCuts();
double *FindAltAngles(int, int, int);

TH1D *Eplot;
TH1D *dEplot;
TH1D *blurplot;
TH2D *EvTheta[6];
TH2D *EvThetaCut[2];
TH2D *builtEvTheta;
TH2D *dEvE;
TH2D *BADdEvE;
TH2D *badEvTheta[2];
TH2D *MaxAngle;
TH1D *AllTheta;

TH1D *ThetaPlot;
TH1I *SY;
TH1I *SX;
TH3D *XYZ;
TH2D *DetImage;
TH2I *HitPattern[6];
TH2D *cuttest;

//THStack *EvThetaOverlay;

TH1D *ThetaDiff;

TFile *OutputFile;

TList *Cuts;


class actsel : public TSelector {
public :

  
  TTree		*fChain;   //!pointer to the analyzed TTree or TChain

   // Declaration of leaf types


  Int_t		Hits;
  Double_t	DetectorX[10];
  Double_t	DetectorY[10];
  Double_t	DetectorZ[10];
  Int_t		DetectorID[10];
  Double_t	EnergyDep[10];
  Int_t		StripX[10];
  Int_t		StripY[10];
  Double_t	Blur[10];
  Double_t	Theta[10];
  Double_t	Phi[10];
  

   TBranch        *b_Hits;   //!
   TBranch        *b_mDetectorX;   //!
   TBranch        *b_mDetectorY;   //!
   TBranch        *b_mDetectorZ;   //!
   TBranch        *b_mDetectorID;   //!
   TBranch	*b_mEnergyDep;
   TBranch	*b_StripX;
   TBranch	*b_StripY;
   TBranch	*b_Blur;
   TBranch	*b_Theta;
   TBranch	*b_Phi;


   actsel(TTree * /*tree*/ =0) : fChain(0) { }
   virtual ~actsel() { }
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

   //TList *Cuts;
   TCutG *FindCut(const char* pName) { return (TCutG*)Cuts->FindObject(pName); }
   
   ClassDef(actsel,0);
};

#endif

#ifdef actsel_cxx
void actsel::Init(TTree *tree)
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

   fChain->SetBranchAddress("Hits", &Hits, &b_Hits);
   fChain->SetBranchAddress("DetectorX", DetectorX, &b_mDetectorX);
   fChain->SetBranchAddress("DetectorY", DetectorY, &b_mDetectorY);
   fChain->SetBranchAddress("DetectorZ", DetectorZ, &b_mDetectorZ);
   fChain->SetBranchAddress("DetectorID", DetectorID, &b_mDetectorID);
   fChain->SetBranchAddress("EnergyDep", EnergyDep, &b_mEnergyDep);
   fChain->SetBranchAddress("StripX", StripX, &b_StripX);
   fChain->SetBranchAddress("StripY", StripY, &b_StripY);
   fChain->SetBranchAddress("Blur",Blur,&b_Blur);
   fChain->SetBranchAddress("Theta",Theta,&b_Theta);
   fChain->SetBranchAddress("Phi",Phi,&b_Phi);
   
   
}

Bool_t actsel::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef actsel_cxx
