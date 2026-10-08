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

static void test_p11_f09_fp_exp_taylor(void) {
    p11_q16_t half = P11_FP_ONE / 2; assert(p11_f09_fp_exp_taylor(half) > P11_FP_ONE);
}

static void test_p11_f11_fp_sin_cordic(void) {
    assert(abs(p11_f11_fp_sin_cordic(0)) < 100);
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

static void test_p13_f02_slip_decode(void) {
    const uint8_t raw[] = {1, 0xDB, 0xDC, 0xC0}; uint8_t dec[16]; assert(p13_f02_slip_decode(raw, 4, dec, 16) == 2);
}

static void test_p13_f03_cobs_encode(void) {
    const uint8_t raw[] = {1, 2, 3}; uint8_t enc[16]; assert(p13_f03_cobs_encode(raw, 3, enc, 16) > 0);
}

static void test_p13_f13_byte_swap32(void) {
    assert(p13_f13_byte_swap32(0x12345678) == 0x78563412);
}

static void test_p13_f14_parse_tlv_field(void) {
    const uint8_t b[4] = {1, 2, 0xAA, 0xBB}; uint8_t tag; uint8_t v_len; const uint8_t *val; assert(p13_f14_parse_tlv_field(b, 4, &tag, &v_len, &val) && tag == 1 && v_len == 2);
}

int main(void) {
    test_p11_f01_fp_from_int();
    test_p11_f03_fp_add();
    test_p11_f04_fp_sub();
    test_p11_f05_fp_mul();
    test_p11_f06_fp_div();
    test_p11_f07_fp_abs();
    test_p11_f09_fp_exp_taylor();
    test_p11_f11_fp_sin_cordic();
    test_p11_f13_fp_atan2_cordic();
    test_p11_f14_fp_lerp();
    test_p11_f15_fp_clamp();
    test_p11_f16_fp_poly_eval();
    test_p13_f02_slip_decode();
    test_p13_f03_cobs_encode();
    test_p13_f13_byte_swap32();
    test_p13_f14_parse_tlv_field();
    printf("PASS: test_p11_reuse075 passed.\n");
    return 0;
}
