#ifndef ECG_EXPORT_H
#define ECG_EXPORT_H

#include <iostream>
#include <vector>
#include <fstream>

using namespace std;


void export_to_csv(const vector<vector<float>>& physical_ecg_data, int nsig, const string& filename);

#endif