#define h700_cxx
#include "h700.h"
#include <TH2.h>
#include <TNtuple.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "math.h"
#include <TFile.h>
#include <iostream>
#include <fstream>

#define N_ELE_XA 17
#define N_ELE_A 10
#define m1 22341.92265
#define m2 3728.40095
#define m3 3728.40095
#define m4 22341.92265
//#define ang3 4.5
#define T2 206
#define speed_of_light 299792458

// 'http://skisickness.com/2010/04/25/
// relativistic kinematics for the reaction, 1(2,3)4 

Double_t rele3(Double_t ex, Double_t a3) {
  a3 = a3 * 3.1415926 / 180.0;
  Double_t M4 = m4 + ex;
  Double_t s = pow(m1 + m2,2)  + 2 * m1 * T2;
  Double_t pcm =   sqrt((pow(s - m1*m1 - m2*m2,2) - 4 * m1*m1 * m2*m2) / 4 / s);
  Double_t kai = log((pcm + sqrt(m1*m1  + pcm*pcm)) / m1);
  Double_t SINH_KAI = (exp(kai) - exp(-kai)) / 2;
  Double_t COSH_KAI = (exp(kai) + exp(-kai)) / 2;
  Double_t pcm_p = sqrt((pow(s - m3*m3 - M4*M4,2) - 4 * m3*m3 * M4*M4) / 4 / s);
  Double_t p3a = sqrt(m3*m3 + pow(pcm_p,2)) * cos(a3) * SINH_KAI;
  Double_t p3b = COSH_KAI * sqrt((pow(pcm_p,2) - m3*m3 * pow(sin(a3),2) * pow(SINH_KAI,2)));
  Double_t p3c = 1 + pow(sin(a3),2) * pow(SINH_KAI,2);
  Double_t relp3 = (p3a + p3b) / p3c;
  return (sqrt( pow(relp3,2) + m3*m3) - m3);
}

Double_t relk1(Double_t ex, Double_t a3) {
  Double_t adep = 0.01;
  Double_t a3_lo = a3 - adep;
  Double_t e3_lo = rele3(ex, a3_lo);
  Double_t a3_hi = a3 + adep;
  Double_t e3_hi = rele3(ex, a3_hi);
  Double_t RELK1 = (e3_hi - e3_lo) / 2 / adep;
  return RELK1;
}

Double_t relk2(Double_t ex, Double_t a3) {
  Double_t adep = 0.01;
  Double_t e3_lo = rele3(ex, a3 - adep);
  Double_t e3_0 = rele3(ex, a3);
  Double_t e3_hi = rele3(ex, a3 + adep);
  Double_t RELK2 = (e3_hi + e3_lo - 2*e3_0) / adep /adep;
  return RELK2;
}


Double_t relex(Double_t p3, Double_t a3) {
  a3 = a3 * 3.1415926 / 180.0;
  Double_t p2 = sqrt((T2 + m2)*(T2+m2)  - m2*m2);
  Double_t E2 = T2 + m2;
  Double_t E3 = sqrt(m3*m3 + p3*p3);
  Double_t frelex = m2*m2 + m1*m1 + m3*m3;
  frelex = frelex + 2 * E2 * m1 - 2 * E3 * m1;
  frelex = frelex - 2 * E2 * E3 + 2 * p2 * p3 * cos(a3);
  frelex = sqrt(frelex)-m4;
  return frelex;
}

struct mat {
  int power;
  double element;
};

struct mat xamat[N_ELE_XA]={
  {0x0010, 100.0},
  {0x0001, 6.802887165},
  {0x0002, -1.014170333},
  {0x0003, 2.323257041},
  {0x0004,1.506853484},
  {0x0011,1.053168334},
  {0x0012,1.535695654},
  {0x0013,-0.435066545},
  {0x0014,-0.704371507},
  {0x0021,0.414679598},
  {0x0022,0.403897264},
  {0x0023,0.30809153},
  {0x0024,0.152327521},
  {0x0031,-0.0111186},
  {0x0032, 0.026275879},
  {0x0033,0.269717609},
  {0x0034,0.17050645}
};

