#include <math.h>
#include "so_butterworth_bpf.h"

SO_BUTTERWORTH_BPF::SO_BUTTERWORTH_BPF() {}

SO_BUTTERWORTH_BPF::~SO_BUTTERWORTH_BPF() {}

tp_coeffs SO_BUTTERWORTH_BPF::calculate_coeffs(float bw, int fc, int fs)
{
	F_SIZE c = 1.0 / (tan(pi*fc*bw / fs));
	F_SIZE d = 2.0 * cos(2.0 * pi * fc / fs);
	m_coeffs.a0 = 1.0 / (1.0 + c);
	m_coeffs.a1 = 0.0;
	m_coeffs.a2 = - m_coeffs.a0;
	m_coeffs.b1 = -m_coeffs.a0 * (c * d);
	m_coeffs.b2 = m_coeffs.a0 * (c - 1.0);
	return(m_coeffs);
}

F_SIZE SO_BUTTERWORTH_BPF::filter(F_SIZE sample)
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