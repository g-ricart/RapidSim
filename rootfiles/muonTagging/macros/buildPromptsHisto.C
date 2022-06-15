#include "TFile.h"
#include "TH1F.h"
#include "TString.h"
#include "TAxis.h"
#include "TCanvas.h"

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

//________________________________ETA___________________________________________

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

//________________________________PION__________________________________________

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
    TH1F*  pi_pt_h_var = averageHist(pi_pt_h_1, pi_pt_h_2);
    Int_t  nbins       = pi_pt_h_var->GetNbinsX();
    TAxis* axis        = pi_pt_h_var->GetXaxis();
    TH1F*  pi_pt_h     = new TH1F("pT", "pions p_{T}", 200, axis->GetXmin(),
                                                            axis->GetXmax());

//________________________________KAON__________________________________________

    // Kaons pT distribution file.
    TFile* k_pt_f = TFile::Open("../rawData/ALICE_K_pT.root");
    // 0-5% centrality
    TH1F* k_pt_h_1 = (TH1F*)k_pt_f->Get("Table 3/Hist1D_y1");
    k_pt_h_1->SetDirectory(0);
    // 5-10% centrality
    TH1F* k_pt_h_2 = (TH1F*)k_pt_f->Get("Table 3/Hist1D_y2");
    k_pt_h_2->SetDirectory(0);

    k_pt_f->Close();

    // 0-10% centrality
    TH1F*  k_pt_h_var = averageHist(k_pt_h_1, k_pt_h_2);
           nbins      = k_pt_h_var->GetNbinsX();
           axis       = k_pt_h_var->GetXaxis();
    TH1F*  k_pt_h     = new TH1F("pT", "kaons p_{T}", 200, axis->GetXmin(),
                                                           axis->GetXmax());

//________________________________PROTON________________________________________

    // (Anti)protons pT distribution file.
    TFile* p_pt_f = TFile::Open("../rawData/ALICE_ppbar_pT.root");
    // 0-5% centrality
    TH1F* p_pt_h_1 = (TH1F*)p_pt_f->Get("Table 5/Hist1D_y1");
    p_pt_h_1->SetDirectory(0);
    // 5-10% centrality
    TH1F* p_pt_h_2 = (TH1F*)p_pt_f->Get("Table 5/Hist1D_y2");
    p_pt_h_2->SetDirectory(0);

    p_pt_f->Close();

    // 0-10% centrality
    TH1F*  p_pt_h_var = averageHist(p_pt_h_1, p_pt_h_2);
           nbins      = p_pt_h_var->GetNbinsX();
           axis       = p_pt_h_var->GetXaxis();
    TH1F*  p_pt_h     = new TH1F("pT", "(anti)protons p_{T}", 200,
                                                              axis->GetXmin(),
                                                              axis->GetXmax());


//________________________________FILL__________________________________________

    // pT histograms have variable binning -> Problems with RapidSim
    for (size_t i = 0; i < 10000000; i++) {

        Float_t rnd_pi_value = pi_pt_h_var->GetRandom();
        Int_t   pi_bin       = pi_pt_h_var->FindBin(rnd_pi_value);
        Float_t pi_bin_width = pi_pt_h_var->GetXaxis()->GetBinWidth(pi_bin);

        Float_t rnd_k_value = k_pt_h_var->GetRandom();
        Int_t   k_bin       = k_pt_h_var->FindBin(rnd_k_value);
        Float_t k_bin_width = k_pt_h_var->GetXaxis()->GetBinWidth(k_bin);

        Float_t rnd_p_value = p_pt_h_var->GetRandom();
        Int_t   p_bin       = p_pt_h_var->FindBin(rnd_p_value);
        Float_t p_bin_width = p_pt_h_var->GetXaxis()->GetBinWidth(p_bin);

        pi_pt_h->Fill(rnd_pi_value, pi_bin_width);
        k_pt_h->Fill(rnd_k_value, k_bin_width);
        p_pt_h->Fill(rnd_p_value, p_bin_width);
    }

    TCanvas* c1 = new TCanvas("c1", "", 1000, 800);
    pi_pt_h->Draw();

    TCanvas* c2 = new TCanvas("c2", "", 1000, 800);
    k_pt_h->Draw();

    TCanvas* c3 = new TCanvas("c3", "", 1000, 800);
    p_pt_h->Draw();

//________________________________WRITE_________________________________________
    // Save pions phase space histograms.
    TFile* pion_ps_file = new TFile("../../fonll/LHCpi5.root", "RECREATE");
    eta_hist->Write();
    pi_pt_h->Write();

    pion_ps_file->Close();
    delete pion_ps_file;

    // Save kaons phase space histograms.
    TFile* kaon_ps_file = new TFile("../../fonll/LHCk5.root", "RECREATE");
    eta_hist->Write();
    k_pt_h->Write();

    kaon_ps_file->Close();
    delete kaon_ps_file;

    // Save (anti)protons phase space histograms.
    TFile* ppbar_ps_file = new TFile("../../fonll/LHCp5.root", "RECREATE");
    eta_hist->Write();
    p_pt_h->Write();

    ppbar_ps_file->Close();
    delete ppbar_ps_file;
}
