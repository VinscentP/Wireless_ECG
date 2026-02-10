#pragma once
#include <algorithm>

class AdaptiveThreshold {
private:
    float SPKI;              // Signal peak estimate
    float NPKI;              // Noise peak estimate
    float peak_threshold;
    float noise_threshold;

    int fs;
    int sample_index;
    int last_r_index;
    int min_rr_samples;

    int skip_samples;        // Startup skip
    bool initialized;

    // For local maximum detection
    float prev2;
    float prev1;

    // Parameters
    float alpha;             // 0.125
    float threshold_factor;  // e.g. 0.25

public:
    AdaptiveThreshold(int sampling_rate,
                               float thresh_factor = 0.25f,
                               int skip_initial = 200)
        : fs(sampling_rate),
          sample_index(0),
          last_r_index(0),
          skip_samples(skip_initial),
          initialized(false),
          prev2(0.0f),
          prev1(0.0f),
          alpha(0.125f),
          threshold_factor(thresh_factor)
    {
        min_rr_samples = static_cast<int>(0.2f * fs);   // 200 ms
        last_r_index = -min_rr_samples;

        SPKI = 0.0f;
        NPKI = 0.0f;
        peak_threshold = 0.0f;
        noise_threshold = 0.0f;
    }

    // Process ONE sample
    // Returns R-peak index or -1
    int process(float x) {
        sample_index++;

        // -------------------------------------------------
        // 1. Startup skip + learning phase
        // -------------------------------------------------
        if (sample_index <= skip_samples) {
            // Learn rough signal/noise scale
            SPKI = std::max(SPKI, x);
            NPKI = alpha * x + (1.0f - alpha) * NPKI;

            prev2 = prev1;
            prev1 = x;
            return -1;
        }

        // Initialize thresholds once
        if (!initialized) {
            peak_threshold  = NPKI + threshold_factor * (SPKI - NPKI);
            noise_threshold = 0.5f * peak_threshold;
            initialized = true;
        }

        int detected_index = -1;

        // -------------------------------------------------
        // 2. Local maximum detection
        // -------------------------------------------------
        if (prev1 > prev2 && prev1 > x) {
            int peak_index = sample_index - 1;

            // -------------------------------------------------
            // 3. Adaptive threshold decision
            // -------------------------------------------------
            if (prev1 > peak_threshold &&
                (peak_index - last_r_index) >= min_rr_samples) {

                // QRS detected
                detected_index = peak_index;
                SPKI = alpha * prev1 + (1.0f - alpha) * SPKI;
                last_r_index = peak_index;

            } else {
                // Noise
                NPKI = alpha * prev1 + (1.0f - alpha) * NPKI;
            }

            // Update thresholds
            peak_threshold  = NPKI + threshold_factor * (SPKI - NPKI);
            noise_threshold = 0.5f * peak_threshold;
        }

        prev2 = prev1;
        prev1 = x;

        return detected_index;
    }

    float get_threshold() const { return peak_threshold; }
};
