#include <vector>
#include <cmath>       
using namespace std;

vector<double> raw_ecg; 

//Notch Filter
vector<double> notch_filter(vector<double> raw_ecg, double f_notch, double f_sample){
    vector<double> filtered_ecg;
    //H(z) = (1 - 2cos(ω₀)z⁻¹ + z⁻²) / (1 - 2r·cos(ω₀)z⁻¹ + r²z⁻²)
    double omega_0 = 2 * M_PI * f_notch / f_sample; //angular frequency 
    double r = 0.995; //rolloff factor, closer to 1 -> sharper transition band
    double cos_omega_0 = cos(omega_0);
    
    // b0, b1, b2, a0, a1, a2 are the coefficients of the difference equation
    double b0, b2, a0 = 1;
    double b1 = -2 * cos_omega_0;
    double a1 = 2 * r * cos_omega_0;
    double a2 = r * r;

    //apply filter using difference equation and return y[n] wbich is the filtered signal
    for(int n = 0; n < raw_ecg.size(); n++){
        filtered_ecg[n] = (b0 * raw_ecg[n] + b1 * raw_ecg[n-1] + b2 * raw_ecg[n-2] - a1 * filtered_ecg[n-1] - a2 * filtered_ecg[n-2]) / a0;
    }

    //apply filter using difference equation and return y[n] wbich is the filtered signal
    return filtered_ecg;
}