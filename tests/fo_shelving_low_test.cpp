#include "tests_common.h"

// Text fixture for Class-Under-Test (CUT)
struct fo_shelving_low : public testing::Test {
	FO_SHELVING_LOW *m_filter;
	Biquad::tp_coeffs m_coeffs;
	void SetUp() {
		m_filter = new FO_SHELVING_LOW();
		m_coeffs = m_filter->calculate_coeffs(GAIN, FC, SAMPLING_RATE);
	}
	void TearDown() { delete m_filter; }
};

TEST_F(fo_shelving_low, coeff_calculation) {
	// Assert
	EXPECT_NEAR(m_coeffs.a0, 0.237348, 0.00001);
#ifdef DEBUG_OUTPUT
	debug_print(m_coeffs);
#endif
}

TEST_F(fo_shelving_low, process_0) {
	// Act
	auto yn = m_filter->process(TEST_SAMPLE_0);
	// Assert
	EXPECT_DOUBLE_EQ(yn, 0);
#ifdef DEBUG_OUTPUT
	std::cout << "yn: " << yn << std::endl;
#endif
}

TEST_F(fo_shelving_low, process_1) {
	// Act
	auto yn = m_filter->process(TEST_SAMPLE_1);
	// Assert
	EXPECT_NEAR(yn, 1.02896, 0.00001);
#ifdef DEBUG_OUTPUT
	std::cout << "yn: " << yn << std::endl;
#endif
}

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}
