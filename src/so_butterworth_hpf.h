/**
* Second order Butterworth high-pass filter
* Dimitris Tassopoulos 2016
*
* fc, corner frequency
* Butterworth low-pass and high-pass filters are specialized versions of the ordinary secondorder
* low-pass filter. Their Q values are fixed at 0.707, which is the largest value it can
* assume before peaking in the frequency response is observed.
*/
#pragma once
#include "filter_common.h"

class SO_BUTTERWORTH_HPF {
public:
	SO_BUTTERWORTH_HPF();
	virtual ~SO_BUTTERWORTH_HPF();
	tp_coeffs calculate_coeffs(int fc, int fs = 44100);
	F_SIZE filter(F_SIZE sample);

private:
	F_SIZE m_xnz1, m_xnz2, m_ynz1, m_ynz2;
	tp_coeffs m_coeffs;
};
