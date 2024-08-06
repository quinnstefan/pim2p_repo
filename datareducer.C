using namespace std;

//TLorentzVector mD = (0,0,0,1.875);

const double masspi = 0.13957;


void datareducer()

{cerr << "Hello world\n";
TFile *inputFile = new TFile("../flattree_pim2p_090215.root");
TTree *inputTree = (TTree*) inputFile->Get("gd_pimprotprotinc__B4_F4_T1_S4");
TFile *fout = new TFile("../myoutput.root","RECREATE");
TTree *outputTree = new TTree("T","Output of my test script");

float kin_chisq;
UInt_t kin_ndf;
ULong64_t event;
double CL;
TLorentzVector *p4_pim_kin = 0;
TLorentzVector *p4_pim_meas = 0;
TLorentzVector *p4_prot1_kin = 0;
TLorentzVector *p4_prot1_meas = 0;
TLorentzVector *p4_prot2_kin = 0;
TLorentzVector *p4_prot2_meas = 0;
TLorentzVector *p4_beam_kin = 0;
TLorentzVector *p4_beam_meas = 0;
double weight;

if (!inputTree) {
    cout << "Couldn't get the inputTree. Check spelling" << endl;
  }


inputTree->SetBranchAddress("kin_chisq", &kin_chisq);
inputTree->SetBranchAddress("kin_ndf", &kin_ndf);
inputTree->SetBranchAddress("event", &event);
inputTree->SetBranchAddress("pim_p4_kin", &p4_pim_kin);
inputTree->SetBranchAddress("pim_p4_meas", &p4_pim_meas);
inputTree->SetBranchAddress("p1_p4_kin", &p4_prot1_kin);
inputTree->SetBranchAddress("p1_p4_meas", &p4_prot1_meas);
inputTree->SetBranchAddress("p2_p4_kin", &p4_prot2_kin);
inputTree->SetBranchAddress("p2_p4_meas", &p4_prot2_meas);
inputTree->SetBranchAddress("beam_p4_kin", &p4_beam_kin);
inputTree->SetBranchAddress("beam_p4_meas", &p4_beam_meas);
inputTree->SetBranchAddress("accidweight", &weight);

//check where you need to put the /F
outputTree->Branch("kin_chisq", &kin_chisq, "kin_chisq/F");
outputTree->Branch("kin_ndf", &kin_ndf);
outputTree->Branch("event", &event);
outputTree->Branch("pim_p4_kin", &p4_pim_kin);
outputTree->Branch("pim_p4_meas", &p4_pim_meas);
outputTree->Branch("p1_p4_kin", &p4_prot1_kin);
outputTree->Branch("p1_p4_meas", &p4_prot1_meas);
outputTree->Branch("p2_p4_kin", &p4_prot2_kin);
outputTree->Branch("p2_p4_meas", &p4_prot2_meas);
outputTree->Branch("beam_p4_kin", &p4_beam_kin);
outputTree->Branch("beam_p4_meas", &p4_beam_meas);
outputTree->Branch("accidweight", &weight);




//////////DEFINING HISTOGRAMS//////////

//vector<TH1 *> hist_list;

//create directories
TDirectory *mainDir = fout->mkdir("Uncut");
TDirectory *subDir1 = fout->mkdir("Minus Balance around Zero");
TDirectory *subDir2 = fout->mkdir("Minus Balance around One");

TDirectory *DirXcut = fout->mkdir("After X Balance Cut");
TDirectory *Dirminus0_Xcut = fout->mkdir("Minus around zero after X cut");
TDirectory *Dirminus1_Xcut = fout->mkdir("Minus around one after X cut");

TDirectory *DirYcut = fout->mkdir("After Y Balance Cut");
TDirectory *Dirminus0_Ycut = fout->mkdir("Minus around zero after Y cut");
TDirectory *Dirminus1_Ycut = fout->mkdir("Minus around one after Y cut");

TDirectory *DirPluscut = fout->mkdir("After Plus Balance Cut");
TDirectory *Dirminus0_Pluscut = fout->mkdir("Minus around zero after plus cut");
TDirectory *Dirminus1_Pluscut = fout->mkdir("Minus around one after plus cut");

TDirectory *Dirrhocut = fout->mkdir("After Possible Rho cut");
TDirectory *Dirminus0_rhocut = fout->mkdir("Minus around zero after rho cut");
TDirectory *Dirminus1_rhocut = fout->mkdir("Minus around one after rho cut");

TDirectory *Dirminuscut = fout->mkdir("After Minus Balance Cut");
TDirectory *Dirminus0_minuscut = fout->mkdir("Minus around zero after minus cut");
TDirectory *Dirminus1_minuscut = fout->mkdir("Minus around one after minus cut");

//////////DEFINING HISTOGRAMS/////////

// original hist_list
vector<TH1 *> hist_list;

TH1D *h_CL_uncut = new TH1D("ConfidenceLevel_UNCUT","Confidence Level UNCUT;Confidence Level;Counts", 200, 0, 1);
  hist_list.push_back(h_CL_uncut);

TH1D *h_CL_cut = new TH1D("ConfidenceLevel_CUT","Confidence Level CUT;Confidence Level;Counts", 200, 0, 1);
  hist_list.push_back(h_CL_cut);

TH1D *h_EB_uncut = new TH1D("EnergyBalance_UNCUT","Energy Balance UNCUT;Energy Balance(F-I);Counts", 200, -10, 10);
  hist_list.push_back(h_EB_uncut);

TH1D *h_ZB_uncut = new TH1D("MomentumZBalance_UNCUT","Momentum Z Balance UNCUT;Energy Balance;Counts", 200, -10, 10);
  hist_list.push_back(h_ZB_uncut);

TH1D *h_YB_uncut = new TH1D("MomentumYBalance_UNCUT","Momentum Y Balance UNCUT;Energy Balance;Counts", 200, -10, 10);
  hist_list.push_back(h_YB_uncut);

TH1D *h_XB_uncut = new TH1D("MomentumXBalance_UNCUT","Momentum X Balance UNCUT;Energy Balance;Counts", 200, -10, 10);
  hist_list.push_back(h_XB_uncut);

TH1D *h_PlusBalance_uncut = new TH1D("PlusBalance_UNCUT","Plus Balance UNCUT;Energy Balance;Counts", 200, -10, 10);
  hist_list.push_back(h_PlusBalance_uncut);

TH1D *h_MinusBalance_uncut = new TH1D("MinusBalance_UNCUT","Minus Balance UNCUT;Energy Balance;Counts", 200, -10, 10);
  hist_list.push_back(h_MinusBalance_uncut);


//TH1D *h_MinusBalance_rhocut = new TH1D("MinusBalance_rhoCUT","Minus Balance Rho CUT;Energy Balance;Counts", 200, -10, 10);
//  hist_list.push_back(h_MinusBalance_rhocut);

TH2D *h_Proton1AngleVsMom = new TH2D("Proton1AngleVsMom", "Proton 1 Angle vs Momentum;Angle;Momentum;Counts", 200, 0, 180, 200, 0, 3);
  hist_list.push_back(h_Proton1AngleVsMom);

TH2D *h_Proton2AngleVsMom = new TH2D("Proton2AngleVsMom", "Proton 2 Angle vs Momentum;Angle;Momentum;Counts", 200, 0, 180, 200, 0, 3);
  hist_list.push_back(h_Proton2AngleVsMom);

TH2D *h_PiMinusAngleVsMom = new TH2D("PiMinusAngleVsMom", "Pi Minus Angle vs Momentum;Angle;Momentum;Counts", 200, 0, 180, 200, 0, 3);
  hist_list.push_back(h_PiMinusAngleVsMom);


TH1D *h_PossibleRho = new TH1D("PossibleRho", "Possible Rho Mass Distribution;Mass[GeV];Counts", 50, 0, 3);
  hist_list.push_back(h_PossibleRho);

TH1D *h_PiMinusMass = new TH1D("PiMinusMass", "Pi Minus Mass;Mass[GeV];Counts", 50, 0, 1);
  hist_list.push_back(h_PiMinusMass);

TH1D *h_PossibleRho_cut = new TH1D("PossibleRhoCut", "Possible Rho Cut;Mass[GeV];Counts", 50, 0, 3);
  hist_list.push_back(h_PossibleRho_cut);

TH1D *h_EB_cut = new TH1D("EnergyBalance_CUT","Energy Balance CUT;Energy Balance(F-I);Counts", 200, -10, 10);
  hist_list.push_back(h_EB_cut);

TH2D *h_PlusvsMinus = new TH2D("PlusvsMinus", "Plus Balance vs Minus Balance;Plus Balance;Minus Balance;Counts", 200, -10, 10, 200, -10, 10);
  hist_list.push_back(h_PlusvsMinus);

TH2D *h_MinusvsRho = new TH2D("MinusvsRho", "Minus Balance vs Possible Rho Mass; Minus Balance;Possible Rho Mass;Counts", 200, -10, 10, 50, 0, 3);
  hist_list.push_back(h_MinusvsRho);

for (int h = 0; h < hist_list.size();h++) {
	hist_list[h]->Sumw2();}

// this is when minus momentum is around zero
vector<TH1 *> hist_minus0;

TH1D *h_XB_minus0cut = new TH1D("MomentumXBalance_CUT0","Momentum X Balance Cut (Minus Balance Around 0);Momentum X Balance;Counts", 200, -10, 10);
  hist_minus0.push_back(h_XB_minus0cut);

TH1D *h_YB_minus0cut = new TH1D("MomentumYBalance_CUT0","Momentum Y Balance Cut (Minus Balance Around 0);Momentum Y Balance;Counts", 200, -10, 10);
  hist_minus0.push_back(h_YB_minus0cut);

TH1D *h_PlusBalance_0cut = new TH1D("PlusBalance_CUT0","Plus Balance CUT (Minus Balance around 0);Energy Balance;Counts", 200, -10, 10);
  hist_minus0.push_back(h_PlusBalance_0cut);

TH1D *h_PossibleRho_0cut = new TH1D("PossibleRho_CUT0","Possible Rho CUT (Minus Balance around 0);Mass[GeV];Counts", 50, 0, 3);
  hist_minus0.push_back(h_PossibleRho_0cut);

TH2D *h_PlusvsMinus_0cut = new TH2D("PlusvsMinus_CUT0", "Plus Balance vs Minus Balance (Minus Balance Around 0);Plus Balance;Minus Balance;Counts", 200, -10, 10, 200, -10, 10);
  hist_minus0.push_back(h_PlusvsMinus_0cut);

TH2D *h_MinusvsRho_0cut = new TH2D("MinusvsRho_CUT0", "Minus Balance vs Possible Rho Mass (Minus Balance Around 0); Minus Balance;Possible Rho Mass;Counts", 200, -10, 10, 50, 0, 3);
  hist_minus0.push_back(h_MinusvsRho_0cut);

for (int h=0; h < hist_minus0.size();h++) {
	hist_minus0[h]->Sumw2();}

//this is when minus momentum is around negative one
vector<TH1 *> hist_minus1;

TH1D *h_XB_minus1cut = new TH1D("MomentumXBalance_CUT1","Momentum X Balance Cut (Minus Balance Around 1);Momentum X Balance;Counts", 200, -10, 10);
  hist_minus1.push_back(h_XB_minus1cut);

TH1D *h_YB_minus1cut = new TH1D("MomentumYBalance_CUT1","Momentum Y Balance Cut (Minus Balance Around 1);Momentum Y Balance;Counts", 200, -10, 10);
  hist_minus1.push_back(h_YB_minus1cut);

TH1D *h_PlusBalance_1cut = new TH1D("PlusBalance_CUT1","Plus Balance CUT (Minus Balance around 1);Energy Balance;Counts", 200, -10, 10);
  hist_minus1.push_back(h_PlusBalance_1cut);

TH1D *h_PossibleRho_1cut = new TH1D("PossibleRho_CUT1","Possible Rho CUT (Minus Balance around 1);Mass[GeV];Counts", 50, 0, 3);
  hist_minus1.push_back(h_PossibleRho_1cut);

TH2D *h_PlusvsMinus_1cut = new TH2D("PlusvsMinus_CUT1", "Plus Balance vs Minus Balance (Minus Balance Around 1);Plus Balance;Minus Balance;Counts", 200, -10, 10, 200, -10, 10);
  hist_minus1.push_back(h_PlusvsMinus_1cut);

TH2D *h_MinusvsRho_1cut = new TH2D("MinusvsRho_CUT1", "Minus Balance vs Possible Rho Mass (Minus Balance Around 1); Minus Balance;Possible Rho Mass;Counts", 200, -10, 10, 50, 0, 3);
  hist_minus1.push_back(h_MinusvsRho_1cut);

for (int h = 0; h < hist_minus1.size();h++) {
	hist_minus1[h]->Sumw2();}

// after X balance cut 
vector<TH1 *> hist_Xcut;

TH1D *h_CL_Xcut = new TH1D("ConfidenceLevel_XCUT","Confidence Level After X Balance Cut;Confidence Level;Counts", 200, 0, 1);
  hist_Xcut.push_back(h_CL_Xcut);

TH1D *h_EB_Xcut = new TH1D("EnergyBalance_XCUT","Energy Balance After X Balance Cut;Energy Balance(F-I);Counts", 200, -10, 10);
  hist_Xcut.push_back(h_EB_Xcut);

TH1D *h_ZB_Xcut = new TH1D("MomentumZBalance_XCUT","Momentum Z Balance After X Balance Cut;Energy Balance;Counts", 200, -10, 10);
  hist_Xcut.push_back(h_ZB_Xcut);

TH1D *h_YB_Xcut = new TH1D("MomentumYBalance_XCUT","Momentum Y Balance After X Balance Cut;Energy Balance;Counts", 200, -10, 10);
  hist_Xcut.push_back(h_YB_Xcut);

TH1D *h_XB_Xcut = new TH1D("MomentumXBalance_XCUT","Momentum X Balance After X Balance Cut;Energy Balance;Counts", 200, -10, 10);
  hist_Xcut.push_back(h_XB_Xcut);

TH1D *h_PlusBalance_Xcut = new TH1D("PlusBalance_XCUT","Plus Balance After X Balance Cut;Energy Balance;Counts", 200, -10, 10);
  hist_Xcut.push_back(h_PlusBalance_Xcut);

TH1D *h_MinusBalance_Xcut = new TH1D("MinusBalance_XCUT","Minus Balance After X Balance Cut;Energy Balance;Counts", 200, -10, 10);
  hist_Xcut.push_back(h_MinusBalance_Xcut);

TH1D *h_PossibleRho_Xcut = new TH1D("PossibleRho_XCUT", "Possible Rho Mass Distribution After X Cut;Mass[GeV];Counts", 50, 0, 3);
  hist_Xcut.push_back(h_PossibleRho_Xcut);

TH2D *h_PlusvsMinus_Xcut = new TH2D("PlusvsMinus_Xcut", "Plus Balance vs Minus Balance After X Cut;Plus Balance;Minus Balance;Counts", 200, -10, 10, 200, -10, 10);
  hist_Xcut.push_back(h_PlusvsMinus_Xcut);

TH2D *h_MinusvsRho_Xcut = new TH2D("MinusvsRho_Xcut", "Minus Balance vs Possible Rho Mass After X Cut; Minus Balance;Possible Rho Mass;Counts", 200, -10, 10, 50, 0, 3);
  hist_Xcut.push_back(h_MinusvsRho_Xcut);

for (int h = 0; h < hist_Xcut.size();h++) {
	hist_Xcut[h]->Sumw2();}

// make minus balance cut histograms for here too

// this is when minus momentum is around zero (after X cut)
vector<TH1 *> hist_minus0_Xcut;

TH1D *h_XB_minus0cut_Xcut = new TH1D("MomentumXBalance_CUT0_Xcut","Momentum X Balance Cut (Minus Balance Around 0 After X cut);Momentum X Balance;Counts", 200, -10, 10);
  hist_minus0_Xcut.push_back(h_XB_minus0cut_Xcut);

TH1D *h_YB_minus0cut_Xcut = new TH1D("MomentumYBalance_CUT0_Xcut","Momentum Y Balance Cut (Minus Balance Around 0 After X cut);Momentum Y Balance;Counts", 200, -10, 10);
  hist_minus0_Xcut.push_back(h_YB_minus0cut_Xcut);

TH1D *h_PlusBalance_0cut_Xcut = new TH1D("PlusBalance_CUT0_Xcut","Plus Balance CUT (Minus Balance around 0 After X cut);Energy Balance;Counts", 200, -10, 10);
  hist_minus0_Xcut.push_back(h_PlusBalance_0cut_Xcut);

TH1D *h_PossibleRho_0cut_Xcut = new TH1D("PossibleRho_CUT0_Xcut","Possible Rho CUT (Minus Balance around 0 After X cut);Mass[GeV];Counts", 50, 0, 3);
  hist_minus0_Xcut.push_back(h_PossibleRho_0cut_Xcut);

for (int h=0; h < hist_minus0_Xcut.size();h++) {
	hist_minus0_Xcut[h]->Sumw2();}

//this is when minus momentum is around negative one (after X cut)
vector<TH1 *> hist_minus1_Xcut;

TH1D *h_XB_minus1cut_Xcut = new TH1D("MomentumXBalance_CUT1_Xcut","Momentum X Balance Cut (Minus Balance Around 1 After X cut);Momentum X Balance;Counts", 200, -10, 10);
  hist_minus1_Xcut.push_back(h_XB_minus1cut_Xcut);

TH1D *h_YB_minus1cut_Xcut = new TH1D("MomentumYBalance_CUT1_Xcut","Momentum Y Balance Cut (Minus Balance Around 1 After X cut);Momentum Y Balance;Counts", 200, -10, 10);
  hist_minus1_Xcut.push_back(h_YB_minus1cut_Xcut);

TH1D *h_PlusBalance_1cut_Xcut = new TH1D("PlusBalance_CUT1_Xcut","Plus Balance CUT (Minus Balance around 1 After X cut);Energy Balance;Counts", 200, -10, 10);
  hist_minus1_Xcut.push_back(h_PlusBalance_1cut_Xcut);

TH1D *h_PossibleRho_1cut_Xcut = new TH1D("PossibleRho_CUT1_Xcut","Possible Rho CUT (Minus Balance around 1 After X cut);Mass[GeV];Counts", 50, 0, 3);
  hist_minus1_Xcut.push_back(h_PossibleRho_1cut_Xcut);

for (int h = 0; h < hist_minus1_Xcut.size();h++) {
	hist_minus1_Xcut[h]->Sumw2();}


// after Y balance cut
vector<TH1 *> hist_Ycut;

TH1D *h_CL_Ycut = new TH1D("ConfidenceLevel_YCUT","Confidence Level After Y Balance Cut;Confidence Level;Counts", 200, 0, 1);
  hist_Ycut.push_back(h_CL_Ycut);

TH1D *h_EB_Ycut = new TH1D("EnergyBalance_YCUT","Energy Balance After Y Balance Cut;Energy Balance(F-I);Counts", 200, -10, 10);
  hist_Ycut.push_back(h_EB_Ycut);

TH1D *h_ZB_Ycut = new TH1D("MomentumZBalance_YCUT","Momentum Z Balance After Y Balance Cut;Energy Balance;Counts", 200, -10, 10);
  hist_Ycut.push_back(h_ZB_Ycut);

TH1D *h_YB_Ycut = new TH1D("MomentumYBalance_YCUT","Momentum Y Balance After Y Balance Cut;Energy Balance;Counts", 200, -10, 10);
  hist_Ycut.push_back(h_YB_Ycut);

TH1D *h_XB_Ycut = new TH1D("MomentumXBalance_YCUT","Momentum X Balance After Y Balance Cut;Energy Balance;Counts", 200, -10, 10);
  hist_Ycut.push_back(h_XB_Ycut);

TH1D *h_PlusBalance_Ycut = new TH1D("PlusBalance_YCUT","Plus Balance After Y Balance Cut;Energy Balance;Counts", 200, -10, 10);
  hist_Ycut.push_back(h_PlusBalance_Ycut);

TH1D *h_MinusBalance_Ycut = new TH1D("MinusBalance_YCUT","Minus Balance After Y Balance Cut;Energy Balance;Counts", 200, -10, 10);
  hist_Ycut.push_back(h_MinusBalance_Ycut);

TH1D *h_PossibleRho_Ycut = new TH1D("PossibleRho_YCUT", "Possible Rho Mass Distribution After Y Cut;Mass[GeV];Counts", 50, 0, 3);
  hist_Ycut.push_back(h_PossibleRho_Ycut);

TH2D *h_PlusvsMinus_Ycut = new TH2D("PlusvsMinus_Ycut", "Plus Balance vs Minus Balance After Y Cut;Plus Balance;Minus Balance;Counts", 200, -10, 10, 200, -10, 10);
  hist_Ycut.push_back(h_PlusvsMinus_Ycut);

TH2D *h_MinusvsRho_Ycut = new TH2D("MinusvsRho_Ycut", "Minus Balance vs Possible Rho Mass After Y Cut; Minus Balance;Possible Rho Mass;Counts", 200, -10, 10, 50, 0, 3);
  hist_Ycut.push_back(h_MinusvsRho_Ycut);

for (int h = 0; h < hist_Ycut.size();h++) {
	hist_Ycut[h]->Sumw2();}

// make minus balance cut histograms for here too
// this is when minus momentum is around zero (after Y cut)
vector<TH1 *> hist_minus0_Ycut;

TH1D *h_XB_minus0cut_Ycut = new TH1D("MomentumXBalance_CUT0_Ycut","Momentum X Balance Cut (Minus Balance Around 0 After Y cut);Momentum X Balance;Counts", 200, -10, 10);
  hist_minus0_Ycut.push_back(h_XB_minus0cut_Ycut);

TH1D *h_YB_minus0cut_Ycut = new TH1D("MomentumYBalance_CUT0_Ycut","Momentum Y Balance Cut (Minus Balance Around 0 After Y cut);Momentum Y Balance;Counts", 200, -10, 10);
  hist_minus0_Ycut.push_back(h_YB_minus0cut_Ycut);

TH1D *h_PlusBalance_0cut_Ycut = new TH1D("PlusBalance_CUT0_Ycut","Plus Balance CUT (Minus Balance around 0 After Y cut);Energy Balance;Counts", 200, -10, 10);
  hist_minus0_Ycut.push_back(h_PlusBalance_0cut_Ycut);

TH1D *h_PossibleRho_0cut_Ycut = new TH1D("PossibleRho_CUT0_Ycut","Possible Rho CUT (Minus Balance around 0 After Y cut);Mass[GeV];Counts", 50, 0, 3);
  hist_minus0_Ycut.push_back(h_PossibleRho_0cut_Ycut);

for (int h=0; h < hist_minus0_Ycut.size();h++) {
	hist_minus0_Ycut[h]->Sumw2();}

//this is when minus momentum is around negative one (after Y cut)
vector<TH1 *> hist_minus1_Ycut;

TH1D *h_XB_minus1cut_Ycut = new TH1D("MomentumXBalance_CUT1_Ycut","Momentum X Balance Cut (Minus Balance Around 1 After Y cut);Momentum X Balance;Counts", 200, -10, 10);
  hist_minus1_Ycut.push_back(h_XB_minus1cut_Ycut);

TH1D *h_YB_minus1cut_Ycut = new TH1D("MomentumYBalance_CUT1_Ycut","Momentum Y Balance Cut (Minus Balance Around 1 After Y cut);Momentum Y Balance;Counts", 200, -10, 10);
  hist_minus1_Ycut.push_back(h_YB_minus1cut_Ycut);

TH1D *h_PlusBalance_1cut_Ycut = new TH1D("PlusBalance_CUT1_Ycut","Plus Balance CUT (Minus Balance around 1 After Y cut);Energy Balance;Counts", 200, -10, 10);
  hist_minus1_Ycut.push_back(h_PlusBalance_1cut_Ycut);

TH1D *h_PossibleRho_1cut_Ycut = new TH1D("PossibleRho_CUT1_Ycut","Possible Rho CUT (Minus Balance around 1 After Y cut);Mass[GeV];Counts", 50, 0, 3);
  hist_minus1_Ycut.push_back(h_PossibleRho_1cut_Ycut);

for (int h = 0; h < hist_minus1_Ycut.size();h++) {
	hist_minus1_Ycut[h]->Sumw2();}



// after Plus balance cut
vector<TH1 *> hist_Pluscut;

TH1D *h_CL_Pluscut = new TH1D("ConfidenceLevel_PlusCUT","Confidence Level After Plus Balance Cut;Confidence Level;Counts", 200, 0, 1);
  hist_Pluscut.push_back(h_CL_Pluscut);

TH1D *h_EB_Pluscut = new TH1D("EnergyBalance_PlusCUT","Energy Balance After Plus Balance Cut;Energy Balance(F-I);Counts", 200, -10, 10);
  hist_Pluscut.push_back(h_EB_Pluscut);

TH1D *h_ZB_Pluscut = new TH1D("MomentumZBalance_PlusCUT","Momentum Z Balance After Plus Balance Cut;Energy Balance;Counts", 200, -10, 10);
  hist_Pluscut.push_back(h_ZB_Pluscut);

TH1D *h_YB_Pluscut = new TH1D("MomentumYBalance_PlusCUT","Momentum Y Balance After Plus Balance Cut;Energy Balance;Counts", 200, -10, 10);
  hist_Pluscut.push_back(h_YB_Pluscut);

TH1D *h_XB_Pluscut = new TH1D("MomentumXBalance_PlusCUT","Momentum X Balance After Plus Balance Cut;Energy Balance;Counts", 200, -10, 10);
  hist_Pluscut.push_back(h_XB_Pluscut);

TH1D *h_PlusBalance_Pluscut = new TH1D("PlusBalance_PlusCUT","Plus Balance After Plus Balance Cut;Energy Balance;Counts", 200, -10, 10);
  hist_Pluscut.push_back(h_PlusBalance_Pluscut);

TH1D *h_MinusBalance_Pluscut = new TH1D("MinusBalance_PlusCUT","Minus Balance After Plus Balance Cut;Energy Balance;Counts", 200, -10, 10);
  hist_Pluscut.push_back(h_MinusBalance_Pluscut);

TH1D *h_PossibleRho_Pluscut = new TH1D("PossibleRho_PlusCUT", "Possible Rho Mass Distribution After Plus Cut;Mass[GeV];Counts", 50, 0, 3);
  hist_Pluscut.push_back(h_PossibleRho_Pluscut);

TH2D *h_PlusvsMinus_Pluscut = new TH2D("PlusvsMinus_Pluscut", "Plus Balance vs Minus Balance After Plus Cut;Plus Balance;Minus Balance;Counts", 200, -10, 10, 200, -10, 10);
  hist_Pluscut.push_back(h_PlusvsMinus_Pluscut);

TH2D *h_MinusvsRho_Pluscut = new TH2D("MinusvsRho_Pluscut", "Minus Balance vs Possible Rho Mass After Plus Cut; Minus Balance;Possible Rho Mass;Counts", 200, -10, 10, 50, 0, 3);
  hist_Pluscut.push_back(h_MinusvsRho_Pluscut);

for (int h = 0; h < hist_Pluscut.size();h++) {
	hist_Pluscut[h]->Sumw2();}


// make minus balance cut histograms for here too
// this is when minus momentum is around zero (after Plus cut)
vector<TH1 *> hist_minus0_Pluscut;

TH1D *h_XB_minus0cut_Pluscut = new TH1D("MomentumXBalance_CUT0_Pluscut","Momentum X Balance Cut (Minus Balance Around 0 After Plus cut);Momentum X Balance;Counts", 200, -10, 10);
  hist_minus0_Pluscut.push_back(h_XB_minus0cut_Pluscut);

TH1D *h_YB_minus0cut_Pluscut = new TH1D("MomentumYBalance_CUT0_Pluscut","Momentum Y Balance Cut (Minus Balance Around 0 After Plus cut);Momentum Y Balance;Counts", 200, -10, 10);
  hist_minus0_Pluscut.push_back(h_YB_minus0cut_Pluscut);

TH1D *h_PlusBalance_0cut_Pluscut = new TH1D("PlusBalance_CUT0_Pluscut","Plus Balance CUT (Minus Balance around 0 After Plus cut);Energy Balance;Counts", 200, -10, 10);
  hist_minus0_Pluscut.push_back(h_PlusBalance_0cut_Pluscut);

TH1D *h_PossibleRho_0cut_Pluscut = new TH1D("PossibleRho_CUT0_Pluscut","Possible Rho CUT (Minus Balance around 0 After Plus cut);Mass[GeV];Counts", 50, 0, 3);
  hist_minus0_Pluscut.push_back(h_PossibleRho_0cut_Pluscut);

for (int h=0; h < hist_minus0_Pluscut.size();h++) {
	hist_minus0_Pluscut[h]->Sumw2();}

//this is when minus momentum is around negative one (after Plus cut)
vector<TH1 *> hist_minus1_Pluscut;

TH1D *h_XB_minus1cut_Pluscut = new TH1D("MomentumXBalance_CUT1_Pluscut","Momentum X Balance Cut (Minus Balance Around 1 After Plus cut);Momentum X Balance;Counts", 200, -10, 10);
  hist_minus1_Pluscut.push_back(h_XB_minus1cut_Pluscut);

TH1D *h_YB_minus1cut_Pluscut = new TH1D("MomentumYBalance_CUT1_Pluscut","Momentum Y Balance Cut (Minus Balance Around 1 After Plus cut);Momentum Y Balance;Counts", 200, -10, 10);
  hist_minus1_Pluscut.push_back(h_YB_minus1cut_Pluscut);

TH1D *h_PlusBalance_1cut_Pluscut = new TH1D("PlusBalance_CUT1_Pluscut","Plus Balance CUT (Minus Balance around 1 After Plus cut);Energy Balance;Counts", 200, -10, 10);
  hist_minus1_Pluscut.push_back(h_PlusBalance_1cut_Pluscut);

TH1D *h_PossibleRho_1cut_Pluscut = new TH1D("PossibleRho_CUT1_Pluscut","Possible Rho CUT (Minus Balance around 1 After Plus cut);Mass[GeV];Counts", 50, 0, 3);
  hist_minus1_Pluscut.push_back(h_PossibleRho_1cut_Pluscut);

for (int h = 0; h < hist_minus1_Pluscut.size();h++) {
	hist_minus1_Pluscut[h]->Sumw2();}

// after Possible Rho cut
vector<TH1 *> hist_rhocut;

TH1D *h_CL_rhocut = new TH1D("ConfidenceLevel_rhoCUT","Confidence Level After Possible Rho Cut;Confidence Level;Counts", 200, 0, 1);
  hist_rhocut.push_back(h_CL_rhocut);

TH1D *h_EB_rhocut = new TH1D("EnergyBalance_rhoCUT","Energy Balance After Possible Rho Cut;Energy Balance(F-I);Counts", 200, -10, 10);
  hist_rhocut.push_back(h_EB_rhocut);

TH1D *h_ZB_rhocut = new TH1D("MomentumZBalance_rhoCUT","Momentum Z Balance After Possible Rho Cut;Energy Balance;Counts", 200, -10, 10);
  hist_rhocut.push_back(h_ZB_rhocut);

TH1D *h_YB_rhocut = new TH1D("MomentumYBalance_rhoCUT","Momentum Y Balance After Possible Rho Cut;Energy Balance;Counts", 200, -10, 10);
  hist_rhocut.push_back(h_YB_rhocut);

TH1D *h_XB_rhocut = new TH1D("MomentumXBalance_rhoCUT","Momentum X Balance After Possible Rho Cut;Energy Balance;Counts", 200, -10, 10);
  hist_rhocut.push_back(h_XB_rhocut);

TH1D *h_PlusBalance_rhocut = new TH1D("PlusBalance_rhoCUT","Plus Balance After Possible Rho Cut;Energy Balance;Counts", 200, -10, 10);
  hist_rhocut.push_back(h_PlusBalance_rhocut);

TH1D *h_MinusBalance_rhocut = new TH1D("MinusBalance_rhoCUT","Minus Balance After Possible Rho Cut;Energy Balance;Counts", 200, -10, 10);
  hist_rhocut.push_back(h_MinusBalance_rhocut);

TH1D *h_PossibleRho_rhocut = new TH1D("PossibleRho_rhoCUT", "Possible Rho Mass Distribution After Possible Rho Cut;Mass[GeV];Counts", 50, 0, 3);
  hist_rhocut.push_back(h_PossibleRho_rhocut);

TH2D *h_PlusvsMinus_rhocut = new TH2D("PlusvsMinus_rhocut", "Plus Balance vs Minus Balance After Possible Rho Cut;Plus Balance;Minus Balance;Counts", 200, -10, 10, 200, -10, 10);
  hist_rhocut.push_back(h_PlusvsMinus_rhocut);

TH2D *h_MinusvsRho_rhocut = new TH2D("MinusvsRho_rhocut", "Minus Balance vs Possible Rho Mass After Rho Cut; Minus Balance;Possible Rho Mass;Counts", 200, -10, 10, 50, 0, 3);
  hist_rhocut.push_back(h_MinusvsRho_rhocut);

for (int h = 0; h < hist_rhocut.size();h++) {
	hist_rhocut[h]->Sumw2();}

// this is when minus momentum is around zero (after rho cut)
vector<TH1 *> hist_minus0_rhocut;

TH1D *h_XB_minus0cut_rhocut = new TH1D("MomentumXBalance_CUT0_rhocut","Momentum X Balance Cut (Minus Balance Around 0 After rho cut);Momentum X Balance;Counts", 200, -10, 10);
  hist_minus0_rhocut.push_back(h_XB_minus0cut_rhocut);

TH1D *h_YB_minus0cut_rhocut = new TH1D("MomentumYBalance_CUT0_rhocut","Momentum Y Balance Cut (Minus Balance Around 0 After rho cut);Momentum Y Balance;Counts", 200, -10, 10);
  hist_minus0_rhocut.push_back(h_YB_minus0cut_rhocut);

TH1D *h_PlusBalance_0cut_rhocut = new TH1D("PlusBalance_CUT0_rhocut","Plus Balance CUT (Minus Balance around 0 After rho cut);Energy Balance;Counts", 200, -10, 10);
  hist_minus0_rhocut.push_back(h_PlusBalance_0cut_rhocut);

TH1D *h_PossibleRho_0cut_rhocut = new TH1D("PossibleRho_CUT0_rhocut","Possible Rho CUT (Minus Balance around 0 After rho cut);Mass[GeV];Counts", 50, 0, 3);
  hist_minus0_rhocut.push_back(h_PossibleRho_0cut_rhocut);

for (int h=0; h < hist_minus0_rhocut.size();h++) {
	hist_minus0_rhocut[h]->Sumw2();}

//this is when minus momentum is around negative one (after rho cut)
vector<TH1 *> hist_minus1_rhocut;

TH1D *h_XB_minus1cut_rhocut = new TH1D("MomentumXBalance_CUT1_rhocut","Momentum X Balance Cut (Minus Balance Around 1 After rho cut);Momentum X Balance;Counts", 200, -10, 10);
  hist_minus1_rhocut.push_back(h_XB_minus1cut_rhocut);

TH1D *h_YB_minus1cut_rhocut = new TH1D("MomentumYBalance_CUT1_rhocut","Momentum Y Balance Cut (Minus Balance Around 1 After rho cut);Momentum Y Balance;Counts", 200, -10, 10);
  hist_minus1_rhocut.push_back(h_YB_minus1cut_rhocut);

TH1D *h_PlusBalance_1cut_rhocut = new TH1D("PlusBalance_CUT1_rhocut","Plus Balance CUT (Minus Balance around 1 After rho cut);Energy Balance;Counts", 200, -10, 10);
  hist_minus1_rhocut.push_back(h_PlusBalance_1cut_rhocut);

TH1D *h_PossibleRho_1cut_rhocut = new TH1D("PossibleRho_CUT1_rhocut","Possible Rho CUT (Minus Balance around 1 After rho cut);Mass[GeV];Counts", 50, 0, 3);
  hist_minus1_rhocut.push_back(h_PossibleRho_1cut_rhocut);

for (int h = 0; h < hist_minus1_rhocut.size();h++) {
	hist_minus1_rhocut[h]->Sumw2();}

//after minus balance cut
vector<TH1 *> hist_minuscut;

TH1D *h_CL_minuscut = new TH1D("ConfidenceLevel_minusCUT","Confidence Level After Minus Balance Cut;Confidence Level;Counts", 200, 0, 1);
  hist_minuscut.push_back(h_CL_minuscut);

TH1D *h_EB_minuscut = new TH1D("EnergyBalance_minusCUT","Energy Balance After Minus Balance Cut;Energy Balance(F-I);Counts", 200, -10, 10);
  hist_minuscut.push_back(h_EB_minuscut);

TH1D *h_ZB_minuscut = new TH1D("MomentumZBalance_minusCUT","Momentum Z Balance After Minus Balance Cut;Energy Balance;Counts", 200, -10, 10);
  hist_minuscut.push_back(h_ZB_minuscut);

TH1D *h_YB_minuscut = new TH1D("MomentumYBalance_minusCUT","Momentum Y Balance After Minus Balance Cut;Energy Balance;Counts", 200, -10, 10);
  hist_minuscut.push_back(h_YB_minuscut);

TH1D *h_XB_minuscut = new TH1D("MomentumXBalance_minusCUT","Momentum X Balance After Minus Balance Cut;Energy Balance;Counts", 200, -10, 10);
  hist_minuscut.push_back(h_XB_minuscut);

TH1D *h_PlusBalance_minuscut = new TH1D("PlusBalance_minusCUT","Plus Balance After Minus Balance Cut;Energy Balance;Counts", 200, -10, 10);
  hist_minuscut.push_back(h_PlusBalance_minuscut);

TH1D *h_MinusBalance_minuscut = new TH1D("MinusBalance_minusCUT","Minus Balance After Minus Balance Cut;Energy Balance;Counts", 200, -10, 10);
  hist_minuscut.push_back(h_MinusBalance_minuscut);

TH1D *h_PossibleRho_minuscut = new TH1D("PossibleRho_minusCUT", "Possible Rho Mass Distribution After Minus Balance Cut;Mass[GeV];Counts", 50, 0, 3);
  hist_minuscut.push_back(h_PossibleRho_minuscut);

TH2D *h_PlusvsMinus_minuscut = new TH2D("PlusvsMinus_minuscut", "Plus Balance vs Minus Balance After Minus Balance Cut;Plus Balance;Minus Balance;Counts", 200, -10, 10, 200, -10, 10);
  hist_minuscut.push_back(h_PlusvsMinus_minuscut);

TH2D *h_MinusvsRho_minuscut = new TH2D("MinusvsRho_minuscut", "Minus Balance vs Possible Rho Mass After Minus Cut; Minus Balance;Possible Rho Mass;Counts", 200, -10, 10, 50, 0, 3);
  hist_minuscut.push_back(h_MinusvsRho_minuscut);

for (int h = 0; h < hist_minuscut.size();h++) {
	hist_minuscut[h]->Sumw2();}

// this is when minus momentum is around zero (after minus cut)
vector<TH1 *> hist_minus0_minuscut;

TH1D *h_XB_minus0cut_minuscut = new TH1D("MomentumXBalance_CUT0_minuscut","Momentum X Balance Cut (Minus Balance Around 0 After minus cut);Momentum X Balance;Counts", 200, -10, 10);
  hist_minus0_minuscut.push_back(h_XB_minus0cut_minuscut);

TH1D *h_YB_minus0cut_minuscut = new TH1D("MomentumYBalance_CUT0_minuscut","Momentum Y Balance Cut (Minus Balance Around 0 After minus cut);Momentum Y Balance;Counts", 200, -10, 10);
  hist_minus0_minuscut.push_back(h_YB_minus0cut_minuscut);

TH1D *h_PlusBalance_0cut_minuscut = new TH1D("PlusBalance_CUT0_minuscut","Plus Balance CUT (Minus Balance around 0 After minus cut);Energy Balance;Counts", 200, -10, 10);
  hist_minus0_minuscut.push_back(h_PlusBalance_0cut_minuscut);

TH1D *h_PossibleRho_0cut_minuscut = new TH1D("PossibleRho_CUT0_minuscut","Possible Rho CUT (Minus Balance around 0 After minus cut);Mass[GeV];Counts", 50, 0, 3);
  hist_minus0_minuscut.push_back(h_PossibleRho_0cut_minuscut);

for (int h=0; h < hist_minus0_minuscut.size();h++) {
	hist_minus0_minuscut[h]->Sumw2();}

//this is when minus momentum is around negative one (after minus cut)
vector<TH1 *> hist_minus1_minuscut;

TH1D *h_XB_minus1cut_minuscut = new TH1D("MomentumXBalance_CUT1_minuscut","Momentum X Balance Cut (Minus Balance Around 1 After minus cut);Momentum X Balance;Counts", 200, -10, 10);
  hist_minus1_minuscut.push_back(h_XB_minus1cut_minuscut);

TH1D *h_YB_minus1cut_minuscut = new TH1D("MomentumYBalance_CUT1_minuscut","Momentum Y Balance Cut (Minus Balance Around 1 After minus cut);Momentum Y Balance;Counts", 200, -10, 10);
  hist_minus1_minuscut.push_back(h_YB_minus1cut_minuscut);

TH1D *h_PlusBalance_1cut_minuscut = new TH1D("PlusBalance_CUT1_minuscut","Plus Balance CUT (Minus Balance around 1 After minus cut);Energy Balance;Counts", 200, -10, 10);
  hist_minus1_minuscut.push_back(h_PlusBalance_1cut_minuscut);

TH1D *h_PossibleRho_1cut_minuscut = new TH1D("PossibleRho_CUT1_minuscut","Possible Rho CUT (Minus Balance around 1 After minus cut);Mass[GeV];Counts", 50, 0, 3);
  hist_minus1_minuscut.push_back(h_PossibleRho_1cut_minuscut);

for (int h = 0; h < hist_minus1_minuscut.size();h++) {
	hist_minus1_minuscut[h]->Sumw2();}


/////////EVENT LOOP//////////
for (int event = 0; event < inputTree->GetEntries(); event++) {

    if (event % 10000 == 0)
      cerr << "Working on event " << event << "\n";

    inputTree->GetEvent(event);


    TLorentzVector EB = ((*p4_pim_kin + *p4_prot1_kin + *p4_prot2_kin) - (*p4_beam_kin + TLorentzVector(0,0,0,1.875)));

    h_CL_uncut->Fill(TMath::Prob(kin_chisq, kin_ndf), weight);
    h_EB_uncut->Fill(EB.E(),weight);
    h_EB_uncut->Fill(EB.E(),weight);
    h_ZB_uncut->Fill(EB.Z(),weight);
    h_YB_uncut->Fill(EB.Y(),weight);
    h_XB_uncut->Fill(EB.X(),weight);
    h_PlusBalance_uncut->Fill(EB.E()+EB.Z(),weight);
    h_MinusBalance_uncut->Fill(EB.E()-EB.Z(),weight);
    h_Proton1AngleVsMom->Fill(p4_prot1_kin->Theta() * 180/M_PI,p4_prot1_kin->Vect().Mag(),weight);
    h_Proton2AngleVsMom->Fill(p4_prot2_kin->Theta() * 180/M_PI,p4_prot2_kin->Vect().Mag(),weight);    
    h_PiMinusAngleVsMom->Fill(p4_pim_kin->Theta() * 180/M_PI,p4_pim_kin->Vect().Mag(),weight);

// possible rho
    TLorentzVector p4_protfast_kin, p4_protslow_kin;
    if (p4_prot1_kin->Vect().Mag()>p4_prot2_kin->Vect().Mag()){
	p4_protfast_kin = *p4_prot1_kin;
	p4_protslow_kin = *p4_prot2_kin; }
    else{
	p4_protfast_kin = *p4_prot2_kin;
	p4_protslow_kin = *p4_prot2_kin;}

    TLorentzVector possible_piplus;
    possible_piplus.SetVectM(p4_protfast_kin.Vect(), masspi);

    TLorentzVector possible_rho = possible_piplus + *p4_pim_kin;

    h_PossibleRho->Fill(possible_rho.M(),weight);
    h_PiMinusMass->Fill(p4_pim_kin->M(),weight);


    h_PlusvsMinus->Fill(EB.E()+EB.Z(),EB.E()-EB.Z(),weight);
    h_MinusvsRho->Fill(EB.E()-EB.Z(),possible_rho.M(),weight);


// when minus balance is around zero
    if (((EB.E()-EB.Z()) > -0.2) and ((EB.E()-EB.Z()) < 0.2)){ 
    h_XB_minus0cut->Fill(EB.X(),weight);
    h_YB_minus0cut->Fill(EB.Y(),weight);
    h_PlusBalance_0cut->Fill(EB.E()+EB.Z(),weight);  	
    h_PossibleRho_0cut->Fill(possible_rho.M(),weight);  	
    h_PlusvsMinus_0cut->Fill(EB.E()+EB.Z(),EB.E()-EB.Z(),weight);
    h_MinusvsRho_0cut->Fill(EB.E()-EB.Z(),possible_rho.M(),weight);}

// when minus balance is around -1
    if (((EB.E()-EB.Z()) > -1.2) and ((EB.E()-EB.Z()) < -0.8)){
    h_XB_minus1cut->Fill(EB.X(),weight);
    h_YB_minus1cut->Fill(EB.Y(),weight);
    h_PlusBalance_1cut->Fill(EB.E()+EB.Z(),weight); 
    h_PossibleRho_1cut->Fill(possible_rho.M(),weight);  	
    h_PlusvsMinus_1cut->Fill(EB.E()+EB.Z(),EB.E()-EB.Z(),weight);
    h_MinusvsRho_1cut->Fill(EB.E()-EB.Z(),possible_rho.M(),weight);}


// X balance cut 
    if (EB.X() > 1.3)
	continue;

    if (EB.X() < -1.3)
	continue;

    h_CL_Xcut->Fill(TMath::Prob(kin_chisq, kin_ndf), weight);
    h_EB_Xcut->Fill(EB.E(),weight);
    h_ZB_Xcut->Fill(EB.Z(),weight);
    h_YB_Xcut->Fill(EB.Y(),weight);
    h_XB_Xcut->Fill(EB.X(),weight);
    h_PlusBalance_Xcut->Fill(EB.E()+EB.Z(),weight);
    h_MinusBalance_Xcut->Fill(EB.E()-EB.Z(),weight);
    h_PossibleRho_Xcut->Fill(possible_rho.M(),weight);

    h_PlusvsMinus_Xcut->Fill(EB.E()+EB.Z(),EB.E()-EB.Z(),weight);
    h_MinusvsRho_Xcut->Fill(EB.E()-EB.Z(),possible_rho.M(),weight);

// when minus balance is around zero (after X cut)
    if (((EB.E()-EB.Z()) > -0.2) and ((EB.E()-EB.Z()) < 0.2)){ 
    h_XB_minus0cut_Xcut->Fill(EB.X(),weight);
    h_YB_minus0cut_Xcut->Fill(EB.Y(),weight);
    h_PlusBalance_0cut_Xcut->Fill(EB.E()+EB.Z(),weight); 	
    h_PossibleRho_0cut_Xcut->Fill(possible_rho.M(),weight);  }	

// when minus balance is around -1 (after X cut)
    if (((EB.E()-EB.Z()) > -1.2) and ((EB.E()-EB.Z()) < -0.8)){
    h_XB_minus1cut_Xcut->Fill(EB.X(),weight);
    h_YB_minus1cut_Xcut->Fill(EB.Y(),weight);
    h_PlusBalance_1cut_Xcut->Fill(EB.E()+EB.Z(),weight); 
    h_PossibleRho_1cut_Xcut->Fill(possible_rho.M(),weight);  }	


// Y balance cut
    if (EB.Y() > 1.3)
	continue;

    if (EB.Y() < -1.3)
	continue;

    h_CL_Ycut->Fill(TMath::Prob(kin_chisq, kin_ndf), weight);
    h_EB_Ycut->Fill(EB.E(),weight);
    h_ZB_Ycut->Fill(EB.Z(),weight);
    h_YB_Ycut->Fill(EB.Y(),weight);
    h_XB_Ycut->Fill(EB.X(),weight);
    h_PlusBalance_Ycut->Fill(EB.E()+EB.Z(),weight);
    h_MinusBalance_Ycut->Fill(EB.E()-EB.Z(),weight);
    h_PossibleRho_Ycut->Fill(possible_rho.M(),weight);

    h_PlusvsMinus_Ycut->Fill(EB.E()+EB.Z(),EB.E()-EB.Z(),weight);
    h_MinusvsRho_Ycut->Fill(EB.E()-EB.Z(),possible_rho.M(),weight);

// when minus balance is around zero (after Y cut)
    if (((EB.E()-EB.Z()) > -0.2) and ((EB.E()-EB.Z()) < 0.2)){ 
    h_XB_minus0cut_Ycut->Fill(EB.X(),weight);
    h_YB_minus0cut_Ycut->Fill(EB.Y(),weight);
    h_PlusBalance_0cut_Ycut->Fill(EB.E()+EB.Z(),weight); 	
    h_PossibleRho_0cut_Ycut->Fill(possible_rho.M(),weight);  }	

// when minus balance is around -1 (after Y cut)
    if (((EB.E()-EB.Z()) > -1.2) and ((EB.E()-EB.Z()) < -0.8)){
    h_XB_minus1cut_Ycut->Fill(EB.X(),weight);
    h_YB_minus1cut_Ycut->Fill(EB.Y(),weight);
    h_PlusBalance_1cut_Ycut->Fill(EB.E()+EB.Z(),weight); 
    h_PossibleRho_1cut_Ycut->Fill(possible_rho.M(),weight);  }	


// Plus balance cut
    if ((EB.E()+EB.Z()) > 3)
	continue;

    if ((EB.E()+EB.Z()) < -3)
	continue;

    h_CL_Pluscut->Fill(TMath::Prob(kin_chisq, kin_ndf), weight);
    h_EB_Pluscut->Fill(EB.E(),weight);
    h_ZB_Pluscut->Fill(EB.Z(),weight);
    h_YB_Pluscut->Fill(EB.Y(),weight);
    h_XB_Pluscut->Fill(EB.X(),weight);
    h_PlusBalance_Pluscut->Fill(EB.E()+EB.Z(),weight);
    h_MinusBalance_Pluscut->Fill(EB.E()-EB.Z(),weight);
    h_PossibleRho_Pluscut->Fill(possible_rho.M(),weight);

    h_PlusvsMinus_Pluscut->Fill(EB.E()+EB.Z(),EB.E()-EB.Z(),weight);
    h_MinusvsRho_Pluscut->Fill(EB.E()-EB.Z(),possible_rho.M(),weight);

// when minus balance is around zero (after Plus cut)
    if (((EB.E()-EB.Z()) > -0.2) and ((EB.E()-EB.Z()) < 0.2)){ 
    h_XB_minus0cut_Pluscut->Fill(EB.X(),weight);
    h_YB_minus0cut_Pluscut->Fill(EB.Y(),weight);
    h_PlusBalance_0cut_Pluscut->Fill(EB.E()+EB.Z(),weight); 	
    h_PossibleRho_0cut_Pluscut->Fill(possible_rho.M(),weight);  }	

// when minus balance is around -1 (after Plus cut)
    if (((EB.E()-EB.Z()) > -1.2) and ((EB.E()-EB.Z()) < -0.8)){
    h_XB_minus1cut_Pluscut->Fill(EB.X(),weight);
    h_YB_minus1cut_Pluscut->Fill(EB.Y(),weight);
    h_PlusBalance_1cut_Pluscut->Fill(EB.E()+EB.Z(),weight); 
    h_PossibleRho_1cut_Pluscut->Fill(possible_rho.M(),weight);  }	

   // if (TMath::Prob(kin_chisq, kin_ndf) < 0.1)
     // continue;
   
   // h_CL_cut->Fill(TMath::Prob(kin_chisq, kin_ndf), weight);

/////////commenting out rho cut for now/////////
   
//    if (possible_rho.M() < 1)
//	continue;
    
//    h_EB_cut->Fill(EB.E(),weight);
//    h_PossibleRho_cut->Fill(possible_rho.M(),weight);

//    h_MinusBalance_rhocut->Fill(EB.E()-EB.Z(),weight);

// rho cut
    if (possible_rho.M() < 1)
	continue;

    h_CL_rhocut->Fill(TMath::Prob(kin_chisq, kin_ndf), weight);
    h_EB_rhocut->Fill(EB.E(),weight);
    h_ZB_rhocut->Fill(EB.Z(),weight);
    h_YB_rhocut->Fill(EB.Y(),weight);
    h_XB_rhocut->Fill(EB.X(),weight);
    h_PlusBalance_rhocut->Fill(EB.E()+EB.Z(),weight);
    h_MinusBalance_rhocut->Fill(EB.E()-EB.Z(),weight);
    h_PossibleRho_rhocut->Fill(possible_rho.M(),weight);
    
    h_PlusvsMinus_rhocut->Fill(EB.E()+EB.Z(),EB.E()-EB.Z(),weight);
    h_MinusvsRho_rhocut->Fill(EB.E()-EB.Z(),possible_rho.M(),weight);

// when minus balance is around zero (after rho cut)
    if (((EB.E()-EB.Z()) > -0.2) and ((EB.E()-EB.Z()) < 0.2)){ 
    h_XB_minus0cut_rhocut->Fill(EB.X(),weight);
    h_YB_minus0cut_rhocut->Fill(EB.Y(),weight);
    h_PlusBalance_0cut_rhocut->Fill(EB.E()+EB.Z(),weight); 	
    h_PossibleRho_0cut_rhocut->Fill(possible_rho.M(),weight);  }	

// when minus balance is around -1 (after rho cut)
    if (((EB.E()-EB.Z()) > -1.2) and ((EB.E()-EB.Z()) < -0.8)){
    h_XB_minus1cut_rhocut->Fill(EB.X(),weight);
    h_YB_minus1cut_rhocut->Fill(EB.Y(),weight);
    h_PlusBalance_1cut_rhocut->Fill(EB.E()+EB.Z(),weight); 
    h_PossibleRho_1cut_rhocut->Fill(possible_rho.M(),weight);  }


// minus balance cut
    if ((EB.E()-EB.Z()) > 1)
	continue;

    if ((EB.E()-EB.Z()) < -1)
	continue;

    h_CL_minuscut->Fill(TMath::Prob(kin_chisq, kin_ndf), weight);
    h_EB_minuscut->Fill(EB.E(),weight);
    h_ZB_minuscut->Fill(EB.Z(),weight);
    h_YB_minuscut->Fill(EB.Y(),weight);
    h_XB_minuscut->Fill(EB.X(),weight);
    h_PlusBalance_minuscut->Fill(EB.E()+EB.Z(),weight);
    h_MinusBalance_minuscut->Fill(EB.E()-EB.Z(),weight);
    h_PossibleRho_minuscut->Fill(possible_rho.M(),weight);

    h_PlusvsMinus_minuscut->Fill(EB.E()+EB.Z(),EB.E()-EB.Z(),weight);
    h_MinusvsRho_minuscut->Fill(EB.E()-EB.Z(),possible_rho.M(),weight);

// when minus balance is around zero (after minus cut)
    if (((EB.E()-EB.Z()) > -0.2) and ((EB.E()-EB.Z()) < 0.2)){ 
    h_XB_minus0cut_minuscut->Fill(EB.X(),weight);
    h_YB_minus0cut_minuscut->Fill(EB.Y(),weight);
    h_PlusBalance_0cut_minuscut->Fill(EB.E()+EB.Z(),weight); 	
    h_PossibleRho_0cut_minuscut->Fill(possible_rho.M(),weight);  }	

// when minus balance is around -1 (after minus cut)
    if (((EB.E()-EB.Z()) > -1.2) and ((EB.E()-EB.Z()) < -0.8)){
    h_XB_minus1cut_minuscut->Fill(EB.X(),weight);
    h_YB_minus1cut_minuscut->Fill(EB.Y(),weight);
    h_PlusBalance_1cut_minuscut->Fill(EB.E()+EB.Z(),weight); 
    h_PossibleRho_1cut_minuscut->Fill(possible_rho.M(),weight);  }

 
    outputTree->Fill();
  }

  inputFile->Close();



//////////WRITE OUT HISTOGRAMS/////////

fout->cd();

//uncut
mainDir->cd();
for (int h = 0; h < hist_list.size();h++) {
	hist_list[h]->Write();}

subDir1->cd();
for (int h=0; h < hist_minus0.size();h++) {
	hist_minus0[h]->Write();}

subDir2->cd();
for (int h=0; h < hist_minus1.size();h++) {
	hist_minus1[h]->Write();}

//X cut
DirXcut->cd();
for (int h=0; h < hist_Xcut.size();h++) {
	hist_Xcut[h]->Write();}

Dirminus0_Xcut->cd();
for (int h=0; h < hist_minus0_Xcut.size();h++) {
	hist_minus0_Xcut[h]->Write();}

Dirminus1_Xcut->cd();
for (int h=0; h < hist_minus1_Xcut.size();h++) {
	hist_minus1_Xcut[h]->Write();}

//Y cut
DirYcut->cd();
for (int h=0; h < hist_Ycut.size();h++) {
	hist_Ycut[h]->Write();}

Dirminus0_Ycut->cd();
for (int h=0; h < hist_minus0_Ycut.size();h++) {
	hist_minus0_Ycut[h]->Write();}

Dirminus1_Ycut->cd();
for (int h=0; h < hist_minus1_Ycut.size();h++) {
	hist_minus1_Ycut[h]->Write();}

//Plus cut
DirPluscut->cd();
for (int h=0; h < hist_Pluscut.size();h++) {
	hist_Pluscut[h]->Write();}

Dirminus0_Pluscut->cd();
for (int h=0; h < hist_minus0_Pluscut.size();h++) {
	hist_minus0_Pluscut[h]->Write();}

Dirminus1_Pluscut->cd();
for (int h=0; h < hist_minus1_Pluscut.size();h++) {
	hist_minus1_Pluscut[h]->Write();}
  
//Possible Rho cut
Dirrhocut->cd();
for (int h=0; h < hist_rhocut.size();h++) {
	hist_rhocut[h]->Write();}

Dirminus0_rhocut->cd();
for (int h=0; h < hist_minus0_rhocut.size();h++) {
	hist_minus0_rhocut[h]->Write();}

Dirminus1_rhocut->cd();
for (int h=0; h < hist_minus1_rhocut.size();h++) {
	hist_minus1_rhocut[h]->Write();}

// Minus cut
Dirminuscut->cd();
for (int h=0; h < hist_minuscut.size();h++) {
	hist_minuscut[h]->Write();}

Dirminus0_minuscut->cd();
for (int h=0; h < hist_minus0_minuscut.size();h++) {
	hist_minus0_minuscut[h]->Write();}

Dirminus1_minuscut->cd();
for (int h=0; h < hist_minus1_minuscut.size();h++) {
	hist_minus1_minuscut[h]->Write();}

fout->cd();
outputTree->Write();






fout->Close();

}
