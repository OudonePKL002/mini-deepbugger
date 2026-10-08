#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "query_component.h"
#include "donor_component.h"

static void test_p14_f02_bm_set_bit(void) {
    p14_bitmap_t bm; memset(&bm, 0, sizeof(bm)); bm.num_bits = 64; assert(p14_f02_bm_set_bit(&bm, 5) && (bm.words[0] & (1ULL << 5)));
}

static void test_p14_f03_bm_clear_bit(void) {
    p14_bitmap_t bm; memset(&bm, 0, sizeof(bm)); bm.num_bits = 64; bm.words[0] = (1ULL << 5); assert(p14_f03_bm_clear_bit(&bm, 5) && bm.words[0] == 0);
}

static void test_p14_f04_bm_test_bit(void) {
    p14_bitmap_t bm; memset(&bm, 0, sizeof(bm)); bm.num_bits = 64; bm.words[0] = (1ULL << 5); assert(p14_f04_bm_test_bit(&bm, 5));
}

static void test_p14_f08_bm_find_first_zero(void) {
    p14_bitmap_t bm; memset(&bm, 0, sizeof(bm)); bm.num_bits = 64; bm.words[0] = 1; assert(p14_f08_bm_find_first_zero(&bm) == 1);
}

static void test_p14_f09_bm_find_next_set(void) {
    p14_bitmap_t bm; memset(&bm, 0, sizeof(bm)); bm.num_bits = 64; bm.words[0] = (1ULL << 5) | (1ULL << 10); assert(p14_f09_bm_find_next_set(&bm, 6) == 10);
}

static void test_p14_f10_bm_bitwise_and(void) {
    p14_bitmap_t bm; memset(&bm, 0, sizeof(bm)); bm.num_bits = 64; p14_bitmap_t b2; memset(&b2, 0, sizeof(b2)); b2.num_bits = 64; bm.words[0] = 3; b2.words[0] = 2; p14_bitmap_t out; p14_f10_bm_bitwise_and(&out, &bm, &b2); assert(out.words[0] == 2);
}

static void test_p14_f12_bm_bitwise_xor(void) {
    p14_bitmap_t bm; memset(&bm, 0, sizeof(bm)); bm.num_bits = 64; p14_bitmap_t b2; memset(&b2, 0, sizeof(b2)); b2.num_bits = 64; bm.words[0] = 3; b2.words[0] = 2; p14_bitmap_t out; p14_f12_bm_bitwise_xor(&out, &bm, &b2); assert(out.words[0] == 1);
}

static void test_p14_f14_bm_fill_range(void) {
    p14_bitmap_t bm; memset(&bm, 0, sizeof(bm)); bm.num_bits = 64; p14_f14_bm_fill_range(&bm, 0, 4); assert(bm.words[0] == 15);
}

static void test_p09_f01_avl_init(void) {
    p09_tree_t t; p09_f01_avl_init(&t); assert(t.root == -1 && t.pool_size == 0);
}

static void test_p09_f02_avl_height(void) {
    p09_tree_t t; memset(&t, 0, sizeof(t)); t.pool_size = 1; t.root = 0; t.pool[0].key = 10; t.pool[0].height = 1; t.pool[0].left = -1; t.pool[0].right = -1; assert(p09_f02_avl_height(&t, 0) == 1);
}

static void test_p09_f04_avl_rotate_right(void) {
    p09_tree_t t; memset(&t, 0, sizeof(t)); t.pool_size = 2; t.pool[0].key = 20; t.pool[0].left = 1; t.pool[0].right = -1; t.pool[1].key = 10; t.pool[1].left = -1; t.pool[1].right = -1; assert(p09_f04_avl_rotate_right(&t, 0) == 1);
}

static void test_p09_f08_avl_min_node(void) {
    p09_tree_t t; memset(&t, 0, sizeof(t)); t.pool_size = 1; t.root = 0; t.pool[0].key = 10; t.pool[0].height = 1; t.pool[0].left = -1; t.pool[0].right = -1; assert(p09_f08_avl_min_node(&t, 0) == 0);
}

static void test_p09_f11_avl_search(void) {
    p09_tree_t t; memset(&t, 0, sizeof(t)); t.pool_size = 1; t.root = 0; t.pool[0].key = 10; t.pool[0].height = 1; t.pool[0].left = -1; t.pool[0].right = -1; t.pool[0].value = 99; int32_t val; assert(p09_f11_avl_search(&t, 10, &val) && val == 99);
}

static void test_p09_f12_avl_inorder(void) {
    p09_tree_t t; memset(&t, 0, sizeof(t)); t.pool_size = 1; t.root = 0; t.pool[0].key = 10; t.pool[0].height = 1; t.pool[0].left = -1; t.pool[0].right = -1; int32_t keys[1]; assert(p09_f12_avl_inorder(&t, keys, 1) == 1 && keys[0] == 10);
}

static void test_p09_f13_avl_preorder(void) {
    p09_tree_t t; memset(&t, 0, sizeof(t)); t.pool_size = 1; t.root = 0; t.pool[0].key = 10; t.pool[0].height = 1; t.pool[0].left = -1; t.pool[0].right = -1; int32_t keys[1]; assert(p09_f13_avl_preorder(&t, keys, 1) == 1 && keys[0] == 10);
}

static void test_p09_f14_avl_count(void) {
    p09_tree_t t; memset(&t, 0, sizeof(t)); t.pool_size = 1; t.root = 0; t.pool[0].key = 10; t.pool[0].height = 1; t.pool[0].left = -1; t.pool[0].right = -1; assert(p09_f14_avl_count(&t, 0) == 1);
}

int main(void) {
    test_p14_f02_bm_set_bit();
    test_p14_f03_bm_clear_bit();
    test_p14_f04_bm_test_bit();
    test_p14_f08_bm_find_first_zero();
    test_p14_f09_bm_find_next_set();
    test_p14_f10_bm_bitwise_and();
    test_p14_f12_bm_bitwise_xor();
    test_p14_f14_bm_fill_range();
    test_p09_f01_avl_init();
    test_p09_f02_avl_height();
    test_p09_f04_avl_rotate_right();
    test_p09_f08_avl_min_node();
    test_p09_f11_avl_search();
    test_p09_f12_avl_inorder();
    test_p09_f13_avl_preorder();
    test_p09_f14_avl_count();
    printf("PASS: test_p14_reuse050 passed.\n");
    return 0;
}
