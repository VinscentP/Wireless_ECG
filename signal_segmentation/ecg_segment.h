#ifndef ECG_SEGMENT_H
#define ECG_SEGMENT_H

#include <vector>
#include <cmath>
using namespace std;

vector<double> ht_differentiation(const vector<double>& phys_ecg_data);
vector<double> ht_squaring(const vector<double>& differentiated_ecg);
vector<double> ht_moving_average(const vector<double>& squared_ecg, int window_size);

#endif