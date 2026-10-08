#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "query_component.h"
#include "donor_component.h"

static void test_p08_f01_adler32(void) {
    const uint8_t s[] = "DevBenchmarkCorpus2026"; assert(p08_f01_adler32(s, sizeof(s)-1) != 0);
}

static void test_p08_f02_siphash_round(void) {
    uint64_t v[4] = {1, 2, 3, 4}; p08_f02_siphash_round(v); assert(v[0] != 1);
}

static void test_p08_f04_half_siphash(void) {
    const uint8_t s[] = "DevBenchmarkCorpus2026"; uint8_t k[8] = {1,2,3,4,5,6,7,8}; assert(p08_f04_half_siphash(s, sizeof(s)-1, k) != 0);
}

static void test_p08_f06_sdbm_hash(void) {
    assert(p08_f06_sdbm_hash("test_string") != 0);
}

static void test_p08_f07_rabin_karp_rolling(void) {
    assert(p08_f07_rabin_karp_rolling(100, 'a', 'b', 31, 961) != 0);
}

static void test_p08_f10_rot13_cipher(void) {
    char b[16]; p08_f10_rot13_cipher(b, "Hello", 5); b[5] = 0; assert(strcmp(b, "Uryyb") == 0);
}

static void test_p08_f12_crc16_usb(void) {
    const uint8_t s[] = "DevBenchmarkCorpus2026"; assert(p08_f12_crc16_usb(s, sizeof(s)-1) != 0);
}

static void test_p08_f14_chash_simple(void) {
    const uint8_t s[] = "DevBenchmarkCorpus2026"; assert(p08_f14_chash_simple(s, sizeof(s)-1, 999) != 0);
}

static void test_p10_f01_nfa_init(void) {
    p10_nfa_t nfa; nfa.num_states = 5; p10_f01_nfa_init(&nfa); assert(nfa.num_states == 0);
}

static void test_p10_f02_nfa_add_state(void) {
    p10_nfa_t nfa; memset(&nfa, 0, sizeof(nfa)); assert(p10_f02_nfa_add_state(&nfa, 'a', -1, -1) == 0);
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

static void test_p10_f11_nfa_epsilon_closure(void) {
    p10_nfa_t nfa; memset(&nfa, 0, sizeof(nfa)); nfa.num_states = 2; bool in_s[P10_MAX_STATES] = {true}, out_s[P10_MAX_STATES]; p10_f11_nfa_epsilon_closure(&nfa, in_s, out_s); assert(out_s[0]);
}

static void test_p10_f12_nfa_step(void) {
    p10_nfa_t nfa; memset(&nfa, 0, sizeof(nfa)); nfa.num_states = 2; nfa.states[0].c = 'a'; nfa.states[0].out1 = 1; nfa.states[0].out2 = -1; bool in_s[P10_MAX_STATES] = {true}, out_s[P10_MAX_STATES]; p10_f12_nfa_step(&nfa, in_s, 'a', out_s); assert(out_s[1]);
}

static void test_p10_f16_nfa_reset(void) {
    p10_nfa_t nfa; nfa.num_states = 5; p10_f16_nfa_reset(&nfa); assert(nfa.num_states == 0);
}

int main(void) {
    test_p08_f01_adler32();
    test_p08_f02_siphash_round();
    test_p08_f04_half_siphash();
    test_p08_f06_sdbm_hash();
    test_p08_f07_rabin_karp_rolling();
    test_p08_f10_rot13_cipher();
    test_p08_f12_crc16_usb();
    test_p08_f14_chash_simple();
    test_p10_f01_nfa_init();
    test_p10_f02_nfa_add_state();
    test_p10_f04_nfa_set_match();
    test_p10_f05_nfa_create_literal();
    test_p10_f06_nfa_concat();
    test_p10_f11_nfa_epsilon_closure();
    test_p10_f12_nfa_step();
    test_p10_f16_nfa_reset();
    printf("PASS: test_p08_reuse050 passed.\n");
    return 0;
}
