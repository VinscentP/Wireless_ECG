#include <algorithm>
#include <iostream>
using namespace std;

class AdaptiveThreshold {
private:
    float SPKI;           // Running estimate of signal peaks
    float NPKI;           // Running estimate of noise
    float threshold;      // Current threshold

    int fs;               // Sampling frequency
    int sample_index;     // Current sample number
    int last_r_index;     // Last detected R peak
    int min_rr_samples;   // Minimum RR interval in samples

    float prev2, prev1;   // For local max detection
    bool warmed_up;       // Whether warm-up finished
    int warmup_samples;   // Number of samples to warm up

public:
    AdaptiveThreshold(int sampling_rate, float init_threshold = 0.001f, int warmup_s = 512)
        : SPKI(init_threshold),
          NPKI(init_threshold),
          threshold(init_threshold),
          fs(sampling_rate),
          sample_index(0),
          last_r_index(-100000),
          min_rr_samples(static_cast<int>(0.2f * sampling_rate)),  // min 200 ms between R peaks
          prev2(0.0f),
          prev1(0.0f),
          warmed_up(false),
          warmup_samples(warmup_s) {}

    // Returns R-peak index, or -1 if none
    int process(float x) {
        sample_index++;

        // Warm-up phase: adapt SPKI and NPKI
        if (!warmed_up) {
            SPKI = max(SPKI, x);
            NPKI = 0.125f * x + 0.875f * NPKI;

            if (sample_index >= warmup_samples) {
                threshold = NPKI + 0.25f * (SPKI - NPKI);
                warmed_up = true;
            }

            prev2 = prev1;
            prev1 = x;
            return -1;
        }

        int detected_index = -1;

        // Local maximum detection
        if (prev1 >= prev2 && prev1 >= x) {
            int peak_index = sample_index - 1;

            // R peak detection
            if (prev1 > threshold && (peak_index - last_r_index) >= min_rr_samples) {
                SPKI = 0.125f * prev1 + 0.875f * SPKI;   // update signal peak estimate
                last_r_index = peak_index;
                detected_index = peak_index;
            } else {
                NPKI = 0.125f * prev1 + 0.875f * NPKI;  // update noise estimate
            }

            threshold = NPKI + 0.25f * (SPKI - NPKI);
        }

        prev2 = prev1;
        prev1 = x;

        return detected_index;
    }

    float get_threshold() const { return threshold; }
    bool ready() const { return warmed_up; }
};
