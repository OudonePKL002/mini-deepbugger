#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "p04_matrix_linear.h"

int main(void) {
    p04_mat3_t a, b, out;
    p04_f01_mat_init_zero(&a);
    assert(a.m[0][0] == 0.0f);

    p04_f02_mat_init_identity(&a);
    assert(a.m[0][0] == 1.0f && a.m[0][1] == 0.0f);

    p04_f02_mat_init_identity(&b);
    p04_f03_mat_add(&a, &b, &out);
    assert(out.m[0][0] == 2.0f);

    p04_f04_mat_sub(&out, &b, &out);
    assert(out.m[0][0] == 1.0f);

    p04_f05_mat_scale(&a, 3.0f, &out);
    assert(out.m[0][0] == 3.0f);

    out.m[0][1] = 4.0f;
    p04_mat3_t tr;
    p04_f06_mat_transpose(&out, &tr);
    assert(tr.m[1][0] == 4.0f);

    p04_mat3_t m1, m2, m3;
    p04_f02_mat_init_identity(&m1);
    p04_f02_mat_init_identity(&m2);
    p04_f07_mat_mul(&m1, &m2, &m3);
    assert(m3.m[0][0] == 1.0f);

    p04_vec3_t v = {{1.0f, 2.0f, 3.0f}}, v_out;
    p04_f08_mat_vec_mul(&m1, &v, &v_out);
    assert(v_out.v[1] == 2.0f);

    assert(p04_f09_mat_trace(&m1) == 3.0f);
    assert(p04_f10_mat_det_2x2(1, 2, 3, 4) == -2.0f);
    assert(p04_f11_mat_det_3x3(&m1) == 1.0f);

    float norm = p04_f12_mat_frobenius_norm(&m1);
    assert(fabsf(norm - sqrtf(3.0f)) < 1e-4f);

    assert(p04_f13_mat_is_symmetric(&m1, 1e-5f) == true);

    p04_mat3_t l, u;
    p04_mat3_t test_lu = {{{2, -1, -2}, {-4, 6, 3}, {-4, -2, 8}}};
    assert(p04_f14_mat_lu_decompose_3x3(&test_lu, &l, &u) == true);

    p04_vec3_t sol;
    p04_vec3_t b_vec = {{1, 2, 3}};
    p04_mat3_t u_mat = {{{2, 1, 1}, {0, 1, 2}, {0, 0, 1}}};
    assert(p04_f15_mat_solve_upper_tri(&u_mat, &b_vec, &sol) == true);
    assert(sol.v[2] == 3.0f);

    p04_f16_mat_hadamard_product(&m1, &m2, &out);
    assert(out.m[0][0] == 1.0f);

    printf("PASS: p04_matrix_linear unit tests passed.\n");
    return 0;
}
