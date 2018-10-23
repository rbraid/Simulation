#ifndef SiliconSD_h
#define SiliconSD_h 1

#include "G4VSensitiveDetector.hh"
#include "SiliconHit.hh"
class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

G4ThreeVector GetPosition(int detector,char pos, int horizontalstrip, int verticalstrip, double X=0, double Y=0, double Z=0);

class SiliconSD : public G4VSensitiveDetector
{
	public:
		SiliconSD(G4String name);
		~SiliconSD();
		
		void Initialize(G4HCofThisEvent *HCE);
		G4bool ProcessHits( G4Step *step, G4TouchableHistory *ROhist);
		void EndOfEvent( G4HCofThisEvent *HCE);
		void clear();
		void DrawAll();
		void PrintAll();
		
	private:
      SiliconHitsCollection *siliconCollection;		
};

#endif
