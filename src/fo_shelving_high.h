/**
* First order low-shelving filter
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

class FO_HIGH_SHELVING_F {
public:
	FO_HIGH_SHELVING_F();
	virtual ~FO_HIGH_SHELVING_F();
	tp_coeffs calculate_coeffs(float gain_db, int fc, int fs = 44100);
	F_SIZE filter(F_SIZE sample);

private:
	F_SIZE m_xnz1, m_xnz2, m_ynz1, m_ynz2;
	tp_coeffs m_coeffs;
};