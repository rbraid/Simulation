#include "EventAction.hh"

#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4SDManager.hh"
#include "G4UImanager.hh"
#include "G4ios.hh"
#include "Randomize.hh"
#include <cmath>
// #include "/home/ryan/nuclear/be9/summed/angleheader.h"
#include <algorithm>
#include "/home/ryan/nuclear/mine/Colors.h"


EventAction::EventAction( struct DataPass *Be8InfoPass )
{
    Be8Data = (Be8InfoPass);
    moutFile = new TFile("EventOut.root", "RECREATE");
    mReconDataTree = new TTree("recondata","Event action tree of reconstructed values");
    RyanTree = new TTree("ryandata","More resonable data format");
    
    siliconCollID = -1;

    mResetParameters();
    mSetReconBranches();
    SetRyanBranches();
    mResetParameters();

    mThreshHold = 0.100;
}

EventAction::~EventAction()
{
    moutFile->Write();
    moutFile->Close();
}

void EventAction::BeginOfEventAction(const G4Event* event)
{
    mEventID = event -> GetEventID();

    G4SDManager *SDman = G4SDManager::GetSDMpointer();
    if( siliconCollID < 0 )
    {
        G4String colName;
        siliconCollID = SDman->GetCollectionID( colName="siliconCollection" );
    }
}

