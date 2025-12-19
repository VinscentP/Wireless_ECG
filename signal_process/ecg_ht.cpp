//Segmentation
//Uses Hamiltion and Tomkins algoirthm
//Highpass filter -> Lowpass filter -> Differentiation -> Squaring -> Moving Average -> Adaptive Threshold -> R-peak detection

#include <vector>
#include <algorithm>
#include "ecg_ht.h"
using namespace std;

//Differentiation stage of HT algorithm
vector<double> ht_differentiation(const vector<double>& phys_ecg_data, int sampling_freq){
    vector<double> differentiated_ecg;
    //differentiate the ecg data using the difference equation
    for(int i = 1; i < phys_ecg_data.size(); i++){
        double derivative = (phys_ecg_data[i] - phys_ecg_data[i-1]) * sampling_freq;
        differentiated_ecg.push_back(derivative);
    }
    return differentiated_ecg;
}

//Squaring stage of HT algorithm
vector<double> ht_squaring(const vector<double>& differentiated_ecg){
    vector<double> squared_ecg;
    
    for(int i = 0; i < differentiated_ecg.size(); i++){
        squared_ecg.push_back(differentiated_ecg[i] * differentiated_ecg[i]);  
    }
    return squared_ecg;
}

//Moving Average stage of HT algorithm
vector<double> ht_moving_average(const vector<double>& squared_ecg, int window_size){
    double sum = 0.0;
    vector<double> averaged_ecg;
    
    for(int j = 0; j < squared_ecg.size(); j++){
        // If window is full, remove the oldest sample (sliding window)
        if(j >= window_size){
            sum -= squared_ecg[j - window_size];  // Remove sample falling out of window
        }
        
        sum += squared_ecg[j];
        int actual_window = min(j + 1, window_size);
        averaged_ecg.push_back(sum / actual_window);
    }
    
    return averaged_ecg;
}

//Adaptive Threshold and Peak Detection stage of HT algorithm
vector<double> ht_adaptive_threshold(const vector<double>& averaged_ecg, double threshold_factor, int sampling_rate){
    double SPKI, NPKI;
    double noise_threshold, peak_threshold;
    vector<double> r_peak_indices;
    
    int init_segment_length = max(1, (int)(averaged_ecg.size() * 0.05));
    vector<double> init_segment(averaged_ecg.begin(), averaged_ecg.begin() + init_segment_length);
    double PEAKI = *max_element(init_segment.begin(), init_segment.end());

    SPKI = 0.125 * PEAKI;
    NPKI = 0.125 * PEAKI;

    peak_threshold = NPKI + threshold_factor * (SPKI - NPKI);
    noise_threshold = 0.5 * peak_threshold;

    int minimum_samples = (int)(0.2 * sampling_rate);
    int last_r_index = -minimum_samples; // initialize negative so first peak can be detected

    for(int i = 0; i < averaged_ecg.size(); i++){
        if(averaged_ecg[i] > peak_threshold && i - last_r_index >= minimum_samples){
            r_peak_indices.push_back(i );
            last_r_index = i;
            SPKI = 0.125 * averaged_ecg[i] + 0.875 * SPKI;
        }
        else if (averaged_ecg[i] > noise_threshold){
            NPKI = 0.125 * averaged_ecg[i] + 0.875 * NPKI;
        }
        peak_threshold = NPKI + threshold_factor * (SPKI - NPKI);
        noise_threshold = 0.5 * peak_threshold;
    }
    return r_peak_indices;
}