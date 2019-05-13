#include "SiliconSD.hh"
#include "SiliconHit.hh"
#include "G4Step.hh"
#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4ios.hh"
#include <cstdio>
#include "DetectorConstruction.hh"
#include "/home/ryan/nuclear/mine/Colors.h"

using namespace std;


SiliconSD::SiliconSD( G4String name )
:G4VSensitiveDetector(name)
{
	G4String HCname;
	collectionName.insert(HCname="siliconCollection");
}

SiliconSD::~SiliconSD()
{}

void SiliconSD::Initialize( G4HCofThisEvent *HCE )
{
	static int HCID = -1;
	siliconCollection = new SiliconHitsCollection( SensitiveDetectorName, collectionName[0] );
	
	if( HCID < 0 )
	{
		HCID = GetCollectionID(0);
	}
	HCE->AddHitsCollection(HCID, siliconCollection);
}

G4bool SiliconSD::ProcessHits( G4Step *step, G4TouchableHistory* )
{
  bool DEBUG = 0;
  
	G4Track *track = step->GetTrack(); 	
	
	G4double edep = step->GetTotalEnergyDeposit();
	
   G4String particleName = track -> GetDefinition() -> GetParticleName();
	G4String volumeName = track -> GetVolume() -> GetName();
	G4String nextvolumeName = track ->GetNextVolume() -> GetName();
	
	G4StepPoint* preStepPoint = step->GetPreStepPoint();
	//G4TouchableHistory* theTouchable = (G4TouchableHistory*)(preStepPoint->GetTouchable());
	
	G4TouchableHandle touch1 = preStepPoint->GetTouchableHandle();
	//std::cout<<touch1->GetCopyNumber(0)<<" "<<touch1->GetCopyNumber(1)<<std::endl;
	
	G4int copyNo = touch1->GetCopyNumber(0);//thePhysical->GetCopyNo();
	G4int momcopyNo = touch1->GetCopyNumber(1);//thePhysical->GetCopyNo(1);
	
	SiliconHit *newHit = new SiliconHit();
	newHit->SetStripX(momcopyNo);
	newHit->SetStripY(copyNo);
        
    if( !(strncmp("E", volumeName,1))) 
    {
//       newHit->SetStripX(15-momcopyNo);
//       newHit->SetStripY(15-copyNo);
    }
	newHit->SetEdep( edep );
	newHit->SetPos( step->GetPreStepPoint()->GetPosition() );
 
	newHit->SetParticleName( particleName );
	newHit->SetVolumeName( volumeName );

	siliconCollection->insert( newHit );
//     std::cout<<"newHit->GetVolumeName(): "<<newHit->GetVolumeName()<<std::endl;
//     if(newHit->GetVolumeName().contains('R'))
//       std::cout<<"R"<<std::endl;
    
//     int det = 2;
//     if(newHit->GetVolumeName().contains('R'))
//       det = 1;
//     
//     char type = 'E';
//     if(newHit->GetVolumeName().contains('d'))
//       type = 'D';
//     
//     bool side = 0;
//     if(newHit->GetVolumeName().contains('S'))
//       side = 1;
    
	return true;
}

void SiliconSD::EndOfEvent( G4HCofThisEvent* )
{}

void SiliconSD::clear()
{}

void SiliconSD::DrawAll()
{}

void SiliconSD::PrintAll()
{}

G4ThreeVector GetPosition(int detector,char pos, int horizontalstrip, int verticalstrip, double X, double Y, double Z)
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
  
  Pos.setPhi(Pos.getPhi() + CLHEP::pi);
  
  return(Pos);
}

// int* GetStrips(double theta, double phi)
// {
//   int retInt[2];
//   retInt[0] = -1;
//   retInt[1] = -1;
//   
//   G4ThreeVector Spos;
//   
//   Spos.setRThetaPhi(1,theta,phi);
//   
//   Spos.rotateY(31.);
// }