from ROOT import TFile, TGraph, TH2D, TH1D, TCanvas, TGraphErrors

inf = TFile.Open("out.root","read")
assert inf.IsOpen(), "inf Failed"

canvas = TCanvas('canvas','shouldnotseethis',0,0,1280,720)

histo = inf.Get("RingVThetaCOM_s00_d1_pid")
graph = inf.Get("AD_d1_s0_pid")

for ring in range(0,graph.GetN()):
  ringHisto = histo.ProjectionX("Ring {}".format(ring),ring,ring+1)

  height = ringHisto.GetMaximum()/2

  #ringHisto->Write()
  ringHisto.Draw()

  #graph.Draw("same*")

  point = TGraphErrors(1);
  point.SetPoint(1,graph.GetX()[ring],height);
  point.SetPointError(1,graph.GetEX()[ring],0);

  point.Draw('same*')

  canvas.SaveAs('Ring {}.png'.format(ring))

#outf = TFile.Open("compare.root","recreate")
#assert outf.IsOpen(), "outf Failed"

