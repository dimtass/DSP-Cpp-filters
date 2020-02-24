/**
* Second order Linkwitz-Riley low-pass filter
* Dimitris Tassopoulos 2016-2020
*
* fc , corner frequency (-6 dB)
* Second-order Linkwitz/Riley LPFs are designed to have an attenuation of -6 dB
* at the corner frequency rather than the standard -3 dB When these
* filters are placed in parallel with the same cutoff frequency, their outputs sum exactly and
* the resulting response is flat. They are often used in crossovers.
*/
#pragma once
#include "filter_common.h"

class SO_LINKWITZ_RILEY_LPF : public Biquad {
public:
    tp_coeffs& calculate_coeffs(int fc, int fs)
    {
        coef_size_t th = pi * fc / fs;
        coef_size_t Wc = pi * fc;
        coef_size_t k = Wc / tan(th);

        coef_size_t d = pow(k, 2.0) + pow(Wc, 2.0) + 2.0 * k * Wc;
        m_coeffs.a0 = pow(Wc, 2.0) / d;
        m_coeffs.a1 = 2.0 * pow(Wc, 2.0) / d;
        m_coeffs.a2 = m_coeffs.a0;
        m_coeffs.b1 = (-2.0 * pow(k, 2.0) + 2.0 * pow(Wc, 2.0)) / d;
        m_coeffs.b2 = (-2.0 * k * Wc + pow(k, 2.0) + pow(Wc, 2.0)) / d;
        return(std::ref(m_coeffs));
    }
};
