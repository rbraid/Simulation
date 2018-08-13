#define FinalSelector_cxx

#include "FinalSelector.h"
#include <TH2.h>
#include <TStyle.h>

using namespace std;

void FinalSelector::Begin(TTree * /*tree*/)
{
  TString option = GetOption();
  OutputFile = new TFile("final_event.root","RECREATE");

  SetFinalHistos();
  
  cout<<"Begin"<<endl;
  
}

void FinalSelector::SlaveBegin(TTree * /*tree*/)
{
  TString option = GetOption();
  cout<<"Slave Begin"<<endl;
}

Bool_t FinalSelector::Process(Long64_t entry)
{
  fChain->GetTree()->GetEntry(entry);

  const double pi = M_PI;

  if(TGTSTRAGCORR)
  {
    if(AlphaBool[1])
    {
      AlphaEnergy[1]=CorrectE_Alpha(AlphaEnergy[1],AlphaTheta[1]);
      AlphaEnergy[0]=CorrectE_Alpha(AlphaEnergy[0],AlphaTheta[0]);
    }
    else if(AlphaBool[0])
    {
      AlphaEnergy[0]=CorrectE_Alpha(AlphaEnergy[0],AlphaTheta[0]);
    }

    if(BeBool)
    {
      BeEnergy=CorrectE_Be(BeEnergy,BeTheta);
    }
  }

  if(AlphaBool[1])
  {
    AlphaVAlpha->Fill(AlphaEnergy[0],AlphaEnergy[1]);
  }
  AlphaVAlphaMath->Fill(Alpha1EnergyMathNext,Alpha2EnergyMathNext);
  
  CheckEx[0]->Fill(GetExciteE_Heavy(BeEnergyMathNext,BeThetaMathNext));
  CheckEx[1]->Fill(GetExciteE_Light(Alpha1EnergyMathNext,Alpha1ThetaMathNext,Alpha1PhiMathNext,Alpha2EnergyMathNext,Alpha2ThetaMathNext,Alpha2PhiMathNext));

  double* Be8valsMath = new double[3];
  Be8valsMath = CalcBe8fromAlpha(Alpha1EnergyMathNext,Alpha1ThetaMathNext,Alpha1PhiMathNext,Alpha2EnergyMathNext,Alpha2ThetaMathNext,Alpha2PhiMathNext);
  CheckBe8->Fill(Be8valsMath[1]*180./pi,Be8valsMath[0]);
  //CheckBe8->Fill(Be8ThetaMathNext,Be8EnergyMathNext);
  
  CheckAlpha->Fill(Alpha1ThetaMathNext,Alpha1EnergyMathNext);
  CheckAlpha->Fill(Alpha2ThetaMathNext,Alpha2EnergyMathNext);
  
  if(BeBool)
  {
    EvTheta->Fill(BeTheta,BeEnergy);
    BeEvTheta->Fill(BeTheta,BeEnergy);
  }
  if(AlphaBool[0])
  {
    EvTheta->Fill(AlphaTheta[0],AlphaEnergy[0]);
    AlphaEvTheta[0]->Fill(AlphaTheta[0],AlphaEnergy[0]);
  }
  if(AlphaBool[1])
  {
    EvTheta->Fill(AlphaTheta[1],AlphaEnergy[1]);
    AlphaEvTheta[0]->Fill(AlphaTheta[1],AlphaEnergy[1]);
    
    double* Be8vals = new double[3];
    Be8vals = CalcBe8fromAlpha(AlphaEnergy[0],AlphaTheta[0],AlphaPhi[0],AlphaEnergy[1],AlphaTheta[1],AlphaPhi[1]);
    AlphaEvTheta[1]->Fill(Be8vals[1]*180./pi,Be8vals[0]);

    /*if(abs(Be8valsMath[1]*180./pi-Be8ThetaMathNext)>1.5)
    {
    cout<<"                 Energy   Theta"<<endl;
    cout<<"Be8Actual:      "<<setw(5)<<Be8EnergyMathNext<<", "<<setw(5)<<Be8ThetaMathNext<<endl;
    cout<<"Be8ActualRecon: "<<setw(5)<<Be8valsMath[0]<<", "<<setw(5)<<Be8valsMath[1]*180./pi<<endl;
    cout<<"Be8Recon:       "<<setw(5)<<Be8vals[0]<<", "<<setw(5)<<Be8vals[1]*180./pi<<endl<<endl;
    }*/
  }

  if(BeBool&&AlphaBool[0]&&AlphaBool[1])
  {
    EvThetaMulti[2]->Fill(BeTheta,BeEnergy);
    EvThetaMulti[2]->Fill(AlphaTheta[0],AlphaEnergy[0]);
    EvThetaMulti[2]->Fill(AlphaTheta[1],AlphaEnergy[1]);
    
    double ExciteHeavy = GetExciteE_Heavy(BeEnergy,BeTheta);
    double ExciteLight = GetExciteE_Light(AlphaEnergy[0],AlphaTheta[0],AlphaPhi[0],AlphaEnergy[1],AlphaTheta[1],AlphaPhi[1]);
    ExcitePlot[1]->Fill(ExciteHeavy);
    ExcitePlot[0]->Fill(ExciteLight);
    
    if(abs(ExciteHeavy-ExciteLight)<1.)
    {

    ExcitePlot[2]->Fill(ExciteHeavy);
    ExcitePlot[2]->Fill(ExciteLight);
    //cout<<ExciteHeavy<<" "<<ExciteLight<<" "<<abs(ExciteHeavy-ExciteLight)<<endl;
    }
    checkKins(BeEnergy,BeTheta,BePhi,AlphaEnergy[0],AlphaTheta[0],AlphaPhi[0],AlphaEnergy[1],AlphaTheta[1],AlphaPhi[1]);
    
  }
  else if(BeBool&&AlphaBool[0])
  {
    EvThetaMulti[1]->Fill(BeTheta,BeEnergy);
    EvThetaMulti[1]->Fill(AlphaTheta[0],AlphaEnergy[0]);
    ExcitePlot[1]->Fill(GetExciteE_Heavy(BeEnergy,BeTheta));
  }
  else if(AlphaBool[0]&&AlphaBool[1])
  {
    EvThetaMulti[1]->Fill(AlphaTheta[0],AlphaEnergy[0]);
    EvThetaMulti[1]->Fill(AlphaTheta[1],AlphaEnergy[1]);
    ExcitePlot[0]->Fill(GetExciteE_Light(AlphaEnergy[0],AlphaTheta[0],AlphaPhi[0],AlphaEnergy[1],AlphaTheta[1],AlphaPhi[1]));
  }
  else if(BeBool)
  {
    EvThetaMulti[0]->Fill(BeTheta,BeEnergy);
    ExcitePlot[1]->Fill(GetExciteE_Heavy(BeEnergy,BeTheta));
  }
  else if(AlphaBool[0])
  {
    EvThetaMulti[0]->Fill(AlphaTheta[0],AlphaEnergy[0]);
  }

  TotalEPlot->Fill(AlphaEnergy[0]+AlphaEnergy[1]+BeEnergy);
  
  MultiplicityPlot->Fill(int(BeBool)+int(AlphaBool[0])+int(AlphaBool[1]));

  return kTRUE;
}

