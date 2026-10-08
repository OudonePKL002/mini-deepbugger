#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "query_component.h"
#include "donor_component.h"

static void test_p17_f01_tok_init(void) {
    const char j[] = "{}"; p17_tokenizer_t t; p17_f01_tok_init(&t, j, 2); assert(t.pos == 0);
}

static void test_p17_f03_parse_string(void) {
    p17_tokenizer_t t; t.src = "\"hi\""; t.len = 4; t.pos = 0; p17_token_t tok; assert(p17_f03_parse_string(&t, &tok) && tok.length == 4);
}

static void test_p17_f04_parse_number(void) {
    p17_tokenizer_t t; t.src = "123"; t.len = 3; t.pos = 0; p17_token_t tok; assert(p17_f04_parse_number(&t, &tok) && tok.length == 3);
}

static void test_p17_f09_validate_brackets(void) {
    assert(p17_f09_validate_brackets("{[]}", 4));
}

static void test_p16_f01_btree_init(void) {
    p16_btree_t bt; p16_f01_btree_init(&bt); assert(bt.pool_size == 0 && bt.root == -1);
}

static void test_p16_f02_btree_alloc_node(void) {
    p16_btree_t bt; memset(&bt, 0, sizeof(bt)); bt.root = -1; assert(p16_f02_btree_alloc_node(&bt, true) == 0);
}

static void test_p16_f03_btree_node_is_full(void) {
    p16_btree_t bt; memset(&bt, 0, sizeof(bt)); bt.root = 0; bt.pool_size = 1; bt.pool[0].num_keys = 1; bt.pool[0].keys[0] = 42; bt.pool[0].is_leaf = true; assert(!p16_f03_btree_node_is_full(&bt.pool[0]));
}

static void test_p16_f04_btree_find_key_idx(void) {
    p16_btree_t bt; memset(&bt, 0, sizeof(bt)); bt.root = 0; bt.pool_size = 1; bt.pool[0].num_keys = 1; bt.pool[0].keys[0] = 42; bt.pool[0].is_leaf = true; assert(p16_f04_btree_find_key_idx(&bt.pool[0], 42) == 0);
}

static void test_p16_f08_btree_search(void) {
    p16_btree_t bt; memset(&bt, 0, sizeof(bt)); bt.root = 0; bt.pool_size = 1; bt.pool[0].num_keys = 1; bt.pool[0].keys[0] = 42; bt.pool[0].is_leaf = true; assert(p16_f08_btree_search(&bt, 0, 42));
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

static void test_p16_f13_btree_max_key(void) {
    p16_btree_t bt; memset(&bt, 0, sizeof(bt)); bt.root = 0; bt.pool_size = 1; bt.pool[0].num_keys = 1; bt.pool[0].keys[0] = 42; bt.pool[0].is_leaf = true; assert(p16_f13_btree_max_key(&bt, 0) == 42);
}

static void test_p16_f14_btree_contains(void) {
    p16_btree_t bt; memset(&bt, 0, sizeof(bt)); bt.root = 0; bt.pool_size = 1; bt.pool[0].num_keys = 1; bt.pool[0].keys[0] = 42; bt.pool[0].is_leaf = true; assert(p16_f14_btree_contains(&bt, 42));
}

static void test_p16_f15_btree_is_valid_node(void) {
    p16_btree_t bt; memset(&bt, 0, sizeof(bt)); bt.root = 0; bt.pool_size = 1; bt.pool[0].num_keys = 1; bt.pool[0].keys[0] = 42; bt.pool[0].is_leaf = true; assert(p16_f15_btree_is_valid_node(&bt, 0));
}

static void test_p16_f16_btree_clear(void) {
    p16_btree_t bt; memset(&bt, 0, sizeof(bt)); bt.root = 0; bt.pool_size = 1; bt.pool[0].num_keys = 1; bt.pool[0].keys[0] = 42; bt.pool[0].is_leaf = true; p16_f16_btree_clear(&bt); assert(bt.pool_size == 0 && bt.root == -1);
}

int main(void) {
    test_p17_f01_tok_init();
    test_p17_f03_parse_string();
    test_p17_f04_parse_number();
    test_p17_f09_validate_brackets();
    test_p16_f01_btree_init();
    test_p16_f02_btree_alloc_node();
    test_p16_f03_btree_node_is_full();
    test_p16_f04_btree_find_key_idx();
    test_p16_f08_btree_search();
    test_p16_f09_btree_traverse();
    test_p16_f10_btree_count_keys();
    test_p16_f11_btree_depth();
    test_p16_f13_btree_max_key();
    test_p16_f14_btree_contains();
    test_p16_f15_btree_is_valid_node();
    test_p16_f16_btree_clear();
    printf("PASS: test_p17_reuse025 passed.\n");
    return 0;
}
