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

static void test_p17_f02_skip_whitespace(void) {
    p17_tokenizer_t t; t.src = "  abc"; t.len = 5; t.pos = 0; p17_f02_skip_whitespace(&t); assert(t.pos == 2);
}

static void test_p17_f05_parse_literal(void) {
    p17_tokenizer_t t; t.src = "true"; t.len = 4; t.pos = 0; p17_token_t tok; assert(p17_f05_parse_literal(&t, "true", P17_TOK_TRUE, &tok));
}

static void test_p17_f09_validate_brackets(void) {
    assert(p17_f09_validate_brackets("{[]}", 4));
}

static void test_p17_f13_extract_string_value(void) {
    p17_token_t tok = {.start = "\"abc\"", .length = 5, .type = P17_TOK_STRING}; char dst[8]; assert(p17_f13_extract_string_value(&tok, dst, 8) == 3 && strcmp(dst, "abc") == 0);
}

static void test_p17_f14_is_valid_number(void) {
    assert(p17_f14_is_valid_number("123", 3));
}

static void test_p17_f15_parse_integer(void) {
    p17_token_t tok = {.start = "123", .length = 3, .type = P17_TOK_NUMBER}; assert(p17_f15_parse_integer(&tok) == 123);
}

static void test_p17_f16_tok_reset(void) {
    p17_tokenizer_t t; t.src = "abc"; t.len = 3; t.pos = 2; p17_f16_tok_reset(&t); assert(t.pos == 0);
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

static void test_p16_f05_btree_split_child(void) {
    p16_btree_t bt; memset(&bt, 0, sizeof(bt)); bt.pool_size = 2; bt.pool[0].is_leaf = false; bt.pool[0].children[0] = 1; bt.pool[1].is_leaf = true; bt.pool[1].keys[0] = 1; bt.pool[1].keys[1] = 2; bt.pool[1].keys[2] = 3; bt.pool[1].num_keys = 3; p16_f05_btree_split_child(&bt, 0, 0, 1); assert(bt.pool[0].num_keys == 1);
}

static void test_p16_f09_btree_traverse(void) {
    p16_btree_t bt; memset(&bt, 0, sizeof(bt)); bt.root = 0; bt.pool_size = 1; bt.pool[0].num_keys = 1; bt.pool[0].keys[0] = 42; bt.pool[0].is_leaf = true; int32_t k[2]; assert(p16_f09_btree_traverse(&bt, 0, k, 2) == 1 && k[0] == 42);
}

static void test_p16_f13_btree_max_key(void) {
    p16_btree_t bt; memset(&bt, 0, sizeof(bt)); bt.root = 0; bt.pool_size = 1; bt.pool[0].num_keys = 1; bt.pool[0].keys[0] = 42; bt.pool[0].is_leaf = true; assert(p16_f13_btree_max_key(&bt, 0) == 42);
}

static void test_p16_f15_btree_is_valid_node(void) {
    p16_btree_t bt; memset(&bt, 0, sizeof(bt)); bt.root = 0; bt.pool_size = 1; bt.pool[0].num_keys = 1; bt.pool[0].keys[0] = 42; bt.pool[0].is_leaf = true; assert(p16_f15_btree_is_valid_node(&bt, 0));
}

int main(void) {
    test_p17_f01_tok_init();
    test_p17_f02_skip_whitespace();
    test_p17_f05_parse_literal();
    test_p17_f09_validate_brackets();
    test_p17_f13_extract_string_value();
    test_p17_f14_is_valid_number();
    test_p17_f15_parse_integer();
    test_p17_f16_tok_reset();
    test_p16_f01_btree_init();
    test_p16_f02_btree_alloc_node();
    test_p16_f03_btree_node_is_full();
    test_p16_f04_btree_find_key_idx();
    test_p16_f05_btree_split_child();
    test_p16_f09_btree_traverse();
    test_p16_f13_btree_max_key();
    test_p16_f15_btree_is_valid_node();
    printf("PASS: test_p17_reuse050 passed.\n");
    return 0;
}
