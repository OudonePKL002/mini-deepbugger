#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "query_component.h"
#include "donor_component.h"

static void test_p06_f01_tlv_encode_uint8(void) {
    uint8_t buf[16]; assert(p06_f01_tlv_encode_uint8(1, 0xFE, buf, 16) == 3);
}

static void test_p06_f02_tlv_encode_uint16(void) {
    uint8_t buf[16]; assert(p06_f02_tlv_encode_uint16(2, 0x1234, buf, 16) == 4);
}

static void test_p06_f03_tlv_encode_uint32(void) {
    uint8_t buf[16]; assert(p06_f03_tlv_encode_uint32(3, 0x12345678, buf, 16) == 6);
}

static void test_p06_f04_tlv_encode_bytes(void) {
    uint8_t raw[2] = {1, 2}; uint8_t buf[16]; assert(p06_f04_tlv_encode_bytes(4, raw, 2, buf, 16) == 4);
}

static void test_p06_f05_tlv_decode_header(void) {
    uint8_t buf[4] = {1, 2, 0xAA, 0xBB}; uint8_t tag, len; assert(p06_f05_tlv_decode_header(buf, 4, &tag, &len) && tag == 1 && len == 2);
}

static void test_p06_f06_tlv_decode_uint8(void) {
    uint8_t buf[3] = {1, 1, 0x55}; uint8_t val; assert(p06_f06_tlv_decode_uint8(buf, 3, &val) && val == 0x55);
}

static void test_p06_f07_tlv_decode_uint16(void) {
    uint8_t buf[4] = {2, 2, 0x12, 0x34}; uint16_t val; assert(p06_f07_tlv_decode_uint16(buf, 4, &val) && val == 0x1234);
}

static void test_p06_f08_tlv_decode_uint32(void) {
    uint8_t buf[6] = {3, 4, 0x12, 0x34, 0x56, 0x78}; uint32_t val; assert(p06_f08_tlv_decode_uint32(buf, 6, &val) && val == 0x12345678);
}

static void test_p06_f09_tlv_find_tag(void) {
    uint8_t buf[6] = {1, 1, 0xAA, 2, 1, 0xBB}; assert(p06_f09_tlv_find_tag(buf, 6, 2) == 3);
}

static void test_p06_f10_tlv_count_tags(void) {
    uint8_t buf[6] = {1, 1, 0xAA, 2, 1, 0xBB}; assert(p06_f10_tlv_count_tags(buf, 6) == 2);
}

static void test_p06_f11_tlv_validate_buffer(void) {
    uint8_t buf[3] = {1, 1, 0xAA}; assert(p06_f11_tlv_validate_buffer(buf, 3) == true);
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

static void test_p06_f16_packet_serialize(void) {
    uint8_t wire[64]; assert(p06_f16_packet_serialize(42, (const uint8_t*)"hello", 5, wire, 64) == 12);
}

int main(void) {
    test_p06_f01_tlv_encode_uint8();
    test_p06_f02_tlv_encode_uint16();
    test_p06_f03_tlv_encode_uint32();
    test_p06_f04_tlv_encode_bytes();
    test_p06_f05_tlv_decode_header();
    test_p06_f06_tlv_decode_uint8();
    test_p06_f07_tlv_decode_uint16();
    test_p06_f08_tlv_decode_uint32();
    test_p06_f09_tlv_find_tag();
    test_p06_f10_tlv_count_tags();
    test_p06_f11_tlv_validate_buffer();
    test_p06_f12_packet_header_build();
    test_p06_f13_packet_header_parse();
    test_p06_f14_packet_payload_pad();
    test_p06_f15_packet_payload_unpad();
    test_p06_f16_packet_serialize();
    printf("PASS: test_p02_reuse000 passed.\n");
    return 0;
}
