#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "query_component.h"
#include "donor_component.h"

static void test_p12_f01_dag_init(void) {
    p12_dag_t g; p12_f01_dag_init(&g, 5); assert(g.num_vertices == 5);
}

static void test_p12_f02_dag_add_edge(void) {
    p12_dag_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 3; assert(p12_f02_dag_add_edge(&g, 0, 1, 1) && g.adj[0][1]);
}

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

static void test_p12_f09_dag_count_ancestors(void) {
    p12_dag_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 3; g.adj[0][1] = true; assert(p12_f09_dag_count_ancestors(&g, 1) == 1);
}

static void test_p12_f10_dag_count_descendants(void) {
    p12_dag_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 3; g.adj[0][1] = true; assert(p12_f10_dag_count_descendants(&g, 0) == 1);
}

static void test_p12_f12_dag_transitive_reduction(void) {
    p12_dag_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 3; g.adj[0][1] = true; g.adj[1][2] = true; g.adj[0][2] = true; p12_f12_dag_transitive_reduction(&g); assert(!g.adj[0][2]);
}

static void test_p12_f13_dag_find_sources(void) {
    p12_dag_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 3; g.adj[0][1] = true; size_t s[3]; assert(p12_f13_dag_find_sources(&g, s, 3) >= 1);
}

static void test_p12_f14_dag_find_sinks(void) {
    p12_dag_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 3; g.adj[0][1] = true; size_t s[3]; assert(p12_f14_dag_find_sinks(&g, s, 3) >= 1);
}

static void test_p14_f01_bm_init(void) {
    p14_bitmap_t bm; bm.num_bits = 0; p14_f01_bm_init(&bm, 64); assert(bm.num_bits == 64);
}

static void test_p14_f05_bm_toggle_bit(void) {
    p14_bitmap_t bm; memset(&bm, 0, sizeof(bm)); bm.num_bits = 64; assert(p14_f05_bm_toggle_bit(&bm, 5) && (bm.words[0] & (1ULL << 5)));
}

static void test_p14_f06_bm_popcount(void) {
    p14_bitmap_t bm; memset(&bm, 0, sizeof(bm)); bm.num_bits = 64; bm.words[0] = 7; assert(p14_f06_bm_popcount(&bm) == 3);
}

static void test_p14_f12_bm_bitwise_xor(void) {
    p14_bitmap_t bm; memset(&bm, 0, sizeof(bm)); bm.num_bits = 64; p14_bitmap_t b2; memset(&b2, 0, sizeof(b2)); b2.num_bits = 64; bm.words[0] = 3; b2.words[0] = 2; p14_bitmap_t out; p14_f12_bm_bitwise_xor(&out, &bm, &b2); assert(out.words[0] == 1);
}

int main(void) {
    test_p12_f01_dag_init();
    test_p12_f02_dag_add_edge();
    test_p12_f03_dag_compute_indegrees();
    test_p12_f04_dag_compute_outdegrees();
    test_p12_f05_dag_kahn_toposort();
    test_p12_f07_dag_longest_path();
    test_p12_f08_dag_shortest_path();
    test_p12_f09_dag_count_ancestors();
    test_p12_f10_dag_count_descendants();
    test_p12_f12_dag_transitive_reduction();
    test_p12_f13_dag_find_sources();
    test_p12_f14_dag_find_sinks();
    test_p14_f01_bm_init();
    test_p14_f05_bm_toggle_bit();
    test_p14_f06_bm_popcount();
    test_p14_f12_bm_bitwise_xor();
    printf("PASS: test_p12_reuse075 passed.\n");
    return 0;
}
