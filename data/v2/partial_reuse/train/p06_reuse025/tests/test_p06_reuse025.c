#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "query_component.h"
#include "donor_component.h"

static void test_p06_f02_tlv_encode_uint16(void) {
    uint8_t buf[16]; assert(p06_f02_tlv_encode_uint16(2, 0x1234, buf, 16) == 4);
}

static void test_p06_f03_tlv_encode_uint32(void) {
    uint8_t buf[16]; assert(p06_f03_tlv_encode_uint32(3, 0x12345678, buf, 16) == 6);
}

static void test_p06_f05_tlv_decode_header(void) {
    uint8_t buf[4] = {1, 2, 0xAA, 0xBB}; uint8_t tag, len; assert(p06_f05_tlv_decode_header(buf, 4, &tag, &len) && tag == 1 && len == 2);
}

static void test_p06_f08_tlv_decode_uint32(void) {
    uint8_t buf[6] = {3, 4, 0x12, 0x34, 0x56, 0x78}; uint32_t val; assert(p06_f08_tlv_decode_uint32(buf, 6, &val) && val == 0x12345678);
}

static void test_p03_f01_str_trim(void) {
    char s[16] = "  abc  "; assert(p03_f01_str_trim(s) == 3 && strcmp(s, "abc") == 0);
}

static void test_p03_f02_str_split_delim(void) {
    char toks[4][32]; assert(p03_f02_str_split_delim("a,b,c", ',', toks, 4) == 3);
}

static void test_p03_f03_str_join(void) {
    char toks[2][32] = {"foo", "bar"}; char out[32]; assert(p03_f03_str_join(toks, 2, '-', out, 32) == 7 && strcmp(out, "foo-bar") == 0);
}

static void test_p03_f06_str_replace_char(void) {
    char s[16] = "a_b_c"; assert(p03_f06_str_replace_char(s, '_', '-') == 2 && strcmp(s, "a-b-c") == 0);
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

static void test_p03_f11_str_levenshtein(void) {
    assert(p03_f11_str_levenshtein("kitten", "sitting") == 3);
}

static void test_p03_f12_str_escape_c(void) {
    char esc[16]; assert(p03_f12_str_escape_c("a\nb", esc, 16) > 0);
}

static void test_p03_f13_str_unescape_c(void) {
    char unesc[16]; assert(p03_f13_str_unescape_c("a\\nb", unesc, 16) > 0);
}

static void test_p03_f14_str_parse_int(void) {
    int32_t val = 0; assert(p03_f14_str_parse_int("-123", &val) == true && val == -123);
}

static void test_p03_f15_str_format_hex(void) {
    uint8_t raw[2] = {0x12, 0xAB}; char buf[8]; assert(p03_f15_str_format_hex(raw, 2, buf, 8) == 4);
}

int main(void) {
    test_p06_f02_tlv_encode_uint16();
    test_p06_f03_tlv_encode_uint32();
    test_p06_f05_tlv_decode_header();
    test_p06_f08_tlv_decode_uint32();
    test_p03_f01_str_trim();
    test_p03_f02_str_split_delim();
    test_p03_f03_str_join();
    test_p03_f06_str_replace_char();
    test_p03_f08_str_to_lower_ascii();
    test_p03_f09_str_to_upper_ascii();
    test_p03_f10_str_reverse();
    test_p03_f11_str_levenshtein();
    test_p03_f12_str_escape_c();
    test_p03_f13_str_unescape_c();
    test_p03_f14_str_parse_int();
    test_p03_f15_str_format_hex();
    printf("PASS: test_p06_reuse025 passed.\n");
    return 0;
}
