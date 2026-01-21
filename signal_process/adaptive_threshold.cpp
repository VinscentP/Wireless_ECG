#include <vector>
#include <algorithm>
using namespace std;

class AdaptiveThreshold{
    private:
    float SPKI;
    float NPKI;
    float threshold;
    int sample_count;
    int warmup_samples;
    bool warmed_up;

    public:
        AdaptiveThreshold(int fs, float init_threshold = 0.3f)
        :SPKI(init_threshold),
        NPKI(init_threshold * 0.5f),
        threshold(init_threshold),
        sample_count(0),
        warmup_samples(fs),
        warmed_up(false)
        {}

        bool process(float peak){
            sample_count++;

            if(!warmed_up){
                SPKI = max(SPKI, peak);
                NPKI = 0.125f * peak + 0.875f * NPKI;
                if (sample_count >= warmup_samples){
                    threshold = NPKI + 0.25f * (SPKI - NPKI);
                    warmed_up = true;
                }
                return false;
            }
            bool qrs = false;
            if(peak > threshold){
                SPKI = 0.125f * peak + 0.875f * SPKI;
                qrs = true;
            }
            else{
                NPKI = 0.125f * peak + 0.875f * NPKI;
            }

            threshold = NPKI + 0.25f * (SPKI - NPKI);

            return qrs;
        }

        float get_threshold() const{
            return threshold;
        }

        bool ready() const{
            return warmed_up;
        }
    
        
};