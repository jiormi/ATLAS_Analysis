// File: simulate_bjet_reco.C
#include <TFile.h>
#include <TTree.h>
#include <TRandom3.h>
#include <iostream>

void simulate_bjet_reco() {
    // Open the generated b-quark file
    TFile *genFile = TFile::Open("bquark_output.root");
    if (!genFile || genFile->IsZombie()) {
        std::cerr << "Error opening generated file!" << std::endl;
        return;
    }

    TTree *genTree = (TTree*)genFile->Get("bquarkTree");
    if (!genTree) {
        std::cerr << "TTree 'bquarkTree' not found!" << std::endl;
        return;
    }

    // Variables to read
    float gen_pT, gen_eta, gen_phi;
    genTree->SetBranchAddress("pT",  &gen_pT);
    genTree->SetBranchAddress("eta", &gen_eta);
    genTree->SetBranchAddress("phi", &gen_phi);

    // Create output file for reconstructed jets
    TFile *recoFile = new TFile("bjet_reco_output.root", "RECREATE");
    TTree *recoTree = new TTree("bquarkTree", "Simulated reconstructed b-jet kinematics");

    // Variables to store
    float reco_pT, reco_eta, reco_phi;

    recoTree->Branch("pT",  &reco_pT,  "pT/F");
    recoTree->Branch("eta", &reco_eta, "eta/F");
    recoTree->Branch("phi", &reco_phi, "phi/F");

    // Random number generator
    TRandom3 rng(0);

    int nEntries = genTree->GetEntries();
    for (int i = 0; i < nEntries; ++i) {
        genTree->GetEntry(i);

        // Apply smearing
        // Assume 10% relative pT resolution
        double pT_resolution = 0.10;
        reco_pT = rng.Gaus(gen_pT, pT_resolution * gen_pT);

        // Assume eta resolution ~ 0.02
        double eta_resolution = 0.02;
        reco_eta = rng.Gaus(gen_eta, eta_resolution);

        // Assume phi resolution ~ 0.02
        double phi_resolution = 0.02;
        reco_phi = rng.Gaus(gen_phi, phi_resolution);

        recoTree->Fill();
    }

    recoTree->Write();
    recoFile->Close();
    genFile->Close();

    std::cout << "Reconstructed b-jet events saved to bjet_reco_output.root" << std::endl;
}
