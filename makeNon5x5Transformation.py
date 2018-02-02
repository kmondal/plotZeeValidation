import ROOT
import array, sys
from optparse import OptionParser
ROOT.gROOT.SetBatch(True)

hmcCorr = []
hmc = []
hdata = []
                
transfName   = ["transfr9EB", "transfr9EE", "transfsieieEB", "transfsieieEE", "transfsipipEB", "transfsipipEE", "transfsieipEB", "transfsieipEE"]
plotNameData = ["hdatar9EB", "hdatar9EE", "hdatasieieEB", "hdatasieieEE", "hdatasipipEB", "hdatasipipEE", "hdatasieipEB", "hdatasieipEE"]
plotNameMC   = ["hmcr9EB", "hmcr9EE", "hmcsieieEB", "hmcsieieEE", "hmcsipipEB", "hmcsipipEE", "hmcsieipEB", "hmcsieipEE"]
plotDef      = [(1000, 0., 1.), (1000, 0., 1.), (1000, 0.0, 0.02), (1000, 0.0, 0.04), (1000, 0.0, 0.03), (1000, 0.0, 0.07), (1000, -0.0001, 0.0001), (1000, -0.0005, 0.0005)]
variables = ["R9", "R9", "sieie", "sieie", "sipip", "sipip", "sieip", "sieip"]

def test(makeOutput=False):
    graphs = []
    trans = ""
    
    if (not makeOutput):
        trans = ROOT.TFile("transformation_non5x5.root")
        for t in transfName:
            graphs.append(trans.Get(t))
        
    if (len(plotNameMC) != len(plotNameData)):
        print "You need same number of plots for data and MC"
        sys.exit(1)

    for histoList, plotNames, suffix in (
        (hdata,   plotNameData, ""),
        (hmc,     plotNameMC,   ""),
        (hmcCorr, plotNameMC,   "_corr"),
        ):
        
        for z in xrange(len(plotNames)):

            title = plotNames[z]
            if title.startswith('h'):
                title = title[1:]

            histoList.append(ROOT.TH1F(plotNames[z] + suffix, title, plotDef[z][0], plotDef[z][1], plotDef[z][2]))

    filenames = [sys.argv[-2], sys.argv[-1]]
    for nf, f in enumerate(filenames):

        print "processing",f

        fin = ROOT.TFile(f)
        t = fin.Get("diphotonDumper/trees/zeevalidation_13TeV_All")

        et1 = array.array('f', [0])
        eta1 = array.array('f', [0])
        r91 = array.array('f', [0])
        sieie1 = array.array('f', [0])
        sipip1 = array.array('f', [0])
        sieip1 = array.array('f', [0])
        et2 = array.array('f', [0])
        eta2 = array.array('f', [0])
        r92 = array.array('f', [0])
        sieie2 = array.array('f', [0])
        sipip2 = array.array('f', [0])
        sieip2 = array.array('f', [0])
        weight = array.array('f', [0])
        mass = array.array('f', [0])

        t.SetBranchStatus("*", 0)
        t.SetBranchStatus("leadPt", 1)
        t.SetBranchStatus("leadEta", 1)
        t.SetBranchStatus("leadoldr9", 1)
        t.SetBranchStatus("leadoldsieie", 1)
        t.SetBranchStatus("leadoldcovipip", 1)
        t.SetBranchStatus("leadoldcovieip", 1)
        t.SetBranchStatus("subleadPt", 1)
        t.SetBranchStatus("subleadEta", 1)
        t.SetBranchStatus("subleadoldr9", 1)
        t.SetBranchStatus("subleadoldsieie", 1)
        t.SetBranchStatus("subleadoldcovipip", 1)
        t.SetBranchStatus("subleadoldcovieip", 1)
        t.SetBranchStatus("weight", 1)
        t.SetBranchStatus("mass", 1)



        t.SetBranchAddress("leadPt", et1)
        t.SetBranchAddress("leadEta", eta1)
        t.SetBranchAddress("leadoldr9", r91)
        t.SetBranchAddress("leadoldsieie", sieie1)
        t.SetBranchAddress("leadoldcovipip", sipip1)
        t.SetBranchAddress("leadoldcovieip", sieip1)

        t.SetBranchAddress("subleadPt", et2)
        t.SetBranchAddress("subleadEta", eta2)
        t.SetBranchAddress("subleadoldr9", r92)
        t.SetBranchAddress("subleadoldsieie", sieie2)
        t.SetBranchAddress("subleadoldcovipip", sipip2)
        t.SetBranchAddress("subleadoldcovieip", sieip2)

        t.SetBranchAddress("weight", weight)
        t.SetBranchAddress("mass", mass)

        entries = t.GetEntries()