void initXA() {
// read calibration file for xamat
   char fcalib[20];
   std::cout<<"Calibration file : ";
   std::cin>>fcalib;
   FILE* Fcalib=fopen(fcalib,"r");
   std::cout<<std::endl;
   for(int i=1;i<N_ELE_XA;i++) {
     fscanf(Fcalib,"%lf",&(xamat[i].element));
     printf("%x %10.8lf\n", xamat[i].power, xamat[i].element);
   }
   fclose(Fcalib);
}

Double_t getXcp(Double_t Xc, Double_t ac) {
  Double_t Xcp=0;
  //correct beam optics distortion
  for(int i=0;i<N_ELE_XA;i++){
    Xcp+=xamat[i].element*pow((Xc/100.0),(double)((xamat[i].power>>4)&0xf))
      *pow(ac,(double)(xamat[i].power&0xf));
    //    std::cout<<i<<" "<<Xcp<<" "<<xamat[i].power<<" "<<xamat[i].element<<std::endl;
  }
  return Xcp;
}

void h700::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L h700.C
//      Root > h700 t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;


//  amat[0][#] * (1 + amat[1][#] * X + amat[2][#] * X^2)
  struct mat amat[3][N_ELE_A]={
{
{	0x0000  , -0.144369116	},
{	0x0001	, 0.001726492	},
{	0x0002	, 4.00898E-05	},
{	0x0003	, -2.0521E-07	},
{	0x0010	, -0.451120807	},
{	0x0011	, -0.000134769	},
{	0x0012	, -1.61257E-06	},
{	0x0020	, -0.002659823	},
{	0x0021	, 7.55164E-05	},
{	0x0030	, 0.000808001	},
},			
{			
{	0x0000	, 0.000481683	},
{	0x0001	, -0.000692343	},
{	0x0002	, 0.002949834	},
{	0x0003	, -0.016809593	},
{	0x0010	, 2.03637E-05	},
{	0x0011	, 0.001687495	},
{	0x0012	, 0.003448503	},
{	0x0020	, 0.007498246	},
{	0x0021	, -0.00379668	},
{	0x0030	, -0.001340471	},
},			
{			
{	0x0000	, -3.2295E-06	},
{	0x0001	, -8.01126E-06	},
{	0x0002	, -4.42452E-06	},
{	0x0003	, -2.76519E-07	},
{	0x0010	, -5.8683E-07	},
{	0x0011	, -1.01101E-08	},
{	0x0012	, 1.96953E-07	},
{	0x0020	, -6.88339E-09	},
{	0x0021	, -7.57985E-08	},
{	0x0030	, -9.42964E-08	},
}
};

  struct mat ymat[4][N_ELE_A]={
{
{	0x0000	,	0.883847224	},
{	0x0001	,	0.099781394	},
{	0x0002	,	1.82016E-05	},
{	0x0003	,	1.16329E-06	},
{	0x0010	,	-0.00011099	},
{	0x0011	,	0.009828633	},
{	0x0012	,	2.91931E-05	},
{	0x0020	,	-4.5289E-05	},
{	0x0021	,	-7.0312E-06	},
{	0x0030	,	0.005174844	},
},
{				
{	0x0000	,	0.048235331	},
{	0x0001	,	0.18413027	},
{	0x0002	,	1.989345137	},
{	0x0003	,	-2.000184854	},
{	0x0010	,	242.6542384	},
{	0x0011	,	-0.207891394	},
{	0x0012	,	0.463853054	},
{	0x0020	,	132.1295148	},
{	0x0021	,	170.1788858	},
{	0x0030	,	0.307070507	},
},
{				
{	0x0000	,	-0.009497174	},
{	0x0001	,	0.025624575	},
{	0x0002	,	13.64273221	},
{	0x0003	,	9.966381792	},
{	0x0010	,	-109.5372676	},
{	0x0011	,	-0.541911663	},
{	0x0012	,	-1.80068115	},
{	0x0020	,	245.8007125	},
{	0x0021	,	154.176787	},
{	0x0030	,	-1.35412323	},
},
{				
{	0x0000	,	-0.030333031	},
{	0x0001	,	-0.068680943	},
{	0x0002	,	10.76130094	},
{	0x0003	,	13.36926695	},
{	0x0010	,	94.99239224	},
{	0x0011	,	-0.393844096	},
{	0x0012	,	-1.701971734	},
{	0x0020	,	133.1727362	},
{	0x0021	,	91.18770427	},
{	0x0030	,	-0.515385275	},
}
};


   gStyle->SetPalette(1);

