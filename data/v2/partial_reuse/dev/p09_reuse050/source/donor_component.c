#include "donor_component.h"
#include <string.h>
#include <math.h>

/* Support helper: cordic_kernel */
// CORDIC lookup table for angles in Q16.16 (atan(2^-i))
static const p11_q16_t cordic_angles[12] = {
    51471, // atan(1)   = 0.785398 rad
    30385, // atan(0.5) = 0.463647
    16054, // atan(0.25)
    8149,  // atan(0.125)
    4090,  // atan(0.0625)
    2047,
    1023,
    511,
    255,
    127,
    63,
    31
};
#ifndef CORDIC_GAIN
#define CORDIC_GAIN 39796 // ~0.607252935 in Q16.16
#endif

static void cordic_kernel(p11_q16_t theta, p11_q16_t *out_cos, p11_q16_t *out_sin) {
    p11_q16_t x = CORDIC_GAIN;
    p11_q16_t y = 0;
    p11_q16_t z = theta;

    for (int i = 0; i < 12; ++i) {
        p11_q16_t x_shift = x >> i;
        p11_q16_t y_shift = y >> i;
        if (z >= 0) {
            x -= y_shift;
            y += x_shift;
            z -= cordic_angles[i];
        } else {
            x += y_shift;
            y -= x_shift;
            z += cordic_angles[i];
        }
    }
    if (out_cos) *out_cos = x;
    if (out_sin) *out_sin = y;
}

P11_NOINLINE int32_t p11_f02_fp_to_int(p11_q16_t val) {
    return val >> P11_FP_SHIFT;
}

P11_NOINLINE p11_q16_t p11_f03_fp_add(p11_q16_t a, p11_q16_t b) {
    return a + b;
}

P11_NOINLINE p11_q16_t p11_f04_fp_sub(p11_q16_t a, p11_q16_t b) {
    return a - b;
}

P11_NOINLINE p11_q16_t p11_f05_fp_mul(p11_q16_t a, p11_q16_t b) {
    int64_t prod = (int64_t)a * (int64_t)b;
    return (p11_q16_t)(prod >> P11_FP_SHIFT);
}

P11_NOINLINE p11_q16_t p11_f06_fp_div(p11_q16_t a, p11_q16_t b) {
    if (b == 0) return (a >= 0) ? INT32_MAX : INT32_MIN;
    int64_t num = ((int64_t)a) << P11_FP_SHIFT;
    return (p11_q16_t)(num / b);
}

P11_NOINLINE p11_q16_t p11_f07_fp_abs(p11_q16_t a) {
    return (a < 0) ? -a : a;
}

P11_NOINLINE p11_q16_t p11_f11_fp_sin_cordic(p11_q16_t theta) {
    p11_q16_t s = 0;
    cordic_kernel(theta, NULL, &s);
    return s;
}

P11_NOINLINE p11_q16_t p11_f15_fp_clamp(p11_q16_t val, p11_q16_t min_v, p11_q16_t max_v) {
    if (val < min_v) return min_v;
    if (val > max_v) return max_v;
    return val;
}
