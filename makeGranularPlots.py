import ROOT
import array, sys
from optparse import OptionParser

hmcCorr = []
hmc = []
hdata = []
                
transfName   = ["transfEtaWidthEB", "transfEtaWidthEE", "transfS4EB", "transfS4EE", "transffull5x5R9EB", "transffull5x5R9EE","transffull5x5sieieEB","transffull5x5sieieEE"]
plotNameData = ["hetaWidthdata_eta1", "hs4data_eta1", "hfull5x5r9data_eta1", "hfull5x5sieiedata_eta1", "hetaWidthdata_eta2", "hs4data_eta2", "hfull5x5r9data_eta2", "hfull5x5sieiedata_eta2", "hetaWidthdata_eta3", "hs4data_eta3", "hfull5x5r9data_eta3", "hfull5x5sieiedata_eta3", "hetaWidthdata_eta4", "hs4data_eta4", "hfull5x5r9data_eta4", "hfull5x5sieiedata_eta4", "hetaWidthdata_eta5", "hs4data_eta5", "hfull5x5r9data_eta5", "hfull5x5sieiedata_eta5"]
plotNameMC   = ["hetaWidthmc_eta1", "hs4mc_eta1", "hfull5x5r9mc_eta1", "hfull5x5sieiemc_eta1", "hetaWidthmc_eta2", "hs4mc_eta2", "hfull5x5r9mc_eta2", "hfull5x5sieiemc_eta2", "hetaWidthmc_eta3", "hs4mc_eta3", "hfull5x5r9mc_eta3", "hfull5x5sieiemc_eta3", "hetaWidthmc_eta4", "hs4mc_eta4", "hfull5x5r9mc_eta4", "hfull5x5sieiemc_eta4", "hetaWidthmc_eta5", "hs4mc_eta5", "hfull5x5r9mc_eta5", "hfull5x5sieiemc_eta5"]
plotDef      = [(1000, 0.0, 0.02), (1000, 0.3, 1.), (1000, 0.5, 1.), (1000, 0.0, 0.014), (1000, 0.0, 0.02), (1000, 0.3, 1.), (1000, 0.5, 1.), (1000, 0.0, 0.014), (1000, 0.0, 0.02), (1000, 0.3, 1.), (1000, 0.5, 1.), (1000, 0.0, 0.014), (1000, 0.0, 0.05), (1000, 0.3, 1.), (1000, 0.8, 1.), (1000, 0.015, 0.04), (1000, 0.0, 0.05), (1000, 0.3, 1.), (1000, 0.8, 1.), (1000, 0.015, 0.04)]
variables = ["Eta_width", "S4_ratio", "full5x5_R9", "full5x5_sieie", "Eta_width", "S4_ratio", "full5x5_R9", "full5x5_sieie", "Eta_width", "S4_ratio", "full5x5_R9", "full5x5_sieie", "Eta_width", "S4_ratio", "full5x5_R9", "full5x5_sieie", "Eta_width", "S4_ratio", "full5x5_R9", "full5x5_sieie"]
def test(makeOutput=False):
    graphs = []
    trans = ""
    
    if (not makeOutput):
        trans = ROOT.TFile("transformation.root")
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
        s41 = array.array('f', [0])
        etawidth1 = array.array('f', [0])
        full5x5r91 = array.array('f', [0])
        full5x5sieie1 = array.array('f', [0])
        et2 = array.array('f', [0])
        eta2 = array.array('f', [0])
        s42 = array.array('f', [0])
        etawidth2 = array.array('f', [0])
        full5x5r92 = array.array('f', [0])
        full5x5sieie2 = array.array('f', [0])
        weight = array.array('f', [0])
        mass = array.array('f', [0])

        t.SetBranchStatus("*", 0)
        t.SetBranchStatus("leadPt", 1)
        t.SetBranchStatus("leadEta", 1)
        t.SetBranchStatus("leads4ratio", 1)
        t.SetBranchStatus("leadetawidth", 1)
        t.SetBranchStatus("leadfull5x5r9", 1)
        t.SetBranchStatus("leadfull5x5sieie", 1)
        t.SetBranchStatus("subleadPt", 1)
        t.SetBranchStatus("subleadEta", 1)
        t.SetBranchStatus("subleads4ratio", 1)
        t.SetBranchStatus("subleadetawidth", 1)
        t.SetBranchStatus("subleadfull5x5r9", 1)
        t.SetBranchStatus("subleadfull5x5sieie", 1)
        t.SetBranchStatus("weight", 1)
        t.SetBranchStatus("mass", 1)

        t.SetBranchAddress("leadPt", et1)
        t.SetBranchAddress("leadEta", eta1)
        t.SetBranchAddress("leads4ratio", s41)
        t.SetBranchAddress("leadetawidth", etawidth1)
        t.SetBranchAddress("leadfull5x5r9", full5x5r91)
        t.SetBranchAddress("leadfull5x5sieie", full5x5sieie1)
        t.SetBranchAddress("subleadPt", et2)
        t.SetBranchAddress("subleadEta", eta2)
        t.SetBranchAddress("subleads4ratio", s42)
        t.SetBranchAddress("subleadetawidth", etawidth2)
        t.SetBranchAddress("subleadfull5x5r9", full5x5r92)
        t.SetBranchAddress("subleadfull5x5sieie", full5x5sieie2)
        t.SetBranchAddress("weight", weight)
        t.SetBranchAddress("mass", mass)

        entries = t.GetEntries()
