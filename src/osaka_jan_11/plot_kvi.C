void change_gr(TGraph* grt, Double_t x_of, Double_t y_of, Double_t y_g) {
  Double_t* px = grt->GetX();
  Double_t* py = grt->GetY();

  Int_t n = grt->GetN();
  for(int i=0;i<n;i++) {
    px[i]=px[i]+x_of;
    py[i]=y_g*py[i]+y_of;
  }
}

Double_t get_ex(Double_t Xcp, Double_t b_percent, Double_t ang3) {
// get brho in the unit of T*m
// calibration states for 24Mg : 4.238 (2+), 6.01084 (4+), 6.4323 (0+), 7.349 (2+), 7.61647 (3-), 8.35798 (3-)
//2074 B=1.0 calibration, (-316,-143)
//       Double_t brho1=b_percent*(-2.250822659e-05*(Xcp/100)*(Xcp/100) - 1.272363076e-02*(Xcp/100) + 2.032063799e+00);
//       Double_t brho1 = b_percent*((-5.830280e-06*(Xcp/100) - 1.254694e-02)*Xcp/100 + 2.032361e+00); 
       Double_t brho1 = b_percent*((-5.609632e-05*(Xcp/100) - 1.289706e-02)*Xcp/100 + 2.031985e+00);
// run2077 B=1.00981 (-156,13)
//       Double_t brho2=b_percent*(-1.976358992E-05*(Xcp/100)*(Xcp/100) - 1.292492453E-02*(Xcp/100) + 2.051871383E+00)/1.00981;
//       Double_t brho2=b_percent*((-2.133197e-05*(Xcp/100) - 1.293502e-02)*(Xcp/100) + 2.051932e+00)/1.00981;
       Double_t brho2 = b_percent*((-5.256583e-05*(Xcp/100) - 1.298288e-02)*(Xcp/100) + 2.051979e+00)/1.00983;
// run2079: (-1.77,163.98)
// B=1.01956 (based on 2074)
// B'=1.00981*1.00972 (based on 2077)
//       Double_t brho3=b_percent*(-5.606547926E-05*(Xcp/100)*(Xcp/100) - 1.311684871E-02*(Xcp/100) + 2.071815426E+00)/1.01956;
//       Double_t brho3=b_percent*((-1.304745e-04*(Xcp/100) - 1.300470e-02)*(Xcp/100) + 2.071812e+00)/1.01961;
       Double_t brho3=b_percent*((-1.032281e-04*(Xcp/100) -  1.302167e-02)*(Xcp/100) + + 2.071913e+00)/1.01966;

       Double_t brho=0;
       if( Xcp<-142) {
         brho=brho1;
       } else if(Xcp<13) {
           brho=brho2;
       } else {
         brho=brho3;
       }
       Double_t ex4=relex(brho *2*speed_of_light/1e6,ang3);
       return ex4;
}

void fit_peak(TH1F* h1, Double_t low, Double_t hi) {
  TF1 *f1=new TF1("f1","gaus(0)",low,hi);
  f1->SetParameters(10,(low+hi)/2.0,4);
  f1->SetParLimits(1,low,hi);
  h1->Fit(f1,"R");
  f1->Delete();
}

void write_ex(TH1F* h1, char* str) {
  Int_t n=h1->GetNbinsX();

  TAxis *xaxis = h1->GetXaxis();
  ofstream fout(str);

  for(int i=0;i<n;i++) {
    Double_t y=h1->GetBinContent(i);
    if( y>0 )
    fout<<xaxis->GetBinCenter(i)<<" "<<y<<endl;                   
  }
  fout.close();
}

