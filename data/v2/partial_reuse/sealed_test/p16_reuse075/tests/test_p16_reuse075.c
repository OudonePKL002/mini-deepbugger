#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "query_component.h"
#include "donor_component.h"

static void test_p16_f02_btree_alloc_node(void) {
    p16_btree_t bt; memset(&bt, 0, sizeof(bt)); bt.root = -1; assert(p16_f02_btree_alloc_node(&bt, true) == 0);
}

static void test_p16_f03_btree_node_is_full(void) {
    p16_btree_t bt; memset(&bt, 0, sizeof(bt)); bt.root = 0; bt.pool_size = 1; bt.pool[0].num_keys = 1; bt.pool[0].keys[0] = 42; bt.pool[0].is_leaf = true; assert(!p16_f03_btree_node_is_full(&bt.pool[0]));
}

static void test_p16_f04_btree_find_key_idx(void) {
    p16_btree_t bt; memset(&bt, 0, sizeof(bt)); bt.root = 0; bt.pool_size = 1; bt.pool[0].num_keys = 1; bt.pool[0].keys[0] = 42; bt.pool[0].is_leaf = true; assert(p16_f04_btree_find_key_idx(&bt.pool[0], 42) == 0);
}

static void test_p16_f05_btree_split_child(void) {
    p16_btree_t bt; memset(&bt, 0, sizeof(bt)); bt.pool_size = 2; bt.pool[0].is_leaf = false; bt.pool[0].children[0] = 1; bt.pool[1].is_leaf = true; bt.pool[1].keys[0] = 1; bt.pool[1].keys[1] = 2; bt.pool[1].keys[2] = 3; bt.pool[1].num_keys = 3; p16_f05_btree_split_child(&bt, 0, 0, 1); assert(bt.pool[0].num_keys == 1);
}

static void test_p16_f06_btree_insert_nonfull(void) {
    p16_btree_t bt; memset(&bt, 0, sizeof(bt)); bt.pool_size = 1; bt.pool[0].is_leaf = true; p16_f06_btree_insert_nonfull(&bt, 0, 42); assert(bt.pool[0].num_keys == 1);
}

static void test_p16_f09_btree_traverse(void) {
    p16_btree_t bt; memset(&bt, 0, sizeof(bt)); bt.root = 0; bt.pool_size = 1; bt.pool[0].num_keys = 1; bt.pool[0].keys[0] = 42; bt.pool[0].is_leaf = true; int32_t k[2]; assert(p16_f09_btree_traverse(&bt, 0, k, 2) == 1 && k[0] == 42);
}

static void test_p16_f10_btree_count_keys(void) {
    p16_btree_t bt; memset(&bt, 0, sizeof(bt)); bt.root = 0; bt.pool_size = 1; bt.pool[0].num_keys = 1; bt.pool[0].keys[0] = 42; bt.pool[0].is_leaf = true; assert(p16_f10_btree_count_keys(&bt, 0) == 1);
}

static void test_p16_f11_btree_depth(void) {
    p16_btree_t bt; memset(&bt, 0, sizeof(bt)); bt.root = 0; bt.pool_size = 1; bt.pool[0].num_keys = 1; bt.pool[0].keys[0] = 42; bt.pool[0].is_leaf = true; assert(p16_f11_btree_depth(&bt, 0) >= 1);
}

static void test_p16_f12_btree_min_key(void) {
    p16_btree_t bt; memset(&bt, 0, sizeof(bt)); bt.root = 0; bt.pool_size = 1; bt.pool[0].num_keys = 1; bt.pool[0].keys[0] = 42; bt.pool[0].is_leaf = true; assert(p16_f12_btree_min_key(&bt, 0) == 42);
}

static void test_p16_f13_btree_max_key(void) {
    p16_btree_t bt; memset(&bt, 0, sizeof(bt)); bt.root = 0; bt.pool_size = 1; bt.pool[0].num_keys = 1; bt.pool[0].keys[0] = 42; bt.pool[0].is_leaf = true; assert(p16_f13_btree_max_key(&bt, 0) == 42);
}

static void test_p16_f15_btree_is_valid_node(void) {
    p16_btree_t bt; memset(&bt, 0, sizeof(bt)); bt.root = 0; bt.pool_size = 1; bt.pool[0].num_keys = 1; bt.pool[0].keys[0] = 42; bt.pool[0].is_leaf = true; assert(p16_f15_btree_is_valid_node(&bt, 0));
}

static void test_p16_f16_btree_clear(void) {
    p16_btree_t bt; memset(&bt, 0, sizeof(bt)); bt.root = 0; bt.pool_size = 1; bt.pool[0].num_keys = 1; bt.pool[0].keys[0] = 42; bt.pool[0].is_leaf = true; p16_f16_btree_clear(&bt); assert(bt.pool_size == 0 && bt.root == -1);
}

static void test_p15_f02_fnv1a_32(void) {
    const uint8_t msg[] = "MiniDeepBuggerV2"; assert(p15_f02_fnv1a_32(msg, sizeof(msg)-1) != 0);
}

static void test_p15_f04_fnv1a_64(void) {
    const uint8_t msg[] = "MiniDeepBuggerV2"; assert(p15_f04_fnv1a_64(msg, sizeof(msg)-1) != 0);
}

static void test_p15_f13_ap_hash(void) {
    assert(p15_f13_ap_hash("TestString") != 0);
}

static void test_p15_f15_hash_combine64(void) {
    assert(p15_f15_hash_combine64(10, 20) != 0);
}

int main(void) {
    test_p16_f02_btree_alloc_node();
    test_p16_f03_btree_node_is_full();
    test_p16_f04_btree_find_key_idx();
    test_p16_f05_btree_split_child();
    test_p16_f06_btree_insert_nonfull();
    test_p16_f09_btree_traverse();
    test_p16_f10_btree_count_keys();
    test_p16_f11_btree_depth();
    test_p16_f12_btree_min_key();
    test_p16_f13_btree_max_key();
    test_p16_f15_btree_is_valid_node();
    test_p16_f16_btree_clear();
    test_p15_f02_fnv1a_32();
    test_p15_f04_fnv1a_64();
    test_p15_f13_ap_hash();
    test_p15_f15_hash_combine64();
    printf("PASS: test_p16_reuse075 passed.\n");
    return 0;
}
