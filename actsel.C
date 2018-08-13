#define actsel_cxx

#include "actsel.h"
#include <TStyle.h>
#include <iostream>
#include <iomanip>
#include <cmath>
#include "angleheader.h"

using namespace std;

void actsel::Begin(TTree * /*tree*/)
{
   TString option = GetOption();
   
  OutputFile = new TFile("ProcessedEventOut.root","recreate");
  randgen = new TRandom();
  
  SetHistos();
  SetCuts();
}

void actsel::SlaveBegin(TTree * /*tree*/)
{
   TString option = GetOption();

}

void SetCuts()
{
  TCutG *cut;
  Cuts = new TList();
  
  gROOT->ProcessLine(".x cuts/alphas2.C");
  cut = (TCutG*)gROOT->FindObjectAny("alphas");
  Cuts->Add(cut);

  gROOT->ProcessLine(".x cuts/be12.C");
  cut = (TCutG*)gROOT->FindObjectAny("be12");
  Cuts->Add(cut);

  gROOT->ProcessLine(".x cuts/bad2.C");
  cut = (TCutG*)gROOT->FindObjectAny("bad");
  Cuts->Add(cut);

  gROOT->ProcessLine(".x cuts/baddEvE.C");
  cut = (TCutG*)gROOT->FindObjectAny("baddEvE");
  Cuts->Add(cut);

  gROOT->ProcessLine(".x cuts/ForbiddenTheta.C");
  cut = (TCutG*)gROOT->FindObjectAny("ForbiddenTheta");
  Cuts->Add(cut);
  

  //(this->FindCut("alphas"))->IsInside(energy[2],energy[1])
}

void SetHistos()
{
  XYZ = new TH3D("3Dplot", "Raw Positions", 400,-100,100,120,-30,30,170,0,85);
  XYZ->GetXaxis()->SetTitle("X");
  XYZ->GetYaxis()->SetTitle("Y");
  XYZ->GetZaxis()->SetTitle("Z");
  DetImage = new TH2D("DetImage", "Detector Positions",400,-100,100,170,0,85);
  
  EvTheta[0] = new TH2D("EvTheta_rs", "Energy vs Theta Right Side",400,50,100,400,0,50);
  EvTheta[1] = new TH2D("EvTheta_rd", "Energy vs Theta Right dE",400,10,60,400,0,50);
  EvTheta[2] = new TH2D("EvTheta_re", "Energy vs Theta Right E",400,10,60,400,0,50);
  EvTheta[3] = new TH2D("EvTheta_ld", "Energy vs Theta Left dE",400,10,60,400,0,50);
  EvTheta[4] = new TH2D("EvTheta_le", "Energy vs Theta Left E",400,10,60,400,0,50);
  EvTheta[5] = new TH2D("EvTheta_ls", "Energy vs Theta Left Side",400,50,100,400,0,50);

  badEvTheta[0] = new TH2D("BadEvTheta_dE", "Energy vs Theta Bad",400,10,60,400,0,50);
  badEvTheta[1] = new TH2D("BadEvTheta_E", "Energy vs Theta Bad",400,10,60,400,0,50);

  builtEvTheta = new TH2D("EvThetaFull", "Total Energy vs Theta",640,10,90,480,0,60);

  AllTheta = new TH1D("All_Theta", "All Thetas",380,0,95);
  
  dEvE = new TH2D("dEvE", "dE vs E",880,0,55,880,0,55);
  BADdEvE = new TH2D("BADdEvE", "BAD dE vs E",220,0,55,220,0,55);

  MaxAngle = new TH2D("MaxAngle", "Energy vs Theta",640,10,90,480,0,60);
  
  
  //Eplot = new TH1D("Eplot", "Energy in E",600,0,60);
  //dEplot = new TH1D("dEplot", "Energy in dE",600,0,60);
  //SY = new TH1I("SY", "Strip number for Y",16,0,16);
  //SX = new TH1I("SX", "Strip number for X",16,0,16);

  blurplot = new TH1D("blurplot", "Value of Blur factor",800,-200,200);

  EvThetaCut[0] = new TH2D("EvThetaCut_alpha", "Alpha Cut",110,0,55,110,0,55);
  EvThetaCut[1] = new TH2D("EvThetaCut_be12", "Be12 Cut",110,0,55,110,0,55);
  
  HitPattern[0] = new TH2I("HPrs", "Hit Pattern for Right Side Detector",16,0,16,16,0,16);
  HitPattern[1] = new TH2I("HPrd", "Hit Pattern for Right dE Detector",16,0,16,16,0,16);
  HitPattern[2] = new TH2I("HPre", "Hit Pattern for Right E Detector",16,0,16,16,0,16);
  HitPattern[3] = new TH2I("HPld", "Hit Pattern for Left dE Detector",16,0,16,16,0,16);
  HitPattern[4] = new TH2I("HPle", "Hit Pattern for Left E Detector",16,0,16,16,0,16);
  HitPattern[5] = new TH2I("HPls", "Hit Pattern for Left Side Detector",16,0,16,16,0,16);
  
}

