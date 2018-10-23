#include "PrimaryGenerator.hh"
#include <iostream>
#include <cmath>
#include "G4RandomDirection.hh"

using namespace std;

G4ParticleGun* PrimaryGenerator::particleGun(0);

PrimaryGenerator::PrimaryGenerator( struct DataPass *Be8InfoPass )
{
    mBe8Energy = &(Be8InfoPass->Be8Energy);
    mBe8Theta = &(Be8InfoPass->Be8Theta);
    mBe8ThetaCOM = &(Be8InfoPass->Be8ThetaCOM);
    mBe8Phi = &(Be8InfoPass->Be8Phi);
    mBe8X = &(Be8InfoPass->Be8X);
    mBe8Y = &(Be8InfoPass->Be8Y);
    mBe8Z =	&(Be8InfoPass->Be8Z);
    mBe8ExciteE = &(Be8InfoPass->Be8ExciteE);

    mBe12Energy = &(Be8InfoPass->Be12Energy);
    mBe12Theta = &(Be8InfoPass->Be12Theta);
    mBe12ThetaCOM = &(Be8InfoPass->Be12ThetaCOM);
//     mBe12PhiCOM = &(Be8InfoPass->Be12PhiCOM);
    mBe12Phi = &(Be8InfoPass->Be12Phi);
    mBe12X = &(Be8InfoPass->Be12X);
    mBe12Y = &(Be8InfoPass->Be12Y);
    mBe12Z = &(Be8InfoPass->Be12Z);
    mBe12ExciteE = &(Be8InfoPass->Be12ExciteE);

    mA1Energy = &(Be8InfoPass->A1Energy);
    mA1Theta = &(Be8InfoPass->A1Theta);
    mA1ThetaCOM = &(Be8InfoPass->A1ThetaCOM);
    mA1Phi = &(Be8InfoPass->A1Phi);
    mA1X = &(Be8InfoPass->A1X);
    mA1Y = &(Be8InfoPass->A1Y);
    mA1Z = &(Be8InfoPass->A1Z);

    mA2Energy = &(Be8InfoPass->A2Energy);
    mA2Theta = &(Be8InfoPass->A2Theta);
    mA2ThetaCOM = &(Be8InfoPass->A2ThetaCOM);
    mA2Phi = &(Be8InfoPass->A2Phi);
    mA2X = &(Be8InfoPass->A2X);
    mA2Y = &(Be8InfoPass->A2Y);
    mA2Z = &(Be8InfoPass->A2Z);

    G4String nameOfParticles = "proton";

    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    particleGun = new G4ParticleGun(1);
    particleGun -> SetParticleDefinition(particleTable -> FindParticle(nameOfParticles));

    particleGun -> SetParticlePosition(G4ThreeVector(0.0, 0.0, 0.0));
    
}


PrimaryGenerator::~PrimaryGenerator()
{
    delete particleGun;
}


