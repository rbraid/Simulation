#ifndef DetectorConstruction_H
#define DetectorConstruction_H 1

#include "G4Box.hh"
#include "G4Tet.hh"
#include "G4Tubs.hh"
#include "G4Colour.hh"
#include "G4Polyhedra.hh"
#include "G4UnionSolid.hh"
#include "G4PVPlacement.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4VisAttributes.hh"
#include "G4PVParameterised.hh"
#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4CylindricalSurface.hh"
#include "G4SDManager.hh"
#include "G4PVDivision.hh"

#include "Randomize.hh"

#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"

#include "Material.hh"
#include "GlobalSettings.hh"


class G4Material;
class G4LogicalVolume;
class G4VPhysicalVolume;
class DetectorConstruction:public G4VUserDetectorConstruction
{

public:
	DetectorConstruction();
	virtual ~DetectorConstruction();
	virtual G4VPhysicalVolume* Construct();

private:
	void ExperimentalHall();
	void Target();
	void MakeDSSD(G4double, G4double, G4double, G4int);
    void MakeDSSDNew(int detector, char pos);
    
    G4ThreeVector GetPosition(int detector,char pos, int horizontalstrip, int verticalstrip, double X = 0, double Y = 0, double Z = 0);
    G4ThreeVector* GetPositionsNew(int detector,char pos, int horizontalstrip, int verticalstrip);
    

	G4double pi;

	G4int materialOfDetector;

	G4LogicalVolume* expHallLogical;
	G4VPhysicalVolume* expHallPhysical;
	
	G4LogicalVolume* StripSideDetLogical;
	G4LogicalVolume* PixelSideDetLogical;
	G4LogicalVolume *PixelLogical;

	G4LogicalVolume* TargetLogical;

	Material* materials;

};

#endif



