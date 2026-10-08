#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "query_component.h"
#include "donor_component.h"

static void test_p10_f01_nfa_init(void) {
    p10_nfa_t nfa; nfa.num_states = 5; p10_f01_nfa_init(&nfa); assert(nfa.num_states == 0);
}

static void test_p10_f04_nfa_set_match(void) {
    p10_nfa_t nfa; memset(&nfa, 0, sizeof(nfa)); nfa.num_states = 1; assert(p10_f04_nfa_set_match(&nfa, 0, true) && nfa.states[0].is_match);
}

static void test_p10_f06_nfa_concat(void) {
    p10_nfa_t nfa; memset(&nfa, 0, sizeof(nfa)); nfa.num_states = 2; assert(p10_f06_nfa_concat(&nfa, 0, 1) >= 0);
}

static void test_p10_f11_nfa_epsilon_closure(void) {
    p10_nfa_t nfa; memset(&nfa, 0, sizeof(nfa)); nfa.num_states = 2; bool in_s[P10_MAX_STATES] = {true}, out_s[P10_MAX_STATES]; p10_f11_nfa_epsilon_closure(&nfa, in_s, out_s); assert(out_s[0]);
}

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

static void test_p12_f06_dag_has_cycle(void) {
    p12_dag_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 3; g.adj[0][1] = true; assert(!p12_f06_dag_has_cycle(&g));
}

static void test_p12_f07_dag_longest_path(void) {
    p12_dag_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 3; g.adj[0][1] = true; g.adj[1][2] = true; g.weights[0][1] = 1; g.weights[1][2] = 1; assert(p12_f07_dag_longest_path(&g, 0, 2) == 2);
}

static void test_p12_f09_dag_count_ancestors(void) {
    p12_dag_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 3; g.adj[0][1] = true; assert(p12_f09_dag_count_ancestors(&g, 1) == 1);
}

static void test_p12_f13_dag_find_sources(void) {
    p12_dag_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 3; g.adj[0][1] = true; size_t s[3]; assert(p12_f13_dag_find_sources(&g, s, 3) >= 1);
}

static void test_p12_f14_dag_find_sinks(void) {
    p12_dag_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 3; g.adj[0][1] = true; size_t s[3]; assert(p12_f14_dag_find_sinks(&g, s, 3) >= 1);
}

static void test_p12_f15_dag_is_reachable(void) {
    p12_dag_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 3; g.adj[0][1] = true; assert(p12_f15_dag_is_reachable(&g, 0, 1));
}

static void test_p12_f16_dag_clear(void) {
    p12_dag_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 3; g.adj[0][1] = true; p12_f16_dag_clear(&g); assert(!g.adj[0][1]);
}

int main(void) {
    test_p10_f01_nfa_init();
    test_p10_f04_nfa_set_match();
    test_p10_f06_nfa_concat();
    test_p10_f11_nfa_epsilon_closure();
    test_p12_f01_dag_init();
    test_p12_f02_dag_add_edge();
    test_p12_f03_dag_compute_indegrees();
    test_p12_f04_dag_compute_outdegrees();
    test_p12_f05_dag_kahn_toposort();
    test_p12_f06_dag_has_cycle();
    test_p12_f07_dag_longest_path();
    test_p12_f09_dag_count_ancestors();
    test_p12_f13_dag_find_sources();
    test_p12_f14_dag_find_sinks();
    test_p12_f15_dag_is_reachable();
    test_p12_f16_dag_clear();
    printf("PASS: test_p10_reuse025 passed.\n");
    return 0;
}
