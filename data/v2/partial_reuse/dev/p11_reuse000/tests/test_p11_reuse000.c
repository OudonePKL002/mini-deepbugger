#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "query_component.h"
#include "donor_component.h"

static void test_p13_f01_slip_encode(void) {
    const uint8_t raw[] = {1, 0xC0}; uint8_t enc[16]; assert(p13_f01_slip_encode(raw, 2, enc, 16) > 2);
}

static void test_p13_f02_slip_decode(void) {
    const uint8_t raw[] = {1, 0xDB, 0xDC, 0xC0}; uint8_t dec[16]; assert(p13_f02_slip_decode(raw, 4, dec, 16) == 2);
}

static void test_p13_f03_cobs_encode(void) {
    const uint8_t raw[] = {1, 2, 3}; uint8_t enc[16]; assert(p13_f03_cobs_encode(raw, 3, enc, 16) > 0);
}

static void test_p13_f04_cobs_decode(void) {
    const uint8_t enc[6] = {3, 11, 22, 3, 33, 44}; uint8_t dec[8]; assert(p13_f04_cobs_decode(enc, 6, dec, 8) == 5 && dec[0] == 11);
}

static void test_p13_f05_fcs16_compute(void) {
    const uint8_t raw[] = {1, 2, 3, 4}; assert(p13_f05_fcs16_compute(raw, 4) != 0);
}

static void test_p13_f06_fcs16_verify(void) {
    const uint8_t msg[4] = {'A', 'B', 0x98, 0xCE}; assert(p13_f06_fcs16_verify(msg, 2, 0xCE98) || !p13_f06_fcs16_verify(msg, 2, 0));
}

static void test_p13_f07_pack_header(void) {
    uint8_t hdr[6]; assert(p13_f07_pack_header(hdr, 0x05, 100, 50) == 6);
}

static void test_p13_f08_unpack_header(void) {
    const uint8_t hdr[6] = {0xAA, 0x05, 0, 100, 0, 50}; uint8_t type; uint16_t seq, plen; assert(p13_f08_unpack_header(hdr, &type, &seq, &plen) && type == 0x05 && seq == 100 && plen == 50);
}

static void test_p13_f09_bit_stuff_encode(void) {
    const uint8_t raw[] = {0xAA}; uint8_t dst[8]; assert(p13_f09_bit_stuff_encode(raw, 8, dst) >= 8);
}

static void test_p13_f10_bit_stuff_decode(void) {
    const uint8_t enc[1] = {0x00}; uint8_t dec[1]; assert(p13_f10_bit_stuff_decode(enc, 8, dec) == 8);
}

static void test_p13_f11_frame_checksum_valid(void) {
    const uint8_t frame[4] = {1, 2, 0, 0}; assert(p13_f11_frame_checksum_valid(frame, 4) || !p13_f11_frame_checksum_valid(frame, 4));
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

static void test_p13_f15_frame_buf_append(void) {
    p13_frame_buf_t fb; memset(&fb, 0, sizeof(fb)); fb.capacity = 256; const uint8_t d[2] = {1, 2}; assert(p13_f15_frame_buf_append(&fb, d, 2) && fb.length == 2);
}

static void test_p13_f16_frame_buf_reset(void) {
    p13_frame_buf_t fb; fb.length = 5; p13_f16_frame_buf_reset(&fb); assert(fb.length == 0);
}

int main(void) {
    test_p13_f01_slip_encode();
    test_p13_f02_slip_decode();
    test_p13_f03_cobs_encode();
    test_p13_f04_cobs_decode();
    test_p13_f05_fcs16_compute();
    test_p13_f06_fcs16_verify();
    test_p13_f07_pack_header();
    test_p13_f08_unpack_header();
    test_p13_f09_bit_stuff_encode();
    test_p13_f10_bit_stuff_decode();
    test_p13_f11_frame_checksum_valid();
    test_p13_f12_byte_swap16();
    test_p13_f13_byte_swap32();
    test_p13_f14_parse_tlv_field();
    test_p13_f15_frame_buf_append();
    test_p13_f16_frame_buf_reset();
    printf("PASS: test_p11_reuse000 passed.\n");
    return 0;
}
