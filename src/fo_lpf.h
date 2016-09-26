/**
* First order low-pass filter
* Dimitris Tassopoulos 2016
*/
#pragma once
#include "filter_common.h"

class FO_LPF {
public:
	FO_LPF();
	virtual ~FO_LPF();
	tp_coeffs calculate_coeffs(int fc, int fs = 44100);
	F_SIZE filter(F_SIZE sample);

private:
	F_SIZE m_xnz1, m_xnz2, m_ynz1, m_ynz2;
	tp_coeffs m_coeffs;
};