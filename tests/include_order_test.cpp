#include "fo_shelving_low.h"
#include <gtest/gtest.h>
#include <cstddef>

std::size_t common_first_coefficient_size();
std::size_t common_first_layout_size();
std::size_t biquad_first_coefficient_size();
std::size_t biquad_first_layout_size();

TEST(PrecisionConfiguration, HeaderOrderPreservesTypeAndLayout) {
#ifdef COEFF_SIZE_FLOAT
    const std::size_t expected = sizeof(float);
#else
    const std::size_t expected = sizeof(double);
#endif
    EXPECT_EQ(sizeof(Biquad::coef_size_t), expected);
    EXPECT_EQ(common_first_coefficient_size(), expected);
    EXPECT_EQ(biquad_first_coefficient_size(), expected);
    EXPECT_EQ(common_first_layout_size(), sizeof(Biquad));
    EXPECT_EQ(biquad_first_layout_size(), sizeof(Biquad));
}
