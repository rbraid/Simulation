#include "DetectorConstruction.hh"
#include "SiliconSD.hh"
#include <math.h>
#include <vector>
#include <cstdio>

using namespace std;

DetectorConstruction::DetectorConstruction()
	:expHallLogical(0),expHallPhysical(0)
{
	pi = 4*atan(1);

	materials = new Material();

	materialOfDetector = 1;
}

DetectorConstruction::~DetectorConstruction()
{
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  G4SDManager *SDman = G4SDManager::GetSDMpointer();
  G4String    SiliconSDname = "/SiDet/Silicon";
  SiliconSD *siliconSD = new SiliconSD( SiliconSDname );
  SDman->AddNewDetector( siliconSD );
  
	materials -> DefineMaterials();

	ExperimentalHall();
	
	Target();
	
// 	MakeDSSD(500*um,70.91*mm,73.52, 0);//side
// 	PixelLogical->SetSensitiveDetector(siliconSD);
// 	MakeDSSD(40*um,65.13*mm,31, 1);//de
// 	PixelLogical->SetSensitiveDetector(siliconSD);
// 	MakeDSSD(1000*um,71.13*mm,31, 2);//e
// 	PixelLogical->SetSensitiveDetector(siliconSD);
// 	
// 	MakeDSSD(500*um,70.91*mm,-73.52, 5);//side
// 	PixelLogical->SetSensitiveDetector(siliconSD);
// 	MakeDSSD(40*um,65.13*mm,-31, 3);//de
// 	PixelLogical->SetSensitiveDetector(siliconSD);
// 	MakeDSSD(500*um,71.13*mm,-31, 4);//e
// 	PixelLogical->SetSensitiveDetector(siliconSD);
    
    MakeDSSDNew(1,'D');
    PixelLogical->SetSensitiveDetector(siliconSD);
    MakeDSSDNew(1,'E');
    PixelLogical->SetSensitiveDetector(siliconSD);
    
    MakeDSSDNew(2,'D');
    PixelLogical->SetSensitiveDetector(siliconSD);
    MakeDSSDNew(2,'E');
    PixelLogical->SetSensitiveDetector(siliconSD);
    
    MakeDSSDNew(3,'D');
    PixelLogical->SetSensitiveDetector(siliconSD);
    
    MakeDSSDNew(4,'D');
    PixelLogical->SetSensitiveDetector(siliconSD);
    
	
	return expHallPhysical;
}

void DetectorConstruction::ExperimentalHall()
{
	G4double expHallSide = 2.0*m;
	G4ThreeVector ExpHallPos;
	ExpHallPos.set(0.0,0.0,0.0);

	G4Box* expHallSolid = new G4Box("Experimental Hall Solid", expHallSide, expHallSide, expHallSide);
	expHallLogical = new G4LogicalVolume(expHallSolid, materials -> Vacuum, "Experimental Hall Logical");
	expHallPhysical = new G4PVPlacement(0, ExpHallPos, expHallLogical, "Experimental Hall", 0, false, 0);

	expHallLogical -> SetVisAttributes(G4VisAttributes::Invisible);

}

void DetectorConstruction::Target()
{
  G4double TargetArialDensity = TARGET_THICKNESS;
  G4double TargetDensity = 1.85/1e-6;
  G4double TargetThickness = TargetArialDensity / TargetDensity;

  G4double Target_X = (5)*mm;
  G4double Target_Y = (5)*mm;
  G4double Target_Z = ( 2.4 )*um;

  G4Box* Target = new G4Box("MyBeTarget", Target_X/2, Target_Y/2, Target_Z/2 );

  TargetLogical = new G4LogicalVolume(Target, materials->Target, "ReactionTarget");

  G4PVPlacement *TargetPlace;
  TargetPlace = new G4PVPlacement(0, G4ThreeVector( 0.0 , 0.0 , 0.0), TargetLogical, "target", expHallLogical, false, 0);

  G4VisAttributes* targetAttributes = new G4VisAttributes(G4Colour::White());
  targetAttributes -> SetForceSolid(true);
  TargetLogical -> SetVisAttributes(targetAttributes);
}

