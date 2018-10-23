#define AngSelector_cxx

#include "AngSelector.h"
#include <TH2.h>
#include <TStyle.h>
#include <vector>
#include <iostream>
#include <TFile.h>
#include <algorithm>

#define DEBUG 0

const double MEVpNUC = 931.494061;

const double MASS_BE8 = 8*MEVpNUC+4.9416;
const double MASS_BE12 = 12*MEVpNUC+25.0766;
const double MASS_BE9 = 9*MEVpNUC+11.3484;
const double MASS_BE11 = 11*MEVpNUC+20.1771;
const double MASS_HE4 = 4*MEVpNUC+2.4249;
const double MASS_C12 = 12*MEVpNUC;
const double MASS_C13 = 13*MEVpNUC+3.1250;
const double MASS_BE10 = 10*MEVpNUC+12.6074;

const double BEAM_ENERGY = 30.14;

using namespace std;

struct HitStruct
{
  double Theta = -1.;
  double Phi = -1.;
  double Energy = -1.;
  int StripX = -1;
  int StripY = -1;
  double ThetaCOM = -1.;
  double Excite = -1.;
  double TotalExcite = -1.;
  bool PunchThrough = 0;
  int Detector = 0;
};

void SetHistos()
{
  cout<<"SetHistos start"<<endl;
  for(int det = 1; det<=2; det++)
  {
    for(int state =0; state<=12; state += 3)
    {
      for(int dettypeI = 0; dettypeI<3;dettypeI++)
      {
        TString dettype = "def";
        if(dettypeI == 0)
          dettype = "pid";
        else if(dettypeI == 1)
          dettype = "dual";
        else if(dettypeI ==2)
          dettype = "other";
        else
          cerr<<"Overshot on Dettype"<<endl;
        
        outlist->Add(new TH1D(Form("RingCounts_s%02i_d%i_%s",state,det,dettype.Data()),Form("RingCounts_s%02i_d%i_%s",state,det,dettype.Data()),200,0,200));
        outlist->Add(new TH2D(Form("HitPattern_s%02i_d%i_%s",state,det,dettype.Data()),Form("HitPattern for detector %i, state %i, %s",det,state,dettype.Data()),16,0,16,16,0,16));
        outlist->Add(new TH2D(Form("RingVThetaCOM_s%02i_d%i_%s",state,det,dettype.Data()),Form("RingVThetaCOM state %i, detector %i %s",state,det,dettype.Data()),180,0,180,200,0,200));
        outlist->Add(new TH2D(Form("RingVThetaLab_s%02i_d%i_%s",state,det,dettype.Data()),Form("RingVThetaLab state %i, detector %i %s",state,det,dettype.Data()),180,0,180,200,0,200));
        outlist->Add(new TH2D(Form("EvTheta_s%02i_d%i_%s",state,det,dettype.Data()),Form("EvTheta_s%02i_d%i_pid_%s",state,det,dettype.Data()),90,0,90,45,0,45));
        outlist->Add(new TH1D(Form("COMTheta_s%02i_d%i_%s",state,det,dettype.Data()),Form("COMTheta_s%02i_d%i_pid_%s",state,det,dettype.Data()),720,-360,360));
        outlist->Add(new TH1D(Form("AngHisto_s%02i_d%i_%s",state,det,dettype.Data()),Form("AngHisto_s%02i_d%i_pid_%s",state,det,dettype.Data()),720,-360,360));
        
        outlist->Add(new TH1D(Form("COMTheta_convert_s%02i_d%i_%s",state,det,dettype.Data()),Form("COMTheta_convert_s%02i_d%i_pid_%s",state,det,dettype.Data()),720,-360,360));
      }
    }
  }
  outlist->Add(new TH1D("COMThetaAll","COMThetaAll",720,-360,360));
  outlist->Add(new TH1D("COMThetaAll_convert","COMThetaAll_convert",720,-360,360));
  
  outlist->Add(new TH1D("COMThetaHit","COMThetaHit",720,-360,360));
  outlist->Add(new TH1D("COMThetaHit_loop","COMThetaHit_loop",720,-360,360));
  outlist->Add(new TH1D("COMThetaHit_vec","COMThetaHit_vec",720,-360,360));
  
  outlist->Add(new TH1D("COMThetaHit_frac","COMThetaHit_frac",720,-360,360));
  outlist->Add(new TH1D("COMThetaHit_solid","COMThetaHit_solid",720,-360,360));
  outlist->Add(new TH1D("COMThetaHit_frac_solid","COMThetaHit_frac_solid",720,-360,360));
  
  cout<<"SetHistos End"<<endl;
}

