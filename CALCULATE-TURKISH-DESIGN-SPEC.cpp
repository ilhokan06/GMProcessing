// This code is designed to calculate Turkish Design Spectrum defined in
// Section 2 of Turkish Building Code (2018). The compiled code requests
// six inputs from the user:
// 1. Soil type for the site at which the design spectrum is computed.
// 2. Ss and S1 value extracted from the table at the beginning of the
//    Turkish Design Spectrum with respect to the site location.
// 3. The period increment, dT, value.
// 4. The path at which the output .csv file including design spectrum
//	  is written.
// 5. The name of the .csv file including design spectrum
// Written by Okan Ilhan
// Date: 07/28/2021

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <cmath>

double interpolate(double Sl, double Su, double afl, double afu, double Sv)
{
	// This part of the code is designed to perform interpolation between
	// Ss and S1 values using given Ss value from TBC (2018)
	double afv = 0.0;
	
	if (Sv <= Sl){
		afv = afl;
	} else if ((Sv > Sl) and (Sv < Su)){
		afv = ((Sv-Sl)/(Su-Sl))*(afu-afl)+afl;
	} else if (Sv >= Su){
		afv = afu;
	}
	
	return afv;
}

std::vector<double> FSF1(std::string ST, double Ss_r, double S1_r)
{
	// This part of the code returns the FS and F1 factor for given
	// Ss and S1 values.
	
	std::map<std::string, std::vector<double> > FS;
	std::map<std::string, std::vector<double> > F1;
	
	std::vector<double> ZA_Ss = {0.8, 0.8, 0.8, 0.8, 0.8, 0.8}; std::vector<double> ZB_Ss = {0.9, 0.9, 0.9, 0.9, 0.9, 0.9};
	std::vector<double> ZC_Ss = {1.3, 1.3, 1.2, 1.2, 1.2, 1.2}; std::vector<double> ZD_Ss = {1.6, 1.4, 1.2, 1.1, 1.0, 1.0};
	std::vector<double> ZE_Ss = {2.4, 1.7, 1.3, 1.1, 0.9, 0.8}; std::vector<double> ZA_S1 = {0.8, 0.8, 0.8, 0.8, 0.8, 0.8};
	std::vector<double> ZB_S1 = {0.8, 0.8, 0.8, 0.8, 0.8, 0.8}; std::vector<double> ZC_S1 = {1.5, 1.5, 1.5, 1.5, 1.5, 1.4};
	std::vector<double> ZD_S1 = {2.4, 2.2, 2.0, 1.9, 1.8, 1.7}; std::vector<double> ZE_S1 = {4.2, 3.3, 2.8, 2.4, 2.2, 2.0};
	std::vector<double> FS_Ss = {0.25, 0.5, 0.75, 1.0, 1.25, 1.5}; std::vector<double> F1_S1 = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6};
	
	for (int i = 1; i <= ZA_Ss.size(); ++i){
		FS["ZA"].push_back(ZA_Ss[i-1]); FS["ZB"].push_back(ZB_Ss[i-1]); FS["ZC"].push_back(ZC_Ss[i-1]);
		FS["ZD"].push_back(ZD_Ss[i-1]); FS["ZE"].push_back(ZE_Ss[i-1]); F1["ZA"].push_back(ZA_S1[i-1]);
		F1["ZB"].push_back(ZB_S1[i-1]); F1["ZC"].push_back(ZC_S1[i-1]); F1["ZD"].push_back(ZD_S1[i-1]);
		F1["ZE"].push_back(ZE_S1[i-1]);
	}
	
	int Ss_ind = 0; int S1_ind = 0;
	double Ss_Sl = 0.0; double Ss_Su = 0.0;
	double S1_Sl = 0.0; double S1_Su = 0.0;
	double afl_Ss = 0.0; double afu_Ss = 0.0;
	double afl_S1 = 0.0; double afu_S1 = 0.0;
	
	for (int i = 0; i <= (FS_Ss.size()-1); ++i){
		if ((Ss_r <= FS_Ss[i]) and (i > 0) and (i < FS_Ss.size())){
			Ss_ind = i; Ss_Sl = FS_Ss[Ss_ind-1]; Ss_Su = FS_Ss[Ss_ind];
			afl_Ss = FS[ST][Ss_ind-1]; afu_Ss = FS[ST][Ss_ind];
		} else if ((Ss_r <= FS_Ss[i]) and (i == 0)){
			Ss_ind = i; Ss_Sl = FS_Ss[Ss_ind]; Ss_Su = FS_Ss[Ss_ind];
			afl_Ss = FS[ST][Ss_ind]; afu_Ss = FS[ST][Ss_ind];
		} else if ((Ss_r >= FS_Ss[i]) and (i == (FS_Ss.size()-1))){
			Ss_ind = i; Ss_Sl = FS_Ss[Ss_ind]; Ss_Su = FS_Ss[Ss_ind];
			afl_Ss = FS[ST][Ss_ind]; afu_Ss = FS[ST][Ss_ind];
		}
	}
	
	for (int i = 0; i <= (F1_S1.size()-1); ++i){
		if ((S1_r <= F1_S1[i]) and (i > 0) and (i < F1_S1.size())){
			S1_ind = i; S1_Sl = F1_S1[S1_ind-1]; S1_Su = F1_S1[S1_ind];
			afl_S1 = F1[ST][S1_ind-1]; afu_S1 = F1[ST][S1_ind];
		} else if ((S1_r <= F1_S1[i]) and (i == 0)){
			S1_ind = i; S1_Sl = F1_S1[S1_ind]; S1_Su = F1_S1[S1_ind];
			afl_S1 = F1[ST][S1_ind]; afu_S1 = F1[ST][S1_ind];
		} else if ((S1_r >= F1_S1[i]) and (i == (F1_S1.size()-1))){
			S1_ind = i; S1_Sl = F1_S1[S1_ind]; S1_Su = F1_S1[S1_ind];
			afl_S1 = F1[ST][S1_ind]; afu_S1 = F1[ST][S1_ind];
		}
	}
	
	double afv_Ss = 0.0; double afv_S1 = 0.0;
	
	afv_Ss = interpolate(Ss_Sl, Ss_Su, afl_Ss, afu_Ss, Ss_r);
	afv_S1 = interpolate(S1_Sl, S1_Su, afl_S1, afu_S1, S1_r);
	
	std::vector<double> FS_F1 = {afv_Ss, afv_S1};
	
	//std::vector<double> FS_F1 = {Ss_ind, Ss_Sl, Ss_Su, S1_ind, S1_Sl, S1_Su};
	
	return FS_F1;
}

