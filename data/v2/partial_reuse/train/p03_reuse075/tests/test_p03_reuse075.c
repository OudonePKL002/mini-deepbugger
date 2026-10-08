#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "query_component.h"
#include "donor_component.h"

static void test_p03_f01_str_trim(void) {
    char s[16] = "  abc  "; assert(p03_f01_str_trim(s) == 3 && strcmp(s, "abc") == 0);
}

static void test_p03_f02_str_split_delim(void) {
    char toks[4][32]; assert(p03_f02_str_split_delim("a,b,c", ',', toks, 4) == 3);
}

static void test_p03_f03_str_join(void) {
    char toks[2][32] = {"foo", "bar"}; char out[32]; assert(p03_f03_str_join(toks, 2, '-', out, 32) == 7 && strcmp(out, "foo-bar") == 0);
}

static void test_p03_f04_str_starts_with(void) {
    assert(p03_f04_str_starts_with("hello world", "hello") == true);
}

static void test_p03_f05_str_ends_with(void) {
    assert(p03_f05_str_ends_with("hello world", "world") == true);
}

static void test_p03_f06_str_replace_char(void) {
    char s[16] = "a_b_c"; assert(p03_f06_str_replace_char(s, '_', '-') == 2 && strcmp(s, "a-b-c") == 0);
}

static void test_p03_f07_str_count_substr(void) {
    assert(p03_f07_str_count_substr("bananana", "na") == 3);
}

static void test_p03_f08_str_to_lower_ascii(void) {
    char s[16] = "HeLLo"; p03_f08_str_to_lower_ascii(s); assert(strcmp(s, "hello") == 0);
}

static void test_p03_f09_str_to_upper_ascii(void) {
    char s[16] = "hello"; p03_f09_str_to_upper_ascii(s); assert(strcmp(s, "HELLO") == 0);
}

static void test_p03_f10_str_reverse(void) {
    char s[16] = "12345"; p03_f10_str_reverse(s); assert(strcmp(s, "54321") == 0);
}

static void test_p03_f15_str_format_hex(void) {
    uint8_t raw[2] = {0x12, 0xAB}; char buf[8]; assert(p03_f15_str_format_hex(raw, 2, buf, 8) == 4);
}

static void test_p03_f16_str_parse_hex(void) {
    uint8_t out[2]; assert(p03_f16_str_parse_hex("12ab", out, 2) == 2);
}

static void test_p07_f06_slab_mark_busy(void) {
    p07_slab_t s; memset(&s, 0, sizeof(s)); s.total_blocks = P07_NUM_BLOCKS; s.free_count = P07_NUM_BLOCKS; p07_f06_slab_mark_busy(&s, 0); assert((s.alloc_bitmap & 1) == 1);
}

static void test_p07_f08_slab_find_first_free_bit(void) {
    assert(p07_f08_slab_find_first_free_bit(0) == 0);
}

static void test_p07_f09_slab_defragment_check(void) {
    p07_slab_t s; memset(&s, 0, sizeof(s)); s.total_blocks = P07_NUM_BLOCKS; s.free_count = P07_NUM_BLOCKS; s.alloc_bitmap = 0x55; assert(p07_f09_slab_defragment_check(&s) > 0);
}

static void test_p07_f12_slab_index_to_pointer(void) {
    p07_slab_t s; memset(&s, 0, sizeof(s)); s.total_blocks = P07_NUM_BLOCKS; s.free_count = P07_NUM_BLOCKS; assert(p07_f12_slab_index_to_pointer(&s, 0) == (void*)s.memory_pool);
}

int main(void) {
    test_p03_f01_str_trim();
    test_p03_f02_str_split_delim();
    test_p03_f03_str_join();
    test_p03_f04_str_starts_with();
    test_p03_f05_str_ends_with();
    test_p03_f06_str_replace_char();
    test_p03_f07_str_count_substr();
    test_p03_f08_str_to_lower_ascii();
    test_p03_f09_str_to_upper_ascii();
    test_p03_f10_str_reverse();
    test_p03_f15_str_format_hex();
    test_p03_f16_str_parse_hex();
    test_p07_f06_slab_mark_busy();
    test_p07_f08_slab_find_first_free_bit();
    test_p07_f09_slab_defragment_check();
    test_p07_f12_slab_index_to_pointer();
    printf("PASS: test_p03_reuse075 passed.\n");
    return 0;
}
