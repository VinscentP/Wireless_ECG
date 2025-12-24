#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

void export_to_csv(const vector<vector<float>>& data, int nsig, const string& filename) {
    ofstream ecg_csv_file(filename);
    if (!ecg_csv_file.is_open()) {
        cout << "Error opening CSV file." << endl;
        return;
    }

    // Write header
    for (int j = 0; j < nsig; j++) {
        ecg_csv_file << "Lead" << (j + 1);
        if (j < nsig - 1) ecg_csv_file << ",";
    }
    ecg_csv_file << "\n";

    // Find the maximum number of samples/peaks across all leads
    size_t max_size = 0;
    for (int j = 0; j < nsig; j++) {
        if (data[j].size() > max_size)
            max_size = data[j].size();
    }

    // Write data row by row
    for (size_t i = 0; i < max_size; i++) {
        for (int j = 0; j < nsig; j++) {
            if (i < data[j].size())
                ecg_csv_file << data[j][i];  // write value if exists
            // else leave blank if this lead has fewer values
            if (j < nsig - 1) ecg_csv_file << ",";
        }
        ecg_csv_file << "\n";
    }

    ecg_csv_file.close();
    cout << "Data export complete." << endl;
}
