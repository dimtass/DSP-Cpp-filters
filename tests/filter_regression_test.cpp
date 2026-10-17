#include "filter_common.h"
#include "filter_includes.h"
#include <gtest/gtest.h>
#include <array>
#include <algorithm>
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

// Independent first-edition oracles. See docs/filter-validation.md for the
// page/equation map; do not use later-edition Q conventions here.
using Coefficients = std::array<double, 7>;
const double pi = std::acos(-1.0);
const bool single_precision = sizeof(Biquad::coef_size_t) == sizeof(float);
const double coefficient_tolerance = single_precision ? 2e-6 : 1e-12;
const double impulse_tolerance = single_precision ? 5e-5 : 1e-10;
const double response_tolerance = single_precision ? 3e-3 : 1e-8;

const char* const filter_names[] = {
    "fo_apf", "fo_hpf", "fo_lpf", "fo_shelving_high", "fo_shelving_low",
    "so_apf", "so_bpf", "so_bsf", "so_butterworth_bpf",
    "so_butterworth_bsf", "so_butterworth_hpf", "so_butterworth_lpf",
    "so_hpf", "so_linkwitz_riley_hpf", "so_linkwitz_riley_lpf", "so_lpf",
    "so_parametric_cq_boost", "so_parametric_cq_cut", "so_parametric_ncq"
};

Coefficients coefficients(const Biquad::tp_coeffs& c) {
    return {{c.a0, c.a1, c.a2, c.b1, c.b2, c.c0, c.d0}};
}

// Bilinear-transform an analog quadratic ratio with prewarping k.
Coefficients bilinear(const std::array<double, 3>& num,
                      const std::array<double, 3>& den, double k) {
    auto transform = [k](const std::array<double, 3>& p) {
        return std::array<double, 3>{{p[0] / (k*k) + p[1] / k + p[2],
            -2*p[0] / (k*k) + 2*p[2], p[0] / (k*k) - p[1] / k + p[2]}};
    };
    const auto a = transform(num);
    const auto b = transform(den);
    return {{a[0]/b[0], a[1]/b[0], a[2]/b[0], b[1]/b[0], b[2]/b[0], 0, 0}};
}

Coefficients reference(int kind, double q, double gain, double bandwidth,
                       int fc, int fs) {
    const double k = std::tan(pi*fc/fs);
    const double mu = std::pow(10.0, gain/20.0);
    if (kind == 0) { // Eq. 6.40, first-order APF
        const double a = (k-1)/(k+1);
        return {{a, 1, 0, a, 0, 0, 0}};
    }
    if (kind == 1 || kind == 2 || kind == 3 || kind == 4) {
        // Eqs. 6.34/6.41 via the equivalent bilinear form.
        const bool high = kind == 1 || kind == 3;
        const bool shelf = kind == 3 || kind == 4;
        const double t = k * (shelf ? (high ? (1+mu)/4 : 4/(1+mu)) : 1);
        const double a = (high ? 1 : t)/(1+t);
        return {{a, high ? -a : a, 0, (t-1)/(t+1), 0,
                 shelf ? mu-1 : 0, shelf ? 1.0 : 0.0}};
    }
    if (kind == 5) { // Eq. 6.40 + p.188 errata: q is width-like, not fc/Q.
        const double t = std::tan(pi*q/fs);
        const double r = (1-t)/(1+t);
        const double b = -std::cos(2*pi*fc/fs)*(1+r);
        return {{r, b, 1, b, r, 0, 0}};
    }
    if (kind >= 6 && kind <= 9) { // Eqs. 6.36/6.38 + p.186 erratum
        const double width = kind >= 8 ? bandwidth : fc/q;
        const double t = std::tan(pi*width/fs);
        const double b1 = -2*std::cos(2*pi*fc/fs)/(1+t);
        const double b2 = (1-t)/(1+t);
        if (kind == 6 || kind == 8)
            return {{t/(1+t), 0, -t/(1+t), b1, b2, 0, 0}};
        return {{1/(1+t), b1, 1/(1+t), b1, b2, 0, 0}};
    }
    if (kind >= 10 && kind <= 15) { // Eqs. 6.35/6.37/6.39
        const bool high = kind == 10 || kind == 12 || kind == 13;
        const double damping = kind == 10 || kind == 11 ? std::sqrt(2.0) :
                               kind == 13 || kind == 14 ? 2.0 : 1/q;
        return bilinear(high ? std::array<double, 3>{{1, 0, 0}} :
                               std::array<double, 3>{{0, 0, 1}},
                        {{1, damping, 1}}, k);
    }
    if (kind == 16 || kind == 17) { // Eq. 6.43, signed dB for cut
        auto c = kind == 16 ? bilinear({{1, mu/q, 1}}, {{1, 1/q, 1}}, k) :
                              bilinear({{1, 1/q, 1}}, {{1, 1/(mu*q), 1}}, k);
        c[5] = 1;
        return c;
    }
    // Eq. 6.42: internal band-pass plus the dry signal, Fig. 6.27.
    const double t = 4/(1+mu)*std::tan(pi*fc/(fs*q));
    const double beta = 0.5*(1-t)/(1+t);
    return {{0.5-beta, 0, beta-0.5,
             -2*(0.5+beta)*std::cos(2*pi*fc/fs), 2*beta, mu-1, 1}};
}

