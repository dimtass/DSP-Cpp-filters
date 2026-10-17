#pragma once

// Select one precision consistently for every translation unit in a program.
// Default to double; define COEFF_SIZE_FLOAT before including any library header
// (or via compiler flags) to opt into single precision.
#if defined(COEFF_SIZE_DOUBLE) && defined(COEFF_SIZE_FLOAT)
#error "Define only one of COEFF_SIZE_DOUBLE and COEFF_SIZE_FLOAT"
#elif !defined(COEFF_SIZE_DOUBLE) && !defined(COEFF_SIZE_FLOAT)
#define COEFF_SIZE_DOUBLE
#endif
