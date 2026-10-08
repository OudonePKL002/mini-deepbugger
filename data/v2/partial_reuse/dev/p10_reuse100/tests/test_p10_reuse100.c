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

static void test_p10_f07_nfa_alternate(void) {
    p10_nfa_t nfa; memset(&nfa, 0, sizeof(nfa)); nfa.num_states = 2; assert(p10_f07_nfa_alternate(&nfa, 0, 1) >= 0);
}

static void test_p10_f08_nfa_star(void) {
    p10_nfa_t nfa; memset(&nfa, 0, sizeof(nfa)); nfa.num_states = 1; assert(p10_f08_nfa_star(&nfa, 0) >= 0);
}

static void test_p10_f09_nfa_plus(void) {
    p10_nfa_t nfa; memset(&nfa, 0, sizeof(nfa)); nfa.num_states = 1; assert(p10_f09_nfa_plus(&nfa, 0) >= 0);
}

static void test_p10_f10_nfa_optional(void) {
    p10_nfa_t nfa; memset(&nfa, 0, sizeof(nfa)); nfa.num_states = 1; assert(p10_f10_nfa_optional(&nfa, 0) >= 0);
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

static void test_p10_f15_nfa_active_count(void) {
    bool s[4] = {true, false, true, true}; assert(p10_f15_nfa_active_count(s, 4) == 3);
}

static void test_p10_f16_nfa_reset(void) {
    p10_nfa_t nfa; nfa.num_states = 5; p10_f16_nfa_reset(&nfa); assert(nfa.num_states == 0);
}

int main(void) {
    test_p10_f01_nfa_init();
    test_p10_f02_nfa_add_state();
    test_p10_f03_nfa_set_transition();
    test_p10_f04_nfa_set_match();
    test_p10_f05_nfa_create_literal();
    test_p10_f06_nfa_concat();
    test_p10_f07_nfa_alternate();
    test_p10_f08_nfa_star();
    test_p10_f09_nfa_plus();
    test_p10_f10_nfa_optional();
    test_p10_f11_nfa_epsilon_closure();
    test_p10_f12_nfa_step();
    test_p10_f13_nfa_simulate();
    test_p10_f14_nfa_match_prefix();
    test_p10_f15_nfa_active_count();
    test_p10_f16_nfa_reset();
    printf("PASS: test_p10_reuse100 passed.\n");
    return 0;
}
