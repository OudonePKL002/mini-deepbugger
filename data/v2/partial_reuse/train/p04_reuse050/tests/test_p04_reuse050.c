#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "query_component.h"
#include "donor_component.h"

static void test_p04_f03_mat_add(void) {
    p04_mat3_t a = {.m = {{1,0,0},{0,1,0},{0,0,1}}}; p04_mat3_t b = a, out; p04_f03_mat_add(&a, &b, &out); assert(out.m[0][0] == 2.0f);
}

static void test_p04_f05_mat_scale(void) {
    p04_mat3_t a = {.m = {{1,0,0},{0,1,0},{0,0,1}}}; p04_mat3_t out; p04_f05_mat_scale(&a, 3.0f, &out); assert(out.m[0][0] == 3.0f);
}

static void test_p04_f06_mat_transpose(void) {
    p04_mat3_t a = {.m = {{0,4,0},{0,0,0},{0,0,0}}}, out; p04_f06_mat_transpose(&a, &out); assert(out.m[1][0] == 4.0f);
}

static void test_p04_f07_mat_mul(void) {
    p04_mat3_t a = {.m = {{1,0,0},{0,1,0},{0,0,1}}}; p04_mat3_t b = a, out; p04_f07_mat_mul(&a, &b, &out); assert(out.m[0][0] == 1.0f);
}

static void test_p04_f10_mat_det_2x2(void) {
    assert(p04_f10_mat_det_2x2(1, 2, 3, 4) == -2.0f);
}

static void test_p04_f11_mat_det_3x3(void) {
    p04_mat3_t a = {.m = {{1,0,0},{0,1,0},{0,0,1}}}; assert(p04_f11_mat_det_3x3(&a) == 1.0f);
}

static void test_p04_f15_mat_solve_upper_tri(void) {
    p04_vec3_t sol; p04_vec3_t b = {{1, 2, 3}}; p04_mat3_t u = {{{2, 1, 1}, {0, 1, 2}, {0, 0, 1}}}; assert(p04_f15_mat_solve_upper_tri(&u, &b, &sol) == true && sol.v[2] == 3.0f);
}

static void test_p04_f16_mat_hadamard_product(void) {
    p04_mat3_t a = {.m = {{1,0,0},{0,1,0},{0,0,1}}}; p04_mat3_t b = a, out; p04_f16_mat_hadamard_product(&a, &b, &out); assert(out.m[0][0] == 1.0f);
}

static void test_p01_f02_crc8_cdma(void) {
    
    const uint8_t s[] = "MiniDeepBugger2026";
    assert(p01_f02_crc8_cdma(s, sizeof(s) - 1) != 0);

}

static void test_p01_f05_crc32_ieee(void) {
    
    const uint8_t s[] = "MiniDeepBugger2026";
    assert(p01_f05_crc32_ieee(s, sizeof(s) - 1) != 0);

}

static void test_p01_f07_fletcher16(void) {
    
    const uint8_t s[] = "MiniDeepBugger2026";
    assert(p01_f07_fletcher16(s, sizeof(s) - 1) != 0);

}

static void test_p01_f08_fletcher32(void) {
    
    uint16_t wbuf[8] = { 0x1234, 0x5678, 0x9ABC, 0xDEF0, 0x1357, 0x2468, 0x3579, 0x4680 };
    assert(p01_f08_fletcher32(wbuf, 8) != 0);

}

static void test_p01_f09_bsd_checksum(void) {
    
    const uint8_t s[] = "MiniDeepBugger2026";
    assert(p01_f09_bsd_checksum(s, sizeof(s) - 1) != 0);

}

static void test_p01_f10_sysv_checksum(void) {
    
    const uint8_t s[] = "MiniDeepBugger2026";
    assert(p01_f10_sysv_checksum(s, sizeof(s) - 1) != 0);

}

static void test_p01_f13_internet_checksum(void) {
    
    const uint8_t s[] = "MiniDeepBugger2026";
    assert(p01_f13_internet_checksum(s, sizeof(s) - 1) != 0);

}

static void test_p01_f14_adler16_simple(void) {
    
    const uint8_t s[] = "MiniDeepBugger2026";
    assert(p01_f14_adler16_simple(s, sizeof(s) - 1) != 0);

}

int main(void) {
    test_p04_f03_mat_add();
    test_p04_f05_mat_scale();
    test_p04_f06_mat_transpose();
    test_p04_f07_mat_mul();
    test_p04_f10_mat_det_2x2();
    test_p04_f11_mat_det_3x3();
    test_p04_f15_mat_solve_upper_tri();
    test_p04_f16_mat_hadamard_product();
    test_p01_f02_crc8_cdma();
    test_p01_f05_crc32_ieee();
    test_p01_f07_fletcher16();
    test_p01_f08_fletcher32();
    test_p01_f09_bsd_checksum();
    test_p01_f10_sysv_checksum();
    test_p01_f13_internet_checksum();
    test_p01_f14_adler16_simple();
    printf("PASS: test_p04_reuse050 passed.\n");
    return 0;
}
