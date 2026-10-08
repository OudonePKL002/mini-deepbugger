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

static void test_p10_f02_nfa_add_state(void) {
    p10_nfa_t nfa; memset(&nfa, 0, sizeof(nfa)); assert(p10_f02_nfa_add_state(&nfa, 'a', -1, -1) == 0);
}

static void test_p10_f03_nfa_set_transition(void) {
    p10_nfa_t nfa; memset(&nfa, 0, sizeof(nfa)); nfa.num_states = 2; assert(p10_f03_nfa_set_transition(&nfa, 0, 1, -1));
}

static void test_p10_f04_nfa_set_match(void) {
    p10_nfa_t nfa; memset(&nfa, 0, sizeof(nfa)); nfa.num_states = 1; assert(p10_f04_nfa_set_match(&nfa, 0, true) && nfa.states[0].is_match);
}

static void test_p10_f05_nfa_create_literal(void) {
    p10_nfa_t nfa; memset(&nfa, 0, sizeof(nfa)); assert(p10_f05_nfa_create_literal(&nfa, 'a') >= 0);
}

static void test_p10_f06_nfa_concat(void) {
    p10_nfa_t nfa; memset(&nfa, 0, sizeof(nfa)); nfa.num_states = 2; assert(p10_f06_nfa_concat(&nfa, 0, 1) >= 0);
}

static void test_p10_f08_nfa_star(void) {
    p10_nfa_t nfa; memset(&nfa, 0, sizeof(nfa)); nfa.num_states = 1; assert(p10_f08_nfa_star(&nfa, 0) >= 0);
}

static void test_p10_f11_nfa_epsilon_closure(void) {
    p10_nfa_t nfa; memset(&nfa, 0, sizeof(nfa)); nfa.num_states = 2; bool in_s[P10_MAX_STATES] = {true}, out_s[P10_MAX_STATES]; p10_f11_nfa_epsilon_closure(&nfa, in_s, out_s); assert(out_s[0]);
}

static void test_p10_f12_nfa_step(void) {
    p10_nfa_t nfa; memset(&nfa, 0, sizeof(nfa)); nfa.num_states = 2; nfa.states[0].c = 'a'; nfa.states[0].out1 = 1; nfa.states[0].out2 = -1; bool in_s[P10_MAX_STATES] = {true}, out_s[P10_MAX_STATES]; p10_f12_nfa_step(&nfa, in_s, 'a', out_s); assert(out_s[1]);
}

static void test_p10_f13_nfa_simulate(void) {
    p10_nfa_t nfa; memset(&nfa, 0, sizeof(nfa)); nfa.num_states = 2; nfa.start_state = 0; nfa.states[0].c = 'a'; nfa.states[0].out1 = 1; nfa.states[0].out2 = -1; nfa.states[1].is_match = true; nfa.states[1].out1 = -1; nfa.states[1].out2 = -1; assert(p10_f13_nfa_simulate(&nfa, "a"));
}

static void test_p10_f14_nfa_match_prefix(void) {
    p10_nfa_t nfa; memset(&nfa, 0, sizeof(nfa)); nfa.num_states = 2; nfa.start_state = 0; nfa.states[0].c = 'a'; nfa.states[0].out1 = 1; nfa.states[0].out2 = -1; nfa.states[1].is_match = true; nfa.states[1].out1 = -1; nfa.states[1].out2 = -1; size_t out_len; assert(p10_f14_nfa_match_prefix(&nfa, "ab", &out_len) && out_len == 1);
}

static void test_p10_f16_nfa_reset(void) {
    p10_nfa_t nfa; nfa.num_states = 5; p10_f16_nfa_reset(&nfa); assert(nfa.num_states == 0);
}

static void test_p12_f01_dag_init(void) {
    p12_dag_t g; p12_f01_dag_init(&g, 5); assert(g.num_vertices == 5);
}

static void test_p12_f03_dag_compute_indegrees(void) {
    p12_dag_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 3; g.adj[0][1] = true; size_t in_deg[3]; p12_f03_dag_compute_indegrees(&g, in_deg); assert(in_deg[1] == 1);
}

static void test_p12_f05_dag_kahn_toposort(void) {
    p12_dag_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 3; g.adj[0][1] = true; g.adj[1][2] = true; size_t ord[3], len; assert(p12_f05_dag_kahn_toposort(&g, ord, &len) && len == 3);
}

static void test_p12_f11_dag_assign_levels(void) {
    p12_dag_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 3; g.adj[0][1] = true; size_t lvls[3]; p12_f11_dag_assign_levels(&g, lvls); assert(lvls[1] == 1);
}

int main(void) {
    test_p10_f01_nfa_init();
    test_p10_f02_nfa_add_state();
    test_p10_f03_nfa_set_transition();
    test_p10_f04_nfa_set_match();
    test_p10_f05_nfa_create_literal();
    test_p10_f06_nfa_concat();
    test_p10_f08_nfa_star();
    test_p10_f11_nfa_epsilon_closure();
    test_p10_f12_nfa_step();
    test_p10_f13_nfa_simulate();
    test_p10_f14_nfa_match_prefix();
    test_p10_f16_nfa_reset();
    test_p12_f01_dag_init();
    test_p12_f03_dag_compute_indegrees();
    test_p12_f05_dag_kahn_toposort();
    test_p12_f11_dag_assign_levels();
    printf("PASS: test_p10_reuse075 passed.\n");
    return 0;
}