void EventAction::EndOfEventAction(const G4Event* evt)
{
    G4double blurval = blur();

    if( siliconCollID < 0 )
    {
        return;
    }
    G4HCofThisEvent *HCE = evt->GetHCofThisEvent();
    SiliconHitsCollection *SHC = 0;

    if(HCE)
    {
        SHC = (SiliconHitsCollection*)(HCE->GetHC(siliconCollID));
    }
    std::vector<SiliconHit*> *SiHits = SHC->GetVector();
    G4int size = SiHits->size();

    bool abort = 0;

    //cout<<size<<endl;

    if(size>0&&size<8)
    {
      mHits=size;
      
      int xI[6];
      int xII[6];
      int yI[6];
      int yII[6];
      double energyI[6];
      double energyII[6];
      bool bI[6] = {0};
      bool bII[6] = {0};
      bool problem[6] = {0};
      
      for(int index=0;index<6;index++)//Intitialize my arrays
      {
        xI[index] = -10;
        xII[index] = -10;
        yI[index] = -10;
        yII[index] = -10;
        energyI[index] = 0.;
        energyII[index] = 0.;
      }
      
      for(int i =0; i < size; i++)
      {
        G4String volumeName = SiHits->at(i)->GetVolumeName();
        mDetectorID[i] = mFindDetectorID(volumeName);
        
        if(mDetectorID[i]>5||mDetectorID[i]<0)
          cout<<"************************************"<<mDetectorID[i]<<" "<<volumeName<<endl;
        //else
        //cout<<mDetectorID[i]<<" "<<volumeName<<endl;
        
        //cout<<SiHits->at(i)->GetParticleName()<<" "<<volumeName<<endl;
        
        StripX[i] = SiHits->at(i)->GetStripX();
        StripY[i] = SiHits->at(i)->GetStripY();	    
        
        G4ThreeVector Position = SiHits->at(i)->GetPos();
        mDetectorX[i] = Position.getX();
        mDetectorY[i] = Position.getY();
        mDetectorZ[i] = Position.getZ();
        
//         mTheta[i] = thetaarray[mDetectorID[i]][StripY[i]][StripX[i]];
//         mPhi[i] = phiarray[mDetectorID[i]][StripY[i]][StripX[i]];
        
        bool DEBUG = 0;
        
        if(DEBUG) std::cout<<DGREEN<<"EventAction Theta: "<<Position.theta()*180./3.14159<<", Phi: "<<Position.phi()*180./3.14159<<RESET_COLOR<<std::endl;
//         if(DEBUG) std::cout<<DYELLOW<<"Header Theta: "<<mTheta[i]<<", Phi: "<<mPhi[i]<<RESET_COLOR<<std::endl;
        
        mEnergyDep[i] = (SiHits->at(i)->GetEdep());
        if(BLURRING)
          mEnergyDep[i]+=blurval/1000.;
        
        mBlur[i] = blurval;
        
        //Check for XY strip summation
        if(mEnergyDep[i]<.0001);
        
        else if(xI[mDetectorID[i]]==-10 && yI[mDetectorID[i]]==-10)
        {
          xI[mDetectorID[i]] = StripX[i];
          yI[mDetectorID[i]] = StripY[i];
        }
        
        else if(StripX[i]==xI[mDetectorID[i]] && StripY[i]==yI[mDetectorID[i]]);
        
        else if(xII[mDetectorID[i]]==-10 && yII[mDetectorID[i]] == -10)
        {
          xII[mDetectorID[i]] = StripX[i];
          yII[mDetectorID[i]] = StripY[i];
        }
        
        //Sum the energies in the same pixel
        if(mEnergyDep[i]<.0001);
        
        else if(StripX[i]==xI[mDetectorID[i]] && StripY[i]==yI[mDetectorID[i]])
        {
          energyI[mDetectorID[i]]+=mEnergyDep[i];
          bI[mDetectorID[i]]=1;
        }
        else if(StripX[i]==xII[mDetectorID[i]] && StripY[i]==yII[mDetectorID[i]])
        {
          energyII[mDetectorID[i]]+=mEnergyDep[i];
          bII[mDetectorID[i]]=1;
        }
        else if(StripX[i]!=-10 || StripY[i]!=-10)
        {
          problem[mDetectorID[i]] = 1;
        }
        
      }
      
      for(int detector=0;detector<6;detector++)
      {
        Energy[detector][0]=energyI[detector];
        Energy[detector][1]=energyII[detector];
        XStrip[detector][0]=xI[detector];
        XStrip[detector][1]=xII[detector];
        YStrip[detector][0]=yI[detector];
        YStrip[detector][1]=yII[detector];
        Hits[detector]=bI[detector]+bII[detector];
        
        /*if(YStrip[detector][1]!=-10)
         *	  {
         *	    double temp =0.;
         *	    temp = YStrip[detector][0];
         *	    YStrip[detector][0] = YStrip[detector][1];
         *	    YStrip[detector][1] = temp;
      }*/
        
        if(problem[detector])
          abort=1;
      }
      
      
    }
    
    BeEnergyMath = Be8Data->Be12Energy;
    BeExciteE = Be8Data->Be12ExciteE;
    BeThetaMath = (Be8Data->Be12Theta)*180./pi;
    Alpha1EnergyMath = Be8Data->A1Energy;
    Alpha1ThetaMath = Be8Data->A1Theta*180./pi;
    Alpha2EnergyMath = Be8Data->A2Energy;
    Alpha2ThetaMath = Be8Data->A2Theta*180./pi;
    Be8EnergyMath = Be8Data->Be8Energy;
    Be8ExciteE = Be8Data->Be8ExciteE;
    Be8ThetaMath = Be8Data->Be8Theta*180./pi;
    BeThetaCOMMath = (Be8Data->Be12ThetaCOM)*180./pi;
    Be8ThetaCOMMath = Be8Data->Be8ThetaCOM*180./pi;
    BePhiMath = Be8Data->Be12Phi*180./pi;
    Be8PhiMath = Be8Data->Be8Phi*180./pi;
    Alpha1PhiMath = Be8Data->A1Phi*180./pi;
    Alpha2PhiMath = Be8Data->A2Phi*180./pi;
    
//     if(abort)
//     {
//       mResetParameters();
//       //cout<<".";      
//     }
    mReconDataTree->Fill();
    RyanTree->Fill();
    
    mResetParameters();
    
}

G4double EventAction::blur()
{
  G4double blurval = G4RandGauss::shoot(0.0,50.);
  return(blurval);
}

