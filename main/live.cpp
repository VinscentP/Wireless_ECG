#include <iostream>
#include <vector>
#include "/usr/local/include/wfdb/wfdb.h"
#include "/Users/vincentpham/Desktop/Senior_Capstone/signal_acquisition/ecg_get.h"
#include "/Users/vincentpham/Desktop/Senior_Capstone/signal_process/bandpass_filter.cpp"
#include "/Users/vincentpham/Desktop/Senior_Capstone/signal_process/signal_differetiation.cpp"
#include "/Users/vincentpham/Desktop/Senior_Capstone/signal_process/signal_squaring.cpp"
#include "/Users/vincentpham/Desktop/Senior_Capstone/signal_export/ecg_export.h"
#include "/Users/vincentpham/Desktop/Senior_Capstone/signal_process/moving_average.cpp"
#include "/Users/vincentpham/Desktop/Senior_Capstone/signal_process/adaptive_threshold.cpp"
#include "/Users/vincentpham/Desktop/Senior_Capstone/signal_process/peak_detection.cpp"
#include "/Users/vincentpham/Desktop/Senior_Capstone/feature_extraction/ecg_features.h"
#include "/Users/vincentpham/Desktop/Senior_Capstone/signal_export/feature_export.h"

using namespace std;

const int sampling_freq = 1024;

int main() {
    setwfdb("/Users/vincentpham/Desktop/Senior_Capstone/data");
    char filename[] = "ECGMRI3T17Ff";

    int nsig = isigopen(filename, NULL, 0);
    if (nsig < 1) {
        cout << "Failed to open signal" << endl;
        return 1;
    }

    WFDB_Siginfo* meta_data_array = new WFDB_Siginfo[nsig];
    isigopen(filename, meta_data_array, nsig);

    vector<vector<int>> ecg_data = get_ecg_raw_data(nsig);
    vector<vector<float>> phys_ecg_data = convert_raw_to_phys(ecg_data, meta_data_array, nsig);

    vector<vector<float>> filtered_ecg_data(nsig);
    vector<vector<float>> differentiated_ecg_data(nsig);
    vector<vector<float>> squared_ecg_data(nsig);
    vector<vector<float>> averaged_ecg_data(nsig);
    vector<vector<float>> segmentated_ecg_data(nsig);
    vector<vector<float>> r_indices(nsig);
    vector<vector<pvc_features>> pvc_features_vector(nsig);

    

    for (int i = 0; i < nsig; i++) {
        BandpassFilter bandpass_filter(5, 25, sampling_freq);
        SignalDifferentiator signal_differentiator(sampling_freq);
        //square signal by using the function signal_squaring
        MovingAverage moving_average(0.1f * sampling_freq); // 37 = fs * duration of QRS
        AdaptiveThreshold adaptive_threshold(sampling_freq, 0.01f);
        PeakDetector peak_detector(sampling_freq);
        for(int j = 0; j < phys_ecg_data[i].size(); j++){
            float filtered_sample = bandpass_filter.filterSample(phys_ecg_data[i][j]);
            float differentiated_sample = signal_differentiator.differentiateSample(filtered_sample);
            float squared_sample = signal_squaring(differentiated_sample);
            float averaged_sample = moving_average.moving_average(squared_sample);
            int r_index = adaptive_threshold.process(averaged_sample);
     
            filtered_ecg_data[i].push_back(filtered_sample);
            differentiated_ecg_data[i].push_back(differentiated_sample);
            squared_ecg_data[i].push_back(squared_sample);
            averaged_ecg_data[i].push_back(averaged_sample);
            segmentated_ecg_data[i].push_back(averaged_sample);
            r_indices[i].push_back(r_index);
        }
    }

    // Export every pipeline stage to CSV for inspection
    export_to_csv(phys_ecg_data, nsig, "ecg_phys.csv");
    export_to_csv(filtered_ecg_data, nsig, "ecg_filtered.csv");
    export_to_csv(differentiated_ecg_data, nsig, "ecg_differentiated.csv");
    export_to_csv(squared_ecg_data, nsig, "ecg_squared.csv");
    export_to_csv(averaged_ecg_data, nsig, "ecg_averaged.csv");
    //export_to_csv(segmentated_ecg_data, nsig, "ecg_segmentated.csv");  // approx R-peak indices from threshold
    export_to_csv(r_indices, nsig, "r_indices.csv");
    //export_to_csv(pvc_features_vector, nsig, "pvc_features.csv");

    return 0;
}