std::vector<double> DS_Params(double FS, double F1, double Ss, double S1)
{
	// This part of the code return the required SDS, S1 and
	// TA and TB parameters for Turkish Design Spectrum
	// calculation.
	
	double SDS = Ss*FS;
	double SD1 = S1*F1;
	double TA = 0.2*SD1/SDS;
	double TB = SD1/SDS;
	
	std::vector<double> SDS_SD1_TA_TB = {SDS, SD1, TA, TB};
	
	return SDS_SD1_TA_TB;
}

std::map<std::string, std::vector<double>> Calculate_RS(double SDS, double SD1, double TA, double TB, double dT)
{
	// This part of the code calculates the Turkish Design Spectrum (2018).
	
	double TL = 6.0;
	double T_V;
	
	std::map<std::string, std::vector<double> > TDS18;
	
	for (int i = 1; i <= 10.0/dT+1; ++i){
		T_V = i*dT;
		if ((T_V >= 0) and (T_V <= TA)){
			TDS18["T"].push_back(T_V);
			TDS18["SA"].push_back((0.4+0.6*T_V/TA)*SDS);
		} else if ((T_V > TA) and (T_V <= TB)){
			TDS18["T"].push_back(T_V);
			TDS18["SA"].push_back(SDS);
		} else if ((T_V > TB) and (T_V <= TL)){
			TDS18["T"].push_back(T_V);
			TDS18["SA"].push_back(SD1/T_V);
		} else if (T_V > TL){
			TDS18["T"].push_back(T_V);
			TDS18["SA"].push_back(SD1*TL/(pow(T_V, 2.0)));
		}
	}
	
	return TDS18;
}