std::complex<double> response(const Coefficients& c, double w, bool mixed) {
    const auto z = std::exp(std::complex<double>(0, -w));
    const auto h = (c[0]+c[1]*z+c[2]*z*z)/(1.0+c[3]*z+c[4]*z*z);
    return mixed ? c[6]+c[5]*h : h;
}

template<class Filter, class... Args>
void compare_with_source(int kind, const Coefficients& expected, Args... args) {
    Filter filter;
    const auto actual = coefficients(filter.calculate_coeffs(args...));
    for (std::size_t i = 0; i < actual.size(); ++i) {
        ASSERT_NEAR(actual[i], expected[i], coefficient_tolerance) << "coefficient " << i;
    }
    const bool mixed = kind == 3 || kind == 4 || kind >= 16;
    double x1 = 0, x2 = 0, v1 = 0, v2 = 0;
    for (int n = 0; n < 512; ++n) {
        const double x = n == 0 ? 1 : 0;
        const double v = expected[0]*x + expected[1]*x1 + expected[2]*x2 -
                         expected[3]*v1 - expected[4]*v2;
        const double y = mixed ? expected[6]*x+expected[5]*v : v;
        const double actual_y = filter.process(static_cast<Biquad::coef_size_t>(x));
        ASSERT_TRUE(std::isfinite(actual_y)) << n;
        ASSERT_NEAR(actual_y, y, impulse_tolerance) << "sample " << n;
        x2 = x1; x1 = x; v2 = v1; v1 = v;
    }
    const auto disc = std::sqrt(std::complex<double>(actual[3]*actual[3]-4*actual[4], 0));
    EXPECT_LT(std::abs((-actual[3]+disc)/2.0), 1);
    EXPECT_LT(std::abs((-actual[3]-disc)/2.0), 1);
    for (int i = 0; i <= 256; ++i) {
        EXPECT_LT(std::abs(response(actual, pi*i/256, mixed)-
                           response(expected, pi*i/256, mixed)), response_tolerance);
    }
}

class BookSourceRegression : public testing::TestWithParam<int> {};

TEST_P(BookSourceRegression, CoefficientsImpulseAndResponseMatchFirstEdition) {
    const int kind = GetParam();
    for (int fs : {44100, 48000, 96000}) for (int fc : {1000, 5000}) {
        for (int variant = 0; variant < 3; ++variant) {
            const float q = kind == 5 ? static_cast<float>(100*(variant+1)) :
                            variant == 0 ? 0.5f : variant == 1 ? 1.0f : 4.0f;
            const float gain = static_cast<float>(12*(variant-1));
            const float bw = variant == 0 ? 100.0f : variant == 1 ? 1000.0f : 5000.0f;
            SCOPED_TRACE(testing::Message() << filter_names[kind] << " fs=" << fs
                         << " fc=" << fc << " Q=" << q << " gain=" << gain << " BW=" << bw);
            const auto r = reference(kind, q, gain, bw, fc, fs);
            switch (kind) {
            case 0: compare_with_source<FO_APF>(kind, r, fc, fs); break;
            case 1: compare_with_source<FO_HPF>(kind, r, fc, fs); break;
            case 2: compare_with_source<FO_LPF>(kind, r, fc, fs); break;
            case 3: compare_with_source<FO_SHELVING_HIGH>(kind, r, gain, fc, fs); break;
            case 4: compare_with_source<FO_SHELVING_LOW>(kind, r, gain, fc, fs); break;
            case 5: compare_with_source<SO_APF>(kind, r, q, fc, fs); break;
            case 6: compare_with_source<SO_BPF>(kind, r, q, fc, fs); break;
            case 7: compare_with_source<SO_BSF>(kind, r, q, fc, fs); break;
            case 8: compare_with_source<SO_BUTTERWORTH_BPF>(kind, r, bw, fc, fs); break;
            case 9: compare_with_source<SO_BUTTERWORTH_BSF>(kind, r, bw, fc, fs); break;
            case 10: compare_with_source<SO_BUTTERWORTH_HPF>(kind, r, fc, fs); break;
            case 11: compare_with_source<SO_BUTTERWORTH_LPF>(kind, r, fc, fs); break;
            case 12: compare_with_source<SO_HPF>(kind, r, q, fc, fs); break;
            case 13: compare_with_source<SO_LINKWITZ_RILEY_HPF>(kind, r, fc, fs); break;
            case 14: compare_with_source<SO_LINKWITZ_RILEY_LPF>(kind, r, fc, fs); break;
            case 15: compare_with_source<SO_LPF>(kind, r, q, fc, fs); break;
            case 16: compare_with_source<SO_PARAMETRIC_CQ_BOOST>(kind, r, gain, q, fc, fs); break;
            case 17: compare_with_source<SO_PARAMETRIC_CQ_CUT>(kind, r, gain, q, fc, fs); break;
            case 18: compare_with_source<SO_PARAMETRIC_NCQ>(kind, r, gain, q, fc, fs); break;
            }
        }
    }
}