void PrimaryGenerator::GeneratePrimaries(G4Event* event)
{
  bool DEBUGPG = 0;
  if(DEBUGPG) cout<<endl;
    //G4double pi = 4.0*atan(1.0);

    //G4double randX = ((G4UniformRand()-0.5)*5.0)*CLHEP::mm;
    //G4double randY = ((G4UniformRand()-0.5)*5.0)*CLHEP::mm;

    G4double sigma = 1.215;
    G4double randX = G4RandGauss::shoot(0.0,sigma)*CLHEP::mm;
    G4double randY = G4RandGauss::shoot(0.0,sigma)*CLHEP::mm;

    G4double TAreaDensity = TARGET_THICKNESS;
    G4double TDensity = 1.85/1e-6;
    G4double TThickness = TAreaDensity/TDensity;
    G4double randZ = TThickness*(G4UniformRand()-0.5)*CLHEP::cm;
    if(BEAM)
      particleGun -> SetParticlePosition(G4ThreeVector(randX, randY, randZ));
    else
      particleGun -> SetParticlePosition(G4ThreeVector(0,0,0));

    G4ThreeVector PVecDirection;

    //cout<<"{"<<randX<<","<<randY<<"}, ";

    double x,y,z;
    double E, theta, phi;
    G4double *kinematics = mCalcLightProdKinematics();

//      cout<<"KINEMATICS OUTPUT:"<<endl;
//      cout<<kinematics[0]<<endl;
//      cout<<kinematics[1]<<endl;
//      cout<<kinematics[2]<<endl;
//      cout<<kinematics[3]<<endl;
//      cout<<kinematics[4]<<endl;
//      cout<<kinematics[5]<<endl;
//      cout<<kinematics[6]<<endl;
//      cout<<kinematics[7]<<endl;
//      cout<<kinematics[8]<<endl;
    

//------------------  Test Particle -------------//
   /* E = 10.*MeV;
    theta = 51.;
    phi = G4UniformRand()*360.;
    particleGun -> SetParticleEnergy(E);
    
    x = sin(theta)*cos(phi);
    y = sin(theta)*sin(phi);
    z = cos(theta);
    
    PVecDirection.set( x, y, z );
    particleGun -> SetParticleMomentumDirection(PVecDirection);
    
    G4int Z = 2, A = 4;
    G4double ioncharge = 0.0*eplus;
    G4double excitEnergy = 0.*keV;
    
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition *ion = particleTable->GetIon(Z,A,excitEnergy);
    particleGun->SetParticleDefinition( ion );
    particleGun->SetParticleCharge( ioncharge );
    
    particleGun -> GeneratePrimaryVertex(event);*/

//------------------  10Be 1 -------------//
    

    E = kinematics[0]*CLHEP::MeV;
    theta = kinematics[1];
    phi = kinematics[2];
    particleGun -> SetParticleEnergy(E);

    x = sin(theta)*cos(phi);
    y = sin(theta)*sin(phi);
    z = cos(theta);

    PVecDirection.set( x, y, z );
    particleGun -> SetParticleMomentumDirection(PVecDirection);

    G4int Z = 4, A = 10;
    G4double ioncharge = 0.0*CLHEP::eplus;
    G4double excitEnergy = 0.*CLHEP::keV;

    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition *ion = particleTable->GetIon(Z,A,excitEnergy);
    particleGun->SetParticleDefinition( ion );
    particleGun->SetParticleCharge( ioncharge );
    
    particleGun -> GeneratePrimaryVertex(event);
    
    if(DEBUGPG) cout<<"PrimaryGenerator 10BeA Theta: "<<theta*180./3.14159<<", Phi: "<<phi*180./3.14159<<endl;

    *mA1Energy=E;
    *mA1Theta=theta;
    *mA1Phi=phi;
    //*mA1X=x;
    //*mA1Y=y;
    //*mA1Z=z;

//------------------  10Be 2 -------------//

    E = kinematics[3]*CLHEP::MeV;
    theta = kinematics[4];
    phi = kinematics[5];

    particleGun -> SetParticleEnergy(E);
    x = sin(theta)*cos(phi);
    y = sin(theta)*sin(phi);
    z = cos(theta);

    PVecDirection.set( x, y, z );
    particleGun -> SetParticleMomentumDirection(PVecDirection);

    //Create the 12Be beam ion
    Z = 4;
    A = 10;
    ioncharge = 0.0*CLHEP::eplus;
    excitEnergy = 0.*CLHEP::keV;

    particleTable = G4ParticleTable::GetParticleTable();
    ion = particleTable->GetIon(Z,A,excitEnergy);
    particleGun->SetParticleDefinition( ion );
    particleGun->SetParticleCharge( ioncharge );

    particleGun -> GeneratePrimaryVertex(event);

    if(DEBUGPG) cout<<"PrimaryGenerator 10BeB Theta: "<<theta*180./3.14159<<", Phi: "<<phi*180./3.14159<<endl;
    
    *mBe12Energy = E;
    *mBe12Theta = theta;
    *mBe12Phi = phi;
    //*mBe12X = x;
    //*mBe12Y = y;
    //*mBe12Z = z;
    
}

