/**
* First order high-shelving filter
* Dimitris Tassopoulos 2016
*
* fc , high shelf frequency
* Low-frequency gain/attenuation in dB
* Shelving filters are used in many tone controls, especially when there are only two, bass
* and treble, which are almost always implemented as shelf types. The filters have a corner
* frequency and gain or attenuation value.
*/
#pragma once
#include "filter_common.h"
#include "biquad_modified.h"

class FO_SHELVING_HIGH : public BiquadModified {
public:
	tp_coeffs& calculate_coeffs(float gain_db, int fc, int fs)
	{
		coef_size_t th = 2.0 * pi * fc / fs;
		coef_size_t m = pow(10.0, gain_db / 20.0);
		coef_size_t b = (1.0 + m) / 4.0;
		coef_size_t d = b * tan(th / 2.0);
		coef_size_t g = (1.0 - d) / (1.0 + d);
		m_coeffs.a0 = (1.0 + g) / 2.0;
		m_coeffs.a1 = -((1.0 + g) / 2.0);
		m_coeffs.a2 = 0.0;
		m_coeffs.b1 = -g;
		m_coeffs.b2 = 0.0;
		m_coeffs.c0 = m - 1.0;
		m_coeffs.d0 = 1.0;
		return(std::ref(m_coeffs));
	}
};