void FinalSelector::SlaveTerminate()
{
  cout<<"Slave Terminate"<<endl;
}

void FinalSelector::Terminate()
{
  OutputFile->Write();
  OutputFile->Close();
  cout<<"Terminate"<<endl;
}

void SetFinalHistos()
{
  MultiplicityPlot = new TH1I("MultiplicityPlot","Multiplicity",5,0,5);
  EvTheta = new TH2D("EvTheta", "Total Energy vs Theta",720,0,90,480,0,60);
  
  EvThetaMulti[0] = new TH2D("EvThetaMulti_1", "Total Energy vs Theta with 1 hit",720,0,90,480,0,60);
  EvThetaMulti[1] = new TH2D("EvTheta3Multi_2", "Total Energy vs Theta with 2 hits",720,0,90,480,0,60);
  EvThetaMulti[2] = new TH2D("EvTheta3Multi_3", "Total Energy vs Theta with 3 hits",720,0,90,480,0,60);
  
  ExcitePlot[0] = new TH1D("ExcitePlot_Alpha","Excitation Energy",400,-5,20);
  ExcitePlot[1] = new TH1D("ExcitePlot_Be","Excitation Energy",400,-5,20);
  ExcitePlot[2] = new TH1D("ExcitePlot_Extra","Excitation Energy",400,-5,20);

  ThetaPlots[0] = new TH1D("Theta_8","Theta of Be8",720,-90,90);
  ThetaPlots[1] = new TH1D("Theta_8_predict","Theta of Be8 from Be12",720,-90,90);
  ThetaPlots[2] = new TH1D("Theta_8_diff","Difference in Theta of Be8",720,-90,90);
  
  AlphaEvTheta[0] = new TH2D("Alphas","Alpha E v Theta",720,0,90,480,0,60);
  AlphaEvTheta[1] = new TH2D("Be8s","Be8 E v Theta",720,0,90,480,0,60);
  BeEvTheta = new TH2D("Be12s","Be12 E v Theta",720,0,90,480,0,60);
  
  CheckEx[0] = new TH1D("CheckHeavy","Checking Heavy reconstruction",600,-1,5);
  CheckEx[1] = new TH1D("CheckLight","Checking Light reconstruction",600,-1,5);

  CheckBe8 = new TH2D("CheckBe8","Checking Be8 Reconstruction",720,0,90,480,0,60);
  CheckAlpha = new TH2D("CheckAlpha","Checking Alpha Reconstruction",720,0,90,480,0,60);

  TotalEPlot = new TH1D("TotalEPlot","Sum of energy of alphas and Be",100,0,100);

  AlphaVAlpha = new TH2D("AlphaVAlpha","Comparing the two alphas",280,0,40,280,0,40);
  AlphaVAlphaMath = new TH2D("AlphaVAlphaMath","Comparing the two alphas",280,0,40,280,0,40);
  
}

