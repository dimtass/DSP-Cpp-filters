/**
* Second order all-pass filter
* Dimitris Tassopoulos 2016-2020
*
* fc, corner frequency
* Q, steepness of phase shift at fc (second-order only)
*/
#pragma once
#include "filter_common.h"

class SO_APF : public Biquad {
public:
    tp_coeffs& calculate_coeffs(float Q, int fs)
    {
        coef_size_t a = (tan(pi*Q / fs) - 1.0) / (tan(pi*Q / fs) + 1.0);
        coef_size_t b = -cos(pi*Q / fs);
        m_coeffs.a0 = -a;
        m_coeffs.a1 = b*(1.0 - a);
        m_coeffs.a2 = 1.0;
        m_coeffs.b1 = m_coeffs.a1;
        m_coeffs.b2 = m_coeffs.a0;
        return(std::ref(m_coeffs));
    }
};