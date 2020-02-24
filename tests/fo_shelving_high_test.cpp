#include "tests_common.h"

// Text fixture for Class-Under-Test (CUT)
struct fo_shelving_high : public testing::Test {
	FO_SHELVING_HIGH *m_filter;
	Biquad::tp_coeffs m_coeffs;
	void SetUp() {
		m_filter = new FO_SHELVING_HIGH();
		m_coeffs = m_filter->calculate_coeffs(GAIN, FC, SAMPLING_RATE);
	}
	void TearDown() { delete m_filter; }
};

TEST_F(fo_shelving_high, coeff_calculation) {
	// Assert
	EXPECT_NEAR(m_coeffs.a0, 0.91946, 0.00001);
#ifdef DEBUG_OUTPUT
	debug_print(m_coeffs);
#endif
}

TEST_F(fo_shelving_high, process_0) {
	// Act
	auto yn = m_filter->process(TEST_SAMPLE_0);
	// Assert
	EXPECT_DOUBLE_EQ(yn, 0);
#ifdef DEBUG_OUTPUT
	std::cout << "yn: " << yn << std::endl;
#endif
}

TEST_F(fo_shelving_high, process_1) {
	// Act
	auto yn = m_filter->process(TEST_SAMPLE_1);
	// Assert
	EXPECT_NEAR(yn, 1.11219, 0.00001);
#ifdef DEBUG_OUTPUT
	std::cout << "yn: " << yn << std::endl;
#endif
}

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}
