#ifndef ECG_FEATURES_H
#define ECG_FEATURES_H

#include <vector>
using namespace std;

struct pvc_features {
    float pre_rr, post_rr, r_amp;
    float qrs_interval, qrs_area;
    //float pr_interval, qt_interval;
};

int get_q_index (const vector<float>& filtered_ecg, int sampling_freq, float threshold_voltage, int r_index);
int get_s_index(const vector<float>& filtered_ecg, int sampling_freq, float threshold_voltage, int r_index);
float get_qrs_interval(int q_index, int s_index, int sampling_freq);
float get_qrs_area(float qrs_interval, float r_amp);
vector<pvc_features> extract_pvc_features (const vector<float>& r_peak_indices, const vector<float>& filtered_ecg_data, int sampling_freq);

#endif
