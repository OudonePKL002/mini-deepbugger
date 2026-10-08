#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "query_component.h"
#include "donor_component.h"

static void test_p12_f03_dag_compute_indegrees(void) {
    p12_dag_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 3; g.adj[0][1] = true; size_t in_deg[3]; p12_f03_dag_compute_indegrees(&g, in_deg); assert(in_deg[1] == 1);
}

static void test_p12_f04_dag_compute_outdegrees(void) {
    p12_dag_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 3; g.adj[0][1] = true; size_t out_deg[3]; p12_f04_dag_compute_outdegrees(&g, out_deg); assert(out_deg[0] == 1);
}

static void test_p12_f05_dag_kahn_toposort(void) {
    p12_dag_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 3; g.adj[0][1] = true; g.adj[1][2] = true; size_t ord[3], len; assert(p12_f05_dag_kahn_toposort(&g, ord, &len) && len == 3);
}

static void test_p12_f07_dag_longest_path(void) {
    p12_dag_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 3; g.adj[0][1] = true; g.adj[1][2] = true; g.weights[0][1] = 1; g.weights[1][2] = 1; assert(p12_f07_dag_longest_path(&g, 0, 2) == 2);
}

static void test_p12_f08_dag_shortest_path(void) {
    p12_dag_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 3; g.adj[0][1] = true; g.adj[1][2] = true; g.weights[0][1] = 1; g.weights[1][2] = 1; assert(p12_f08_dag_shortest_path(&g, 0, 2) == 2);
}

static void test_p12_f10_dag_count_descendants(void) {
    p12_dag_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 3; g.adj[0][1] = true; assert(p12_f10_dag_count_descendants(&g, 0) == 1);
}

static void test_p12_f11_dag_assign_levels(void) {
    p12_dag_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 3; g.adj[0][1] = true; size_t lvls[3]; p12_f11_dag_assign_levels(&g, lvls); assert(lvls[1] == 1);
}

static void test_p12_f13_dag_find_sources(void) {
    p12_dag_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 3; g.adj[0][1] = true; size_t s[3]; assert(p12_f13_dag_find_sources(&g, s, 3) >= 1);
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

static void test_p14_f07_bm_find_first_set(void) {
    p14_bitmap_t bm; memset(&bm, 0, sizeof(bm)); bm.num_bits = 64; bm.words[0] = (1ULL << 5); assert(p14_f07_bm_find_first_set(&bm) == 5);
}

static void test_p14_f10_bm_bitwise_and(void) {
    p14_bitmap_t bm; memset(&bm, 0, sizeof(bm)); bm.num_bits = 64; p14_bitmap_t b2; memset(&b2, 0, sizeof(b2)); b2.num_bits = 64; bm.words[0] = 3; b2.words[0] = 2; p14_bitmap_t out; p14_f10_bm_bitwise_and(&out, &bm, &b2); assert(out.words[0] == 2);
}

static void test_p14_f11_bm_bitwise_or(void) {
    p14_bitmap_t bm; memset(&bm, 0, sizeof(bm)); bm.num_bits = 64; p14_bitmap_t b2; memset(&b2, 0, sizeof(b2)); b2.num_bits = 64; bm.words[0] = 1; b2.words[0] = 2; p14_bitmap_t out; p14_f11_bm_bitwise_or(&out, &bm, &b2); assert(out.words[0] == 3);
}

static void test_p14_f15_bm_clear_range(void) {
    p14_bitmap_t bm; memset(&bm, 0, sizeof(bm)); bm.num_bits = 64; bm.words[0] = 15; p14_f15_bm_clear_range(&bm, 0, 4); assert(bm.words[0] == 0);
}

int main(void) {
    test_p12_f03_dag_compute_indegrees();
    test_p12_f04_dag_compute_outdegrees();
    test_p12_f05_dag_kahn_toposort();
    test_p12_f07_dag_longest_path();
    test_p12_f08_dag_shortest_path();
    test_p12_f10_dag_count_descendants();
    test_p12_f11_dag_assign_levels();
    test_p12_f13_dag_find_sources();
    test_p14_f03_bm_clear_bit();
    test_p14_f04_bm_test_bit();
    test_p14_f05_bm_toggle_bit();
    test_p14_f06_bm_popcount();
    test_p14_f07_bm_find_first_set();
    test_p14_f10_bm_bitwise_and();
    test_p14_f11_bm_bitwise_or();
    test_p14_f15_bm_clear_range();
    printf("PASS: test_p12_reuse050 passed.\n");
    return 0;
}
