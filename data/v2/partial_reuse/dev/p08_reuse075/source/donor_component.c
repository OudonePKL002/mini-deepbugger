#include "donor_component.h"
#include <string.h>
#include <math.h>

P10_NOINLINE int32_t p10_f02_nfa_add_state(p10_nfa_t *nfa, int32_t c, int32_t out1, int32_t out2) {
    if (!nfa || nfa->num_states >= P10_MAX_STATES) return -1;
    int32_t idx = (int32_t)nfa->num_states++;
    nfa->states[idx].c = c;
    nfa->states[idx].out1 = out1;
    nfa->states[idx].out2 = out2;
    nfa->states[idx].is_match = false;
    return idx;
}

P10_NOINLINE int32_t p10_f07_nfa_alternate(p10_nfa_t *nfa, int32_t s1, int32_t s2) {
    if (!nfa || s1 < 0 || s2 < 0) return -1;
    return p10_f02_nfa_add_state(nfa, P10_EPSILON, s1, s2);
}

P10_NOINLINE int32_t p10_f08_nfa_star(p10_nfa_t *nfa, int32_t s) {
    if (!nfa || s < 0) return -1;
    int32_t new_start = p10_f02_nfa_add_state(nfa, P10_EPSILON, s, -1);
    for (size_t i = 0; i < nfa->num_states; ++i) {
        if (nfa->states[i].is_match) {
            nfa->states[i].out2 = s;
        }
    }
    return new_start;
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