double* CalcBe8fromAlpha(double a1E, double a1T, double a1P, double a2E, double a2T, double a2P)
{
  const double pi=M_PI;  //M_PI is a defined constant
  
  a1T=a1T*pi/180.; //These lines are to convert from the inputted degrees to radians,
  a1P=a1P*pi/180.; //Since that is all cmath will take.
  a2T=a2T*pi/180.;
  a2P=a2P*pi/180.;
  
  double *Be8Values = new double[3];

  //Make the masses for the 8Be and 4He
  const double mBe8 = MASS_BE8-BE8EX;//8.0*931.494027 + 4.9416;
  const double mAlpha = MASS_HE4;//4.0*931.494027 + 2.4249156;
  
  vector<double> PVecAlpha1, PVecAlpha2, pBe;
  //Convert from energy to momentum
  double PAlpha1 = sqrt( 2.0*mAlpha*a1E );
  double PAlpha2 = sqrt( 2.0*mAlpha*a2E );
  
  //fill the momentum vector for the first alpha
  PVecAlpha1.push_back( PAlpha1*sin( a1T )*cos( a1P ) );
  PVecAlpha1.push_back( PAlpha1*sin( a1T )*sin( a1P ) );
  PVecAlpha1.push_back( PAlpha1*cos( a1T ) );
  
  //fill the momentum vector for the second alpha
  PVecAlpha2.push_back( PAlpha2*sin( a2T )*cos( a2P ) );
  PVecAlpha2.push_back( PAlpha2*sin( a2T )*sin( a2P ) );
  PVecAlpha2.push_back( PAlpha2*cos( a2T ) );
  
  //fill the 8Be vector
  pBe.push_back( ( PVecAlpha1[0]+PVecAlpha2[0] ) );
  pBe.push_back( ( PVecAlpha1[1]+PVecAlpha2[1] ) );
  pBe.push_back( ( PVecAlpha1[2]+PVecAlpha2[2] ) );
  
  //make the 8Be physical parameters, energy, theta, phi
  Be8Values[0] = ( (pBe[0]*pBe[0] + pBe[1]*pBe[1] + pBe[2]*pBe[2]) )/ (2.*mBe8 );  //Energy, from E=p^2/2m
  Be8Values[1] = acos( pBe[2]/ ( sqrt( pBe[0]*pBe[0] + pBe[1]*pBe[1] + pBe[2]*pBe[2] ) ) );  //Theta
  //Be8Values[2] = atan( pBe[1]/pBe[0] ); //Phi

  if( pBe[0] > 0.0 && pBe[1] > 0.0 )
    Be8Values[2] = atan( pBe[1] / pBe[0] );
  else if( pBe[0] < 0.0 && pBe[1] > 0.0 )
    Be8Values[2] = atan( pBe[1] / pBe[0] ) + pi;
  else if( pBe[0] < 0.0 && pBe[1] < 0.0 )
    Be8Values[2] = atan( pBe[1] / pBe[0] ) + pi;
  else if( pBe[0] > 0.0 && pBe[1] < 0.0 )
    Be8Values[2] = atan( pBe[1] / pBe[0] ) + 2*pi;

    return Be8Values;
}

