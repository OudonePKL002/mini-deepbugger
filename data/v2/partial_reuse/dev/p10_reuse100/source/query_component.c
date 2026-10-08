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

P10_NOINLINE int32_t p10_f05_nfa_create_literal(p10_nfa_t *nfa, char c) {
    if (!nfa) return -1;
    int32_t match_state = p10_f02_nfa_add_state(nfa, P10_EPSILON, -1, -1);
    if (match_state < 0) return -1;
    p10_f04_nfa_set_match(nfa, match_state, true);
    int32_t lit_state = p10_f02_nfa_add_state(nfa, (int32_t)(uint8_t)c, match_state, -1);
    return lit_state;
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

P10_NOINLINE int32_t p10_f09_nfa_plus(p10_nfa_t *nfa, int32_t s) {
    if (!nfa || s < 0) return -1;
    for (size_t i = 0; i < nfa->num_states; ++i) {
        if (nfa->states[i].is_match) {
            nfa->states[i].out2 = s;
        }
    }
    return s;
}

P10_NOINLINE int32_t p10_f10_nfa_optional(p10_nfa_t *nfa, int32_t s) {
    if (!nfa || s < 0) return -1;
    int32_t match_state = -1;
    for (size_t i = 0; i < nfa->num_states; ++i) {
        if (nfa->states[i].is_match) { match_state = (int32_t)i; break; }
    }
    return p10_f02_nfa_add_state(nfa, P10_EPSILON, s, match_state);
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

P10_NOINLINE void p10_f12_nfa_step(const p10_nfa_t *nfa, const bool *curr_set, char c, bool *next_set) {
    if (!nfa || !curr_set || !next_set) return;
    bool temp[P10_MAX_STATES];
    for (size_t i = 0; i < nfa->num_states; ++i) temp[i] = false;

    for (size_t i = 0; i < nfa->num_states; ++i) {
        if (curr_set[i] && nfa->states[i].c == (int32_t)(uint8_t)c) {
            int32_t o1 = nfa->states[i].out1;
            if (o1 >= 0) temp[o1] = true;
        }
    }
    p10_f11_nfa_epsilon_closure(nfa, temp, next_set);
}

P10_NOINLINE bool p10_f13_nfa_simulate(const p10_nfa_t *nfa, const char *text) {
    if (!nfa || nfa->start_state < 0 || !text) return false;
    bool curr[P10_MAX_STATES];
    bool init_set[P10_MAX_STATES];
    for (size_t i = 0; i < nfa->num_states; ++i) init_set[i] = false;
    init_set[nfa->start_state] = true;
    p10_f11_nfa_epsilon_closure(nfa, init_set, curr);

    for (size_t i = 0; text[i] != '\0'; ++i) {
        bool next[P10_MAX_STATES];
        p10_f12_nfa_step(nfa, curr, text[i], next);
        for (size_t j = 0; j < nfa->num_states; ++j) curr[j] = next[j];
    }
    for (size_t i = 0; i < nfa->num_states; ++i) {
        if (curr[i] && nfa->states[i].is_match) return true;
    }
    return false;
}

P10_NOINLINE bool p10_f14_nfa_match_prefix(const p10_nfa_t *nfa, const char *text, size_t *out_len) {
    if (!nfa || nfa->start_state < 0 || !text) return false;
    bool curr[P10_MAX_STATES];
    bool init_set[P10_MAX_STATES];
    for (size_t i = 0; i < nfa->num_states; ++i) init_set[i] = false;
    init_set[nfa->start_state] = true;
    p10_f11_nfa_epsilon_closure(nfa, init_set, curr);

    size_t matched_len = 0;
    bool had_match = false;
    for (size_t i = 0; i < nfa->num_states; ++i) {
        if (curr[i] && nfa->states[i].is_match) { had_match = true; matched_len = 0; }
    }
    for (size_t i = 0; text[i] != '\0'; ++i) {
        bool next[P10_MAX_STATES];
        p10_f12_nfa_step(nfa, curr, text[i], next);
        for (size_t j = 0; j < nfa->num_states; ++j) curr[j] = next[j];
        for (size_t j = 0; j < nfa->num_states; ++j) {
            if (curr[j] && nfa->states[j].is_match) { had_match = true; matched_len = i + 1; }
        }
    }
    if (had_match && out_len) *out_len = matched_len;
    return had_match;
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
