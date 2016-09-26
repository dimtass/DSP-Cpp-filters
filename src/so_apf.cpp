#include <math.h>
#include "so_apf.h"

SO_APF::SO_APF() {}

SO_APF::~SO_APF() {}

tp_coeffs SO_APF::calculate_coeffs(float Q, int fs)
{
	F_SIZE a = (tan(pi*Q / fs) - 1.0) / (tan(pi*Q / fs) + 1.0);
	F_SIZE b = -cos(pi*Q / fs);
	m_coeffs.a0 = -a;
	m_coeffs.a1 = b*(1.0 - a);
	m_coeffs.a2 = 1.0;
	m_coeffs.b1 = m_coeffs.a1;
	m_coeffs.b2 = m_coeffs.a0;
	return(m_coeffs);
}

F_SIZE SO_APF::filter(F_SIZE sample)
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