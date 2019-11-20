#include <math.h>
#include "so_linkwitz_riley_hpf.h"

SO_LINKWITZ_RILEY_HPF::SO_LINKWITZ_RILEY_HPF() {}

SO_LINKWITZ_RILEY_HPF::~SO_LINKWITZ_RILEY_HPF() {}

tp_coeffs SO_LINKWITZ_RILEY_HPF::calculate_coeffs(int fc, int fs)
{
	F_SIZE th = pi * fc / fs;
	F_SIZE Wc = pi * fc;
	F_SIZE k = Wc / tan(th);

	F_SIZE d = pow(k, 2.0) + pow(Wc, 2.0) + 2.0 * k * Wc;
	m_coeffs.a0 = pow(k, 2.0) / d;
	m_coeffs.a1 = -2.0 * pow(k, 2.0) / d;
	m_coeffs.a2 = m_coeffs.a0;
	m_coeffs.b1 = (-2.0 * pow(k, 2.0) + 2.0 * pow(Wc, 2.0)) / d;
	m_coeffs.b2 = (-2.0 * k * Wc + pow(k, 2.0) + pow(Wc, 2.0)) / d;
	return(m_coeffs);
}

F_SIZE SO_LINKWITZ_RILEY_HPF::filter(F_SIZE sample)
{
	F_SIZE xn = sample;
	F_SIZE yn = m_coeffs.a0*xn + m_coeffs.a1*m_xnz1 + m_coeffs.a2*m_xnz2
		- m_coeffs.b1*m_ynz1 - m_coeffs.b2*m_ynz2;

	m_xnz2 = m_xnz1;
	m_xnz1 = xn;
	m_ynz2 = m_ynz1;
	m_ynz1 = yn;

	return(yn);
}