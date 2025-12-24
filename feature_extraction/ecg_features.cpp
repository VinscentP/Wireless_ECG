#include <vector>
//#include <cmath>
#include "ecg_features.h"

using namespace std;

//Need to get: PR interval, QT interval
//PR interval = (qrs_start - p_start) / sample rate
//QRS interval = (qrs_end - qrs_start) / sample rate
//QT interval = (t_end - qrs_start) / sample rate
//QRS area = QRS duration (seconds) * r_amp

//calculate qrs_start
/*based on paper normal duration is 0.04-0.11s for qrs complex 
  10 - 27.5 samples at 250 Hz for search interval 1 -> calculate this in the algorithm

  search interval 2 is 200% wider than search interval 1
  30 - 84 samples 
*/

int get_q_index (const vector<float>& filtered_ecg, 
        int sampling_freq, float threshold_voltage, int r_index){
    
    int search_interval_1  = 0.09 * sampling_freq;      //chose 0.09 seconds for duration
    int search_interval_2 = 3 * search_interval_1;
    
    int Q1_index = r_index; 
    float Q1_val = filtered_ecg[r_index];

    for(int i = max(0, r_index - search_interval_1); i < r_index; i++){
      if(filtered_ecg[i] < Q1_val){
        Q1_val = filtered_ecg[i];
        Q1_index = i;
      }
    }

    int Q2_index = r_index;
    float Q2_val = filtered_ecg[r_index];

    for(int i = max(0, r_index - search_interval_2); i < r_index; i++){
        if(filtered_ecg[i] < Q2_val){
            Q2_val = filtered_ecg[i];
            Q2_index = i;
          }
    }
    
    float MV_qq = 0.0;
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

int get_s_index(const vector<float>& filtered_ecg, 
        int sampling_freq, float threshold_voltage, int r_index){
    
    int search_interval_1  = 0.09 * sampling_freq;      //chose 0.09 seconds for duration
    int search_interval_2 = 3 * search_interval_1;
    
    int S1_index = r_index; 
    float S1_val = filtered_ecg[r_index];

    for(int i = r_index; i <= min((int)filtered_ecg.size() - 1, r_index + search_interval_1); i++){
      if(filtered_ecg[i] < S1_val){
        S1_val = filtered_ecg[i];
        S1_index = i;
      }
    }

    int S2_index = r_index;
    float S2_val = filtered_ecg[r_index];

    for(int i = r_index; i <= min((int)filtered_ecg.size() - 1, r_index + search_interval_2); i++){
        if(filtered_ecg[i] < S2_val){
            S2_val = filtered_ecg[i];
            S2_index = i;
          }
    }
    
    float MV_ss = 0.0;
    for(int i = min(S1_index, S2_index); i <= max(S1_index, S2_index); i++){
        MV_ss = max(MV_ss, abs(filtered_ecg[i]));
    }
    if(S1_index == S2_index){
        return S1_index;
    }
    else if (MV_ss > abs(S1_val) + threshold_voltage) {
        return S2_index;
    }
    else if (S2_val > S1_val){
        return S1_index;
    }
    else{
        return S2_index;
    }
}

float get_qrs_interval(int q_index, int s_index, int sampling_freq){
    return (s_index - q_index) / sampling_freq;
}

float get_qrs_area(float qrs_interval, float r_amp){
    return qrs_interval * r_amp;
}
//get p start & t end
vector<pvc_features> extract_pvc_features (const vector<float>& r_peak_indices, 
    const vector<float>& filtered_ecg_data, int sampling_freq){
    vector<pvc_features> feature_vector;

    for(int i = 1; i + 1 < r_peak_indices.size(); i++){
        pvc_features bf;
        bf.pre_rr  = (r_peak_indices[i] - r_peak_indices[i - 1]) / (float)sampling_freq;
        bf.post_rr = (r_peak_indices[i + 1] - r_peak_indices[i]) / (float)sampling_freq;
        bf.r_amp   = filtered_ecg_data[r_peak_indices[i]];
        bf.qrs_interval = get_qrs_interval(get_q_index(filtered_ecg_data, sampling_freq, 0.18, i),
                                        get_s_index(filtered_ecg_data, sampling_freq, 0.18, i),
                                        sampling_freq);
        bf.qrs_area = get_qrs_area(bf.qrs_interval, bf.r_amp);
        feature_vector.push_back(bf);
    }

    return feature_vector;
}