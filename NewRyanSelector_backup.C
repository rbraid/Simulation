#define NewRyanSelector_cxx

#include "NewRyanSelector.h"
#include <TH2.h>
#include <TStyle.h>

using namespace std;

void NewRyanSelector::Begin(TTree * /*tree*/)
{

  TString option = GetOption();

  OutputFile = new TFile("processed_event.root","recreate");
  //OutputTreeFile = new TFile("particle_out.root","recreate");
  ParticleTree = new TTree("particletree","Particles");

  SetParticleBranches();
  
  SetHistos();
  SetCuts();

  cout<<"Begin"<<endl;
}

void NewRyanSelector::SlaveBegin(TTree * /*tree*/)
{

  TString option = GetOption();

  cout<<"Slave Begin"<<endl;
  
}

Bool_t NewRyanSelector::Process(Long64_t entry)
{
  fChain->GetTree()->GetEntry(entry);
  
  ResetTreeVars();

  MathEvTheta[0]->Fill(BeThetaMath,BeEnergyMath);
  MathEvTheta[1]->Fill(Alpha1ThetaMath,Alpha1EnergyMath);
  MathEvTheta[2]->Fill(Be8ThetaMath,Be8EnergyMath);
  if(BeThetaMath > 10. && Be8ThetaMath > 10. &&
      BeThetaMath < 52 && Be8ThetaMath < 52)
    MathEvTheta[3]->Fill(BeThetaMath,BeEnergyMath);
  else if(BeThetaMath > 68 &&
            Be8ThetaMath > 10 && Be8ThetaMath < 52)
    MathEvTheta[3]->Fill(BeThetaMath,BeEnergyMath);
  else if(Be8ThetaMath > 68 &&
            BeThetaMath > 10 && BeThetaMath < 52)
    MathEvTheta[3]->Fill(BeThetaMath,BeEnergyMath);

    TVector3 A1(1,0,0);
    //A1.SetMag(Alpha1EnergyMath);
    A1.SetTheta(toRadians(Alpha1ThetaMath));
    A1.SetPhi(toRadians(Alpha1PhiMath));

    TVector3 A2(1,0,0);
    //A2.SetMag(Alpha2EnergyMath);
    A2.SetTheta(toRadians(Alpha2ThetaMath));
    A2.SetPhi(toRadians(Alpha2PhiMath));

    AlphaAngDiff->Fill(A1.Angle(A2)*180/3.14159,Alpha1EnergyMath+Alpha2EnergyMath);

    cone->Fill(Alpha1EnergyMath,Alpha2EnergyMath);

  Etot->Fill(BeEnergyMath+Alpha1EnergyMath+Alpha2EnergyMath+BeExciteE);

  TvP[0]->Fill(BeThetaMath,BePhiMath);
  TvP[1]->Fill(Be8ThetaMath,Be8PhiMath);

  be8valphas->Fill(Be8EnergyMath,Alpha1EnergyMath+Alpha2EnergyMath);
    
  double* CPMV;
  CPMV = CorrParticle(BeEnergyMath, toRadians(BeThetaMath), toRadians(BePhiMath));
  CorrParMath->Fill(CPMV[1]*180/3.14,CPMV[0]);

  ExciteE->Fill(GetExciteE(BeThetaMath,BeEnergyMath));
  
  angVang[0]->Fill(Be8ThetaMath,BeThetaMath);
  
    angVang[1]->Fill(Be8ThetaMath,BeThetaMath);
  
    if( (Be8ThetaMath>10 && Be8ThetaMath<55) &&
    (BeThetaMath>10 && BeThetaMath<55) )
      angVang[2]->Fill(Be8ThetaMath,BeThetaMath);
      
    else if( (Be8ThetaMath>57 ) &&
    (BeThetaMath>10 && BeThetaMath<55) )
      angVang[2]->Fill(Be8ThetaMath,BeThetaMath);

    else if( (BeThetaMath>57) &&
    (Be8ThetaMath>10 && Be8ThetaMath<55) )
      angVang[2]->Fill(Be8ThetaMath,BeThetaMath);

    else if(Be8ThetaMath>57 && BeThetaMath>57)
      angVang[2]->Fill(Be8ThetaMath,BeThetaMath);

  BeThetaMathNext=BeThetaMath;
  BeThetaCOMMathNext=BeThetaCOMMath;
  BeEnergyMathNext=BeEnergyMath;
  Alpha1EnergyMathNext=Alpha1EnergyMath;
  Alpha1ThetaMathNext=Alpha1ThetaMath;
  Alpha2EnergyMathNext=Alpha2EnergyMath;
  Alpha2ThetaMathNext=Alpha2ThetaMath;
  Be8ThetaMathNext=Be8ThetaMath;
  Be8ThetaCOMMathNext=Be8ThetaCOMMath;
  Be8EnergyMathNext=Be8EnergyMath;
  BePhiMathNext=BePhiMath;
  Alpha1PhiMathNext=Alpha1PhiMath;
  Alpha2PhiMathNext=Alpha2PhiMath;
  Be8PhiMathNext=Be8PhiMath;

  //cout<<Alpha1ThetaMath<<" "<<Alpha2ThetaMath<<" "<<Alpha1ThetaMath-Alpha2ThetaMath<<endl;
  AlphaDiffMath[0]->Fill(Alpha1ThetaMath-Alpha2ThetaMath);
  //if( (this->FindCut("baddEvE"))->IsInside(Energy[2][0],Energy[1][0]) || (this->FindCut("baddEvE"))->IsInside(Energy[4][0],Energy[3][0]) )
    //AlphaDiffMath[1]->Fill(Alpha1ThetaMath-Alpha2ThetaMath);
  
  for(int detector=0;detector<6;detector++)//This loop sets the Angles
  {    
    if(XStrip[detector][0]!=-10&&YStrip[detector][0]!=-10)
    {
      Theta[detector][0] = thetaarray[detector][YStrip[detector][0]][XStrip[detector][0]];
      Phi[detector][0] = phiarray[detector][YStrip[detector][0]][XStrip[detector][0]];
      R[detector][0] = rarray[detector][YStrip[detector][0]][XStrip[detector][0]];
    }
  
    else
    {
      Theta[detector][0] = 0.;
      Phi[detector][0] = 0.;
      R[detector][0] = 0.;
    }
    
    if(XStrip[detector][1]!=-10&&YStrip[detector][1]!=-10)
    {
      Theta[detector][1] = thetaarray[detector][YStrip[detector][1]][XStrip[detector][1]];
      Phi[detector][1] = phiarray[detector][YStrip[detector][1]][XStrip[detector][1]];
      R[detector][1] = rarray[detector][YStrip[detector][1]][XStrip[detector][1]];
    }
    else
    {
      Theta[detector][1] = 0.;
      Phi[detector][1] = 0.;
      R[detector][1] = 0.;
    }
  }

  if(abs(Theta[1][0]-Theta[2][1])<1.||abs(Theta[1][1]-Theta[2][0])<1.)
  {
    double temp = Theta[2][0];
    Theta[2][0] = Theta[2][1];
    Theta[2][1] = temp;

    temp = Phi[2][0];
    Phi[2][0] = Phi[2][1];
    Phi[2][1] = temp;
    
    temp = Energy[2][0];
    Energy[2][0] = Energy[2][1];
    Energy[2][1] = temp;
  }
  if(abs(Theta[3][0]-Theta[4][1])<1.||abs(Theta[3][1]-Theta[4][0])<1.)
  {
    double temp = Theta[4][0];
    Theta[4][0] = Theta[4][1];
    Theta[4][1] = temp;

    temp = Phi[4][0];
    Phi[4][0] = Phi[4][1];
    Phi[4][1] = temp;
    
    temp = Energy[4][0];
    Energy[4][0] = Energy[4][1];
    Energy[4][1] = temp;
  }
  
  for(int detector=0;detector<6;detector++)//This loop does EvTheta and Hit pattern for each detector
  {
    if(Hits[detector]>0)
    {
      EvTheta[detector]->Fill(Theta[detector][0],Energy[detector][0]);
      HitPattern[detector]->Fill(XStrip[detector][0],YStrip[detector][0]);
      if(Hits[detector]>1)
      {
        EvTheta[detector]->Fill(Theta[detector][1],Energy[detector][1]);
        HitPattern[detector]->Fill(XStrip[detector][1],YStrip[detector][1]);
      }
      
    }
    
  }

  for(int j=0;j<2;j++)//I fill the cut hit patterns
  {
    if(XStrip[1][j]==14&&YStrip[1][j]==8)
    {
      HitPatCut[0]->Fill(XStrip[2][j],YStrip[2][j]);
    }
    if(XStrip[3][j]==14&&YStrip[3][j]==2)
    {
      HitPatCut[1]->Fill(XStrip[4][j],YStrip[4][j]);
    }
  }

  if(Hits[1]>0&&Hits[2]==0)
  {
    builtEvTheta->Fill(Theta[1][0],Energy[1][0]);
    if(Hits[1]>1)
      builtEvTheta->Fill(Theta[1][1],Energy[1][1]);
  }
  else if(Hits[1]>0&&Hits[2]>0)
  {
    dEvE->Fill(Energy[2][0],Energy[1][0]);
    if( (this->FindCut("baddEvE"))->IsInside(Energy[2][0],Energy[1][0]))
      AlphaDiffMath[1]->Fill(Alpha1ThetaMath-Alpha2ThetaMath);
    builtEvTheta->Fill((Theta[1][0]+Theta[2][0])/2.,Energy[1][0]+Energy[2][0]);
    if(Hits[1]>1&&Hits[2]>1)
    {
      dEvE->Fill(Energy[2][1],Energy[1][1]);
      if( (this->FindCut("baddEvE"))->IsInside(Energy[2][1],Energy[1][1]))
	AlphaDiffMath[1]->Fill(Alpha1ThetaMath-Alpha2ThetaMath);
      builtEvTheta->Fill((Theta[1][1]+Theta[2][1])/2.,Energy[1][1]+Energy[2][1]);
    }
  }
  
  if(Hits[3]>0&&Hits[4]==0)
  {
    builtEvTheta->Fill(Theta[3][0],Energy[3][0]);
    if(Hits[3]>1)
      builtEvTheta->Fill(Theta[3][1],Energy[3][1]);
  }
  else if(Hits[3]>0&&Hits[4]>0)
  {
    dEvE->Fill(Energy[4][0],Energy[3][0]);
    if( (this->FindCut("baddEvE"))->IsInside(Energy[4][0],Energy[3][0]))
      AlphaDiffMath[1]->Fill(Alpha1ThetaMath-Alpha2ThetaMath);
    builtEvTheta->Fill((Theta[3][0]+Theta[4][0])/2.,Energy[3][0]+Energy[4][0]);
    if(Hits[3]>1&&Hits[4]>1)
    {
      dEvE->Fill(Energy[4][1],Energy[3][1]);
      if( (this->FindCut("baddEvE"))->IsInside(Energy[4][1],Energy[3][1]))
	AlphaDiffMath[1]->Fill(Alpha1ThetaMath-Alpha2ThetaMath);
      builtEvTheta->Fill((Theta[3][1]+Theta[4][1])/2.,Energy[3][1]+Energy[4][1]);
    }
  }

  if(Hits[0]>0)
  {
    builtEvTheta->Fill(Theta[0][0],Energy[0][0]);
    if(Hits[0]>1)
      builtEvTheta->Fill(Theta[0][1],Energy[0][1]);
  }
  
  if(Hits[5]>0)
  {
    builtEvTheta->Fill(Theta[5][0],Energy[5][0]);
    if(Hits[5]>1)
      builtEvTheta->Fill(Theta[5][1],Energy[5][1]);
  }

  //if((this->FindCut("AlphaCut"))->IsInside(Theta[detector][0],Energy[detector][0]))

    if((this->FindCut("AlphaCut"))->IsInside(Theta[0][0],Energy[0][0]))
    {
      SetAlpha(Theta[0][0],Energy[0][0],Phi[0][0]);
    }

    if((this->FindCut("AlphaCut"))->IsInside(Theta[1][0],Energy[1][0]))
    {
      SetAlpha(Theta[1][0],Energy[1][0],Phi[1][0]);
    }
    
    if((this->FindCut("AlphaCut"))->IsInside(Theta[1][0],Energy[1][0]+Energy[2][0])&&(this->FindCut("dEvE_Alpha"))->IsInside(Energy[2][0],Energy[1][0]))
    {
      SetAlpha((Theta[1][0]+Theta[2][0])/2,Energy[1][0]+Energy[2][0],Phi[1][0]);
    }
    
    if((this->FindCut("AlphaCut"))->IsInside(Theta[5][0],Energy[5][0]))
    {
      SetAlpha(Theta[5][0],Energy[5][0],Phi[5][0]);
    }
    
    if((this->FindCut("AlphaCut"))->IsInside(Theta[3][0],Energy[3][0]))
    {
      SetAlpha(Theta[3][0],Energy[3][0],Phi[3][0]);
    }
    
    if((this->FindCut("AlphaCut"))->IsInside(Theta[3][0],Energy[3][0]+Energy[4][0])&&(this->FindCut("dEvE_Alpha"))->IsInside(Energy[4][0],Energy[3][0]))
    {
      SetAlpha((Theta[3][0]+Theta[4][0])/2.,Energy[3][0]+Energy[4][0],Phi[4][0]);
    }
    
    if((this->FindCut("AlphaCut"))->IsInside(Theta[0][1],Energy[0][1]))
    {
      SetAlpha(Theta[0][1],Energy[0][1],Phi[0][1]);
    }
    
    if((this->FindCut("AlphaCut"))->IsInside(Theta[1][1],Energy[1][1]))
    {
      SetAlpha(Theta[1][1],Energy[1][1],Phi[1][1]);
    }
    
    if((this->FindCut("AlphaCut"))->IsInside(Theta[1][1],Energy[1][1]+Energy[2][1])&&(this->FindCut("dEvE_Alpha"))->IsInside(Energy[2][1],Energy[1][1]))
    {
      SetAlpha((Theta[1][1]+Theta[2][1])/2.,Energy[1][1]+Energy[2][1],Phi[1][1]);
    }
    
    if((this->FindCut("AlphaCut"))->IsInside(Theta[5][1],Energy[5][1]))
    {
      SetAlpha(Theta[5][1],Energy[5][1],Phi[5][1]);
    }
    
    if((this->FindCut("AlphaCut"))->IsInside(Theta[3][1],Energy[3][1]))
    {
      SetAlpha(Theta[3][1],Energy[3][1],Phi[3][1]);
    }
    
    if((this->FindCut("AlphaCut"))->IsInside(Theta[3][1],Energy[3][1]+Energy[4][1])&&(this->FindCut("dEvE_Alpha"))->IsInside(Energy[4][1],Energy[3][1]))
    {
      SetAlpha((Theta[3][1]+Theta[4][1])/2.,Energy[3][1]+Energy[4][1],Phi[3][1]);
    }
//*******************************************
    double th =0.;
    
    if((this->FindCut("Be12Cut"))->IsInside(Theta[0][0],Energy[0][0]))
    {
      SetBe(Theta[0][0],Energy[0][0],Phi[0][0]);
    }

    else if((this->FindCut("Be12Cut"))->IsInside(Theta[1][0],Energy[1][0]))
    {
      SetBe(Theta[1][0],Energy[1][0],Phi[1][0]);
    }
    
    else if(((this->FindCut("Be12Cut"))->IsInside(Theta[1][0],Energy[1][0]+Energy[2][0])||(this->FindCut("Be12Cut"))->IsInside(Theta[2][0],Energy[1][0]+Energy[2][0]))&&(this->FindCut("dEvE_Be"))->IsInside(Energy[2][0],Energy[1][0]))
    {
      if(FANCYTHETA)
	th = findtheta(Theta[1][0],Phi[1][0],R[1][0],Theta[2][0],Phi[2][0],R[2][0],1);
      else
	th = Theta[1][0];
      SetBe(th,Energy[1][0]+Energy[2][0],Phi[1][0]);
    }
    
    else if((this->FindCut("Be12Cut"))->IsInside(Theta[5][0],Energy[5][0]))
    {
      SetBe(Theta[5][0],Energy[5][0],Phi[5][0]);
    }
    
    else if((this->FindCut("Be12Cut"))->IsInside(Theta[3][0],Energy[3][0]))
    {
      SetBe(Theta[3][0],Energy[3][0],Phi[3][0]);
    }
    
    else if(((this->FindCut("Be12Cut"))->IsInside(Theta[3][0],Energy[3][0]+Energy[4][0])||(this->FindCut("Be12Cut"))->IsInside(Theta[4][0],Energy[3][0]+Energy[4][0]))&&(this->FindCut("dEvE_Be"))->IsInside(Energy[4][0],Energy[3][0]))
    {
      if(FANCYTHETA)
	th = findtheta(Theta[3][0],Phi[3][0],R[3][0],Theta[4][0],Phi[4][0],R[4][0],3);
      else
	th = Theta[3][0];
      SetBe(th,Energy[3][0]+Energy[4][0],Phi[3][0]);
    }
    
    else if((this->FindCut("Be12Cut"))->IsInside(Theta[0][1],Energy[0][1]))
    {
      SetBe(Theta[0][1],Energy[0][1],Phi[0][1]);
    }
    
    else if((this->FindCut("Be12Cut"))->IsInside(Theta[1][1],Energy[1][1]))
    {
      SetBe(Theta[1][1],Energy[1][1],Phi[1][1]);
    }
    
    else if(((this->FindCut("Be12Cut"))->IsInside(Theta[1][1],Energy[1][1]+Energy[2][1])||(this->FindCut("Be12Cut"))->IsInside(Theta[2][1],Energy[1][1]+Energy[2][1]))&&(this->FindCut("dEvE_Be"))->IsInside(Energy[2][1],Energy[1][1]))
    {
      if(FANCYTHETA)
	th = findtheta(Theta[1][1],Phi[1][1],R[1][1],Theta[2][1],Phi[2][1],R[2][1],1);
      else
	th = Theta[1][1];
      SetBe(th,Energy[1][1]+Energy[2][1],Phi[1][1]);
      
    }
    
    else if((this->FindCut("Be12Cut"))->IsInside(Theta[5][1],Energy[5][1]))
    {
      SetBe(Theta[5][1],Energy[5][1],Phi[5][1]);
    }
    
    else if((this->FindCut("Be12Cut"))->IsInside(Theta[3][1],Energy[3][1]))
    {
      SetBe(Theta[3][1],Energy[3][1],Phi[3][1]);
    }
    
    else if(((this->FindCut("Be12Cut"))->IsInside(Theta[3][1],Energy[3][1]+Energy[4][1])||(this->FindCut("Be12Cut"))->IsInside(Theta[4][1],Energy[3][1]+Energy[4][1]))&&(this->FindCut("dEvE_Be"))->IsInside(Energy[4][1],Energy[3][1]))
    {
      if(FANCYTHETA)
	th = findtheta(Theta[3][1],Phi[3][1],R[3][1],Theta[4][1],Phi[4][1],R[4][1],3);
      else
	th = Theta[3][1];
      SetBe(th,Energy[3][1]+Energy[4][1],Phi[3][1]);
    }
    
      
    /*else if(AlphaBool[0]&&AlphaBool[1])
    {

      for(int detector = 0;detector<6;detector++)
      {
	for(int hit = 0;hit<2;hit++)
	{
	  if(Theta[detector][hit]!=AlphaTheta[0]&&Theta[detector][hit]!=AlphaTheta[1]&&Theta[detector][hit]!=0.&&Energy[detector][hit]>.01)
	  {
	    BeTheta = Theta[detector][hit];
	    BeEnergy = Energy[detector][hit];
	    cout<<".";
	    if(abs(Theta[detector][hit]-Theta[detector+1][hit])<1.)
	    {
	      BeEnergy += Energy[detector][hit];
	      cout<<":";
	    }
	  }
	}
      }
    }*/

    
    if(BeBool&&AlphaBool[1]&&REJECTMISIDENTIFY)
    {
      double angthresh = 4.;
      if(abs(AlphaTheta[0]-AlphaTheta[1])<angthresh);
      else if(abs(BeTheta-AlphaTheta[0])<angthresh)
      {
	/*double temp[2] = {0};
	 * temp[0] = BeEnergy;
	 * temp[1] = BeTheta;
	 *
	 * BeEnergy = AlphaEnergy[1];
	 * BeTheta = AlphaTheta[1];
	 *
	 * AlphaTheta[1] = temp[1];
	 * AlphaEnergy[1] = temp[0];*/

	BeBool=0;
	AlphaBool[0]=0;
	AlphaBool[1]=0;

	//cout<<"0";
      }
      else if(abs(BeTheta-AlphaTheta[1])<angthresh)
      {
	/*double temp[2] = {0};
	 * temp[0] = BeEnergy;
	 * temp[1] = BeTheta;
	 *
	 * BeEnergy = AlphaEnergy[0];
	 * BeTheta = AlphaTheta[0];
	 *
	 * AlphaTheta[0] = temp[1];
	 * AlphaEnergy[0] = temp[0];*/

	BeBool=0;
	AlphaBool[0]=0;
	AlphaBool[1]=0;

	//cout<<"1";
      }
    }
    
  HitPlot->Fill(Hits[0]+Hits[1]+Hits[2]+Hits[3]+Hits[4]+Hits[5]);


  if(BeBool||AlphaBool[0])
  {
    ParticleTree->Fill();
    //PrintStatus();
  }
  return kTRUE;
}

