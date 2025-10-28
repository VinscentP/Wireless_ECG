//prototype ecg accuqisition function using sample dataset
//adaptive sampling -> filter noise -> segment signals -> convert segment to numerical values
// put features into classification model

#include <iostream>
#include <string> 
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
    else{
        WFDB_Siginfo *meta_data_array = new WFDB_Siginfo[nsig];
        isigopen("ECGMRI3T01Ff", meta_data_array, nsig);
    }
}

vector<vector<int>> get_ecg_raw_data(int nsig){
    vector<vector<int>> ecg_data[nsig];

    WFDB_Sample *raw_sample = new WFDB_Sample[nsig];

    //use getvec to load data from .dat file into raw_sample
    // load data from raw_sample into ecg_data pushback

}