void plot_agui(double gg, double of) {
    // read in aguilera resonance information
    char str[50];
    TF1* fg[24];
    float strength[24]={0.4, 35, 2.37, 9.0,
			0.04,1.0,2.0,1.4,
			6.0, 0.38, 6.2, 15,
			11, 0.045, 0.06, 1.5,
                        0.4, 6.621e-3, 0.493e-3, 65.2e-6,
                        3.47e-6, 1.062e-6, 0.127e-6, 0.02e-6};

    float er[24]={4.25, 5.71, 5.80, 5.97,
	          4.64, 4.865,4.99, 5.38,
                  5.66, 5.78, 6.01, 6.29,
                  6.65, 4.44, 5.75, 5.95,
                  6.49, 3.789,3.410,3.122,
                  2.88, 2.72, 2.59, 2.25};
    float fwhm[24]={0.08, 0.07, 0.05, 0.05,
	            0.04, 0.08, 0.10, 0.08,
                    0.05, 0.06, 0.05, 0.06,
                    0.05, 0.06, 0.06, 0.05,
                    0.05, 0.08, 0.078,0.09,
                    0.02, 0.025, 0.025,0.10};
    float jpi[24]={ 0, 0, 0, 0,
                    2, 2, 2, 2,
                    2, 2, 2, 2,
                    2, 4, 4, 4,
                    6, 0, 0, 0,
                    0, 0, 0, 0};

    // apply an energ shift on KVI data
//    double of=0.0;
//    double gg=800*2;
    for(int i=0;i<24;i++) {
      sprintf(str,"ag%i",i);
      cout<<"Buliding "<<str<<endl;
      fg[i]=new TF1(str,"[0]*([2]/2)^2/((x-[1])^2+([2]/2)^2)+(155-37)/(22-14)*(x-14)*0+37",er[i]+of-4*fwhm[i],er[i]+of+4*fwhm[i]);

      strength[i]=(2*jpi[i]+1)*strength[i]*er[i]*exp(87.21/sqrt(er[i])+0.46*er[i]);
      fg[i]->SetParameters(strength[i]*gg/(er[i]+of),er[i]+of,fwhm[i]/2.56);
      cout<<i<<" "<<strength[i]<<" "<<er[i]<<" "<<fwhm[i]<<endl;
      if( jpi[i]==2 ) fg[i]->SetLineColor(kBlue);
      else if(jpi[i]==3 ) fg[i]->SetLineColor(kMagenta);
      else if(jpi[i]==0 ) fg[i]->SetLineColor(kRed);
      else fg[i]->SetLineColor(kCyan);
      fg[i]->SetNpx(1000);
    }
      for(int i=0;i<24;i++) fg[i]->Draw("same");
}

void plot_kvi(double gg, double of) {
    // read in KVI resonance information
    char str[50];
    TF1* fg[20];
    float strength[20]={1.4,1.9,2.3,1.8,0.3,
                        0.3,1.0,4.3,4.1,1.8,
                        1.5,1.3,2.4,1.2,2.8,
                        0.9,4.4,8.1,2.6,2.2};

    float er[20]={14.10,14.47,14.74,15.35,15.72,
                  16.08,16.53,16.92,17.36,17.75,
                  18.00,18.30,18.68,19.03,19.50,
                  19.85,20.20,21.80,24.00,26.50};
    float fwhm[20]={0.170,0.170,0.170,0.350,0.140,
                    0.210,0.140,0.490,0.220,0.250,
                    0.350,0.280,0.280,0.210,0.350,
                    0.280,0.490,1.410,1.060,1.410};
    float jpi[20]={3,2,2,4,4,4
                   2,2,2,2,2,
                   2,2,2,2,2,
                   2,2,2,3,4};

    // apply an energ shift on KVI data
//    double of=0.0;
//    double gg=800*2;
    for(int i=0;i<20;i++) {
      sprintf(str,"g%i",i);
      cout<<"Buliding "<<str<<endl;
      fg[i]=new TF1(str,"[0]*([2]/2)^2/((x-[1])^2+([2]/2)^2)+(155-37)/(22-14)*(x-14)*0+37",er[i]+of-4*fwhm[i],er[i]+of+4*fwhm[i]);
      fg[i]->SetParameters(strength[i]*gg/(er[i]+of),er[i]+of,fwhm[i]/2.56);
      cout<<i<<" "<<strength[i]<<" "<<er[i]<<" "<<fwhm[i]<<endl;
      if( jpi[i]==2 ) fg[i]->SetLineColor(kBlue);
      else if(jpi[i]==3 ) fg[i]->SetLineColor(kMagenta);
      else fg[i]->SetLineColor(kCyan);
      fg[i]->SetNpx(1000);
    }
      for(int i=0;i<20;i++) fg[i]->Draw("same");
}
