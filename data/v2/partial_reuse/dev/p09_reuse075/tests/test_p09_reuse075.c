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

static void test_p09_f03_avl_balance_factor(void) {
    p09_tree_t t; memset(&t, 0, sizeof(t)); t.pool_size = 1; t.root = 0; t.pool[0].key = 10; t.pool[0].height = 1; t.pool[0].left = -1; t.pool[0].right = -1; assert(p09_f03_avl_balance_factor(&t, 0) == 0);
}

static void test_p09_f04_avl_rotate_right(void) {
    p09_tree_t t; memset(&t, 0, sizeof(t)); t.pool_size = 2; t.pool[0].key = 20; t.pool[0].left = 1; t.pool[0].right = -1; t.pool[1].key = 10; t.pool[1].left = -1; t.pool[1].right = -1; assert(p09_f04_avl_rotate_right(&t, 0) == 1);
}

static void test_p09_f05_avl_rotate_left(void) {
    p09_tree_t t; memset(&t, 0, sizeof(t)); t.pool_size = 2; t.pool[0].key = 10; t.pool[0].left = -1; t.pool[0].right = 1; t.pool[1].key = 20; t.pool[1].left = -1; t.pool[1].right = -1; assert(p09_f05_avl_rotate_left(&t, 0) == 1);
}

static void test_p09_f06_avl_insert_internal(void) {
    p09_tree_t t; memset(&t, 0, sizeof(t)); t.root = -1; assert(p09_f06_avl_insert_internal(&t, -1, 42, 100) == 0);
}

static void test_p09_f07_avl_insert(void) {
    p09_tree_t t; memset(&t, 0, sizeof(t)); t.root = -1; assert(p09_f07_avl_insert(&t, 42, 100));
}

static void test_p09_f08_avl_min_node(void) {
    p09_tree_t t; memset(&t, 0, sizeof(t)); t.pool_size = 1; t.root = 0; t.pool[0].key = 10; t.pool[0].height = 1; t.pool[0].left = -1; t.pool[0].right = -1; assert(p09_f08_avl_min_node(&t, 0) == 0);
}

static void test_p09_f09_avl_delete_internal(void) {
    p09_tree_t t; memset(&t, 0, sizeof(t)); t.pool_size = 1; t.root = 0; t.pool[0].key = 10; t.pool[0].height = 1; t.pool[0].left = -1; t.pool[0].right = -1; assert(p09_f09_avl_delete_internal(&t, 0, 10) == -1);
}

static void test_p09_f10_avl_delete(void) {
    p09_tree_t t; memset(&t, 0, sizeof(t)); t.pool_size = 1; t.root = 0; t.pool[0].key = 10; t.pool[0].height = 1; t.pool[0].left = -1; t.pool[0].right = -1; assert(p09_f10_avl_delete(&t, 10));
}

static void test_p09_f12_avl_inorder(void) {
    p09_tree_t t; memset(&t, 0, sizeof(t)); t.pool_size = 1; t.root = 0; t.pool[0].key = 10; t.pool[0].height = 1; t.pool[0].left = -1; t.pool[0].right = -1; int32_t keys[1]; assert(p09_f12_avl_inorder(&t, keys, 1) == 1 && keys[0] == 10);
}

static void test_p09_f13_avl_preorder(void) {
    p09_tree_t t; memset(&t, 0, sizeof(t)); t.pool_size = 1; t.root = 0; t.pool[0].key = 10; t.pool[0].height = 1; t.pool[0].left = -1; t.pool[0].right = -1; int32_t keys[1]; assert(p09_f13_avl_preorder(&t, keys, 1) == 1 && keys[0] == 10);
}

static void test_p09_f15_avl_is_balanced(void) {
    p09_tree_t t; memset(&t, 0, sizeof(t)); t.pool_size = 1; t.root = 0; t.pool[0].key = 10; t.pool[0].height = 1; t.pool[0].left = -1; t.pool[0].right = -1; assert(p09_f15_avl_is_balanced(&t, 0));
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

static void test_p11_f12_fp_cos_cordic(void) {
    assert(abs(p11_f12_fp_cos_cordic(0) - 65536) < 100);
}

int main(void) {
    test_p09_f02_avl_height();
    test_p09_f03_avl_balance_factor();
    test_p09_f04_avl_rotate_right();
    test_p09_f05_avl_rotate_left();
    test_p09_f06_avl_insert_internal();
    test_p09_f07_avl_insert();
    test_p09_f08_avl_min_node();
    test_p09_f09_avl_delete_internal();
    test_p09_f10_avl_delete();
    test_p09_f12_avl_inorder();
    test_p09_f13_avl_preorder();
    test_p09_f15_avl_is_balanced();
    test_p11_f06_fp_div();
    test_p11_f07_fp_abs();
    test_p11_f11_fp_sin_cordic();
    test_p11_f12_fp_cos_cordic();
    printf("PASS: test_p09_reuse075 passed.\n");
    return 0;
}
