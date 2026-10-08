#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "query_component.h"
#include "donor_component.h"

static void test_p12_f02_dag_add_edge(void) {
    p12_dag_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 3; assert(p12_f02_dag_add_edge(&g, 0, 1, 1) && g.adj[0][1]);
}

static void test_p12_f10_dag_count_descendants(void) {
    p12_dag_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 3; g.adj[0][1] = true; assert(p12_f10_dag_count_descendants(&g, 0) == 1);
}

static void test_p12_f12_dag_transitive_reduction(void) {
    p12_dag_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 3; g.adj[0][1] = true; g.adj[1][2] = true; g.adj[0][2] = true; p12_f12_dag_transitive_reduction(&g); assert(!g.adj[0][2]);
}

static void test_p12_f15_dag_is_reachable(void) {
    p12_dag_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 3; g.adj[0][1] = true; assert(p12_f15_dag_is_reachable(&g, 0, 1));
}

static void test_p14_f02_bm_set_bit(void) {
    p14_bitmap_t bm; memset(&bm, 0, sizeof(bm)); bm.num_bits = 64; assert(p14_f02_bm_set_bit(&bm, 5) && (bm.words[0] & (1ULL << 5)));
}

static void test_p14_f03_bm_clear_bit(void) {
    p14_bitmap_t bm; memset(&bm, 0, sizeof(bm)); bm.num_bits = 64; bm.words[0] = (1ULL << 5); assert(p14_f03_bm_clear_bit(&bm, 5) && bm.words[0] == 0);
}

static void test_p14_f04_bm_test_bit(void) {
    p14_bitmap_t bm; memset(&bm, 0, sizeof(bm)); bm.num_bits = 64; bm.words[0] = (1ULL << 5); assert(p14_f04_bm_test_bit(&bm, 5));
}

static void test_p14_f05_bm_toggle_bit(void) {
    p14_bitmap_t bm; memset(&bm, 0, sizeof(bm)); bm.num_bits = 64; assert(p14_f05_bm_toggle_bit(&bm, 5) && (bm.words[0] & (1ULL << 5)));
}

static void test_p14_f06_bm_popcount(void) {
    p14_bitmap_t bm; memset(&bm, 0, sizeof(bm)); bm.num_bits = 64; bm.words[0] = 7; assert(p14_f06_bm_popcount(&bm) == 3);
}

static void test_p14_f09_bm_find_next_set(void) {
    p14_bitmap_t bm; memset(&bm, 0, sizeof(bm)); bm.num_bits = 64; bm.words[0] = (1ULL << 5) | (1ULL << 10); assert(p14_f09_bm_find_next_set(&bm, 6) == 10);
}

static void test_p14_f10_bm_bitwise_and(void) {
    p14_bitmap_t bm; memset(&bm, 0, sizeof(bm)); bm.num_bits = 64; p14_bitmap_t b2; memset(&b2, 0, sizeof(b2)); b2.num_bits = 64; bm.words[0] = 3; b2.words[0] = 2; p14_bitmap_t out; p14_f10_bm_bitwise_and(&out, &bm, &b2); assert(out.words[0] == 2);
}

static void test_p14_f11_bm_bitwise_or(void) {
    p14_bitmap_t bm; memset(&bm, 0, sizeof(bm)); bm.num_bits = 64; p14_bitmap_t b2; memset(&b2, 0, sizeof(b2)); b2.num_bits = 64; bm.words[0] = 1; b2.words[0] = 2; p14_bitmap_t out; p14_f11_bm_bitwise_or(&out, &bm, &b2); assert(out.words[0] == 3);
}

static void test_p14_f12_bm_bitwise_xor(void) {
    p14_bitmap_t bm; memset(&bm, 0, sizeof(bm)); bm.num_bits = 64; p14_bitmap_t b2; memset(&b2, 0, sizeof(b2)); b2.num_bits = 64; bm.words[0] = 3; b2.words[0] = 2; p14_bitmap_t out; p14_f12_bm_bitwise_xor(&out, &bm, &b2); assert(out.words[0] == 1);
}

static void test_p14_f13_bm_bitwise_not(void) {
    p14_bitmap_t bm; memset(&bm, 0, sizeof(bm)); bm.num_bits = 64; p14_bitmap_t out; p14_f13_bm_bitwise_not(&out, &bm); assert(out.words[0] == ~0ULL);
}

static void test_p14_f14_bm_fill_range(void) {
    p14_bitmap_t bm; memset(&bm, 0, sizeof(bm)); bm.num_bits = 64; p14_f14_bm_fill_range(&bm, 0, 4); assert(bm.words[0] == 15);
}

static void test_p14_f16_bm_jaccard_similarity(void) {
    p14_bitmap_t bm; memset(&bm, 0, sizeof(bm)); bm.num_bits = 64; p14_bitmap_t b2 = bm; bm.words[0] = 1; b2.words[0] = 1; assert(p14_f16_bm_jaccard_similarity(&bm, &b2) == 1000);
}

int main(void) {
    test_p12_f02_dag_add_edge();
    test_p12_f10_dag_count_descendants();
    test_p12_f12_dag_transitive_reduction();
    test_p12_f15_dag_is_reachable();
    test_p14_f02_bm_set_bit();
    test_p14_f03_bm_clear_bit();
    test_p14_f04_bm_test_bit();
    test_p14_f05_bm_toggle_bit();
    test_p14_f06_bm_popcount();
    test_p14_f09_bm_find_next_set();
    test_p14_f10_bm_bitwise_and();
    test_p14_f11_bm_bitwise_or();
    test_p14_f12_bm_bitwise_xor();
    test_p14_f13_bm_bitwise_not();
    test_p14_f14_bm_fill_range();
    test_p14_f16_bm_jaccard_similarity();
    printf("PASS: test_p12_reuse025 passed.\n");
    return 0;
}
