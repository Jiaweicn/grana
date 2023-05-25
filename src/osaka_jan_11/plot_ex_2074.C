{
  gROOT->Reset();
  gROOT->LoadMacro("h700_xf_12_22.C");
  gROOT->LoadMacro("plot_kvi.C");


  TChain ch("nt");

  Double_t b_percent, ang3;
  cout<<"Input b-field (in percentage) and angel (in deg) : ";
  cin>>b_percent>>ang3;

  char fname[20], fnumber[20];
  while (1) {
    cout<<"run number (type -1 to end): ";
    cin>>fnumber;
    if(fnumber[0]=='-') { cout<<fnumber<<endl; break; }
    sprintf(fname,"res%s.root", fnumber);
    ch.Add(fname);
  }


  TH1F *hex4_a = new TH1F("hex4_a","ex4_a", 5000,0,25);
  TH1F *hex4_b = new TH1F("hex4_b","ex4_b", 5000,0,25);
  TH1F *hex5_a = new TH1F("hex5_a","ex5_a", 5000,0,25);
  TH1F *hex5_b = new TH1F("hex5_b","ex5_b", 5000,0,25);
  hex5_b->SetLineColor(kBlue);

  TH2F *h2_AC_XCp   = new TH2F("h2_AC_XCp","AC:XCp",1200,-400,400,200,-3,3);
  TH2F *h2_AC_XC    = new TH2F("h2_AC_XC","AC:XC",1200,-400,400,200,-3,3);

  TCanvas c2("c2","",0,0,800,600);
  ch->Draw("AC:XCp>>h2_AC_XCp","Pid1>220 && ((RFc>776 && RFc<830) || (RFc>261 && RFc<320) ) && abs(YC-0.27)<1.5","col");
 
  TCanvas c3("c3","",0,0,800,600);
  ch->Draw("AC:XC-(((-1.6617e+00*AC-2.2753e+00)*AC+ 1.5228e+00)*AC-6.4893e+00)*AC>>h2_AC_XC","Pid1>220 && ((RFc>776 && RFc<830) || (RFc>261 && RFc<320) ) && abs(YC-0.27)<1.5","col");

  TCanvas c4("c4","",0,0,800,600);
  h2_AC_XCp->ProjectionX("h1_XC_a",73,100,"");
  h1_XC_a->SetLineColor(2);
  h2_AC_XCp->ProjectionX("h1_XC_b",100,127,"");
  h1_XC_a->Draw();
  h1_XC_b->Draw("same");
  TH1F* h1_XC_a = (TH1F*) gROOT->FindObject("h1_XC_a");
  TH1F* h1_XC_b = (TH1F*) gROOT->FindObject("h1_XC_b");

  TCanvas c1("c1","",0,0,800,600);
  sprintf(fname,"4a_%s.dat",fnumber);
  ofstream fout(fname);
  TGraph *gr_ex4_a = new TGraph(h1_XC_a->GetNbinsX());
  for(int i=0;i<h1_XC_a->GetNbinsX();i++) {
        Double_t Xcp_tmp = h1_XC_a->GetBinCenter(i);
        Double_t ex4 = get_ex(Xcp_tmp,b_percent,ang3);
        Double_t cnt = h1_XC_a->GetBinContent(i);
        gr_ex4_a->SetPoint(i,ex4,cnt);
        fout<<ex4<<" "<<cnt<<endl;
//        cout<<Xcp_tmp<<" "<<ex4<<" "<<cnt<<endl;
  }
  gr_ex4_a->SetLineColor(kBlue);
  gr_ex4_a->SetLineWidth(2);
  gr_ex4_a->Draw("AL");
  fout.close();

  TGraph *gr_ex4_b = new TGraph(h1_XC_b->GetNbinsX());
  for(int i=0;i<h1_XC_b->GetNbinsX();i++) {
        Double_t Xcp_tmp = h1_XC_b->GetBinCenter(i);
        Double_t ex4 = get_ex(Xcp_tmp,b_percent,ang3);
        Double_t cnt = h1_XC_b->GetBinContent(i);
        gr_ex4_b->SetPoint(i,ex4,cnt);
//        cout<<Xcp_tmp<<" "<<ex4<<" "<<cnt<<endl;
  }
  gr_ex4_b->SetLineColor(kGreen+3);
  gr_ex4_b->SetLineWidth(2);
  gr_ex4_b->Draw("L");


  TGraph gr("kawabata_zero.dat");
  gr->SetLineColor(2);
  gr->Draw("L");
}
