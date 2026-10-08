#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "query_component.h"
#include "donor_component.h"

static void test_p14_f05_bm_toggle_bit(void) {
    p14_bitmap_t bm; memset(&bm, 0, sizeof(bm)); bm.num_bits = 64; assert(p14_f05_bm_toggle_bit(&bm, 5) && (bm.words[0] & (1ULL << 5)));
}

static void test_p14_f06_bm_popcount(void) {
    p14_bitmap_t bm; memset(&bm, 0, sizeof(bm)); bm.num_bits = 64; bm.words[0] = 7; assert(p14_f06_bm_popcount(&bm) == 3);
}

static void test_p14_f11_bm_bitwise_or(void) {
    p14_bitmap_t bm; memset(&bm, 0, sizeof(bm)); bm.num_bits = 64; p14_bitmap_t b2; memset(&b2, 0, sizeof(b2)); b2.num_bits = 64; bm.words[0] = 1; b2.words[0] = 2; p14_bitmap_t out; p14_f11_bm_bitwise_or(&out, &bm, &b2); assert(out.words[0] == 3);
}

static void test_p14_f12_bm_bitwise_xor(void) {
    p14_bitmap_t bm; memset(&bm, 0, sizeof(bm)); bm.num_bits = 64; p14_bitmap_t b2; memset(&b2, 0, sizeof(b2)); b2.num_bits = 64; bm.words[0] = 3; b2.words[0] = 2; p14_bitmap_t out; p14_f12_bm_bitwise_xor(&out, &bm, &b2); assert(out.words[0] == 1);
}

static void test_p09_f01_avl_init(void) {
    p09_tree_t t; p09_f01_avl_init(&t); assert(t.root == -1 && t.pool_size == 0);
}

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

static void test_p09_f08_avl_min_node(void) {
    p09_tree_t t; memset(&t, 0, sizeof(t)); t.pool_size = 1; t.root = 0; t.pool[0].key = 10; t.pool[0].height = 1; t.pool[0].left = -1; t.pool[0].right = -1; assert(p09_f08_avl_min_node(&t, 0) == 0);
}

static void test_p09_f09_avl_delete_internal(void) {
    p09_tree_t t; memset(&t, 0, sizeof(t)); t.pool_size = 1; t.root = 0; t.pool[0].key = 10; t.pool[0].height = 1; t.pool[0].left = -1; t.pool[0].right = -1; assert(p09_f09_avl_delete_internal(&t, 0, 10) == -1);
}

static void test_p09_f10_avl_delete(void) {
    p09_tree_t t; memset(&t, 0, sizeof(t)); t.pool_size = 1; t.root = 0; t.pool[0].key = 10; t.pool[0].height = 1; t.pool[0].left = -1; t.pool[0].right = -1; assert(p09_f10_avl_delete(&t, 10));
}

static void test_p09_f11_avl_search(void) {
    p09_tree_t t; memset(&t, 0, sizeof(t)); t.pool_size = 1; t.root = 0; t.pool[0].key = 10; t.pool[0].height = 1; t.pool[0].left = -1; t.pool[0].right = -1; t.pool[0].value = 99; int32_t val; assert(p09_f11_avl_search(&t, 10, &val) && val == 99);
}

static void test_p09_f14_avl_count(void) {
    p09_tree_t t; memset(&t, 0, sizeof(t)); t.pool_size = 1; t.root = 0; t.pool[0].key = 10; t.pool[0].height = 1; t.pool[0].left = -1; t.pool[0].right = -1; assert(p09_f14_avl_count(&t, 0) == 1);
}

static void test_p09_f15_avl_is_balanced(void) {
    p09_tree_t t; memset(&t, 0, sizeof(t)); t.pool_size = 1; t.root = 0; t.pool[0].key = 10; t.pool[0].height = 1; t.pool[0].left = -1; t.pool[0].right = -1; assert(p09_f15_avl_is_balanced(&t, 0));
}

static void test_p09_f16_avl_clear(void) {
    p09_tree_t t; memset(&t, 0, sizeof(t)); t.pool_size = 1; t.root = 0; t.pool[0].key = 10; t.pool[0].height = 1; t.pool[0].left = -1; t.pool[0].right = -1; p09_f16_avl_clear(&t); assert(t.root == -1);
}

int main(void) {
    test_p14_f05_bm_toggle_bit();
    test_p14_f06_bm_popcount();
    test_p14_f11_bm_bitwise_or();
    test_p14_f12_bm_bitwise_xor();
    test_p09_f01_avl_init();
    test_p09_f02_avl_height();
    test_p09_f03_avl_balance_factor();
    test_p09_f04_avl_rotate_right();
    test_p09_f05_avl_rotate_left();
    test_p09_f08_avl_min_node();
    test_p09_f09_avl_delete_internal();
    test_p09_f10_avl_delete();
    test_p09_f11_avl_search();
    test_p09_f14_avl_count();
    test_p09_f15_avl_is_balanced();
    test_p09_f16_avl_clear();
    printf("PASS: test_p14_reuse025 passed.\n");
    return 0;
}