int main()
{
	std::string soil_type = " ";
	double Ss = 0.0;
	double S1 = 0.0;
	double dT = 0.0;
	std::string file_path;
	std::string file_name;
	double sigma_v;
	
	std::cout << "Enter the Soil Type: " << std::endl;
	std::cin >> soil_type;
	std::cout << "Ss value: " << std::endl;
	std::cin >> Ss;
	std::cout << "S1 value: " << std::endl;
	std::cin >> S1;
	std::cout << "dT value: " << std::endl;
	std::cin >> dT;
	std::cout << "Enter the File Path: " << std::endl;
	std::cin >> file_path;
	std::cout << "Enter the File Name: " << std::endl;
	std::cin >> file_name;
	std::cout << "Enter the sigma for Design Spectrum :" << std::endl;
	std::cin >> sigma_v;
	
	
	std::vector<double> FSF1_V;
	std::vector<double> SDS_SD1_TA_TB_V;
	std::map<std::string, std::vector<double> > TDS18_V;
	std::string file_total = file_path + "/" + file_name + ".csv";
	std::string file_total_param = file_path + "/" + file_name + "_params.csv";
	
	FILE *pFile = fopen(file_total.c_str(), "w");
	
	FSF1_V = FSF1(soil_type, Ss, S1);
	SDS_SD1_TA_TB_V = DS_Params(FSF1_V[0], FSF1_V[1], Ss, S1);
	TDS18_V = Calculate_RS(SDS_SD1_TA_TB_V[0], SDS_SD1_TA_TB_V[1], SDS_SD1_TA_TB_V[2], SDS_SD1_TA_TB_V[3], dT);
	
	double FS = FSF1_V[0]; double F1 = FSF1_V[1];
	double SDS = SDS_SD1_TA_TB_V[0] ; double SD1 = SDS_SD1_TA_TB_V[1];
	double TA = SDS_SD1_TA_TB_V[2] ; double TB = SDS_SD1_TA_TB_V[3];
	
	std::cout << "FS and F1 Values: " << FSF1_V[0] << " " << FSF1_V[1] << std::endl;
	std::cout << "SDS, SD1, TA and TB Values: " << SDS_SD1_TA_TB_V[0] << " " << SDS_SD1_TA_TB_V[1]
			  << " " << SDS_SD1_TA_TB_V[2] << " " << SDS_SD1_TA_TB_V[3] << std::endl;
			  
	for (int i = 1; i <= 10.0/dT+1; ++i){
		if (i == 1){
			//std::cout << "T (s)" << "," << "SA (g)" << std::endl;
			fprintf(pFile, "%s,%s,%s\n", "T (s)","SA (g)","ln(sigma)");
		}
		//std::cout << TDS18_V["T"][i-1] << "," << TDS18_V["SA"][i-1] << std::endl;
		fprintf(pFile, "%f,%f,%f\n", TDS18_V["T"][i-1], TDS18_V["SA"][i-1], sigma_v);
	}
	//fprintf(pFile, "%f, %f\n", TDS18_V["T"][TDS18_V["T"].size()], TDS18_V["SA"][TDS18_V["T"].size()]);
	fclose(pFile);
	
	FILE *pFile_2 = fopen(file_total_param.c_str(), "w");
	
	fprintf(pFile_2, "%s,%f\n", "Fs",FS);
	fprintf(pFile_2, "%s,%f\n", "F1",F1);
	fprintf(pFile_2, "%s,%f\n", "SDS",SDS);
	fprintf(pFile_2, "%s,%f\n", "SD1",SD1);
	fprintf(pFile_2, "%s,%f\n", "TA",TA);
	fprintf(pFile_2, "%s,%f\n", "TB",TB);
	
	fclose(pFile_2);
}