void DetectorConstruction::MakeDSSD(G4double DetZ, G4double PosR, G4double PosTheta, G4int Type)
{
  const G4double pi = 4*atan(1);
  G4double PosRad = PosTheta*pi/180;
  G4double DetX = 50.*mm;
  G4double DetY = 50.*mm;
  G4String Name = "Default";
  G4Colour Color;

  if(Type==0)
    Name="SideR";
  else if(Type==1)
    Name="dER";
  else if(Type==2)
    Name="ER";
  else if(Type==5)
    Name="SideL";
  else if(Type==3)
    Name="dEL";
  else if(Type==4)
    Name="EL";
  
  G4Box* DSSDBox = new G4Box("DSSDBox",DetX/2.,DetY/2.,DetZ/2.);
  G4Box* DSSDStrip = new G4Box("DSSDStrip",DetX/32.,DetY/2.,DetZ/2.);
  G4Box* DSSDPixel = new G4Box("DSSDPixel",DetX/32.,DetY/32.,DetZ/2.);

  G4LogicalVolume *BoxLogical;
  BoxLogical = new G4LogicalVolume(DSSDBox,materials->Silicon,"BoxLogical");
  G4LogicalVolume *StripLogical;
  StripLogical = new G4LogicalVolume(DSSDStrip,materials->Silicon,"StripLogical");
  PixelLogical = new G4LogicalVolume(DSSDPixel,materials->Silicon,"PixelLogical");

  G4ThreeVector *Pos3 = new G4ThreeVector((PosR+DetZ/2.)*sin(PosRad),0.,(PosR+DetZ/2.)*cos(PosRad));

  G4RotationMatrix RMx;

  if(Type<3)
    RMx.rotateY(PosRad-pi);
  else
    RMx.rotateY(PosRad);
  
  if(Type==0) RMx.rotateY((90-PosTheta)*pi/180);
  else if(Type==5) RMx.rotateY(((90-PosTheta)*pi/180)+pi);

  G4PVPlacement *PhysicalDSSD;
  PhysicalDSSD = new G4PVPlacement(G4Transform3D(RMx,*Pos3),BoxLogical,"PhysicalDSSD",expHallLogical,false,1);
  G4PVDivision *PhysicalStrip;
  PhysicalStrip = new G4PVDivision("PhysicalStrip",StripLogical,BoxLogical,kXAxis,16,0.*m);
  G4PVDivision *PhysicalPixel;
  PhysicalPixel = new G4PVDivision(Name,PixelLogical,StripLogical,kYAxis,16,0.*m);
  
  if(Type==0||Type==5)
  {
    Color=G4Colour::Blue();
  }
  else if(Type==1||Type==3)
  {
    Color=G4Colour::Red();
  }
  else if(Type==2||Type==4)
  {
    Color=G4Colour::Green();
  }
  
  G4VisAttributes* MyVis = new G4VisAttributes(Color);
  MyVis->SetForceWireframe(true);
  BoxLogical->SetVisAttributes(MyVis);
  StripLogical->SetVisAttributes(MyVis);
  PixelLogical->SetVisAttributes(MyVis);
}