void AngSelector::Begin(TTree * /*tree*/)
{
   TString option = GetOption();
   
    ringFile = TFile::Open("/home/ryan/nuclear/mine/rb/angulardistribution/DumbRings.root","read");
    if(!ringFile)
      cerr<<"No Ring file open"<<endl;
    SAfile = TFile::Open("/home/ryan/nuclear/mine/rb/SolidAngle/solidAngleDiag.root","read");
    if(!SAfile)
      cerr<<"No Solid Angle file open"<<endl;
    
    cout<<"outlist creation"<<endl;
   outlist = new TList();
   
   outfile = TFile::Open("out.root","recreate");
   
   SetHistos();
   
   cout<<"End of Begin()"<<endl;
}

void AngSelector::SlaveBegin(TTree * /*tree*/)
{
   TString option = GetOption();
}

Bool_t AngSelector::Process(Long64_t entry)
{
  if(entry%10000 == 0)
    cout<<"Processed "<<entry<<" entries"<<endl;
  fChain->GetTree()->GetEntry(entry);
  
  int TotalHits = 0;
  for(int i =1;i<5;i++)
  {
    TotalHits += Hits[i];
  }
  TH1D* allptr = (TH1D*)outlist->FindObject(Form("COMThetaAll"));
  allptr->Fill(BeThetaCOMMath);
  allptr->Fill(Be8ThetaCOMMath);
  
  allptr = (TH1D*)outlist->FindObject(Form("COMThetaAll_convert"));
  allptr->Fill(BeThetaCOMMath,1/(2.*TMath::Pi()*TMath::Sin(toRadians(BeThetaCOMMath))*toRadians(1)));
  allptr->Fill(Be8ThetaCOMMath,1/(2.*TMath::Pi()*TMath::Sin(toRadians(Be8ThetaCOMMath))*toRadians(1)));
  
  if(TotalHits == 0)
  {
//     cout<<"Continue due to TotalHits == 0"<<endl;
    return 0;
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
        TotalHits--;
      }
    }
  }

  TH1D* hitptr = (TH1D*)outlist->FindObject(Form("COMThetaHit"));
  hitptr->Fill(BeThetaCOMMath);
  hitptr->Fill(Be8ThetaCOMMath);

  vector<HitStruct> HitsVec;
  
  if(((-180<BePhiMath && BePhiMath<-90) || (90<BePhiMath && BePhiMath<180)) && Hits[3]>0) //Be went to D3
  {
    HitStruct tmpStruct;
    
    tmpStruct.Theta = BeThetaMath;
    tmpStruct.Phi = BePhiMath;
    tmpStruct.Energy = BeEnergyMath;
    tmpStruct.StripX = 15-XStrip[3][0];
    tmpStruct.StripY = YStrip[3][0];
    tmpStruct.ThetaCOM = BeThetaCOMMath;
    tmpStruct.Excite = BeExciteE;
    tmpStruct.TotalExcite = BeExciteE + Be8ExciteE;
    if(Hits[4]>0)
      tmpStruct.PunchThrough = 1;
    else
      tmpStruct.PunchThrough = 0;
    tmpStruct.Detector = 2;
    
    HitsVec.push_back(tmpStruct);
//     cout<<"A"<<endl;
  }
  
  if((BePhiMath > -90. && BePhiMath < 90.) && Hits[1]>0)//Be went to D1
  {
    HitStruct tmpStruct;
    
    tmpStruct.Theta = BeThetaMath;
    tmpStruct.Phi = BePhiMath;
    tmpStruct.Energy = BeEnergyMath;
    tmpStruct.StripX = XStrip[1][0];
    tmpStruct.StripY = YStrip[1][0];
    tmpStruct.ThetaCOM = BeThetaCOMMath;
    tmpStruct.Excite = BeExciteE;
    tmpStruct.TotalExcite = BeExciteE + Be8ExciteE;
    if(Hits[2]>0)
      tmpStruct.PunchThrough = 1;
    else
      tmpStruct.PunchThrough = 0;
    tmpStruct.Detector = 1;
    
    HitsVec.push_back(tmpStruct);
//     cout<<"B"<<endl;
    
  }
  
  if(((-180<Be8PhiMath && Be8PhiMath<-90) || (90<Be8PhiMath && Be8PhiMath<180)) && Hits[3]>0) //Be8 went to D3
  {
    HitStruct tmpStruct;
    
    tmpStruct.Theta = Be8ThetaMath;
    tmpStruct.Phi = Be8PhiMath;
    tmpStruct.Energy = Be8EnergyMath;
    tmpStruct.StripX = 15-XStrip[3][0];
    tmpStruct.StripY = YStrip[3][0];
    tmpStruct.ThetaCOM = Be8ThetaCOMMath;
    tmpStruct.Excite = Be8ExciteE;
    tmpStruct.TotalExcite = BeExciteE + Be8ExciteE;
    if(Hits[4]>0)
      tmpStruct.PunchThrough = 1;
    else
      tmpStruct.PunchThrough = 0;
    tmpStruct.Detector = 2;
    
    HitsVec.push_back(tmpStruct);
//     cout<<"C"<<endl;
    
  }
  
  if((Be8PhiMath > -90. && Be8PhiMath < 90.) && Hits[1]>0)//Be8 went to D1
  {
    HitStruct tmpStruct;
    
    tmpStruct.Theta = Be8ThetaMath;
    tmpStruct.Phi = Be8PhiMath;
    tmpStruct.Energy = Be8EnergyMath;
    tmpStruct.StripX = XStrip[1][0];
    tmpStruct.StripY = YStrip[1][0];
    tmpStruct.ThetaCOM = Be8ThetaCOMMath;
    tmpStruct.Excite = Be8ExciteE;
    tmpStruct.TotalExcite = BeExciteE + Be8ExciteE;
    if(Hits[2]>0)
      tmpStruct.PunchThrough = 1;
    else
      tmpStruct.PunchThrough = 0;
    tmpStruct.Detector = 1;
    
    HitsVec.push_back(tmpStruct);
//     cout<<"D"<<endl;
    
  }  

  if(DEBUG) cout<<"Made HitVec"<<endl;
  random_shuffle(HitsVec.begin(),HitsVec.end());
  
