void SmearingChi2Map() {
    gErrorIgnoreLevel = kWarning;
    // Open the ROOT file
    TFile* file = TFile::Open("Smearingtest.root");
    if (!file || file->IsZombie()) {
        std::cout << "Error opening file!" << std::endl;
        return;
    }

    // Define r and s ranges
    const int nR = 10; // r from 1.0 to 10.0
    const int nS = 7;  // s from 0.97 to 1.03 (step 0.01)

    double rMin = 1.0, rMax = 11.0;
    double sMin = 0.97, sMax = 1.03;

    std::cout << "Bin\tBest s (JES)\tBest r (JER)\tChi2 min" << std::endl;
    std::cout << "----------------------------------------------" << std::endl;

    // Loop over bins 1 to 10
    for (int bin = 1; bin <= 10; ++bin) {

        // Get the reference data histogram for this bin
        TString hDataName = Form("hData_%d", bin);
        TH1D* hData = (TH1D*)file->Get(hDataName);
        if (!hData) {
            std::cout << "Reference histogram " << hDataName << " not found!" << std::endl;
            continue;
        }

        // Create a chi2 TH2D for this bin
        TString chi2HistName = Form("chi2_map_bin%d", bin);
        TString chi2HistTitle = Form("Chi2 Map Bin %d; r; s", bin);
        TH2D* hChi2 = new TH2D(chi2HistName, chi2HistTitle,
                               nR, rMin, rMax,
                               nS, sMin, sMax);

        double minChi2 = 1e9;
        double bestS = -1;
        double bestR = -1;

        // Loop over s and r values
        for (int iS = 0; iS < nS; ++iS) {
            double s = 0.97 + iS * 0.01;

            for (int iR = 0; iR < nR; ++iR) {
                double r = 1.0 + iR * 1.0;

                // Build histogram name
                TString histName = Form("hDataFolded_%d_r%.1f_s%.2f", bin, r, s);

                TH1D* hFold = (TH1D*)file->Get(histName);
                if (!hFold) {
                    std::cout << "Histogram " << histName << " not found!" << std::endl;
                    continue;
                }

                   if(hData->GetEntries() == 0) printf("%s has 0 entries !!\n", hData->GetName());

                // Compute chi2
                double chi2 = hData->Chi2Test(hFold, "CHI2");
                hChi2->Fill(r, s, chi2);

                // Check for minimum
                if (chi2 < minChi2) {
                    minChi2 = chi2;
                    bestS = s;
                    bestR = r;
                }
            }
        }

        // Output best-fit values
        std::cout << bin << "\t" << bestS << "\t\t" << bestR << "\t\t" << minChi2 << std::endl;

        // Draw and save the chi2 map
        TCanvas* c = new TCanvas(Form("c_bin%d", bin), Form("Chi2 Map Bin %d", bin), 800, 600);
        hChi2->Draw("COLZ");

        // Mark best-fit point
        TMarker* marker = new TMarker(bestR, bestS, 29);
        marker->SetMarkerColor(kRed);
        marker->SetMarkerSize(2.0);
        marker->Draw("SAME");

        TString outName = Form("Chi2Map_bin%d.png", bin);
        c->SaveAs(outName);
    }
}