void DetectorConstruction::MakeDSSDNew(int detector, char pos)
{
  G4double DetX = 50.*mm;
  G4double DetY = 50.*mm;
  G4double DetZ = 0.*mm;
  G4String Name = "Default";
  G4Colour Color = G4Colour::Black();
  
  double detTheta = 31. * (CLHEP::pi/180.);
  
  
  if(detector==3)
  {
    Name="SideR";
    detTheta = CLHEP::pi / 2.;
    DetZ = 500.*um; 
  }
  else if(detector==2 && pos == 'D')
  {
    Name="dER";
//     DetZ = 41.3338*um; 
    DetZ = 34.8293*um; 
  }
  else if(detector==2 && pos == 'E')
  {
    Name="ER";
    DetZ = 1000.*um; 
  }
  else if(detector==4)
  {
    Name="SideL";
    detTheta = CLHEP::pi / 2.;
    
    DetZ = 500.*um; 
  }
  else if(detector==1 && pos == 'D')
  {
    Name="dEL";
    detTheta = -detTheta;
//     DetZ = 34.8293*um; 
    DetZ = 41.3338*um; 
  }
  else if(detector==1 && pos == 'E')
  {
    Name="EL";
    detTheta = -detTheta;
    DetZ = 500.*um; 
  }
  else
  {
    cerr<<"Error, MakeDSSDNew unrecognized det&pos combo"<<endl;
    Name="Undefined";
  }
  
  G4Box* DSSDBox = new G4Box("DSSDBox",DetX/2.,DetY/2.,DetZ/2.);
  G4Box* DSSDStrip = new G4Box("DSSDStrip",DetX/32.,DetY/2.,DetZ/2.);
  G4Box* DSSDPixel = new G4Box("DSSDPixel",DetX/32.,DetY/32.,DetZ/2.);
  
  G4LogicalVolume *BoxLogical;
  BoxLogical = new G4LogicalVolume(DSSDBox,materials->Silicon,"BoxLogical");
  G4LogicalVolume *StripLogical;
  StripLogical = new G4LogicalVolume(DSSDStrip,materials->Silicon,"StripLogical");
  PixelLogical = new G4LogicalVolume(DSSDPixel,materials->Silicon,"PixelLogical");
  
  // 	MakeDSSD(40*um,65.13*mm,-31, 3);//de
  //   void DetectorConstruction::MakeDSSD(G4double DetZ, G4double PosR, G4double PosTheta, G4int Type)
//   G4double DetZ = 40*um;
//   G4double PosR = 65.13*mm;
//   G4double PosTheta = -31;
//   G4double PosRad = -31.*pi/180;
//   G4double DetX = 50.*mm;
//   G4double DetY = 50.*mm;
//   G4ThreeVector *Pos3 = new G4ThreeVector((PosR+DetZ/2.)*sin(PosRad),0.,(PosR+DetZ/2.)*cos(PosRad)); //Points to the center of the detector.
//   G4RotationMatrix RMx; //this is the angle it is rotated relative to the target.
  
  G4ThreeVector CenterOfDetNew = (GetPositionsNew(detector,pos,7,7)[4] + GetPositionsNew(detector,pos,7,8)[4] + GetPositionsNew(detector,pos,8,7)[4] +GetPositionsNew(detector,pos,8,8)[4])/4.;
  G4ThreeVector CenterOfDet = (GetPosition(detector,pos,7,7) + GetPosition(detector,pos,7,8) + GetPosition(detector,pos,8,7) +GetPosition(detector,pos,8,8))/4.;
  
//   G4double factor = 1.5;
//   CenterOfDet.setX(CenterOfDet.x()*factor);
//   CenterOfDet.setZ(CenterOfDet.z()*factor);
  
  
//   cout<<"Position Checks for "<<Name<<endl;
// //   cout<<" Ancient XYZ: "<<Pos3->getX()<<", "<<Pos3->getY()<<", "<<Pos3->getZ()<<endl;
//   cout<<" Old XYZ: "<<CenterOfDet.getX()<<", "<<CenterOfDet.getY()<<", "<<CenterOfDet.getZ()<<endl;
//   cout<<" New XYZ: "<<CenterOfDetNew.getX()<<", "<<CenterOfDetNew.getY()<<", "<<CenterOfDetNew.getZ()<<endl;
//   
// //   cout<<" Ancient Theta,Phi,R: "<<Pos3->getTheta()<<", "<<Pos3->getPhi()<<", "<<Pos3->getR()<<endl;
//   cout<<" Old Theta,Phi,R: "<<CenterOfDet.getTheta()*180./3.14159<<", "<<CenterOfDet.getPhi()*180./3.14159<<", "<<CenterOfDet.getR()<<endl;
//   cout<<" New Theta,Phi,R: "<<CenterOfDetNew.getTheta()*180./3.14159<<", "<<CenterOfDetNew.getPhi()*180./3.14159<<", "<<CenterOfDetNew.getR()<<endl;
//   
  G4RotationMatrix RMx;
  RMx.rotateY(detTheta);
  
//   RMx.Print();
//   
//   G4Transform3D(RMx,CenterOfDet);
//   G4Transform3D(RMx,CenterOfDetNew);
//   cout<<"Position Checks after rotation for "<<Name<<endl;
//   //   cout<<" Ancient XYZ: "<<Pos3->getX()<<", "<<Pos3->getY()<<", "<<Pos3->getZ()<<endl;
//   cout<<" Old XYZ: "<<CenterOfDet.getX()<<", "<<CenterOfDet.getY()<<", "<<CenterOfDet.getZ()<<endl;
//   cout<<" New XYZ: "<<CenterOfDetNew.getX()<<", "<<CenterOfDetNew.getY()<<", "<<CenterOfDetNew.getZ()<<endl;
//   
//   //   cout<<" Ancient Theta,Phi,R: "<<Pos3->getTheta()<<", "<<Pos3->getPhi()<<", "<<Pos3->getR()<<endl;
//   cout<<" Old Theta,Phi,R: "<<CenterOfDet.getTheta()<<", "<<CenterOfDet.getPhi()<<", "<<CenterOfDet.getR()<<endl;
//   cout<<" New Theta,Phi,R: "<<CenterOfDetNew.getTheta()<<", "<<CenterOfDetNew.getPhi()<<", "<<CenterOfDetNew.getR()<<endl;
  G4PVPlacement *PhysicalDSSD;
  PhysicalDSSD = new G4PVPlacement(G4Transform3D(RMx,CenterOfDet),BoxLogical,"PhysicalDSSD",expHallLogical,false,1);
  G4PVDivision *PhysicalStrip;
  
  
  PhysicalStrip = new G4PVDivision("PhysicalStrip",StripLogical,BoxLogical,kXAxis,16,0.*m);
  G4PVDivision *PhysicalPixel;
  PhysicalPixel = new G4PVDivision(Name,PixelLogical,StripLogical,kYAxis,16,0.*m);
  
  if(detector==3||detector==4)
  {
    Color=G4Colour::Blue();
  }
  else if(pos=='D')
  {
    Color=G4Colour::Red();
  }
  else if(pos=='E')
  {
    Color=G4Colour::Green();
  }
  else
  {
    cerr<<"Error, MakeDSSDNew no color given"<<endl;
    Color=G4Colour::White();
  }
  
  G4VisAttributes* MyVis = new G4VisAttributes(Color);
  MyVis->SetForceWireframe(true);
  BoxLogical->SetVisAttributes(MyVis);
  StripLogical->SetVisAttributes(MyVis);
  PixelLogical->SetVisAttributes(MyVis);
}

