#ifndef FEATURE_EXPORT_H
#define FEATURE_EXPORT_H

#include <iostream>
#include <vector>
#include <fstream>
#include "/Users/vincentpham/Desktop/Senior_Capstone/feature_extraction/ecg_features.h"

using namespace std;

void export_to_csv(const vector<vector<pvc_features>>& data, int nsig, const string& filename);

#endif