INSTANTIATE_TEST_SUITE_P(AllFilters, BookSourceRegression, testing::Range(0, 19),
    [](const testing::TestParamInfo<int>& info) { return filter_names[info.param]; });

TEST(FilterResponseRegression, NonConstantQZeroDbIsIdentity) {
    SO_PARAMETRIC_NCQ filter;
    filter.calculate_coeffs(0, 1, 1000, 48000);
    for (int n = 0; n < 256; ++n) {
        const auto x = static_cast<Biquad::coef_size_t>(std::sin(0.3*n));
        EXPECT_NEAR(filter.process(x), x, impulse_tolerance);
    }
}

template<class Filter>
std::complex<double> impulse_response(Filter& filter, double w) {
    std::complex<double> sum(0, 0);
    for (int n = 0; n < 8192; ++n) {
        const double y = filter.process(n == 0 ? 1 : 0);
        EXPECT_TRUE(std::isfinite(y)) << n;
        sum += y*std::exp(std::complex<double>(0, -w*n));
    }
    return sum;
}

TEST(FilterResponseRegression, PeakingGainIsAtRequestedCenter) {
    for (float gain : {-12.0f, 0.0f, 12.0f}) {
        SO_PARAMETRIC_CQ_BOOST boost;
        SO_PARAMETRIC_CQ_CUT cut;
        SO_PARAMETRIC_NCQ ncq;
        boost.calculate_coeffs(gain, 2, 1000, 48000);
        cut.calculate_coeffs(gain, 2, 1000, 48000);
        ncq.calculate_coeffs(gain, 2, 1000, 48000);
        const double expected = std::pow(10.0, gain/20.0);
        for (auto measured : {impulse_response(boost, 2*pi*1000/48000),
                              impulse_response(cut, 2*pi*1000/48000),
                              impulse_response(ncq, 2*pi*1000/48000)}) {
            EXPECT_NEAR(std::abs(measured), expected, response_tolerance);
        }
    }
}

TEST(FilterResponseRegression, AllPassCenterPhaseAndMagnitude) {
    FO_APF first;
    SO_APF second;
    first.calculate_coeffs(1000, 48000);
    second.calculate_coeffs(500, 1000, 48000);
    EXPECT_LT(std::abs(impulse_response(first, 2*pi*1000/48000)-
                       std::complex<double>(0, -1)), response_tolerance);
    EXPECT_LT(std::abs(impulse_response(second, 2*pi*1000/48000)+1.0), response_tolerance);
}

TEST(FilterResponseRegression, LinkwitzRileyNeedsOppositePolarity) {
    SO_LINKWITZ_RILEY_LPF low;
    SO_LINKWITZ_RILEY_HPF high;
    const auto l = coefficients(low.calculate_coeffs(1000, 48000));
    const auto h = coefficients(high.calculate_coeffs(1000, 48000));
    EXPECT_LT(std::abs(response(l, 2*pi*1000/48000, false)+
                       response(h, 2*pi*1000/48000, false)), response_tolerance);
    for (int n = 0; n <= 256; ++n) {
        EXPECT_NEAR(std::abs(response(l, pi*n/256, false)-
                             response(h, pi*n/256, false)), 1, response_tolerance);
    }
}

TEST(FilterResponseRegression, PeakingControlsVaryIndependently) {
    for (int fs : {48000, 96000}) for (int fc : {1000, 5000}) {
        for (float q : {0.5f, 1.0f, 2.0f, 8.0f}) {
            for (float gain : {-12.0f, -6.0f, 0.0f, 6.0f, 12.0f}) {
                SCOPED_TRACE(testing::Message() << "fs=" << fs << " fc=" << fc
                             << " Q=" << q << " gain=" << gain);
                compare_with_source<SO_PARAMETRIC_CQ_BOOST>(16,
                    reference(16, q, gain, 0, fc, fs), gain, q, fc, fs);
                compare_with_source<SO_PARAMETRIC_CQ_CUT>(17,
                    reference(17, q, gain, 0, fc, fs), gain, q, fc, fs);
                compare_with_source<SO_PARAMETRIC_NCQ>(18,
                    reference(18, q, gain, 0, fc, fs), gain, q, fc, fs);
            }
        }
    }
}

TEST(FilterResponseRegression, LowFrequencyAllPassCenterPhase) {
    SO_APF filter;
    const auto c = coefficients(filter.calculate_coeffs(500, 100, 96000));
    // Use a finite width with margin for float cancellation near DC. This is
    // not a guarantee for arbitrarily narrow, near-unit-pole float settings.
    EXPECT_LT(std::abs(response(c, 2*pi*100/96000, false)+1.0), response_tolerance);
}

} // namespace