// 2081: 4.5 deg: 0.98548 / 0.98547

   Double_t b_percent, ang3;
   std::cout<<"Input B field (in percentage) and angle (in deg) ";
   std::cin>>b_percent>>ang3;

   initXA();

   std::cout<<"ang3= "<<ang3<<" e3= "<<rele3(20.0, ang3)<<" k1="<<relk1(20.0,ang3)<<" k2="<<relk2(20.0,ang3)<<std::endl;
   std::cout<<"(-0.020467*0.5*ac*ac-0.092385*ac)/0.0237"<<std::endl;
   Double_t rk1=relk1(20.0,ang3)/relk1(20.0,4.5)*(-0.092385);
   Double_t rk2=relk2(20.0,ang3)/relk2(20.0,4.5)*(-0.020467);

   rk1=relk1(20.0,ang3);
   rk2=relk2(20.0,ang3);
   std::cout<<std::endl<<rk1<<" "<<rk2<<std::endl;

   TFile hfile("result.root","RECREATE","sorting result");

   TH2F* h2_ang = new TH2F("h2_ang","Phi vs. Theta",500,-3,3,500,-5,5);
   TH2F* h2_p_ang = new TH2F("h2_p_ang","p vs. ang",1024,-500,500,500,-5,5);
   TH2F* h2_Rfc_X = new TH2F("h2_Rfc_X","Rfc:X",1000,-500,500,500,0,1000);
   TNtuple *nt = new TNtuple("nt","GR event","X:Y:AC:YC:B:Pid1:RFc:XC:XCp:ex1:ex2:ex3:ex4:ex5");

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;

// correct A into Theta_target (ac)
      Double_t tmpA = (A-(X/50-1.77792)/4.00458-1.13);
      Double_t ac=0;
      Double_t yc=0;
      Double_t Xc=0;
      Double_t Xcp=0;

/*      int k=0;
      if( X>150 ) k=0;
      else if( X>0 && X<=150 ) k=1;
      else if( X>-150 && X<=0 ) k=2;
      else if( X<=-150 ) k=3;
*/
      
      for(int i=0;i<N_ELE_A;i++){
            ac+=(amat[0][i].element*(1+amat[1][i].element*X+amat[2][i].element*pow(X,2.0))
                 *pow(X,(double)(amat[0][i].power>>12))
                 *pow(B,(double)((amat[0][i].power>>8)&0xf))
                 *pow(tmpA,(double)((amat[0][i].power>>4)&0xf))
                 *pow(Y,(double)(amat[0][i].power&0xf)));

            /*yc+=(ymat[0][i].element
                 *pow(X,(double)(ymat[k][i].power>>12))
                 *pow(B,(double)((ymat[k][i].power>>8)&0xf))
                 *pow(tmpA,(double)((ymat[k][i].power>>4)&0xf))
                 *pow(Y,(double)(ymat[k][i].power&0xf)));
*/	
	}

	for(int i=0;i<N_ELE_A;i++){

		yc+=(ymat[0][i].element*(1+ymat[1][i].element*ac+ymat[2][i].element*pow(ac,2.0)+ymat[3][i].element*pow(ac,3.0))
                 	*pow(X,(double)(ymat[0][i].power>>12))
                 	*pow(B,(double)((ymat[0][i].power>>8)&0xf))
                 	*pow((X/100.0),(double)((ymat[0][i].power>>4)&0xf))
                 	*pow(Y,(double)(ymat[0][i].power&0xf)));
	}