Bool_t actsel::Process(Long64_t entry)
{
  fChain->GetTree()->GetEntry(entry);  
  
  //double pi = 4.*atan(1.);
  double energy[6][5] = {{0}};
  double theta[6][5] = {{0}};
  double phi[6][5]= {{0}};
  double sx[6][5] = {{0}};
  double sy[6][5] = {{0}};
  int hit[6] = {0};
  
  for( int i = 0; i < Hits; i++ )
  {
    
    XYZ->Fill(DetectorX[i],DetectorY[i],DetectorZ[i]);
    DetImage->Fill(DetectorX[i],DetectorZ[i]);
    
    blurplot->Fill(Blur[i]);
    HitPattern[DetectorID[i]]->Fill(StripX[i],StripY[i]);
    
    energy[DetectorID[i]][hit[DetectorID[i]]] = EnergyDep[i];	
    //theta[DetectorID[i]][hit[DetectorID[i]]] = Theta[i];

    //phi[DetectorID[i]][hit[DetectorID[i]]] = Phi[i];
    sx[DetectorID[i]][hit[DetectorID[i]]] = StripX[i];
    sy[DetectorID[i]][hit[DetectorID[i]]] = StripY[i];
    
    hit[DetectorID[i]]++;

    //cout<<"I: "<<i<<" DetectorID: "<<DetectorID[i]<<" X:"<<StripX[i]<<" Y:"<<StripY[i]<<" Energy: "<<EnergyDep[i]<<endl;
    
  }

  
/*
  for(int det=0;det<6;det++)
  {
    cout<<endl<<endl<<"BEFORE ******  "<<det<<" Hit: "<<hit[det]<<endl<<"X:  Y: Energy:"<<endl;
    
    for(int num=0;num<hit[det];num++)
    {
      cout<<sx[det][num]<<"  "<<sy[det][num]<<"  "<<energy[det][num]<<endl;
    }
  }
*/


//cout<<endl;
  for(int det=0;det<6;det++)
  {
    int xI = -10;
    int xII = -10;
    int yI = -10;
    int yII = -10;
    double energyI = 0.;
    double energyII = 0.;
    
    for(int num=0;num<5;num++)
    {

      if(energy[det][num]<.0001);
      
      else if(xI==-10 && yI==-10)
      {
	xI = sx[det][num];
	yI = sy[det][num];
      }

      else if(sx[det][num]==xI && sy[det][num]==yI);
      
      else if(xII==-10 && yII == -10)
      {
	xII = sx[det][num];
	yII = sy[det][num];
      }
      
    }
    
    bool bI=0;
    bool bII=0;
    bool problem=0;
    
    for(int num=0;num<5;num++)
    {
      if(energy[det][num]<.0001);
      
      else if(sx[det][num]==xI && sy[det][num]==yI)
      {
	energyI+=energy[det][num];
	bI=1;
      }
      else if(sx[det][num]==xII && sy[det][num]==yII)
      {
	energyII+=energy[det][num];
	bII=1;
      }
      else if(sx[det][num] != -10 || sy[det][num] != -10)
      {
	//cout<<"Problem Det: "<<det<<endl;
	//cout<<"Problem x: "<<xI<<", "<<xII<<": "<<sx[det][num]<<endl;
	//cout<<"Problem y: "<<yI<<", "<<yII<<": "<<sy[det][num]<<endl;
	//cout<<"Problem Energy: "<<energyI<<", "<<energyII<<": "<<energy[det][num]<<endl<<endl;

	problem=1;
      }
      
    }
    
    hit[det]=int(bI)+int(bII);
    energy[det][0]=energyI;
    energy[det][1]=energyII;

    sx[det][0] = xI;
    sx[det][1] = xII;

    sy[det][0] = yI;
    sy[det][1] = yII;
    
    /*double *angarrayI = FindAltAngles(xI,yI,det);
    theta[det][0] = angarrayI[0];
    
    phi[det][0] = angarrayI[1];
    
    double *angarrayII = FindAltAngles(xII,yII,det);
    theta[det][1] = angarrayII[0];
    phi[det][1] = angarrayII[1];*/

    theta[det][0]=thetaarray[det][yI][xI];
    theta[det][1]=thetaarray[det][yII][xII];
    
    

    if(problem)
    {
      sx[det][0]=0;
      sy[det][0]=0;
      energy[det][0]=0.;
      theta[det][0]=0.;
      phi[det][0]=0.;
      
      sx[det][1]=0;
      sy[det][1]=0;
      energy[det][1]=0.;
      theta[det][1]=0.;
      phi[det][1]=0.;
    }
    
    sx[det][2]=0;
    sy[det][2]=0;
    energy[det][2]=0.;
    theta[det][2]=0.;
    phi[det][2]=0.;
    
    sx[det][3]=0;
    sy[det][3]=0;
    energy[det][3]=0.;
    theta[det][3]=0.;
    phi[det][3]=0.;

    sx[det][4]=0;
    sy[det][4]=0;
    energy[det][4]=0.;
    theta[det][4]=0.;
    phi[det][4]=0.;
  }
/*
  for(int det=0;det<6;det++)
  {
    cout<<endl<<endl<<"AFTER ******  "<<det<<" Hit: "<<hit[det]<<endl<<"X:  Y: Energy:"<<endl;
    
    for(int num=0;num<hit[det];num++)
    {
      cout<<sx[det][num]<<"  "<<sy[det][num]<<"  "<<energy[det][num]<<endl;
    }
  }
  */

  /*
  cout<<"--------------Before--------------"<<endl;
  cout<<"  "<<hit[2]<<" "<<hit[4]<<endl;
  cout<<"  "<<hit[1]<<" "<<hit[3]<<endl;
  cout<<hit[0]<<"     "<<hit[5]<<endl;

  for(int i=0; i<6; i++)
  {
    for(int j=0;j<10;j++)
    {
      if(energy[i][j]!=0)
	cout<<"Energy["<<i<<"]["<<j<<"]= "<<energy[i][j]<<" ";
    }
      cout<<endl;
    for(int j=0;j<10;j++)
    {
      if(energy[i][j]!=0)
	cout<<"Theta["<<i<<"]["<<j<<"] = "<<theta[i][j]<<" ";
    }
      cout<<endl;
    
  }
*/

//Old Theta based sorter
/*
  for(int det=0;det<6;det++)
  {
    double phiI = 0;
    double phiII = 0;
    double thetaI = 0;
    double thetaII = 0;
    double energyI = 0;
    double energyII = 0;
    hit[det]=0;
    for(int num=0;num<10;num++)
    {
      if(theta[det][num]==0);
      
      else if(thetaI==0)
      {
	thetaI = theta[det][num];
	phiI = phi[det][num];
      }
      //else if(theta[det][num]==thetaI && phi[det][num]!=phiI)
      //else if()
	  //cout<<".";
      else if(thetaII==0 && (theta[det][num]!=thetaI || (abs(theta[det][num]-thetaI)<.01 && theta[det][num] != thetaI)))
      {
	thetaII = theta[det][num];
	phiII = phi[det][num];
      }
    }

    bool bI=0;
    bool bII=0;
    
    for(int num=0;num<10;num++)
    {
      if(theta[det][num]==0);
      
      else if(theta[det][num]==thetaI && phi[det][num]==phiI)
      {
	energyI+=energy[det][num];
	bI=1;
      }
      else if(theta[det][num]==thetaII && phi[det][num]==phiII)
      {
	energyII+=energy[det][num];
	bII=1;
      }
      else if(theta[det][num]!=0)
      {
	cout<<"Problem Angles: "<<thetaI<<", "<<thetaII<<": "<<theta[det][num]<<endl;
        cout<<"Problem Energy: "<<energyI<<", "<<energyII<<": "<<energy[det][num]<<endl;
	cout<<"Problem Phi: "<<phiI<<", "<<phiII<<": "<<phi[det][num]<<endl<<endl;;
      }

    }
    hit[det]=int(bI)+int(bII);
    theta[det][0]=thetaI;
    theta[det][1]=thetaII;
    phi[det][0]=phiI;
    phi[det][1]=phiII;
    energy[det][0]=energyI;
    energy[det][1]=energyII;
    for(int j=2;j<10;j++)
    {
      theta[det][j]=0;
      energy[det][j]=0;
      phi[det][j]=0;
    }
    
    //if((this->FindCut("baddEvE"))->IsInside(energy[2][0],energy[1][0]))
      //cout<<stripI<<", "<<stripII<<endl;
    
  }

*/
/*
if((this->FindCut("baddEvE"))->IsInside(energy[2][0],energy[1][0]))
{
  cout<<"  "<<hit[2]<<" "<<hit[4]<<endl;
  cout<<"  "<<hit[1]<<" "<<hit[3]<<endl;
  cout<<hit[0]<<"     "<<hit[5]<<endl<<endl;

  cout<<setprecision(5);

  cout<<"2 "<<setw(3)<<sx[2][0]<<" "<<setw(3)<<sy[2][0]<<"       Theta "<<setw(5)<<theta[2][0]<<"            Energy "<<setw(6)<<energy[2][0]<<endl;
  cout<<"  "<<setw(3)<<sx[2][1]<<" "<<setw(3)<<sy[2][1]<<"             "<<setw(5)<<theta[2][1]<<"            Energy "<<setw(6)<<energy[2][1]<<endl<<endl;

  cout<<"1 "<<setw(3)<<sx[1][0]<<" "<<setw(3)<<sy[1][0]<<"       Theta "<<setw(5)<<theta[1][0]<<"            Energy "<<setw(6)<<energy[1][0]<<endl;
  cout<<"  "<<setw(3)<<sx[1][1]<<" "<<setw(3)<<sy[1][1]<<"             "<<setw(5)<<theta[1][1]<<"            Energy "<<setw(6)<<energy[1][1]<<endl<<endl;
      
}
  */

  if(theta[1][0]==theta[2][1]||theta[1][1]==theta[2][0])
  {
    double temp = theta[2][0];
    theta[2][0] = theta[2][1];
    theta[2][1] = temp;

    temp = energy[2][0];
    energy[2][0] = energy[2][1];
    energy[2][1] = temp;
  }
  else if(theta[3][0]==theta[4][1]||theta[3][1]==theta[4][0])
  {
    double temp = theta[4][0];
    theta[4][0] = theta[4][1];
    theta[4][1] = temp;

    temp = energy[4][0];
    energy[4][0] = energy[4][1];
    energy[4][1] = temp;
  }
  
/*
  cout<<"--------------After---------------"<<endl;
  cout<<"  "<<hit[2]<<" "<<hit[4]<<endl;
  cout<<"  "<<hit[1]<<" "<<hit[3]<<endl;
  cout<<hit[0]<<"     "<<hit[5]<<endl;
  
  for(int i=0; i<6; i++)
  {
    for(int j=0;j<10;j++)
    {
      if(hit[i]>2&&energy[i][j]!=0)
	cout<<"Energy["<<i<<"]["<<j<<"]= "<<energy[i][j]<<" ";
    }
      cout<<endl;
    for(int j=0;j<10;j++)
    {
      if(hit[i]>2&&energy[i][j]!=0)
	cout<<"Theta["<<i<<"]["<<j<<"] = "<<theta[i][j]<<" ";
    }
      cout<<endl;
  }

  for(int i=0; i<6; i++)
  {
    for(int j=0;j<10;j++)
    {
      if(energy[i][j]!=0)
      cout<<"Energy["<<i<<"]["<<j<<"]= "<<energy[i][j]<<" ";
    }
    cout<<endl;
    for(int j=0;j<10;j++)
    {
      if(energy[i][j]!=0)
      cout<<"Theta["<<i<<"]["<<j<<"] = "<<theta[i][j]<<" ";
    }
    cout<<endl;
    
  }
  
  cout<<"*******************************"<<endl<<endl;

*/

  double randE = randgen->Uniform(80.);
    
  MaxAngle->Fill(52.,randE);//dE
  MaxAngle->Fill(50.3657,randE);//E
  MaxAngle->Fill(56.43655,randE);//Side

  MaxAngle->Fill(10.,randE);//dE
  MaxAngle->Fill(11.6343,randE);//E
  MaxAngle->Fill(94.10764,randE);//Side
    

  
  if(hit[1]&&!hit[2])//&&(sx[1][0]==1&&sy[1][0]==8))
  {
    builtEvTheta->Fill(theta[1][0],energy[1][0]);
    if(energy[1][1]>0)
      builtEvTheta->Fill(theta[1][1],energy[1][1]);
  }
  else if(hit[3]&&!hit[4])//&&(sx[3][0]==1&&sy[3][0]==8))
  {
    builtEvTheta->Fill(theta[3][0],energy[3][0]);
    if(energy[3][1]>0)
      builtEvTheta->Fill(theta[3][1],energy[3][1]);
  }
  else if(hit[1]&&hit[2])//&&(sx[1][0]==1&&sy[1][0]==8))
  {
    builtEvTheta->Fill(theta[2][0],energy[2][0]+energy[1][0]);
    if((this->FindCut("be12"))->IsInside(energy[2][0],energy[1][0]))
      EvThetaCut[1]->Fill(theta[2][0],energy[2][0]+energy[1][0]);
    else if((this->FindCut("alphas"))->IsInside(energy[2][0],energy[1][0]))
      EvThetaCut[0]->Fill(theta[2][0],energy[2][0]+energy[1][0]);
  }
  else if(hit[3]&&hit[4])//&&(sx[3][0]==1&&sy[3][0]==8))
  {
    builtEvTheta->Fill(theta[4][0],energy[4][0]+energy[3][0]);
    if((this->FindCut("be12"))->IsInside(energy[4][0],energy[3][0]))
      EvThetaCut[1]->Fill(theta[4][0],energy[4][0]+energy[3][0]);
    else if((this->FindCut("alphas"))->IsInside(energy[4][0],energy[3][0]))
      EvThetaCut[0]->Fill(theta[4][0],energy[4][0]+energy[3][0]);
  }


  if(hit[0]&&energy[0][0]>.5)
  {
    builtEvTheta->Fill(theta[0][0],energy[0][0]);
    //cout<<".";
    if(energy[0][1]>0)
      builtEvTheta->Fill(theta[0][1],energy[0][1]);
  }
  else if(hit[5]&&energy[5][0]>.5)
  {
    builtEvTheta->Fill(theta[5][0],energy[5][0]);
    //cout<<":";
    if(energy[5][1]>0)
      builtEvTheta->Fill(theta[5][1],energy[5][1]);
  }

  
  
  for(int mydet=0;mydet<6;mydet++)
  {
  if(energy[mydet][0]>0&&hit[mydet])
  {
    EvTheta[mydet]->Fill(theta[mydet][0],energy[mydet][0]);
    AllTheta->Fill(theta[mydet][0]);
    if(energy[mydet][1]>0)
    {
      EvTheta[mydet]->Fill(theta[mydet][1],energy[mydet][1]);
      AllTheta->Fill(theta[mydet][1]);
      //cout<<".";
    }
  }
  }
  
  if(hit[1]&&hit[2])
  {
    dEvE->Fill(energy[2][0],energy[1][0]);
    
    if((this->FindCut("baddEvE"))->IsInside(energy[2][0],energy[1][0]))
    {
      BADdEvE->Fill(energy[2][0],energy[1][0]);
      badEvTheta[0]->Fill(theta[1][0],energy[1][0]);
      badEvTheta[1]->Fill(theta[2][0],energy[2][0]);
      badEvTheta[0]->Fill(theta[1][1],energy[1][1]);
      badEvTheta[1]->Fill(theta[2][1],energy[2][1]);
    }
    
    if(energy[2][1]>0.&&energy[1][1]>0.)
      dEvE->Fill(energy[2][1],energy[1][1]);
  }
  else if(hit[3]&&hit[4])
  {
    dEvE->Fill(energy[4][0],energy[3][0]);
    
    if((this->FindCut("baddEvE"))->IsInside(energy[4][0],energy[3][0]))
    {
      BADdEvE->Fill(energy[4][0],energy[3][0]);
      badEvTheta[0]->Fill(theta[3][0],energy[3][0]);
      badEvTheta[1]->Fill(theta[4][0],energy[4][0]);
      badEvTheta[0]->Fill(theta[3][1],energy[3][1]);
      badEvTheta[1]->Fill(theta[4][1],energy[4][1]);
    }
    
    if(energy[4][1]>0.&&energy[3][1]>0.)
      dEvE->Fill(energy[4][1],energy[3][1]);
  }

  //cout<<endl;
  
   return kTRUE;
}

