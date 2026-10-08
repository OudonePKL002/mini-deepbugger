#include <stdio.h>
#include <assert.h>
#include "p11_fixed_point.h"

int main(void) {
    p11_q16_t a = p11_f01_fp_from_int(5);
    p11_q16_t b = p11_f01_fp_from_int(3);

    assert(p11_f02_fp_to_int(a) == 5);
    assert(p11_f02_fp_to_int(p11_f03_fp_add(a, b)) == 8);
    assert(p11_f02_fp_to_int(p11_f04_fp_sub(a, b)) == 2);
    assert(p11_f02_fp_to_int(p11_f05_fp_mul(a, b)) == 15);
    assert(p11_f02_fp_to_int(p11_f06_fp_div(a, b)) == 1);
    assert(p11_f07_fp_abs(-a) == a);

    p11_q16_t val16 = p11_f01_fp_from_int(16);
    assert(p11_f02_fp_to_int(p11_f08_fp_sqrt(val16)) == 4);

    p11_q16_t half = P11_FP_ONE / 2;
    p11_q16_t exp_half = p11_f09_fp_exp_taylor(half);
    assert(exp_half > P11_FP_ONE && exp_half < 2 * P11_FP_ONE);

    p11_q16_t sin_val = p11_f11_fp_sin_cordic(0);
    p11_q16_t cos_val = p11_f12_fp_cos_cordic(0);
    assert(p11_f07_fp_abs(sin_val) < 500); // approx 0
    assert(p11_f07_fp_abs(cos_val - P11_FP_ONE) < 500); // approx 1

    p11_q16_t lerped = p11_f14_fp_lerp(a, b, half);
    assert(p11_f02_fp_to_int(lerped) == 4);

    p11_q16_t clamped = p11_f15_fp_clamp(a, 0, b);
    assert(clamped == b);

    p11_q16_t poly_c[3] = {p11_f01_fp_from_int(1), p11_f01_fp_from_int(2), p11_f01_fp_from_int(3)};
    // 3*x^2 + 2*x + 1 for x=2 -> 12 + 4 + 1 = 17
    p11_q16_t p_res = p11_f16_fp_poly_eval(poly_c, 3, p11_f01_fp_from_int(2));
    assert(p11_f02_fp_to_int(p_res) == 17);

    printf("PASS: p11_fixed_point unit tests passed.\n");
    return 0;
}