double GetExciteE_Heavy(double be12E, double be12T)
{
  const double pi = M_PI;

  const double BeamE = 55.;

  be12T=be12T*pi/180.;
  
  //11Be(9Be,8Be)12Be*
  const double M1 = MASS_BE11;
  const double M2 = MASS_BE9;
  const double M3 = MASS_BE8;
  const double M4 = MASS_BE12;
  double mQ = M1+M2-M3-M4;
  
  double V1 = sqrt(2*BeamE/M1);
  double COMV = ( M1 / ( M1 + M2 ) ) * V1;
  double V4 = sqrt(2*be12E/M4);
  double kPrimeM4 = COMV / V4;
  
  double COMTotalE = M2 / ( M1 + M2 ) * BeamE;
  double COMEnergyM4 = be12E * ( 1 + kPrimeM4*kPrimeM4 - 2*kPrimeM4*cos( be12T ) );
  double QVal =  ( COMEnergyM4*( M3 + M4 ) ) / M3 - COMTotalE;
  double ExcitedState = mQ - QVal;
  
  return(ExcitedState);
  
}

double GetExciteE_Light(double a1E, double a1T, double a1P, double a2E, double a2T, double a2P)
{  
  const double BeamE = 55.;

  double *Be8Values = new double[3];

  Be8Values = CalcBe8fromAlpha(a1E,a1T,a1P,a2E,a2T,a2P);
  
  //11Be(9Be,8Be)12Be*
  const double M1 = MASS_BE11;
  const double M2 = MASS_BE9;
  const double M3 = MASS_BE8;
  const double M4 = MASS_BE12;
  double mQ = M1+M2-M3-M4;
  
  double VelBeam = sqrt(2*BeamE/M1);
  double COMV = ( M1 / ( M1 + M2 ) ) * VelBeam;
  double VelocityM3 = sqrt(2 * (Be8Values[0]) / M3);
  double kPrimeM3 = COMV / VelocityM3;
  
  double COMTotalE = M2 / ( M1 + M2 ) * BeamE;
  double COMEnergyM3 = Be8Values[0] * ( 1 + kPrimeM3*kPrimeM3 - 2*kPrimeM3*cos( Be8Values[1] ) );
  double QVal =  ( COMEnergyM3*( M3 + M4 ) ) / M4 - COMTotalE;
  double ExcitedState = mQ - QVal;

  return(ExcitedState);
  
}

