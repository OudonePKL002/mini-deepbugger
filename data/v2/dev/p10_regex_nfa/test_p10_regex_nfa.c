#include <stdio.h>
#include <assert.h>
#include "p10_regex_nfa.h"

int main(void) {
    p10_nfa_t nfa;
    p10_f01_nfa_init(&nfa);

    // Build regex (a|b)
    int32_t lit_a = p10_f05_nfa_create_literal(&nfa, 'a');
    int32_t lit_b = p10_f05_nfa_create_literal(&nfa, 'b');
    int32_t alt = p10_f07_nfa_alternate(&nfa, lit_a, lit_b);
    nfa.start_state = alt;

    assert(p10_f13_nfa_simulate(&nfa, "a"));
    assert(p10_f13_nfa_simulate(&nfa, "b"));
    assert(!p10_f13_nfa_simulate(&nfa, "c"));
    assert(!p10_f13_nfa_simulate(&nfa, "ab"));

    size_t out_len = 0;
    assert(p10_f14_nfa_match_prefix(&nfa, "axyz", &out_len) && out_len == 1);

    bool test_set[P10_MAX_STATES] = {true, false, true, true};
    assert(p10_f15_nfa_active_count(test_set, 4) == 3);

    p10_f16_nfa_reset(&nfa);
    assert(nfa.num_states == 0);

    printf("PASS: p10_regex_nfa unit tests passed.\n");
    return 0;
}
