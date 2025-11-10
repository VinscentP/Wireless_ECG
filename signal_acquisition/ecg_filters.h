#ifndef ECG_FILTERS_H
#define ECG_FILTERS_H

#include <vector>
using namespace std;

vector<double> notch_filter(const vector<double>& raw_ecg, double f_notch, double f_sample);

#endif