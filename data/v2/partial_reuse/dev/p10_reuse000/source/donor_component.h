#ifndef TARGET_DONOR_COMPONENT_H
#define TARGET_DONOR_COMPONENT_H

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
P12_NOINLINE void    p12_f01_dag_init(p12_dag_t *g, size_t n);
P12_NOINLINE bool    p12_f02_dag_add_edge(p12_dag_t *g, size_t u, size_t v, int32_t weight);
P12_NOINLINE void    p12_f03_dag_compute_indegrees(const p12_dag_t *g, size_t *in_deg);
P12_NOINLINE void    p12_f04_dag_compute_outdegrees(const p12_dag_t *g, size_t *out_deg);
P12_NOINLINE bool    p12_f05_dag_kahn_toposort(const p12_dag_t *g, size_t *order, size_t *out_len);
P12_NOINLINE bool    p12_f06_dag_has_cycle(const p12_dag_t *g);
P12_NOINLINE int32_t p12_f07_dag_longest_path(const p12_dag_t *g, size_t src, size_t dst);
P12_NOINLINE int32_t p12_f08_dag_shortest_path(const p12_dag_t *g, size_t src, size_t dst);
P12_NOINLINE size_t  p12_f09_dag_count_ancestors(const p12_dag_t *g, size_t target);
P12_NOINLINE size_t  p12_f10_dag_count_descendants(const p12_dag_t *g, size_t target);
P12_NOINLINE void    p12_f11_dag_assign_levels(const p12_dag_t *g, size_t *levels);
P12_NOINLINE void    p12_f12_dag_transitive_reduction(p12_dag_t *g);
P12_NOINLINE size_t  p12_f13_dag_find_sources(const p12_dag_t *g, size_t *sources, size_t max_s);
P12_NOINLINE size_t  p12_f14_dag_find_sinks(const p12_dag_t *g, size_t *sinks, size_t max_s);
P12_NOINLINE bool    p12_f15_dag_is_reachable(const p12_dag_t *g, size_t src, size_t dst);
P12_NOINLINE void    p12_f16_dag_clear(p12_dag_t *g);

#endif /* TARGET_DONOR_COMPONENT_H */