double checkKins(double be12E, double be12T, double be12P, double a1E, double a1T, double a1P, double a2E, double a2T, double a2P)
{
  const double mAlpha = MASS_HE4;
  const double mBe12 = MASS_BE12;
  const double mBe8 = MASS_BE8;
  const double mBe9 = MASS_BE9;
  const double mBe11 = MASS_BE11;
  double mQ = mBe11 + mBe9 - mBe8 - mBe12;
  
  const double pi=M_PI;

  const double c=299792458;
  
  const double BeamE = 55.;

  double ExHeavy = GetExciteE_Heavy(be12E,be12T);
  double ExLight = GetExciteE_Light(a1E, a1T, a1P, a2E, a2T, a2P);

  double *Be8Values = new double[3];
  
  Be8Values = CalcBe8fromAlpha(a1E,a1T,a1P,a2E,a2T,a2P);

  vector<double> PVecBe12;
  double PBe12 = sqrt(2.*mBe12*be12E);
  PVecBe12.push_back( PBe12*sin( be12T )*cos( be12P ) );
  PVecBe12.push_back( PBe12*sin( be12T )*sin( be12P ) );
  PVecBe12.push_back( PBe12*cos( be12T ) );

  double PBeam = sqrt( 2.0*mBe11*BeamE );

  ThetaPlots[0]->Fill(Be8Values[1]*180./pi);
  ThetaPlots[1]->Fill(Calc8from12(be12E,be12T,Be8Values[1]*180./pi));
  ThetaPlots[2]->Fill(Be8Values[1]*180./pi-Calc8from12(be12E,be12T,Be8Values[1]*180./pi));

  /*cout<<"Checking Thetas"<<endl;
  cout<<"Alpha1: "<<a1T<<", "<<a1P<<endl;
  cout<<"Alpha2: "<<a2T<<", "<<a2P<<endl;
  cout<<"Be8   : "<<Be8Values[1]*180./pi<<", "<<Be8Values[2]*180./pi<<endl;
  cout<<"Predicted Be8: "<<Calc8from12(be12E,be12T,Be8Values[1]*180./pi)<<endl;
  cout<<"Be12  : "<<be12T<<", "<<be12P<<endl<<endl;

  cout<<"Checking Energies"<<endl;
  cout<<"Energy of Alphas: "<<a1E<<", "<<a2E<<endl;
  cout<<"Energy of Be8: "<<Be8Values[0]<<" ("<<a1E+a2E<<") Energy of Be12: "<<be12E<<endl;
  cout<<"Energy of Beam: 55 ("<<be12E+Be8Values[0]<<"), Difference: "<<55.-(be12E+Be8Values[0])<<endl;
  cout<<"Excite Light: "<<ExLight<<", Excite Heavy: "<<ExHeavy<<", Difference: "<<ExLight-ExHeavy<<endl<<endl;;

  cout<<"Checking Momentums"<<endl;
  cout<<"Beam Momentum: "<<PBeam<<endl;
  cout<<"Be12 info: "<<PVecBe12[0]<<", "<<PVecBe12[1]<<", "<<PVecBe12[2]<<", Total: "<<sqrt(PVecBe12[0]*PVecBe12[0]+PVecBe12[1]*PVecBe12[1]+PVecBe12[2]*PVecBe12[2])<<endl;
  cout<<"Alpha1 info: "<<PVecAlpha1[0]<<", "<<PVecAlpha1[1]<<", "<<PVecAlpha1[2]<<endl;
  cout<<"Alpha2 info: "<<PVecAlpha2[0]<<", "<<PVecAlpha2[1]<<", "<<PVecAlpha2[2]<<endl;
  cout<<"Be8 info: "<<pBe[0]<<", "<<pBe[1]<<", "<<pBe[2]<<", Total: "<<sqrt(pBe[0]*pBe[0]+pBe[1]*pBe[1]+pBe[2]*pBe[2])<<endl;

  
  cout<<"************"<<endl;*/
  //cout<<abs(pBe[0]-PVecBe12[0])<<", "<<abs(pBe[1]-PVecBe12[1])<<", "<<abs(pBe[2]-PVecBe12[2])<<endl<<endl;
  
  return(0.);
  
}