void NewRyanSelector::SlaveTerminate()
{
  cout<<"Slave Terminate"<<endl;
  
}

void NewRyanSelector::Terminate()
{
  OutputFile->Write();
  OutputFile->Close();
  cout<<"Terminate"<<endl;
  
}

void SetCuts()
{
  TCutG *cut;
  Cuts = new TList();
  
  gROOT->ProcessLine(".x cuts/Be12Cut3.C");
  cut = (TCutG*)gROOT->FindObjectAny("Be12Cut");
  Cuts->Add(cut);
  gROOT->ProcessLine(".x cuts/AlphaCut3.C");
  cut = (TCutG*)gROOT->FindObjectAny("AlphaCut");
  Cuts->Add(cut);
  
  gROOT->ProcessLine(".x cuts/dEvE_Be2.C");
  cut = (TCutG*)gROOT->FindObjectAny("dEvE_Be");
  Cuts->Add(cut);
  gROOT->ProcessLine(".x cuts/dEvE_Alpha3.C");
  cut = (TCutG*)gROOT->FindObjectAny("dEvE_Alpha");
  Cuts->Add(cut);

  gROOT->ProcessLine(".x cuts/baddEvE.C");
  cut = (TCutG*)gROOT->FindObjectAny("baddEvE");
  Cuts->Add(cut);
  
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

  TvP[0] = new TH2D("TvP12","Theta vs Phi for 12Be",720,-360,360,720,-360,360);
  TvP[1] = new TH2D("TvP8","Theta vs Phi for 8Be",720,-360,360,720,-360,360);

  be8valphas = new TH2D("be8valphas","be8valphas",110,-10,100,110,-10,100);
  
  AlphaAngDiff = new TH2D("AlphaAngDiff","Difference in Angle vs Sum of Energy of 2 alphas",1440,-360,360,280,0,70);
  
  builtEvTheta = new TH2D("EvThetaFull", "Total Energy vs Theta",720,0,90,480,0,60);
  MathEvTheta[0] = new TH2D("MathEvTheta_Be12", "Total Energy vs Theta",720,0,90,480,0,60);
  MathEvTheta[1] = new TH2D("MathEvTheta_Alpha", "Total Energy vs Theta",720,0,90,480,0,60);
  MathEvTheta[2] = new TH2D("MathEvTheta_Be8", "Total Energy vs Theta",720,0,90,480,0,60);
  MathEvTheta[3] = new TH2D("MathEvTheta_det", "Total Energy vs Theta cut on detector positions",720,0,90,480,0,60);

  cone = new TH2D("cone","cone",260,0,65,260,0,65);

  XYZ = new TH3D("3Dplot", "Raw Positions", 400,-100,100,120,-30,30,170,0,85);
  XYZ->GetXaxis()->SetTitle("X");
  XYZ->GetYaxis()->SetTitle("Y");
  XYZ->GetZaxis()->SetTitle("Z");

  proj[0] = new TH2D("proj_0","Raw",400,-100,100,120,-30,30);
  proj[1] = new TH2D("proj_1","Raw",400,-100,100,120,-30,30);

  CorrParMath = new TH2D("CPM","CPM",720,0,360,400,0,100);
  CorrParMathCheck = new TH2D("CPMC","CPMC",720,0,360,400,0,100);
  
  dEvE = new TH2D("dEvE", "dE vs E",520,0,65,520,0,65);

  Etot = new TH1D("Etot", "Total Energy",40,45,65);

  ExciteE = new TH1D("ExciteE","Calculated Excite Energy",1100,-10,100);
  
  EvThetaCut[0] = new TH2D("EvThetaCut_alpha", "Alpha Cut",180,0,90,110,0,55);
  EvThetaCut[1] = new TH2D("EvThetaCut_be12", "Be12 Cut",110,0,55,110,0,55);
  
  HitPattern[0] = new TH2I("HPrs", "Hit Pattern for Right Side Detector",16,0,16,16,0,16);
  HitPattern[1] = new TH2I("HPrd", "Hit Pattern for Right dE Detector",16,0,16,16,0,16);
  HitPattern[2] = new TH2I("HPre", "Hit Pattern for Right E Detector",16,0,16,16,0,16);
  HitPattern[3] = new TH2I("HPld", "Hit Pattern for Left dE Detector",16,0,16,16,0,16);
  HitPattern[4] = new TH2I("HPle", "Hit Pattern for Left E Detector",16,0,16,16,0,16);
  HitPattern[5] = new TH2I("HPls", "Hit Pattern for Left Side Detector",16,0,16,16,0,16);

  angVang[0] = new TH2D("angle_vs_angle_all","Angle of Be8 vs Angle of Be12 all",300,0,150,300,0,150);
  angVang[1] = new TH2D("angle_vs_angle_or","Angle of Be8 vs Angle of Be12 or",300,0,150,300,0,150);
  angVang[2] = new TH2D("angle_vs_angle_and","Angle of Be8 vs Angle of Be12 and",300,0,150,300,0,150);
  

  HitPatCut[0] = new TH2I("HPC_0", "Hit Pattern Cut",16,0,16,16,0,16);
  HitPatCut[1] = new TH2I("HPC_1", "Hit Pattern Cut",16,0,16,16,0,16);

  AlphaDiffMath[0] = new TH1D("AlphaDiffMath","AlphaDiffMath",1000,-100,100);
  AlphaDiffMath[1] = new TH1D("AlphaDiffMath_cut","AlphaDiffMath cut on strange dEvE",1000,-100,100);
  
  HitPlot = new TH1I("HitPlot","Hit",15,0,15);
}