//   for(int iter = 0;iter<int(HitsVec.size());iter++)
//   {
//     if(HitsVec.at(iter).Detector ==1 && HitsVec.at(iter).StripX==1 && HitsVec.at(iter).StripY==1)
//     {
//       cout<<"HitsVecIter: "<<iter+1<<" of "<<HitsVec.size()<<endl;
//       cout<<"Hits[1]: "<<Hits[1]<<", Hits[2]: "<<Hits[2]<<", Hits[3]: "<<Hits[3]<<", Hits[4]: "<<Hits[4]<<endl;
//       
//       cout<<"BeEnergyMath     "<<BeEnergyMath    <<endl;
//       cout<<"***BeThetaMath      "<<BeThetaMath     <<endl;
//       cout<<"Be8EnergyMath    "<<Be8EnergyMath   <<endl;
//       cout<<"***Be8ThetaMath     "<<Be8ThetaMath    <<endl;
//       cout<<"BeThetaCOMMath   "<<BeThetaCOMMath  <<endl;
//       cout<<"Be8ThetaCOMMath  "<<Be8ThetaCOMMath <<endl;
//       cout<<"BePhiMath        "<<BePhiMath       <<endl;
//       cout<<"Be8PhiMath       "<<Be8PhiMath      <<endl;
//       cout<<"BeExciteE        "<<BeExciteE       <<endl;
//       cout<<"Be8ExciteE       "<<Be8ExciteE      <<endl;
//       
//       cout<<"Theta       : "<<HitsVec.at(iter).Theta       <<endl;
//       cout<<"Phi         : "<<HitsVec.at(iter).Phi         <<endl;
//       cout<<"Energy      : "<<HitsVec.at(iter).Energy      <<endl;
//       cout<<"StripX      : "<<HitsVec.at(iter).StripX      <<endl;
//       cout<<"StripY      : "<<HitsVec.at(iter).StripY      <<endl;
//       cout<<"ThetaCOM    : "<<HitsVec.at(iter).ThetaCOM    <<endl;
//       cout<<"Excite      : "<<HitsVec.at(iter).Excite      <<endl;
//       cout<<"TotalExcite : "<<HitsVec.at(iter).TotalExcite <<endl;
//       cout<<"PunchThrough: "<<HitsVec.at(iter).PunchThrough<<endl;
//       cout<<"Detector    : "<<HitsVec.at(iter).Detector    <<endl;
//       
//       TVector3 tmppos = GetPositions(HitsVec.at(iter).Detector,'d', HitsVec.at(iter).StripX, HitsVec.at(iter).StripY);
//       cout<<"Position Checks! HitVec Theta: "<<HitsVec.at(iter).Theta<<", Phi: "<<HitsVec.at(iter).Phi<<endl;
//       cout<<"Position Checks! tmppos Theta: "<<toDegrees(tmppos.Theta())<<", Phi: "<<toDegrees(tmppos.Phi())<<endl<<endl;
// 
//     }
//   }
  
  for(int iter = 0;iter<int(HitsVec.size());iter++)
  {
//     if(HitsVec.at(iter).ThetaCOM > 140.)
//       continue;
    
//     if(HitsVec.at(iter).ThetaCOM > 90.)
//       continue;
//     
//     if(HitsVec.size() >1)
//     {
//       double ThisEnergy = HitsVec.at(iter).Energy;
//       double OtherEnergy = -1;
//       if(iter == 0)
//         OtherEnergy = HitsVec.at(1).Energy;
//       else
//         OtherEnergy = HitsVec.at(0).Energy;
//       if(ThisEnergy < OtherEnergy)
//       {
// //         cout<<" Continue due to ThisEnergy < OtherEnergy, didn't expect to see this"<<endl;
//         continue;
//       }
//     }
    
//     if(HitsVec.size() <2)
//       continue;
    
    TH1D* hitptr = (TH1D*)outlist->FindObject(Form("COMThetaHit_loop"));
    hitptr->Fill(BeThetaCOMMath);
    hitptr->Fill(Be8ThetaCOMMath);
    hitptr = (TH1D*)outlist->FindObject(Form("COMThetaHit_vec"));
    hitptr->Fill(HitsVec.at(iter).ThetaCOM);
    hitptr = (TH1D*)outlist->FindObject(Form("COMThetaHit_frac"));
    double frac = ManualFracCOM(HitsVec.at(iter).TotalExcite, toRadians(HitsVec.at(iter).ThetaCOM));
    hitptr->Fill(HitsVec.at(iter).ThetaCOM,frac);
    double solid = 1./GetSolidAngle(HitsVec.at(iter).StripX,HitsVec.at(iter).StripY);
    hitptr = (TH1D*)outlist->FindObject(Form("COMThetaHit_solid"));
    hitptr->Fill(HitsVec.at(iter).ThetaCOM,solid);
    hitptr = (TH1D*)outlist->FindObject(Form("COMThetaHit_frac_solid"));
    hitptr->Fill(HitsVec.at(iter).ThetaCOM,frac*solid);
    
    int ringNo = RingNumber(HitsVec.at(iter).StripX, HitsVec.at(iter).StripY);
    int state = -1;
    if(HitsVec.at(iter).TotalExcite >11)
      state=12;
    else if(int(HitsVec.at(iter).TotalExcite)==9)
      state=9;
    else if(HitsVec.at(iter).TotalExcite>5)
      state=6;
    else if(int(HitsVec.at(iter).TotalExcite)==3)
      state=3;
    else if(int(HitsVec.at(iter).TotalExcite)==0)
      state=0;
    else
      cout<<"Unknown state: "<<HitsVec.at(iter).TotalExcite<<", "<<HitsVec.at(iter).Excite<<endl;
        
    TString type = "default";
    if(HitsVec.at(iter).PunchThrough && int(HitsVec.size()) == 1)
      type = "pid";
    else if(int(HitsVec.size()) == 2)
      type = "dual";
    else
    {
      type = "other";
//       cout<<"Exception in PID vs Dual determination!"<<endl;
//       cout<<" HitsVec.size(): "<<HitsVec.size()<<endl;
//       continue;
    }
//     type = "pid";
//     cout<<"State: "<<state<<", Detector: "<<HitsVec.at(iter).Detector<<", type: "<<type<<endl;
    TH1D* tmpPtr1D;
    TH2D* tmpPtr2D;

    if(DEBUG) cout<<"Looking for: "<<Form("RingCounts_s%02i_d%i_%s",state,HitsVec.at(iter).Detector,type.Data())<<endl;
    
    tmpPtr1D = (TH1D*)outlist->FindObject(Form("RingCounts_s%02i_d%i_%s",state,HitsVec.at(iter).Detector,type.Data()));
    tmpPtr1D->Fill(ringNo);
    
    tmpPtr1D = (TH1D*)outlist->FindObject(Form("COMTheta_s%02i_d%i_%s",state,HitsVec.at(iter).Detector,type.Data()));
    tmpPtr1D->Fill(HitsVec.at(iter).ThetaCOM);

    tmpPtr1D = (TH1D*)outlist->FindObject(Form("COMTheta_convert_s%02i_d%i_%s",state,HitsVec.at(iter).Detector,type.Data()));
    tmpPtr1D->Fill(HitsVec.at(iter).ThetaCOM,1./(2.*TMath::Pi()*TMath::Sin(toRadians(HitsVec.at(iter).ThetaCOM))*toRadians(1)));
    
    tmpPtr2D = (TH2D*)outlist->FindObject(Form("HitPattern_s%02i_d%i_%s",state,HitsVec.at(iter).Detector,type.Data()));
    tmpPtr2D->Fill(HitsVec.at(iter).StripX,HitsVec.at(iter).StripY);
    
    tmpPtr2D = (TH2D*)outlist->FindObject(Form("RingVThetaCOM_s%02i_d%i_%s",state,HitsVec.at(iter).Detector,type.Data()));
    tmpPtr2D->Fill(HitsVec.at(iter).ThetaCOM,ringNo);
    
    tmpPtr2D = (TH2D*)outlist->FindObject(Form("RingVThetaLab_s%02i_d%i_%s",state,HitsVec.at(iter).Detector,type.Data()));
    tmpPtr2D->Fill(HitsVec.at(iter).Theta,ringNo);
    
    tmpPtr2D = (TH2D*)outlist->FindObject(Form("EvTheta_s%02i_d%i_%s",state,HitsVec.at(iter).Detector,type.Data()));
    tmpPtr2D->Fill(HitsVec.at(iter).Theta,HitsVec.at(iter).Energy);
  }

   return 1;
}

