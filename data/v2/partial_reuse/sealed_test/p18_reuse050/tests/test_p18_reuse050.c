#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "query_component.h"
#include "donor_component.h"

static void test_p18_f01_vec_mean(void) {
    double d[3] = {1, 2, 3}; assert(fabs(p18_f01_vec_mean(d, 3) - 2.0) < 1e-6);
}

static void test_p18_f02_vec_variance(void) {
    double d[3] = {1, 2, 3}; assert(p18_f02_vec_variance(d, 3) > 0.0);
}

static void test_p18_f04_vec_min_max(void) {
    double d[3] = {1, 5, 2}; double mn, mx; p18_f04_vec_min_max(d, 3, &mn, &mx); assert(mn == 1.0 && mx == 5.0);
}

static void test_p18_f05_vec_dot_product(void) {
    double a[2] = {1, 2}; double b[2] = {3, 4}; assert(p18_f05_vec_dot_product(a, b, 2) == 11.0);
}

static void test_p18_f07_vec_l2_norm(void) {
    double d[2] = {3, 4}; assert(fabs(p18_f07_vec_l2_norm(d, 2) - 5.0) < 1e-6);
}

static void test_p18_f12_vec_histogram(void) {
    double a[3] = {1, 2, 3}; size_t bins[2] = {0}; p18_f12_vec_histogram(a, 3, 1, 3, bins, 2); assert(bins[0] + bins[1] == 3);
}

static void test_p18_f14_vec_quantile(void) {
    double a[3] = {1, 2, 3}; assert(p18_f14_vec_quantile(a, 3, 0.5) >= 1.0);
}

static void test_p18_f16_vec_scale_add(void) {
    double s[2] = {1, 2}; double d[2]; p18_f16_vec_scale_add(d, s, 2.0, 1.0, 2); assert(d[0] == 3.0 && d[1] == 5.0);
}

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

static void test_p17_f10_minify_json(void) {
    char dst[16]; assert(p17_f10_minify_json("{  }", 4, dst, 16) < 4);
}

static void test_p17_f13_extract_string_value(void) {
    p17_token_t tok = {.start = "\"abc\"", .length = 5, .type = P17_TOK_STRING}; char dst[8]; assert(p17_f13_extract_string_value(&tok, dst, 8) == 3 && strcmp(dst, "abc") == 0);
}

static void test_p17_f15_parse_integer(void) {
    p17_token_t tok = {.start = "123", .length = 3, .type = P17_TOK_NUMBER}; assert(p17_f15_parse_integer(&tok) == 123);
}

int main(void) {
    test_p18_f01_vec_mean();
    test_p18_f02_vec_variance();
    test_p18_f04_vec_min_max();
    test_p18_f05_vec_dot_product();
    test_p18_f07_vec_l2_norm();
    test_p18_f12_vec_histogram();
    test_p18_f14_vec_quantile();
    test_p18_f16_vec_scale_add();
    test_p17_f01_tok_init();
    test_p17_f02_skip_whitespace();
    test_p17_f03_parse_string();
    test_p17_f04_parse_number();
    test_p17_f05_parse_literal();
    test_p17_f10_minify_json();
    test_p17_f13_extract_string_value();
    test_p17_f15_parse_integer();
    printf("PASS: test_p18_reuse050 passed.\n");
    return 0;
}
