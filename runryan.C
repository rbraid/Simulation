void runryan()
{
  TFile *file = new TFile("EventOut.root","read");

  TTree *tree = (TTree*)file->Get("ryandata");

  tree->Process("NewRyanSelector.C++");

  gROOT->ProcessLine(".q");
}	