void AngSelector::SlaveTerminate()
{
}

void AngSelector::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.
  
  cout<<"Terminate"<<endl;

  TH1D* tmpPtr1D;
  TH2D* tmpPtr2D;
  
  for(int dettypeI = 0; dettypeI<3;dettypeI++)
  {
    for(int state = 0; state<=12;state += 3)
    {
  
//   int dettypeI = 0;
//   int state = 0;
      for(int det = 1;det<3;det++)
      {
        TString dettype = "def";
        if(dettypeI == 0)
          dettype = "pid";
        else if(dettypeI == 1)
          dettype = "dual";
        else if(dettypeI == 2)
          dettype = "other";
        else
          cerr<<"Overshot on Dettype"<<endl;
        
        bool bendback = 1;
        if(state == 0)
          bendback = 0;
        
        cout<<endl<<"Detector "<<det<<", state: "<<state<<", dettype: "<<dettype<<endl;
        TGraphAsymmErrors *ringG = (TGraphAsymmErrors*)ringFile->Get(Form("COM_d%i_s%i",det,state));
        TGraphAsymmErrors *labG = (TGraphAsymmErrors*)ringFile->Get(Form("Lab_d%i_s%i",det,state));
        TGraphAsymmErrors *ringGOther = new TGraphAsymmErrors();
        if(bendback)
          ringGOther = (TGraphAsymmErrors*)ringFile->Get(Form("COMOther_d%i_s%i",det,state));        
        if (!ringG)
          continue;
        
        vector<double> center;
        vector<double> centererr;
        vector<double> counts;
        vector<double> countserr;
        vector<double> centerLab;
        vector<double> centererrLab;
        

        for(int i =0;i<ringG->GetN();i++)
        {
//           cout<<" i: "<<i<<endl;
          double cen = ringG->GetY()[i];
          double labCen = labG->GetY()[i];
          double cerr = .6827*((ringG->GetEYhigh()[i]+ringG->GetEYlow()[i])/2.);
          double labCenerr = .6827*((labG->GetEYhigh()[i]+labG->GetEYlow()[i])/2.);
          double cenOther = 0.;
          double cerrOther = 0.;
          if(bendback)
          {
            cenOther = ringGOther->GetY()[i];
            cerrOther = .6827*((ringGOther->GetEYhigh()[i]+ringGOther->GetEYlow()[i])/2.);
            
            if(int(ringG->GetX()[i]) != int(ringGOther->GetX()[i]))
              cout<<"X and X Other don't match: "<<ringG->GetX()[i]<<", "<<ringGOther->GetX()[i]<<endl;
          }

          double c = -1.;
          
          TH1D* tmpPtr1D = (TH1D*)outlist->FindObject(Form("RingCounts_s%02i_d%i_%s",state,det,dettype.Data()));
          c = tmpPtr1D->GetBinContent(i+1);
          
          TH2D* tmpPtr2D = (TH2D*)outlist->FindObject(Form("RingVThetaCOM_s%02i_d%i_%s",state,det,dettype.Data()));
          tmpPtr1D = tmpPtr2D->ProjectionX(Form("Ring %i",i),i,i+1);
          
//           if(tmpPtr1D->GetMean() > 1)
//             cout<<"Old Center: "<<cen<<", New Center: "<<tmpPtr1D->GetMean()<<", difference: "<<cen-tmpPtr1D->GetMean()<<endl;
//           
//           cen = tmpPtr1D->GetMean();
//           cerr = tmpPtr1D->GetRMS();
          
          if(c<1)
          {
//             cout<<"contine due to no counts"<<endl;
            continue;
          }
          
          double exciteVal = -1;
          switch(state)
          {
            case 0:
              exciteVal = 0.;
              break;
            case 3:
              exciteVal = 3.368;
              break;
            case 6:
              exciteVal = 6.;
              break;
            case 9:
              exciteVal = 9.368;
              break;
            case 12:
              exciteVal = 12.;
              break;
          }
          
          double s = RingSA(i);
          if(s == 0.)
          {
            cout<<"continue due to no solid angle"<<endl;
            continue;
          }
          double w = ManualFracCOM(exciteVal, toRadians(cen));
          
          counts.push_back(c/s*w);
          countserr.push_back(sqrt(c)/s*w);
          
          printf("Ring: %2i, counts: %5.f, solidAngle: %5f, weight: %5f, weighted counts: %6.f, error: %5.f\n center: %8.3f, error: %3.1f, centerOther: %5.1f, errorOther: %3.1f\n",i,c,s,w,c/s*w,sqrt(c)/s*w,cen,cerr,cenOther,cerrOther);
          center.push_back(cen);
          centererr.push_back(cerr);
          centerLab.push_back(labCen);
          centererrLab.push_back(labCenerr);
        }
        
        if(center.size() != centererr.size())
          cerr<<"Size mismatch between center and centererr"<<endl;
        
        if(counts.size() != countserr.size())
          cerr<<"Size mismatch between counts and countserr"<<endl;
        
        if(center.size() != counts.size())
          cerr<<"Size mismatch between center and counts"<<endl;
        
        if(center.size() < 1)
          continue;     
        
        TGraphErrors *TG = new TGraphErrors(center.size(),&(center[0]),&(counts[0]),&(centererr[0]),&(countserr[0]));
        TG->SetTitle(Form("Angular Distribution detector %i, state %i, %s detection type",det,state,dettype.Data()));
        TG->SetName(Form("AD_d%i_s%i_%s",det,state,dettype.Data()));
        outfile->cd();
        TG->Write();
        
//         TGraphErrors *TGLab = new TGraphErrors(center.size(),&(centerLab[0]),&(counts[0]),&(centererrLab[0]),&(countserr[0]));
//         TGLab->SetTitle(Form("Lab Distribution detector %i, state %i, %s detection type",det,state,dettype.Data()));
//         TGLab->SetName(Form("Lab_d%i_s%i_%s",det,state,dettype.Data()));
//         outfile->cd();
//         TGLab->Write();
      }
    }
  }  
  
  outfile->cd();
  
      TH1* hist = (TH1*)outlist->First();
      TH1* nexthist;// = (TH1*)outlist->After(outlist->First());
      while(hist)
      {
        nexthist = (TH1*)outlist->After(hist);
        if(hist->GetEntries() < 1)
        {
  //         cout<<"For removal:"<<endl;
  //         hist->Print();
          outlist->Remove(hist);
        }
        hist = nexthist;
      }
      
