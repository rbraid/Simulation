#include "SiliconSD.hh"
#include "SiliconHit.hh"
#include "G4Step.hh"
#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4ios.hh"
#include <cstdio>


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
	G4Track *track = step->GetTrack(); 	
	
	G4double edep = step->GetTotalEnergyDeposit();
   
	if( edep==0.0)
		return false;
	
   G4String particleName = track -> GetDefinition() -> GetParticleName();
	G4String volumeName = track -> GetVolume() -> GetName();
	G4String nextvolumeName = track ->GetNextVolume() -> GetName();
	
	G4StepPoint* preStepPoint = step->GetPreStepPoint();
	//G4TouchableHistory* theTouchable = (G4TouchableHistory*)(preStepPoint->GetTouchable());
	
	G4TouchableHandle touch1 = preStepPoint->GetTouchableHandle();
	//std::cout<<touch1->GetCopyNumber(0)<<" "<<touch1->GetCopyNumber(1)<<std::endl;
	
	G4int copyNo = touch1->GetCopyNumber(0);//thePhysical->GetCopyNo();
	G4int momcopyNo = touch1->GetCopyNumber(1);//thePhysical->GetCopyNo(1);
	//std::cout<<momcopyNo<<" "<<copyNo<<std::endl;
	
	SiliconHit *newHit = new SiliconHit();
	newHit->SetStripX(momcopyNo);
	newHit->SetStripY(copyNo);
    if( !(strncmp("E", volumeName,1))) 
    {
      newHit->SetStripX(15-momcopyNo);
      newHit->SetStripY(15-copyNo);
    }
	newHit->SetEdep( edep );
	newHit->SetPos( step->GetPreStepPoint()->GetPosition() );
	newHit->SetParticleName( particleName );
	newHit->SetVolumeName( volumeName );
	siliconCollection->insert( newHit );
	
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