double Calc8from12(double E12, double T12, double E8)
{
  const double mBe12 = MASS_BE12;
  const double mBe8 = MASS_BE8;
  
  const double pi=M_PI;
    
  double V12=sqrt(2*E12/mBe12);
  double V8= sqrt(2*E8/mBe8);

  double T8 = abs(asin(-(mBe12*V12)/(mBe8*V8)*sin(T12*pi/180.)));
  
  return(T8*180./pi);
}

double CorrectE_Alpha(double E, double T)
{
  const double targetT=TARGET_THICKNESS;
  
  double pi = M_PI;
  double RealEnergy = E;
  T = T*pi/180.;
  double thickness = (targetT/2.)/cos(T);
  double steplength = 10;

  while(thickness>0.)
  {
    RealEnergy+= (0.27267185723475307 + 1.6844812118388253/pow(M_E,0.2542936852380424*RealEnergy))*steplength/1000.;
    thickness-=steplength;
  }
  
  //cout<<"Correcting Alpha Energy, E: "<<E<<", Theta: "<<T*180./pi<<", New Energy: "<<RealEnergy<<endl;
  
  return(RealEnergy);
}

double CorrectE_Be(double E, double T)
{
  const double targetT=TARGET_THICKNESS;
  
  double pi = M_PI;
  double RealEnergy = E;
  T = T*pi/180.;
  double thickness = (targetT/2.)/cos(T);
  double steplength = 10;
  
  while(thickness>0.)
  {
    RealEnergy+= (1.205966255137941 + 4.339223452110011/pow(M_E,0.06138967944782508*RealEnergy))*steplength/1000.;
    thickness-=steplength;
  }
  
  //cout<<"Correcting Beryllium Energy, E: "<<E<<", Theta: "<<T*180./pi<<", New Energy: "<<RealEnergy<<endl;
  
  return(RealEnergy);
}

/*
if(abs(MathT-(Be8Values[1]*180./pi))>2.&&MathE>.5)
{
  vector<double> pBeMathvec;
  double pBeMath = sqrt(2.*mBe8*MathE);
  pBeMathvec.push_back( pBeMath*sin( MathT )*cos( MathP ) );
  pBeMathvec.push_back( pBeMath*sin( MathT )*sin( MathP ) );
  pBeMathvec.push_back( pBeMath*cos( MathT ) );
  
  cout<<"I am checking the momentums"<<endl;
  cout<<"A1 xyz: "<<PVecAlpha1[0]<<", "<<PVecAlpha1[1]<<", "<<PVecAlpha1[2]<<endl;
  cout<<"A2 xyz: "<<PVecAlpha2[0]<<", "<<PVecAlpha2[1]<<", "<<PVecAlpha2[2]<<endl;
  cout<<"Be xyz: "<<pBe[0]<<", "<<pBe[1]<<", "<<pBe[2]<<endl;
  //cout<<"Theory Be: "<<pBeMathvec[0]<<", "<<pBeMathvec[1]<<", "<<pBeMathvec[2]<<endl;
  cout<<"Now I am looking for rounding errors"<<endl;
  cout<<"Be8 momentums squared: "<<pBe[0]*pBe[0]<<", "<<pBe[1]*pBe[1]<<", "<<pBe[2]*pBe[2]<<" Sum: "<<(pBe[0]*pBe[0] + pBe[1]*pBe[1] + pBe[2]*pBe[2])<<endl;
  cout<<"Taking an arccos of: "<<pBe[2]<<"/"<<( sqrt( pBe[0]*pBe[0] + pBe[1]*pBe[1] + pBe[2]*pBe[2] ) )<<"="<<pBe[2]/ ( sqrt( pBe[0]*pBe[0] + pBe[1]*pBe[1] + pBe[2]*pBe[2] ) )<<endl;
  cout<<"Finally I am checking against theoretical values"<<endl;
  cout<<"Be8 Energy theory: "<<MathE<<", Calc: "<<Be8Values[0]<<" diff: "<<MathE-Be8Values[0]<<endl;
  cout<<"Be8 Theta theory: "<<MathT<<", Calc: "<<Be8Values[1]*180./pi<<" diff: "<<MathT-(Be8Values[1]*180./pi)<<endl;
  cout<<endl;
  
}
*/
