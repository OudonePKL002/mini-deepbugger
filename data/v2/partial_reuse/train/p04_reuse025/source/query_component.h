#ifndef TARGET_QUERY_COMPONENT_H
#define TARGET_QUERY_COMPONENT_H

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
P04_NOINLINE void  p04_f02_mat_init_identity(p04_mat3_t *out);
P04_NOINLINE void  p04_f03_mat_add(const p04_mat3_t *a, const p04_mat3_t *b, p04_mat3_t *out);
P04_NOINLINE void  p04_f07_mat_mul(const p04_mat3_t *a, const p04_mat3_t *b, p04_mat3_t *out);
P04_NOINLINE float p04_f10_mat_det_2x2(float a11, float a12, float a21, float a22);

#endif /* TARGET_QUERY_COMPONENT_H */
