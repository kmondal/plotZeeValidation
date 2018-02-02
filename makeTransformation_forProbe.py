import ROOT
import array, sys
from optparse import OptionParser
ROOT.gROOT.SetBatch(True)

hmcCorr = []
hmc = []
hdata = []
                
transfName   = ["transfEtaWidthEB", "transfEtaWidthEE", "transfS4EB", "transfS4EE", "transffull5x5R9EB", "transffull5x5R9EE","transffull5x5sieieEB","transffull5x5sieieEE"]
plotNameData = ["hetaWidthdata_EB", "hetaWidthdata_EE", "hs4data_EB", "hs4data_EE", "hfull5x5r9data_EB", "hfull5x5r9data_EE","hfull5x5sieiedata_EB","hfull5x5sieiedata_EE"]
plotNameMC   = ["hetaWidthmc_EB", "hetaWidthmc_EE", "hs4mc_EB", "hs4mc_EE", "hfull5x5r9mc_EB", "hfull5x5r9mc_EE","hfull5x5sieiemc_EB","hfull5x5sieiemc_EE"]
#plotDef      = [(10000, 0.00, 0.02), (10000, 0.00, 0.05), (10000, 0.0, 1.0), (10000, 0.0, 1.0), (10000, 0.0, 1.1), (10000, 0.0, 1.1),(10000, 0.000, 0.015), (10000, 0.015, 0.040)]
plotDef      = [(10100, 0.0, 1.01), (10100, 0.0, 1.01), (10100, 0.0, 1.01), (10100, 0.0, 1.01), (10100, 0.0, 1.01), (10100, 0.0, 1.01),(10100, 0.0, 1.01), (10100, 0.0, 1.01)]
variables = ["eta width", "eta width", "S4 ratio", "S4 ratio", "full5x5 R9", "full5x5 R9", "full5x5 sieie", "full5x5 sieie"]

