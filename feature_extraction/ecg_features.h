#ifndef ECG_FEATURES_H
#define ECG_FEATURES_H

#include <vector>
using namespace std;

struct pvc_features {
    double pre_rr, post_rr, r_amp;
    double pr_interval, qrs_interval, qt_interval, qrs_area;
};

vector<pvc_features> extract_beat_features(const vector<double>& r_peak_times,const vector<double>& r_amplitudes);

#endif
