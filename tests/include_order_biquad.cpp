#include "biquad.h"
#include <cstddef>
std::size_t biquad_first_coefficient_size() { return sizeof(Biquad::coef_size_t); }
std::size_t biquad_first_layout_size() { return sizeof(Biquad); }
