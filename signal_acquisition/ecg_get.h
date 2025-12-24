#ifndef ECG_GET_H
#define ECG_GET_H

#include <iostream>
#include <vector>
#include "/usr/local/include/wfdb/wfdb.h" 
using namespace std;

WFDB_Siginfo*  load_ecg_metaset();

vector<vector<int>> get_ecg_raw_data(int nsig);

vector<vector<float>> convert_raw_to_phys(const vector<vector<int>>& raw_data, WFDB_Siginfo* siginfo, int nsig);

#endif