void rungrsiMakerSelector(TString fileloc)
{
  TFile *file = new TFile(fileloc,"read");

  TTree *tree = (TTree*)file->Get("ryandata");

  tree->Process("~/nuclear/be9/1010/GRSIMaker/grsiMakerSelector.C++");

  gROOT->ProcessLine(".q");
}	
