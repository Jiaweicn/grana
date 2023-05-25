#include <stdio.h>
#include <TFile.h>
#include <TTree.h>
#include <TF2.h>


TF2* f2;

struct xyab  {
	float X;
	float Y;
	float A;
	float B;
	float LA1;
	float RA1;
	float LA2;
	float RA2;
	float LT1;
	float RT1;
	float LT2;
	float RT2;
	float RF;
	float A_tar;
	float B_tar;
	float x_corr;
};

void create_f2()
{
	float p0 , p1 , p2 , p3 , p4 , p5 , p6 , p7 , p8 , p9 , p10, p11, p12,
	      p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24;
	p0                          =    -0.0170026 ;
	p1                          =  -0.000224363 ;
	p2                          =  -3.46683e-07 ;
	p3                          =   4.58973e-09 ;
	p4                          =   1.37185e-11 ;
	p5                          =     0.0348129 ;
	p6                          =  -0.000280421 ;
	p7                          =   1.04616e-06 ;
	p8                          =   -1.2865e-09 ;
	p9                          =  -1.05041e-11 ;
	p10                         =      0.439883 ;
	p11                         =   0.000553858 ;
	p12                         =   9.69785e-07 ;
	p13                         =  -1.15765e-08 ;
	p14                         =  -2.46523e-11 ;
	p15                         =  -0.000308052 ;
	p16                         =   4.99558e-06 ;
	p17                         =   7.32877e-08 ;
	p18                         =  -8.10408e-11 ;
	p19                         =   3.76338e-13 ;
	p20                         =   -0.00575184 ;
	p21                         =  -4.96487e-05 ;
	p22                         =  -4.60709e-08 ;
	p23                         =   1.65604e-09 ;
	p24                         =   5.13057e-12 ;

	f2 = new TF2("f2", "[0] + [1]*x +[2]*x*x + [3]*x*x*x +[4]*x*x*x*x + [5]*y + [6]*x*y +[7]*x*x*y + [8]*x*x*x*y +[9]*x*x*x*x*y  +  [10]*y*y + [11]*x*y*y +[12]*x*x*y*y + [13]*x*x*x*y*y +[14]*x*x*x*x*y*y  +  [15]*y*y*y + [16]*x*y*y*y +[17]*x*x*y*y*y + [18]*x*x*x*y*y*y +[19]*x*x*x*x*y*y*y   + [20]*y*y*y*y + [21]*x*y*y*y*y +[22]*x*x*y*y*y*y + [23]*x*x*x*y*y*y*y +[24]*x*x*x*x*y*y*y*y", -350, 300, -1.5, 1);
	f2->SetParameter(0 , p0 );
	f2->SetParameter(1 , p1 );
	f2->SetParameter(2 , p2 );
	f2->SetParameter(3 , p3 );
	f2->SetParameter(4 , p4 );
	f2->SetParameter(5 , p5 );
	f2->SetParameter(6 , p6 );
	f2->SetParameter(7 , p7 );
	f2->SetParameter(8 , p8 );
	f2->SetParameter(9 , p9 );
	f2->SetParameter(10, p10);
	f2->SetParameter(11, p11);
	f2->SetParameter(12, p12);
	f2->SetParameter(13, p13);
	f2->SetParameter(14, p14);
	f2->SetParameter(15, p15);
	f2->SetParameter(16, p16);
	f2->SetParameter(17, p17);
	f2->SetParameter(18, p18);
	f2->SetParameter(19, p19);
	f2->SetParameter(20, p20);
	f2->SetParameter(21, p21);
	f2->SetParameter(22, p22);
	f2->SetParameter(23, p23);
	f2->SetParameter(24, p24);

}
float get_delta_x(float x, float a)
{
	return f2->Eval(x,a);
}


int soft_corr_x(struct xyab* xyab)
{
	float x,b, delta_x;
	x = xyab->x_corr;
	b = xyab->B_tar;
	
	delta_x = get_delta_x(x,b);
	xyab->x_corr = x - delta_x;

	return 1;
}


int main(int argc, char* argv[])
{
	TFile* f_in, *f_out;
	TTree* t_in, *t_out;
	struct xyab xyab;
	int n_entry, i, n_entry1;
	
	if (argc != 3) {
		printf("usage: ./xxx <input root file> <output root file>\n");
		return -1;
	}

	f_in = new TFile(argv[1]);
	if (f_in->IsZombie()) {
		printf("canot open root file for input: %s\n", argv[1]);
		return -1;
	}
	t_in = (TTree*)f_in->Get("gr_tree_ang_softcorr");
	if (t_in == NULL) {
		printf("cannot get gr_tree\n");
		return -1;
	}
	t_in->SetBranchAddress("xyab_ang_softcorr", &xyab);

	f_out = new TFile(argv[2], "create");
	if (f_out->IsZombie()) {
		printf("cannot open root file for output: %s\n", argv[2]);
		return -1;
	}
	t_out = new TTree("gr_tree_ang_softcorr_all", "soft corrected in X");
	t_out->Branch("xyab_ang_softcorr_all", &xyab, "X/F:Y:A:B:LA1:RA1:LA2:RA2:LT1:RT1:LT2:RT2:RF:A_tar:B_tar:x_corr");
	
	n_entry = t_in->GetEntries();
	n_entry1 = 0;
	create_f2();
	for (i = 0; i < n_entry; i++) {
		if (i%50000 == 0)
			printf("finish: %.1f%%\n", 100.*i/n_entry);
		t_in->GetEntry(i);
		if (soft_corr_x(&xyab)) {
			t_out->Fill();
			n_entry1++;
		}
	}
	t_out->Write();
	f_out->Close();
	f_in->Close();

	printf("all finish, good_evt/all_evt: %d/%d = %.1f%%\n", 
			n_entry1, n_entry, 100.*n_entry1/n_entry);
	return 0;
}
