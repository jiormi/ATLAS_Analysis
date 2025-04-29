// File: generate_bquark_tree.C
#include <TFile.h>
#include <TTree.h>
#include <TRandom3.h>
#include <TMath.h>
#include <iostream>

void generate_bquark_tree() {
    // Settings
    const int nEvents = 100000; // Number of events to simulate
    const double p0 = 5.0;       // Scale parameter for pT exponential (GeV)

    // Initialize random number generator
    TRandom3 rng(0); // 0: random seed

    // Create output ROOT file
    TFile *outFile = new TFile("bquark_output.root", "RECREATE");

    // Create a TTree
    TTree *tree = new TTree("bquarkTree", "Simulated b-quark kinematics");

    // Variables to be stored
    float pT, eta, phi;

    // Create branches
    tree->Branch("pT",  &pT,  "pT/F");
    tree->Branch("eta", &eta, "eta/F");
    tree->Branch("phi", &phi, "phi/F");

    // Event generation loop
    for (int i = 0; i < nEvents; ++i) {
        // Generate pT: exponential distribution
        pT = rng.Exp(p0);

        // Generate eta: flat within realistic range (say |eta| < 2.5)
        eta = rng.Uniform(-2.5, 2.5);

        // Generate phi: flat in [0, 2*pi]
        phi = rng.Uniform(0, 2*TMath::Pi());

        // Fill the tree
        tree->Fill();
    }

    // Write and close the file
    tree->Write();
    outFile->Close();

    std::cout << "Generated " << nEvents << " events and saved to bquark_output.root" << std::endl;
}
