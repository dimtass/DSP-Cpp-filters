/**
* Second order high-pass filter
* Dimitris Tassopoulos 2016-2020
* 
* fc , corner frequency
* Q , quality factor controlling resonant peaking
*/
#pragma once
#include "filter_common.h"

class SO_HPF : public Biquad {
public:
    tp_coeffs& calculate_coeffs(float Q, int fc, int fs)
    {
        coef_size_t w = 2.0 * pi * fc / fs;
        coef_size_t d = 1.0 / Q;
        coef_size_t b = 0.5*(1.0 - (d / 2)*sin(w)) / (1.0 + (d / 2.0)*sin(w));
        coef_size_t g = (0.5 + b)*cos(w);
        m_coeffs.a0 = (0.5 + b + g) / 2.0;
        m_coeffs.a1 = -(0.5 + b + g);
        m_coeffs.a2 = m_coeffs.a0;
        m_coeffs.b1 = -2.0 * g;
        m_coeffs.b2 = 2.0 * b;
        return(std::ref(m_coeffs));
    }
};
