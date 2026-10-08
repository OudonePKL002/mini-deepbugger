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

static void test_p17_f03_parse_string(void) {
    p17_tokenizer_t t; t.src = "\"hi\""; t.len = 4; t.pos = 0; p17_token_t tok; assert(p17_f03_parse_string(&t, &tok) && tok.length == 4);
}

static void test_p17_f04_parse_number(void) {
    p17_tokenizer_t t; t.src = "123"; t.len = 3; t.pos = 0; p17_token_t tok; assert(p17_f04_parse_number(&t, &tok) && tok.length == 3);
}

static void test_p17_f05_parse_literal(void) {
    p17_tokenizer_t t; t.src = "true"; t.len = 4; t.pos = 0; p17_token_t tok; assert(p17_f05_parse_literal(&t, "true", P17_TOK_TRUE, &tok));
}

static void test_p17_f06_next_token(void) {
    p17_tokenizer_t t; t.src = "{}"; t.len = 2; t.pos = 0; p17_token_t tok; assert(p17_f06_next_token(&t, &tok) == P17_TOK_START_OBJ);
}

static void test_p17_f08_consume_expected(void) {
    p17_tokenizer_t t; t.src = "{}"; t.len = 2; t.pos = 0; assert(p17_f08_consume_expected(&t, P17_TOK_START_OBJ));
}

static void test_p17_f09_validate_brackets(void) {
    assert(p17_f09_validate_brackets("{[]}", 4));
}

static void test_p17_f10_minify_json(void) {
    char dst[16]; assert(p17_f10_minify_json("{  }", 4, dst, 16) < 4);
}

static void test_p17_f12_find_key_in_object(void) {
    const char j[] = "{\"k\": 1}"; p17_token_t v; assert(p17_f12_find_key_in_object(j, 8, "k", &v));
}

static void test_p17_f14_is_valid_number(void) {
    assert(p17_f14_is_valid_number("123", 3));
}

static void test_p17_f16_tok_reset(void) {
    p17_tokenizer_t t; t.src = "abc"; t.len = 3; t.pos = 2; p17_f16_tok_reset(&t); assert(t.pos == 0);
}

static void test_p16_f01_btree_init(void) {
    p16_btree_t bt; p16_f01_btree_init(&bt); assert(bt.pool_size == 0 && bt.root == -1);
}

static void test_p16_f04_btree_find_key_idx(void) {
    p16_btree_t bt; memset(&bt, 0, sizeof(bt)); bt.root = 0; bt.pool_size = 1; bt.pool[0].num_keys = 1; bt.pool[0].keys[0] = 42; bt.pool[0].is_leaf = true; assert(p16_f04_btree_find_key_idx(&bt.pool[0], 42) == 0);
}

static void test_p16_f11_btree_depth(void) {
    p16_btree_t bt; memset(&bt, 0, sizeof(bt)); bt.root = 0; bt.pool_size = 1; bt.pool[0].num_keys = 1; bt.pool[0].keys[0] = 42; bt.pool[0].is_leaf = true; assert(p16_f11_btree_depth(&bt, 0) >= 1);
}

static void test_p16_f15_btree_is_valid_node(void) {
    p16_btree_t bt; memset(&bt, 0, sizeof(bt)); bt.root = 0; bt.pool_size = 1; bt.pool[0].num_keys = 1; bt.pool[0].keys[0] = 42; bt.pool[0].is_leaf = true; assert(p16_f15_btree_is_valid_node(&bt, 0));
}

int main(void) {
    test_p17_f01_tok_init();
    test_p17_f02_skip_whitespace();
    test_p17_f03_parse_string();
    test_p17_f04_parse_number();
    test_p17_f05_parse_literal();
    test_p17_f06_next_token();
    test_p17_f08_consume_expected();
    test_p17_f09_validate_brackets();
    test_p17_f10_minify_json();
    test_p17_f12_find_key_in_object();
    test_p17_f14_is_valid_number();
    test_p17_f16_tok_reset();
    test_p16_f01_btree_init();
    test_p16_f04_btree_find_key_idx();
    test_p16_f11_btree_depth();
    test_p16_f15_btree_is_valid_node();
    printf("PASS: test_p17_reuse075 passed.\n");
    return 0;
}
