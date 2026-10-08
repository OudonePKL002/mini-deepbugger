#include "query_component.h"
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

P11_NOINLINE p11_q16_t p11_f01_fp_from_int(int32_t val) {
    return val << P11_FP_SHIFT;
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

P11_NOINLINE p11_q16_t p11_f09_fp_exp_taylor(p11_q16_t x) {
    // 1 + x + x^2/2 + x^3/6 + x^4/24
    p11_q16_t sum = P11_FP_ONE + x;
    p11_q16_t term = x;
    for (int32_t i = 2; i <= 6; ++i) {
        term = p11_f05_fp_mul(term, x);
        sum = p11_f03_fp_add(sum, term / i);
    }
    return sum;
}

P11_NOINLINE p11_q16_t p11_f11_fp_sin_cordic(p11_q16_t theta) {
    p11_q16_t s = 0;
    cordic_kernel(theta, NULL, &s);
    return s;
}

P11_NOINLINE p11_q16_t p11_f13_fp_atan2_cordic(p11_q16_t y, p11_q16_t x) {
    if (x == 0) {
        if (y > 0) return P11_FP_PI / 2;
        if (y < 0) return -P11_FP_PI / 2;
        return 0;
    }
    p11_q16_t z = 0;
    p11_q16_t vx = (x < 0) ? -x : x;
    p11_q16_t vy = (x < 0) ? -y : y;

    for (int i = 0; i < 12; ++i) {
        p11_q16_t x_shift = vx >> i;
        p11_q16_t y_shift = vy >> i;
        if (vy > 0) {
            vx += y_shift;
            vy -= x_shift;
            z += cordic_angles[i];
        } else {
            vx -= y_shift;
            vy += x_shift;
            z -= cordic_angles[i];
        }
    }
    if (x < 0) {
        z = (y >= 0) ? (P11_FP_PI - z) : (-P11_FP_PI + z);
    }
    return z;
}

P11_NOINLINE p11_q16_t p11_f14_fp_lerp(p11_q16_t a, p11_q16_t b, p11_q16_t t) {
    // a + (b - a) * t
    return p11_f03_fp_add(a, p11_f05_fp_mul(p11_f04_fp_sub(b, a), t));
}

P11_NOINLINE p11_q16_t p11_f15_fp_clamp(p11_q16_t val, p11_q16_t min_v, p11_q16_t max_v) {
    if (val < min_v) return min_v;
    if (val > max_v) return max_v;
    return val;
}

P11_NOINLINE p11_q16_t p11_f16_fp_poly_eval(const p11_q16_t *coeffs, size_t deg, p11_q16_t x) {
    if (!coeffs || deg == 0) return 0;
    p11_q16_t res = coeffs[deg - 1];
    for (int i = (int)deg - 2; i >= 0; --i) {
        res = p11_f03_fp_add(p11_f05_fp_mul(res, x), coeffs[i]);
    }
    return res;
}