double *FindAltAngles(int sx, int sy, int det)
{
  
  double localpi = M_PI;
  
  double *angles = new double[2];
  angles[0]=0.;
  angles[1]=0.;
  
  if(sx==-10&&sy==-10)
    return(angles);
  
  double SideX = 68;
  double SideZ = -4.88;
  
  double dEX = 54.97;
  double dEZ = 42.95;
  
  double EX = 58.06;
  double EZ = 48.09;
  
  double detTheta = 31 * (localpi/180.);
  
  double x,y,z;
  double r,theta,phi;
  
  switch(det)
  {
    case 0: //Right Side
      x = SideX;
      z = SideZ + (50./32.)*(2*sx+1);
      break;
      
    case 5: //Left Side
      x = -SideX;
      z = SideZ + (50./32.)*(2*sx+1);
      break;
      
    case 1: //Right dE
      x = dEX - (50./32.)*cos(detTheta)*(2*sx+1);
      z = dEZ + (50./32.)*sin(detTheta)*(2*sx+1);
      break;
      
    case 3: //Left dE
      x = -dEX + (50./32.)*cos(detTheta)*(2*sx+1);
      z = dEZ + (50./32.)*sin(detTheta)*(2*sx+1);
      break;
      
    case 2: //Right E
      x = EX - (50./32.)*cos(detTheta)*(2*sx+1);
      z = EZ + (50./32.)*sin(detTheta)*(2*sx+1);
      break;
      
    case 4: //Left E
      x = -EX + (50./32.)*cos(detTheta)*(2*sx+1);
      z = EZ + (50./32.)*sin(detTheta)*(2*sx+1);
      break;
  }
  
  y = (50./32.)*(2*sy+1) - (50/16.)*8;
  
  
  r=sqrt(x*x+y*y+z*z);
  theta=acos(z/r);
  phi=atan(y/x);
  
  angles[0]=theta*180/localpi;
  angles[1]=phi*180/localpi;
  
  return(angles);
}

void actsel::SlaveTerminate()
{
cout<<"Slave Termination"<<endl;
}

void actsel::Terminate()
{
  cout<<"Terminating"<<endl;
  //EvThetaOverlay->Write();
  OutputFile->Write();
  OutputFile->Close();
  cout<<"Terminated"<<endl;
}