G4ThreeVector DetectorConstruction::GetPosition(int detector,char pos, int horizontalstrip, int verticalstrip, double X, double Y, double Z)
{
  //horizontal strips collect N charge!
  //vertical strips collect P charge!
  horizontalstrip -=1;
  
  G4ThreeVector Pos;
  double detTheta = -31. * (CLHEP::pi/180.);
  double SideX = 64.62;
  double SideZ = 2.05;
  double dER = 62.14;
  double ER = 75.35;
  double x = 0.0,y = 0.0,z = 0.0;
  
  pos = toupper(pos);
  
  if(pos != 'D' && pos != 'E')
    std::cerr<<"Unrecognized pos: "<<pos<<std::endl;
  
  if(detector == 1)
    detTheta = -detTheta;
  if(pos=='E')
    verticalstrip=15-verticalstrip;
  
  if(detector == 1)
    detTheta -= 2* (CLHEP::pi/180.);
  
  x = (50./32.)*(2*verticalstrip+1) - (50./16.)*8;
  y = (50./32.)*(2*horizontalstrip+1) - (50/16.)*8;
  
  if(pos=='D')
    z = dER;
  else
    z = ER;
  
  double xp = z*sin(detTheta) + x*cos(detTheta);
  double zp = z*cos(detTheta) - x*sin(detTheta);
  
  if(detector==3&&pos=='D')
  {
    //Right Side
    verticalstrip=15-verticalstrip;
    xp = SideX;
    zp = SideZ + (50./32.)*(2*verticalstrip+1) - (50/16.)*8;
  }
  else if(detector==4&&pos=='D')
  {
    //Left Side
    xp = -SideX;
    zp = SideZ + (50./32.)*(2*verticalstrip+1) - (50/16.)*8;
  }
  
  Pos.setX(xp + X);
  Pos.setY(y + Y);
  Pos.setZ(zp+ Z);
  
//   Pos.setX(Pos.getX()*1.15);
//   Pos.setZ(Pos.getZ()*1.15);
  
  Pos.setPhi(Pos.getPhi() + CLHEP::pi);
  Pos.setY(0);
  
  return(Pos);
}

