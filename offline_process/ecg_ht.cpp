// Hamiltion and Tomkins algoirthm
//Highpass filter -> Lowpass filter -> Differentiation -> Squaring -> Moving Average -> Adaptive Threshold -> R-peak detection

#include <vector>
#include <algorithm>
#include "ecg_ht.h"
using namespace std;

//Differentiation stage 
vector<float> ht_differentiation(const vector<float>& phys_ecg_data, int sampling_freq){
    vector<float> differentiated_ecg;
    //differentiate the ecg data using the difference equation
    for(int i = 1; i < phys_ecg_data.size(); i++){
        float derivative = (phys_ecg_data[i] - phys_ecg_data[i-1]) * sampling_freq;
        differentiated_ecg.push_back(derivative);
    }
    return differentiated_ecg;
}

//Squaring stage 
vector<float> ht_squaring(const vector<float>& differentiated_ecg){
    vector<float> squared_ecg;
    
    for(int i = 0; i < differentiated_ecg.size(); i++){
        squared_ecg.push_back(differentiated_ecg[i] * differentiated_ecg[i]);  
    }
    return squared_ecg;
}

//Moving Average stage 
vector<float> ht_moving_average(const vector<float>& squared_ecg, int window_size){
    float sum = 0.0;
    vector<float> averaged_ecg;
    
    // Pad beginning with zeros until window is full to avoid edge effects
    for(int j = 0; j < window_size - 1; j++){
        averaged_ecg.push_back(0.0);
    }
    
    // Initialize sum with first window_size samples
    for(int j = 0; j < window_size; j++){
        sum += squared_ecg[j];
    }
    averaged_ecg.push_back(sum / window_size);
    
    // Now slide the window
    for(int j = window_size; j < squared_ecg.size(); j++){
        sum -= squared_ecg[j - window_size];  // Remove oldest sample
        sum += squared_ecg[j];                 // Add newest sample
        averaged_ecg.push_back(sum / window_size);
    }
   
    return averaged_ecg;
}

//Adaptive Threshold stage 
vector<float> ht_adaptive_threshold(const vector<float>& averaged_ecg, float threshold_factor, int sampling_rate) {
    float SPKI, NPKI;
    float peak_threshold, noise_threshold;
    vector<float> approx_indices;
    
    int minimum_samples = (int)(0.2 * sampling_rate);
    int last_r_index = -minimum_samples;

    // Skip initial samples to avoid edge effects (use ~0.2 seconds or minimum 100 samples)
    int start_index = max(100, minimum_samples);
    int init_segment_length = max(1, (int)((averaged_ecg.size() - start_index) * 0.05));
    float PEAKI = *max_element(averaged_ecg.begin() + start_index, averaged_ecg.begin() + start_index + init_segment_length);

    SPKI = 0.125 * PEAKI;
    NPKI = 0.125 * PEAKI;

    peak_threshold = NPKI + threshold_factor * (SPKI - NPKI);
    noise_threshold = 0.5 * peak_threshold;

    
    for (int i = start_index; i < averaged_ecg.size() - 1; i++) { 
        float current_val = averaged_ecg[i];

        // Simple local max check
        if (current_val > averaged_ecg[i - 1] && current_val > averaged_ecg[i + 1]) {
            if (current_val > peak_threshold && (i - last_r_index) >= minimum_samples) {
                // QRS detected
                approx_indices.push_back(i);
                SPKI = 0.125 * current_val + 0.875 * SPKI;
                last_r_index = i;
            } else {
                // Noise
                NPKI = 0.125 * current_val + 0.875 * NPKI;
            }

            // Update thresholds
            peak_threshold = NPKI + threshold_factor * (SPKI - NPKI);
            noise_threshold = 0.5 * peak_threshold;
        }
    }
    return approx_indices;
}

//R Peak detection stage 
vector<float> ht_peak_detection(const vector<float>& threshold_ecg, const vector<float>& filtered_ecg, int window_size){
    vector<float> r_peak_indices;
    
    for(int i = 0; i < threshold_ecg.size(); i++){
        int approx_index = (int)threshold_ecg[i];
        int start = max(0, approx_index - window_size);
        int end = min((int)filtered_ecg.size() - 1, approx_index + window_size);

        int best_index = approx_index;
        float best_val = filtered_ecg[approx_index];

        for(int j = start; j <= end; j++){
            float val = filtered_ecg[j];
            if(val > best_val){
                best_val = val;
                best_index = j;
            }
        }
        r_peak_indices.push_back(best_index);
    }

    return r_peak_indices;
}