/**
* Second order parametric/peaking filter with non-constant-Q
* Dimitris Tassopoulos 2016
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
*
* This design creates an almost perfect constant-Q filter with only a small amount of error
* for low-boost (or cut) values.
*/
#pragma once
#include "filter_common.h"

class SO_PARAMETRIC_CQ_CUT {
public:
	SO_PARAMETRIC_CQ_CUT();
	virtual ~SO_PARAMETRIC_CQ_CUT();
	tp_coeffs calculate_coeffs(float gain_db, float Q, int fc, int fs = 44100);
	F_SIZE filter(F_SIZE sample);

private:
	F_SIZE m_xnz1, m_xnz2, m_ynz1, m_ynz2;
	tp_coeffs m_coeffs;
};