void SetParticleBranches()
{
  ParticleTree->Branch("BeEnergy",&(BeEnergy),"BeEnergy/D");
  ParticleTree->Branch("BeEnergyErr",&(BeEnergyErr),"BeEnergyErr/D");
  ParticleTree->Branch("BeTheta",&(BeTheta),"BeTheta/D");
  ParticleTree->Branch("BeThetaErr",&(BeThetaErr),"BeThetaErr/D");
  ParticleTree->Branch("BePhi",&(BePhi),"BePhi/D");
  ParticleTree->Branch("BePhiErr",&(BePhiErr),"BePhiErr/D");
  ParticleTree->Branch("BeBool",&(BeBool),"BeBool/O");

  ParticleTree->Branch("AlphaEnergy",&(AlphaEnergy),"AlphaEnergy[2]/D");
  ParticleTree->Branch("AlphaEnergyErr",&(AlphaEnergyErr),"AlphaEnergyErr[2]/D");
  ParticleTree->Branch("AlphaTheta",&(AlphaTheta),"AlphaTheta[2]/D");
  ParticleTree->Branch("AlphaThetaErr",&(AlphaThetaErr),"AlphaThetaErr[2]/D");
  ParticleTree->Branch("AlphaPhi",&(AlphaPhi),"AlphaPhi[2]/D");
  ParticleTree->Branch("AlphaPhiErr",&(AlphaPhiErr),"AlphaPhiErr[2]/D");
  ParticleTree->Branch("AlphaBool",&(AlphaBool),"AlphaBool[2]/O");

  ParticleTree->Branch("BeEnergyMathNext",&(BeEnergyMathNext));
  ParticleTree->Branch("BeThetaMathNext",&(BeThetaMathNext));
  ParticleTree->Branch("Alpha1EnergyMathNext",&(Alpha1EnergyMathNext));
  ParticleTree->Branch("Alpha1ThetaMathNext",&(Alpha1ThetaMathNext));
  ParticleTree->Branch("Alpha2EnergyMathNext",&(Alpha2EnergyMathNext));
  ParticleTree->Branch("Alpha2ThetaMathNext",&(Alpha2ThetaMathNext));
  ParticleTree->Branch("Be8EnergyMathNext",&(Be8EnergyMathNext));
  ParticleTree->Branch("Be8ThetaMathNext",&(Be8ThetaMathNext));
  ParticleTree->Branch("BeThetaCOMMathNext",&(BeThetaCOMMathNext));
  ParticleTree->Branch("Be8ThetaCOMMathNext",&(Be8ThetaCOMMathNext));
  ParticleTree->Branch("BePhiMathNext",&(BePhiMathNext));
  ParticleTree->Branch("Alpha1PhiMathNext",&(Alpha1PhiMathNext));
  ParticleTree->Branch("Alpha2PhiMathNext",&(Alpha2PhiMathNext));
  ParticleTree->Branch("Be8PhiMathNext",&(Be8PhiMathNext));
  
}

