#define grsiMakerSelector_cxx
// The class definition in grsiMakerSelector.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.

// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// Root > T->Process("grsiMakerSelector.C")
// Root > T->Process("grsiMakerSelector.C","some options")
// Root > T->Process("grsiMakerSelector.C+")
//
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
  TCSM csm;
  AnalysisTree->Branch("TCSM",&csm,128000,99);
  
//   cout<<"Making csm_hits Branch"<<endl;
//   vector<TCSMHit> csm_hits;
//   AnalysisTree->Branch("csm_hits",&csm_hits);
}

void grsiMakerSelector::SlaveBegin(TTree * /*tree*/)
{
  // The SlaveBegin() function is called after the Begin() function.
  // When running with PROOF SlaveBegin() is called on each slave server.
  // The tree argument is deprecated (on PROOF 0 is passed).
  
  TString option = GetOption();
  
}

Bool_t grsiMakerSelector::Process(Long64_t entry)
{
  // The Process() function is called for each entry in the tree (or possibly
  // keyed object in the case of PROOF) to be processed. The entry argument
  // specifies which entry in the currently loaded tree is to be processed.
  // It can be passed to either grsiMakerSelector::GetEntry() or TBranch::GetEntry()
  // to read either all or the required parts of the data. When processing
  // keyed objects with PROOF, the object is already loaded and is available
  // via the fObject pointer.
  //
  // This function should contain the "body" of the analysis. It can contain
  // simple or elaborate selection criteria, run algorithms on the data
  // of the event and typically fill histograms.
  //
  // The processing can be stopped by calling Abort().
  //
  // Use fStatus to set the return value of TTree::Process().
  //
  // The return value is currently not used.
  
  bool DEBUGProcess = false;
  bool DEBUGMaker = false;
  
  
  if(entry%10000 == 0)
    cout<<"Processed "<<entry<<" entries"<<endl;
  
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
  
//   if(Hits[1]+Hits[3] >=2)
//   {
//     DEBUGMaker = true;
//     DEBUGProcess = true;
//   }
  
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

  
//   vector<TCSMHit> *csm_hits;
//   AnalysisTree->SetBranchAddress("csm_hits",&csm_hits);

  if(DEBUGProcess) cout<<"Making TCSM in Process"<<endl;
  
  TCSM *csm;
  
  if(DEBUGProcess) cout<<"Setting TCSM branch in Process"<<endl;
  
  AnalysisTree->SetBranchAddress("TCSM",&csm);
  
  vector<TCSMHit> tmpVec;
  
  if(DEBUGProcess) cout<<"Making hits in Process"<<endl;
  
  if(Hits[1] == 1)
  {
    TCSMHit csmhit;
    csmhit.Clear();
    
    csmhit.SetDetectorNumber(1);
    
    //csmhit.SetEHorizontalCharge(const Int_t temp);
    //csmhit.SetEVerticalCharge(const Int_t temp);
    
    //csmhit.SetDHorizontalCharge(const Int_t temp);
    //csmhit.SetDVerticalCharge(const Int_t temp);
    
    if(DEBUGMaker) cout<<" SetDHorizontalStrip: "<<YStrip[1][0]<<endl;
    csmhit.SetDHorizontalStrip(YStrip[1][0]);
    if(DEBUGMaker) cout<<" SetDVerticalStrip: "<<XStrip[1][0]<<endl;
    csmhit.SetDVerticalStrip(XStrip[1][0]);
    
    //csmhit.SetEHorizontalCFD(const Int_t tempd)
    //csmhit.SetEVerticalCFD(const Int_t tempd)
    
    //csmhit.SetDHorizontalCFD(const Int_t tempd)
    //csmhit.SetDVerticalCFD(const Int_t tempd)
    
    //csmhit.SetEHorizontalTime(const Int_t tempd)
    //csmhit.SetEVerticalTime(const Int_t tempd)
    
    //csmhit.SetDHorizontalTime(const Int_t tempd)
    //csmhit.SetDVerticalTime(const Int_t tempd)
    
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
    //static TVector3 GetPosition(int detector, char pos, int horizontalstrip, int verticalstrip, double X=0.00, double Y=0.00, double Z=0.00);	//! 
    
    
    //csmhit.SetTriggerID(const Int_t tempi)
    
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
    
    //csmhit.SetEHorizontalCharge(const Int_t temp);
    //csmhit.SetEVerticalCharge(const Int_t temp);
    
    //csmhit.SetDHorizontalCharge(const Int_t temp);
    //csmhit.SetDVerticalCharge(const Int_t temp);
    
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
    
    //csmhit.SetEHorizontalCFD(const Int_t tempd)
    //csmhit.SetEVerticalCFD(const Int_t tempd)
    
    //csmhit.SetDHorizontalCFD(const Int_t tempd)
    //csmhit.SetDVerticalCFD(const Int_t tempd)
    
    //csmhit.SetEHorizontalTime(const Int_t tempd)
    //csmhit.SetEVerticalTime(const Int_t tempd)
    
    //csmhit.SetDHorizontalTime(const Int_t tempd)
    //csmhit.SetDVerticalTime(const Int_t tempd)
    
    csmhit.SetDPosition(csm->GetPosition(2,'D',YStrip[3][0],XStrip[3][0]));
    
    //csmhit.SetTriggerID(const Int_t tempi)
    
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
  
  csm->SetHits(tmpVec);
  
//   if(DEBUGProcess)
//   {
//     cout<<DYELLOW<<"Showing what CSM has"<<endl;
//     for(int loop = 0;loop<tmpVec.size();loop++)
//     {
//       csm->GetHit(loop)->Print();
//     }
//     cout<<RESET_COLOR<<endl;
//   }
  
  if(DEBUGProcess) cout<<"Filling"<<endl;
  
  AnalysisTree->Fill();
  
  if(DEBUGProcess) cout<<"Deleting TCSM in Process"<<endl;
  
  //delete csm;
  
  if(DEBUGProcess) cout<<"At end of Process"<<endl;
  if(DEBUGProcess) cout<<"***************************************"<<endl<<endl;
  
  return kTRUE;
}

void grsiMakerSelector::SlaveTerminate()
{
  // The SlaveTerminate() function is called after all entries or objects
  // have been processed. When running with PROOF SlaveTerminate() is called
  // on each slave server.
  
}

void grsiMakerSelector::Terminate()
{
  // The Terminate() function is the last function to be called during
  // a query. It always runs on the client, it can be used to present
  // the results graphically or save the results to file.
  
  cout<<endl<<"Terminate"<<endl;

  OutputFile->Write();  
}
