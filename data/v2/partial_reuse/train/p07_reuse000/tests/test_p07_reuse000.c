#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "query_component.h"
#include "donor_component.h"

static void test_p04_f01_mat_init_zero(void) {
    p04_mat3_t m; p04_f01_mat_init_zero(&m); assert(m.m[0][0] == 0.0f);
}

static void test_p04_f02_mat_init_identity(void) {
    p04_mat3_t m; p04_f02_mat_init_identity(&m); assert(m.m[0][0] == 1.0f);
}

static void test_p04_f03_mat_add(void) {
    p04_mat3_t a = {.m = {{1,0,0},{0,1,0},{0,0,1}}}; p04_mat3_t b = a, out; p04_f03_mat_add(&a, &b, &out); assert(out.m[0][0] == 2.0f);
}

static void test_p04_f04_mat_sub(void) {
    p04_mat3_t a = {.m = {{1,0,0},{0,1,0},{0,0,1}}}; p04_mat3_t b = a, out; p04_f04_mat_sub(&a, &b, &out); assert(out.m[0][0] == 0.0f);
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

static void test_p04_f08_mat_vec_mul(void) {
    p04_mat3_t a = {.m = {{1,0,0},{0,1,0},{0,0,1}}}; p04_vec3_t v = {{1.0f, 2.0f, 3.0f}}, out; p04_f08_mat_vec_mul(&a, &v, &out); assert(out.v[1] == 2.0f);
}

static void test_p04_f09_mat_trace(void) {
    p04_mat3_t a = {.m = {{1,0,0},{0,1,0},{0,0,1}}}; assert(p04_f09_mat_trace(&a) == 3.0f);
}

static void test_p04_f10_mat_det_2x2(void) {
    assert(p04_f10_mat_det_2x2(1, 2, 3, 4) == -2.0f);
}

static void test_p04_f11_mat_det_3x3(void) {
    p04_mat3_t a = {.m = {{1,0,0},{0,1,0},{0,0,1}}}; assert(p04_f11_mat_det_3x3(&a) == 1.0f);
}

static void test_p04_f12_mat_frobenius_norm(void) {
    p04_mat3_t a = {.m = {{1,0,0},{0,1,0},{0,0,1}}}; assert(fabsf(p04_f12_mat_frobenius_norm(&a) - sqrtf(3.0f)) < 1e-4f);
}

static void test_p04_f13_mat_is_symmetric(void) {
    p04_mat3_t a = {.m = {{1,0,0},{0,1,0},{0,0,1}}}; assert(p04_f13_mat_is_symmetric(&a, 1e-5f) == true);
}

static void test_p04_f14_mat_lu_decompose_3x3(void) {
    p04_mat3_t l, u; p04_mat3_t t = {{{2, -1, -2}, {-4, 6, 3}, {-4, -2, 8}}}; assert(p04_f14_mat_lu_decompose_3x3(&t, &l, &u) == true);
}

static void test_p04_f15_mat_solve_upper_tri(void) {
    p04_vec3_t sol; p04_vec3_t b = {{1, 2, 3}}; p04_mat3_t u = {{{2, 1, 1}, {0, 1, 2}, {0, 0, 1}}}; assert(p04_f15_mat_solve_upper_tri(&u, &b, &sol) == true && sol.v[2] == 3.0f);
}

static void test_p04_f16_mat_hadamard_product(void) {
    p04_mat3_t a = {.m = {{1,0,0},{0,1,0},{0,0,1}}}; p04_mat3_t b = a, out; p04_f16_mat_hadamard_product(&a, &b, &out); assert(out.m[0][0] == 1.0f);
}

int main(void) {
    test_p04_f01_mat_init_zero();
    test_p04_f02_mat_init_identity();
    test_p04_f03_mat_add();
    test_p04_f04_mat_sub();
    test_p04_f05_mat_scale();
    test_p04_f06_mat_transpose();
    test_p04_f07_mat_mul();
    test_p04_f08_mat_vec_mul();
    test_p04_f09_mat_trace();
    test_p04_f10_mat_det_2x2();
    test_p04_f11_mat_det_3x3();
    test_p04_f12_mat_frobenius_norm();
    test_p04_f13_mat_is_symmetric();
    test_p04_f14_mat_lu_decompose_3x3();
    test_p04_f15_mat_solve_upper_tri();
    test_p04_f16_mat_hadamard_product();
    printf("PASS: test_p07_reuse000 passed.\n");
    return 0;
}