void ResetTreeVars()
{
  BeEnergy = 0.0;
  BeEnergyErr = 0.0;
  BeTheta = 0.0;
  BeThetaErr = 0.0;
  BePhi = 0.0;
  BePhiErr = 0.0;
  BeBool = 0;

  AlphaEnergy[0] = 0.0;
  AlphaEnergyErr[0] = 0.0;
  AlphaTheta[0] = 0.0;
  AlphaThetaErr[0] = 0.0;
  AlphaPhi[0] = 0.0;
  AlphaPhiErr[0] = 0.0;
  AlphaBool[0] = 0;

  AlphaEnergy[1] = 0.0;
  AlphaEnergyErr[1] = 0.0;
  AlphaTheta[1] = 0.0;
  AlphaThetaErr[1] = 0.0;
  AlphaPhi[1] = 0.0;
  AlphaPhiErr[1] = 0.0;
  AlphaBool[1] = 0;
}

void SetAlpha(double theta, double energy, double phi)
{
  if(!AlphaBool[0])
  {
    AlphaBool[0]=1;
    AlphaEnergy[0]=energy;
    AlphaTheta[0]=theta;
    AlphaPhi[0]=phi;
  }
  else if(!AlphaBool[1])
  {
    AlphaBool[1]=1;
    AlphaEnergy[1]=energy;
    AlphaTheta[1]=theta;
    AlphaPhi[1]=phi;
  }
  EvThetaCut[0]->Fill(theta,energy);
}

