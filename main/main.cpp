#include <iostream>
#include <vector>
#include "/usr/local/include/wfdb/wfdb.h" 
#include "/Users/vincentpham/Desktop/Senior_Capstone/signal_acquisition/ecg_get.h"
#include "/Users/vincentpham/Desktop/Senior_Capstone/signal_process/ecg_filters.h"
#include "/Users/vincentpham/Desktop/Senior_Capstone/signal_process/ecg_ht.h"
#include "/Users/vincentpham/Desktop/Senior_Capstone/signal_export/ecg_export.h"

using namespace std;

const int sampling_freq = 1250;

int main(){
    setwfdb("/Users/vincentpham/Desktop/Senior_Capstone/data");
    char filename[] = "ECGMRI3T17Ff";
    int nsig = isigopen(filename, NULL, 0);
    if (nsig < 1){
        cout << "Failed to open signal" << endl;
        return 1;
    }
    
    WFDB_Siginfo *meta_data_array = new WFDB_Siginfo[nsig];
    
    isigopen(filename, meta_data_array, nsig);

    vector<vector<int>> ecg_data = get_ecg_raw_data(nsig);
    vector<vector<double>> phys_ecg_data = convert_raw_to_phys(ecg_data, meta_data_array, nsig);
    //export_to_csv(phys_ecg_data, nsig, "ecg_signal.csv");
    // Filter all leads (assuming 60 Hz notch at 1000 Hz sampling rate)
    vector<vector<double>> filtered_ecg_data(nsig);
    vector<vector<double>> differentiated_ecg_data(nsig);
    vector<vector<double>> squared_ecg_data(nsig);
    vector<vector<double>> averaged_ecg_data(nsig);
    vector<vector<double>> segmentated_ecg_data(nsig);

    for (int i = 0; i < nsig; i++) {
        filtered_ecg_data[i] = bandpass_filter(phys_ecg_data[i], 5, 25, sampling_freq);
        differentiated_ecg_data[i] = ht_differentiation(filtered_ecg_data[i], sampling_freq);
        squared_ecg_data[i] = ht_squaring(differentiated_ecg_data[i]);
        averaged_ecg_data[i] = ht_moving_average(squared_ecg_data[i], 35);
        segmentated_ecg_data[i] = ht_adaptive_threshold(averaged_ecg_data[i], 0.1, sampling_freq);
    }
    export_to_csv(phys_ecg_data, nsig, "ecg_phys.csv" );
    export_to_csv(filtered_ecg_data, nsig, "ecg_filtered.csv" );
    export_to_csv(differentiated_ecg_data, nsig, "ecg_differentiated.csv" );
    export_to_csv(squared_ecg_data, nsig, "ecg_squared.csv" );
    export_to_csv(averaged_ecg_data, nsig, "ecg_average.csv");
    export_to_csv(segmentated_ecg_data, nsig, "ecg_peak.csv");
    return 0;
}