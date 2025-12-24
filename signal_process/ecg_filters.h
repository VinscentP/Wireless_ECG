#ifndef ECG_FILTERS_H
#define ECG_FILTERS_H

#include <vector>
using namespace std;

vector<float> notch_filter(const vector<float>& raw_ecg, float f_notch, float f_sample);
vector<float> bandpass_filter(const vector<float>& raw_ecg, float f_low, float f_high, float f_sample);

#endif