void SetBe(double theta, double energy, double phi)
{
  BeBool = 1;
  BeEnergy = energy;
  BeTheta = theta;
  BePhi = phi;

  EvThetaCut[1]->Fill(theta,energy);
}

void PrintStatus()
{
  cout<<setprecision(5);
  cout<<"           Be      A1      A2"<<endl;
  cout<<"Bool   "<<setw(6)<<BeBool<<"  "<<setw(6)<<AlphaBool[0]<<"  "<<setw(6)<<AlphaBool[1]<<endl;
  cout<<"Theta  "<<setw(6)<<BeTheta<<"  "<<setw(6)<<AlphaTheta[0]<<"  "<<setw(6)<<AlphaTheta[1]<<endl;
  cout<<"Phi    "<<setw(6)<<BePhi<<"  "<<setw(6)<<AlphaPhi[0]<<"  "<<setw(6)<<AlphaPhi[1]<<endl;
  cout<<"Energy "<<setw(6)<<BeEnergy<<"  "<<setw(6)<<AlphaEnergy[0]<<"  "<<setw(6)<<AlphaEnergy[1]<<endl<<endl;
}

double findtheta(double t1, double p1, double r1, double t2, double p2, double r2, int det)
{
  //t1-=31.;
  //t2-=31.;
  t1=toRadians(t1);
  t2=toRadians(t2);
  p1=toRadians(p1);
  p2=toRadians(p2);
  
  double theta = 0.;
  if(det!=1&&det!=3)
  {
    cout<<"Error, det must be the dE detector ID"<<endl;
    return(-100.);
  }
  
  double *vals1 = new double[3];
  double *vals2 = new double[3];
  double *deltavals = new double[3];
  
  vals1 = findxyz(t1,p1,r1);
  vals2 = findxyz(t2,p2,r2);

  XYZ->Fill(vals1[0],vals1[1],vals1[2]);
  XYZ->Fill(vals2[0],vals2[1],vals2[2]);
  proj[0]->Fill(vals1[0],vals1[1]);
  
  for(int coord=0;coord<3;coord++)
  {
    deltavals[coord]=(vals2[coord]+vals1[coord])/2.;
  }
  
  //sad attempt to do a dot product
  double mag = sqrt(deltavals[0]*deltavals[0]+deltavals[1]*deltavals[1]+deltavals[2]*deltavals[2]);
  double dot = deltavals[0]*0.+deltavals[1]*0.+deltavals[2]*1.;
  
  theta = acos(dot/mag);

  theta = toDegrees(theta);

  /*{
  cout<<"Theta correction output (All angles in Degrees):"<<endl;
  cout<<"Arguments (Theta, Phi, R): "<<toDegrees(t1)<<", "<<toDegrees(p1)<<", "<<r1<<",   "<<toDegrees(t2)<<", "<<toDegrees(p2)<<", "<<r2<<endl;
  cout<<"vals1 (x,y,z): "<<setw(5)<<vals1[0]<<", "<<setw(5)<<vals1[1]<<", "<<setw(5)<<vals1[2]<<endl;
  cout<<"vals2 (x,y,z): "<<setw(5)<<vals2[0]<<", "<<setw(5)<<vals2[1]<<", "<<setw(5)<<vals2[2]<<endl;
  cout<<"deltavals (x,y,z): "<<setw(5)<<deltavals[0]<<", "<<setw(5)<<deltavals[1]<<", "<<setw(5)<<deltavals[2]<<endl;

  cout<<"magnitude of deltavals is: "<<mag<<endl;
  cout<<"dotproduct of deltavalues.zaxis is: "<<dot<<endl;
  cout<<"taking acos of (dotproduct/magnitude): "<<dot/mag<<endl;
  cout<<"theta is: "<<theta<<endl;
  cout<<endl;
  }*/
  
  return(theta);
}

