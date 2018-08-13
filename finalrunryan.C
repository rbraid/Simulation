void finalrunryan()
{
  TFile *file = new TFile("processed_event.root","read");

  TTree *tree = (TTree*)file->Get("particletree");

  tree->Process("FinalSelector.C++");

  gROOT->ProcessLine(".q");
}	