#        entries = 10000

        for z in xrange(entries):
            if (z+1) % 5000 == 0:
               print "processing entry %d/%d (%5.1f%%)\r" % (z + 1, entries, (z+1) / float(entries) * 100.),
               sys.stdout.flush()

            t.GetEntry(z)
            
            if (mass[0] < 70 or mass[0] > 110):
                continue
                
            if (et1[0] > 30.):
                if (nf == 0):
                    if (abs(eta1[0])<1.5 and r91[0]>0.5):
                        hmc[0].Fill(r91[0], weight[0])
                        hmc[2].Fill(sieie1[0], weight[0])
                        hmc[4].Fill(sipip1[0], weight[0])
                        hmc[6].Fill(sieip1[0], weight[0])
                        if (not makeOutput):
                            hmcCorr[0].Fill(graphs[0].Eval(r91[0]), weight[0])
                            hmcCorr[2].Fill(graphs[2].Eval(sieie1[0]), weight[0])
                            hmcCorr[4].Fill(graphs[4].Eval(sipip1[0]), weight[0])
                            hmcCorr[6].Fill(graphs[6].Eval(sieip1[0]), weight[0])
                    elif (r91[0]>0.9):
                        hmc[1].Fill(r91[0], weight[0])
                        hmc[3].Fill(sieie1[0], weight[0])
                        hmc[5].Fill(sipip1[0], weight[0])
                        hmc[7].Fill(sieip1[0], weight[0])
                        if (not makeOutput):
                            hmcCorr[1].Fill(graphs[1].Eval(r91[0]), weight[0])
                            hmcCorr[3].Fill(graphs[3].Eval(sieie1[0]), weight[0])
                            hmcCorr[5].Fill(graphs[5].Eval(sipip1[0]), weight[0])
                            hmcCorr[7].Fill(graphs[7].Eval(sieip1[0]), weight[0])
                else:
                    if (abs(eta1[0])<1.5 and r91[0]>0.5):
                        hdata[0].Fill(r91[0], weight[0])
                        hdata[2].Fill(sieie1[0], weight[0])
                        hdata[4].Fill(sipip1[0], weight[0])
                        hdata[6].Fill(sieip1[0], weight[0])
                    elif (r91[0]>0.9):
                        hdata[1].Fill(r91[0], weight[0])
                        hdata[3].Fill(sieie1[0], weight[0])
                        hdata[5].Fill(sipip1[0], weight[0])
                        hdata[7].Fill(sieip1[0], weight[0])
        
            if (et2[0] > 20.):
                if (nf == 0):
                    if (abs(eta2[0])<1.5 and r92[0]>0.5):
                        hmc[0].Fill(r92[0], weight[0])
                        hmc[2].Fill(sieie2[0], weight[0])
                        hmc[4].Fill(sipip2[0], weight[0])
                        hmc[6].Fill(sieip2[0], weight[0])
                        if (not makeOutput):
                            hmcCorr[0].Fill(graphs[0].Eval(r92[0]), weight[0])
                            hmcCorr[2].Fill(graphs[2].Eval(sieie2[0]), weight[0])
                            hmcCorr[4].Fill(graphs[4].Eval(sipip2[0]), weight[0])
                            hmcCorr[6].Fill(graphs[6].Eval(sieip2[0]), weight[0])
                    elif (r92[0]>0.9):
                        hmc[1].Fill(r92[0], weight[0])
                        hmc[3].Fill(sieie2[0], weight[0])
                        hmc[5].Fill(sipip2[0], weight[0])
                        hmc[7].Fill(sieip2[0], weight[0])
                        if (not makeOutput):
                            hmcCorr[1].Fill(graphs[1].Eval(r92[0]), weight[0])
                            hmcCorr[3].Fill(graphs[3].Eval(sieie2[0]), weight[0])
                            hmcCorr[5].Fill(graphs[5].Eval(sipip2[0]), weight[0])
                            hmcCorr[7].Fill(graphs[7].Eval(sieip2[0]), weight[0])
                else:
                    if (abs(eta2[0])<1.5 and r92[0]>0.5):
                        hdata[0].Fill(r92[0], weight[0])
                        hdata[2].Fill(sieie2[0], weight[0])
                        hdata[4].Fill(sipip2[0], weight[0])
                        hdata[6].Fill(sieip2[0], weight[0])
                    elif (r92[0]>0.9):
                        hdata[1].Fill(r92[0], weight[0])
                        hdata[3].Fill(sieie2[0], weight[0])
                        hdata[5].Fill(sipip2[0], weight[0])
                        hdata[7].Fill(sieip2[0], weight[0])

        print                
        # end of loop over tree entries
    # end of loop over input files

                
    if (not makeOutput):
        c = []
        leg = []
        for i in xrange(len(hmc)):
            c.append(ROOT.TCanvas("c"+str(i), ""))
            c[-1].cd()

            ROOT.gStyle.SetOptStat(0)
            hmc[i].Scale(hdata[i].Integral()/hmc[i].Integral())
            hmcCorr[i].Scale(hdata[i].Integral()/hmcCorr[i].Integral())
            hmc[i].SetTitle("")
            hmc[i].Draw("HIST")
            hmc[i].SetMinimum(0.0)
            hmc[i].SetLineColor(ROOT.kRed)
            hmc[i].SetLineWidth(2)
            hmc[i].GetXaxis().SetTitle(variables[i])
            hmc[i].GetYaxis().SetTitle("Events")
            hmcCorr[i].Draw("SAMEHIST")
            hmcCorr[i].SetLineColor(ROOT.kBlue)
            hmcCorr[i].SetLineWidth(2)
            hdata[i].Draw("SAMEPE")
            hdata[i].SetMarkerStyle(20)
            hdata[i].SetMarkerSize(0.7)
            hdata[i].SetMarkerColor(ROOT.kBlack)
            
            leg.append(ROOT.TLegend(.18,.65,.48, .86))
            if (i==0 or i==2 or i==4 or i==6):
                leg[-1].SetHeader("|eta|<1.479")
            else:
                leg[-1].SetHeader("|eta|>1.556")

            leg[-1].SetName("leg"+str(i))
            leg[-1].SetBorderSize(0)
            leg[-1].AddEntry(hdata[i],"Data", "PE")
            leg[-1].AddEntry(hmc[i],"UnCorr MC", "L")
            leg[-1].AddEntry(hmcCorr[i],"Corr MC", "L")
            leg[-1].Draw("SAME")

            c[-1].Update()
            c[-1].SaveAs(plotNames[i]+".root")
            c[-1].SaveAs(plotNames[i]+".png")
            c[-1].SaveAs(plotNames[i]+".pdf")

        print "plotting done, press enter to continue"
        raw_input()
    else:
        output = ROOT.TFile("inputHistos_non5x5.root", "recreate")
        for i, h in enumerate(hmc):
            hmc[i].Scale(hdata[i].Integral()/hmc[i].Integral())
            h.Write()
        for h in hdata:
            h.Write()
        output.Close()
        print "wrote inputHistos_non5x5.root"

