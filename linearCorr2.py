import ROOT
import sys

# first EB second value EE
hNominal = []
hUp = []
hDown = []
xmin = (-0.9, -0.9)
yminup = (1.7, 1.7)
ymindown = (0.5, 0.5)
xmax = (0.8, 0.8)
ymaxup = (1.0, 1.0)
ymaxdown = (1.0, 1.0)

def computeSlope1(x, b):
    m = (ymaxup[b]-yminup[b])/(xmax[b]-xmin[b])
    y = m*(x-xmin[b]) + yminup[b]
    return y

def computeSlope2(x, b):
    m = (ymaxdown[b]-ymindown[b])/(xmax[b]-xmin[b])
    y = m*(x-xmin[b]) + ymindown[b]
    return y

#def computeSlope2(x, b):
#    m = (ymax[b]-ymin[b])/(xmax[b]-xmin[b])
#    y = m*(x-xmin[b]) + ymin[b]
#    return y

f = ROOT.TFile(sys.argv[1])
hNominal.append(f.Get("idmva2_cat0_DYToEE"))
hNominal.append(f.Get("idmva2_cat1_DYToEE"))
hDown.append(f.Get("idmvadown2_cat0_DYToEE"))
hDown.append(f.Get("idmvadown2_cat1_DYToEE"))
hUp.append(f.Get("idmvaup2_cat0_DYToEE"))
hUp.append(f.Get("idmvaup2_cat1_DYToEE"))

for b in xrange(len(hNominal)):
    x0 = hNominal[b].FindBin(xmin[b])
    x1 = hNominal[b].FindBin(xmax[b])
    for i in xrange(x0, x1):
        val = abs(hNominal[b].GetBinContent(i) - hUp[b].GetBinContent(i))
        k = computeSlope1(hNominal[b].GetBinLowEdge(i), b)
        hUp[b].SetBinContent(i, (hNominal[b].GetBinContent(i)+val)*k)
        print k

        val = abs(hNominal[b].GetBinContent(i) - hDown[b].GetBinContent(i))
        k = computeSlope2(hNominal[b].GetBinLowEdge(i), b)
        hDown[b].SetBinContent(i, (hNominal[b].GetBinContent(i)-val)*k)
        print k

out = ROOT.TFile("output2.root","RECREATE")
for h in hNominal:
    h.Write()
for h in hUp:
    h.Write()
for h in hDown:
    h.Write()
out.Close()




        