double findtheta_avg(double t1, double p1, double r1, double t2, double p2, double r2)
{
  //t1-=31.;
  //t2-=31.;
  t1=toRadians(t1);
  t2=toRadians(t2);
  p1=toRadians(p1);
  p2=toRadians(p2);
  
  double theta = 0.;
  
  double *vals1 = new double[3];
  double *vals2 = new double[3];
  double *deltavals = new double[3];
  
  vals1 = findxyz(t1,p1,r1);
  vals2 = findxyz(t2,p2,r2);
  
  //sad attempt to do a dot product
  double mag = sqrt(deltavals[0]*deltavals[0]+deltavals[1]*deltavals[1]+deltavals[2]*deltavals[2]);
  double dot = deltavals[0]*0.+deltavals[1]*0.+deltavals[2]*1.;
  
  theta = acos(dot/mag);
  
  theta = toDegrees(theta);
  
  /*{
   * cout<<"Theta correction output (All angles in Degrees):"<<endl;
   * cout<<"Arguments (Theta, Phi, R): "<<toDegrees(t1)<<", "<<toDegrees(p1)<<", "<<r1<<",   "<<toDegrees(t2)<<", "<<toDegrees(p2)<<", "<<r2<<endl;
   * cout<<"vals1 (x,y,z): "<<setw(5)<<vals1[0]<<", "<<setw(5)<<vals1[1]<<", "<<setw(5)<<vals1[2]<<endl;
   * cout<<"vals2 (x,y,z): "<<setw(5)<<vals2[0]<<", "<<setw(5)<<vals2[1]<<", "<<setw(5)<<vals2[2]<<endl;
   * cout<<"deltavals (x,y,z): "<<setw(5)<<deltavals[0]<<", "<<setw(5)<<deltavals[1]<<", "<<setw(5)<<deltavals[2]<<endl;
   * 
   * cout<<"magnitude of deltavals is: "<<mag<<endl;
   * cout<<"dotproduct of deltavalues.zaxis is: "<<dot<<endl;
   * cout<<"taking acos of (dotproduct/magnitude): "<<dot/mag<<endl;
   * cout<<"theta is: "<<theta<<endl;
   * cout<<endl;
}*/
  
  return(theta);
}