def makeTransformation():
    global hmc, hdata, transfName, plotNameData, plotNameMC, plotDef      

    f = ROOT.TFile("inputHistos_non5x5.root")
    for p in plotNameData:
        hdata.append(f.Get(p))

    for p in plotNameMC:              
        hmc.append(f.Get(p))

    if (len(plotNameMC) != len(plotNameData)):
        print "You need same number of plots for data and MC"
        sys.exit(1)

    graphs = []
    for z in xrange(len(hmc)):
        # NORMALIZE MC TO DATA
        hmc[z].Scale(hdata[z].Integral()/hmc[z].Integral())
        hcdfmc = hmc[z].GetCumulative()
        hcdfmc.Scale(1./hmc[z].Integral())
        
        # Make general
        uniform = ROOT.TH1F("uniform"+str(z), "", plotDef[z][0], plotDef[z][1], plotDef[z][2])
        for i in xrange(plotDef[z][0]):
            uniform.SetBinContent(i, 10) 

        uniform.Scale(hdata[z].Integral()/uniform.Integral())
        uniformcdf = uniform.GetCumulative()
        uniformcdf.Scale(1./hmc[z].Integral())

        xt = array.array('d', [x*0.001 for x in xrange(1000)])
        yt = array.array('d', [x*0.001 for x in xrange(1000)])

        hmc[z].GetQuantiles(len(yt), xt, yt)

        hcdfdata = hdata[z].GetCumulative()
        hcdfdata.Scale(1./hdata[z].Integral())

        xdatat = array.array('d', [x*0.001 for x in xrange(1000)])
        ydatat = array.array('d', [x*0.001 for x in xrange(1000)])
        hdata[z].GetQuantiles(len(ydatat), xdatat, ydatat)

        graphs.append(ROOT.TGraph(len(xt), xt, xdatat))
        graphs[-1].SetName(transfName[z])

    out = ROOT.TFile("transformation_non5x5.root", "recreate")
    for g in graphs:
        g.Write()
    out.Close()
    print "wrote transformation_non5x5.root"

if (__name__ == "__main__"):
    parser = OptionParser(usage="Usage: %prog [options] [mc_ntuple_filename] [target_ntuple_filename]",)
    parser.add_option("-p", "--prepare-plots", dest="preparePlots", action="store_true", help="Dump plots", default=False)
    parser.add_option("-c", "--transform", action="store_true", help="Derive actual transformations", default=False)
    parser.add_option("-t", "--test", action="store_true", help="Test transformations", default=False)
    # parser.add_option(

    (options, arg) = parser.parse_args()

    if (options.preparePlots):
        print "Preparing necessary plots..."
        test(True)   
        print "Done."
        sys.exit(0)
    elif (options.transform): 
        print "Deriving transformations..."
        makeTransformation()
        print "Done."
        sys.exit(0)
    elif (options.test): 
        print "Testing..."
        test(False)
        print "Done."
        sys.exit(0)