G4int EventAction::mFindDetectorID(G4String Name)
{
    G4int id = -10;

    if( !(strncmp("dER", Name,3)) )
    {
        id = 1;
    }
    else if( !(strncmp("dEL", Name,3)) )
    {
      id = 3;
    }
    else if( !(strncmp("ER", Name,2)) )
    {
      id = 2;
    }
    else if( !(strncmp("EL", Name,2)) )
    {
      id = 4;
    }
    else if(!(strncmp("SideR", Name,5)) )
    {
      id = 0;
    }
    else if(!(strncmp("SideL", Name,5)) )
    {
      id = 5;
    }
    else
      cout<<"***************************************"<<endl<<endl<<"ERROR in DetectorID"<<endl<<endl<<"***************************************"<<endl;

    return(id);
}

void EventAction::mResetParameters()
{
  for(int detector=0;detector<6;detector++)
  {
    Energy[detector][0]=0.;
    Energy[detector][1]=0.;
    XStrip[detector][0]=-10;
    XStrip[detector][1]=-10;
    YStrip[detector][0]=-10;
    YStrip[detector][1]=-10;
    Hits[detector]=0;
  }
}

void EventAction::SetRyanBranches()
{
  RyanTree->Branch("Energy",&(Energy),"Energy[6][2]/D");

  RyanTree->Branch("XStrip",&(XStrip),"XStrip[6][2]/I");

  RyanTree->Branch("YStrip",&(YStrip),"YStrip[6][2]/I");

  RyanTree->Branch("Hits",&(Hits),"Hits[6]/I");

  RyanTree->Branch("BeEnergyMath",&(BeEnergyMath));
  RyanTree->Branch("BeThetaMath",&(BeThetaMath));
  RyanTree->Branch("Alpha1EnergyMath",&(Alpha1EnergyMath));
  RyanTree->Branch("Alpha1ThetaMath",&(Alpha1ThetaMath));
  RyanTree->Branch("Alpha2EnergyMath",&(Alpha2EnergyMath));
  RyanTree->Branch("Alpha2ThetaMath",&(Alpha2ThetaMath));
  RyanTree->Branch("Be8EnergyMath",&(Be8EnergyMath));
  RyanTree->Branch("Be8ThetaMath",&(Be8ThetaMath));
  RyanTree->Branch("BeThetaCOMMath",&(BeThetaCOMMath));
  RyanTree->Branch("Be8ThetaCOMMath",&(Be8ThetaCOMMath));
  RyanTree->Branch("BePhiMath",&(BePhiMath));
  RyanTree->Branch("Be8PhiMath",&(Be8PhiMath));
  RyanTree->Branch("Alpha1PhiMath",&(Alpha1PhiMath));
  RyanTree->Branch("Alpha2PhiMath",&(Alpha2PhiMath));
  
  RyanTree->Branch("BeExciteE",&(BeExciteE));
  RyanTree->Branch("Be8ExciteE",&(Be8ExciteE));
  
  
}

void EventAction::mSetReconBranches()
{
    mReconDataTree->Branch("DetectorX",&(mDetectorX),"mDetectorX[10]/D");
    mReconDataTree->Branch("DetectorY",&(mDetectorY),"mDetectorY[10]/D");
    mReconDataTree->Branch("DetectorZ",&(mDetectorZ),"mDetectorZ[10]/D");
    mReconDataTree->Branch("DetectorID",&(mDetectorID),"mDetectorID[10]/I");
    mReconDataTree->Branch("Theta",&(mTheta),"mTheta[10]/D");
    mReconDataTree->Branch("Phi",&(mPhi),"mPhi[10]/D");

    mReconDataTree->Branch("StripX",&(StripX),"StripX[10]/I");
    mReconDataTree->Branch("StripY",&(StripY),"StripY[10]/I");

    mReconDataTree->Branch("EnergyDep",&(mEnergyDep),"mEnergyDep[10]/D");
    mReconDataTree->Branch("Blur",&(mBlur),"mBlur[10]/D");

    mReconDataTree->Branch("Hits",&(mHits));
}
