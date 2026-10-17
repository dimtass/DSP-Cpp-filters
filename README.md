DSP filters in C++
----

[![Build and Test](https://github.com/dimtass/DSP-Cpp-filters/actions/workflows/build-and-test.yml/badge.svg)](https://github.com/dimtass/DSP-Cpp-filters/actions/workflows/build-and-test.yml)

This repo contains some DSP biquad filters used in audio. I've extracted those
filters from the `Designing Audio Effect Plug-Ins in C++: With Digital Audio Signal Processing Theory`
book that you can find [here](https://www.amazon.com/Designing-Audio-Effect-Plug-Ins-Processing/dp/0240825152).

I've also implemented a real-time testing on a Cortex-M4 MCU, using the on-chip ADC and DAC. You can find the post [here](https://www.stupid-projects.com/biquad-audio-dsp-filters-using-stm32f303cc-black-pill/).

This is the formula I'm using for the [digital biquad filter](https://en.wikipedia.org/wiki/Digital_biquad_filter)
in the source code:
```
y(n) = a0*x(n) + a1*x(n-1) + a2*x(n-2) - b1*y(n-1) - b2*y(n-2)
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

All the filters are now header files and they are located in the `lib/` folder.
In order to use them just copy the `lib/` folder (and rename it if needed) into
your project folder. There's an example how to build later in the README.

## Build & run tests
You can use cmake to build the tests. On Linux, you can just run this:
```sh
./build-tests.sh
```

The above command will build the tests and run them. Alternatively, on Linux or
macOS, use an out-of-source build:

```sh
cmake -S . -B build -DBUILD_UNIT_TESTS=ON
cmake --build build --parallel 2
ctest --test-dir build --output-on-failure
```

The tests compare all 19 filters with independent first-edition coefficient
oracles and multi-sample impulse responses. Additional checks cover stability,
center-frequency gain/phase, zero-dB bypass, crossover polarity and consistent
header inclusion in default, double and float precision. Tests require C++14
for GoogleTest; the filter headers remain usable with C++11.

## Precision and parameter conventions

Double precision is the default, independent of header include order. Define
`COEFF_SIZE_FLOAT` before including any library header (or in compiler flags)
to opt into float. Do not define both precision macros, and use the same
selection in every translation unit. Rebuild all consumers after changing it.
Very narrow filters and poles close to the unit circle are more sensitive to
single-precision rounding.

Inputs must satisfy the documented frequency, bandwidth and Q preconditions;
the coefficient APIs do not clamp or reject invalid inputs. In particular,
`SO_BPF`, `SO_BSF` and `SO_PARAMETRIC_NCQ` require `fc/Q < fs/2`.
`SO_APF` preserves the first edition's width-like `Q` control in
`tan(pi*Q/fs)`; it is not the later edition's conventional `fc/Q` convention.
For a second-order Linkwitz-Riley crossover, invert one branch before adding
it to the other (`LPF - HPF`) to obtain a flat electrical magnitude response.

See [filter validation and source references](docs/filter-validation.md) for
all parameter domains, the equation map, corrected errata, behavior changes
and validation limits.

## Usage:
The filters can be used in your C++ code in the part where the audio sample is about to be processed. You need to include the _filter_common.h_ and _filter_includes.h_ files and the create an object with filter(s) you want to apply and calculate the coefficients with the calculate_coeffs() function. Then in the sample processing function call the concrete filter's process() function with the current sample as a parameter.

I've used [RackAFX](http://www.willpirkle.com/rackafx/) to test these filters.

## Code example
For example, to use the so-LPF filter then first create a `main.cpp` file
in the top directory of this repo.

```sh
touch main.cpp
```

Then add this code inside:

```cpp
#include <iostream>
#include <memory>
#include "filter_common.h"
#include "filter_includes.h"

int main() {
    std::unique_ptr<SO_LPF> filter (new SO_LPF);

    auto coeffs = filter->calculate_coeffs(1.0, 5000, 96000);
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

Now to build the file run:
```sh
g++ main.cpp -I./lib
```

And then run the executable:
```sh
./a.out
```

This is will print the filter coefficients and then will process
a sample with the value `0.303`. You should see an output similar
to this:

```
Coeffs: 
a0: 0.0228608
a1: 0.0457215
a2: 0.0228608
b1: -1.63163
b2: 0.723069
yn: 0.00692681
```