G4double *PrimaryGenerator::mCalcLightProdKinematics()
{

  bool DEBUGKIN = 0;
  bool CRAZYDISTS = 1;
  
  G4double *kinematics = new G4double[9];

  double MBeam = MASS_BE11;
  double EBeam = BEAM_ENERGY;

  double MTarget = MASS_BE9;

  double MWhole = MASS_BE10;
  double MSplit = MASS_BE10;

  double QVal = MBeam + MTarget - MWhole - MSplit;
  if(DEBUGKIN) cout<<"QVal: "<<QVal<<" NNDC Ref: 6.310"<<endl;
  
  const int nbins(360);
  
  double scalegg = 0.00345167* 100.;
  double scale3g = 0.0145617 * 100.;
  double scale6g = 0.156889  * 100.;
  double scale33 = 0.156889  * 100.;
  double scale63 = 0.30612   * 100.;
  double scale66 = 0.362089  * 100.;
  

  double WholeEx = -1000.;
  double SplitEx = -1000.;
  
  double RandExVal = CLHEP::RandFlat::shoot(0.,100.);
  
  double VelCOMTheta = 0;
  
  if(RandExVal < 0)
  {
    cerr<<"Error in Excitation Values, Random Value out of range: "<<RandExVal<<", reverting to ground state"<<endl;
    WholeEx = 0.;
    SplitEx = 0.;
    VelCOMTheta = CLHEP::RandFlat::shoot(0.,1.);
    
  }
  else if(RandExVal < scalegg)
  {
    WholeEx = 0.;
    SplitEx = 0.;
    CLHEP::RandGeneral rgen0(pyramid,nbins);
    VelCOMTheta = rgen0.fire(); //right now i span 0 to 1
//     VelCOMTheta *= CLHEP::pi;  
  }
  else if(RandExVal < scalegg + scale3g)
  {
    WholeEx = 3.368;
    SplitEx = 0.;
    CLHEP::RandGeneral rgen0(sawtoothInc2,nbins);
    VelCOMTheta = rgen0.fire(); //right now i span 0 to 1
//     VelCOMTheta *= CLHEP::pi;  
  }
  else if(RandExVal < scalegg + scale3g + scale6g)
  {
    WholeEx = CLHEP::RandFlat::shoot(5.958,6.263);
    SplitEx = 0.;
    CLHEP::RandGeneral rgen0(pyramidInv,nbins);
    VelCOMTheta = rgen0.fire(); //right now i span 0 to 1
//     VelCOMTheta *= CLHEP::pi;  
  }
  else if(RandExVal < scalegg + scale3g + scale6g + scale33)
  {
    WholeEx = 3.368;
    SplitEx = 3.368;
    CLHEP::RandGeneral rgen0(sawtoothDec2,nbins);
    VelCOMTheta = rgen0.fire(); //right now i span 0 to 1
//     VelCOMTheta *= CLHEP::pi;  
  }
  else if(RandExVal <= scalegg + scale3g + scale6g + scale33 + scale63)
  {
    WholeEx = CLHEP::RandFlat::shoot(5.958,6.263);
    SplitEx = 3.368;
    CLHEP::RandGeneral rgen0(pyramidInv,nbins);
    VelCOMTheta = rgen0.fire(); //right now i span 0 to 1
//     VelCOMTheta *= CLHEP::pi;  
  }
  else if(RandExVal <= scalegg + scale3g + scale6g + scale33 + scale63 + scale66)
  {
    WholeEx = CLHEP::RandFlat::shoot(5.958,6.263);
    SplitEx = CLHEP::RandFlat::shoot(5.958,6.263);
    CLHEP::RandGeneral rgen0(pyramidInv,nbins);
    VelCOMTheta = rgen0.fire(); //right now i span 0 to 1
//     VelCOMTheta *= CLHEP::pi;  
  }
  else if(RandExVal > 100)
  {
    cerr<<"Error in Excitation Values, Random Value out of range: "<<RandExVal<<", reverting to ground state"<<endl;
    WholeEx = 0.;
    SplitEx = 0.;
    VelCOMTheta = CLHEP::RandFlat::shoot(0.,1.);
  }
//   VelCOMTheta *= CLHEP::pi;
  //VelCOMTheta spans 0 to 1 now
  VelCOMTheta = 2*VelCOMTheta - 1;
  //VelCOMTheta spans -1 to 1 now
  VelCOMTheta = acos(VelCOMTheta);//add in phi-compression compensation;
  
  if(!CRAZYDISTS)
    VelCOMTheta = acos(CLHEP::RandFlat::shoot(-1,1));
  
  //****************************************
//   WholeEx = 0.;
//   SplitEx = 0.;
//   int dist = 4;
//   if(dist == 0)
//     VelCOMTheta = CLHEP::RandFlat::shoot(0.,1.);
//   
//   else if(dist ==1)
//   {
//     CLHEP::RandGeneral rgen0(pyramidInv,nbins);
//     VelCOMTheta = rgen0.fire();
//   }
//   else if(dist ==2)
//   {
//     CLHEP::RandGeneral rgen0(pyramid,nbins);
//     VelCOMTheta = rgen0.fire();
//   }
//   else if(dist ==3)
//   {
//     CLHEP::RandGeneral rgen0(sawtoothDec2,nbins);
//     VelCOMTheta = rgen0.fire();
//   }
//   else if(dist ==4)
//   {
//     CLHEP::RandGeneral rgen0(sawtoothInc2,nbins);
//     VelCOMTheta = rgen0.fire();
//   }  
//   VelCOMTheta = 2*VelCOMTheta - 1;
//   
//   VelCOMTheta = acos(VelCOMTheta);
  
  //****************************************
  
  if(DEBUGKIN) cout<<"WholeEx: "<<WholeEx<<", SplitEx: "<<SplitEx<<endl;
  
  if(CLHEP::RandFlat::shootBit())
  {
    double temp = WholeEx;
    WholeEx = SplitEx;
    SplitEx = temp;
  }

  if(DEBUGKIN) cout<<"WholeEx: "<<WholeEx<<endl;

  G4ThreeVector COMVel(0.,0.,(MBeam)/(MBeam+MTarget)*sqrt(2.*EBeam/MBeam));
  if(DEBUGKIN) cout<<"Mag of COMVel: "<<COMVel.mag()<<endl;
  double COME = MTarget/(MBeam + MTarget)*EBeam;
  if(DEBUGKIN) cout<<"Center of Mass Energy: "<<COME<<endl;

  G4ThreeVector VelWholeCOM;

  double VelWholeCOMmag = sqrt((2*(COME+QVal-WholeEx-SplitEx)*MSplit)/(MWhole*(MWhole+MSplit)));
  
  double VelCOMPhi = CLHEP::RandFlat::shoot(CLHEP::twopi);
  
  VelWholeCOM.setRThetaPhi( VelWholeCOMmag, VelCOMTheta, VelCOMPhi);
    
  G4ThreeVector VelSplitCOM = -MWhole/MSplit*VelWholeCOM;

  if(DEBUGKIN) cout<<"Whole COM info:"<<endl;
  if(DEBUGKIN) cout<<"Vector: "<<VelWholeCOM<<endl;
  if(DEBUGKIN) cout<<"Energy: "<<.5*MWhole*VelWholeCOM.mag2()<<endl;
  if(DEBUGKIN) cout<<"Theta: "<<VelWholeCOM.theta()*180./CLHEP::pi<<endl;
  if(DEBUGKIN) cout<<"Phi: "<<VelWholeCOM.phi()*180./CLHEP::pi<<endl;

  if(DEBUGKIN) cout<<"Split COM info:"<<endl;
  if(DEBUGKIN) cout<<"Vector: "<<VelSplitCOM<<endl;
  if(DEBUGKIN) cout<<"Energy: "<<.5*MSplit*VelSplitCOM.mag2()<<endl;
  if(DEBUGKIN) cout<<"Theta: "<<VelSplitCOM.theta()*180./CLHEP::pi<<endl;
  if(DEBUGKIN) cout<<"Phi: "<<VelSplitCOM.phi()*180./CLHEP::pi<<endl;
  
  if(DEBUGKIN) cout<<"Sum: "<<(.5*MWhole*VelWholeCOM.mag2()+.5*MSplit*VelSplitCOM.mag2())<<endl;
  if(DEBUGKIN) cout<<"Relative Angle: "<<VelWholeCOM.angle(VelSplitCOM)<<" NOTE: should always be pi"<<endl;

  G4ThreeVector VelWhole = VelWholeCOM + COMVel;
  G4ThreeVector VelSplit = VelSplitCOM + COMVel;

  if(DEBUGKIN) cout<<"Lab Energy Whole: "<<.5*MWhole*VelWhole.mag2()<<endl;
  if(DEBUGKIN) cout<<"Lab Energy Split: "<<.5*MSplit*VelSplit.mag2()<<endl;
  if(DEBUGKIN) cout<<"Sum Lab Energy: "<<(.5*MWhole*VelWhole.mag2()+.5*MSplit*VelSplit.mag2())<<endl;
  if(DEBUGKIN) cout<<endl;

  *mBe8Energy = .5*MSplit*VelSplit.mag2();
  *mBe8Theta = VelSplit.theta();
  *mBe8ThetaCOM = VelSplitCOM.theta();
  *mBe8Phi = VelSplit.phi();
  *mBe8X = VelSplit.x();
  *mBe8Y = VelSplit.y();
  *mBe8Z = VelSplit.z();
  *mBe12ThetaCOM = VelCOMTheta;
  
  *mBe12ExciteE = WholeEx;
  *mBe8ExciteE = SplitEx;

  kinematics[3] = .5*MWhole*VelWhole.mag2();
  kinematics[4] = VelWhole.theta();
  kinematics[5] = VelWhole.phi();

  kinematics[0] = .5*MSplit*VelSplit.mag2();
  kinematics[1] = VelSplit.theta();
  kinematics[2] = VelSplit.phi();
  
//   if(kinematics[0] < kinematics[3]) // This code makes it so the first particle is the higher energy particle.  This should not be in production!
//   {
//     kinematics[0] = .5*MWhole*VelWhole.mag2();
//     kinematics[1] = VelWhole.theta();
//     kinematics[2] = VelWhole.phi();
//     
//     kinematics[3] = .5*MSplit*VelSplit.mag2();
//     kinematics[4] = VelSplit.theta();
//     kinematics[5] = VelSplit.phi();
//     
//     *mBe8Energy = .5*MWhole*VelWhole.mag2();
//     *mBe8Theta = VelWhole.theta();
//     *mBe8ThetaCOM = VelWholeCOM.theta();
//     *mBe8Phi = VelWhole.phi();
//     *mBe8X = VelWhole.x();
//     *mBe8Y = VelWhole.y();
//     *mBe8Z = VelWhole.z();
//     //*mBe12ThetaCOM = VelCOMTheta;
//     
//     *mBe12ExciteE = SplitEx;
//     *mBe8ExciteE = WholeEx;
//   }

  
  return kinematics;
}
