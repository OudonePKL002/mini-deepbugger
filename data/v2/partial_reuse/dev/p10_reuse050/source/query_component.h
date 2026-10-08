#ifndef TARGET_QUERY_COMPONENT_H
#define TARGET_QUERY_COMPONENT_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define P10_NOINLINE __attribute__((noinline))
#define P10_MAX_STATES 64
#define P10_EPSILON -1

typedef struct {
    int32_t c;              // character match, or P10_EPSILON
    int32_t out1;           // next state 1 (-1 if none)
    int32_t out2;           // next state 2 (-1 if none)
    bool is_match;          // terminal match state
} p10_state_t;

typedef struct {
    p10_state_t states[P10_MAX_STATES];
    size_t num_states;
    int32_t start_state;
} p10_nfa_t;

/* Benchmark Function Prototypes */
P10_NOINLINE void    p10_f01_nfa_init(p10_nfa_t *nfa);
P10_NOINLINE int32_t p10_f02_nfa_add_state(p10_nfa_t *nfa, int32_t c, int32_t out1, int32_t out2);
P10_NOINLINE bool    p10_f03_nfa_set_transition(p10_nfa_t *nfa, int32_t s, int32_t out1, int32_t out2);
P10_NOINLINE bool    p10_f04_nfa_set_match(p10_nfa_t *nfa, int32_t s, bool is_match);
P10_NOINLINE int32_t p10_f06_nfa_concat(p10_nfa_t *nfa, int32_t s1, int32_t s2);
P10_NOINLINE int32_t p10_f07_nfa_alternate(p10_nfa_t *nfa, int32_t s1, int32_t s2);
P10_NOINLINE size_t  p10_f15_nfa_active_count(const bool *set, size_t max_states);
P10_NOINLINE void    p10_f16_nfa_reset(p10_nfa_t *nfa);

#endif /* TARGET_QUERY_COMPONENT_H */
