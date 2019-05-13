void rungrsiMakerSelector(TString fileloc, TString outloc = "SimCSMHits.root")
{
  TFile *file = new TFile(fileloc,"read");

  TTree *tree = (TTree*)file->Get("ryandata");

  tree->Process("~/nuclear/be9/1010/GRSIMaker/grsiMakerSelector.C++");
//   OutputFile = new TFile(outloc,"recreate");
  
  gROOT->ProcessLine(".q");
}	
