//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Dec  2 14:31:39 2011 by ROOT version 5.26/00b
// from TTree h700/XYAB
// found on file: run2003.root
//////////////////////////////////////////////////////////

#ifndef h700_h
#define h700_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <iostream>

class h700 {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Float_t         X;
   Float_t         Y;
   Float_t         A;
   Float_t         B;
   Float_t         La1;
   Float_t         Ra1;
   Float_t         La2;
   Float_t         Ra2;
   Float_t         Lt1;
   Float_t         Rt1;
   Float_t         Lt2;
   Float_t         Rt2;
   Float_t         Rf;

   // List of branches
   TBranch        *b_X;   //!
   TBranch        *b_Y;   //!
   TBranch        *b_A;   //!
   TBranch        *b_B;   //!
   TBranch        *b_La1;   //!
   TBranch        *b_Ra1;   //!
   TBranch        *b_La2;   //!
   TBranch        *b_Ra2;   //!
   TBranch        *b_Lt1;   //!
   TBranch        *b_Rt1;   //!
   TBranch        *b_Lt2;   //!
   TBranch        *b_Rt2;   //!
   TBranch        *b_Rf;   //!

   h700(TTree *tree=0);
   virtual ~h700();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef h700_cxx
h700::h700(TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   char fname[50];
   std::cout<<"Input root file name : ";
   std::cin>>fname;
   
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(fname);
      if (!f) {
         f = new TFile(fname);
      }
      tree = (TTree*)gDirectory->Get("h700");

   }
   Init(tree);
}

h700::~h700()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t h700::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t h700::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (!fChain->InheritsFrom(TChain::Class()))  return centry;
   TChain *chain = (TChain*)fChain;
   if (chain->GetTreeNumber() != fCurrent) {
      fCurrent = chain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void h700::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("X", &X, &b_X);
   fChain->SetBranchAddress("Y", &Y, &b_Y);
   fChain->SetBranchAddress("A", &A, &b_A);
   fChain->SetBranchAddress("B", &B, &b_B);
   fChain->SetBranchAddress("La1", &La1, &b_La1);
   fChain->SetBranchAddress("Ra1", &Ra1, &b_Ra1);
   fChain->SetBranchAddress("La2", &La2, &b_La2);
   fChain->SetBranchAddress("Ra2", &Ra2, &b_Ra2);
   fChain->SetBranchAddress("Lt1", &Lt1, &b_Lt1);
   fChain->SetBranchAddress("Rt1", &Rt1, &b_Rt1);
   fChain->SetBranchAddress("Lt2", &Lt2, &b_Lt2);
   fChain->SetBranchAddress("Rt2", &Rt2, &b_Rt2);
   fChain->SetBranchAddress("Rf", &Rf, &b_Rf);
   Notify();
}

Bool_t h700::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void h700::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t h700::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef h700_cxx
