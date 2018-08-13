#define RyanSelector_cxx

#include "RyanSelector.h"
#include <TH2.h>
#include <TStyle.h>


void RyanSelector::Begin(TTree * /*tree*/)
{
  TString option = GetOption();
  
  OutputFile = new TFile("processed_event.root","recreate");

  SetHistos();
  SetCuts();
}

void RyanSelector::SlaveBegin(TTree * /*tree*/)
{
  TString option = GetOption();

}

Bool_t RyanSelector::Process(Long64_t entry)
{
  SetThetas();

  S_R_Theta[0] = thetaarray[0][S_R_Y[0]][S_R_X[0]];
  S_R_Theta[1] = thetaarray[0][S_R_Y[1]][S_R_X[1]];
  dE_R_Theta[0] = thetaarray[1][dE_R_Y[0]][dE_R_X[0]];
  dE_R_Theta[1] = thetaarray[1][dE_R_Y[1]][dE_R_X[1]];
  E_R_Theta[0] = thetaarray[2][E_R_Y[0]][E_R_X[0]];
  E_R_Theta[1] = thetaarray[2][E_R_Y[1]][E_R_X[1]];
  S_L_Theta[0] = thetaarray[3][S_L_Y[0]][S_L_X[0]];
  S_L_Theta[1] = thetaarray[3][S_L_Y[1]][S_L_X[1]];
  dE_L_Theta[0] = thetaarray[4][dE_L_Y[0]][dE_L_X[0]];
  dE_L_Theta[1] = thetaarray[4][dE_L_Y[1]][dE_L_X[1]];
  E_L_Theta[0] = thetaarray[5][E_L_Y[0]][E_L_X[0]];
  E_L_Theta[1] = thetaarray[5][E_L_Y[1]][E_L_X[1]];

  if(S_R_Hits>0)
  {
    EvTheta[0]->Fill(S_R_Theta[0],S_R_Energy[0]);
    if(S_R_Hits>1)
      EvTheta[1]->Fill(S_R_Theta[1],S_R_Energy[1]);
  }
    

  return kTRUE;
}

void RyanSelector::SlaveTerminate()
{

}

void RyanSelector::Terminate()
{
  OutputFile->Write();
  OutputFile->Close();
}

void SetCuts()
{
  TCutG *cut;
  Cuts = new TList();
  
  //gROOT->ProcessLine(".x cuts/alphas2.C");
  //cut = (TCutG*)gROOT->FindObjectAny("alphas");
  //Cuts->Add(cut);
  
  //(this->FindCut("alphas"))->IsInside(energy[2],energy[1])
}

void SetHistos()
{
  EvTheta[0] = new TH2D("EvTheta_rs", "Energy vs Theta Right Side",400,50,100,400,0,50);
  EvTheta[1] = new TH2D("EvTheta_rd", "Energy vs Theta Right dE",400,10,60,400,0,50);
  EvTheta[2] = new TH2D("EvTheta_re", "Energy vs Theta Right E",400,10,60,400,0,50);
  EvTheta[3] = new TH2D("EvTheta_ld", "Energy vs Theta Left dE",400,10,60,400,0,50);
  EvTheta[4] = new TH2D("EvTheta_le", "Energy vs Theta Left E",400,10,60,400,0,50);
  EvTheta[5] = new TH2D("EvTheta_ls", "Energy vs Theta Left Side",400,50,100,400,0,50);

  builtEvTheta = new TH2D("EvThetaFull", "Total Energy vs Theta",640,10,90,480,0,60);
    
  dEvE = new TH2D("dEvE", "dE vs E",880,0,55,880,0,55);
    
  EvThetaCut[0] = new TH2D("EvThetaCut_alpha", "Alpha Cut",110,0,55,110,0,55);
  EvThetaCut[1] = new TH2D("EvThetaCut_be12", "Be12 Cut",110,0,55,110,0,55);
  
  HitPattern[0] = new TH2I("HPrs", "Hit Pattern for Right Side Detector",16,0,16,16,0,16);
  HitPattern[1] = new TH2I("HPrd", "Hit Pattern for Right dE Detector",16,0,16,16,0,16);
  HitPattern[2] = new TH2I("HPre", "Hit Pattern for Right E Detector",16,0,16,16,0,16);
  HitPattern[3] = new TH2I("HPld", "Hit Pattern for Left dE Detector",16,0,16,16,0,16);
  HitPattern[4] = new TH2I("HPle", "Hit Pattern for Left E Detector",16,0,16,16,0,16);
  HitPattern[5] = new TH2I("HPls", "Hit Pattern for Left Side Detector",16,0,16,16,0,16);

  Multiplicity = new TH1I("Multi", "Multiplicity Plot",10,0,10);
}

void SetThetas()
{

}