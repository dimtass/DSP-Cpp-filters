/**
* Second order Linkwitz-Riley low-pass filter
* Dimitris Tassopoulos 2016
*
* fc , corner frequency (-6 dB)
* Second-order Linkwitz–Riley LPFs are designed to have an attenuation of -6 dB
* at the corner frequency rather than the standard -3 dB When these
* filters are placed in parallel with the same cutoff frequency, their outputs sum exactly and
* the resulting response is flat. They are often used in crossovers.
*/
#pragma once
#include "filter_common.h"

class SO_LINKWITZ_RILEY_BPF {
public:
	SO_LINKWITZ_RILEY_BPF();
	virtual ~SO_LINKWITZ_RILEY_BPF();
	tp_coeffs calculate_coeffs(int fc, int fs = 44100);
	F_SIZE filter(F_SIZE sample);

private:
	F_SIZE m_xnz1 = 0, m_xnz2 = 0, m_ynz1 = 0, m_ynz2 = 0;
	tp_coeffs m_coeffs;
};
