void runAngSelector(TString fileloc)
{
  TFile *file = new TFile(fileloc,"read");

  TTree *tree = (TTree*)file->Get("ryandata");

  tree->Process("/home/ryan/nuclear/be9/1010/MathAngDist/AngSelector.C++");

  gROOT->ProcessLine(".q");
}	