double* findxyz(double theta, double phi, double r)
{
  //THESE ANGLES ARE PASSED AS RADIANS, THIS IS DIFFERENT FROM CONVENTION
  /*double r=0.;
  
  switch(det)
  {
    case 0:
    case 5:
      r=70.91;
      break;
      
    case 1:
    case 3:
      r=65.13;
      break;
      
    case 2:
    case 4:
      r=71.13;
      break;
  }

  double x, d, DD, h, RR;

  x = 2.*r*sin(theta/2.);

  d=x*sin(theta/2.)/sin(90.-theta);

  //if(theta>0.)
    RR = (r+d)/cos(phi);
  //else
    //RR = (r-d)/cos(phi);
  //DD = sqrt((r+d)*(r+d)-r*r);

  //h = DD*tan(phi);

  //RR = sqrt((r+d)*(r+d)+h*h);

  //RR=r;//**************************/

  double *vals = new double[3];
  
  vals[0] = r*sin(theta)*cos(phi);
  vals[1] = r*sin(theta)*sin(phi);
  vals[2] = r*cos(theta);
  
  return(vals);
}

double toRadians(double angle)
{
  return(angle*M_PI/180.);
}

double toDegrees(double angle)
{
  return(angle*180./M_PI);
}

double* CorrParticle(double Energy, double Theta, double Phi)
{
  bool debug = 0;
  const double pi = TMath::Pi();
  //const double QVal = 6.310645;//from http://www.nndc.bnl.gov/qcalc/
  const double Mass = MASS_BE10;

  if(debug)
    cout<<"CORR PARTICLE DEBUG ACTIVE, E: "<<Energy<<" T: "<<Theta*180./pi<<" P: "<<Phi*180./pi<<endl;

  //Energy = Energy/1000.;

  double *Values = new double[3];

  double pParticleMag = sqrt( 2. * Mass * Energy);

  TVector3 pParticle = TVector3(pParticleMag,0.,0.);
  pParticle.SetTheta(Theta);
  pParticle.SetPhi(Phi);

  if(debug)
    cout<<"BEAM_ENERGY: "<<BEAM_ENERGY<<" sqrt: "<<sqrt( 2. * MASS_BE11 * BEAM_ENERGY)<<", prod: "<<double(MASS_BE11) * double(BEAM_ENERGY)<<endl;

  double pBeamMag = sqrt( 2. * MASS_BE11 * BEAM_ENERGY); //This is all in the z direction

  if(debug)
  {
    cout<<"PARTICLE X: "<<pParticle.X()<<" Y: "<<pParticle.Y()<<" Z: "<<pParticle.Z()<<endl;
  }

  double CorrMass = MASS_BE10;

  //double ECorr = BEAM_ENERGY - Energy - QVal;

  //double pCorrMag = pBeamMag*pBeamMag/MASS_BE11 - pParticle.Mag2()/Mass;

  TVector3 pCorr;
  pCorr.SetX(-pParticle.X());
  pCorr.SetY(-pParticle.Y());
  pCorr.SetZ(pBeamMag-pParticle.Z());

  if(debug)
  {
    cout<<"CORR X: "<<pCorr.X()<<" Y: "<<pCorr.Y()<<" Z: "<<pCorr.Z()<<endl;
    //cout<<"Magnitudes    Particle: "<<pParticle.Mag()<<", Beam: "<<pBeamMag<<", Corr: "<<pCorr.Mag2()<<endl;
    cout<<"Energies from Mag  Particle: "<<pParticle.Mag2()/(2*Mass)<<", Beam: "<<pBeamMag*pBeamMag/(2*MASS_BE11)<<", Corr: "<<pCorr.Mag2()/(2*CorrMass)<<endl;
  }

  Values[0] = pCorr.Mag2() / (2.*CorrMass );  //Energy, from E=p^2/2m
  Values[1] = pCorr.Theta();
  Values[2] = pCorr.Phi();

  if(debug)
    cout<<"Energy: "<<Values[0]<<" Theta: "<<Values[1]*180/pi<<" Phi: "<<Values[2]*180/pi<<endl;

  if(debug)
    cout<<"Checking energy: "<<"Particle E: "<<Energy<<", Corresponding Energy: "<<Values[0]<<", "<<BEAM_ENERGY<<" MeV - those two: "<<BEAM_ENERGY - (Energy + Values[0])<<endl;

  if(debug)
    cout<<endl;

  return Values;

}

