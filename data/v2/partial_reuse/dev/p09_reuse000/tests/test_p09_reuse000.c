#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "query_component.h"
#include "donor_component.h"

static void test_p11_f01_fp_from_int(void) {
    assert(p11_f01_fp_from_int(5) == (5 << P11_FP_SHIFT));
}

static void test_p11_f02_fp_to_int(void) {
    assert(p11_f02_fp_to_int(5 << P11_FP_SHIFT) == 5);
}

static void test_p11_f03_fp_add(void) {
    assert(p11_f03_fp_add(10, 20) == 30);
}

static void test_p11_f04_fp_sub(void) {
    assert(p11_f04_fp_sub(30, 10) == 20);
}

static void test_p11_f05_fp_mul(void) {
    p11_q16_t a = 2 * 65536, b = 3 * 65536; assert(p11_f05_fp_mul(a, b) == 6 * 65536);
}

static void test_p11_f06_fp_div(void) {
    p11_q16_t a = 6 * 65536, b = 2 * 65536; assert(p11_f06_fp_div(a, b) == 3 * 65536);
}

static void test_p11_f07_fp_abs(void) {
    assert(p11_f07_fp_abs(-50) == 50);
}

static void test_p11_f08_fp_sqrt(void) {
    p11_q16_t v = 4 * 65536; assert(p11_f08_fp_sqrt(v) == 2 * 65536);
}

static void test_p11_f09_fp_exp_taylor(void) {
    p11_q16_t half = P11_FP_ONE / 2; assert(p11_f09_fp_exp_taylor(half) > P11_FP_ONE);
}

static void test_p11_f10_fp_ln_approx(void) {
    p11_q16_t v = 65536; assert(p11_f10_fp_ln_approx(v) == 0);
}

static void test_p11_f11_fp_sin_cordic(void) {
    assert(abs(p11_f11_fp_sin_cordic(0)) < 100);
}

static void test_p11_f12_fp_cos_cordic(void) {
    assert(abs(p11_f12_fp_cos_cordic(0) - 65536) < 100);
}

static void test_p11_f13_fp_atan2_cordic(void) {
    assert(abs(p11_f13_fp_atan2_cordic(65536, 65536) - 51471) < 200);
}

static void test_p11_f14_fp_lerp(void) {
    p11_q16_t a = 65536, b = 3 * 65536, t = 32768; assert(p11_f14_fp_lerp(a, b, t) == 2 * 65536);
}

static void test_p11_f15_fp_clamp(void) {
    assert(p11_f15_fp_clamp(15, 0, 10) == 10);
}

static void test_p11_f16_fp_poly_eval(void) {
    p11_q16_t c[2] = {65536, 2 * 65536}; assert(p11_f16_fp_poly_eval(c, 2, 65536) == 3 * 65536);
}

int main(void) {
    test_p11_f01_fp_from_int();
    test_p11_f02_fp_to_int();
    test_p11_f03_fp_add();
    test_p11_f04_fp_sub();
    test_p11_f05_fp_mul();
    test_p11_f06_fp_div();
    test_p11_f07_fp_abs();
    test_p11_f08_fp_sqrt();
    test_p11_f09_fp_exp_taylor();
    test_p11_f10_fp_ln_approx();
    test_p11_f11_fp_sin_cordic();
    test_p11_f12_fp_cos_cordic();
    test_p11_f13_fp_atan2_cordic();
    test_p11_f14_fp_lerp();
    test_p11_f15_fp_clamp();
    test_p11_f16_fp_poly_eval();
    printf("PASS: test_p09_reuse000 passed.\n");
    return 0;
}
