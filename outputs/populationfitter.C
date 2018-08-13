using namespace std;

void populationfitter()
{
  gStyle->SetOptStat(0);
  
  TFile *outlow = TFile::Open("outputlow.root","read");
  outlow->cd();
  
  TH1D *data1 = (TH1D*)Be10Ex1->Clone("data1");
  TH1D *data2 = (TH1D*)Be10Ex2->Clone("data2");
  TH1D *data = data1->Clone("data");
  data->Add(data2);
  data->Rebin();
  
  TH1D *dataDual = (TH1D*)DualBe10_ex_allcut->Clone("dataDual");
  //cout<<"Number of bins in data: "<<data->GetSize()<<endl;
  
  //////////////////////////////////////////////////////////// 
  TFile *evt_GS_GS = TFile::Open("/home/ryan/nuclear/be9/1010/eventoutputs/simoutputlow_gs_gs.root","read");
  evt_GS_GS->cd();
  TH1D *mathevt10_GS_GS_1 = (TH1D*)Be10Ex1->Clone("mathevt10_GS_GS_1");
  TH1D *mathevt10_GS_GS_2 = (TH1D*)Be10Ex2->Clone("mathevt10_GS_GS_2");
  TH1D *dual_GS_GS = (TH1D*)DualBe10_ex_allcut->Clone("dual_GS_GS");
  
  TFile *evt_3368_GS = TFile::Open("/home/ryan/nuclear/be9/1010/eventoutputs/simoutputlow_gs_3368.root","read");
  evt_3368_GS->cd();
  TH1D *mathevt10_3368_GS_1 = (TH1D*)Be10Ex1->Clone("mathevt10_3368_GS_1");
  TH1D *mathevt10_3368_GS_2 = (TH1D*)Be10Ex2->Clone("mathevt10_3368_GS_2");
  TH1D *dual_3368_GS = (TH1D*)DualBe10_ex_allcut->Clone("dual_3368_GS");
  
  TFile *evt_6band_GS = TFile::Open("/home/ryan/nuclear/be9/1010/eventoutputs/simoutputlow_6band_gs.root","read");
  evt_6band_GS->cd();
  TH1D *mathevt10_6band_GS_1 = (TH1D*)Be10Ex1->Clone("mathevt10_6band_GS_1");
  TH1D *mathevt10_6band_GS_2 = (TH1D*)Be10Ex2->Clone("mathevt10_6band_GS_2");
  TH1D *dual_6band_GS = (TH1D*)DualBe10_ex_allcut->Clone("dual_6band_GS");
  
  TFile *evt_7542_GS = TFile::Open("/home/ryan/nuclear/be9/1010/eventoutputs/simoutputlow_7542_gs.root","read");
  evt_7542_GS->cd();
  TH1D *mathevt10_7542_GS_1 = (TH1D*)Be10Ex1->Clone("mathevt10_7542_GS_1");
  TH1D *mathevt10_7542_GS_2 = (TH1D*)Be10Ex2->Clone("mathevt10_7542_GS_2");
  TH1D *dual_7542_GS = (TH1D*)DualBe10_ex_allcut->Clone("dual_7542_GS");
  
  TFile *evt_3368_3368 = TFile::Open("/home/ryan/nuclear/be9/1010/eventoutputs/simoutputlow_3368_3368.root","read");
  evt_3368_3368->cd();
  TH1D *mathevt10_3368_3368_1 = (TH1D*)Be10Ex1->Clone("mathevt10_3368_3368_1");
  TH1D *mathevt10_3368_3368_2 = (TH1D*)Be10Ex2->Clone("mathevt10_3368_3368_2");
  TH1D *dual_3368_3368 = (TH1D*)DualBe10_ex_allcut->Clone("dual_3368_3368");
  
  TFile *evt_6band_3368 = TFile::Open("/home/ryan/nuclear/be9/1010/eventoutputs/simoutputlow_3368_6band.root","read");
  evt_6band_3368->cd();
  TH1D *mathevt10_6band_3368_1 = (TH1D*)Be10Ex1->Clone("mathevt10_6band_3368_1");
  TH1D *mathevt10_6band_3368_2 = (TH1D*)Be10Ex2->Clone("mathevt10_6band_3368_2");
  TH1D *dual_6band_3368 = (TH1D*)DualBe10_ex_allcut->Clone("dual_6band_3368");
  
  TFile *evt_6band_6band = TFile::Open("/home/ryan/nuclear/be9/1010/eventoutputs/simoutputlow_6band_6band.root","read");
  evt_6band_6band->cd();
  TH1D *mathevt10_6band_6band_1 = (TH1D*)Be10Ex1->Clone("mathevt10_6band_6band_1");
  TH1D *mathevt10_6band_6band_2 = (TH1D*)Be10Ex2->Clone("mathevt10_6band_6band_2");
  TH1D *dual_6band_6band = (TH1D*)DualBe10_ex_allcut->Clone("dual_6band_6band");
  
  TH1D *mathevt10_GS_GS = mathevt10_GS_GS_1->Clone("mathevt10_GS_GS");
  mathevt10_GS_GS->Add(mathevt10_GS_GS_2);
  
  TH1D *mathevt10_3368_GS = mathevt10_3368_GS_1->Clone("mathevt10_3368_GS");
  mathevt10_3368_GS->Add(mathevt10_3368_GS_2);
  
  TH1D *mathevt10_6band_GS = mathevt10_6band_GS_1->Clone("mathevt10_6band_GS");
  mathevt10_6band_GS->Add(mathevt10_6band_GS_2);
  
  TH1D *mathevt10_7542_GS = mathevt10_7542_GS_1->Clone("mathevt10_7542_GS");
  mathevt10_7542_GS->Add(mathevt10_7542_GS_2);
  
  TH1D *mathevt10_3368_3368 = mathevt10_3368_3368_1->Clone("mathevt10_3368_3368");
  mathevt10_3368_3368->Add(mathevt10_3368_3368_2);
  
  TH1D *mathevt10_6band_3368 = mathevt10_6band_3368_1->Clone("mathevt10_6band_3368");
  mathevt10_6band_3368->Add(mathevt10_6band_3368_2);
  
  TH1D *mathevt10_6band_6band = mathevt10_6band_6band_1->Clone("mathevt10_6band_6band");
  mathevt10_6band_6band->Add(mathevt10_6band_6band_2);
  
  data->GetXaxis()->SetRangeUser(-1,2);
  double scalegg = data->GetMaximum() / mathevt10_GS_GS->GetMaximum();
  
  data->GetXaxis()->SetRangeUser(3,4.5);
  double scale3g = data->GetMaximum() / mathevt10_3368_GS->GetMaximum();
  
  data->GetXaxis()->SetRangeUser(6,8);
  double scale6g = (data->GetMaximum() / mathevt10_6band_GS->GetMaximum()) / 2.;
  data->GetXaxis()->SetRangeUser(6,8);
  double scale33 = (data->GetMaximum() / mathevt10_3368_3368->GetMaximum()) / 2.;
  
  data->GetXaxis()->SetRangeUser(7,7);
  double scale7g = data->GetMaximum() / mathevt10_7542_GS->GetMaximum();
  
  data->GetXaxis()->SetRangeUser(9,10.5);
  double scale63 = data->GetMaximum() / mathevt10_6band_3368->GetMaximum();
  
//   data->GetXaxis()->SetRangeUser(-1,2);
//   double scale66 = data->GetMaximum() / mathevt10_6band_6band->GetMaximum();
  
  data->GetXaxis()->SetRangeUser(-5,20);
  
  mathevt10_GS_GS->Scale(scalegg);
  mathevt10_3368_GS->Scale(scale3g);
  mathevt10_6band_GS->Scale(scale6g);
  mathevt10_3368_3368->Scale(scale33);
  mathevt10_6band_3368->Scale(scale63);
  mathevt10_7542_GS->Scale(scale7g);
  //mathevt10_6band_6band->Scale(scale66);
  
  TH1D *simsum = mathevt10_GS_GS->Clone("simsum");
  simsum->Add(mathevt10_3368_GS);
  simsum->Add(mathevt10_6band_GS);
  simsum->Add(mathevt10_3368_3368);
  simsum->Add(mathevt10_6band_3368);
//   simsum->Add(mathevt10_7542_GS);
  //simsum->Add(mathevt10_6band_6band);
  
  //*********
  dataDual->GetXaxis()->SetRangeUser(9,10.5);
  double scaleD63 = dataDual->GetMaximum() / dual_6band_3368->GetMaximum();
  
  dataDual->GetXaxis()->SetRangeUser(11,13);
  double scaleD66 = dataDual->GetMaximum() / dual_6band_6band->GetMaximum();
  
  dataDual->GetXaxis()->SetRangeUser(-5,20);
  dual_6band_3368->Scale(scaleD63);
  dual_6band_6band->Scale(scaleD66);
  
  TH1D *Dualsimsum = dual_6band_3368->Clone("Dualsimsum");
  Dualsimsum->Add(dual_6band_6band);
  
  vector<double> height;
  
  height.push_back(mathevt10_GS_GS->GetMaximum());
  height.push_back(mathevt10_3368_GS->GetMaximum()   );
  height.push_back(mathevt10_6band_GS->GetMaximum()  );
  height.push_back(mathevt10_3368_3368->GetMaximum() );
  height.push_back(mathevt10_6band_3368->GetMaximum());
  //height.push_back(dual_6band_3368->GetMaximum()     *mathevt10_6band_3368->GetMaximum()/dual_6band_3368->GetMaximum()  );
  height.push_back(dual_6band_6band->GetMaximum()*mathevt10_6band_3368->GetMaximum()/dual_6band_3368->GetMaximum()    );
  
  
  cout<<"Before Scaling, my heights are:"<<endl;
  cout<<"\tGS_GS: "<<mathevt10_GS_GS->GetMaximum()<<endl;
  cout<<"\t3368_GS: "<<mathevt10_3368_GS->GetMaximum()<<endl;
  cout<<"\t6band_GS: "<<mathevt10_6band_GS->GetMaximum()<<endl;
  cout<<"\t3368_3368: "<<mathevt10_3368_3368->GetMaximum()<<endl;
  cout<<"\t6band_3368: "<<mathevt10_6band_3368->GetMaximum()<<endl;
  //cout<<"\tdual_6band_3368: "<<dual_6band_3368->GetMaximum()<<endl;
  cout<<"\t6band_6band: "<<dual_6band_6band->GetMaximum()*mathevt10_6band_3368->GetMaximum()/dual_6band_3368->GetMaximum()    <<endl;
  
  //cout<<"Number of bins in simsum: "<<simsum->GetSize()<<endl;
  ////////////////////////////////////////////////////////////
  
  mathevt10_GS_GS->SetMarkerColor(kBlue);
  mathevt10_3368_GS->SetMarkerColor(kMagenta);
  mathevt10_3368_3368->SetMarkerColor(kRed);
  mathevt10_6band_GS->SetMarkerColor(kCyan);
  mathevt10_6band_3368->SetMarkerColor(kOrange);
  mathevt10_6band_6band->SetMarkerColor(TColor::GetColorDark(kGreen));
  mathevt10_7542_GS->SetMarkerColor(TColor::GetColorDark(kGreen));
  simsum->SetMarkerColor(kBlack);
  data->SetMarkerColor(kGreen);
  
  mathevt10_GS_GS->SetLineColor(kBlue);
  mathevt10_3368_GS->SetLineColor(kMagenta);
  mathevt10_3368_3368->SetLineColor(kRed);
  mathevt10_6band_GS->SetLineColor(kCyan);
  mathevt10_6band_3368->SetLineColor(kOrange);
  mathevt10_6band_6band->SetLineColor(TColor::GetColorDark(kGreen));
  mathevt10_7542_GS->SetLineColor(TColor::GetColorDark(kGreen));
  simsum->SetLineColor(kBlack);
  data->SetLineColor(kGreen);
  
  simsum->SetLineStyle(2);
  data->SetLineStyle(2);
  simsum->SetLineWidth(3);
  data->SetLineWidth(3);
  
  mathevt10_GS_GS->SetTitle("Ground State and Ground State");
  mathevt10_3368_GS->SetTitle("3.368 MeV and Ground State");
  mathevt10_3368_3368->SetTitle("3.368 MeV and 3.368 MeV");
  mathevt10_6band_GS->SetTitle("6 MeV Band and Ground State");
  mathevt10_6band_3368->SetTitle("6 MeV Band and 3.368 MeV");
  mathevt10_6band_6band->SetTitle("6 MeV Band and 6 MeV Band");
  mathevt10_7542_GS->SetTitle("7.542 MeV and Ground State");
  simsum->SetTitle("Sum of Simulated Excitations");
  
  //**********
  dual_6band_3368 ->SetMarkerColor(kBlue);
  dual_6band_6band->SetMarkerColor(kRed);
  Dualsimsum->SetMarkerColor(kBlack);
  dataDual->SetMarkerColor(kGreen);
  
  dual_6band_3368 ->SetLineColor(kBlue);
  dual_6band_6band->SetLineColor(kRed);
  Dualsimsum->SetLineColor(kBlack);
  dataDual->SetLineColor(kGreen);
  
  Dualsimsum->SetLineStyle(2);
  dataDual->SetLineStyle(2);
  Dualsimsum->SetLineWidth(3);
  dataDual->SetLineWidth(3);
  
  dual_6band_3368 ->SetTitle("Dual Particle 6 MeV Band and 3.368 MeV");
  dual_6band_6band->SetTitle("Dual Particle 6 MeV Band and 6 MeV Band");
  Dualsimsum->SetTitle("Sum of Simulated Excitations for Dual Particles");
  dataDual->SetTitle("Actual Dual Data");
  
  ////////////////////////////////////////////////////////////
  
  TCanvas *canvas = new TCanvas("canvas","shouldnotseethis",0,0,1280,720);
  
//   data1->Draw();
//   canvas->SaveAs("excheck1.png");
//   data2->Draw();
//   canvas->SaveAs("excheck2.png");  
  data->Draw();
  canvas->SaveAs("excheck.png");
  
  dataDual->Draw();
  canvas->SaveAs("excheckDual.png");
  
  THStack *Be10Stack = new THStack("Be10Stack","Total Excitation Spectrum and constituents, with overlaid data.");
   Be10Stack->Add(mathevt10_GS_GS);
   Be10Stack->Add(mathevt10_3368_GS);
   Be10Stack->Add(mathevt10_3368_3368);
   Be10Stack->Add(mathevt10_6band_GS);
   Be10Stack->Add(mathevt10_6band_3368);
//    Be10Stack->Add(mathevt10_7542_GS);
  //Be10Stack->Add(mathevt10_6band_6band);
  Be10Stack->Add(simsum);
  Be10Stack->Add(data);
  
  Be10Stack->Draw("nostack");
  Be10Stack->GetXaxis()->SetRangeUser(-2,15);
  //Be10Stack->GetYaxis()->SetLimits(0,40);
  //canvas->BuildLegend();
  
  TLegend *legend = new TLegend(.1,.9,.4,.25);
  //legend->SetTextAlign(13);
  //legend->SetBorderSize(0);
  legend->AddEntry(mathevt10_GS_GS);
  legend->AddEntry(mathevt10_3368_GS);
  legend->AddEntry(mathevt10_3368_3368);
  legend->AddEntry(mathevt10_6band_GS);
  legend->AddEntry(mathevt10_6band_3368);
//   legend->AddEntry(mathevt10_7542_GS);
  //legend->AddEntry(mathevt10_6band_6band);
  legend->AddEntry(simsum);
  legend->AddEntry(data);
  legend->Draw();
  canvas->SaveAs("populations.png");
  
  THStack *Be10StackDual = new THStack("Be10StackDual","Total Excitation Spectrum and constituents, with overlaid data.");
  Be10StackDual->Add(dataDual);
  Be10StackDual->Add(dual_6band_3368);
  Be10StackDual->Add(dual_6band_6band);
  Be10StackDual->Add(Dualsimsum);
  
  TLegend *legendDual = new TLegend(.1,.9,.4,.25);
  legendDual->AddEntry(dataDual);
  legendDual->AddEntry(dual_6band_3368);
  legendDual->AddEntry(dual_6band_6band);
  legendDual->AddEntry(Dualsimsum);
  
  Be10StackDual->Draw("nostack");
  Be10StackDual->GetXaxis()->SetRangeUser(-5,20);
  legendDual->Draw();
  canvas->SaveAs("populationsDual.png");
  
  vector<double> stateE;
  
  stateE.push_back(0.);
  stateE.push_back(0.+3.368);
  stateE.push_back(0.+6);
  stateE.push_back(3.368*2.);
  stateE.push_back(6.+3.368);
  stateE.push_back(6.*2.);

  double totalheight = 0;
  
  for(int iter = 0; iter<height.size(); iter++)
  {
    totalheight += height.at(iter);
  }
  
  vector<double> heightNorm;
  for(int iter = 0; iter<height.size(); iter++)
  {
    heightNorm.push_back(height.at(iter) / totalheight);
  }
  
  TGraph *HeightBasedPop = new TGraph(stateE.size(), &(stateE[0]), &(heightNorm[0]));
  HeightBasedPop->SetTitle("Fractional weight of each state in data");
  HeightBasedPop->GetXaxis()->SetTitle("State Total Energy");
  HeightBasedPop->GetYaxis()->SetTitle("State Weight");
  HeightBasedPop->Print();
  HeightBasedPop->Draw("AP*");
  canvas->SaveAs("final.png");
}