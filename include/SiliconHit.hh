#ifndef SiliconHit_h
#define SiliconHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

class G4AttDef;

class SiliconHit : public G4VHit
{
	public:
		SiliconHit();
		~SiliconHit();
		SiliconHit( const SiliconHit &right );
		const SiliconHit& operator=(const SiliconHit &right);
		G4int operator==(const SiliconHit &right) const;
		
		inline void *operator new(size_t);
		inline void operator delete(void *aHit);
		
		void Draw();
		const std::map<G4String, G4AttDef>* GetAttDefs() const;
		std::vector<G4AttValue>* CreateAttValues() const;
		void Print();
		
	private:
		G4int StripX;
		G4int StripY;
		G4double 		edep;
		G4ThreeVector 	pos;
		G4String		  	ParticleName;
		G4String			VolumeName;
		static std::map<G4String, G4AttDef> fAttDefs;
		
	public:
		inline void SetStripX(G4int st)
			{StripX = st;}
		inline G4int GetStripX()
			{return StripX;}
		inline void SetStripY(G4int st)
			{StripY = st;}
		inline G4int GetStripY()
			{return StripY;}
		inline void SetEdep(G4double de)
			{ edep = de; }
		inline G4double GetEdep()
			{ return edep; }
		inline void SetPos( G4ThreeVector xyz )
			{ pos = xyz; }
		inline G4ThreeVector GetPos()
			{ return pos; }
		inline void SetParticleName( G4String parName )
			{ ParticleName = parName; }				
		inline G4String GetParticleName()
			{ return ParticleName; }
		inline void SetVolumeName( G4String volName )
			{ VolumeName = volName; }				
		inline G4String GetVolumeName()
			{ return VolumeName; }			
};

typedef G4THitsCollection<SiliconHit> SiliconHitsCollection;

extern G4Allocator<SiliconHit> SiliconHitAllocator;

inline void* SiliconHit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) SiliconHitAllocator.MallocSingle();
  return aHit;
}

inline void SiliconHit::operator delete(void *aHit)
{
  SiliconHitAllocator.FreeSingle((SiliconHit*) aHit);
}

#endif

