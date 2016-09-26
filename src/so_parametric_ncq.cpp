#include <math.h>
#include "so_parametric_ncq.h"

SO_PARAMETRIC_NCQ::SO_PARAMETRIC_NCQ() {}

SO_PARAMETRIC_NCQ::~SO_PARAMETRIC_NCQ() {}

tp_coeffs SO_PARAMETRIC_NCQ::calculate_coeffs(float gain_db, float Q, int fc, int fs)
{
	F_SIZE w = 2.0 * pi * fc / fs;
	F_SIZE m = pow(10.0, gain_db / 20.0);
	F_SIZE z = 4.0 / (1.0 + m);
	F_SIZE b = 0.5 * ((1.0 - z*tan(w / (2.0*Q)) / (1 + z*tan(w / (2.0*Q)))));
	F_SIZE g = (0.5 + b) * cos(w);
	m_coeffs.a0 = 0.5 - b;
	m_coeffs.a1 = 0.0;
	m_coeffs.a2 = -(0.5 - b);
	m_coeffs.b1 = -2.0*g;
	m_coeffs.b2 = 2.0 * b;
	m_coeffs.c0 = m - 1.0;
	m_coeffs.d0 = 1.0;
	return(m_coeffs);
}

F_SIZE SO_PARAMETRIC_NCQ::filter(F_SIZE sample)
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