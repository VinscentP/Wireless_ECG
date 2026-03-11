#pragma once

template<int MAX_PEAKS = 64>
class PeakDetector {
private:
    int   r_peak_indices[MAX_PEAKS];
    int   peak_head;
    int   peak_count;

    float prev_sample, current_sample, next_sample;
    int   sample_counter;
    int   refractory_period;
    int   last_peak_index;

public:
    PeakDetector(int fs)
        : prev_sample(0), current_sample(0), next_sample(0),
          sample_counter(0), peak_head(0), peak_count(0),
          refractory_period(static_cast<int>(fs * 0.2f)),
          last_peak_index(-10000) {}

    int peakDetection(float input) {
        sample_counter++;
        prev_sample    = current_sample;
        current_sample = next_sample;
        next_sample    = input;

        if (sample_counter < 3) return -1;
        if (sample_counter - last_peak_index < refractory_period) return -1;

        if (current_sample > prev_sample && current_sample > next_sample) {
            int peak_index = sample_counter - 1;
            r_peak_indices[peak_head % MAX_PEAKS] = peak_index;
            peak_head++;
            if (peak_count < MAX_PEAKS) peak_count++;
            last_peak_index = peak_index;
            return peak_index;
        }
        return -1;
    }

    // Returns most recent N peaks, newest-first
    int getPeak(int offset = 0) const {
        if (offset >= peak_count) return -1;
        int i = ((peak_head - 1 - offset) % MAX_PEAKS + MAX_PEAKS) % MAX_PEAKS;
        return r_peak_indices[i];
    }

    int getPeakCount() const { return peak_count; }

    void reset() {
        peak_head = 0; peak_count = 0;
        sample_counter = 0; last_peak_index = -10000;
    }
};