//correction to remove kinematic effect
        Double_t xgain=(6.6387e-7*ang3*ang3+3.0385e-8*ang3+0.02371)*b_percent*b_percent;
// angle calibration based on run2074
        ac=1.0224*ac + 0.2323;
// angle re-calibration at 4.5 deg using 4He+1H line intercept 4He+24Mg lines
        xgain=xgain*(3.7487e-6*X/2.428e-2+1);
        Xc=X-(rk2*0.5*ac*ac+rk1*ac)/xgain;

// correct strange beam optictical distortion.
        Xcp=getXcp(Xc,ac);
// add kinematic shift back
	Xcp=Xcp+(rk2*0.5*ac*ac+rk1*ac)/xgain;


// coefficient: (11.8-9.35)/(-343.3+231.7)=0.02195 [2092 2.5 deg]
//     Xc=Xc-(1.15243*ac*ac+0.138395); // run2081 (fit line @-189.81, 12.8 MeV)
//       Xc=Xcp-(1.2762e+00*ac*ac + 9.6021e-02*ac);
//       Xc=X-(-0.020467*0.5*ac*ac-0.092385*ac)/0.0237;

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
       if( Xc<-142) {
	 brho=brho1;
       } else if(Xc<13) {
	   brho=brho2;
       } else {
	 brho=brho3;
       }
       Double_t brho5=b_percent*(-3.2598478e-05*pow(Xcp/100,2)-1.2713141e-02*(Xcp/100) + 2.0321236e+00);
   
        if(X>-600) {
           Float_t Rfc = Rf-0.044*X-24*ac;
	   h2_Rfc_X->Fill(X,Rfc);
           Double_t ex1=relex(brho1*2*speed_of_light/1e6,ang3+ac);
           Double_t ex2=relex(brho2*2*speed_of_light/1e6,ang3+ac);
           Double_t ex3=relex(brho3*2*speed_of_light/1e6,ang3+ac);
           Double_t ex4=relex(brho *2*speed_of_light/1e6,ang3+ac);
	   Double_t ex5=relex(brho5*2*speed_of_light/1e6,ang3+ac);
	   nt->Fill(X,Y,ac,yc,B,sqrt(La1*Ra1),Rfc,Xc,Xcp,ex1,ex2,ex3,ex4,ex5);

        if( sqrt(La1*Ra1)>220 && abs(yc)<5.0 && ((Rfc>776 && Rfc<830) || (Rfc>261 && Rfc<320) )) {
      //  if( sqrt(La1*Ra1)>220 && abs(yc)<5.0) {
                h2_ang->Fill(ac,yc);
          	if( abs(yc)<1.0 ) h2_p_ang->Fill(X,ac);
              }
         }

            // if (Cut(ientry) < 0) continue;
   }
   TCanvas *c1=new TCanvas("c1","",0,0,600,600);
   c1->SetFillColor(10);
   c1->Divide(1,2);
   c1->cd(1);
   h2_ang->Draw("col");
   c1->cd(2);
   h2_p_ang->Draw("col");

  // Save all objects in this file
  hfile.Write();

  // Close the file. Note that this is automatically done when you leave
  // the application.
  hfile.Close();   
}

/////////////////////////////////////////////////////////////////////////
// 
// Main program rootCERN.cxx to call CERNLIB functions
// 
/////////////////////////////////////////////////////////////////////////

#include "TROOT.h"

// This line is no longer needed in the current root
// TROOT root("Rint","The ROOT Interactive Interface");

//_______________________________________________________________________
int main(int argc, char **argv)
{

  h700 *h = new h700();
  h->Loop();
      
  return 0;

}
