/**
* Second order all-pass filter
* Dimitris Tassopoulos 2016
*
* fc, corner frequency
* Q, steepness of phase shift at fc (second-order only)
*/
#pragma once
#include "filter_common.h"

class SO_APF {
public:
	SO_APF();
	virtual ~SO_APF();
	tp_coeffs calculate_coeffs(float Q, int fs = 44100);
	F_SIZE filter(F_SIZE sample);

private:
	F_SIZE m_xnz1, m_xnz2, m_ynz1, m_ynz2;
	tp_coeffs m_coeffs;
};