#        entries = 1000

        for z in xrange(entries):
            if (z+1) % 5000 == 0:
               print "processing entry %d/%d (%5.1f%%)\r" % (z + 1, entries, (z+1) / float(entries) * 100.),
               sys.stdout.flush()

            t.GetEntry(z)
            
            if (mass[0] < 70 or mass[0] > 110):
                continue
                
            if (et1[0] > 30.):
                if (nf == 0):
                    if (abs(eta1[0])<=0.5 and full5x5r91[0]>0.5):
                        hmc[0].Fill(etawidth1[0], weight[0])
                        hmc[1].Fill(s41[0], weight[0])
                        hmc[2].Fill(full5x5r91[0], weight[0])
                        hmc[3].Fill(full5x5sieie1[0], weight[0])
                        if (not makeOutput):
                            hmcCorr[0].Fill(graphs[0].Eval(etawidth1[0]), weight[0])
                            hmcCorr[1].Fill(graphs[2].Eval(s41[0]), weight[0])
                            hmcCorr[2].Fill(graphs[4].Eval(full5x5r91[0]), weight[0])
                            hmcCorr[3].Fill(graphs[6].Eval(full5x5sieie1[0]), weight[0])
                    elif (abs(eta1[0])>0.5 and abs(eta1[0])<=1.0 and full5x5r91[0]>0.5):
                        hmc[4].Fill(etawidth1[0], weight[0])
                        hmc[5].Fill(s41[0], weight[0])
                        hmc[6].Fill(full5x5r91[0], weight[0])
                        hmc[7].Fill(full5x5sieie1[0], weight[0])
                        if (not makeOutput):
                            hmcCorr[4].Fill(graphs[0].Eval(etawidth1[0]), weight[0])
                            hmcCorr[5].Fill(graphs[2].Eval(s41[0]), weight[0])
                            hmcCorr[6].Fill(graphs[4].Eval(full5x5r91[0]), weight[0])
                            hmcCorr[7].Fill(graphs[6].Eval(full5x5sieie1[0]), weight[0])
                    elif (abs(eta1[0])>1.0 and abs(eta1[0])<1.5 and full5x5r91[0]>0.5):
                        hmc[8].Fill(etawidth1[0], weight[0])
                        hmc[9].Fill(s41[0], weight[0])
                        hmc[10].Fill(full5x5r91[0], weight[0])
                        hmc[11].Fill(full5x5sieie1[0], weight[0])
                        if (not makeOutput):
                            hmcCorr[8].Fill(graphs[0].Eval(etawidth1[0]), weight[0])
                            hmcCorr[9].Fill(graphs[2].Eval(s41[0]), weight[0])
                            hmcCorr[10].Fill(graphs[4].Eval(full5x5r91[0]), weight[0])
                            hmcCorr[11].Fill(graphs[6].Eval(full5x5sieie1[0]), weight[0])
                    elif (abs(eta1[0])>1.5 and abs(eta1[0])<=2.0 and full5x5r91[0]>0.8):
                        hmc[12].Fill(etawidth1[0], weight[0])
                        hmc[13].Fill(s41[0], weight[0])
                        hmc[14].Fill(full5x5r91[0], weight[0])
                        hmc[15].Fill(full5x5sieie1[0], weight[0])
                        if (not makeOutput):
                            hmcCorr[12].Fill(graphs[1].Eval(etawidth1[0]), weight[0])
                            hmcCorr[13].Fill(graphs[3].Eval(s41[0]), weight[0])
                            hmcCorr[14].Fill(graphs[5].Eval(full5x5r91[0]), weight[0])
                            hmcCorr[15].Fill(graphs[7].Eval(full5x5sieie1[0]), weight[0])
                    elif (abs(eta1[0])>2.0 and abs(eta1[0])<=2.5 and full5x5r91[0]>0.8):
                        hmc[16].Fill(etawidth1[0], weight[0])
                        hmc[17].Fill(s41[0], weight[0])
                        hmc[18].Fill(full5x5r91[0], weight[0])
                        hmc[19].Fill(full5x5sieie1[0], weight[0])
                        if (not makeOutput):
                            hmcCorr[16].Fill(graphs[1].Eval(etawidth1[0]), weight[0])
                            hmcCorr[17].Fill(graphs[3].Eval(s41[0]), weight[0])
                            hmcCorr[18].Fill(graphs[5].Eval(full5x5r91[0]), weight[0])
                            hmcCorr[19].Fill(graphs[7].Eval(full5x5sieie1[0]), weight[0])
                else:
                    if (abs(eta1[0])<=0.5 and full5x5r91[0]>0.5):
                        hdata[0].Fill(etawidth1[0], weight[0])
                        hdata[1].Fill(s41[0], weight[0])
                        hdata[2].Fill(full5x5r91[0], weight[0])
                        hdata[3].Fill(full5x5sieie1[0], weight[0])
                    elif (abs(eta1[0])>0.5 and abs(eta1[0])<=1.0 and full5x5r91[0]>0.5):
                        hdata[4].Fill(etawidth1[0], weight[0])
                        hdata[5].Fill(s41[0], weight[0])
                        hdata[6].Fill(full5x5r91[0], weight[0])
                        hdata[7].Fill(full5x5sieie1[0], weight[0])
                    elif (abs(eta1[0])>1.0 and abs(eta1[0])<1.5 and full5x5r91[0]>0.5):
                        hdata[8].Fill(etawidth1[0], weight[0])
                        hdata[9].Fill(s41[0], weight[0])
                        hdata[10].Fill(full5x5r91[0], weight[0])
                        hdata[11].Fill(full5x5sieie1[0], weight[0])
                    elif (abs(eta1[0])>1.5 and abs(eta1[0])<=2.0 and full5x5r91[0]>0.8):
                        hdata[12].Fill(etawidth1[0], weight[0])
                        hdata[13].Fill(s41[0], weight[0])
                        hdata[14].Fill(full5x5r91[0], weight[0])
                        hdata[15].Fill(full5x5sieie1[0], weight[0])
                    elif (abs(eta1[0])>2.0 and abs(eta1[0])<=2.5 and full5x5r91[0]>0.8):
                        hdata[16].Fill(etawidth1[0], weight[0])
                        hdata[17].Fill(s41[0], weight[0])
                        hdata[18].Fill(full5x5r91[0], weight[0])
                        hdata[19].Fill(full5x5sieie1[0], weight[0])
        
            if (et2[0] > 20.):
                if (nf == 0):
                    if (abs(eta2[0])<=0.5 and full5x5r92[0]>0.5):
                        hmc[0].Fill(etawidth2[0], weight[0])
                        hmc[1].Fill(s42[0], weight[0])
                        hmc[2].Fill(full5x5r92[0], weight[0])
                        hmc[3].Fill(full5x5sieie2[0], weight[0])
                        if (not makeOutput):
                            hmcCorr[0].Fill(graphs[0].Eval(etawidth2[0]), weight[0])
                            hmcCorr[1].Fill(graphs[2].Eval(s42[0]), weight[0])
                            hmcCorr[2].Fill(graphs[4].Eval(full5x5r92[0]), weight[0])
                            hmcCorr[3].Fill(graphs[6].Eval(full5x5sieie2[0]), weight[0])
                    elif (abs(eta2[0])>0.5 and abs(eta2[0])<=1.0 and full5x5r92[0]>0.5):
                        hmc[4].Fill(etawidth2[0], weight[0])
                        hmc[5].Fill(s42[0], weight[0])
                        hmc[6].Fill(full5x5r92[0], weight[0])
                        hmc[7].Fill(full5x5sieie2[0], weight[0])
                        if (not makeOutput):
                            hmcCorr[4].Fill(graphs[0].Eval(etawidth2[0]), weight[0])
                            hmcCorr[5].Fill(graphs[2].Eval(s42[0]), weight[0])
                            hmcCorr[6].Fill(graphs[4].Eval(full5x5r92[0]), weight[0])
                            hmcCorr[7].Fill(graphs[6].Eval(full5x5sieie2[0]), weight[0])
                    elif (abs(eta2[0])>1.0 and abs(eta2[0])<1.5 and full5x5r92[0]>0.5):
                        hmc[8].Fill(etawidth2[0], weight[0])
                        hmc[9].Fill(s42[0], weight[0])
                        hmc[10].Fill(full5x5r92[0], weight[0])
                        hmc[11].Fill(full5x5sieie2[0], weight[0])
                        if (not makeOutput):
                            hmcCorr[8].Fill(graphs[0].Eval(etawidth2[0]), weight[0])
                            hmcCorr[9].Fill(graphs[2].Eval(s42[0]), weight[0])
                            hmcCorr[10].Fill(graphs[4].Eval(full5x5r92[0]), weight[0])
                            hmcCorr[11].Fill(graphs[6].Eval(full5x5sieie2[0]), weight[0])
                    elif (abs(eta2[0])>1.5 and abs(eta2[0])<=2.0 and full5x5r92[0]>0.8):
                        hmc[12].Fill(etawidth2[0], weight[0])
                        hmc[13].Fill(s42[0], weight[0])
                        hmc[14].Fill(full5x5r92[0], weight[0])
                        hmc[15].Fill(full5x5sieie2[0], weight[0])
                        if (not makeOutput):
                            hmcCorr[12].Fill(graphs[1].Eval(etawidth2[0]), weight[0])
                            hmcCorr[13].Fill(graphs[3].Eval(s42[0]), weight[0])
                            hmcCorr[14].Fill(graphs[5].Eval(full5x5r92[0]), weight[0])
                            hmcCorr[15].Fill(graphs[7].Eval(full5x5sieie2[0]), weight[0])
                    elif (abs(eta2[0])>2.0 and abs(eta2[0])<=2.5 and full5x5r92[0]>0.8):
                        hmc[16].Fill(etawidth2[0], weight[0])
                        hmc[17].Fill(s42[0], weight[0])
                        hmc[18].Fill(full5x5r92[0], weight[0])
                        hmc[19].Fill(full5x5sieie2[0], weight[0])
                        if (not makeOutput):
                            hmcCorr[16].Fill(graphs[1].Eval(etawidth2[0]), weight[0])
                            hmcCorr[17].Fill(graphs[3].Eval(s42[0]), weight[0])
                            hmcCorr[18].Fill(graphs[5].Eval(full5x5r92[0]), weight[0])
                            hmcCorr[19].Fill(graphs[7].Eval(full5x5sieie2[0]), weight[0])
                else:
                    if (abs(eta2[0])<=0.5 and full5x5r92[0]>0.5):
                        hdata[0].Fill(etawidth2[0], weight[0])
                        hdata[1].Fill(s42[0], weight[0])
                        hdata[2].Fill(full5x5r92[0], weight[0])
                        hdata[3].Fill(full5x5sieie2[0], weight[0])
                    elif (abs(eta2[0])>0.5 and abs(eta2[0])<=1.0 and full5x5r92[0]>0.5):
                        hdata[4].Fill(etawidth2[0], weight[0])
                        hdata[5].Fill(s42[0], weight[0])
                        hdata[6].Fill(full5x5r92[0], weight[0])
                        hdata[7].Fill(full5x5sieie2[0], weight[0])
                    elif (abs(eta2[0])>1.0 and abs(eta2[0])<1.5 and full5x5r92[0]>0.5):
                        hdata[8].Fill(etawidth2[0], weight[0])
                        hdata[9].Fill(s42[0], weight[0])
                        hdata[10].Fill(full5x5r92[0], weight[0])
                        hdata[11].Fill(full5x5sieie2[0], weight[0])
                    elif (abs(eta2[0])>1.5 and abs(eta2[0])<=2.0 and full5x5r92[0]>0.8):
                        hdata[12].Fill(etawidth2[0], weight[0])
                        hdata[13].Fill(s42[0], weight[0])
                        hdata[14].Fill(full5x5r92[0], weight[0])
                        hdata[15].Fill(full5x5sieie2[0], weight[0])
                    elif (abs(eta2[0])>2.0 and abs(eta2[0])<=2.5 and full5x5r92[0]>0.8):
                        hdata[16].Fill(etawidth2[0], weight[0])
                        hdata[17].Fill(s42[0], weight[0])
                        hdata[18].Fill(full5x5r92[0], weight[0])
                        hdata[19].Fill(full5x5sieie2[0], weight[0])

        print                
        # end of loop over tree entries
    # end of loop over input files

                
    if (not makeOutput):
        c = []
        pad1 = []
        pad2 = []
        ratio1 = []
        ratio2 = []
        leg = []
        for i in xrange(len(hmc)):
            c.append(ROOT.TCanvas("c"+str(i), ""))
            c[-1].cd()

            ROOT.gStyle.SetOptStat(0)
