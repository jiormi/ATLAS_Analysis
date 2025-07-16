void drawText(double x, double y, TString txt) {
  static auto tex = new TLatex();
  tex->SetNDC(); tex->SetTextSize(0.036); tex->SetTextFont(42);
  tex->DrawLatex(x, y, txt);
}

double returnESmear(double Etrue = 40, double sigMC = 4.0, double s = 0.98, double r = 1.4) {
    double Ereco = gRandom->Gaus(Etrue, sigMC);
    double Esmear = r>s? s * Ereco + sqrt(r*r - s*s) * gRandom->Gaus(0, sigMC) : s * Ereco + sqrt(s*s - r*r) * gRandom->Gaus(0, sigMC);

    return Esmear;
}


void testSmearing() {
  gStyle->SetOptStat(0); gStyle->SetPadRightMargin(0.02); gStyle->SetPadTopMargin(0.03);
  std::vector<double> ptBins = {25, 45, 64, 85, 95, 105, 120, 140, 180, 220, 280, 350};
  double ptBinsNum = ptBins.size()-1;
  std::vector<double> s_values = {0.97, 0.98, 0.99, 1.00, 1.01, 1.02, 1.03};
  std::vector<double> r_values = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0};

  double sigData = 6.0; // For Smearing this is the only accepted case
  double sigMC = 4.0;

  TFile *f = TFile::Open("Smearingtest.root","RECREATE");

  

  for(int bin=1;bin<ptBinsNum;bin++){
    double low = ptBins.at(bin-1);
    double up = ptBins.at(bin);
    double Nevents = gRandom->Poisson(1e5);
    int Nevents_int = static_cast<int>(Nevents);
    TH1D *h = new TH1D(Form("hData_%d",bin),"",50,low,up);

    for(int event = 0; event<Nevents_int; event++){

      double ErecoData = gRandom->Gaus((low + up) / 2., sigData);
      h->Fill(ErecoData);
    }
  
    h->Write();
    delete h;
  
    for(auto s_i:s_values){
      for(auto r_i:r_values){
        
        TH1D *hf = new TH1D(Form("hDataFolded_%d_r%.1f_s%.2f",bin,r_i,s_i),"",50,low,up);

        for(int event = 0; event<Nevents_int; event++){
        double EfoldedDataMC = returnESmear((low + up) / 2. , sigMC, s_i,r_i);
        hf->Fill(EfoldedDataMC);

        }

        
        hf->Write();
        delete hf;

    }
  }


  }



}
