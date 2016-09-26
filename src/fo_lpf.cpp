#include <math.h>
#include "fo_lpf.h"

FO_LPF::FO_LPF() {}

FO_LPF::~FO_LPF() {}

tp_coeffs FO_LPF::calculate_coeffs(int fc, int fs)
{
	F_SIZE th = 2.0 * pi * fc / fs;
	F_SIZE g = cos(th) / (1.0 + sin(th));
	m_coeffs.a0 = (1.0 - g) / 2.0;
	m_coeffs.a1 = (1.0 - g) / 2.0;
	m_coeffs.a2 = 0.0;
	m_coeffs.b1 = -g;
	m_coeffs.b2 = 0.0;
	return(m_coeffs);
}

F_SIZE FO_LPF::filter(F_SIZE sample)
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