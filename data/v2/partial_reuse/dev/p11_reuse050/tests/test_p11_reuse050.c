#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "query_component.h"
#include "donor_component.h"

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

static void test_p11_f14_fp_lerp(void) {
    p11_q16_t a = 65536, b = 3 * 65536, t = 32768; assert(p11_f14_fp_lerp(a, b, t) == 2 * 65536);
}

static void test_p13_f01_slip_encode(void) {
    const uint8_t raw[] = {1, 0xC0}; uint8_t enc[16]; assert(p13_f01_slip_encode(raw, 2, enc, 16) > 2);
}

static void test_p13_f02_slip_decode(void) {
    const uint8_t raw[] = {1, 0xDB, 0xDC, 0xC0}; uint8_t dec[16]; assert(p13_f02_slip_decode(raw, 4, dec, 16) == 2);
}

static void test_p13_f03_cobs_encode(void) {
    const uint8_t raw[] = {1, 2, 3}; uint8_t enc[16]; assert(p13_f03_cobs_encode(raw, 3, enc, 16) > 0);
}

static void test_p13_f05_fcs16_compute(void) {
    const uint8_t raw[] = {1, 2, 3, 4}; assert(p13_f05_fcs16_compute(raw, 4) != 0);
}

static void test_p13_f10_bit_stuff_decode(void) {
    const uint8_t enc[1] = {0x00}; uint8_t dec[1]; assert(p13_f10_bit_stuff_decode(enc, 8, dec) == 8);
}

static void test_p13_f12_byte_swap16(void) {
    assert(p13_f12_byte_swap16(0x1234) == 0x3412);
}

static void test_p13_f14_parse_tlv_field(void) {
    const uint8_t b[4] = {1, 2, 0xAA, 0xBB}; uint8_t tag; uint8_t v_len; const uint8_t *val; assert(p13_f14_parse_tlv_field(b, 4, &tag, &v_len, &val) && tag == 1 && v_len == 2);
}

static void test_p13_f15_frame_buf_append(void) {
    p13_frame_buf_t fb; memset(&fb, 0, sizeof(fb)); fb.capacity = 256; const uint8_t d[2] = {1, 2}; assert(p13_f15_frame_buf_append(&fb, d, 2) && fb.length == 2);
}

int main(void) {
    test_p11_f02_fp_to_int();
    test_p11_f03_fp_add();
    test_p11_f04_fp_sub();
    test_p11_f05_fp_mul();
    test_p11_f06_fp_div();
    test_p11_f07_fp_abs();
    test_p11_f08_fp_sqrt();
    test_p11_f14_fp_lerp();
    test_p13_f01_slip_encode();
    test_p13_f02_slip_decode();
    test_p13_f03_cobs_encode();
    test_p13_f05_fcs16_compute();
    test_p13_f10_bit_stuff_decode();
    test_p13_f12_byte_swap16();
    test_p13_f14_parse_tlv_field();
    test_p13_f15_frame_buf_append();
    printf("PASS: test_p11_reuse050 passed.\n");
    return 0;
}
