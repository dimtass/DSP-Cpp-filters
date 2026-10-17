#include "tests_common.h"

// Text fixture for Class-Under-Test (CUT)
struct so_parametric_ncq : public testing::Test {
	SO_PARAMETRIC_NCQ *m_filter;
	Biquad::tp_coeffs m_coeffs;
	void SetUp() {
		m_filter = new SO_PARAMETRIC_NCQ();
		m_coeffs = m_filter->calculate_coeffs(GAIN, Q_DEF, FC, SAMPLING_RATE);
	}
	void TearDown() { delete m_filter; }
};

TEST_F(so_parametric_ncq, coeff_calculation) {
	// Assert
	// Eq. 6.42: the entire beta numerator is divided by its denominator.
	EXPECT_NEAR(m_coeffs.a0, 0.2373482095659755, 0.00001);
#ifdef DEBUG_OUTPUT
	debug_print(m_coeffs);
#endif
}

TEST_F(so_parametric_ncq, process_0) {
	// Act
	auto yn = m_filter->process(TEST_SAMPLE_0);
	// Assert
	EXPECT_DOUBLE_EQ(yn, 0);
#ifdef DEBUG_OUTPUT
	std::cout << "yn: " << yn << std::endl;
#endif
}

TEST_F(so_parametric_ncq, process_1) {
	// Act
	auto yn = m_filter->process(TEST_SAMPLE_1);
	// Assert
	// With zero state, y[0] = d0 + c0*a0, not just the internal a0.
	EXPECT_NEAR(yn, 1.0289608616625787, 0.00001);
#ifdef DEBUG_OUTPUT
	std::cout << "yn: " << yn << std::endl;
#endif
}

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}
