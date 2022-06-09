#include "TFile.h"
#include "TH1F.h"
#include "TString.h"

TH1F* copyBinning(TH1F* hist_to_copy) {
    Int_t nbins = hist_to_copy->GetNbinsX(); // Number of bins of histogram 1.
    Double_t xbins[nbins+1]; // Array for low edges of histogram 1.
    hist_to_copy->GetXaxis()->GetLowEdge(xbins);
    // Must set last element of array as lower edge of overflow bin.
    xbins[nbins] = hist_to_copy->GetXaxis()->GetBinLowEdge(nbins+1);

    TH1F* hist = new TH1F("", "", nbins, xbins);
    return hist;
}

TH1F* averageHist(TH1F* hist_1, TH1F* hist_2) {

    TH1F* mean_hist = copyBinning(hist_1);
    Int_t nbins = mean_hist->GetNbinsX();

    for (size_t bin = 1; bin < nbins+1; bin++) {
        Float_t value_1 = hist_1->GetBinContent(bin);
        Float_t value_2 = hist_2->GetBinContent(bin);
        Float_t mean = (value_1 + value_2)/2.;

        mean_hist->SetBinContent(bin, mean);
    }

    return mean_hist;
}

void buildPromptsHisto() {

    // Charged particles density file.
    TFile* density_file = TFile::Open("../rawData/chargedPartDensity.root");
    // 0-5% centrality
    TH1F* density_hist_1 = (TH1F*)density_file->Get("DN-DETARAP/Hist1D_y1");
    density_hist_1->SetDirectory(0);
    // 5-10% centrality
    TH1F* density_hist_2 = (TH1F*)density_file->Get("DN-DETARAP/Hist1D_y2");
    density_hist_2->SetDirectory(0);

    density_file->Close();

    // 0-10% centrality
    TH1F* eta_hist = averageHist(density_hist_1, density_hist_2);
    eta_hist->SetNameTitle("eta", "pions, kaons and protons #eta");

    // Pions pT distribution file.
    TFile* pi_pt_f = TFile::Open("../rawData/ALICE_pi_pT.root");
    // 0-5% centrality
    TH1F* pi_pt_h_1 = (TH1F*)pi_pt_f->Get("Table 1/Hist1D_y1");
    pi_pt_h_1->SetDirectory(0);
    // 5-10% centrality
    TH1F* pi_pt_h_2 = (TH1F*)pi_pt_f->Get("Table 1/Hist1D_y2");
    pi_pt_h_2->SetDirectory(0);

    pi_pt_f->Close();

    // 0-10% centrality
    TH1F* pi_pt_h_var = averageHist(pi_pt_h_1, pi_pt_h_2);
    Int_t nbins = pi_pt_h_var->GetNbinsX();
    TH1F* pi_pt_h     = new TH1F("pT", "pions p_{T}", 500, 0, 20);

    // pT histogram has varaible binning -> Problems with RapidSim
    for (size_t i = 0; i < 10000000; i++) {
        Float_t rnd_value = pi_pt_h_var->GetRandom();
        Int_t bin = pi_pt_h_var->FindBin(rnd_value);
        Float_t bin_width = pi_pt_h_var->GetXaxis()->GetBinWidth(bin);

        pi_pt_h->Fill(rnd_value, bin_width);
    }

    pi_pt_h->Draw();


    // Save pion phase space histograms.
    TFile* pion_ps_file = new TFile("../../fonll/LHCpi5.root", "RECREATE");
    eta_hist->Write();
    pi_pt_h->Write();

    pion_ps_file->Close();
    delete pion_ps_file;
}
