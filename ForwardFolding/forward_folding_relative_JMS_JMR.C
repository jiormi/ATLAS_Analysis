void forward_folding_relative_JMS_JMR() {
    const int nEvents = 10000;
    const int nBins = 50;
    const double massMin = 0;
    const double massMax = 200;
    double prob_mass = 100;
    double prob_sigma = 8.0;

    // MC truth → reco constants
    double R_mc = 0.98;         // Average MC mass response
    double sigma_mc = 8.0;     // MC mass resolution (GeV)

    // ---- Generate truth-level MC jet mass
    std::vector<double> truth_mass;
    for (int i = 0; i < nEvents; ++i) {
        double m_truth = gRandom->Gaus(prob_mass, prob_sigma);
        truth_mass.push_back(m_truth);
    }

    // ---- Reco-level MC: smear truth
    std::vector<double> reco_mass_mc;
    for (auto m_truth : truth_mass) {
        double m_reco = gRandom->Gaus(m_truth * R_mc, sigma_mc);
        reco_mass_mc.push_back(m_reco);
    }

    // ---- Generate data: smear same truth with DIFFERENT scale and resolution
    double scale_data = 1.00;       // True JMS
    double sigma_data = 8.0;       // True JMR
    TH1D* h_data = new TH1D("h_data", "Fake Data Mass;Mass [GeV];Events", nBins, massMin, massMax);

    for (auto m_truth : truth_mass) {
        double m_data = gRandom->Gaus(prob_mass * scale_data, sigma_data);
        h_data->Fill(m_data);
    }

    // ---- Prepare chi2 scan over relative s (scale) and r (resolution)
    const int nSteps = 100;
    TH2D* h_chi2 = new TH2D("h_chi2", "Chi2 scan;Rel. JMS (s);Rel. JMR (r)", nSteps, 0.8, 1.2, nSteps, 0.2, 1);

    double bestChi2 = 1e9; // For chi2
    double bestS = -1, bestR = -1;

    for (int ix = 1; ix <= h_chi2->GetNbinsX(); ++ix) {
        double s = h_chi2->GetXaxis()->GetBinCenter(ix); // JMS factor
        for (int iy = 1; iy <= h_chi2->GetNbinsY(); ++iy) {
            double r = h_chi2->GetYaxis()->GetBinCenter(iy); // JMR factor

            // Create folded histogram
            TH1D* h_folded = new TH1D("h_folded", "", nBins, massMin, massMax);

            for (size_t i = 0; i < reco_mass_mc.size(); ++i) {
                double m_reco = reco_mass_mc[i];
                double m_truth = truth_mass[i];

                // Forward fold: apply scale and resolution relative to MC
                double m_fold = s * m_reco + (m_reco - m_truth * R_mc) * (r - s);
                h_folded->Fill(m_fold);
            }

            // Compute chi2
            
            double chi2 = 0;
            int ndf = 0;
            for (int b = 1; b <= nBins; ++b) {
                double d = h_data->GetBinContent(b);
                double m = h_folded->GetBinContent(b);
                double err = std::sqrt(d + 1e-6); // avoid 0
                if (err > 0) {
                    chi2 += std::pow((d - m) / err, 2);
                    ndf++;
                }
            }

            double chi2_ndf = chi2 / ndf;

            if (chi2_ndf < bestChi2) {
                bestChi2 = chi2_ndf;
                bestS = s;
                bestR = r;
            }

            h_chi2->SetBinContent(ix, iy, chi2_ndf);

     

            delete h_folded;

        }
    }

    // ---- Output best-fit parameters
    std::cout << "Best-fit JMS (s): " << bestS << std::endl;
    std::cout << "Best-fit JMR (r): " << bestR << std::endl;
    std::cout << "Best Chi-2 value   : " << bestChi2 << std::endl;

    // ---- Plot results
    gStyle->SetOptStat(0);
    TCanvas* c = new TCanvas("c", "Chi2 Scan", 1000, 800);
    h_chi2->Draw("COLZ");

    // Draw best point
    TMarker* marker = new TMarker(bestS, bestR, 29);
    marker->SetMarkerColor(kRed);
    marker->SetMarkerSize(2);
    marker->Draw("SAME");

    c->SetLogz();

    c->SaveAs("chi2_scan_JMS_JMR.png");
}
