void forward_folding_relative_JMS_JMR_likelihood() {
    const int nEvents = 10000;
    const int nBins = 50;
    const double massMin = 0;
    const double massMax = 200;

    // MC truth → reco constants
    double R_mc = 0.95;         // MC jet mass response
    double sigma_mc = 12.0;     // MC jet mass resolution (GeV)

    // ---- Generate truth-level MC jet mass
    std::vector<double> truth_mass;
    for (int i = 0; i < nEvents; ++i) {
        double m_truth = gRandom->Gaus(100, 10); // True mass: mean 100, sigma 10
        truth_mass.push_back(m_truth);
    }

    // ---- Reco-level MC: smear truth
    std::vector<double> reco_mass_mc;
    for (auto m_truth : truth_mass) {
        double m_reco = gRandom->Gaus(m_truth * R_mc, sigma_mc);
        reco_mass_mc.push_back(m_reco);
    }

    // ---- Generate fake data: use slightly different scale and resolution
    double scale_data = 1.00;       // True JMS
    double sigma_data = 14.0;       // True JMR
    TH1D* h_data = new TH1D("h_data", "Fake Data Mass;Mass [GeV];Events", nBins, massMin, massMax);

    for (auto m_truth : truth_mass) {
        double m_data = gRandom->Gaus(m_truth * scale_data, sigma_data);
        h_data->Fill(m_data);
    }

    // ---- Prepare scan over relative JMS (s) and JMR (r)
    const int nSteps = 40;
    TH2D* h_nll = new TH2D("h_nll", "-2 Log Likelihood Scan;Rel. JMS (s);Rel. JMR (r)", nSteps, 0.8, 1.2, nSteps, 0.6, 1.6);

    double bestNLL = 1e9;
    double bestS = -1, bestR = -1;

    for (int ix = 1; ix <= h_nll->GetNbinsX(); ++ix) {
        double s = h_nll->GetXaxis()->GetBinCenter(ix); // JMS factor
        for (int iy = 1; iy <= h_nll->GetNbinsY(); ++iy) {
            double r = h_nll->GetYaxis()->GetBinCenter(iy); // JMR factor

            // Create folded histogram
            TH1D* h_folded = new TH1D("h_folded", "", nBins, massMin, massMax);

            for (size_t i = 0; i < reco_mass_mc.size(); ++i) {
                double m_reco = reco_mass_mc[i];
                double m_truth = truth_mass[i];

                // Forward fold
                double m_fold = s * m_reco + (m_reco - m_truth * R_mc) * (r - s);
                h_folded->Fill(m_fold);
            }

            // Compute binned likelihood
            double logL = 0;
            for (int b = 1; b <= nBins; ++b) {
                double d = h_data->GetBinContent(b);
                double m = h_folded->GetBinContent(b);
                if (m <= 0) m = 1e-6; // Avoid log(0)
                if (d > 0)
                    logL += 2 * (m - d + d * log(d / m));
                else
                    logL += 2 * m;
            }

            h_nll->SetBinContent(ix, iy, logL);
            if (logL < bestNLL) {
                bestNLL = logL;
                bestS = s;
                bestR = r;
            }

            delete h_folded;
        }
    }

    // ---- Normalize to ΔNLL: best point is 0
    for (int ix = 1; ix <= h_nll->GetNbinsX(); ++ix) {
        for (int iy = 1; iy <= h_nll->GetNbinsY(); ++iy) {
            double val = h_nll->GetBinContent(ix, iy);
            h_nll->SetBinContent(ix, iy, val - bestNLL);
        }
    }

    // ---- Output best-fit parameters
    std::cout << "Best-fit Relative JMS (s): " << bestS << std::endl;
    std::cout << "Best-fit Relative JMR (r): " << bestR << std::endl;
    std::cout << "Minimum -2 log L        : " << bestNLL << std::endl;

    // ---- Plot results
    gStyle->SetOptStat(0);
    TCanvas* c = new TCanvas("c", "NLL Scan", 1000, 800);
    h_nll->Draw("COLZ");

    TMarker* marker = new TMarker(bestS, bestR, 29);
    marker->SetMarkerColor(kRed);
    marker->SetMarkerSize(2);
    marker->Draw("SAME");

    c->SaveAs("nll_scan_JMS_JMR.png");
}
