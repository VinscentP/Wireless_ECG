#include <cmath>

using namespace std;

class BandpassFilter {
    private:
        float z1, z2;
        float b0, b1, b2;
        float a1, a2;
    public:
        BandpassFilter(float f_low, float f_high, float f_sample) : z1(0), z2(0){
            const float pi = 3.14159265358979323846;
            float w1 = tan(pi * f_low / f_sample);
            float w2 = tan(pi * f_high / f_sample);
            
            float Bw = w2 - w1;
            float W0 = sqrt(w1 * w2);
            
            float B = Bw;
            float C = W0 * W0;
            float denom = (4.0 + 2.0 * B + C);
            
            b0 = 2.0 * B / denom;
            b1 = 0.0;
            b2 = -2.0 * B / denom;
            
            a1 = (2.0 * C - 8.0) / denom;
            a2 = (4.0 - 2.0 * B + C) / denom;
        }

        float filterSample(float input){
            float output = b0 * input + z1;
            z1 = b1 * input - a1 * output + z2;
            z2 = b2 * input - a2 * output;
            return output;
        }

        void reset(){
            z1 = 0;
            z2 = 0;
        }
};