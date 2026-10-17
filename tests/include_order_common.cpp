#include "filter_common.h"
#include <cstddef>
std::size_t common_first_coefficient_size() { return sizeof(Biquad::coef_size_t); }
std::size_t common_first_layout_size() { return sizeof(Biquad); }
