#include "Material.hh"


Material::Material()
 {}


Material::~Material()
 {}


void Material::DefineMaterials()
 {

   defineElements();
   makeVacuum();
   makeAir();
   makeAluminum();
   makeBC501A();
   makeBC537();
   makeDeuterium();
   makeHydrogen();
   makeSilicon();
   makeTarget();   
   makeFiberGlass();

 }


void Material::defineElements()
 {

   G4double a, z;
   G4String name, symbol;

   elAl = new G4Element(name = "Aluminum", symbol = "Al", z = 13.0, a = 26.98 * CLHEP::g/CLHEP::mole);
   elB = new G4Element(name = "Boron", symbol = "B", z = 5.0, a = 10.811 * CLHEP::g/CLHEP::mole);
   elBe = new G4Element(name = "Beryllium", symbol = "Be", z = 4.0, a = 9.012182 * CLHEP::g/CLHEP::mole);            
   elC = new G4Element(name = "Carbon", symbol = "C", z = 6.0, a = 12.01 * CLHEP::g/CLHEP::mole);
   elCa = new G4Element(name = "Calcium", symbol = "C", z = 20.0, a = 40.078 * CLHEP::g/CLHEP::mole);   
   elK = new G4Element(name = "Potassium", symbol = "K", z = 19.0, a = 39.0983 * CLHEP::g/CLHEP::mole);      
   elD = new G4Element(name = "Deuterium", symbol = "D", z = 1.0, a = 2.01 * CLHEP::g/CLHEP::mole);
   elH = new G4Element(name = "Hydrogen", symbol = "H", z = 1.0, a = 1.01 * CLHEP::g/CLHEP::mole);
   elN = new G4Element(name = "Nitrogen", symbol = "N", z = 7.0, a = 14.01 * CLHEP::g/CLHEP::mole);
   elNa = new G4Element(name = "Sodium", symbol = "Na", z = 11.0, a = 22.990 * CLHEP::g/CLHEP::mole);   
   elO = new G4Element(name = "Oxygen", symbol = "O", z = 8.0, a = 16.00 * CLHEP::g/CLHEP::mole);
   elSi = new G4Element(name = "Silicon", symbol = "Si", z = 14.0, a = 28.0855 * CLHEP::g/CLHEP::mole);

 }

void Material::makeVacuum()
{
	G4double density = CLHEP::universe_mean_density;
	G4double pressure = 3.0e-18*CLHEP::hep_pascal;
	G4double temperature = 2.73*CLHEP::kelvin;
	G4String name;
	G4int ncomponents = 1;
	
	Vacuum = new G4Material( name = "Vacuum", density, ncomponents, kStateGas, temperature, pressure);
	Vacuum -> AddElement( elH, 100*CLHEP::perCent );
}

void Material::makeAir()
 {

   G4int ncomponents;
   G4double density, fractionmass;
   G4String name;

   Air = new G4Material(name = "Air", density = 1.29 * CLHEP::mg/CLHEP::cm3, ncomponents = 2);
   Air -> AddElement(elN, fractionmass = 76.5481 * CLHEP::perCent);
   Air -> AddElement(elO, fractionmass = 23.4519 * CLHEP::perCent);

 }


void Material::makeAluminum()
 {

   G4int ncomponents;
   G4double density, fractionmass;
   G4String name;

   Aluminum = new G4Material(name = "Aluminum", density = 2700 * CLHEP::kg/CLHEP::m3, ncomponents = 1);
   Aluminum -> AddElement(elAl, fractionmass = 100 * CLHEP::perCent);

 }
 
void Material::makeFiberGlass()
 {

   G4int ncomponents;
   G4double density, fractionmass;
   G4String name;

   Board = new G4Material(name = "Board", density = 2.16 * CLHEP::g/CLHEP::cm3, ncomponents = 7);
   Board -> AddElement(elAl, fractionmass = 0.22 * CLHEP::perCent);
   Board -> AddElement(elB, fractionmass = 8.8 * CLHEP::perCent);
   Board -> AddElement(elCa, fractionmass = 0.35 * CLHEP::perCent);
   Board -> AddElement(elO, fractionmass = 63.9967 * CLHEP::perCent);
   Board -> AddElement(elK, fractionmass = 1.3 * CLHEP::perCent);
   Board -> AddElement(elSi, fractionmass = 24.667 * CLHEP::perCent);
   Board -> AddElement(elNa, fractionmass = 0.6667 * CLHEP::perCent);                  

 } 


void Material::makeBC501A()
 {

   G4int ncomponents;
   G4double density, fractionmass;
   G4String name;

   BC501A = new G4Material(name = "BC501A", density = 0.874 * CLHEP::g/CLHEP::cm3, ncomponents = 2);
   BC501A -> AddElement(elH, fractionmass = 9.2497 * CLHEP::perCent);
   BC501A -> AddElement(elC, fractionmass = 90.7503 * CLHEP::perCent);

 }


void Material::makeBC537()
 {

   G4int ncomponents;
   G4double density, fractionmass;
   G4String name;

   BC537 = new G4Material(name = "BC537", density = 0.954 * CLHEP::g/CLHEP::cm3, ncomponents = 3);
   BC537 -> AddElement(elH, fractionmass = 0.0625 * CLHEP::perCent);
   BC537 -> AddElement(elC, fractionmass = 85.7326 * CLHEP::perCent);
   BC537 -> AddElement(elD, fractionmass = 14.2049 * CLHEP::perCent);

 }


void Material::makeDeuterium()
 {

   G4int ncomponents;
   G4double density, fractionmass;
   G4String name;

   Deuterium = new G4Material(name = "Deuterium", density = 1.0 * CLHEP::g/CLHEP::cm3, ncomponents = 1);
   Deuterium -> AddElement(elD, fractionmass = 100 * CLHEP::perCent);

 }


void Material::makeHydrogen()
 {

   G4int ncomponents;
   G4double density, fractionmass;
   G4String name;

   Hydrogen = new G4Material(name = "Hydrogen", density = 1.0 * CLHEP::g/CLHEP::cm3, ncomponents = 1);
   Hydrogen -> AddElement(elH, fractionmass = 100 * CLHEP::perCent);

 }
 
void Material::makeSilicon()
{
	G4int ncomponents;
	G4double density, fractionmass;
	G4String name;
	
	Silicon = new G4Material( name = "Silica", density = 2.3290 * CLHEP::g/CLHEP::cm3, ncomponents = 1);
	Silicon->AddElement(elSi, fractionmass = 100 * CLHEP::perCent );
}

void Material::makeTarget()
{
	G4int ncomponents;
	G4double density, fractionmass;
	G4String name;
	
	Target = new G4Material( name = "BeTarget", density = 1.848 * CLHEP::g/CLHEP::cm3, ncomponents = 1);
	Target->AddElement(elBe, fractionmass = 100 * CLHEP::perCent );		
}
