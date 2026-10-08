#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "query_component.h"
#include "donor_component.h"

static void test_p09_f02_avl_height(void) {
    p09_tree_t t; memset(&t, 0, sizeof(t)); t.pool_size = 1; t.root = 0; t.pool[0].key = 10; t.pool[0].height = 1; t.pool[0].left = -1; t.pool[0].right = -1; assert(p09_f02_avl_height(&t, 0) == 1);
}

static void test_p09_f04_avl_rotate_right(void) {
    p09_tree_t t; memset(&t, 0, sizeof(t)); t.pool_size = 2; t.pool[0].key = 20; t.pool[0].left = 1; t.pool[0].right = -1; t.pool[1].key = 10; t.pool[1].left = -1; t.pool[1].right = -1; assert(p09_f04_avl_rotate_right(&t, 0) == 1);
}

static void test_p09_f05_avl_rotate_left(void) {
    p09_tree_t t; memset(&t, 0, sizeof(t)); t.pool_size = 2; t.pool[0].key = 10; t.pool[0].left = -1; t.pool[0].right = 1; t.pool[1].key = 20; t.pool[1].left = -1; t.pool[1].right = -1; assert(p09_f05_avl_rotate_left(&t, 0) == 1);
}

static void test_p09_f08_avl_min_node(void) {
    p09_tree_t t; memset(&t, 0, sizeof(t)); t.pool_size = 1; t.root = 0; t.pool[0].key = 10; t.pool[0].height = 1; t.pool[0].left = -1; t.pool[0].right = -1; assert(p09_f08_avl_min_node(&t, 0) == 0);
}

static void test_p09_f11_avl_search(void) {
    p09_tree_t t; memset(&t, 0, sizeof(t)); t.pool_size = 1; t.root = 0; t.pool[0].key = 10; t.pool[0].height = 1; t.pool[0].left = -1; t.pool[0].right = -1; t.pool[0].value = 99; int32_t val; assert(p09_f11_avl_search(&t, 10, &val) && val == 99);
}

static void test_p09_f13_avl_preorder(void) {
    p09_tree_t t; memset(&t, 0, sizeof(t)); t.pool_size = 1; t.root = 0; t.pool[0].key = 10; t.pool[0].height = 1; t.pool[0].left = -1; t.pool[0].right = -1; int32_t keys[1]; assert(p09_f13_avl_preorder(&t, keys, 1) == 1 && keys[0] == 10);
}

static void test_p09_f14_avl_count(void) {
    p09_tree_t t; memset(&t, 0, sizeof(t)); t.pool_size = 1; t.root = 0; t.pool[0].key = 10; t.pool[0].height = 1; t.pool[0].left = -1; t.pool[0].right = -1; assert(p09_f14_avl_count(&t, 0) == 1);
}

static void test_p09_f16_avl_clear(void) {
    p09_tree_t t; memset(&t, 0, sizeof(t)); t.pool_size = 1; t.root = 0; t.pool[0].key = 10; t.pool[0].height = 1; t.pool[0].left = -1; t.pool[0].right = -1; p09_f16_avl_clear(&t); assert(t.root == -1);
}

static void test_p11_f02_fp_to_int(void) {
    assert(p11_f02_fp_to_int(5 << P11_FP_SHIFT) == 5);
}

static void test_p11_f03_fp_add(void) {
    assert(p11_f03_fp_add(10, 20) == 30);
}

static void test_p11_f04_fp_sub(void) {
    assert(p11_f04_fp_sub(30, 10) == 20);
}

static void test_p11_f05_fp_mul(void) {
    p11_q16_t a = 2 * 65536, b = 3 * 65536; assert(p11_f05_fp_mul(a, b) == 6 * 65536);
}

static void test_p11_f06_fp_div(void) {
    p11_q16_t a = 6 * 65536, b = 2 * 65536; assert(p11_f06_fp_div(a, b) == 3 * 65536);
}

static void test_p11_f07_fp_abs(void) {
    assert(p11_f07_fp_abs(-50) == 50);
}

static void test_p11_f11_fp_sin_cordic(void) {
    assert(abs(p11_f11_fp_sin_cordic(0)) < 100);
}

static void test_p11_f15_fp_clamp(void) {
    assert(p11_f15_fp_clamp(15, 0, 10) == 10);
}

int main(void) {
    test_p09_f02_avl_height();
    test_p09_f04_avl_rotate_right();
    test_p09_f05_avl_rotate_left();
    test_p09_f08_avl_min_node();
    test_p09_f11_avl_search();
    test_p09_f13_avl_preorder();
    test_p09_f14_avl_count();
    test_p09_f16_avl_clear();
    test_p11_f02_fp_to_int();
    test_p11_f03_fp_add();
    test_p11_f04_fp_sub();
    test_p11_f05_fp_mul();
    test_p11_f06_fp_div();
    test_p11_f07_fp_abs();
    test_p11_f11_fp_sin_cordic();
    test_p11_f15_fp_clamp();
    printf("PASS: test_p09_reuse050 passed.\n");
    return 0;
}
