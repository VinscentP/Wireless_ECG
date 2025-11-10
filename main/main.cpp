#include <iostream>
#include <vector>
#include "/usr/local/include/wfdb/wfdb.h" 
#include "/Users/vincentpham/Desktop/Senior_Capstone/signal_acquisition/ecg_get.cpp"
#include "/Users/vincentpham/Desktop/Senior_Capstone/signal_acquisition/ecg_filters.h"
#include "/Users/vincentpham/Desktop/Senior_Capstone/signal_segmentation/ecg_segment.h"

using namespace std;

int main(){
    int nsig = isigopen("ECGMRI3T17Ff", NULL, 0);
    if (nsig < 1){
        cout << "Failed to open signal" << endl;
        return 1;
    }
    WFDB_Siginfo *meta_data_array = new WFDB_Siginfo[nsig];
    isigopen("ECGMRI3T17Ff", meta_data_array, nsig);
    vector<vector<int>> ecg_data = get_ecg_raw_data(nsig);
    vector<vector<double>> phys_ecg_data = convert_raw_to_phys(ecg_data, meta_data_array, nsig);
    
    // Filter all leads (assuming 60 Hz notch at 1000 Hz sampling rate)
    vector<vector<double>> filtered_ecg_data(nsig);
    vector<vector<double>> differentiated_ecg_data(nsig);
    vector<vector<double>> squared_ecg_data(nsig);
    vector<vector<double>> averaged_ecg_data(nsig);

    for (int i = 0; i < nsig; i++) {
        filtered_ecg_data[i] = notch_filter(phys_ecg_data[i], 60, 1000);
        differentiated_ecg_data[i] = ht_differentiation(filtered_ecg_data[i]);
        squared_ecg_data[i] = ht_squaring(differentiated_ecg_data[i]);
        averaged_ecg_data[i] = ht_moving_average(squared_ecg_data[i], 35);
    }

    return 0;
}