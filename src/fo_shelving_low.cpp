#include <math.h>
#include "fo_shelving_low.h"

FO_LOW_SHELVING_F::FO_LOW_SHELVING_F() {}

FO_LOW_SHELVING_F::~FO_LOW_SHELVING_F() {}

tp_coeffs FO_LOW_SHELVING_F::calculate_coeffs(float gain_db, int fc, int fs)
{
	F_SIZE th = 2.0 * pi * fc / fs;
	F_SIZE m = pow(10.0, gain_db / 20.0);
	F_SIZE b = 4.0 / (1.0 + m);
	F_SIZE d = b * tan(th / 2.0);
	F_SIZE g = (1.0 - d) / (1.0 + d);
	m_coeffs.a0 = (1.0 - g) / 2.0;
	m_coeffs.a1 = (1.0 - g) / 2.0;
	m_coeffs.a2 = 0.0;
	m_coeffs.b1 = -g;
	m_coeffs.b2 = 0.0;
	m_coeffs.c0 = m - 1.0;
	m_coeffs.d0 = 1.0;
	return(m_coeffs);
}

F_SIZE FO_LOW_SHELVING_F::filter(F_SIZE sample)
{
	F_SIZE xn = sample;
	F_SIZE ynn = m_coeffs.a0*xn + m_coeffs.a1*m_xnz1 + m_coeffs.a2*m_xnz2
		- m_coeffs.b1*m_ynz1 - m_coeffs.b2*m_xnz2;
	F_SIZE yn = m_coeffs.d0*xn + m_coeffs.c0*ynn;

	m_xnz2 = m_xnz1;
	m_xnz1 = xn;
	m_xnz2 = m_ynz1;
	m_ynz1 = yn;

	return(yn);
}