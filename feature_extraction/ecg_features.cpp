#include <vector>
//#include <cmath>
#include "ecg_features.h"

using namespace std;

//Need to get: PR interval, QRS interval, QT interval, QRS area
//PR interval = (qrs_start - p_start) / sample rate
//QRS interval = (qrs_end - qrs_start) / sample rate
//QT interval = (t_end - qrs_start) / sample rate
//QRS area = QRS duration (seconds) * r_amp

vector<pvc_features> extract_pvc_features (const vector<double>& r_peak_indices, 
    const vector<double>& filtered_ecg_data, int sampling_frequency){
    vector<pvc_features> feature_vector;
    
    for(int i = 1; i + 1 < r_peak_indices.size(); i++){
        pvc_features bf;
        bf.pre_rr  = (r_peak_indices[i] - r_peak_indices[i - 1]) / (double)sampling_frequency;
        bf.post_rr = (r_peak_indices[i + 1] - r_peak_indices[i]) / (double)sampling_frequency;
        bf.r_amp   = filtered_ecg_data[r_peak_indices[i]];

        feature_vector.push_back(bf);
    }

    return feature_vector;
}

