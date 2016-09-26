#include <math.h>
#include "so_lpf.h"

SO_LPF::SO_LPF() {}

SO_LPF::~SO_LPF() {}

tp_coeffs SO_LPF::calculate_coeffs(float Q, int fc, int fs)
{
	F_SIZE w = 2.0 * pi * fc / fs;
	F_SIZE d = 1.0 / Q;
	F_SIZE b = 0.5*(1.0 - (d / 2)*sin(w)) / (1.0 + (d / 2.0)*sin(w));
	F_SIZE g = (0.5 + b)*cos(w);
	m_coeffs.a0 = (0.5 + b - g) / 2.0;
	m_coeffs.a1 = 0.5 + b - g;
	m_coeffs.a2 = m_coeffs.a0;
	m_coeffs.b1 = -2.0 * g;
	m_coeffs.b2 = 2.0 * b;
	return(m_coeffs);
}

F_SIZE SO_LPF::filter(F_SIZE sample)
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