//       TH1D* tmp = (TH1D*)outlist->FindObject("COMTheta_s00_d1_pid");
//       TH1D* tmpSum = tmp->Clone("tmpSum");
//       tmp = (TH1D*)outlist->FindObject("COMTheta_s00_d2_pid");
//       tmpSum->Add(tmp);
//       tmpSum->Write();
      
  
  outlist->Write();
}

double ManualFracCOM(double ExcitedState, double ThetaCOM)
{
  if(ExcitedState > 15.)
    cerr<<"ManualFracCOM takes MeV"<<endl;
  const double M1 = MASS_BE11;
  const double M2 = MASS_BE9;
  const double M3 = MASS_BE10;
  const double M4 = MASS_BE10;
    
  ExcitedState -= MASS_BE11 + MASS_BE9 - (MASS_BE10*2.);
  ExcitedState = -ExcitedState;
  
//   cout<<"Excited State after: "<<ExcitedState<<endl;
  
  double COMTotalE = M2 / ( M1 + M2 ) * BEAM_ENERGY;
  double K = sqrt((M1*M4*COMTotalE)/(M2*M3*(COMTotalE+ExcitedState)));
  
//   cout<<"ManualFracCOM ExcitedState: "<<ExcitedState<<endl;
//   
//   cout<<"ManualFracCOM K: "<<K<<endl;
  
  double Num = 1. + K * cos(ThetaCOM);
  double Den = pow(1. + K*K + 2*K*cos(ThetaCOM),1.5);
  
//   cout<<"ManualFracCOM Num: "<<Num<<", Den: "<<Den<<", Result: "<<abs(Num/Den)<<endl;
  
  return (abs(Num/Den));
}

