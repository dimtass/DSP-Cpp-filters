#include "tests_common.h"

// Text fixture for Class-Under-Test (CUT)
struct so_apf : public testing::Test {
	SO_APF *m_filter;
	Biquad::tp_coeffs m_coeffs;
	void SetUp() {
		m_filter = new SO_APF();
		m_coeffs = m_filter->calculate_coeffs(Q_DEF, FC, SAMPLING_RATE);
	}
	void TearDown() { delete m_filter; }
};

TEST_F(so_apf, coeff_calculation) {
	// Assert
	EXPECT_NEAR(m_coeffs.a0, 0.99993455, 0.00000001);
	EXPECT_NEAR(m_coeffs.b1, -1.89379828, 0.000001);
#ifdef DEBUG_OUTPUT
	debug_print(m_coeffs);
#endif
}

TEST_F(so_apf, process_0) {
	// Act
	auto yn = m_filter->process(TEST_SAMPLE_0);
	// Assert
	EXPECT_DOUBLE_EQ(yn, 0);
#ifdef DEBUG_OUTPUT
	std::cout << "yn: " << yn << std::endl;
#endif
}

TEST_F(so_apf, process_1) {
	// Act
	auto yn = m_filter->process(TEST_SAMPLE_1);
	// Assert
	// With zero delay state and unit input, y[0] = a0 = -alpha.
	// alpha = (tan(pi*Q/fs)-1)/(tan(pi*Q/fs)+1); equivalently,
	// a0 = (cos(theta)-sin(theta))/(cos(theta)+sin(theta)).
	// The old 0.71659 value used Q=FC=5000, not this fixture's Q=1.
	const double theta = std::acos(-1.0) * Q_DEF / SAMPLING_RATE;
	const double expected = (std::cos(theta) - std::sin(theta)) /
	                        (std::cos(theta) + std::sin(theta));
	EXPECT_NEAR(yn, expected, 0.00001);
#ifdef DEBUG_OUTPUT
	std::cout << "yn: " << yn << std::endl;
#endif
}

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}
