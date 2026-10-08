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
P12_NOINLINE void    p12_f03_dag_compute_indegrees(const p12_dag_t *g, size_t *in_deg);
P12_NOINLINE bool    p12_f05_dag_kahn_toposort(const p12_dag_t *g, size_t *order, size_t *out_len);
P12_NOINLINE void    p12_f11_dag_assign_levels(const p12_dag_t *g, size_t *levels);

#endif /* TARGET_DONOR_COMPONENT_H */
