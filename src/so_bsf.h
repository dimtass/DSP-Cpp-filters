/**
* Second order band-stop filter
* Dimitris Tassopoulos 2016-2020
*
* fc , corner frequency
* Q , quality factor controlling width of peak or notch = 1/BW
*/
#pragma once
#include "filter_common.h"

class SO_BSF : public Biquad {
public:
    tp_coeffs& calculate_coeffs(float Q, int fc, int fs)
    {
        coef_size_t w = 2.0 * pi * fc / fs;
        coef_size_t b = 0.5*((1.0 - tan(w / (2.0*Q))) / (1.0 + tan(w / (2.0*Q))));
        coef_size_t g = (0.5 + b)*cos(w);
        m_coeffs.a0 = 0.5 + b;
        m_coeffs.a1 = -2.0 * g;
        m_coeffs.a2 = 0.5 + b;
        m_coeffs.b1 = -2.0 * g;
        m_coeffs.b2 = 2.0 * b;
        return(std::ref(m_coeffs));
    }
};
