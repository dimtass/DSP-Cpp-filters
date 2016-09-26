/**
* Second order Butterworth band-pass filter
* Dimitris Tassopoulos 2016
*
* fc , corner frequency
* BW , bandwidth of peak/notch = fc/Q
* Butterworth BPF and BSF are made by cascading (BPF) or paralleling (BSF) a Butterworth
* LPF and Butterworth HPF.
*/
#pragma once
#include "filter_common.h"

class SO_BUTTERWORTH_BPF {
public:
	SO_BUTTERWORTH_BPF();
	virtual ~SO_BUTTERWORTH_BPF();
	tp_coeffs calculate_coeffs(float bw, int fc, int fs = 44100);
	F_SIZE filter(F_SIZE sample);

private:
	F_SIZE m_xnz1, m_xnz2, m_ynz1, m_ynz2;
	tp_coeffs m_coeffs;
};
