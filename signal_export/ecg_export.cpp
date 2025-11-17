#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

void export_to_csv(const vector<vector<double>>& physical_ecg_data, int nsig, const string& filename){
    ofstream ecg_csv_file(filename);

    if(!ecg_csv_file.is_open()){
        cout << "Error opening csv file." << endl;
    }
    else{
        ecg_csv_file << "Lead1,Lead2,Lead3\n";

        int num_samples = physical_ecg_data[0].size();

        for(int i = 0; i < num_samples; i++){
            for(int j = 0; j < nsig; j++){
                ecg_csv_file << physical_ecg_data[j][i];
                if(j < nsig - 1){
                    ecg_csv_file << ",";
                }
            }
            ecg_csv_file << "\n";
        }

        ecg_csv_file.close();
        cout << "Data export complete." << endl;
    }
}