#include "SiliconHit.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4UnitsTable.hh"
#include "G4AttValue.hh"
#include "G4AttDef.hh"
#include "G4AttCheck.hh"
#include "/home/ryan/nuclear/mine/Colors.h"

G4Allocator<SiliconHit> SiliconHitAllocator;

SiliconHit::SiliconHit()
{}

SiliconHit::~SiliconHit()
{}

SiliconHit::SiliconHit( const SiliconHit &right )
: G4VHit()
{
	edep = right.edep;
	pos = right.pos;
	ParticleName = right.ParticleName;
	VolumeName = right.VolumeName;
}

G4int SiliconHit::operator==( const SiliconHit &right) const
{
	return (this==&right) ? 1 : 0;
}

std::map<G4String, G4AttDef> SiliconHit::fAttDefs;

void SiliconHit::Draw()
{
   G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
   if(pVVisManager)
   {
      G4Circle circle(pos);
      circle.SetScreenSize(0.04);
      circle.SetFillStyle(G4Circle::filled);
      G4Colour colour(1.,0.,0.);
      G4VisAttributes attribs(colour);
      circle.SetVisAttributes(attribs);
      pVVisManager->Draw(circle);
   }
}

const std::map<G4String, G4AttDef>* SiliconHit::GetAttDefs() const
{
	if( fAttDefs.empty() )
	{
		fAttDefs["HitTypes"] = 
			G4AttDef("HitType", "Type of hit", "physics", "", "G4String");
	}
	return &fAttDefs;
}

std::vector<G4AttValue>* SiliconHit::CreateAttValues() const
{
	std::vector<G4AttValue>* attValues = new std::vector<G4AttValue>;
	attValues->push_back(G4AttValue("HitType","ExN04TrackerHit",""));
	return attValues;
}

void SiliconHit::Print()
{
  std::cout<<"SiliconHit Print()"<<std::endl;
  std::cout<<"StripX: "<<StripX<<std::endl;
  std::cout<<"StripY: "<<StripY<<std::endl;
  std::cout<<"edep: "<<edep<<std::endl;
//   pos.Print();
  std::cout<<DRED<<"Pos XYZ: "<<pos.x()<<", "<<pos.y()<<", "<<pos.z()<<RESET_COLOR<<std::endl;
  std::cout<<DGREEN<<"Pos RThetaPhi: "<<pos.mag()<<", "<<pos.theta()*180./3.14159<<", "<<pos.phi()*180./3.14159<<RESET_COLOR<<std::endl;
  std::cout<<"ParticleName: "<<ParticleName<<std::endl;
  std::cout<<"VolumeName: "<<VolumeName<<std::endl;
}