double GetExciteE(double be12E, double be12T)
{
//     cout<<"BeamE: "<<BEAM_ENERGY<<endl;
//     cout<<"BeE: "<<be12E<<endl;
//     cout<<"BeT: "<<be12T<<endl;

  be12T=toRadians(be12T);
  //be12E=be12E/1000.;
  const double pi = TMath::Pi();
  
  const double M1 = MASS_BE11;
  const double M2 = MASS_BE9;
  const double M3 = MASS_BE10;
  const double M4 = MASS_BE10;
  double mQ = M1+M2-M3-M4;
  
  double V1 = sqrt(2*BEAM_ENERGY/M1);
//   cout<<"V1: "<<V1<<" should be: .1035"<<endl;
  double COMV = ( M1 / ( M1 + M2 ) ) * V1;
//   cout<<"COMV: "<<COMV<<" should be:"<<endl;
  double V4 = sqrt(2*be12E/M4);
  double kPrimeM4 = COMV / V4;
  
  double COMTotalE = M2 / ( M1 + M2 ) * BEAM_ENERGY;
  double COMEnergyM4 = be12E * ( 1 + kPrimeM4*kPrimeM4 - 2*kPrimeM4*cos( be12T ) );
  double QVal =  ((COMEnergyM4*(M3+M4))-(M3*COMTotalE))/M3;//( COMEnergyM4*( M3 + M4 ) ) / M3 - COMTotalE;

//   cout<<"QVal: "<<QVal<<endl;
//   cout<<"mQ: "<<mQ<<endl<<endl;
  double ExcitedState = mQ - QVal;
  
  //   cout<<"EX: "<<ExcitedState<<endl<<endl;
  
  return(ExcitedState);
  
}