G4ThreeVector* DetectorConstruction::GetPositionsNew(int detector,char pos, int horizontalstrip, int verticalstrip)
{
  //horizontal strips collect N charge!
  //vertical strips collect P charge!

  //all dE detectors are flipped compared to the E detectors.
  
  //for side detectors, strip 0 has lowest theta in detector 3
  //strip 15 has lowest theta for detector 4
  
  G4ThreeVector Pos;
  G4ThreeVector horMove;
  G4ThreeVector vertMove;
  G4ThreeVector *RetPos = new G4ThreeVector[5];
  double detTheta = 31. * (CLHEP::pi/180.);
  double SideX = 66.255;
  double SideZ = 12.99;
  double dER = 58.98-1.5;
  double ER = 70.99-1.5;
  double x = 0.0,y = 0.0,z = 0.0;
  
  double halfpixel = 50./32.;
  double fullpixel = 50./16.;
  
  if(detector<3)
  {
    x = (verticalstrip-8) * fullpixel  +  halfpixel;
    y = (horizontalstrip-8) * fullpixel  +  halfpixel;
    
    if(pos == 'D')
      z = dER;
    else if(pos == 'E')
      z = ER;
    else
    {
      cerr<<" Unrecognized position: "<<pos<<", reverting to dE to fail safe."<<endl;
      z = dER;
    }
    if(detector==1)
    {
      x=-x;
      detTheta = -detTheta;
    }
    Pos.set(x,y,z);
    
    Pos.setX(Pos.getX()*1.15);
    Pos.setZ(Pos.getZ()*1.15);
    
    vertMove.set(0.,halfpixel,0.);
    horMove.set(halfpixel,0.,0.);
    
    RetPos[0] = Pos + vertMove + horMove;
    RetPos[1] = Pos - vertMove + horMove;
    RetPos[2] = Pos - vertMove - horMove;
    RetPos[3] = Pos + vertMove - horMove;
    
    RetPos[4] = Pos;
    
    RetPos[0].rotateY(detTheta);
    RetPos[1].rotateY(detTheta);
    RetPos[2].rotateY(detTheta);
    RetPos[3].rotateY(detTheta);
    RetPos[4].rotateY(detTheta);
  }
  
  else
  {
    //According to the SolidWorks diagram, the centerline for the side detectors should be 12.01 mm from rear edge of detector.
    //12.01/fullpixel = 3.84
    //center of 
    if(detector==4)
    {
      x = SideX;
      verticalstrip = -verticalstrip+15;
    }
    else if(detector==3)
    {
      x = -SideX;
    }
    y = (horizontalstrip-8) * fullpixel  +  halfpixel;
    z = (verticalstrip-8) * fullpixel  +  halfpixel;
    
    z += SideZ;
    
    Pos.set(x,y,z);
    vertMove.set(0.,halfpixel,0.);
    horMove.set(0.,0.,halfpixel);
    
    RetPos[0] = Pos + vertMove + horMove;
    RetPos[1] = Pos - vertMove + horMove;
    RetPos[2] = Pos - vertMove - horMove;
    RetPos[3] = Pos + vertMove - horMove;
    
    RetPos[4] = Pos;
  }
  
  return(RetPos);
}