#include <vector>
#include <cmath>
#include "ecg_filters.h"
using namespace std;

// Notch Filter using a biquad (Direct Form II) implementation
// H(z) = (1 - 2cos(ω₀)z⁻¹ + z⁻²) / (1 - 2r·cos(ω₀)z⁻¹ + r²z⁻²)
vector<double> notch_filter(const vector<double>& raw_ecg, double f_notch, double f_sample){
    const double pi = 3.14159265358979323846;
    const double r = 0.995; // closer to 1.0 -> narrower notch
    const double omega_0 = 2.0 * pi * f_notch / f_sample;
    const double cos_omega_0 = cos(omega_0);
    const double b0 = 1.0;
    const double b1 = -2.0 * cos_omega_0;
    const double b2 = 1.0;
    const double a1 = -2.0 * r * cos_omega_0;
    const double a2 = r * r;

    vector<double> filtered_ecg(raw_ecg.size(), 0.0);

    // Direct Form II state
    double z1 = 0.0;
    double z2 = 0.0;

    for (size_t n = 0; n < raw_ecg.size(); ++n) {
        const double x = raw_ecg[n];
        const double y = b0 * x + z1;            // output
        const double z1_next = b1 * x - a1 * y + z2; // update state 1
        const double z2_next = b2 * x - a2 * y;      // update state 2
        filtered_ecg[n] = y;
        z1 = z1_next;
        z2 = z2_next;
    }

    return filtered_ecg;
}

vector<double> bandpass_filter(const vector<double>& raw_ecg, double f_low, double f_high, double f_sample){
    const double pi = 3.14159265358979323846;

    // Normalize frequencies
    double w1 = tan(pi * f_low / f_sample);
    double w2 = tan(pi * f_high / f_sample);

    // Band-pass center + bandwidth
    double Bw = w2 - w1;
    double W0 = sqrt(w1 * w2);

    // Pre-warped analog coefficients
    double B = Bw;
    double C = W0 * W0;

    // Bilinear transform denominator terms
    double A = B + 2.0;
    double denom = (4.0 + 2.0 * B + C);

    // Digital filter coefficients
    double b0 = 2.0 * B / denom;
    double b1 = 0.0;
    double b2 = -2.0 * B / denom;

    double a1 = (2.0 * C - 8.0) / denom;
    double a2 = (4.0 - 2.0 * B + C) / denom;

    vector<double> filtered(raw_ecg.size(), 0.0);

    // Direct Form II state
    double z1 = 0.0;
    double z2 = 0.0;

    for (size_t n = 0; n < raw_ecg.size(); ++n) {
        const double x = raw_ecg[n];

        double y = b0 * x + z1;

        double z1_next = b1 * x - a1 * y + z2;
        double z2_next = b2 * x - a2 * y;

        filtered[n] = y;
        z1 = z1_next;
        z2 = z2_next;
    }

    return filtered;
}
