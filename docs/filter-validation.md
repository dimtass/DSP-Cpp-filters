# Filter equations, errata and validation

## Authoritative source

These headers follow the **first edition** of Will Pirkle's *Designing Audio
Effect Plug-Ins in C++: With Digital Audio Signal Processing Theory*, ISBN
9780240825151, chapter 6, together with the author's corrections:

- [Book identification](https://www.amazon.com/Designing-Audio-Effect-Plug-Ins-Processing/dp/0240825152)
- [Official first-edition FX book errata](https://www.willpirkle.com/support/errata/)

Printed page numbers below refer to that edition, not PDF viewer numbering.
Later-edition example code is not interchangeable: in particular, its
second-order APF uses a different Q-to-bandwidth convention.

## Complete source map

| Header | Printed page | Equation / structure |
| --- | --- | --- |
| `fo_lpf.h` | 182 | 6.34, LPF column |
| `fo_hpf.h` | 182 | 6.34, HPF column |
| `so_lpf.h` | 183 | 6.35, LPF column |
| `so_hpf.h` | 183 | 6.35, HPF column |
| `so_bpf.h` | 184 | 6.36, BPF column |
| `so_bsf.h` | 184 | 6.36, BSF column |
| `so_butterworth_lpf.h` | 185 | 6.37, LPF column |
| `so_butterworth_hpf.h` | 185 | 6.37, HPF column |
| `so_butterworth_bpf.h` | 186 | 6.38, BPF column + p.186 erratum |
| `so_butterworth_bsf.h` | 186 | 6.38, BSF column + p.186 erratum |
| `so_linkwitz_riley_lpf.h` | 186 | 6.39, LPF column |
| `so_linkwitz_riley_hpf.h` | 186 | 6.39, HPF column |
| `fo_apf.h` | 188 | 6.40, first-order column |
| `so_apf.h` | 188 | 6.40, second-order column + p.188 errata |
| `fo_shelving_low.h` | 190 | 6.41, low shelf + Fig.6.27 on p.189 |
| `fo_shelving_high.h` | 190 | 6.41, high shelf + Fig.6.27 on p.189 |
| `so_parametric_ncq.h` | 192 | 6.42 + Fig.6.27 on p.189 |
| `so_parametric_cq_boost.h` | 193 | 6.43, boost column |
| `so_parametric_cq_cut.h` | 193 | 6.43, cut column |

## Corrections and their reasons

### Butterworth bandwidth: apply the p.186 erratum

The printed tangent argument incorrectly multiplies bandwidth by center
frequency. The author removes `fc` from that argument. For bandwidth `bw` in
Hz, the BPF uses `C = 1/tan(pi*bw/fs)` and the BSF uses
`C = tan(pi*bw/fs)`. The separate center-frequency cosine still contains `fc`.
The extra multiplication previously produced incorrect bandwidths and unstable
poles for otherwise valid inputs.

### Modified biquad: feedback precedes output mixing

Figure 6.27 places both feedback delays before the `c0` multiplier and the dry
input summation. With internal filtered signal `v`, the structure is:

    v[n] = a0*x[n] + a1*x[n-1] + a2*x[n-2]
           - b1*v[n-1] - b2*v[n-2]
    y[n] = d0*x[n] + c0*v[n]

Store `v`, not the mixed `y`, in recursive history. The second feedback delay
must also use internal output history, not input history. Both mistakes are
covered by synthetic multi-sample tests. The first error broke both shelves;
the second becomes active for second-order users of the modified kernel.

### Non-constant-Q peaking filter

Equation 6.42 divides the **entire** beta numerator by its denominator. It also
requires the modified processing path so `c0 = mu - 1` and `d0 = 1` actually
contribute to the result. At 0 dB this is identity, not a band-pass response.
Moving this filter to the modified kernel is safe only after correcting that
kernel's two history errors.

### Constant-Q peaking filters

Equation 6.43 requires `K = tan(pi*fc/fs)`, not `2*pi*fc/fs`. The latter shifted
the requested peak/notch frequency. Keep the book's signed-dB convention:
negative `gain_db` attenuates; positive `gain_db` boosts. The cut formula's
`10^(gain_db/20)` conversion is not an additional error.

### APF: preserve the cited edition

The p.188 errata define `theta_c = 2*pi*fc/fs` and correct a printed `a2`
label. The cutoff fix in PR #8 already applies this. The first-edition alpha
uses `tan(pi*Q/fs)`: its `Q` is a width-like phase control used in Hz, not the
usual dimensionless quality factor. Do not silently substitute the later
edition's `fc/Q` expression or change this API's control semantics.

### Linkwitz-Riley second-order summation

Each branch has magnitude 0.5 at `fc`, but the branch phases differ by 180
degrees. Flat electrical magnitude requires inverting one branch before
summation (`LPF - HPF` or `HPF - LPF`). Direct `LPF + HPF` cancels at `fc`.
This clarifies a condition omitted by the book's prose and copied comments;
the coefficient table itself does not require modification. No acoustic
speaker/crossover alignment is implied by this electrical test.

## Precision and compatibility

`filter_config.h` selects precision before `Biquad` is defined. Double is the
default; define `COEFF_SIZE_FLOAT` before the first library include or through
compiler flags to use float. Defining both precision macros is a compile-time
error. The old circular include could choose different coefficient types and
class layouts depending on header order. Use one selection consistently in
all translation units and **rebuild all consumers** after this change.

Corrected filters intentionally produce different coefficients and outputs
from the buggy versions. NCQ now derives from `BiquadModified`; call `process`
on the concrete filter type (processing is not virtual). Existing API argument
lists are otherwise unchanged by these fixes. The earlier PR #8 independently
added the `fc` argument to `SO_APF::calculate_coeffs(Q, fc, fs)`.

## Caller preconditions

The coefficient functions are lightweight, unchecked configuration APIs. This
change documents valid domains instead of introducing exceptions, arbitrary
clamping, or an error-handling ABI into embedded callers:

- `fs > 0`; frequencies in Hz satisfy `0 < fc < fs/2`.
- Ordinary quality factors are finite and positive.
- `SO_BPF`, `SO_BSF` and `SO_PARAMETRIC_NCQ` additionally require
  `fc/Q < fs/2`, following the tangent-domain warnings on pp.184 and 191.
- Butterworth band filters take finite bandwidth in Hz, `0 < bw < fs/2`.
- The first-edition SO_APF width-like control is finite and satisfies
  `0 < Q < fs/2`; it is not interchangeable with ordinary dimensionless Q.
- Gain arguments are finite signed dB. Choose representable, well-conditioned
  settings for the selected precision; extreme gain/Q combinations or values
  arbitrarily close to singular boundaries are not guaranteed numerically safe.
- Configure a fresh filter before processing it. Coefficient recalculation does
  not reset sample history; time-varying coefficients/click-free modulation are
  not validated by the static-response suite.

Invalid input can produce NaN, infinity or unstable filters. Float also has
less margin for very narrow filters and poles near the unit circle. These are
explicit limitations, not claims that every positive-Q input is safe.

## Automated verification

`filter_regression_test.cpp` maps every filter above to an independent oracle.
For LP/HP, Butterworth LP/HP, Linkwitz-Riley and constant-Q peaking filters, it
bilinear-transforms analog numerator/denominator polynomials rather than
copying the production assignments. The remaining oracles use equivalent
algebra from the inspected first-edition equations and corrected errata.

Each of the 19 filter types is checked with 18 configurations: three sample
rates, two center frequencies and three control settings. For each case the
suite compares all seven coefficient slots, 512 real `process` samples against
a separate internal-state recurrence, pole radii, and a 257-point complex
response grid. Focused runtime impulse DFTs check center-frequency peaking gain
and APF phase; NCQ 0 dB and LR2 signed-sum behavior have separate regressions.
An additional peaking sweep varies gain and Q independently at multiple sample
rates and centers, and a 100 Hz APF case exercises cancellation sensitivity in
both precisions without claiming safety for arbitrarily narrow float filters.

The suite runs in three builds: default precision, explicit double and explicit
float, with precision-appropriate tolerances. Separate multi-translation-unit
tests include the common header, biquad header and a concrete filter header in
different orders and compare coefficient sizes and class layouts. The original
single-sample tests remain, but their five affected fixtures now use expected
values independently evaluated from the corrected source, not old outputs.

These are finite sampled-domain checks, not an exhaustive mathematical proof
or a hardware/audio listening test. In particular, algebraic frequency-response
evaluation alone would not establish stability; the suite also checks poles
and executes the sample recurrence.

## Build and reproduce

    cmake -S . -B build -DBUILD_UNIT_TESTS=ON
    cmake --build build --parallel 2
    ctest --test-dir build --output-on-failure

CTest is enabled at the root, and the legacy `check` target also depends on all
regression executables. The nested GoogleTest downloader no longer requests
CMake compatibility older than 3.5, so current CMake does not need a policy
override. GoogleTest is still pinned to v1.14.0; tests select C++14 by default
without lowering a caller-selected standard. The filter library itself remains
header-only and C++11-compatible.
