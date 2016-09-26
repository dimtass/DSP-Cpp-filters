#include <math.h>
#include "fo_apf.h"

FO_APF::FO_APF() {}

FO_APF::~FO_APF() {}

tp_coeffs FO_APF::calculate_coeffs(int fc, int fs)
{
	F_SIZE a = (tan(pi*fc / fs) - 1.0) / (tan(pi*fc / fs) + 1.0);
	m_coeffs.a0 = a;
	m_coeffs.a1 = 1.0;
	m_coeffs.a2 = 0.0;
	m_coeffs.b1 = a;
	m_coeffs.b2 = 0.0;
	return(m_coeffs);
}

F_SIZE FO_APF::filter(F_SIZE sample)
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