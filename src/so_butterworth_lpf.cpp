#include <math.h>
#include "so_butterworth_lpf.h"

SO_BUTTERWORTH_LPF::SO_BUTTERWORTH_LPF() {}

SO_BUTTERWORTH_LPF::~SO_BUTTERWORTH_LPF() {}

tp_coeffs SO_BUTTERWORTH_LPF::calculate_coeffs(int fc, int fs)
{
	F_SIZE c = 1.0 / (tan(pi*fc / fs));
	m_coeffs.a0 = 1.0 / (1.0 + sqrt2*c + pow(c, 2.0));
	m_coeffs.a1 = 2.0 * m_coeffs.a0;
	m_coeffs.a2 = m_coeffs.a0;
	m_coeffs.b1 = 2.0 * m_coeffs.a0*(1.0 - pow(c, 2.0));
	m_coeffs.b2 = m_coeffs.a0 * (1.0 - sqrt2*c + pow(c, 2.0));
	return(m_coeffs);
}

F_SIZE SO_BUTTERWORTH_LPF::filter(F_SIZE sample)
{
	F_SIZE xn = sample;
	F_SIZE yn = m_coeffs.a0*xn + m_coeffs.a1*m_xnz1 + m_coeffs.a2*m_xnz2
		- m_coeffs.b1*m_ynz1 - m_coeffs.b2*m_xnz2;

	m_xnz2 = m_xnz1;
	m_xnz1 = xn;
	m_xnz2 = m_ynz1;
	m_ynz1 = yn;

	return(yn);
}