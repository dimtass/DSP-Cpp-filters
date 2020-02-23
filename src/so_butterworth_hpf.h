/**
* Second order Butterworth high-pass filter
* Dimitris Tassopoulos 2016-2020
*
* fc, corner frequency
* Butterworth low-pass and high-pass filters are specialized versions of the ordinary secondorder
* low-pass filter. Their Q values are fixed at 0.707, which is the largest value it can
* assume before peaking in the frequency response is observed.
*/
#pragma once
#include "filter_common.h"

class SO_BUTTERWORTH_HPF : public Biquad {
public:
    tp_coeffs& calculate_coeffs(int fc, int fs)
    {
        coef_size_t c = tan(pi*fc / fs);
        m_coeffs.a0 = 1.0 / (1.0 + sqrt2*c + pow(c, 2.0));
        m_coeffs.a1 = -2.0 * m_coeffs.a0;
        m_coeffs.a2 = m_coeffs.a0;
        m_coeffs.b1 = 2.0 * m_coeffs.a0*(pow(c, 2.0) - 1.0);
        m_coeffs.b2 = m_coeffs.a0 * (1.0 - sqrt2*c + pow(c, 2.0));
        return(std::ref(m_coeffs));
    }
};
