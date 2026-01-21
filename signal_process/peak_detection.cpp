#include <vector>

using namespace std;
class PeakDetector{
    private:
        //vector<float> buffer;
        vector<int> r_peak_indices;
        float prev_sample, current_sample, next_sample;

        int sample_counter;

        int refractory_period;
        int last_peak_index;

    public:
        PeakDetector(int fs): 
            prev_sample(0), 
            next_sample(0), 
            sample_counter(0),
            current_sample(0),
            refractory_period(fs * 0.2),
            last_peak_index(-10000)
            {}
        int peakDetection(float input){
            sample_counter++;

            prev_sample = current_sample;
            current_sample = next_sample;
            next_sample = input;

            if (sample_counter < 3){
            return -1;
            }

            if(sample_counter - last_peak_index < refractory_period){
            return -1;
            }

            if (current_sample > prev_sample && current_sample > next_sample) {
                int peak_index = sample_counter - 1;
                addPeak(peak_index);
                last_peak_index = peak_index;
                return peak_index;
            }
            return -1;
        }
        void addPeak(int index){
            r_peak_indices.push_back(index);
        }

        const vector<int>& getPeaks(){
            return r_peak_indices;
        }

        void reset(){
            r_peak_indices.clear();
            sample_counter = 0;
            last_peak_index = -10000;
        }
};