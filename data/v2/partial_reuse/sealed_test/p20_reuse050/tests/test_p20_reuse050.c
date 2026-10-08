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

static void test_p20_f04_ring_available(void) {
    p20_ring_t r; memset(&r, 0, sizeof(r)); assert(p20_f04_ring_available(&r) == 0);
}

static void test_p20_f06_ring_write_byte(void) {
    p20_ring_t r; memset(&r, 0, sizeof(r)); assert(p20_f06_ring_write_byte(&r, 'A') && r.count == 1);
}

static void test_p20_f07_ring_read_byte(void) {
    p20_ring_t r; memset(&r, 0, sizeof(r)); r.buffer[0] = 'A'; r.head = 1; r.tail = 0; r.count = 1; uint8_t b; assert(p20_f07_ring_read_byte(&r, &b) && b == 'A');
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

static void test_p20_f16_ring_clear(void) {
    p20_ring_t r; memset(&r, 0, sizeof(r)); r.buffer[0] = 'A'; r.head = 1; r.tail = 0; r.count = 1; p20_f16_ring_clear(&r); assert(r.count == 0);
}

static void test_p19_f03_dijkstra(void) {
    p19_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 2; for(size_t i=0;i<P19_MAX_VERTICES;i++) for(size_t j=0;j<P19_MAX_VERTICES;j++) g.weights[i][j]=(i==j)?0:P19_INF; g.weights[0][1] = 5; int32_t d[2], p[2]; p19_f03_dijkstra(&g, 0, d, p); assert(d[1] == 5);
}

static void test_p19_f04_bellman_ford(void) {
    p19_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 2; for(size_t i=0;i<P19_MAX_VERTICES;i++) for(size_t j=0;j<P19_MAX_VERTICES;j++) g.weights[i][j]=(i==j)?0:P19_INF; g.weights[0][1] = 5; int32_t d[2]; assert(p19_f04_bellman_ford(&g, 0, d) && d[1] == 5);
}

static void test_p19_f05_floyd_warshall(void) {
    p19_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 2; for(size_t i=0;i<P19_MAX_VERTICES;i++) for(size_t j=0;j<P19_MAX_VERTICES;j++) g.weights[i][j]=(i==j)?0:P19_INF; g.weights[0][1] = 5; int32_t fw[P19_MAX_VERTICES][P19_MAX_VERTICES]; p19_f05_floyd_warshall(&g, fw); assert(fw[0][1] == 5);
}

static void test_p19_f07_has_negative_cycle(void) {
    p19_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 2; for(size_t i=0;i<P19_MAX_VERTICES;i++) for(size_t j=0;j<P19_MAX_VERTICES;j++) g.weights[i][j]=(i==j)?0:P19_INF; g.weights[0][1] = 5; assert(!p19_f07_has_negative_cycle(&g));
}

static void test_p19_f09_prim_mst(void) {
    p19_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 2; for(size_t i=0;i<P19_MAX_VERTICES;i++) for(size_t j=0;j<P19_MAX_VERTICES;j++) g.weights[i][j]=(i==j)?0:P19_INF; g.weights[0][1] = 5; g.weights[1][0] = 5; assert(p19_f09_prim_mst(&g, 0) == 5);
}

static void test_p19_f11_eccentricity(void) {
    p19_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 2; for(size_t i=0;i<P19_MAX_VERTICES;i++) for(size_t j=0;j<P19_MAX_VERTICES;j++) g.weights[i][j]=(i==j)?0:P19_INF; g.weights[0][1] = 5; assert(p19_f11_eccentricity(&g, 0) == 5);
}

static void test_p19_f12_graph_diameter(void) {
    p19_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 2; for(size_t i=0;i<P19_MAX_VERTICES;i++) for(size_t j=0;j<P19_MAX_VERTICES;j++) g.weights[i][j]=(i==j)?0:P19_INF; g.weights[0][1] = 5; assert(p19_f12_graph_diameter(&g) >= 5);
}

static void test_p19_f13_graph_radius(void) {
    p19_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 2; for(size_t i=0;i<P19_MAX_VERTICES;i++) for(size_t j=0;j<P19_MAX_VERTICES;j++) g.weights[i][j]=(i==j)?0:P19_INF; g.weights[0][1] = 5; assert(p19_f13_graph_radius(&g) >= 0);
}

int main(void) {
    test_p20_f01_ring_init();
    test_p20_f04_ring_available();
    test_p20_f06_ring_write_byte();
    test_p20_f07_ring_read_byte();
    test_p20_f11_ring_discard();
    test_p20_f12_ring_find_marker();
    test_p20_f13_ring_linearize();
    test_p20_f16_ring_clear();
    test_p19_f03_dijkstra();
    test_p19_f04_bellman_ford();
    test_p19_f05_floyd_warshall();
    test_p19_f07_has_negative_cycle();
    test_p19_f09_prim_mst();
    test_p19_f11_eccentricity();
    test_p19_f12_graph_diameter();
    test_p19_f13_graph_radius();
    printf("PASS: test_p20_reuse050 passed.\n");
    return 0;
}
