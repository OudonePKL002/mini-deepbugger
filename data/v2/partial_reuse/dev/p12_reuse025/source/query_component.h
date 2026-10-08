#ifndef TARGET_QUERY_COMPONENT_H
#define TARGET_QUERY_COMPONENT_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define P12_NOINLINE __attribute__((noinline))
#define P12_MAX_VERTICES 32

typedef struct {
    uint8_t adj[P12_MAX_VERTICES][P12_MAX_VERTICES];
    int32_t weights[P12_MAX_VERTICES][P12_MAX_VERTICES];
    size_t num_vertices;
} p12_dag_t;

/* Benchmark Function Prototypes */
P12_NOINLINE bool    p12_f02_dag_add_edge(p12_dag_t *g, size_t u, size_t v, int32_t weight);
P12_NOINLINE size_t  p12_f10_dag_count_descendants(const p12_dag_t *g, size_t target);
P12_NOINLINE void    p12_f12_dag_transitive_reduction(p12_dag_t *g);
P12_NOINLINE bool    p12_f15_dag_is_reachable(const p12_dag_t *g, size_t src, size_t dst);

#endif /* TARGET_QUERY_COMPONENT_H */
