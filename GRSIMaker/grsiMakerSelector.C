#define grsiMakerSelector_cxx
#include "grsiMakerSelector.h"


void grsiMakerSelector::Begin(TTree * /*tree*/)
{
  // The Begin() function is called at the start of the query.
  // When running with PROOF Begin() is only called on the client.
  // The tree argument is deprecated (on PROOF 0 is passed).
  
  cout<<"Begin"<<endl;
  TString option = GetOption();
    
  cout<<"Opening OutputFile"<<endl;
  OutputFile = new TFile("SimCSMHits.root","recreate");
    
  cout<<"Making AnalysisTree"<<endl;
  AnalysisTree = new TTree("AnalysisTree","AnalysisTree");
  
  cout<<"Making csm Branch"<<endl;
  TCSM *csm = new TCSM();
  AnalysisTree->Branch("TCSM",&csm);//,128000,99);
}

void grsiMakerSelector::SlaveBegin(TTree * /*tree*/)
{
  TString option = GetOption();
}

Bool_t grsiMakerSelector::Process(Long64_t entry)
{
  bool DEBUGProcess = false;
  bool DEBUGMaker = false;
  
  
  if(entry%10000 == 0)
    cout<<"Processed "<<entry<<" entries\r"<<endl;
  
  if(DEBUGProcess)
    cout<<"Getting entry"<<endl;
  
  fChain->GetTree()->GetEntry(entry);
  
  if(Hits[0] + Hits[1] + Hits[2] + Hits[3] + Hits[4] + Hits[5] == 0) //If nothing hit the detectors, skip.
  {
    if(DEBUGProcess)
      cout<<"Skipping 0 Hit Entry."<<endl;
    return kTRUE;
  }
  
  for(int detectoriter = 0;detectoriter<6;detectoriter++)
  {
    if(Hits[detectoriter] > 1)
    {      
      if(abs(XStrip[detectoriter][0] - XStrip[detectoriter][1]) + abs(YStrip[detectoriter][0] - YStrip[detectoriter][1]) == 1)
      {
        Energy[detectoriter][0] += Energy[detectoriter][1];
        Energy[detectoriter][1] = 0.;
        XStrip[detectoriter][1] = -10;
        YStrip[detectoriter][1] = -10;
        Hits[detectoriter]--;
      }
    }
  }
  
  if(DEBUGProcess) cout<<"Hits: ";
  
  for(int detectoriter2 = 0;detectoriter2<6;detectoriter2++)
  {
    if(DEBUGProcess) cout<<Hits[detectoriter2]<<" ";
    
    if(Hits[detectoriter2] > 1)
    {
      return kFALSE;
      cout<<RED<<"More than one hit in a detector after summation!"<<RESET_COLOR<<endl;
      
      for(int dumb=0;dumb<6;dumb++)
      {
        cout<<Hits[dumb]<<" ";
      }
      cout<<endl;
      
      cout<<" Energy 0: "<<Energy[detectoriter2][0]<<" Energy 1: "<<Energy[detectoriter2][1]<<endl;
      cout<<" XStrip 0: "<<XStrip[detectoriter2][0]<<" XStrip 1: "<<XStrip[detectoriter2][1]<<endl;
      cout<<" YStrip 0: "<<YStrip[detectoriter2][0]<<" YStrip 1: "<<YStrip[detectoriter2][1]<<endl;
    }
  }
  
  if(DEBUGProcess) cout<<endl;
  if(DEBUGProcess) cout<<"Making TCSM in Process"<<endl;
  
  TCSM *csm = new TCSM();
  
  if(DEBUGProcess) cout<<"Setting TCSM branch in Process"<<endl;
  
  AnalysisTree->SetBranchAddress("TCSM",&csm);
  
  vector<TCSMHit> tmpVec;
  
  if(DEBUGProcess) cout<<"Making hits in Process"<<endl;
  
  if(Hits[1] == 1)
  {
    TCSMHit csmhit;
    csmhit.Clear();
    
    csmhit.SetDetectorNumber(1);

    if(DEBUGMaker) cout<<" SetDHorizontalStrip: "<<YStrip[1][0]<<endl;
    csmhit.SetDHorizontalStrip(YStrip[1][0]);
    if(DEBUGMaker) cout<<" SetDVerticalStrip: "<<XStrip[1][0]<<endl;
    csmhit.SetDVerticalStrip(XStrip[1][0]);

    if(DEBUGMaker)
    {
      TVector3 tmppos1 = csm->GetPosition(1,'D',YStrip[1][0],XStrip[1][0]);
      TVector3 tmppos2 = csm->GetPosition(2,'D',YStrip[1][0],XStrip[1][0]);
      
      cout<<DGREEN<<"Debugging Positions."<<endl<<"Theta, Phi"<<endl;
      cout<<"From GetPosition1: "<<tmppos1.Theta()*180./TMath::Pi()<<", "<<tmppos1.Phi()*180./TMath::Pi()<<endl;
      cout<<"From GetPosition2: "<<tmppos2.Theta()*180./TMath::Pi()<<", "<<tmppos2.Phi()*180./TMath::Pi()<<endl;
      
      cout<<"From BeMath: "<<BeThetaMath<<", "<<BePhiMath<<endl;
      cout<<"From Be8Math: "<<Be8ThetaMath<<", "<<Be8PhiMath<<RESET_COLOR<<endl;
    }
    
    csmhit.SetDPosition(csm->GetPosition(1,'D',YStrip[1][0],XStrip[1][0]));

    if(DEBUGMaker) cout<<" SetDHorizontal/VerticalEnergy: "<<Energy[1][0]<<endl;
    csmhit.SetDHorizontalEnergy(Energy[1][0]*1000.);
    csmhit.SetDVerticalEnergy(Energy[1][0]*1000.);
    
    if(Hits[2] == 1)
    {
      
      if(DEBUGMaker) cout<<" SetEHorizontalStrip: "<<YStrip[2][0]<<endl;
      csmhit.SetEHorizontalStrip(YStrip[2][0]);
      if(DEBUGMaker) cout<<" SetEVerticalStrip: "<<XStrip[2][0]<<endl;
      csmhit.SetEVerticalStrip(XStrip[2][0]);
      
      if(DEBUGMaker) cout<<" SetEHorizontal/VerticalEnergy: "<<Energy[2][0]<<endl;
      csmhit.SetEHorizontalEnergy(Energy[2][0]*1000.);
      csmhit.SetEVerticalEnergy(Energy[2][0]*1000.);
      
      csmhit.SetEPosition(csm->GetPosition(1,'E',YStrip[2][0],XStrip[2][0]));
    }
    
    if(DEBUGMaker) csmhit.Print();
    
    if(DEBUGMaker) cout<<endl;
    
    tmpVec.push_back(csmhit);
  }
  
  if(Hits[3] == 1)
  {
    TCSMHit csmhit;
    csmhit.Clear();
    
    csmhit.SetDetectorNumber(2);

    if(DEBUGMaker) cout<<" SetDHorizontalStrip: "<<YStrip[3][0]<<endl;
    csmhit.SetDHorizontalStrip(YStrip[3][0]);
    if(DEBUGMaker) cout<<" SetDVerticalStrip: "<<XStrip[3][0]<<endl;
    csmhit.SetDVerticalStrip(XStrip[3][0]);
    
    if(DEBUGMaker)
    {
      TVector3 tmppos1 = csm->GetPosition(1,'D',YStrip[3][0],XStrip[3][0]);
      TVector3 tmppos2 = csm->GetPosition(2,'D',YStrip[3][0],XStrip[3][0]);
      
      cout<<DRED<<"Debugging Positions."<<endl<<"Theta, Phi"<<endl;
      cout<<"From GetPosition1: "<<tmppos1.Theta()*180./TMath::Pi()<<", "<<tmppos1.Phi()*180./TMath::Pi()<<endl;
      cout<<"From GetPosition2: "<<tmppos2.Theta()*180./TMath::Pi()<<", "<<tmppos2.Phi()*180./TMath::Pi()<<endl;
      
      cout<<"From BeMath: "<<BeThetaMath<<", "<<BePhiMath<<endl;
      cout<<"From Be8Math: "<<Be8ThetaMath<<", "<<Be8PhiMath<<RESET_COLOR<<endl;
    }

    csmhit.SetDPosition(csm->GetPosition(2,'D',YStrip[3][0],XStrip[3][0]));
    
    if(DEBUGMaker) cout<<" SetDHorizontal/VerticalEnergy: "<<Energy[3][0]<<endl;
    csmhit.SetDHorizontalEnergy(Energy[3][0]*1000.);
    csmhit.SetDVerticalEnergy(Energy[3][0]*1000.);
    
    if(Hits[4] == 1)
    {
      
      if(DEBUGMaker) cout<<" SetEHorizontalStrip: "<<YStrip[4][0]<<endl;
      csmhit.SetEHorizontalStrip(YStrip[4][0]);
      if(DEBUGMaker) cout<<" SetEVerticalStrip: "<<XStrip[4][0]<<endl;
      csmhit.SetEVerticalStrip(XStrip[4][0]);
      
      if(DEBUGMaker) cout<<" SetEHorizontal/VerticalEnergy: "<<Energy[4][0]<<endl;
      csmhit.SetEHorizontalEnergy(Energy[4][0]*1000.);
      csmhit.SetEVerticalEnergy(Energy[4][0]*1000.);
      csmhit.SetEPosition(csm->GetPosition(2,'E',YStrip[4][0],XStrip[4][0]));
    }
    
    if(DEBUGMaker) csmhit.Print();
    
    if(DEBUGMaker) cout<<endl;
    
    tmpVec.push_back(csmhit);
  }
  
  //https://root.cern.ch/root/htmldoc/guides/users-guide/Trees.html#why-should-you-use-a-tree
  //https://root.cern.ch/trees-five-steps
  
  if(DEBUGProcess) cout<<"tmpVec's size is "<<tmpVec.size()<<endl;
  
  csm->Clear();  
  
  if(DEBUGProcess) cout<<"cleared csm"<<endl;
  
  csm->SetHits(tmpVec);
  
  if(DEBUGProcess)
    csm->Print();
  
  if(DEBUGProcess) cout<<"Filling"<<endl;
  
  AnalysisTree->Fill();
  
  if(DEBUGProcess) cout<<"Deleting TCSM in Process"<<endl;
  
  delete csm;
  
  if(DEBUGProcess) cout<<"At end of Process"<<endl;
  if(DEBUGProcess) cout<<"***************************************"<<endl<<endl;
  
  return kTRUE;
}

void grsiMakerSelector::SlaveTerminate()
{
}

void grsiMakerSelector::Terminate()
{
  cout<<endl<<"Terminate"<<endl;

  OutputFile->Write();  
}
