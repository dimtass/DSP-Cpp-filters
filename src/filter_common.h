#pragma once
#ifndef FILTER_COMMON_H__
#define FILTER_COMMON_H__

#define F_COEFF_SIZE_DOUBLE
//#define F_COEFF_SIZE_FLOAT

#ifdef F_COEFF_SIZE_DOUBLE
#define F_SIZE double
#elif F_COEFF_SIZE_FLOAT
#define F_SIZE float
#endif

#ifndef pi
#define  pi 3.1415926535897932384626433832795
#endif

#ifndef sqrt2
#define sqrt2 (2.0 * 0.707106781186547524401)
#endif

#ifndef sqrt2over2
#define  sqrt2over2  0.707106781186547524401
#endif

typedef struct {
	F_SIZE a0;
	F_SIZE a1;
	F_SIZE a2;
	F_SIZE b1;
	F_SIZE b2;
	F_SIZE c0;
	F_SIZE d0;
} tp_coeffs;


#endif