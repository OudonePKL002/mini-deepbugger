#ifndef TARGET_DONOR_COMPONENT_H
#define TARGET_DONOR_COMPONENT_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define P04_NOINLINE __attribute__((noinline))

typedef struct {
    float m[3][3];
} p04_mat3_t;

typedef struct {
    float v[3];
} p04_vec3_t;

/* Benchmark Function Prototypes */
P04_NOINLINE void  p04_f01_mat_init_zero(p04_mat3_t *out);
P04_NOINLINE float p04_f09_mat_trace(const p04_mat3_t *a);
P04_NOINLINE float p04_f12_mat_frobenius_norm(const p04_mat3_t *a);
P04_NOINLINE bool  p04_f15_mat_solve_upper_tri(const p04_mat3_t *u, const p04_vec3_t *b, p04_vec3_t *x);

#endif /* TARGET_DONOR_COMPONENT_H */
