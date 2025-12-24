#ifndef ECG_FEATURES_H
#define ECG_FEATURES_H

#include <vector>
using namespace std;

struct pvc_features {
    float pre_rr, post_rr, r_amp;
    float pr_interval, qrs_interval, qt_interval, qrs_area;
};

vector<pvc_features> extract_beat_features(const vector<float>& r_peak_times,const vector<float>& r_amplitudes);
int get_q_index (const vector<float>& filtered_ecg, int sampling_freq, float threshold_voltage, int r_index);

#endif
