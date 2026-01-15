#include <vector>

using namespace std;
class PeakDetector{
    private:
        vector<float> buffer;
        vector<int> r_peak_indices;
        float prev_sample, next_sample;

        int sample_counter;

    public:
    PeakDetector(): prev_sample(0), next_sample(0), sample_counter(0){}
    int peakDetection(float input){
        
    }
    void addPeak(int index){
        r_peak_indices.push_back(index);
    }

    const vector<int>& getPeaks(){
        return r_peak_indices;
    }
};