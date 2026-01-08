#include <vector>
#include <cmath>
#include "ecg_filters.h"
using namespace std;

// Notch Filter using a biquad (Direct Form II) implementation
// H(z) = (1 - 2cos(ω₀)z⁻¹ + z⁻²) / (1 - 2r·cos(ω₀)z⁻¹ + r²z⁻²)
vector<float> notch_filter(const vector<float>& raw_ecg, float f_notch, float f_sample){
    const float pi = 3.14159265358979323846;
    const float r = 0.995; // closer to 1.0 -> narrower notch
    const float omega_0 = 2.0 * pi * f_notch / f_sample;
    const float cos_omega_0 = cos(omega_0);
    const float b0 = 1.0;
    const float b1 = -2.0 * cos_omega_0;
    const float b2 = 1.0;
    const float a1 = -2.0 * r * cos_omega_0;
    const float a2 = r * r;

    vector<float> filtered_ecg(raw_ecg.size(), 0.0);

    // Direct Form II state
    float z1 = 0.0;
    float z2 = 0.0;

    for (size_t n = 0; n < raw_ecg.size(); ++n) {
        const float x = raw_ecg[n];
        const float y = b0 * x + z1;            // output
        const float z1_next = b1 * x - a1 * y + z2; // update state 1
        const float z2_next = b2 * x - a2 * y;      // update state 2
        filtered_ecg[n] = y;
        z1 = z1_next;
        z2 = z2_next;
    }

    return filtered_ecg;
}

vector<float> bandpass_filter(const vector<float>& raw_ecg, float f_low, float f_high, float f_sample){
    const float pi = 3.14159265358979323846;

    // Normalize frequencies
    float w1 = tan(pi * f_low / f_sample);
    float w2 = tan(pi * f_high / f_sample);

    // Band-pass center + bandwidth
    float Bw = w2 - w1;
    float W0 = sqrt(w1 * w2);

    // Pre-warped analog coefficients
    float B = Bw;
    float C = W0 * W0;

    // Bilinear transform denominator terms
    float A = B + 2.0;
    float denom = (4.0 + 2.0 * B + C);

    // Digital filter coefficients
    float b0 = 2.0 * B / denom;
    float b1 = 0.0;
    float b2 = -2.0 * B / denom;

    float a1 = (2.0 * C - 8.0) / denom;
    float a2 = (4.0 - 2.0 * B + C) / denom;

    vector<float> filtered(raw_ecg.size(), 0.0);

    // Direct Form II state
    float z1 = 0.0;
    float z2 = 0.0;

    for (size_t n = 0; n < raw_ecg.size(); ++n) {
        const float x = raw_ecg[n];

        float y = b0 * x + z1;

        float z1_next = b1 * x - a1 * y + z2;
        float z2_next = b2 * x - a2 * y;

        filtered[n] = y;
        z1 = z1_next;
        z2 = z2_next;
    }

    return filtered;
}
