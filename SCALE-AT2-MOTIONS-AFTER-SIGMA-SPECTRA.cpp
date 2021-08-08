// This code is designed to scale .AT2 motions using scale factors
// obtained from Sigma Spectra
// Date: 08/09/2021
// Okan Ilhan
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <filesystem>
#include <map>
using namespace std;
namespace fs = std::filesystem;

string removeSpaces(string str)
{
    // This part of the code removes the spaces in string.
	str.erase(remove(str.begin(), str.end(), ' '), str.end());
    return str;
}

vector<string> split_str(string str, string delimiter)
{
	// This part of the code splits the string for given delimiter.
	size_t pos = 0;
	string token;
	vector<string> str_sp;
	while ((pos = str.find(delimiter)) != string::npos) {
		token = str.substr(0, pos);
		str_sp.push_back(token);
		//std::cout << token << std::endl;
		str.erase(0, pos + delimiter.length());
	}
	str_sp.push_back(str);
	//std::cout << s << std::endl;
	
	return str_sp;
}

map<string, double > get_scale_factors(string path_to_csv, string csv_name)
{
	// This part of the code reads the scale factors from the .csv file
	// created using the .csv file output of the SigmaSpectra.
	map<string, double > Sc_Fac;
	vector<double> SF;
	vector<string> motions;
	vector<string> csv_file_cont;
	string line;
	string delimiter = ",";
	string path_all = path_to_csv + "/" + csv_name;
	
	int cter = 0;
	
	ifstream myfile(path_all);
	if (myfile.is_open()){
		while(getline(myfile, line)){
			if (cter == 0){
				;
			} else if (cter > 0){
				csv_file_cont = split_str(line, delimiter);
				motions.push_back(csv_file_cont[0]);
				double lol = atof(csv_file_cont[1].c_str());
				SF.push_back(lol);
			}
			++cter;
		}
	}
	
	for (int i = 0; i <= (motions.size()-1); ++i){
		Sc_Fac[motions[i]] = SF[i];
	}
	
	return Sc_Fac;
}

vector<string> get_AT2_file(string path_to_AT2, string AT2_fn)
{
	// This part of the reads the .AT2 file.
	vector<string> AT2_all;
	
	string path_all = path_to_AT2 + "/" + AT2_fn;
	string line;
	string line_1; string line_2; string line_3; string line_4; string line_5;
	
	int cter = 0;
	double line_1d; double line_2d; double line_3d; double line_4d; double line_5d;
	
	ifstream AT2file_h(path_all);
	if (AT2file_h.is_open()){
		while (getline(AT2file_h, line)){
			if (cter <= 3){
				AT2_all.push_back(line);
			} else if (cter > 3){
				line_1 = line.substr(0, 16); line_2 = line.substr(16, 16);
				line_3 = line.substr(32, 16); line_4 = line.substr(48, 16);
				line_5 = line.substr(64, 16);
				
				line_1 = removeSpaces(line_1); line_2 = removeSpaces(line_2);
				line_3 = removeSpaces(line_3); line_4 = removeSpaces(line_4);
				line_5 = removeSpaces(line_5);
				
				AT2_all.push_back(line_1); AT2_all.push_back(line_2);
				AT2_all.push_back(line_3); AT2_all.push_back(line_4);
				AT2_all.push_back(line_5);
			}
			++cter;
		}
		AT2file_h.close();
	}
	
	return AT2_all;
}

vector<double> scale_AT2(vector<string> AT2_file, double SF)
{
	// This part of the code scales the .AT2 file for given
	// scale factor.
	vector<double> sc_AT2;
	
	int cter = 0;
	double line_v;
	
	for (int i = 0; i <= (AT2_file.size()-1); ++i){
		if (i <= 3){
			;
		} else if (i > 3){
			line_v = atof(AT2_file[i].c_str());
			sc_AT2.push_back(line_v*SF);
		}
	}
	
	return sc_AT2;
}

int write_AT2_file(vector<string> AT2_all, vector<double> sc_AT2, string path_to_AT2, string AT2_fn){
	
	// This part of the code writes the .At2 file.
	string path_all = path_to_AT2 + "/" + AT2_fn;
	
	FILE *pFile;
	pFile = fopen(path_all.c_str(), "w");
	
	for (int i = 0; i <= 3; ++i){
		fprintf(pFile, "%s\n", AT2_all[i].c_str());
	}
	
	for (int j = 1; j <= (sc_AT2.size()/5); ++j){
		fprintf(pFile, "%16.6E%16.6E%16.6E%16.6E%16.6E\n", sc_AT2[5*j-5], sc_AT2[5*j-4], sc_AT2[5*j-3], sc_AT2[5*j-2], sc_AT2[5*j-1]);
	}
	
	fclose(pFile);
	
	return 0;
}

int main()
{
	// This code constitutes the main part of the code and executes
	// the code to get scale factors for motions from .AT2 along with
	// scaling motion and writing the .AT2 file.
	map<string, double> csv_file_cont;
	vector<string> motions;
	vector<double> scaled_AT2;
	vector<string> AT2_h;
	
	string path_to_csv;
	string csv_fn;
	string path_to_AT2_files;
	string path_to_AT2_files_w;
		
	cout << "Enter the Path including csv file: " << endl;
	cin >> path_to_csv;
	cout << "Enter the csv file name: " << endl;
	cin >> csv_fn;
	cout << "Enter the path including AT2 file" << endl;
	cin >> path_to_AT2_files;
	cout << "Enter the path AT2 files will be written" << endl;
	cin >> path_to_AT2_files_w;
	
	csv_file_cont = get_scale_factors(path_to_csv, csv_fn);
	
	for(std::map<string,double>::iterator it = csv_file_cont.begin(); it != csv_file_cont.end(); ++it) {
		motions.push_back(it->first);
		//cout << "Key: " << it->first << endl;
		//cout << "Value: " << it->second << endl;
	}
	
	for (int i = 0; i <= (motions.size()-1); ++i){
		cout << motions[i] << endl;
		
		AT2_h = get_AT2_file(path_to_AT2_files, motions[i]);
		double SF = csv_file_cont[motions[i]];
		//cout << AT2_h[0] << endl;
		cout << SF << endl;
		scaled_AT2 = scale_AT2(AT2_h, SF);
		write_AT2_file(AT2_h, scaled_AT2, path_to_AT2_files_w, motions[i]);
	}
	
	return 0;
}