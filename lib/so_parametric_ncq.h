/**
* Second order parametric/peaking filter with non-constant-Q
* Dimitris Tassopoulos 2016-2020
*
* fc , center frequency
* Q quality factor
* Gain/attenuation in dB
* Parametric EQs allow you to adjust the center frequency, Q and boost or cut creating any
* arbitrary bumps or notches in the frequency response. The parametric EQ is a
* variation on the ordinary band-pass and band-stop fi lters that generates symmetrical boost/
* cut curves and mixes in the dry signal to create the fi nal response. A true digital parametric
* EQ not only has independent controls, but each control only varies one coeffi cient in the
* fi lter. The parametric EQs in this section afford the same frequency response but adjustments
* in any parameter require a recalculation of all the coeffi cients. These fi lters are also called
* peaking fi lters.
* This parametric EQ is not constant-Q, which means the bandwidth varies depending on the
* boost/cut value. Some analog fi lters have the same issue, although there is occasional debate
* over whether or not this is desirable in an EQ design.
*/
#pragma once
#include "filter_common.h"

class SO_PARAMETRIC_NCQ : public Biquad {
public:
    tp_coeffs& calculate_coeffs(float gain_db, float Q, int fc, int fs)
    {
        coef_size_t w = 2.0 * pi * fc / fs;
        coef_size_t m = pow(10.0, gain_db / 20.0);
        coef_size_t z = 4.0 / (1.0 + m);
        coef_size_t b = 0.5 * ((1.0 - z*tan(w / (2.0*Q)) / (1 + z*tan(w / (2.0*Q)))));
        coef_size_t g = (0.5 + b) * cos(w);
        m_coeffs.a0 = 0.5 - b;
        m_coeffs.a1 = 0.0;
        m_coeffs.a2 = -(0.5 - b);
        m_coeffs.b1 = -2.0*g;
        m_coeffs.b2 = 2.0 * b;
        m_coeffs.c0 = m - 1.0;
        m_coeffs.d0 = 1.0;
		return(std::ref(m_coeffs));
    }
};