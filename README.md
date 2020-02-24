DSP filters in C++
----

[![dimtass](https://circleci.com/gh/dimtass/DSP-Cpp-filters.svg?style=svg)](https://circleci.com/gh/dimtass/DSP-Cpp-filters)

This repo contains some DSP biquad filters used in audio. I've extracted those
filters from the `Designing Audio Effect Plug-Ins in C++: With Digital Audio Signal Processing Theory`
book that you can find [here](https://www.amazon.com/Designing-Audio-Effect-Plug-Ins-Processing/dp/0240825152).

Not all those filters are tested, but I think that the high-pass filters don't work
very well.

This is the formula I'm using for the [digital biquad filter](https://en.wikipedia.org/wiki/Digital_biquad_filter)
in the source code:
```
y(n) = a0*x(n) + a1*x(n-1) + a2*x(n-2) - b*y(n-1) + b2*y(n-2)
```

- First order all-pass filter (fo_apf)
- First order high-pass filter (fo_hpf)
- First order low-pass filter (fo_lpf)
- First order high-shelving filter (fo_shelving_high)
- First order low-shelving filter (fo_shelving_low)
- Second order all-pass filter (so_apf)
- Second order band-pass filter (so_bpf)
- Second order band-stop filter (so_bsf)
- Second order Butterworth band-pass filter (so_butterworth_bpf)
- Second order Butterworth band-stop filter (so_butterworth_bsf)
- Second order Butterworth high-pass filter (so_butterworth_hpf)
- Second order Butterworth low-pass filter (so_butterworth_lpf)
- Second order high-pass filter (so_hpf)
- Second order Linkwitz-Riley high-pass filter (so_linkwitz_riley_hpf)
- Second order Linkwitz-Riley low-pass filter (so_linkwitz_riley_lpf)
- Second order Low-pass filter (so_lpf)
- Second order parametric/peaking boost filter with constant-Q (so_parametric_cq_boost)
- Second order parametric/peaking cut filter with constant-Q (so_parametric_cq_cut)
- Second order parametric/peaking filter with non-constant-Q (so_parametric_ncq)

## Build & run tests
You can use cmake to build the tests. On Linux, you can just run this:
```sh
./build_tests.sh
```

The above command will build the tests and run them.

> Note: Tests are a bit naive in this case, since it doesn't make
much sense for testing using unit-tests, but anyways, I've added them

## Usage:
The filters can be used in your C++ code in the part where the audio sample is about to be processed. You need to include the _filter_common.h_ and _filter_includes.h_ files and the create an object with filter(s) you want to apply and calculate the coefficients with the calculate_coeffs() function. Then in the sample processing function run the filter() function with the current sample as a parameter.

I've used [RackAFX](http://www.willpirkle.com/rackafx/) to test these filters.

## Code example
To use the so-LPF filter add this in to your main.cpp file

```cpp
#include "filter_common.h"
#include "filter_includes.h"

int main() {
	std::unique_ptr<SO_LPF> filter (new SO_LPF);

	auto coeffs = filter->calculate_coeffs(5000, 96000);
	auto yn = filter->process(0.303);

	std::cout << "Coeffs: " << std::endl;
	std::cout << "a0: " << coeffs.a0 << std::endl;
	std::cout << "a1: " << coeffs.a1 << std::endl;
	std::cout << "a2: " << coeffs.a2 << std::endl;
	std::cout << "b1: " << coeffs.b1 << std::endl;
	std::cout << "b2: " << coeffs.b2 << std::endl;

	std::cout << "yn: " << yn << std::endl;
	return 0;
}
```

This is will print the filter coefficients and then will process
a sample with the value `0.303`