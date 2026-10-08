#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "query_component.h"
#include "donor_component.h"

static void test_p07_f01_slab_init(void) {
    p07_slab_t s; p07_f01_slab_init(&s); assert(s.alloc_bitmap == 0 && s.free_count == 32);
}

static void test_p07_f09_slab_defragment_check(void) {
    p07_slab_t s; memset(&s, 0, sizeof(s)); s.total_blocks = P07_NUM_BLOCKS; s.free_count = P07_NUM_BLOCKS; s.alloc_bitmap = 0x55; assert(p07_f09_slab_defragment_check(&s) > 0);
}

static void test_p07_f11_slab_block_index(void) {
    p07_slab_t s; memset(&s, 0, sizeof(s)); s.total_blocks = P07_NUM_BLOCKS; s.free_count = P07_NUM_BLOCKS; assert(p07_f11_slab_block_index(&s, s.memory_pool) == 0);
}

static void test_p07_f12_slab_index_to_pointer(void) {
    p07_slab_t s; memset(&s, 0, sizeof(s)); s.total_blocks = P07_NUM_BLOCKS; s.free_count = P07_NUM_BLOCKS; assert(p07_f12_slab_index_to_pointer(&s, 0) == (void*)s.memory_pool);
}

static void test_p07_f13_slab_stats_utilization(void) {
    p07_slab_t s; memset(&s, 0, sizeof(s)); s.total_blocks = P07_NUM_BLOCKS; s.free_count = P07_NUM_BLOCKS; assert(p07_f13_slab_stats_utilization(&s) == 0);
}

static void test_p07_f14_slab_audit_integrity(void) {
    p07_slab_t s; memset(&s, 0, sizeof(s)); s.total_blocks = P07_NUM_BLOCKS; s.free_count = P07_NUM_BLOCKS; assert(p07_f14_slab_audit_integrity(&s) == true);
}

static void test_p07_f15_slab_scrub_pattern(void) {
    p07_slab_t s; memset(&s, 0, sizeof(s)); s.total_blocks = P07_NUM_BLOCKS; s.free_count = P07_NUM_BLOCKS; p07_f15_slab_scrub_pattern(&s, 0, 0xAA); assert(s.memory_pool[0] == 0xAA);
}

static void test_p07_f16_slab_compact_scan(void) {
    p07_slab_t s; memset(&s, 0, sizeof(s)); s.total_blocks = P07_NUM_BLOCKS; s.free_count = P07_NUM_BLOCKS; size_t run; assert(p07_f16_slab_compact_scan(&s, &run) == 1 && run == 32);
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

static void test_p04_f16_mat_hadamard_product(void) {
    p04_mat3_t a = {.m = {{1,0,0},{0,1,0},{0,0,1}}}; p04_mat3_t b = a, out; p04_f16_mat_hadamard_product(&a, &b, &out); assert(out.m[0][0] == 1.0f);
}

int main(void) {
    test_p07_f01_slab_init();
    test_p07_f09_slab_defragment_check();
    test_p07_f11_slab_block_index();
    test_p07_f12_slab_index_to_pointer();
    test_p07_f13_slab_stats_utilization();
    test_p07_f14_slab_audit_integrity();
    test_p07_f15_slab_scrub_pattern();
    test_p07_f16_slab_compact_scan();
    test_p04_f04_mat_sub();
    test_p04_f05_mat_scale();
    test_p04_f06_mat_transpose();
    test_p04_f07_mat_mul();
    test_p04_f08_mat_vec_mul();
    test_p04_f09_mat_trace();
    test_p04_f10_mat_det_2x2();
    test_p04_f16_mat_hadamard_product();
    printf("PASS: test_p07_reuse050 passed.\n");
    return 0;
}
