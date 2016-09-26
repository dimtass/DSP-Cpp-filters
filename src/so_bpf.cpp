#include <math.h>
#include "so_bpf.h"

SO_BPF::SO_BPF() {}

SO_BPF::~SO_BPF() {}

tp_coeffs SO_BPF::calculate_coeffs(float Q, int fc, int fs)
{
	F_SIZE w = 2.0 * pi * fc / fs;
	F_SIZE b = 0.5*((1.0 - tan(w / (2.0*Q))) / (1.0 + tan(w / (2.0*Q))));
	F_SIZE g = (0.5 + b)*cos(w);
	m_coeffs.a0 = 0.5 - b;
	m_coeffs.a1 = 0.0;
	m_coeffs.a2 = -(0.5 - b);
	m_coeffs.b1 = -2.0 * g;
	m_coeffs.b2 = 2.0 * b;
	return(m_coeffs);
}

F_SIZE SO_BPF::filter(F_SIZE sample)
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