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

P10_NOINLINE int32_t p10_f02_nfa_add_state(p10_nfa_t *nfa, int32_t c, int32_t out1, int32_t out2) {
    if (!nfa || nfa->num_states >= P10_MAX_STATES) return -1;
    int32_t idx = (int32_t)nfa->num_states++;
    nfa->states[idx].c = c;
    nfa->states[idx].out1 = out1;
    nfa->states[idx].out2 = out2;
    nfa->states[idx].is_match = false;
    return idx;
}

P10_NOINLINE bool p10_f03_nfa_set_transition(p10_nfa_t *nfa, int32_t s, int32_t out1, int32_t out2) {
    if (!nfa || s < 0 || (size_t)s >= nfa->num_states) return false;
    nfa->states[s].out1 = out1;
    nfa->states[s].out2 = out2;
    return true;
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

P10_NOINLINE int32_t p10_f07_nfa_alternate(p10_nfa_t *nfa, int32_t s1, int32_t s2) {
    if (!nfa || s1 < 0 || s2 < 0) return -1;
    return p10_f02_nfa_add_state(nfa, P10_EPSILON, s1, s2);
}

P10_NOINLINE size_t p10_f15_nfa_active_count(const bool *set, size_t max_states) {
    if (!set) return 0;
    size_t cnt = 0;
    for (size_t i = 0; i < max_states; ++i) if (set[i]) cnt++;
    return cnt;
}

P10_NOINLINE void p10_f16_nfa_reset(p10_nfa_t *nfa) {
    p10_f01_nfa_init(nfa);
}
