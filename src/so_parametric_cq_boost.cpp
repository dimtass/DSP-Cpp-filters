#include <math.h>
#include "so_parametric_cq_boost.h"

SO_PARAMETRIC_CQ_BOOST::SO_PARAMETRIC_CQ_BOOST() {}

SO_PARAMETRIC_CQ_BOOST::~SO_PARAMETRIC_CQ_BOOST() {}

tp_coeffs SO_PARAMETRIC_CQ_BOOST::calculate_coeffs(float gain_db, float Q, int fc, int fs)
{
	F_SIZE K = 2.0 * pi * fc / fs;
	F_SIZE V0 = pow(10.0, gain_db / 20.0);
	F_SIZE dd = 1.0 + K/Q + pow(K, 2.0);
	F_SIZE e = 1.0 + K/(V0*Q) + pow(K, 2.0);
	F_SIZE a = 1.0 + (V0*K)/Q + pow(K, 2.0);
	F_SIZE b = 2.0*(pow(K, 2.0) - 1.0);
	F_SIZE g = 1.0 - (V0*K)/Q + pow(K, 2.0);
	F_SIZE d = 1.0 - K/Q + pow(K, 2.0);
	F_SIZE h = 1.0 - K / (V0*Q) + pow(K, 2.0);
	m_coeffs.a0 = a/dd;
	m_coeffs.a1 = b/dd;
	m_coeffs.a2 = g/dd;
	m_coeffs.b1 = b/dd;
	m_coeffs.b2 = d/dd;
	m_coeffs.c0 = 1.0;
	m_coeffs.d0 = 0.0;
	return(m_coeffs);
}

F_SIZE SO_PARAMETRIC_CQ_BOOST::filter(F_SIZE sample)
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