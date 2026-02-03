#include <vector>

using namespace std;

class MovingAverage {
    private:
        vector<float> buffer;
        float sum;
        int window_size;
        int count;
        int index;

        
    public:
        MovingAverage(float window_size) :
         sum(0), index(0), count(0), 
         buffer(window_size, 0),
         window_size(window_size){}

         float moving_average(float input){
            sum -= buffer[index];
            buffer[index] = input;
            sum += input;
            index = (index + 1) % window_size;
        
            if(count < window_size) count++;
        
            if(count < window_size) return sum / window_size; // use full window size for early samples
            return sum / window_size;
        }
        
};
