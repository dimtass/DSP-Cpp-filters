#include "filter_common.h"
#include "filter_includes.h"
#include <gtest/gtest.h>
#include <cmath>
#include <complex>
#include <vector>

namespace {

class ConfiguredModified : public BiquadModified {
public:
    explicit ConfiguredModified(const tp_coeffs& coeffs) { m_coeffs = coeffs; }
};

TEST(ModifiedBiquadRegression, SecondFeedbackUsesOutputHistory) {
    const Biquad::tp_coeffs coeffs = {1, 0, 0, 0, 0.5, 1, 0};
    ConfiguredModified filter(coeffs);
    const double expected[] = {1, 0, -0.5, 0, 0.25};
    for (int n = 0; n < 5; ++n) {
        EXPECT_DOUBLE_EQ(filter.process(n == 0 ? 1 : 0), expected[n]) << n;
    }
}

TEST(ModifiedBiquadRegression, FeedbackPrecedesWetDryMixing) {
    const Biquad::tp_coeffs coeffs = {1, 0, 0, -0.5, 0, 2, 1};
    ConfiguredModified filter(coeffs);
    const double expected[] = {3, 1, 0.5, 0.25};
    for (int n = 0; n < 4; ++n) {
        EXPECT_DOUBLE_EQ(filter.process(n == 0 ? 1 : 0), expected[n]) << n;
    }
}

} // namespace
