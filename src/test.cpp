//============================================================================
// Name        : cpp_001.cpp
// Author      : dimtass
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <memory>
#include "filter_common.h"
#include "filter_includes.h"


int main() {
	std::unique_ptr<FO_LOW_SHELVING_F> filter (new FO_LOW_SHELVING_F);

	auto coeffs = filter->calculate_coeffs(1.0, 5000, 96000);
	auto yn = filter->process(0.3);

	std::cout << "Coeffs: " << std::endl;
	std::cout << "a0: " << coeffs.a0 << std::endl;
	std::cout << "a1: " << coeffs.a1 << std::endl;
	std::cout << "a2: " << coeffs.a2 << std::endl;
	std::cout << "b1: " << coeffs.b1 << std::endl;
	std::cout << "b2: " << coeffs.b2 << std::endl;

	std::cout << "yn: " << yn << std::endl;

	return 0;
}