#            pad1.append(ROOT.TPad("pad1"+str(i), "", 0., 0.25, 1., 1.))
#            pad1[-1].Draw()
#            pad1[-1].cd()

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
            
            if (i==0 or i==4 or i==8 or i==12 or i==15 or i==16 or i==19):
                leg.append(ROOT.TLegend(.58,.65,.88, .86))
            else:
                leg.append(ROOT.TLegend(.18,.65,.48, .86))
                
            leg[-1].SetName("leg"+str(i))
            leg[-1].SetBorderSize(0)
            if (i<=3):
                leg[-1].SetHeader("0.0<=|eta|<=0.5")
            elif (i>3 and i<=7):
                leg[-1].SetHeader("0.5<|eta|<=1.0")
            elif (i>7 and i<=11):
                leg[-1].SetHeader("1.0<|eta|<1.5")
            elif (i>11 and i<=15):
                leg[-1].SetHeader("1.5<|eta|<=2.0")
            elif (i>15 and i<=19):
                leg[-1].SetHeader("2.0<|eta|<=2.5")
                
            leg[-1].AddEntry(hdata[i],"Data", "PE")
            leg[-1].AddEntry(hmc[i],"UnCorr MC", "L")
            leg[-1].AddEntry(hmcCorr[i],"Corr MC", "L")
            leg[-1].Draw("SAME")
