/**
* Second order Butterworth low-pass filter
* Dimitris Tassopoulos 2016-2020
*
* fc, corner frequency
* Butterworth low-pass and high-pass filters are specialized versions of the ordinary secondorder
* low-pass filter. Their Q values are fixed at 0.707, which is the largest value it can
* assume before peaking in the frequency response is observed.
*/
#pragma once
#include "filter_common.h"
#include <complex>

class SO_BUTTERWORTH_LPF : public Biquad {
public:
    tp_coeffs& calculate_coeffs(int fc, int fs)
    {
        coef_size_t c = 1.0 / (std::tan(pi*fc / fs));
        m_coeffs.a0 = 1.0 / (1.0 + sqrt2*c + std::pow(c, 2.0) );
        m_coeffs.a1 = 2.0 * m_coeffs.a0;
        m_coeffs.a2 = m_coeffs.a0;
        m_coeffs.b1 = 2.0 * m_coeffs.a0*(1.0 - std::pow(c, 2.0));
        m_coeffs.b2 = m_coeffs.a0 * (1.0 - sqrt2*c + std::pow(c, 2.0) );
        return(std::ref(m_coeffs));
    }
};
