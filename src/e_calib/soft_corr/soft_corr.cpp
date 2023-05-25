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
	float x_corr;
};

float get_lab_ang(float a, float b)
{
	float a_c, b_c;
	a_c = 4.5-(a+0.4);
	b_c = b+0.2;

	a_c = a_c/180*3.14159;
	b_c = b_c/180*3.14159;
	return atan(sqrt(tan(a_c)*tan(a_c)+tan(b_c)*tan(b_c)))/3.1415926*180;
}

float get_delta_x(float theta)
{
	float p0, p1, p2;

	p0 = -8.7367;
	p1 = 0.0232143;
	p2 = 0.426339;

	return p0 + p1*theta + p2*theta*theta;
}


int soft_corr_x(struct xyab* xyab)
{
	float theta_lab, delta_x;
	
	theta_lab = get_lab_ang(xyab->A_tar, xyab->B_tar);
	delta_x = get_delta_x(theta_lab);
	xyab->x_corr = xyab->X - delta_x;

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
	t_in = (TTree*)f_in->Get("gr_tree_ang");
	if (t_in == NULL) {
		printf("cannot get gr_tree\n");
		return -1;
	}
	t_in->SetBranchAddress("xyab_ang", &xyab);

	f_out = new TFile(argv[2], "create");
	if (f_out->IsZombie()) {
		printf("cannot open root file for output: %s\n", argv[2]);
		return -1;
	}
	t_out = new TTree("gr_tree_ang_softcorr", "soft corrected in X");
	t_out->Branch("xyab_ang_softcorr", &xyab, "X/F:Y:A:B:LA1:RA1:LA2:RA2:LT1:RT1:LT2:RT2:RF:A_tar:B_tar:x_corr");
	
	n_entry = t_in->GetEntries();
	n_entry1 = 0;
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
