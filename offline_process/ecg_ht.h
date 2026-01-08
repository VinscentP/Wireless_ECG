#ifndef ECG_HT_H
#define ECG_HT_H

#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;

vector<float> ht_differentiation(const vector<float>& phys_ecg_data, int sampling_freq);
vector<float> ht_squaring(const vector<float>& differentiated_ecg);
vector<float> ht_moving_average(const vector<float>& squared_ecg, int window_size);
vector<float> ht_adaptive_threshold(const vector<float>& averaged_ecg, float threshold_factor, int sampling_rate);
vector<float> ht_peak_detection(const vector<float>& threshold_ecg, const vector<float>& filtered_ecg, int window_size);
#endif