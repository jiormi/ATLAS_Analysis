// File: compare_gen_reco_variables.C
#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TLegend.h>
#include <TLine.h>
#include <TStyle.h>
#include <iostream>

void plot_variable(TTree* genTree, TTree* recoTree, const char* varName, const char* title, 
                   float xmin, float xmax, const char* outputName) {
    // Create histograms
    TH1F* h_gen = new TH1F("h_gen", title, 50, xmin, xmax);
    TH1F* h_reco = new TH1F("h_reco", title, 50, xmin, xmax);

    // Fill histograms
    genTree->Draw(Form("%s>>h_gen", varName));
    recoTree->Draw(Form("%s>>h_reco", varName));

    // Create canvas
    TCanvas* c = new TCanvas(Form("c_%s", varName), title, 800, 800);

    TPad* pad1 = new TPad("pad1", "Top pad", 0, 0.3, 1, 1.0);
    pad1->SetBottomMargin(0.02);
    pad1->Draw();
    pad1->cd();

    gStyle->SetOptStat(0);

    h_gen->SetLineColor(kBlue);
    h_gen->GetXaxis()->SetLabelSize(0);
    h_reco->SetLineColor(kRed);
    h_reco->GetXaxis()->SetLabelSize(0);
    h_reco->SetMarkerStyle(20);
    h_reco->SetMarkerSize(0.7);

    h_gen->Draw("hist");
    h_reco->Draw("same p e1");

    TLegend* leg = new TLegend(0.6,0.7,0.85,0.85);
    leg->AddEntry(h_gen, "Generated", "l");
    leg->AddEntry(h_reco, "Reconstructed", "lep");
    leg->Draw();

    // Lower pad
    c->cd();
    TPad* pad2 = new TPad("pad2", "Bottom pad", 0, 0, 1, 0.3);
    pad2->SetTopMargin(0.05);
    pad2->SetBottomMargin(0.3);
    pad2->Draw();
    pad2->cd();

    TH1F* h_ratio = (TH1F*)h_reco->Clone("h_ratio");
    h_ratio->Divide(h_gen);

    h_ratio->SetTitle("");
    h_ratio->GetYaxis()->SetTitle("Reco / Gen");
    h_ratio->GetYaxis()->SetTitleSize(0.08);
    h_ratio->GetYaxis()->SetTitleOffset(0.5);
    h_ratio->GetYaxis()->SetLabelSize(0.07);
    h_ratio->GetYaxis()->SetNdivisions(505);

    h_ratio->GetXaxis()->SetTitle(varName);
    h_ratio->GetXaxis()->SetTitleSize(0.1);
    h_ratio->GetXaxis()->SetLabelSize(0.08);

    h_ratio->SetMarkerStyle(20);
    h_ratio->SetMarkerSize(0.7);
    h_ratio->SetLineColor(kBlack);
    h_ratio->Draw("p e1");

    // Draw a line at y=1
    TLine* line = new TLine(xmin,1,xmax,1);
    line->SetLineStyle(2);
    line->Draw("same");

    // Save
    c->SaveAs(outputName);

    // Clean
    delete h_gen;
    delete h_reco;
    delete h_ratio;
    delete c;
}

void compare_gen_reco() {
    // Open both files
    TFile* genFile = TFile::Open("bquark_output.root");
    TFile* recoFile = TFile::Open("bjet_reco_output.root");
    if (!genFile || !recoFile) {
        std::cerr << "Error opening one of the files!" << std::endl;
        return;
    }

    TTree* genTree = (TTree*)genFile->Get("bquarkTree");
    TTree* recoTree = (TTree*)recoFile->Get("bquarkTree");
    if (!genTree || !recoTree) {
        std::cerr << "Error getting TTree from files!" << std::endl;
        return;
    }

    // Now plot each variable
    plot_variable(genTree, recoTree, "pT",  "p_{T} Comparison;p_{T} [GeV];Events", 0, 50, "gen_vs_reco_pT.png");
    plot_variable(genTree, recoTree, "eta", "Eta Comparison;#eta;Events",          -3, 3, "gen_vs_reco_eta.png");
    plot_variable(genTree, recoTree, "phi", "Phi Comparison;#phi;Events",           0, 6.5, "gen_vs_reco_phi.png");

    genFile->Close();
    recoFile->Close();
}
