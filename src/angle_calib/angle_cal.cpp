#include <stdio.h>
#include <TFile.h>
#include <TTree.h>


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
};


int angle_calib_pid_cut(struct xyab* xyab)
{
	float tmpA, tmpB, xx, yy, aa;
	int i,j,k;
	
	float a[] = {  1.43420124e-01,   9.86491913e-04,   3.38792702e-05,  -4.34841555e-01,
       8.05232908e-05,   3.91915793e-06,  -3.07240278e-03,  -8.08512889e-05,
      -7.16359063e-06,   2.08318135e-03,  -6.23958908e-07,   4.76829396e-08,
       2.28238156e-05,   7.86863667e-07,   1.79329295e-08,  -1.00397755e-05,
       1.64396406e-08,   9.47622414e-09,   4.98848582e-07,  -3.48094273e-09,
       1.39366035e-10,   1.34509728e-07,   3.19441745e-10,  -1.25120188e-10,
       1.65514583e-08,   1.34086052e-09,   9.30718658e-11};

	float b[] = { -7.17739612e-01,  -1.02222488e-01,  -4.86177429e-05,   1.94749924e-02,
       5.92849503e-03,  -7.37258559e-06,   6.78145272e-04,   4.95904055e-05,
       1.85955769e-05,  -2.16763465e-04,  -9.30851326e-05,  -2.08411824e-07,
      -2.09300022e-04,  -2.78665330e-05,  -4.27904356e-07,   4.38400009e-05,
       7.68987390e-06,   1.79916457e-07,   1.30824512e-06,   8.26755832e-08,
      -7.50620956e-10,  -7.14526497e-07,  -1.09908029e-07,  -1.30404607e-09,
       1.60151179e-08,   9.17502128e-09,   4.89660728e-10};
	
	if (xyab->X == -10000)
		return 0;
	if (xyab->LA2*xyab->RA2 < 25e3)
		return 0;

	tmpA = 0;
	tmpB = 0;
	xx = xyab->X;
	aa = xyab->A;
	yy = xyab->Y;
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			for (k = 0; k < 3; k++) {
				tmpA += a[i*9+j*3+k]*pow(xx,i)*pow(aa,j)*pow(yy,k);
				tmpB += b[i*9+j*3+k]*pow(xx,i)*pow(aa,j)*pow(yy,k);
			}
		}
	}
	xyab->A_tar = tmpA;
	xyab->B_tar = tmpB;

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
	t_in = (TTree*)f_in->Get("gr_tree");
	if (t_in == NULL) {
		printf("cannot get gr_tree\n");
		return -1;
	}
	t_in->SetBranchAddress("xyab", &xyab);

	f_out = new TFile(argv[2], "create");
	if (f_out->IsZombie()) {
		printf("cannot open root file for output: %s\n", argv[2]);
		return -1;
	}
	t_out = new TTree("gr_tree_ang", "angle calibrated");
	t_out->Branch("xyab_ang", &xyab, "X/F:Y:A:B:LA1:RA1:LA2:RA2:LT1:RT1:LT2:RT2:RF:A_tar:B_tar");
	
	n_entry = t_in->GetEntries();
	n_entry1 = 0;
	for (i = 0; i < n_entry; i++) {
		if (i%50000 == 0)
			printf("finish: %.1f%%\n", 100.*i/n_entry);
		t_in->GetEntry(i);
		if (angle_calib_pid_cut(&xyab)) {
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
