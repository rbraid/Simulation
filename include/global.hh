#ifndef global_CSM_H
#define global_CSM_H 1

#include "globals.hh"

#define MAXNUMDET 1000

class tree
 {
   public:
   G4double InitKE;
//

   G4double DeltaTime;  
   G4double eNeut;
   G4int eventID;
   G4int numDetHit;
   G4int hitDetFlag[MAXNUMDET];
   G4int hitDetCopyNum[MAXNUMDET];
   G4int hitDetNumHits[MAXNUMDET];
   G4double TOF[MAXNUMDET];
   G4double x[MAXNUMDET];
   G4double y[MAXNUMDET];
   G4double z[MAXNUMDET];
   G4double theta[MAXNUMDET];
   G4double phi[MAXNUMDET];
   G4double eDep[MAXNUMDET];
   G4double eDepCut[MAXNUMDET];
   G4double eNeutRecMin[MAXNUMDET];
   G4double eNeutRecMax[MAXNUMDET];

 };

#endif



