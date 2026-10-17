/**
* Second order all-pass filter
* Dimitris Tassopoulos 2016-2020
*
* fc, corner frequency
* Q, first-edition width-like phase control used in tan(pi*Q/fs), in Hz.
* This is not the conventional dimensionless Q or the later-edition fc/Q.
*/
#pragma once
#include "filter_common.h"

class SO_APF : public Biquad {
public:
    tp_coeffs& calculate_coeffs(float Q, int fc, int fs)
    {
        coef_size_t a = (tan(dcf_pi*Q / fs) - 1.0) / (tan(dcf_pi*Q / fs) + 1.0);
        coef_size_t b = -cos(2.0 * dcf_pi * fc / fs);
        m_coeffs.a0 = -a;
        m_coeffs.a1 = b*(1.0 - a);
        m_coeffs.a2 = 1.0;
        m_coeffs.b1 = m_coeffs.a1;
        m_coeffs.b2 = m_coeffs.a0;
        return(std::ref(m_coeffs));
    }
};