#include <iostream>
#include <vector>
#include <fstream>

#include "/Users/vincentpham/Desktop/Senior_Capstone/feature_extraction/ecg_features.h"

using namespace std;

void export_to_csv(const vector<vector<pvc_features>>& data, int nsig, const string& filename) {
    ofstream feature_csv_file(filename);
    if (!feature_csv_file.is_open()) {
        cout << "Error opening CSV file." << endl;
        return;
    }

    // Write header
    feature_csv_file << "Pre-RR" << "," << "Post-RR" << "," << "R-Amplitude" << "," << "QRS-Interval" << "," << "QRS-Area";
    feature_csv_file << "\n";

    // Write data row by row
    for (int i = 0; i < nsig; i++) {
        for (int j = 0; j < data[i].size(); j++) {
            feature_csv_file << data[i][j].pre_rr << "," << data[i][j].post_rr << "," << data[i][j].r_amp << "," << data[i][j].qrs_interval << "," << data[i][j].qrs_area << "\n";
        }
    }
    feature_csv_file.close();
    cout << "PVC features export complete." << endl;
}