int RingNumber(int stripX, int stripY)
{  
  TH2D* histo = (TH2D*)ringFile->Get("Total_Rings_0_d1_pid");
  
  int binNo = histo->GetBin(stripX+1,stripY+1);
  int Ring = histo->GetBinContent(binNo);
  Ring -= 1;
  
  return Ring;
}

double RingSA(int Ring)
{  
  TH1D* spec = (TH1D*)ringFile->Get("SA_0_d1_pid");
  double TotalSolidAngle = spec->GetBinContent(Ring+1);
  
  return TotalSolidAngle;
}

double Keri_GetfCM(double Exstate, double ThetaCM)
{
  double K = Keri_GetKTransfer(Exstate);
  
//   cout<< "Keri K: "<<K<<endl;
  
  double Num = 1. + K * cos(ThetaCM);
  double Den = pow(1. + K*K + 2*K*cos(ThetaCM),1.5);
  
//   cout<<"Keri Num: "<<Num<<", Den: "<<Den<<", Result: "<<abs(Num/Den)<<endl;
  
  if (Num>0)
    return (Num/Den);
  else
    return (-Num/Den);
}

double Keri_GetKTransfer(double Exstate)
{
  double m1 = MASS_BE11;
  double m2 = MASS_BE9;
  double m3 = MASS_BE10;
  double m4 = MASS_BE10;
  
  double ExciteM = (m4 + Exstate);
  double QExcite = (m2 + m1 - m3 - ExciteM);
  double CenterOfMassTotalE = (m2*BEAM_ENERGY)/(m1+m2);
  double K = sqrt((m1*m4*CenterOfMassTotalE)/(m2*m3*(CenterOfMassTotalE+QExcite)));
//   cout<<"Keri QExcite: "<<QExcite<<endl;
//   double K = sqrt((M1*M4*COMTotalE)/(M2*M3*(COMTotalE+ExcitedState)));
  
  return (K);
}

