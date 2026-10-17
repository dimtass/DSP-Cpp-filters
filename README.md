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

## Errata and corrections

The first-edition source audit and repository fixes are documented in
[PR #9: correct audio filter equations, state and precision](https://github.com/dimtass/DSP-Cpp-filters/pull/9).
The earlier second-order APF cutoff fix is in
[PR #8](https://github.com/dimtass/DSP-Cpp-filters/pull/8).
These findings include both an unapplied book erratum and implementation bugs;
not every item below is an error in the book.

| Area | Finding | Fix and reference |
| --- | --- | --- |
| Modified biquad feedback | The second feedback term used input history, and recursive state stored the final wet/dry mix. This broke shelf responses and could cause instability. | Use both internal output delays and store the signal before mixing. First edition p.189, Fig.6.27; PR #9. |
| Butterworth BPF/BSF | The tangent argument retained the extra `fc` from the printed book typo, causing incorrect bandwidths and potentially unstable filters. | Use `tan(pi*bw/fs)` (or its reciprocal for BPF), as required by the author's p.186 erratum for Eq.6.38; PR #9. |
| Constant-Q boost/cut | `K = 2*pi*fc/fs` shifted the requested peak/notch frequency. | Restore `K = tan(pi*fc/fs)`. First edition p.193, Eq.6.43; PR #9. |
| Non-constant-Q beta | Only part of the numerator was divided by the denominator. | Use the complete fraction `0.5*(1-t)/(1+t)`. First edition p.192, Eq.6.42; PR #9. |
| Non-constant-Q processing | The ordinary biquad ignored `c0`/`d0`, so even 0 dB was not bypass. | Use the repaired modified biquad to apply `y = d0*x + c0*v`. First edition pp.189/192, Fig.6.27 and Eq.6.42; PR #9. |
| Precision selection | Header include order could change coefficient precision and class layout. | Select precision before defining `Biquad`; default consistently to double, allow explicit float, and reject conflicting macros. PR #9; rebuild all consumers with one consistent selection. |
| Second-order APF | The cutoff argument was missing and beta used `Q` instead of the cutoff angle. | Add `fc` and use `beta = -cos(2*pi*fc/fs)`, following the author's p.188 erratum for Eq.6.40. Fixed in PR #8; PR #9 preserves the first-edition width-like `Q` convention. |
| Linkwitz-Riley summation | The copied description omitted the polarity inversion needed for a flat LR2 electrical sum. | Document `LPF - HPF` (or the reverse), not direct `LPF + HPF`. Coefficients remain unchanged from p.186, Eq.6.39; PR #9. |
| Documentation and validation | The README showed the wrong second-feedback sign; shallow first-sample tests did not detect the recursive and coefficient bugs. | Correct both feedback signs (p.182, Eq.6.33), the high-shelf gain description and legacy expectations; add all-filter coefficient, impulse, pole, gain/phase and include-order regressions in default/double/float builds. PR #9 also restores root CTest discovery and current-CMake configuration. |

See the [official first-edition errata](https://www.willpirkle.com/support/errata/)
and the [complete source map and validation notes](docs/filter-validation.md).
The documented input domains remain caller preconditions: these fixes do not
add input clamping or rejection, and do not guarantee arbitrary extreme or
near-singular settings in float precision.

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
