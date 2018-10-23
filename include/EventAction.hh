#ifndef EventAction_H
#define EventAction_H 1

#include "G4Event.hh"
#include "G4UserEventAction.hh"
#include "G4ThreeVector.hh"

#include "SiliconHit.hh"
#include <TTree.h>
#include <TFile.h>
#include "G4ThreeVector.hh"
#include "G4RandomDirection.hh"
#include "DataPass.hh"
#include "GlobalSettings.hh"

#include <vector>

using namespace std;

class EventAction:public G4UserEventAction
{
public:
    EventAction( struct DataPass *Be8InfoPass );
    ~EventAction();
    void BeginOfEventAction(const G4Event* event);
    void EndOfEventAction(const G4Event* evt);

    DataPass *Be8Data;

private:

    TFile	*moutFile;
    TTree	*mActualDataTree;
    TTree	*mReconDataTree;
    TTree	*RyanTree;

    G4int	mEventID;
    G4int	siliconCollID;

    DataPass	*mKinematicInfo;

    G4double	mThreshHold;

    G4double Energy[6][2];
    G4int XStrip[6][2];
    G4int YStrip[6][2];
    G4int Hits[6];
    
    G4double	mBe8Energy;
    G4double	mBe8Theta;
    G4double	mBe8ThetaCOM;
    G4double	mBe8Phi;
    G4double	mBe8X;
    G4double	mBe8Y;
    G4double	mBe8Z;

    G4double	mBlur[10];

    G4double	mBe12Energy;
    G4double	mBe12Theta;
    G4double	mBe12ThetaCOM;
    G4double    mBe12PhiCOM;
    G4double	mBe12Phi;
    G4double	mBe12X[2];
    G4double	mBe12Y[2];
    G4double	mBe12Z[2];

    G4double	mA1Energy;
    G4double	mA1Theta;
    G4double	mA1ThetaCOM;
    G4double	mA1Phi;
    G4double	mA1X[6];
    G4double	mA1Y[6];
    G4double	mA1Z[6];

    G4double	mA2Energy;
    G4double	mA2Theta;
    G4double	mA2ThetaCOM;
    G4double	mA2Phi;
    G4double	mA2X;
    G4double	mA2Y;
    G4double	mA2Z;

    G4double	mDetectorX[10];
    G4double	mDetectorY[10];
    G4double	mDetectorZ[10];
    G4int	mDetectorID[10];
    G4double	mEnergyDep[10];
    G4int	StripX[10];
    G4int	StripY[10];
    G4double	mTheta[10];
    G4double	mPhi[10];

    G4double	BeThetaMath;
    G4double	BeThetaCOMMath;
    G4double	BeEnergyMath;
    G4double	BeExciteE;
    G4double	Alpha1EnergyMath;
    G4double	Alpha1ThetaMath;
    G4double	Alpha2EnergyMath;
    G4double	Alpha2ThetaMath;
    G4double	Be8ThetaMath;
    G4double	Be8ThetaCOMMath;
    G4double	Be8EnergyMath;
    G4double	Be8ExciteE;
    
    G4double	BePhiMath;
    G4double	Alpha1PhiMath;
    G4double	Alpha2PhiMath;
    G4double	Be8PhiMath;
    
    G4double	mDeltaTheta;
    G4double	mBe12ExciteE;

    G4double	mRealPhi[50];
    G4double	mRealTheta[50];

    G4int	mHits;
    G4bool	mBe12Flag[50];
    G4bool	mAlphaFlag[50];
    G4bool	mAlphaFlag2;

    void 	mSetActualBranches();
    void	mSetReconBranches();
    void	SetRyanBranches();
    void	mFillAndResetTree();

    G4int	mFindDetectorID(G4String);
    G4double	blur();    

    void 	mSetActualDataParameters();
    void 	mSetReconDataParameters( std::vector<SiliconHit*> *Hits );

    void	mResetParameters();
};

#endif
