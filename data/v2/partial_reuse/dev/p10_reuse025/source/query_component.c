#include "query_component.h"
#include <string.h>
#include <math.h>

P10_NOINLINE void p10_f01_nfa_init(p10_nfa_t *nfa) {
    if (!nfa) return;
    nfa->num_states = 0;
    nfa->start_state = -1;
    for (size_t i = 0; i < P10_MAX_STATES; ++i) {
        nfa->states[i].c = P10_EPSILON;
        nfa->states[i].out1 = -1;
        nfa->states[i].out2 = -1;
        nfa->states[i].is_match = false;
    }
}

P10_NOINLINE bool p10_f04_nfa_set_match(p10_nfa_t *nfa, int32_t s, bool is_match) {
    if (!nfa || s < 0 || (size_t)s >= nfa->num_states) return false;
    nfa->states[s].is_match = is_match;
    return true;
}

P10_NOINLINE int32_t p10_f06_nfa_concat(p10_nfa_t *nfa, int32_t s1, int32_t s2) {
    if (!nfa || s1 < 0 || s2 < 0) return -1;
    for (size_t i = 0; i < nfa->num_states; ++i) {
        if (nfa->states[i].is_match) {
            nfa->states[i].is_match = false;
            nfa->states[i].c = P10_EPSILON;
            nfa->states[i].out1 = s2;
            nfa->states[i].out2 = -1;
            break;
        }
    }
    return s1;
}

P10_NOINLINE void p10_f11_nfa_epsilon_closure(const p10_nfa_t *nfa, const bool *in_set, bool *out_set) {
    if (!nfa || !in_set || !out_set) return;
    for (size_t i = 0; i < nfa->num_states; ++i) out_set[i] = in_set[i];
    bool changed = true;
    while (changed) {
        changed = false;
        for (size_t i = 0; i < nfa->num_states; ++i) {
            if (out_set[i] && nfa->states[i].c == P10_EPSILON) {
                int32_t o1 = nfa->states[i].out1;
                int32_t o2 = nfa->states[i].out2;
                if (o1 >= 0 && !out_set[o1]) { out_set[o1] = true; changed = true; }
                if (o2 >= 0 && !out_set[o2]) { out_set[o2] = true; changed = true; }
            }
        }
    }
}
