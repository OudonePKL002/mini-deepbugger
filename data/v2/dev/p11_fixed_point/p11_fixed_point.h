#ifndef P11_FIXED_POINT_H
#define P11_FIXED_POINT_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define P11_NOINLINE __attribute__((noinline))
#define P11_FP_SHIFT 16
#define P11_FP_ONE   (1 << P11_FP_SHIFT)
#define P11_FP_PI    205887  // 3.14159265 in Q16.16

typedef int32_t p11_q16_t;

P11_NOINLINE p11_q16_t p11_f01_fp_from_int(int32_t val);
P11_NOINLINE int32_t   p11_f02_fp_to_int(p11_q16_t val);
P11_NOINLINE p11_q16_t p11_f03_fp_add(p11_q16_t a, p11_q16_t b);
P11_NOINLINE p11_q16_t p11_f04_fp_sub(p11_q16_t a, p11_q16_t b);
P11_NOINLINE p11_q16_t p11_f05_fp_mul(p11_q16_t a, p11_q16_t b);
P11_NOINLINE p11_q16_t p11_f06_fp_div(p11_q16_t a, p11_q16_t b);
P11_NOINLINE p11_q16_t p11_f07_fp_abs(p11_q16_t a);
P11_NOINLINE p11_q16_t p11_f08_fp_sqrt(p11_q16_t a);
P11_NOINLINE p11_q16_t p11_f09_fp_exp_taylor(p11_q16_t x);
P11_NOINLINE p11_q16_t p11_f10_fp_ln_approx(p11_q16_t x);
P11_NOINLINE p11_q16_t p11_f11_fp_sin_cordic(p11_q16_t theta);
P11_NOINLINE p11_q16_t p11_f12_fp_cos_cordic(p11_q16_t theta);
P11_NOINLINE p11_q16_t p11_f13_fp_atan2_cordic(p11_q16_t y, p11_q16_t x);
P11_NOINLINE p11_q16_t p11_f14_fp_lerp(p11_q16_t a, p11_q16_t b, p11_q16_t t);
P11_NOINLINE p11_q16_t p11_f15_fp_clamp(p11_q16_t val, p11_q16_t min_v, p11_q16_t max_v);
P11_NOINLINE p11_q16_t p11_f16_fp_poly_eval(const p11_q16_t *coeffs, size_t deg, p11_q16_t x);

#endif
