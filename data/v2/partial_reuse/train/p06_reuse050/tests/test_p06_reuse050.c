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

static void test_p06_f06_tlv_decode_uint8(void) {
    uint8_t buf[3] = {1, 1, 0x55}; uint8_t val; assert(p06_f06_tlv_decode_uint8(buf, 3, &val) && val == 0x55);
}

static void test_p06_f12_packet_header_build(void) {
    uint8_t hdr[8]; assert(p06_f12_packet_header_build(101, 20, hdr, 8) == 6 && hdr[0] == 0xAA && hdr[1] == 0x55);
}

static void test_p06_f13_packet_header_parse(void) {
    const uint8_t hdr[6] = {0xAA, 0x55, 0, 101, 0, 20}; uint16_t seq, plen; assert(p06_f13_packet_header_parse(hdr, 6, &seq, &plen) && seq == 101 && plen == 20);
}

static void test_p06_f14_packet_payload_pad(void) {
    uint8_t buf[16] = {'a', 'b'}; assert(p06_f14_packet_payload_pad(buf, 2, 4, 16) == 4);
}

static void test_p06_f15_packet_payload_unpad(void) {
    const uint8_t buf[4] = {'a', 'b', 2, 2}; assert(p06_f15_packet_payload_unpad(buf, 4) == 2);
}

static void test_p03_f01_str_trim(void) {
    char s[16] = "  abc  "; assert(p03_f01_str_trim(s) == 3 && strcmp(s, "abc") == 0);
}

static void test_p03_f03_str_join(void) {
    char toks[2][32] = {"foo", "bar"}; char out[32]; assert(p03_f03_str_join(toks, 2, '-', out, 32) == 7 && strcmp(out, "foo-bar") == 0);
}

static void test_p03_f06_str_replace_char(void) {
    char s[16] = "a_b_c"; assert(p03_f06_str_replace_char(s, '_', '-') == 2 && strcmp(s, "a-b-c") == 0);
}

static void test_p03_f07_str_count_substr(void) {
    assert(p03_f07_str_count_substr("bananana", "na") == 3);
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

static void test_p03_f14_str_parse_int(void) {
    int32_t val = 0; assert(p03_f14_str_parse_int("-123", &val) == true && val == -123);
}

int main(void) {
    test_p06_f02_tlv_encode_uint16();
    test_p06_f03_tlv_encode_uint32();
    test_p06_f05_tlv_decode_header();
    test_p06_f06_tlv_decode_uint8();
    test_p06_f12_packet_header_build();
    test_p06_f13_packet_header_parse();
    test_p06_f14_packet_payload_pad();
    test_p06_f15_packet_payload_unpad();
    test_p03_f01_str_trim();
    test_p03_f03_str_join();
    test_p03_f06_str_replace_char();
    test_p03_f07_str_count_substr();
    test_p03_f09_str_to_upper_ascii();
    test_p03_f10_str_reverse();
    test_p03_f11_str_levenshtein();
    test_p03_f14_str_parse_int();
    printf("PASS: test_p06_reuse050 passed.\n");
    return 0;
}