def test(makeOutput=False):
    graphs = []
    trans = ""
    
    if (not makeOutput):
        trans = ROOT.TFile("transformation_forProbe.root")
        print "trans file", trans
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
        t = fin.Get("PhotonToRECO/fitter_tree")

        et1 = array.array('f', [0])
        eta1 = array.array('f', [0])

        et2 = array.array('f', [0])
        eta2 = array.array('f', [0])
        s42 = array.array('f', [0])
        etawidth2 = array.array('f', [0])
        full5x5r92 = array.array('f', [0])
        full5x5sieie2 = array.array('f', [0])

        weight = array.array('f', [0])
        mass = array.array('f', [0])
        presel = array.array('i', [0])

        t.SetBranchStatus("*", 0)
        # Probe branches
        t.SetBranchStatus("probe_Pho_pt", 1)
        t.SetBranchStatus("probe_Pho_eta", 1)        
        t.SetBranchStatus("probe_Pho_s4", 1)
        t.SetBranchStatus("probe_Pho_etawidth", 1)
        t.SetBranchStatus("probe_Pho_r9", 1)
        t.SetBranchStatus("probe_Pho_sieie", 1)
        # Tag branches
        t.SetBranchStatus("tag_Pho_pt", 1)
        t.SetBranchStatus("tag_Pho_eta", 1)
        # Common branches
        if (nf == 0):
            print "process MC"
            t.SetBranchStatus("totWeight", 1)
        
        t.SetBranchStatus("mass", 1)
        t.SetBranchStatus("passingPresel", 1)

        #Tag variables
        t.SetBranchAddress("tag_Pho_pt", et1)
        t.SetBranchAddress("tag_Pho_eta", eta1)
        # Probe variables
        t.SetBranchAddress("probe_Pho_pt", et2)
        t.SetBranchAddress("probe_Pho_eta", eta2)
        t.SetBranchAddress("probe_Pho_s4", s42)
        t.SetBranchAddress("probe_Pho_etawidth", etawidth2)
        t.SetBranchAddress("probe_Pho_r9", full5x5r92)
        t.SetBranchAddress("probe_Pho_sieie", full5x5sieie2)
        # Common variables
        if (nf == 0):
            print "process MC"
            t.SetBranchAddress("totWeight", weight)
            
        t.SetBranchAddress("mass", mass)
        t.SetBranchAddress("passingPresel", presel)


        entries = t.GetEntries()
        for z in xrange(entries):
            if (z+1) % 10000 == 0:
               print "processing entry %d/%d (%5.1f%%)\r" % (z + 1, entries, (z+1) / float(entries) * 100.),
               sys.stdout.flush()

            t.GetEntry(z)
            
            if (mass[0] < 70 or mass[0] > 110):
                continue

            if (presel[0] == 1):
                if (nf == 0):
                    if (abs(eta2[0])<1.5):
                        hmc[4].Fill(full5x5r92[0], weight[0])
                        if (full5x5r92[0]>0.5):
                            hmc[0].Fill(etawidth2[0], weight[0])
                            hmc[2].Fill(s42[0], weight[0])
                            hmc[6].Fill(full5x5sieie2[0], weight[0])
                        if (not makeOutput):
                            hmcCorr[4].Fill(graphs[4].Eval(full5x5r92[0]), weight[0])
                            if (full5x5r92[0]>0.5):
                                hmcCorr[0].Fill(graphs[0].Eval(etawidth2[0]), weight[0])
                                hmcCorr[2].Fill(graphs[2].Eval(s42[0]), weight[0])
                                hmcCorr[6].Fill(graphs[6].Eval(full5x5sieie2[0]), weight[0])
                    else:
                        hmc[5].Fill(full5x5r92[0], weight[0])
                        if (full5x5r92[0]>0.8):
                            hmc[1].Fill(etawidth2[0], weight[0])
                            hmc[3].Fill(s42[0], weight[0])
                            hmc[7].Fill(full5x5sieie2[0], weight[0])
                        if (not makeOutput):
                            hmcCorr[5].Fill(graphs[5].Eval(full5x5r92[0]), weight[0])
                            if (full5x5r92[0]>0.8):
                                hmcCorr[1].Fill(graphs[1].Eval(etawidth2[0]), weight[0])
                                hmcCorr[3].Fill(graphs[3].Eval(s42[0]), weight[0])
                                hmcCorr[7].Fill(graphs[7].Eval(full5x5sieie2[0]), weight[0])
                else:
                    if (abs(eta2[0])<1.5):
                        hdata[4].Fill(full5x5r92[0])
                        if (full5x5r92[0]>0.5):
                            hdata[0].Fill(etawidth2[0])
                            hdata[2].Fill(s42[0])
                            hdata[6].Fill(full5x5sieie2[0])
                    else:
                        hdata[5].Fill(full5x5r92[0])
                        if (full5x5r92[0]>0.8):
                            hdata[1].Fill(etawidth2[0])
                            hdata[3].Fill(s42[0])
                            hdata[7].Fill(full5x5sieie2[0])

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
        output = ROOT.TFile("inputHistos_forProbe.root", "recreate")
        for i, h in enumerate(hmc):
            hmc[i].Scale(hdata[i].Integral()/hmc[i].Integral())
            h.Write()
        for h in hdata:
            h.Write()
        output.Close()
        print "wrote inputHistos_forProbe.root"

def makeTransformation():
    global hmc, hdata, transfName, plotNameData, plotNameMC, plotDef      

    f = ROOT.TFile("inputHistos_forProbe.root")
    print "read file: ", f
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

        xt = array.array('d', [x*0.0001 for x in xrange(10000)])
        yt = array.array('d', [x*0.0001 for x in xrange(10000)])

        hmc[z].GetQuantiles(len(yt), xt, yt)

        hcdfdata = hdata[z].GetCumulative()
        hcdfdata.Scale(1./hdata[z].Integral())

        xdatat = array.array('d', [x*0.0001 for x in xrange(10000)])
        ydatat = array.array('d', [x*0.0001 for x in xrange(10000)])
        hdata[z].GetQuantiles(len(ydatat), xdatat, ydatat)

        graphs.append(ROOT.TGraph(len(xt), xt, xdatat))
        graphs[-1].SetName(transfName[z])

    out = ROOT.TFile("transformation_forProbe.root", "recreate")
    for g in graphs:
        g.Write()
    out.Close()
    print "wrote transformation_forProbe.root"

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
