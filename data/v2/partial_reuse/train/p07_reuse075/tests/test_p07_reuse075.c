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

static void test_p07_f03_slab_free_block(void) {
    p07_slab_t s; memset(&s, 0, sizeof(s)); s.total_blocks = P07_NUM_BLOCKS; s.free_count = P07_NUM_BLOCKS; s.alloc_bitmap = 1; assert(p07_f03_slab_free_block(&s, (void*)s.memory_pool));
}

static void test_p07_f06_slab_mark_busy(void) {
    p07_slab_t s; memset(&s, 0, sizeof(s)); s.total_blocks = P07_NUM_BLOCKS; s.free_count = P07_NUM_BLOCKS; p07_f06_slab_mark_busy(&s, 0); assert((s.alloc_bitmap & 1) == 1);
}

static void test_p07_f07_slab_mark_free(void) {
    p07_slab_t s; memset(&s, 0, sizeof(s)); s.total_blocks = P07_NUM_BLOCKS; s.free_count = P07_NUM_BLOCKS; s.alloc_bitmap = 1; p07_f07_slab_mark_free(&s, 0); assert((s.alloc_bitmap & 1) == 0);
}

static void test_p07_f08_slab_find_first_free_bit(void) {
    assert(p07_f08_slab_find_first_free_bit(0) == 0);
}

static void test_p07_f09_slab_defragment_check(void) {
    p07_slab_t s; memset(&s, 0, sizeof(s)); s.total_blocks = P07_NUM_BLOCKS; s.free_count = P07_NUM_BLOCKS; s.alloc_bitmap = 0x55; assert(p07_f09_slab_defragment_check(&s) > 0);
}

static void test_p07_f10_slab_reset(void) {
    p07_slab_t s; memset(&s, 0, sizeof(s)); s.total_blocks = P07_NUM_BLOCKS; s.free_count = P07_NUM_BLOCKS; s.alloc_bitmap = 0xFF; p07_f10_slab_reset(&s); assert(s.alloc_bitmap == 0);
}

static void test_p07_f11_slab_block_index(void) {
    p07_slab_t s; memset(&s, 0, sizeof(s)); s.total_blocks = P07_NUM_BLOCKS; s.free_count = P07_NUM_BLOCKS; assert(p07_f11_slab_block_index(&s, s.memory_pool) == 0);
}

static void test_p07_f12_slab_index_to_pointer(void) {
    p07_slab_t s; memset(&s, 0, sizeof(s)); s.total_blocks = P07_NUM_BLOCKS; s.free_count = P07_NUM_BLOCKS; assert(p07_f12_slab_index_to_pointer(&s, 0) == (void*)s.memory_pool);
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

static void test_p04_f01_mat_init_zero(void) {
    p04_mat3_t m; p04_f01_mat_init_zero(&m); assert(m.m[0][0] == 0.0f);
}

static void test_p04_f09_mat_trace(void) {
    p04_mat3_t a = {.m = {{1,0,0},{0,1,0},{0,0,1}}}; assert(p04_f09_mat_trace(&a) == 3.0f);
}

static void test_p04_f12_mat_frobenius_norm(void) {
    p04_mat3_t a = {.m = {{1,0,0},{0,1,0},{0,0,1}}}; assert(fabsf(p04_f12_mat_frobenius_norm(&a) - sqrtf(3.0f)) < 1e-4f);
}

static void test_p04_f15_mat_solve_upper_tri(void) {
    p04_vec3_t sol; p04_vec3_t b = {{1, 2, 3}}; p04_mat3_t u = {{{2, 1, 1}, {0, 1, 2}, {0, 0, 1}}}; assert(p04_f15_mat_solve_upper_tri(&u, &b, &sol) == true && sol.v[2] == 3.0f);
}

int main(void) {
    test_p07_f01_slab_init();
    test_p07_f03_slab_free_block();
    test_p07_f06_slab_mark_busy();
    test_p07_f07_slab_mark_free();
    test_p07_f08_slab_find_first_free_bit();
    test_p07_f09_slab_defragment_check();
    test_p07_f10_slab_reset();
    test_p07_f11_slab_block_index();
    test_p07_f12_slab_index_to_pointer();
    test_p07_f14_slab_audit_integrity();
    test_p07_f15_slab_scrub_pattern();
    test_p07_f16_slab_compact_scan();
    test_p04_f01_mat_init_zero();
    test_p04_f09_mat_trace();
    test_p04_f12_mat_frobenius_norm();
    test_p04_f15_mat_solve_upper_tri();
    printf("PASS: test_p07_reuse075 passed.\n");
    return 0;
}