#            pad1[-1].Update()

#            c[-1].cd()
#            pad2.append(ROOT.TPad("pad2"+str(i), "", 0., 0.0, 1., 0.25))
#            pad2[-1].SetTopMargin(.05)
#            pad2[-1].SetBottomMargin(0.30)
#            pad2[-1].Draw()
#            pad2[-1].cd()
#
#            ratio1.append(hdata[i].Clone())
##            ratio1[-1].Sumw2(1)
#            ratio1[-1].Divide(hmc[i])
#            ratio1[-1].SetMarkerStyle(20)
#            ratio1[-1].SetMarkerSize(0.5)
#            ratio1[-1].SetMarkerColor(ROOT.kRed)
#            ratio1[-1].SetTitle("")
#            ratio1[-1].GetYaxis().SetRangeUser(0.5, 1.5)
#            ratio1[-1].GetYaxis().SetLabelSize(0.06)
#            ratio1[-1].GetYaxis().SetTitleSize(0.1)
#            ratio1[-1].GetYaxis().SetTitleOffset(0.45)
#            ratio1[-1].GetYaxis().SetTitle("Data/MC")
#            ratio1[-1].GetXaxis().SetTitle(variables[i])
#            ratio1[-1].GetXaxis().SetLabelSize(0.1)
#            ratio1[-1].GetXaxis().SetTitleSize(0.1)
#            ratio1[-1].GetXaxis().SetTitleOffset(1.2)
#
#            ratio2.append(hdata[i].Clone())
##            ratio2[-1].Sumw2(1)
#            ratio2[-1].Divide(hmcCorr[i])
#            ratio2[-1].SetMarkerStyle(20)
#            ratio2[-1].SetMarkerSize(0.5)
#            ratio2[-1].SetMarkerColor(ROOT.kBlue)
#            ratio2[-1].SetTitle("")
#
#            ratio1[-1].Draw("hist p")
#            ratio2[-1].Draw("hist psame")
#            pad2[-1].Update()
            c[-1].Update()

            c[-1].SaveAs(plotNames[i]+".root")
            c[-1].SaveAs(plotNames[i]+".png")
            c[-1].SaveAs(plotNames[i]+".pdf")

        print "plotting done, press enter to continue"
        raw_input()
    else:
        output = ROOT.TFile("inputHistos.root", "recreate")
        for i, h in enumerate(hmc):
            hmc[i].Scale(hdata[i].Integral()/hmc[i].Integral())
            h.Write()
        for h in hdata:
            h.Write()
        output.Close()
        print "wrote inputHistos.root"

def makeTransformation():
    global hmc, hdata, transfName, plotNameData, plotNameMC, plotDef      

    f = ROOT.TFile("inputHistos.root")
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

    out = ROOT.TFile("transformation.root", "recreate")
    for g in graphs:
        g.Write()
    out.Close()
    print "wrote transformation.root"

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
