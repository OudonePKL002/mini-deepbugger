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
P04_NOINLINE void  p04_f02_mat_init_identity(p04_mat3_t *out);
P04_NOINLINE void  p04_f03_mat_add(const p04_mat3_t *a, const p04_mat3_t *b, p04_mat3_t *out);
P04_NOINLINE void  p04_f04_mat_sub(const p04_mat3_t *a, const p04_mat3_t *b, p04_mat3_t *out);
P04_NOINLINE void  p04_f05_mat_scale(const p04_mat3_t *a, float s, p04_mat3_t *out);
P04_NOINLINE void  p04_f06_mat_transpose(const p04_mat3_t *a, p04_mat3_t *out);
P04_NOINLINE void  p04_f08_mat_vec_mul(const p04_mat3_t *a, const p04_vec3_t *x, p04_vec3_t *out);
P04_NOINLINE float p04_f10_mat_det_2x2(float a11, float a12, float a21, float a22);
P04_NOINLINE float p04_f11_mat_det_3x3(const p04_mat3_t *a);
P04_NOINLINE float p04_f12_mat_frobenius_norm(const p04_mat3_t *a);
P04_NOINLINE bool  p04_f15_mat_solve_upper_tri(const p04_mat3_t *u, const p04_vec3_t *b, p04_vec3_t *x);
P04_NOINLINE void  p04_f16_mat_hadamard_product(const p04_mat3_t *a, const p04_mat3_t *b, p04_mat3_t *out);

#endif /* TARGET_DONOR_COMPONENT_H */
