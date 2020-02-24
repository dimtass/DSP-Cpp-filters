
#include <iostream>
#include <memory>
#include <gtest/gtest.h>
#include "filter_common.h"
#include "filter_includes.h"

// #define DEBUG_OUTPUT

#define BW_DEF			1000.0	// Default bandwidth
#define Q_DEF			1.0
#define GAIN			1.0
#define FC				5000
#define SAMPLING_RATE	96000
#define TEST_SAMPLE_0	0
#define TEST_SAMPLE_1	1.0

static inline void debug_print(Biquad::tp_coeffs &coeffs)
{
	std::cout << "Coeffs: " << std::endl;
	std::cout << "\ta0: " << coeffs.a0 << std::endl;
	std::cout << "\ta1: " << coeffs.a1 << std::endl;
	std::cout << "\ta2: " << coeffs.a2 << std::endl;
	std::cout << "\tb1: " << coeffs.b1 << std::endl;
	std::cout << "\tb2: " << coeffs.b2 << std::endl;
}