#ifndef ECG_HT_H
#define ECG_HT_H

#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;

vector<double> ht_differentiation(const vector<double>& phys_ecg_data);
vector<double> ht_squaring(const vector<double>& differentiated_ecg);
vector<double> ht_moving_average(const vector<double>& squared_ecg, int window_size);
vector<double> ht_adaptive_threshold(const vector<double>& averaged_ecg, double threshold_factor, int sampling_rate);
#endif