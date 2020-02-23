/**
* Second order Butterworth band-pass filter
* Dimitris Tassopoulos 2016-2020
*
* fc , corner frequency
* BW , bandwidth of peak/notch = fc/Q
* Butterworth BPF and BSF are made by cascading (BPF) or paralleling (BSF) a Butterworth
* LPF and Butterworth HPF.
*/
#pragma once
#include "filter_common.h"

class SO_BUTTERWORTH_BPF : public Biquad {
public:
    tp_coeffs& calculate_coeffs(float bw, int fc, int fs)
    {
        coef_size_t c = 1.0 / (tan(pi*fc*bw / fs));
        coef_size_t d = 2.0 * cos(2.0 * pi * fc / fs);
        m_coeffs.a0 = 1.0 / (1.0 + c);
        m_coeffs.a1 = 0.0;
        m_coeffs.a2 = - m_coeffs.a0;
        m_coeffs.b1 = -m_coeffs.a0 * (c * d);
        m_coeffs.b2 = m_coeffs.a0 * (c - 1.0);
        return(std::ref(m_coeffs));
    }
};
