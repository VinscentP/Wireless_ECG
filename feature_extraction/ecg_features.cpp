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
//calculate qrs_start
/*based on paper normal duration is 0.04-0.11s for qrs complex 
  10 - 27.5 samples at 250 Hz for search interval 1 -> calculate this in the algorithm

  search interval 2 is 200% wider than search interval 1
  30 - 84 samples 
*/

int get_qrs_start (const vector<double>& filtered_ecg, 
        int sampling_freq, double threshold_voltage, int r_index){
    
    int search_interval_1  = 0.09 * sampling_freq;      //chose 0.09 seconds for duration
    int search_interval_2 = 3 * search_interval_1;
    
    int Q1_index = r_index; 
    double Q1_val = filtered_ecg[r_index];

    for(int i = max(0, r_index - search_interval_1); i < r_index; i++){
      if(filtered_ecg[i] < Q1_val){
        Q1_val = filtered_ecg[i];
        Q1_index = i;
      }
    }

    int Q2_index = r_index;
    double Q2_val = filtered_ecg[r_index];

    for(int i = r_index; i < search_interval_2; i++){
        if(filtered_ecg[i] < Q2_val){
            Q2_val = filtered_ecg[i];
            Q2_index = i;
          }
    }
    
    double MV_qq = 0.0;
    for(int i = min(Q1_index, Q2_index); i <= max(Q1_index, Q2_index); i++){
        MV_qq = max(MV_qq, abs(filtered_ecg[i]));
    }
    if(Q1_index == Q2_index){
        return Q1_index;
    }
    else if (MV_qq > abs(Q1_val) + threshold_voltage) {
        return Q2_index;
    }
    else if (Q2_val > Q1_val){
        return Q1_index;
    }
    else{
        return Q2_index;
    }
}