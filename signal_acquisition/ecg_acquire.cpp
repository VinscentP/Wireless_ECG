//prototype ecg accuqisition function using sample dataset
//adaptive sampling -> filter noise -> segment signals -> convert segment to numerical values
// put features into classification model

#include <iostream>
#include <vector>
#include "/usr/local/include/wfdb/wfdb.h" 
using namespace std;

//load PhysioNet ECG dataset
WFDB_Siginfo*  load_ecg_metaset(){ 
    setwfdb("/Users/vincentpham/Desktop/Senior_Capstone/data");
    
    int nsig = isigopen("ECGMRI3T01Ff", NULL, 0);
    if (nsig < 1){
        cout << "Failed to open signal" << endl;
        return nullptr;
    }
    WFDB_Siginfo *meta_data_array = new WFDB_Siginfo[nsig];
    isigopen("ECGMRI3T01Ff", meta_data_array, nsig);
    return meta_data_array;
}

vector<vector<int>> get_ecg_raw_data(int nsig){
    vector<vector<int>> ecg_data(nsig);

    WFDB_Sample *raw_sample = new WFDB_Sample[nsig];

    //use getvec to load data from .dat file into raw_sample
    while(getvec(raw_sample) > 0){
        for(int i = 0; i < nsig; i++){
            ecg_data[i].push_back(static_cast<int>(raw_sample[i]));
        }
    }

    delete[] raw_sample;
    return ecg_data;
}

vector<vector<double>> convert_raw_to_phys(const vector<vector<int>>& raw_data, WFDB_Siginfo* siginfo, int nsig){
    //physical value = (digital value - baseline) / gain
    vector<vector<double>> phys_ecg_data(nsig);

    for(int i = 0; i < nsig; i++){
        int baseline = siginfo[i].baseline;
        double gain = siginfo[i].gain;

        for(int j = 0; j < raw_data[i].size(); j++){
            int digital = raw_data[i][j];
            phys_ecg_data[i].push_back((digital - baseline) / gain);
        }
    }
    return phys_ecg_data;
}