double toDegrees(double ang)
{
  return ang*180./TMath::Pi();
}

double toRadians(double ang)
{
  return ang*TMath::Pi()/180.;
}

double GetSolidAngle(int xstrip,int ystrip)
{
  TH2D* SAhisto = (TH2D*)SAfile->Get("sa1d");
  double solidAngle = SAhisto->GetBinContent(SAhisto->GetBin(xstrip+1,ystrip+1));
  return(solidAngle);
}

TVector3 GetPositions(int detector,char pos, int horizontalstrip, int verticalstrip)
{
  //horizontal strips collect N charge!
  //vertical strips collect P charge!
  horizontalstrip -=1;
  TVector3 Pos;
  double detTheta = 31. * (TMath::Pi()/180.);
  double SideX = 64.62;
  double SideZ = 2.05;
  double dER = 62.14;
  double ER = 75.35;
  double x = 0.0,y = 0.0,z = 0.0;
  
  double halfpixel = 50./32.;
  double fullpixel = 50./16.;
  
  bool flip = 0;
  
  if(detector == 1)
    detTheta = -detTheta;
  if(pos=='D'&&detector<3)
  {
    verticalstrip=15-verticalstrip;
    flip = 1;
  }
  
  x = halfpixel*(2*verticalstrip+1) - fullpixel*8;
  y = halfpixel*(2*horizontalstrip+1) - fullpixel*8;
  
  if(flip)
  {
    x += fullpixel;
  }
  
  if(pos=='D')
    z = dER;
  else
    z = ER;
  
  double xp = z*sin(detTheta) + x*cos(detTheta);
  double zp = z*cos(detTheta) - x*sin(detTheta);
  
  if(detector==3&&pos=='D')
  {
    //Right Side
    verticalstrip=15-verticalstrip;
    flip = 1;
    xp = -SideX;
    zp = SideZ + halfpixel*(2*verticalstrip+1) - fullpixel*8;
  }
  else if(detector==4&&pos=='D')
  {
    //Left Side
    xp = SideX;
    zp = SideZ + halfpixel*(2*verticalstrip-1) - fullpixel*8;
  }
  
  Pos.SetX(xp);
  Pos.SetY(y);
  Pos.SetZ(zp);
  
//   TVector3 *RetPos = new TVector3[5];
/*  
  TVector3 vertMove(0.,halfpixel,0.);
  TVector3 horMove(halfpixel,0.,0.);
  horMove.RotateY(detTheta);
  
  if(detector>2)
    horMove.SetXYZ(0.,0.,halfpixel);*/
  
//   if(!flip)
//   {
//     RetPos[0] = Pos + vertMove + horMove;
//     RetPos[1] = Pos - vertMove + horMove;
//     RetPos[2] = Pos - vertMove - horMove;
//     RetPos[3] = Pos + vertMove - horMove;
//   }
//   else if(flip)
//   {
//     RetPos[0] = Pos + vertMove - horMove;
//     RetPos[1] = Pos - vertMove - horMove;
//     RetPos[2] = Pos - vertMove + horMove;
//     RetPos[3] = Pos + vertMove + horMove;
//   }
//   
//   RetPos[4] = Pos;

//   return(RetPos);
  return(Pos);
}