#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "query_component.h"
#include "donor_component.h"

static void test_p13_f02_slip_decode(void) {
    const uint8_t raw[] = {1, 0xDB, 0xDC, 0xC0}; uint8_t dec[16]; assert(p13_f02_slip_decode(raw, 4, dec, 16) == 2);
}

static void test_p13_f12_byte_swap16(void) {
    assert(p13_f12_byte_swap16(0x1234) == 0x3412);
}

static void test_p13_f13_byte_swap32(void) {
    assert(p13_f13_byte_swap32(0x12345678) == 0x78563412);
}

static void test_p13_f14_parse_tlv_field(void) {
    const uint8_t b[4] = {1, 2, 0xAA, 0xBB}; uint8_t tag; uint8_t v_len; const uint8_t *val; assert(p13_f14_parse_tlv_field(b, 4, &tag, &v_len, &val) && tag == 1 && v_len == 2);
}

static void test_p08_f01_adler32(void) {
    const uint8_t s[] = "DevBenchmarkCorpus2026"; assert(p08_f01_adler32(s, sizeof(s)-1) != 0);
}

static void test_p08_f02_siphash_round(void) {
    uint64_t v[4] = {1, 2, 3, 4}; p08_f02_siphash_round(v); assert(v[0] != 1);
}

static void test_p08_f03_siphash24(void) {
    const uint8_t s[] = "DevBenchmarkCorpus2026"; uint8_t k[16] = {0}; assert(p08_f03_siphash24(s, sizeof(s)-1, k) != 0);
}

static void test_p08_f04_half_siphash(void) {
    const uint8_t s[] = "DevBenchmarkCorpus2026"; uint8_t k[8] = {1,2,3,4,5,6,7,8}; assert(p08_f04_half_siphash(s, sizeof(s)-1, k) != 0);
}

static void test_p08_f06_sdbm_hash(void) {
    assert(p08_f06_sdbm_hash("test_string") != 0);
}

static void test_p08_f08_jenkins_lookup2(void) {
    const uint8_t s[] = "DevBenchmarkCorpus2026"; assert(p08_f08_jenkins_lookup2(s, sizeof(s)-1, 42) != 0);
}

static void test_p08_f09_knuth_multiplicative(void) {
    assert(p08_f09_knuth_multiplicative(12345) != 0);
}

static void test_p08_f10_rot13_cipher(void) {
    char b[16]; p08_f10_rot13_cipher(b, "Hello", 5); b[5] = 0; assert(strcmp(b, "Uryyb") == 0);
}

static void test_p08_f13_poly1305_clamp(void) {
    uint8_t k[16] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff}; p08_f13_poly1305_clamp(k); assert((k[3] & 15) == k[3]);
}

static void test_p08_f14_chash_simple(void) {
    const uint8_t s[] = "DevBenchmarkCorpus2026"; assert(p08_f14_chash_simple(s, sizeof(s)-1, 999) != 0);
}

static void test_p08_f15_hash_mix32(void) {
    assert(p08_f15_hash_mix32(12345) != 0);
}

static void test_p08_f16_checksum_fold64_to_32(void) {
    assert(p08_f16_checksum_fold64_to_32(0x123456789ABCDEF0ULL) != 0);
}

int main(void) {
    test_p13_f02_slip_decode();
    test_p13_f12_byte_swap16();
    test_p13_f13_byte_swap32();
    test_p13_f14_parse_tlv_field();
    test_p08_f01_adler32();
    test_p08_f02_siphash_round();
    test_p08_f03_siphash24();
    test_p08_f04_half_siphash();
    test_p08_f06_sdbm_hash();
    test_p08_f08_jenkins_lookup2();
    test_p08_f09_knuth_multiplicative();
    test_p08_f10_rot13_cipher();
    test_p08_f13_poly1305_clamp();
    test_p08_f14_chash_simple();
    test_p08_f15_hash_mix32();
    test_p08_f16_checksum_fold64_to_32();
    printf("PASS: test_p13_reuse025 passed.\n");
    return 0;
}
