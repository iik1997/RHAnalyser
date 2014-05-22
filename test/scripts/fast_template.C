void fastm() {
//
//energy_vs_eta_reco //et_vs_eta_reco //towet_vs_eta_reco //energy_reco_0 //energy_reco_1 //cas_etot15 //cas_zprofile //cas_phiprofile15 //towet_reco_3 //cas_chprofile
//
//TFile *_file0 = TFile::Open("root://eoscms//eos/cms/store/group/phys_heavyions/katkov/HydjetTuneDrum_QuenchedMB_RECO_44X_53X/44X_Simple01_gen_out_1_1_ttg.root");
//TFile *_file0 = TFile::Open("/afs/cern.ch/user/k/katkov/afswork/hiforest/CMSSW_4_2_8/src/Analysis/RHAnalyser/test/gen_out_44Xnewer.root");//gen_out_merged44X.root");
//TFile *_file0 = TFile::Open("/afs/cern.ch/user/k/katkov/afswork/hiforest/CMSSW_4_2_8/src/Analysis/RHAnalyser/test/gen_out_53X_LV1.root");
//TFile *_file0 = TFile::Open("/afs/cern.ch/user/k/katkov/afswork/hiforest/CMSSW_4_2_8/src/Analysis/RHAnalyser/test/gen_out_53X_LV1_track8_jet5.root");
//gen_out_relval5316_GR_R_53_LV6_mar03_data.root, HIMinBiasUPC_CMSSW_5_3_16-GR_R_53_LV6_mar03_RelVal_hi2011-v2
//gen_out_relval5316_GR_R_53_LV6_mar03_data_hforbsc_noscr_vtx.root, HIMinBiasUPC_CMSSW_5_3_16-GR_R_53_LV6_mar03_RelVal_hi2011-v2 (but below 'no selection' would be wrong)
//gen_out_rereco5316_Jet55or65ScraVtx_FT_R_53_LV6_14mar2014_data.root, HIHighPt_CMSSW_5_3_16-FT_R_53_LV6_mar14_ReReco, Jet55or65ScraVtx
TFile *_file0 = TFile::Open("arg_full_path_to_file0");
gROOT->ProcessLine(".L tdrstyle.C");
gROOT->ProcessLine("setTDRStyle()");
//gStyle->SetPadRightMargin(0.20);
gStyle->SetPadLeftMargin(0.20);
gROOT->ForceStyle();
TCanvas* c1 = new TCanvas("c1","c1");
_file0->cd("demo");
arghisto->GetYaxis()->SetTitleOffset(1.55);
bool donorm = want_normalization;
if (!donorm) {
   arghisto->GetYaxis()->SetTitle("Response");
} else {
   arghisto->GetYaxis()->SetTitle("a.u.");
}
arghisto->SetLineWidth(2);
//arghisto->SetLineStyle(2);
arghisto->Sumw2();
c1->cd();
int ncombine = bins_to_combine;
if (ncombine > 1) arghisto->Rebin(ncombine); //8000 //2000 //100 //50
double entries0 = 1.0;
if (donorm) { 
   entries0=static_cast<double>(arghisto->GetEntries());
   //entries0=arghisto->Integral();
   arghisto->Scale(1.0/entries0);
}
double rescale = re_scale_factor; 
rescale = arghisto->GetMaximum()*rescale;
arghisto->SetMaximum(rescale);
arghisto->SetMinimum(0.0);
arghisto->Draw("s");
arghisto->Draw("hist same");
//TFile *_file1 = TFile::Open("root://eoscms//eos/cms/store/group/phys_heavyions/katkov/HydjetTuneDrum_QuenchedMB_RECO_44X_53X/53X_Simple01_gen_out_1_1_VCb.root");
//TFile *_file1 = TFile::Open("/afs/cern.ch/user/k/katkov/afswork/hiforest/CMSSW_4_2_8/src/Analysis/RHAnalyser/test/gen_out_53X_LV1.root");
//TFile *_file1 = TFile::Open("/afs/cern.ch/user/k/katkov/afswork/hiforest/CMSSW_4_2_8/src/Analysis/RHAnalyser/test/gen_out_53X_LV1_track8_jet5.root");//gen_out_53X_LV1_track8_jet6_dijet80.root
//TFile *_file1 = TFile::Open("/afs/cern.ch/user/k/katkov/afswork/hiforest/CMSSW_4_2_8/src/Analysis/RHAnalyser/test/gen_out_53X_LV1_track8_jet6_dijet80.root");
//TFile *_file1 = TFile::Open("/afs/cern.ch/user/k/katkov/afswork/hiforest/CMSSW_4_2_8/src/Analysis/RHAnalyser/test/gen_out_53X_LV2B_track8_jet5_data.root");
//TFile *_file1 = TFile::Open("/afs/cern.ch/user/k/katkov/afswork/hiforest/CMSSW_4_2_8/src/Analysis/RHAnalyser/test/gen_out_53X_GR_R_53_LV6_track8_Jet15_data.root");
//gen_out_relval5316_STARTHI_LV1_mar03_mc.root, RelValHydjetQ_MinBias_2760GeV_CMSSW_5_3_16-STARTHI53_LV1_mar03-v2
///gen_out_privreco5316_Jet55or65ScraVtx_GR_R_53_LV6_03Mar2014_1600CET.root, HighPt_CMSSW_5_3_16-GR_R_53_LV6_03Mar2014_1600CET_PrivRecoStas, Jet55or65ScraVtx 
TFile *_file1 = TFile::Open("arg_full_path_to_file1");
_file1->cd("demo");
arghisto->SetLineColor(kRed);
arghisto->SetLineWidth(2);
arghisto->SetMarkerColor(kRed);
//arghisto->SetLineStyle(2);
arghisto->Sumw2();
if (ncombine > 1) arghisto->Rebin(ncombine); //8000 //2000 //100 //50
double entries1 = 1.0;
if (donorm) {
   entries1 = static_cast<double>(arghisto->GetEntries());
   //entries1=arghisto->Integral();
   arghisto->Scale(1.0/entries1);
}
arghisto->Draw("s same");
arghisto->Draw("hist same");
_file0->cd("demo");
TH1D* hrat = (TH1D*)arghisto->Clone("hrat");
_file1->cd("demo");
TH1D* hnew = (TH1D*)arghisto->Clone("hnew");
hrat->Divide(hnew);
hrat->SetYTitle("Ratio");
TCanvas* c2 = new TCanvas("c2","c2");
c2->cd();
//hrat->Draw("hist");
hrat->SetMaximum(1.5);
hrat->SetMinimum(0.0);
hrat->Draw("hist text");
//hrat->Draw("e text0");
c2->Print(Form("figures/%s_%s_c2.pdf","arg_file0_title","arghisto"));
c1->cd();
TLegend* legend = new TLegend(0.18,0.85,0.55,0.90);
legend->SetFillStyle(0);
legend->SetBorderSize(0);
legend->SetTextFont(42);
legend->SetTextSize(0.025);//0.035
legend->AddEntry(hrat, "arg_file0_title", "p");//HIMinBiasHfOrBSC&&noscraping&&vtx//HydjetDrum1p8_MB_STARTHI53_LV1_track8_Jet5_5Feb2014_12pmEST//HydjetTuneDrum_QuenchedMB_44X//HydjetTuneDrum QuenchedMB 44X//HydjetTuneDrum_QuenchedMB_44X
legend->AddEntry(hnew, "arg_file1_title", "p");//HIMinBias2011_GR_R_53_LV6_20Feb2014_1000amEST_Track8_Jet15//HIMinBias2011_GR_R_53_LV2B_Track8_Jet5_RECO_05Feb14_10amEST//HydjetDrum_Pyquen_Dijet80_Embedded_RECO_STARTHI53_LV1_Track8_Jet6_8Feb2014//HydjetDrum1p8_MB_STARTHI53_LV1_track8_Jet5_5Feb2014_12pmEST//HydjetDrum1p8_MB_STARTHI53_LV1_track7_Jet3_31Jan2014_9amEST//HydjetTuneDrum QuenchedMB 53X//HydjetDrum1p8_MB_STARTHI53_LV1_track7_Jet3_31Jan2014_9amEST
legend->Draw();
c1->Print(Form("figures/%s_%s_c1.pdf","arg_file0_title","arghisto"));
//c2->cd();
//legend->Draw();
}
