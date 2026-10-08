#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "query_component.h"
#include "donor_component.h"

static void test_p20_f01_ring_init(void) {
    p20_ring_t r; r.count = 5; p20_f01_ring_init(&r); assert(r.count == 0 && r.head == 0 && r.tail == 0);
}

static void test_p20_f02_ring_is_empty(void) {
    p20_ring_t r; memset(&r, 0, sizeof(r)); assert(p20_f02_ring_is_empty(&r));
}

static void test_p20_f03_ring_is_full(void) {
    p20_ring_t r; memset(&r, 0, sizeof(r)); assert(!p20_f03_ring_is_full(&r));
}

static void test_p20_f04_ring_available(void) {
    p20_ring_t r; memset(&r, 0, sizeof(r)); assert(p20_f04_ring_available(&r) == 0);
}

static void test_p20_f08_ring_write_slice(void) {
    p20_ring_t r; memset(&r, 0, sizeof(r)); uint8_t s[2] = {1, 2}; assert(p20_f08_ring_write_slice(&r, s, 2) == 2);
}

static void test_p20_f09_ring_read_slice(void) {
    p20_ring_t r; memset(&r, 0, sizeof(r)); r.buffer[0] = 'A'; r.head = 1; r.tail = 0; r.count = 1; uint8_t d[2]; assert(p20_f09_ring_read_slice(&r, d, 1) == 1 && d[0] == 'A');
}

static void test_p20_f10_ring_peek_byte(void) {
    p20_ring_t r; memset(&r, 0, sizeof(r)); r.buffer[0] = 'A'; r.head = 1; r.tail = 0; r.count = 1; uint8_t b; assert(p20_f10_ring_peek_byte(&r, 0, &b) && b == 'A');
}

static void test_p20_f11_ring_discard(void) {
    p20_ring_t r; memset(&r, 0, sizeof(r)); r.buffer[0] = 'A'; r.head = 1; r.tail = 0; r.count = 1; assert(p20_f11_ring_discard(&r, 1) == 1 && r.count == 0);
}

static void test_p20_f12_ring_find_marker(void) {
    p20_ring_t r; memset(&r, 0, sizeof(r)); r.buffer[0] = 'b'; r.buffer[1] = 'c'; r.head = 2; r.tail = 0; r.count = 2; assert(p20_f12_ring_find_marker(&r, (const uint8_t*)"bc", 2) == 0);
}

static void test_p20_f13_ring_linearize(void) {
    p20_ring_t r; memset(&r, 0, sizeof(r)); r.buffer[0] = 'A'; r.head = 1; r.tail = 0; r.count = 1; uint8_t d[2]; assert(p20_f13_ring_linearize(&r, d, 1) == 1 && d[0] == 'A');
}

static void test_p20_f15_ring_checksum(void) {
    p20_ring_t r; memset(&r, 0, sizeof(r)); r.buffer[0] = 'A'; r.head = 1; r.tail = 0; r.count = 1; assert(p20_f15_ring_checksum(&r) == 'A');
}

static void test_p20_f16_ring_clear(void) {
    p20_ring_t r; memset(&r, 0, sizeof(r)); r.buffer[0] = 'A'; r.head = 1; r.tail = 0; r.count = 1; p20_f16_ring_clear(&r); assert(r.count == 0);
}

static void test_p19_f03_dijkstra(void) {
    p19_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 2; for(size_t i=0;i<P19_MAX_VERTICES;i++) for(size_t j=0;j<P19_MAX_VERTICES;j++) g.weights[i][j]=(i==j)?0:P19_INF; g.weights[0][1] = 5; int32_t d[2], p[2]; p19_f03_dijkstra(&g, 0, d, p); assert(d[1] == 5);
}

static void test_p19_f04_bellman_ford(void) {
    p19_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 2; for(size_t i=0;i<P19_MAX_VERTICES;i++) for(size_t j=0;j<P19_MAX_VERTICES;j++) g.weights[i][j]=(i==j)?0:P19_INF; g.weights[0][1] = 5; int32_t d[2]; assert(p19_f04_bellman_ford(&g, 0, d) && d[1] == 5);
}

static void test_p19_f07_has_negative_cycle(void) {
    p19_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 2; for(size_t i=0;i<P19_MAX_VERTICES;i++) for(size_t j=0;j<P19_MAX_VERTICES;j++) g.weights[i][j]=(i==j)?0:P19_INF; g.weights[0][1] = 5; assert(!p19_f07_has_negative_cycle(&g));
}

static void test_p19_f15_is_bipartite(void) {
    p19_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 2; for(size_t i=0;i<P19_MAX_VERTICES;i++) for(size_t j=0;j<P19_MAX_VERTICES;j++) g.weights[i][j]=(i==j)?0:P19_INF; g.weights[0][1] = 5; assert(!p19_f15_is_bipartite(&g));
}

int main(void) {
    test_p20_f01_ring_init();
    test_p20_f02_ring_is_empty();
    test_p20_f03_ring_is_full();
    test_p20_f04_ring_available();
    test_p20_f08_ring_write_slice();
    test_p20_f09_ring_read_slice();
    test_p20_f10_ring_peek_byte();
    test_p20_f11_ring_discard();
    test_p20_f12_ring_find_marker();
    test_p20_f13_ring_linearize();
    test_p20_f15_ring_checksum();
    test_p20_f16_ring_clear();
    test_p19_f03_dijkstra();
    test_p19_f04_bellman_ford();
    test_p19_f07_has_negative_cycle();
    test_p19_f15_is_bipartite();
    printf("PASS: test_p20_reuse075 passed.\n");
    return 0;
}
