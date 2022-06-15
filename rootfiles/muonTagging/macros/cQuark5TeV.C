#include "TTree.h"
#include "TH1D.h"
#include "TCanvas.h"

void buildc5Histo() {
    auto pt_tree  = new TTree();
    auto eta_tree = new TTree();
    pt_tree->ReadFile("../rawData/FONLL_pt_c_5TeV.csv",   "pt/D:s");
    eta_tree->ReadFile("../rawData/FONLL_eta_c_5TeV.csv", "eta/D:s");

    auto pt_h  = new TH1D("pT", "", 401, 0, 20);
    auto eta_h = new TH1D("eta", "", 301, 2, 5);

    Double_t pt = 0.;
    Double_t eta = 0.;
    Double_t sigma = 0.;

    pt_tree->SetBranchAddress("pt", &pt);
    pt_tree->SetBranchAddress("s",  &sigma);

    for (size_t entry_index = 0; entry_index < pt_tree->GetEntries(); entry_index++) {
        pt_tree->GetEntry(entry_index);
        pt_h->Fill(pt, sigma);
    }

    delete pt_tree;

    eta_tree->SetBranchAddress("eta", &eta);
    eta_tree->SetBranchAddress("s",   &sigma);

    for (size_t entry_index = 0; entry_index < eta_tree->GetEntries(); entry_index++) {
        eta_tree->GetEntry(entry_index);
        eta_h->Fill(eta, sigma);
    }

    delete eta_tree;

    auto c1 = new TCanvas("c1", "", 1000, 800);
    pt_h->Draw();
    auto c2 = new TCanvas("c2", "", 1000, 800);
    eta_h->Draw();

    TFile* ps_file = new TFile("../../fonll/LHCc5.root", "RECREATE");
    eta_h->Write();
    pt_h->Write();
}
